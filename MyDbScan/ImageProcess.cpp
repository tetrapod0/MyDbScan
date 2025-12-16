#include "pch.h"
#include "ImageProcess.h"


ImageProcess::ImageProcess ( const cv::Vec3i& circle , int iEps , int iMinSamples , int iHsvSThresh , int iHsvVThresh )
{
	m_circle = circle;
	m_iEps = iEps;
	m_iMinSamples = iMinSamples;
	m_iHsvSThresh = iHsvSThresh;
	m_iHsvVThresh = iHsvVThresh;
}


void ImageProcess::process ( const cv::Mat& matRaw , cv::Mat& matResult , cv::Mat& matScatter )
{
	// 마스크 생성
	cv::Mat matMask = cv::Mat::zeros ( matRaw.size ( ) , CV_8UC1 );
	cv::circle ( matMask , cv::Point ( m_circle[ 0 ] , m_circle[ 1 ] ) , m_circle[ 2 ] , cv::Scalar ( 255 ) , -1 );

	// 축소 마스크 생성
	cv::Mat matMaskResized;
	cv::resize ( matMask , matMaskResized , cv::Size ( 100 , 100 ) , 0 , 0 , cv::INTER_NEAREST );

	// 축소 이미지 생성 (100x100) // INTER_NEAREST
	cv::Mat matRawResized;
	cv::resize ( matRaw , matRawResized , cv::Size ( 100 , 100 ) );

	// HSV 변환
	cv::Mat matHsv;
	cv::cvtColor ( matRawResized , matHsv , cv::COLOR_BGR2HSV );

	// 마스크 부분 픽셀 좌표 추출
	std::vector<std::pair<int , int>> vecPos; // (x, y)
	for ( int y = 0; y < matHsv.rows; ++y )
	{
		for ( int x = 0; x < matHsv.cols; ++x )
		{
			// 마스크 픽셀이 0 이면 건너뜀
			if ( matMaskResized.at<uchar> ( y , x ) == 0 )
				continue;

			vecPos.push_back ( std::make_pair ( x , y ) );
		}
	}

	// 데이터 행렬 생성
	cv::Mat matData ( static_cast< int >( vecPos.size ( ) ) , 2 , CV_64F ); // (N, 2) , N: 마스크 픽셀 수
	for ( int i = 0; i < vecPos.size(); ++i )
	{
		const auto& [x , y] = vecPos[ i ];

		// HSV 값 추출
		cv::Vec3b hsv = matHsv.at<cv::Vec3b> ( y , x );
		int s = static_cast< int >( hsv[ 1 ] );
		int v = static_cast< int >( hsv[ 2 ] );

		// 데이터 행렬에 삽입
		matData.at<double> ( i , 0 ) = static_cast< double > ( s );
		matData.at<double> ( i , 1 ) = static_cast< double > ( v );
	}

	// DBSCAN 수행
	std::vector<int> vecLabels;
	Dbscan dbscan ( static_cast< double >( m_iEps ) , m_iMinSamples );
	dbscan.fit ( matData , vecLabels );

	// 유니크 라벨 추출
	std::set<int> setLabelsUnique;
	for ( int label : vecLabels )
		if ( label != -1 ) // 노이즈 제외
			setLabelsUnique.insert ( label );
	std::vector<int> vecLabelsUnique ( setLabelsUnique.begin ( ) , setLabelsUnique.end ( ) );


	// 각 클러스터별 중간값 색상 계산
	std::vector<std::pair<uchar , uchar>> vecSVMedian;
	for ( const int label : vecLabelsUnique )
	{
		std::vector<uchar> vecS;
		std::vector<uchar> vecV;
		for ( size_t i = 0; i < vecLabels.size ( ); ++i )
		{
			if ( vecLabels[ i ] == label )
			{
				const auto& [x , y] = vecPos[ i ];
				//std::cout << "Label: " << label << ", Pos: (" << x << ", " << y << ")\n";
				cv::Vec3b hsv = matHsv.at<cv::Vec3b> ( y , x );
				vecS.push_back ( hsv[ 1 ] );
				vecV.push_back ( hsv[ 2 ] );
			}
		}
		// 중간값 계산
		std::sort ( vecS.begin ( ) , vecS.end ( ) );
		std::sort ( vecV.begin ( ) , vecV.end ( ) );
		uchar sMedian = vecS[ vecS.size ( ) / 2 ];
		uchar vMedian = vecV[ vecV.size ( ) / 2 ];
		vecSVMedian.push_back ( std::make_pair ( sMedian , vMedian ) );
	}

#if 0
	// 각 클러스터별 평균 색상 계산
	std::vector<cv::Scalar> vecColorsMean;
	cv::Mat matClustered ( matRawResized.size ( ) , CV_8UC1 );
	for ( int label : vecLabelsUnique )
	{
		matClustered.setTo ( 0 );
		for ( size_t i = 0; i < vecLabels.size ( ); ++i )
		{
			const auto& [x , y] = vecPos[ i ];
			if ( vecLabels[ i ] == label )
				matClustered.at<uchar> ( y , x ) = 255;
		}

		cv::Scalar colorMean = cv::mean ( matRawResized , matClustered );
		vecColorsMean.push_back ( colorMean );

		//cv::Scalar colorSum ( 0 , 0 , 0 );
		//int count = 0;
		//for ( size_t i = 0; i < vecLabels.size ( ); ++i )
		//{
		//	if ( vecLabels[ i ] == label )
		//	{
		//		int x = vecPos[ i ].first;
		//		int y = vecPos[ i ].second;
		//		cv::Vec3b bgr = matRawResized.at<cv::Vec3b> ( y , x );
		//		colorSum += cv::Scalar ( bgr[ 0 ] , bgr[ 1 ] , bgr[ 2 ] );
		//		++count;
		//	}
		//}
		//if ( count > 0 )
		//{
		//	// int 형 반올림
		//	vecColorsMean.push_back ( cv::Scalar (
		//		static_cast< int >( std::round ( colorSum[ 0 ] / count ) ) ,
		//		static_cast< int >( std::round ( colorSum[ 1 ] / count ) ) ,
		//		static_cast< int >( std::round ( colorSum[ 2 ] / count ) )
		//	) );
		//}
	}
#endif

	// 결과 마스크 생성
	cv::Mat matResultMask = cv::Mat::zeros ( matRawResized.size ( ) , CV_8UC1 );
	std::vector<bool> vecIsClusterUsed ( vecSVMedian.size ( ) , false );
	for ( size_t i = 0; i < vecSVMedian.size ( ); ++i )
	{
		const auto& [sMedian , vMedian] = vecSVMedian[ i ];

		// 임계값 비교 // 거품이 아님
		if ( sMedian > m_iHsvSThresh || vMedian < m_iHsvVThresh )
			continue;

		vecIsClusterUsed[ i ] = true;
	}

	for ( size_t j = 0; j < vecLabels.size ( ); ++j )
	{
		int label = vecLabels[ j ];
		if ( label == -1 || !vecIsClusterUsed[ label ] )
			continue;

		const auto& [x , y] = vecPos[ j ];
		matResultMask.at<uchar> ( y , x ) = 255;
	}

	// 결과 마스크 모폴로지 연산 // 노이즈 제거
	cv::Mat matKernel = cv::getStructuringElement ( cv::MORPH_ELLIPSE , cv::Size ( 3 , 3 ) );
	cv::dilate ( matResultMask , matResultMask , matKernel , cv::Point ( -1 , -1 ) , 1 );
	cv::erode ( matResultMask , matResultMask , matKernel , cv::Point ( -1 , -1 ) , 1 );
	cv::bitwise_and ( matResultMask , matMaskResized , matResultMask );

	// 결과 마스크 원래 크기로 확대
	cv::Mat matResultMaskResized;
	cv::resize ( matResultMask , matResultMaskResized , matRaw.size ( ) , 0 , 0 , cv::INTER_NEAREST );

	// 결과 이미지 생성 // 마스크 부분 녹색
	matRaw.copyTo ( matResult );
	matResult.setTo ( cv::Scalar ( 0 , 255 , 0 ) , matResultMaskResized );

	// 결과 산점도 이미지 생성
	matScatter.create ( 256 , 256 , CV_8UC3 );
	matScatter.setTo ( cv::Scalar ( 255 , 255 , 255 ) );
	cv::cvtColor ( matScatter , matScatter , cv::COLOR_BGR2HSV );


	int iIntervalDegree = static_cast< int >( 180 / vecLabelsUnique.size ( ) );

	for ( int i = 0; i < vecLabels.size ( ); ++i )
	{
		int iS = static_cast< int > ( matData.at<double> ( i , 0 ) );
		int iV = static_cast< int > ( matData.at<double> ( i , 1 ) );
		int label = vecLabels[ i ];

		if ( label == -1 )
		{
			matScatter.at<cv::Vec3b> ( 255 - iV , iS ) = cv::Vec3b ( 0 , 0 , 0 ); // 검정 (노이즈)
		}
		else
		{
			matScatter.at<cv::Vec3b> ( 255 - iV , iS ) = cv::Vec3b ( iIntervalDegree * label , 255 , 255 );
		}
	}
	cv::cvtColor ( matScatter , matScatter , cv::COLOR_HSV2BGR );

	// vecSVMedian 출력
	std::cout << "Cluster SV Medians:\n";
	for ( size_t i = 0; i < vecSVMedian.size ( ); ++i )
	{
		const auto& [sMedian , vMedian] = vecSVMedian[ i ];
		std::cout << "Cluster " << i << ": S Median = " << static_cast< int >( sMedian )
			<< ", V Median = " << static_cast< int >( vMedian ) << "\n";
	}
}





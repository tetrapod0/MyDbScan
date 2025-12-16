#include "pch.h"
#include "DBSCAN.h"


// matData  : (N, dim)
// vecLabels : (N, )
void Dbscan::fit ( const cv::Mat& matData , std::vector<int>& vecLabels )
{
	CV_Assert ( matData.type ( ) == CV_32F || matData.type ( ) == CV_64F );

	// 라벨 초기화
	vecLabels.resize ( matData.rows , -1 ); // -1: noise

	int iNumPoints = matData.rows;
	int iDim = matData.cols;


	std::vector<std::unordered_map<int , int>> vecMapSortedIdxToOriginIdx ( iDim ); // vec[dim] -> map[sorted_idx] -> origin_idx

	std::vector<std::vector<double>> vecVecData ( iDim ); // vec[dim] -> vec[sorted_idx] -> value

	std::vector<std::pair<int , double>> vecPairIdxValueTemp ( iNumPoints ); // vec[sorted_idx] -> pair<origin_idx, value>
	
	for (int i = 0; i < iDim; ++i)
	{
		// 정렬용 임시 벡터 생성
		for ( int j = 0; j < iNumPoints; ++j )
		{
			vecPairIdxValueTemp[ j ] = std::make_pair ( j , matData.at<double> ( j , i ) );
		}

		// 정렬
		std::sort ( vecPairIdxValueTemp.begin ( ) , vecPairIdxValueTemp.end ( ) ,
			[ ] ( const std::pair<int , double>& a , const std::pair<int , double>& b )
			{ return a.second < b.second; }
		);

		// 맵 및 데이터 벡터 생성
		vecVecData[ i ].resize ( iNumPoints );
		for ( int j = 0; j < iNumPoints; ++j )
		{
			vecMapSortedIdxToOriginIdx[ i ][ j ] = vecPairIdxValueTemp[ j ].first;
			vecVecData[ i ][ j ] = vecPairIdxValueTemp[ j ].second;
		}
	}



	int iClusterId = 0;
	int iNextIdx = 0;
	std::queue<int> queueIdx;

	while ( iNextIdx < iNumPoints )
	{
		// 이미 라벨링된 포인트는 건너뜀
		if ( vecLabels[ iNextIdx ] != -1 )
		{
			++iNextIdx;
			continue;
		}

		// 클러스터 집합 크기
		//int iClusterSize = 0;
		std::set<int> setClusterPts;

		// 새로운 클러스터 시작
		queueIdx.push ( iNextIdx );

		// BFS로 클러스터 확장
		while ( queueIdx.empty ( ) == false )
		{
			// 현재 포인트 인덱스
			int iCurrentIdx = queueIdx.front ( );
			queueIdx.pop ( );

			// 현재 포인트의 이웃 찾기
			std::set<int> setCandidate; // 후보 이웃 인덱스 교집합
			for ( int dim = 0; dim < iDim; ++dim )
			{
				// 각 차원별로 이웃 인덱스 찾기
				std::set<int> setCandidateTemp;

				// 현재 포인트의 값
				double dValue = matData.at<double> ( iCurrentIdx , dim );

				// 현재 차원에서 이웃 범위 계산
				double dLowerBound = dValue - m_eps;
				double dUpperBound = dValue + m_eps;

				// 정렬된 데이터에서 이웃 인덱스 찾기
				auto& vecData = vecVecData[ dim ];
				auto itLower = std::lower_bound ( vecData.begin ( ) , vecData.end ( ) , dLowerBound );
				auto itUpper = std::upper_bound ( vecData.begin ( ) , vecData.end ( ) , dUpperBound );
				for ( auto it = itLower; it != itUpper; ++it )
				{
					int idx = static_cast< int >( std::distance ( vecData.begin ( ) , it ) );
					int originIdx = vecMapSortedIdxToOriginIdx[ dim ][ idx ];
					setCandidateTemp.insert ( originIdx );
				}

				// 교집합 갱신
				if ( dim == 0 )
				{
					setCandidate = setCandidateTemp;
				}
				else
				{
					std::set<int> setIntersection;
					std::set_intersection ( setCandidate.begin ( ) , setCandidate.end ( ) ,
						setCandidateTemp.begin ( ) , setCandidateTemp.end ( ) ,
						std::inserter ( setIntersection , setIntersection.begin ( ) )
					);
					setCandidate = setIntersection;
				}
			}

			// 이웃 후보 실제 거리 계산 및 필터링
			std::vector<int> vecNeighbors;
			for ( int idx : setCandidate )
			{
				// 현재 포인트와 후보 포인트
				cv::Mat matPtCurrent = matData.row ( iCurrentIdx );
				cv::Mat matPtCandidate = matData.row ( idx );

				// 실수형 변환
				//matPtCurrent.convertTo ( matPtCurrent , CV_64F );
				//matPtCandidate.convertTo ( matPtCandidate , CV_64F );
				//CV_Assert ( matPtCurrent.type ( ) == CV_32F || matPtCurrent.type ( ) == CV_64F );
				//CV_Assert ( matPtCandidate.type ( ) == CV_32F || matPtCandidate.type ( ) == CV_64F );

				// 거리 계산
				double dDist = cv::norm ( matPtCurrent - matPtCandidate , cv::NORM_L2 );
				if ( dDist <= m_eps )
					vecNeighbors.push_back ( idx );
			}

			// 충분한 이웃이 있는지 확인
			if ( vecNeighbors.size ( ) >= m_minPts )
			{
				//std::cout << "Cluster ID: " << iClusterId << " , CurrentIdx: " << iCurrentIdx << std::endl;
				// 클러스터 라벨링
				for ( int neighborIdx : vecNeighbors )
				{
					setClusterPts.insert ( neighborIdx );

					//std::cout << neighborIdx;

					if ( vecLabels[ neighborIdx ] != -1 )
					{
						//std::cout << " ";
						continue;
					}

					//std::cout << "-";
					vecLabels[ neighborIdx ] = iClusterId;
					//++iClusterSize;
					//if ( neighborIdx > iCurrentIdx )
					//{
					//	queueIdx.push ( neighborIdx ); // 새로운 포인트 추가
					//	//std::cout << "q";
					//}
					queueIdx.push ( neighborIdx ); // 새로운 포인트 추가
					
					//std::cout << " ";
				}
				//std::cout << std::endl;
			}


		} // while ( queueIdx.empty ( ) == false )

		// 다음 클러스터로 이동
		if ( setClusterPts.size ( ) >= m_minPts )
			++iClusterId;


		//std::cout << std::endl;

		// 다음 포인트
		++iNextIdx;

	} // while ( iNextIdx < iNumPoints )



}

#include "DBSCAN.h"
#include "pch.h"


// matxData  : (N, dim)
// vecLabels : (N, )
void DBSCAN::fit ( const Eigen::MatrixXd& matxData , std::vector<int>& vecLabels )
{
	// 라벨 초기화
	vecLabels.resize ( matxData.rows ( ) , -1 ); // -1: noise

	int iNumPoints = static_cast< int >( matxData.rows ( ) );
	int iDim = static_cast< int >( matxData.cols ( ) );


	std::vector<std::map<int , int>> vecMapIdxToVecIdx ( iDim ); // vec[dim] -> map[N] -> N

	std::vector<std::vector<double>> vecVecData ( iDim ); // vec[dim] -> vec[N] -> value

	std::vector<std::pair<int , double>> vecPairIdxValueTemp ( iNumPoints ); // vec[N] -> pair<idx, value>
	
	for (int i = 0; i < iDim; ++i)
	{
		// 정렬용 임시 벡터 생성
		for ( int j = 0; j < iNumPoints; ++j )
		{
			vecPairIdxValueTemp[ j ] = std::make_pair ( j , matxData ( j , i ) );
		}

		// 정렬
		std::sort ( vecPairIdxValueTemp.begin ( ) , vecPairIdxValueTemp.end ( ) ,
			[ ] ( const std::pair<int , double>& a , const std::pair<int , double>& b )
			{ return a.second < b.second; }
		);

		// 맵 및 데이터 벡터 생성
		for ( int j = 0; j < iNumPoints; ++j )
		{
			vecMapIdxToVecIdx[ i ][ vecPairIdxValueTemp[ j ].first ] = j;
			vecVecData[ i ].push_back ( vecPairIdxValueTemp[ j ].second );
		}
	}

	// 접근 예시 : vecMapIdxToVecIdx[dim][0] -> 3 -> vecVecData[dim][3] -> 1.23






}

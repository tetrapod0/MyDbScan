#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include "opencv.hpp"
#include "Eigen/Dense"



class DBSCAN
{
private:
	double m_eps;
	int    m_minPts;

public:
	DBSCAN ( ) = delete;
	DBSCAN ( double eps , int minPts ) : m_eps ( eps ) , m_minPts ( minPts ) { }

	void fit ( const Eigen::MatrixXd& matxData , std::vector<int>& vecLabels ); // (N, 2) -> (N, )
};
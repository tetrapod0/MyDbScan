#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <set>
#include "opencv.hpp"
//#include "Eigen/Dense"


class Dbscan
{
private:
	double m_eps;
	int    m_minPts;

public:
	Dbscan ( ) = delete;
	Dbscan ( double eps , int minPts ) : m_eps ( eps ) , m_minPts ( minPts ) { }

	//void fit ( const Eigen::MatrixXd& matxData , std::vector<int>& vecLabels ); // (N, 2) -> (N, )

	//template <int D>
	//void fit ( const cv::Vec<int , D>& matxData , std::vector<int>& vecLabels ); // (N, 2) -> (N, )

	void fit ( const cv::Mat& matxData , std::vector<int>& vecLabels ); // (N, 2) -> (N, )

};
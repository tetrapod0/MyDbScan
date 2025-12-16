#pragma once

#include <vector>
#include "opencv.hpp"
#include "DBSCAN.h"


class ImageProcess
{
private:
	cv::Vec3i m_circle; // x, y, r
	int m_iEps;
	int m_iMinSamples;
	int m_iHsvSThresh;
	int m_iHsvVThresh;

public:
	ImageProcess ( ) = delete;
	ImageProcess ( const cv::Vec3i& circle , int iEps , int iMinSamples , int iHsvSThresh , int iHsvVThresh );
	~ImageProcess ( ) = default;

	void process ( const cv::Mat& matRaw , cv::Mat& matResult , cv::Mat& matScatter );
};
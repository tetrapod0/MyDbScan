#pragma once

#include <vector>
#include "opencv.hpp"


class ImageProcess
{
private:
	int m_iEps;
	int m_iMinSamples;
	int m_iHsvSThresh;
	int m_iHsvVThresh;

public:
	ImageProcess ( ) = delete;
	ImageProcess ( int iEps , int iMinSamples , int iHsvSThresh , int iHsvVThresh ) :
		m_iEps ( iEps ) , m_iMinSamples ( iMinSamples ) , m_iHsvSThresh ( iHsvSThresh ) , m_iHsvVThresh ( iHsvVThresh ) { }
	~ImageProcess ( ) = default;


};
#pragma once

#include "opencv.hpp"
#include <functional>

// CImageStatic

class CImageStatic : public CStatic
{
	DECLARE_DYNAMIC(CImageStatic)

public:
	CImageStatic();
	virtual ~CImageStatic();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	cv::Mat* m_pImgRaw = nullptr; // 외부 Raw 이미지
	cv::Mat m_imgView; // 사이즈 조정된 View 이미지

	std::function<void ( cv::Mat& )> m_funcDrawInView = nullptr; // View 이미지에 그리기 위한 콜백 함수

	void getImgView ( const cv::Mat& src , cv::Mat& dst , const CRect& clientRect );
	void FillBitmapInfo2 ( BITMAPINFO* bmi , int width , int height , int bpp );
	void Show ( const cv::Mat& img , HDC dc , int xDest , int yDest );

public:
	virtual void bridgeImage ( cv::Mat* pImgRaw ) { m_pImgRaw = pImgRaw; }
	virtual void setDrawInViewFunc ( const std::function<void ( cv::Mat& )>& func ) { m_funcDrawInView = func; }

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd ( CDC* pDC );
};



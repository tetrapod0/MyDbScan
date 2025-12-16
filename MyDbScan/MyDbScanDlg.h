
// MyDbScanDlg.h: 헤더 파일
//

#pragma once

#include "CImageStatic.h"

#include "opencv.hpp"

#include <vector>

#include "DBSCAN.h"
#include "MyUtils/MyCsv.hpp"
#include "ImageProcess.h"


// CMyDbScanDlg 대화 상자
class CMyDbScanDlg : public CDialogEx
{
// 생성입니다.
public:
	CMyDbScanDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYDBSCAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	enum ImageTypeEnum { _RAW , _RESULT , _SCATTER , _LEN_IMAGE_TYPE }; // 이미지 타입
	std::vector<cv::Mat> m_matList = std::vector<cv::Mat> ( ImageTypeEnum::_LEN_IMAGE_TYPE ); // 이미지 리스트
	CImageStatic m_imgStaticShow, m_imgStaticScatter; // 이미지 Static



	void loadImage ( );
	void drawImage ( bool bProcess , BOOL bErase = TRUE );
	void drawCircle ( );



public:
	afx_msg void OnBnClickedMainLoadImgBtn ( );
	afx_msg void OnHScroll ( UINT nSBCode , UINT nPos , CScrollBar* pScrollBar );
	afx_msg void OnBnClickedMainDoBtn ( );
};

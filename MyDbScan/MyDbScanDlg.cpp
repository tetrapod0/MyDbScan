
// MyDbScanDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyDbScan.h"
#include "MyDbScanDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyDbScanDlg 대화 상자



// openCV 로그 레벨 설정
#if 1
#include "opencv2/core/utils/logger.hpp"
struct CvSetSlientLogLevel {
	CvSetSlientLogLevel ( ) {
		cv::utils::logging::setLogLevel ( cv::utils::logging::LOG_LEVEL_SILENT ); // 또는 LOG_LEVEL_ERROR
	}
};
CvSetSlientLogLevel cv_set_slient_log_level; // 전역 객체로 생성하여 main() 전에 실행되도록 함
#endif


// 콘솔창 사용
#if defined(SHOW_CONSOLE_LOG) && SHOW_CONSOLE_LOG == 1

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

// 속성 - 구성 속성 - C/C++ - 명령줄 - 추가옵션 - /utf-8 일 때 사용
// 콘솔을 CP949 -> UTF-8로 변경
#if defined(_MSVC_EXECUTION_CHARACTER_SET) && _MSVC_EXECUTION_CHARACTER_SET == 65001
#include <windows.h>

struct ConsoleUTF8Initializer {
	ConsoleUTF8Initializer ( ) {
		SetConsoleOutputCP ( CP_UTF8 );
		SetConsoleCP ( CP_UTF8 );
	}
};

ConsoleUTF8Initializer console_utf8_init;
#endif

#endif // SHOW_CONSOLE_LOG






CMyDbScanDlg::CMyDbScanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYDBSCAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDbScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyDbScanDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED ( IDC_MAIN_LOAD_IMG_BTN , &CMyDbScanDlg::OnBnClickedMainLoadImgBtn )
	ON_WM_HSCROLL ( )
	ON_BN_CLICKED ( IDC_MAIN_DO_BTN , &CMyDbScanDlg::OnBnClickedMainDoBtn )
END_MESSAGE_MAP()


// CMyDbScanDlg 메시지 처리기

BOOL CMyDbScanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 이미지 Static 컨트롤 서브클래스 설정
	m_imgStaticShow.SubclassDlgItem ( IDC_MAIN_IMG_PC , this );
	m_imgStaticShow.ModifyStyle ( 0 , SS_NOTIFY );
	m_imgStaticScatter.SubclassDlgItem ( IDC_MAIN_SCATTER_PC , this );
	m_imgStaticScatter.ModifyStyle ( 0 , SS_NOTIFY );

	{
		CSliderCtrl* pSlider;

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_CIRCLE_X_SLIDER ) );
		pSlider->SetRange ( 0 , 1000 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 514 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_CIRCLE_Y_SLIDER ) );
		pSlider->SetRange ( 0 , 1000 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 480 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_CIRCLE_R_SLIDER ) );
		pSlider->SetRange ( 0 , 1000 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 392 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_EPS_SLIDER ) );
		pSlider->SetRange ( 0 , 100 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 9 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_MIN_SAMPLES_SLIDER ) );
		pSlider->SetRange ( 0 , 1000 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 50 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_HSV_S_THRESH_SLIDER ) );
		pSlider->SetRange ( 0 , 255 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 80 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_HSV_V_THRESH_SLIDER ) );
		pSlider->SetRange ( 0 , 255 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 68 );

		// 초기값 설정
		this->SetDlgItemInt ( IDC_MAIN_CIRCLE_X_STATIC , 514 );
		this->SetDlgItemInt ( IDC_MAIN_CIRCLE_Y_STATIC , 480 );
		this->SetDlgItemInt ( IDC_MAIN_CIRCLE_R_STATIC , 392 );
		this->SetDlgItemInt ( IDC_MAIN_EPS_STATIC , 9 );
		this->SetDlgItemInt ( IDC_MAIN_MIN_SAMPLES_STATIC , 50 );
		this->SetDlgItemInt ( IDC_MAIN_HSV_S_THRESH_STATIC , 80 );
		this->SetDlgItemInt ( IDC_MAIN_HSV_V_THRESH_STATIC , 68 );
	}



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyDbScanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyDbScanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyDbScanDlg::OnBnClickedMainLoadImgBtn ( )
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	// 이미지 로드
	this->loadImage ( );

	// 원 그리기
	//this->drawImage ( false , TRUE );

	this->drawCircle ( );

}


void CMyDbScanDlg::loadImage ( )
{
	// bmp, png 파일
	CFileDialog fDlg ( TRUE , _T ( "Load Image" ) , NULL ,
		OFN_HIDEREADONLY ,
		_T ( "Image Files (*.bmp;*.png)|*.bmp;*.png||" ) );

	if ( fDlg.DoModal ( ) != IDOK )
	{
		return;
	}

	// 이미지 로드
	CString strFileName = fDlg.GetPathName ( );
	cv::Mat imgTemp = cv::imread ( std::string ( CT2A ( strFileName ) ) );
	if ( imgTemp.empty ( ) )
	{
		this->MessageBox ( _T ( "이미지 로드에 실패했습니다." ) , _T ( "오류" ) , MB_OK | MB_ICONERROR );
		return;
	}

	// 이미지 넣기, 보여주기
	imgTemp.copyTo ( m_matList[ _RAW ] );
}


void CMyDbScanDlg::drawImage ( bool bProcess , BOOL bErase )
{
	// RAW 이미지가 없으면
	if ( m_matList[ _RAW ].empty ( ) )
	{
		m_imgStaticShow.bridgeImage ( nullptr );
	}


	m_imgStaticShow.bridgeImage ( &m_matList[ _RAW ] );


	m_imgStaticShow.Invalidate ( bErase );
}


void CMyDbScanDlg::drawCircle ( )
{
	// RAW 이미지가 없으면
	if ( m_matList[ _RAW ].empty ( ) )
	{
		return;
	}

	// 이미지 복사
	m_matList[ _RAW ].copyTo ( m_matList[ _RESULT ] );

	int iHeight = m_matList[ _RESULT ].rows;
	int iWidth = m_matList[ _RESULT ].cols;

	// 원 그리기
	int iX = static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_CIRCLE_X_STATIC ) ) * iWidth / 1000;
	int iY = static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_CIRCLE_Y_STATIC ) ) * iHeight / 1000;
	int iR = static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_CIRCLE_R_STATIC ) ) * iHeight / 1000;
	
	cv::circle ( m_matList[ _RESULT ] , cv::Point ( iX , iY ) , iR , cv::Scalar ( 0 , 255 , 0 ) , 5 );

	// 이미지 보여주기
	m_imgStaticShow.bridgeImage ( &m_matList[ _RESULT ] );
	m_imgStaticShow.Invalidate ( FALSE );
}


void CMyDbScanDlg::OnHScroll ( UINT nSBCode , UINT nPos , CScrollBar* pScrollBar )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_CIRCLE_X_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_CIRCLE_X_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_CIRCLE_X_STATIC , iValue );
		this->drawCircle ( );
	}
	else if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_CIRCLE_Y_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_CIRCLE_Y_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_CIRCLE_Y_STATIC , iValue );
		this->drawCircle ( );
	}
	else if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_CIRCLE_R_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_CIRCLE_R_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_CIRCLE_R_STATIC , iValue );
		this->drawCircle ( );
	}
	else if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_EPS_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_EPS_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_EPS_STATIC , iValue );
	}
	else if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_MIN_SAMPLES_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_MIN_SAMPLES_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_MIN_SAMPLES_STATIC , iValue );
	}
	else if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_HSV_S_THRESH_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_HSV_S_THRESH_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_HSV_S_THRESH_STATIC , iValue );
	}
	else if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_HSV_V_THRESH_SLIDER )->GetSafeHwnd ( ) )
	{
		int iValue = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_HSV_V_THRESH_SLIDER ) )->GetPos ( );
		this->SetDlgItemInt ( IDC_MAIN_HSV_V_THRESH_STATIC , iValue );
	}

	CDialogEx::OnHScroll ( nSBCode , nPos , pScrollBar );
}


//void CMyDbScanDlg::OnBnClickedMainDoBtn ( )
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//	Dbscan dbscan (
//		static_cast< double >( this->GetDlgItemInt ( IDC_MAIN_EPS_STATIC ) ) ,
//		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_MIN_SAMPLES_STATIC ) )
//	);
//
//	// 데이터 준비
//	cv::Mat matData;
//	std::vector<int> vecLabels;
//
//
//	MyUtils::MyCsv csv;
//	std::vector < std::vector < std::string >> vecData;
//	csv.load ( "./data.csv" , vecData );
//
//
//	matData = cv::Mat ( static_cast< int >( vecData.size ( ) - 1 ) , 2 , CV_64F );
//	for ( size_t i = 1; i < vecData.size ( ); ++i )
//	{
//		matData.at<double> ( static_cast< int >( i - 1 ) , 0 ) = std::stod ( vecData[ i ][ 0 ] );
//		matData.at<double> ( static_cast< int >( i - 1 ) , 1 ) = std::stod ( vecData[ i ][ 1 ] );
//	}
//
//
//
//	// matData 0 ~ 100 사이로 정규화
//	cv::Mat matDataNorm;
//	//cv::normalize ( matData , matDataNorm , 0.0 , 100.0 , cv::NORM_MINMAX );
//
//	matDataNorm.create ( matData.rows , matData.cols , matData.type ( ) );
//
//	for ( int c = 0; c < matData.cols; ++c )
//	{
//		cv::Mat col = matData.col ( c );
//		cv::Mat colNorm = matDataNorm.col ( c );
//		cv::normalize ( col , colNorm , 0.0 , 99.0 , cv::NORM_MINMAX );
//	}
//
//	// matDataNorm 을 int 형으로 변환
//	matDataNorm.convertTo ( matDataNorm , CV_32S );
//
//
//
//
//	// 클러스터링 수행
//	dbscan.fit ( matDataNorm , vecLabels );
//
//
//	m_matList[ _SCATTER ] = cv::Mat ( 100 , 100 , CV_8UC3 , cv::Scalar ( 255 , 255 , 255 ) );
//
//	// 결과 그리기
//	for ( int i = 0; i < matDataNorm.rows; ++i )
//	{
//		int x = matDataNorm.at<int> ( i , 0 );
//		int y = matDataNorm.at<int> ( i , 1 );
//		int label = vecLabels[ i ];
//
//		cv::Scalar color;
//		if ( label == 0 )
//			color = cv::Scalar ( 255 , 0 , 0 ); // 클러스터 0 : 파란색
//		else if ( label == 1 )
//			color = cv::Scalar ( 0 , 255 , 0 ); // 클러스터 1 : 초록색
//		else if ( label == 2 )
//			color = cv::Scalar ( 0 , 0 , 255 ); // 클러스터 2 : 빨간색
//		else
//			color = cv::Scalar ( 0 , 0 , 0 );   // 노이즈 : 검은색
//
//		// 점 그리기
//		cv::circle ( m_matList[ _SCATTER ] , cv::Point ( x , y ) , 0 , color , -1 );
//	}
//
//	m_imgStaticScatter.bridgeImage ( &m_matList[ _SCATTER ] );
//	m_imgStaticScatter.Invalidate ( TRUE );
//}



void CMyDbScanDlg::OnBnClickedMainDoBtn ( )
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// RAW 이미지가 없으면
	if ( m_matList[ _RAW ].empty ( ) )
	{
		return;
	}

	// 원 정보 계산
	int iHeight = m_matList[ _RESULT ].rows;
	int iWidth = m_matList[ _RESULT ].cols;
	cv::Vec3i circle = cv::Vec3i (
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_CIRCLE_X_STATIC ) ) * iWidth / 1000,
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_CIRCLE_Y_STATIC ) ) * iHeight / 1000,
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_CIRCLE_R_STATIC ) ) * iHeight / 1000
	);

	// 이미지 처리
	ImageProcess imgProc( 
		circle ,
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_EPS_STATIC ) ) ,
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_MIN_SAMPLES_STATIC ) ) ,
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_HSV_S_THRESH_STATIC ) ) ,
		static_cast< int >( this->GetDlgItemInt ( IDC_MAIN_HSV_V_THRESH_STATIC ) )
	);

	imgProc.process ( m_matList[ _RAW ] , m_matList[ _RESULT ] , m_matList[ _SCATTER ] );


	// 이미지 보여주기
	m_imgStaticShow.bridgeImage ( &m_matList[ _RESULT ] );
	m_imgStaticShow.Invalidate ( TRUE );
	m_imgStaticScatter.bridgeImage ( &m_matList[ _SCATTER ] );
	m_imgStaticScatter.Invalidate ( TRUE );
}


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

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_EPS_SLIDER ) );
		pSlider->SetRange ( 0 , 100 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 0 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_MIN_SAMPLES_SLIDER ) );
		pSlider->SetRange ( 0 , 1000 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 0 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_HSV_S_THRESH_SLIDER ) );
		pSlider->SetRange ( 0 , 255 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 0 );

		pSlider = static_cast< CSliderCtrl* >( this->GetDlgItem ( IDC_MAIN_HSV_V_THRESH_SLIDER ) );
		pSlider->SetRange ( 0 , 255 );
		pSlider->SetTicFreq ( 1 );
		pSlider->SetPos ( 0 );

		// 초기값 설정
		this->SetDlgItemInt ( IDC_MAIN_EPS_STATIC , 0 );
		this->SetDlgItemInt ( IDC_MAIN_MIN_SAMPLES_STATIC , 0 );
		this->SetDlgItemInt ( IDC_MAIN_HSV_S_THRESH_STATIC , 0 );
		this->SetDlgItemInt ( IDC_MAIN_HSV_V_THRESH_STATIC , 0 );
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
	this->drawImage ( false , TRUE );



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


void CMyDbScanDlg::OnHScroll ( UINT nSBCode , UINT nPos , CScrollBar* pScrollBar )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if ( pScrollBar->GetSafeHwnd ( ) == this->GetDlgItem ( IDC_MAIN_EPS_SLIDER )->GetSafeHwnd ( ) )
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


void CMyDbScanDlg::OnBnClickedMainDoBtn ( )
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



}

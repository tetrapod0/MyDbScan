// CImageStatic.cpp: 구현 파일
//

#include "pch.h"
#include "CImageStatic.h"


// CImageStatic

IMPLEMENT_DYNAMIC(CImageStatic, CStatic)

CImageStatic::CImageStatic()
{

}

CImageStatic::~CImageStatic()
{
}


BEGIN_MESSAGE_MAP(CImageStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND ( )
END_MESSAGE_MAP()



// CImageStatic 메시지 처리기


void CImageStatic::getImgView ( const cv::Mat& src , cv::Mat& dst , const CRect& clientRect )
{
	if ( src.empty ( ) )
		return;

	// Raw 이미지 크기 가져오기
	double rawImgHeight = static_cast< double >( src.rows );
	double rawImgWidth = static_cast< double >( src.cols );

	// 최소 배율 구하기
	double windowHeight = static_cast< double >( clientRect.Height ( ) );
	double windowWidth = static_cast< double >( clientRect.Width ( ) );
	double hRatio = windowHeight / rawImgHeight;
	double wRatio = windowWidth / rawImgWidth;
	double ratio = std::min<double> ( hRatio , wRatio );

	// View 이미지 크기 구하기
	int viewImgHeight = static_cast< int >( rawImgHeight * ratio );
	int viewImgWidth = static_cast< int >( rawImgWidth * ratio );

	// SetDIBitsToDevice는 각 라인당 바이트 수가 4바이트 배수여야 제대로 표시됨.
	viewImgWidth = viewImgWidth / 4 * 4;

	// View 이미지 공간 할당
	if ( viewImgHeight != dst.rows || viewImgWidth != dst.cols || dst.type ( ) != src.type ( ) )
		dst.create ( viewImgHeight , viewImgWidth , src.type ( ) );

	// raw 이미지 리사이즈
	cv::resize ( src , dst , dst.size ( ) );
}


void CImageStatic::FillBitmapInfo2 ( BITMAPINFO* bmi , int width , int height , int bpp )
{
	assert ( bmi && width >= 0 && height >= 0 && ( bpp == 8 || bpp == 24 || bpp == 32 ) );

	BITMAPINFOHEADER* bmih = &( bmi->bmiHeader );

	memset ( bmih , 0 , sizeof ( *bmih ) );
	bmih->biSize = sizeof ( BITMAPINFOHEADER );
	bmih->biWidth = width;
	bmih->biHeight = -height;
	bmih->biPlanes = 1;
	bmih->biBitCount = ( unsigned short ) bpp; // 24
	bmih->biCompression = BI_RGB;

	if ( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for ( i = 0; i < 256; i++ )
		{
			palette[ i ].rgbBlue = palette[ i ].rgbGreen = palette[ i ].rgbRed = ( BYTE ) i;
			palette[ i ].rgbReserved = 0;
		}
	}
}


void  CImageStatic::Show ( const cv::Mat& img , HDC dc , int xDest , int yDest )
{
	if ( img.empty ( ) || img.depth ( ) != CV_8U )
		return;

	//uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
	uchar buffer[ sizeof ( BITMAPINFOHEADER ) + 256 * sizeof ( RGBQUAD ) ];
	BITMAPINFO* pBmi = ( BITMAPINFO* ) buffer;

	this->FillBitmapInfo2 ( pBmi , img.cols , img.rows , img.channels ( ) * 8 );

	::SetDIBitsToDevice (
		dc ,				// 출력 대상 DC
		xDest ,			// 출력 위치 x
		yDest ,			// 출력 위치 y
		img.cols ,		// 출력할 너비 // 4의 배수여야됨
		img.rows ,		// 출력할 높이
		0 ,				// 소스 데이터의 시작 x (일반적으로 0)
		0 ,				// 소스 데이터의 시작 y (일반적으로 0)
		0 ,				// 시작 스캔라인 (일반적으로 0)
		img.rows ,		// 출력할 스캔라인 수 (보통 높이와 같음)
		img.data ,		// 실제 비트맵 픽셀 데이터 (RGB 배열 등)
		pBmi ,			// 비트맵 정보 (해상도, 포맷 등)
		DIB_RGB_COLORS	// 색상 사용 방식 (DIB_RGB_COLORS 또는 DIB_PAL_COLORS)
	); // img가 RGB라 가정하고 모니터에는 BGR로 보여준다? 또는 역순

	//StretchDIBits(
	//	dc,				// 출력 대상 DC
	//	xDest,			// 출력 위치 x
	//	yDest,			// 출력 위치 y
	//	img.cols,		// 출력 너비
	//	img.rows,		// 출력 높이
	//	0,				// 소스 x
	//	0,				// 소스 y
	//	img.cols,		// 소스 너비
	//	img.rows,		// 소스 높이
	//	img.data,		// 이미지 데이터
	//	pBmi,			// 비트맵 정보
	//	DIB_RGB_COLORS,	// 색상 방식
	//	SRCCOPY			// 래스터 연산 방식
	//);
}


void CImageStatic::OnPaint ( )
{
	CPaintDC dc ( this ); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	// 윈도우 크기 가져오기
	CRect rect;
	this->GetClientRect ( &rect );

	// Raw 이미지 비어있을 경우
	if ( m_pImgRaw == nullptr || m_pImgRaw->empty ( ) )
	{
		dc.FillSolidRect ( 0 , 0 , rect.Width ( ) , rect.Height ( ) , RGB ( 0 , 0 , 0 ) );
		m_imgView.release ( );
	}
	// Raw 이미지 있을 경우
	else
	{
		this->getImgView ( *m_pImgRaw , m_imgView , rect ); // raw -> view 이미지 변환
		if ( m_funcDrawInView != nullptr )
			m_funcDrawInView ( m_imgView ); // View 이미지에 그리기
	}

	// DC에 출력
	int xDest = ( rect.Width ( ) - m_imgView.cols ) / 2; // DC에서 x 시작 위치
	int yDest = ( rect.Height ( ) - m_imgView.rows ) / 2; // DC에서 y 시작 위치
	this->Show ( m_imgView , dc.m_hDC , xDest , yDest );
}


BOOL CImageStatic::OnEraseBkgnd ( CDC* pDC )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	this->GetClientRect ( &rect );
	pDC->FillSolidRect ( &rect , RGB ( 240 , 240 , 240 ) );

	return CStatic::OnEraseBkgnd ( pDC );
}

#include "stdafx.h"
#include "instantmessaging.h"
#include "WhiteBoardClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InstantMessagingDlg.h"
#include "WhiteBoardClientSocket.h"

IMPLEMENT_DYNAMIC( CWhiteBoardClientDlg, CDialog )

CWhiteBoardClientDlg::CWhiteBoardClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWhiteBoardClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWhiteBoardClientDlg)
	//}}AFX_DATA_INIT
	
	m_imageList.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 0 );
	CBitmap bitmap32;
	CBitmap bitmap16;
	HBITMAP hBitmap;
	for( int nIndex = 0; nIndex < MAXFACES; nIndex++ )
	{
		bitmap32.LoadBitmap( IDB_BITMAP1 + nIndex );
		hBitmap = ( HBITMAP )::CopyImage( ( HBITMAP )bitmap32, IMAGE_BITMAP, 16, 16, LR_COPYDELETEORG );
		bitmap16.Attach( hBitmap );
		m_imageList.Add( &bitmap16, FACEBGCOLOR );
		bitmap16.DeleteObject();
		bitmap32.DeleteObject();
		::DeleteObject( hBitmap );
	}
	m_emTool		= PENCIL;     //����
	m_nWidth		= WHITEBOARD_WIDTH_1;//�߿�
	m_clDrawColor	= DEFAULT_WHITEBOARD_TOOL_COLOR;//��ɫ
}


void CWhiteBoardClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhiteBoardClientDlg)
	DDX_Control(pDX, IDS_COLOR, m_staticColor);
	DDX_Control(pDX, IDS_CANVAS, m_staticCanvas);
	DDX_Control(pDX, IDB_WIDTH_4, m_btnWidth4);
	DDX_Control(pDX, IDB_WIDTH_3, m_btnWidth3);
	DDX_Control(pDX, IDB_WIDTH_2, m_btnWidth2);
	DDX_Control(pDX, IDB_WIDTH_1, m_btnWidth1);
	DDX_Control(pDX, IDB_RECTANGLE_EMPTY, m_btnRectangleEmpty);
	DDX_Control(pDX, IDB_RECTANGLE, m_btnRectangle);
	DDX_Control(pDX, IDB_PENCIL, m_btnPencil);
	DDX_Control(pDX, IDB_LINE, m_btnLine);
	DDX_Control(pDX, IDB_ERASER, m_btnEraser);
	DDX_Control(pDX, IDB_ELLIPSE_EMPTY, m_btnEllipseEmpty);
	DDX_Control(pDX, IDB_ELLIPSE, m_btnEllipse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhiteBoardClientDlg, CDialog)
	//{{AFX_MSG_MAP(CWhiteBoardClientDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDB_ERASER, OnEraser)
	ON_BN_CLICKED(IDB_PENCIL, OnPencil)
	ON_BN_CLICKED(IDB_LINE, OnLine)
	ON_BN_CLICKED(IDB_RECTANGLE_EMPTY, OnRectangleEmpty)
	ON_BN_CLICKED(IDB_RECTANGLE, OnRectangle)
	ON_BN_CLICKED(IDB_ELLIPSE_EMPTY, OnEllipseEmpty)
	ON_BN_CLICKED(IDB_ELLIPSE, OnEllipse)
	ON_BN_CLICKED(IDS_COLOR, OnColor)
	ON_BN_CLICKED(IDB_WIDTH_1, OnWidth1)
	ON_BN_CLICKED(IDB_WIDTH_2, OnWidth2)
	ON_BN_CLICKED(IDB_WIDTH_3, OnWidth3)
	ON_BN_CLICKED(IDB_WIDTH_4, OnWidth4)
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CWhiteBoardClientDlg::OnOk() 
{	
}

void CWhiteBoardClientDlg::OnCancel() 
{
	if( IDOK == MessageBox( "��Ҫ������������", "��Ҫ����", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2 ) )
	{	
		USER userSelf;
		userSelf.nFace		= m_pMainDlg->GetFace();
		memcpy( userSelf.strName, m_pMainDlg->GetNickName().GetBuffer( MAXNICKNAMELENGTH + 1 ), MAXNICKNAMELENGTH + 1 );
		
		SendUserCommandToIP( WHITEBOARD_CLIENT_COLSED, m_strServerIP, WHITEBOARD_SERVER_PORT, &userSelf );

	
		m_pWBCSocket->Close();
		delete [] m_pWBCSocket;
		m_pWBCSocket = NULL;

		ResetTools();
		ResetWidth();
		m_emTool		= PENCIL;
		m_nWidth		= WHITEBOARD_WIDTH_1;
		m_clDrawColor	= DEFAULT_WHITEBOARD_TOOL_COLOR;

		m_staticCanvas.ClearCanvas();
		DestroyWindow();
	}
}

BOOL CWhiteBoardClientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/// ��ʼ��
	Init();
	
	return TRUE;  
	             
}


/// ��ӷ��������Լ����û��б��
void CWhiteBoardClientDlg::AcceptAddRequest(  const USER userServer )
{
	/// ���÷�����IP
	m_strServerIP = userServer.strIP;

	/// �Լ���USER
	USER userSelf;
	userSelf.bHasCamera	= m_pMainDlg->HasCamera();
	userSelf.nFace		= m_pMainDlg->GetFace();
	memcpy( userSelf.strName, m_pMainDlg->GetNickName().GetBuffer( MAXNICKNAMELENGTH + 1 ), MAXNICKNAMELENGTH + 1 );

	/// �����������װ�
	SendUserCommandToIP( WHITEBOARD_ADDREQUEST_ALLOW, m_strServerIP, WHITEBOARD_SERVER_PORT, &userSelf );
}

/// ���߳��װ�
void CWhiteBoardClientDlg::Kicked()
{
	/// �ر�UDP�׽���
	m_pWBCSocket->Close();

	/// ��ʾ
	MessageBox( "�㱻�߳����˰װ壡", "�װ�" );

	/// �������
	m_staticCanvas.ClearCanvas();
	
	/// ����Ĭ��
	ResetTools();
	ResetWidth();
	m_emTool		= PENCIL;
	m_nWidth		= WHITEBOARD_WIDTH_1;
	m_clDrawColor	= DEFAULT_WHITEBOARD_TOOL_COLOR;

	/// �رնԻ���
	DestroyWindow();
}

/// �������ر�
void CWhiteBoardClientDlg::ServerClosed()
{
	/// �ر�UDP�׽���
	m_pWBCSocket->Close();

	/// ��ʾ
	MessageBox( "�ر��˰װ壡", "�װ�" );

	/// �������
	m_staticCanvas.ClearCanvas();
	
	/// ����Ĭ��
	ResetTools();
	ResetWidth();
	m_emTool		= PENCIL;
	m_nWidth		= WHITEBOARD_WIDTH_1;
	m_clDrawColor	= DEFAULT_WHITEBOARD_TOOL_COLOR;
	
	/// �رնԻ���
	DestroyWindow();
}

/// ������Ļ����
void CWhiteBoardClientDlg::ReceiveDrawData( char *pReceivedData )
{
	DRAWINFO drawInfo;
	memcpy( &drawInfo, pReceivedData + sizeof( DATAPACKET ), sizeof( DRAWINFO ) );
	m_staticCanvas.Draw( drawInfo );
}

void CWhiteBoardClientDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC *pDC = m_staticColor.GetDC();
	CRect rect;
	m_staticColor.GetClientRect( &rect );
	rect.DeflateRect( 1, 1 );
	pDC->FillSolidRect( rect, m_clDrawColor );
	m_staticColor.ReleaseDC( pDC );	
	m_staticCanvas.Invalidate();	
}

/// ��ʼ��
void CWhiteBoardClientDlg::Init()
{
	/// ��ȡ���Ի���
	m_pMainDlg = ( CInstantMessagingDlg * )::AfxGetMainWnd();
	
	/// ������Ƥ�ߵ�ͼ��
	m_btnEraser.setBmp(	IDB_WHITEBOARD_ERASE_NORMAL,
		IDB_WHITEBOARD_ERASE_HOVER,
		IDB_WHITEBOARD_ERASE_DOWN,
		IDB_WHITEBOARD_ERASE_DISABLE );

	/// ���û��ʵ�ͼ��
	m_btnPencil.setBmp(	IDB_WHITEBOARD_PENCIL_NORMAL,
		IDB_WHITEBOARD_PENCIL_HOVER,
		IDB_WHITEBOARD_PENCIL_DOWN,
		IDB_WHITEBOARD_PENCIL_DISABLE );

	/// ����ֱ�ߵ�ͼ��
	m_btnLine.setBmp( IDB_WHITEBOARD_LINE_NORMAL,
		IDB_WHITEBOARD_LINE_HOVER,
		IDB_WHITEBOARD_LINE_DOWN,
		IDB_WHITEBOARD_LINE_DISABLE );

	/// ���ÿ��ľ��ε�ͼ��
	m_btnRectangleEmpty.setBmp(	IDB_WHITEBOARD_RECTANGLE_EMPTY_NORMAL,
		IDB_WHITEBOARD_RECTANGLE_EMPTY_HOVER,
		IDB_WHITEBOARD_RECTANGLE_EMPTY_DOWN,
		IDB_WHITEBOARD_RECTANGLE_EMPTY_DISABLE );

	/// ����ʵ�ľ��ε�ͼ��
	m_btnRectangle.setBmp( IDB_WHITEBOARD_RECTANGLE_NORMAL,
		IDB_WHITEBOARD_RECTANGLE_HOVER,
		IDB_WHITEBOARD_RECTANGLE_DOWN,
		IDB_WHITEBOARD_RECTANGLE_DISABLE );

	/// ���ÿ�����Բ��ͼ��
	m_btnEllipseEmpty.setBmp( IDB_WHITEBOARD_ELLIPSE_EMPTY_NORMAL,
		IDB_WHITEBOARD_ELLIPSE_EMPTY_HOVER,
		IDB_WHITEBOARD_ELLIPSE_EMPTY_DOWN,
		IDB_WHITEBOARD_ELLIPSE_EMPTY_DISABLE );

	/// ����ʵ����Բ��ͼ��
	m_btnEllipse.setBmp( IDB_WHITEBOARD_ELLIPSE_NORMAL,
		IDB_WHITEBOARD_ELLIPSE_HOVER,
		IDB_WHITEBOARD_ELLIPSE_DOWN,
		IDB_WHITEBOARD_ELLIPSE_DISABLE );



	/// �����߿�һ��ͼ��
	m_btnWidth1.setBmp( IDB_WHITEBOARD_WIDTH_1_NORMAL,
		IDB_WHITEBOARD_WIDTH_1_HOVER,
		IDB_WHITEBOARD_WIDTH_1_DOWN,
		IDB_WHITEBOARD_WIDTH_1_DISABLE );

	/// �����߿����ͼ��
	m_btnWidth2.setBmp( IDB_WHITEBOARD_WIDTH_2_NORMAL,
		IDB_WHITEBOARD_WIDTH_2_HOVER,
		IDB_WHITEBOARD_WIDTH_2_DOWN,
		IDB_WHITEBOARD_WIDTH_2_DISABLE );

	/// �����߿�����ͼ��
	m_btnWidth3.setBmp( IDB_WHITEBOARD_WIDTH_3_NORMAL,
		IDB_WHITEBOARD_WIDTH_3_HOVER,
		IDB_WHITEBOARD_WIDTH_3_DOWN,
		IDB_WHITEBOARD_WIDTH_3_DISABLE );

	/// �����߿��ĵ�ͼ��
	m_btnWidth4.setBmp( IDB_WHITEBOARD_WIDTH_4_NORMAL,
		IDB_WHITEBOARD_WIDTH_4_HOVER,
		IDB_WHITEBOARD_WIDTH_4_DOWN,
		IDB_WHITEBOARD_WIDTH_4_DISABLE );

	/// ����ѡ�еĹ��ߺ��߿�
	m_btnPencil.EnableWindow( FALSE );
	m_btnWidth1.EnableWindow( FALSE );
	
	// ��ʼ��UDP�׽���
	m_pWBCSocket = new CWhiteBoardClientSocket( this );
	m_pWBCSocket->Create( WHITEBOARD_CLIENT_PORT, SOCK_DGRAM );

	/// �������ؼ���λ��
	SetPosition();

	/// ���û����ĳ�ʼ״̬
	m_staticCanvas.SetTool( m_emTool );
	m_staticCanvas.SetWidth( m_nWidth );
	m_staticCanvas.SetColor( m_clDrawColor );
}

/// ��UDP�н�������
void CWhiteBoardClientDlg::OnReceive()
{
	/// ��������
	CString strIP;
	UINT uPort;
	char szReceive[ MAXDATAPACKETLENGTH ];
	m_pWBCSocket->ReceiveFrom( szReceive, MAXDATAPACKETLENGTH, strIP, uPort );
	
	/// װ�����ݰ�
	DATAPACKET dataPacket;
	memcpy( &dataPacket, szReceive, sizeof( DATAPACKET ) );
	
	switch( dataPacket.command )
	{
	case WHITEBOARD_SERVER_CLOSED:							/// �װ�������ر�
		ServerClosed();
		break;
	case WHITEBOARD_CLIENT_KICKED:							/// �ͻ��˱���
		Kicked();
		break;
	case WHITEBOARD_DRAWINFO:						    	///	�װ�������Ļ�ͼ����
		ReceiveDrawData( szReceive );
		break;
	default:
		break;
	}
}

/// �������ؼ���λ��
void CWhiteBoardClientDlg::SetPosition()
{
	/// �ؼ�δ����������
	if( !m_staticCanvas )
	{
		return ;
	}
	CRect rect;
	GetClientRect( &rect );

	CRect rectCanvas;
	m_staticCanvas.GetWindowRect( &rectCanvas );
	ScreenToClient( &rectCanvas );

	rectCanvas.right	= rect.right - 8;
	rectCanvas.bottom	= rect.bottom - 8;
	m_staticCanvas.MoveWindow( rectCanvas );
}	

void CWhiteBoardClientDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	SetPosition();
}

void CWhiteBoardClientDlg::OnColor() 
{
	/// ������ɫ��Ի���
	CColorDialog clDlg;
	clDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	clDlg.m_cc.rgbResult = m_clDrawColor;
	if( IDCANCEL == clDlg.DoModal() )
	{
		return ;
	}

	/// ���ò�����
	m_clDrawColor = clDlg.GetColor();
	CDC *pDC = m_staticColor.GetDC();
	CRect rect;
	m_staticColor.GetClientRect( &rect );
	rect.DeflateRect( 1, 1 );
	pDC->FillSolidRect( rect, m_clDrawColor );
	m_staticColor.ReleaseDC( pDC );
	m_staticCanvas.SetColor( m_clDrawColor );
}

void CWhiteBoardClientDlg::OnEraser() 
{
	ResetTools();
	m_emTool = ERASER;
	m_btnEraser.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnPencil() 
{
	ResetTools();
	m_emTool = PENCIL;
	m_btnPencil.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnLine() 
{
	ResetTools();
	m_emTool = LINE;
	m_btnLine.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnRectangle() 
{
	ResetTools();
	m_emTool = RECTANGLE;
	m_btnRectangle.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnRectangleEmpty() 
{
	ResetTools();
	m_emTool = RECTANGLE_EMPTY;
	m_btnRectangleEmpty.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnEllipseEmpty() 
{
	ResetTools();
	m_emTool = ELLIPSE_EMPTY;
	m_btnEllipseEmpty.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnEllipse()
{
	ResetTools();
	m_emTool = ELLIPSE;
	m_btnEllipse.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardClientDlg::OnWidth1() 
{
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_1;
	m_btnWidth1.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

void CWhiteBoardClientDlg::OnWidth2() 
{	
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_2;
	m_btnWidth2.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

void CWhiteBoardClientDlg::OnWidth3() 
{
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_3;
	m_btnWidth3.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

void CWhiteBoardClientDlg::OnWidth4() 
{	
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_4;
	m_btnWidth4.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

/// �ָ����ߵ�״̬
void CWhiteBoardClientDlg::ResetTools()
{
	switch( m_emTool )
	{
	case ERASER:
		m_btnEraser.SetNormal();
		break;
	case PENCIL:
		m_btnPencil.SetNormal();
		break;
	case LINE:
		m_btnLine.SetNormal();
		break;
	case RECTANGLE_EMPTY:
		m_btnRectangleEmpty.SetNormal();
		break;
	case RECTANGLE:
		m_btnRectangle.SetNormal();
		break;
	case ELLIPSE_EMPTY:
		m_btnEllipseEmpty.SetNormal();
		break;
	case ELLIPSE:
		m_btnEllipse.SetNormal();
		break;
	default:
		break;
	}
}

///	�ָ��߿�
void CWhiteBoardClientDlg::ResetWidth()
{
	switch( m_nWidth )
	{
	case WHITEBOARD_WIDTH_1:
		m_btnWidth1.SetNormal();
		break;
	case WHITEBOARD_WIDTH_2:
		m_btnWidth2.SetNormal();
		break;
	case WHITEBOARD_WIDTH_3:
		m_btnWidth3.SetNormal();
		break;
	case WHITEBOARD_WIDTH_4:
		m_btnWidth4.SetNormal();
		break;
	default:
		break;
	}
}

void CWhiteBoardClientDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	
	/// ���ƴ��ڵ���С
	switch( fwSide )
	{
	case 1:
		pRect->left = pRect->right - pRect->left < WHITEBOARD_MIN_WIDTH ? pRect->right - WHITEBOARD_MIN_WIDTH : pRect->left;
		break;
	case 2:
		pRect->right = pRect->right - pRect->left < WHITEBOARD_MIN_WIDTH ? pRect->left + WHITEBOARD_MIN_WIDTH : pRect->right;
		break;
	case 3:
		pRect->top = pRect->bottom - pRect->top < WHITEBOARD_MIN_HEIGHT ? pRect->bottom - WHITEBOARD_MIN_HEIGHT : pRect->top;
		break;
	case 4:
		pRect->left = pRect->right - pRect->left < WHITEBOARD_MIN_WIDTH ? pRect->right - WHITEBOARD_MIN_WIDTH : pRect->left;
		pRect->top = pRect->bottom - pRect->top < WHITEBOARD_MIN_HEIGHT ? pRect->bottom - WHITEBOARD_MIN_HEIGHT : pRect->top;
		break;
	case 5:
		pRect->right = pRect->right - pRect->left < WHITEBOARD_MIN_WIDTH ? pRect->left + WHITEBOARD_MIN_WIDTH : pRect->right;
		pRect->top = pRect->bottom - pRect->top < WHITEBOARD_MIN_HEIGHT ? pRect->bottom - WHITEBOARD_MIN_HEIGHT : pRect->top;
		break;
	case 6:
		pRect->bottom = pRect->bottom - pRect->top < WHITEBOARD_MIN_HEIGHT ? pRect->top + WHITEBOARD_MIN_HEIGHT : pRect->bottom;
		break;
	case 7:
		pRect->left = pRect->right - pRect->left < WHITEBOARD_MIN_WIDTH ? pRect->right - WHITEBOARD_MIN_WIDTH : pRect->left;
		pRect->bottom = pRect->bottom - pRect->top < WHITEBOARD_MIN_HEIGHT ? pRect->top + WHITEBOARD_MIN_HEIGHT : pRect->bottom;
		break;
	case 8:
		pRect->right = pRect->right - pRect->left < WHITEBOARD_MIN_WIDTH ? pRect->left + WHITEBOARD_MIN_WIDTH : pRect->right;
		pRect->bottom = pRect->bottom - pRect->top < WHITEBOARD_MIN_HEIGHT ? pRect->top + WHITEBOARD_MIN_HEIGHT : pRect->bottom;
		break;
	default:
		break;
	}	
}

/// ��ָ��IP��PORT����ָ������
void CWhiteBoardClientDlg::SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT uPort, const USER *pUser )
{
	DATAPACKET dataPacket;
	dataPacket.command	= command;
	
	/// ����ռ�
	UINT uDataLength = sizeof( DATAPACKET );
	if( pUser )
	{
		uDataLength += sizeof( USER );
	}
	BYTE *pSendData = new BYTE[ uDataLength ];
	memcpy( pSendData, &dataPacket, sizeof( DATAPACKET ) );
	if( pUser )
	{
		memcpy( pSendData + sizeof( DATAPACKET ), pUser, sizeof( USER ) );
	}
	
	/// ��������
	m_pWBCSocket->SendTo( pSendData, uDataLength, uPort, szIP );
	
	delete pSendData;
}

/// ����ͼ��Ϣ���͵�������
void CWhiteBoardClientDlg::SendDrawInfoToServer(  const CPoint ptStart, const CPoint ptEnd )
{
	DRAWINFO drawInfo;
	drawInfo.color		= m_clDrawColor;
	drawInfo.nWidth		= m_nWidth;
	drawInfo.tool		= m_emTool;
	drawInfo.ptStart	= ptStart;
	drawInfo.ptEnd		= ptEnd;

	DATAPACKET dataPacket;
	dataPacket.command	= WHITEBOARD_DRAWINFO;

	UINT uDataLength	= sizeof( dataPacket ) + sizeof( DRAWINFO );
	BYTE *pSendData		= new BYTE[ uDataLength ];

	memcpy( pSendData, &dataPacket, sizeof( DATAPACKET ) );
	memcpy( pSendData + sizeof( DATAPACKET ), &drawInfo, sizeof( DRAWINFO ) );
	
	/// ��������
	m_pWBCSocket->SendTo( pSendData, uDataLength, WHITEBOARD_SERVER_PORT, m_strServerIP );
	
	delete [] pSendData;
}

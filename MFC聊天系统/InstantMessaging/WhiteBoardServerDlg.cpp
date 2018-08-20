// WhiteBoardServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "instantmessaging.h"
#include "WhiteBoardServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhiteBoardServerDlg dialog

#include "InstantMessagingDlg.h"
#include "WhiteBoardServerSocket.h"

/// ֧��RUNTIME_CLASS
IMPLEMENT_DYNAMIC( CWhiteBoardServerDlg, CDialog )

CWhiteBoardServerDlg::CWhiteBoardServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWhiteBoardServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWhiteBoardServerDlg)
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

	/// ����Ĭ��
	m_emTool		= PENCIL;
	m_nWidth		= WHITEBOARD_WIDTH_1;
	m_clDrawColor	= DEFAULT_WHITEBOARD_TOOL_COLOR;
}


void CWhiteBoardServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhiteBoardServerDlg)
	DDX_Control(pDX, IDS_CANVAS, m_staticCanvas);
	DDX_Control(pDX, IDB_WIDTH_4, m_btnWidth4);
	DDX_Control(pDX, IDB_WIDTH_3, m_btnWidth3);
	DDX_Control(pDX, IDB_WIDTH_2, m_btnWidth2);
	DDX_Control(pDX, IDB_WIDTH_1, m_btnWidth1);
	DDX_Control(pDX, IDS_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_LIST_OUTOFWHITEBOARD, m_listCtrlOutOfWhiteBoard);
	DDX_Control(pDX, IDC_LIST_INWHITEBOARD, m_listCtrlInWhiteBoard);
	DDX_Control(pDX, IDB_RECTANGLE_EMPTY, m_btnRectangelEmpty);
	DDX_Control(pDX, IDB_RECTANGLE, m_btnRectangle);
	DDX_Control(pDX, IDB_PENCIL, m_btnPencil);
	DDX_Control(pDX, IDB_LINE, m_btnLine);
	DDX_Control(pDX, IDB_ERASER, m_btnEraser);
	DDX_Control(pDX, IDB_ELLIPSE_EMPTY, m_btnElipseEmpty);
	DDX_Control(pDX, IDB_ELLIPSE, m_btnEllipse);
	DDX_Control(pDX, IDB_DELFRIENDS, m_btnDelFriends);
	DDX_Control(pDX, IDB_ADDFRIENDS, m_btnAddFriends);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhiteBoardServerDlg, CDialog)
	//{{AFX_MSG_MAP(CWhiteBoardServerDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDB_ADDFRIENDS, OnAddfriends)
	ON_BN_CLICKED(IDB_DELFRIENDS, OnDelfriends)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDS_COLOR, OnColor)
	ON_BN_CLICKED(IDB_ERASER, OnEraser)
	ON_BN_CLICKED(IDB_PENCIL, OnPencil)
	ON_BN_CLICKED(IDB_LINE, OnLine)
	ON_BN_CLICKED(IDB_RECTANGLE, OnRectangle)
	ON_BN_CLICKED(IDB_RECTANGLE_EMPTY, OnRectangleEmpty)
	ON_BN_CLICKED(IDB_ELLIPSE_EMPTY, OnEllipseEmpty)
	ON_BN_CLICKED(IDB_ELLIPSE, OnEllipse)
	ON_BN_CLICKED(IDB_WIDTH_1, OnWidth1)
	ON_BN_CLICKED(IDB_WIDTH_2, OnWidth2)
	ON_BN_CLICKED(IDB_WIDTH_3, OnWidth3)
	ON_BN_CLICKED(IDB_WIDTH_4, OnWidth4)
	ON_WM_PAINT()
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhiteBoardServerDlg message handlers

void CWhiteBoardServerDlg::OnCancel() 
{
	if( IDOK == MessageBox( "Ҫ�رհװ���", "�ر�", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2 ) )
	{
		/// �����пͻ��˷��͹رշ�������Ϣ
		for( int nIndex = 0; nIndex < m_listCtrlInWhiteBoard.GetItemCount(); nIndex++ )
		{
			USER user = m_arrFriendsInWhiteBoard.GetAt( nIndex );
			SendUserCommandToIP( WHITEBOARD_SERVER_CLOSED, user.strIP, WHITEBOARD_CLIENT_PORT, NULL );
		}
		
		/// �ر�UDP�׽���
		m_pWBSSocket->Close();
		delete [] m_pWBSSocket;
		m_pWBSSocket = NULL;
		
		/// ����û��б�
		m_arrFriendsInWhiteBoard.RemoveAll();
		m_arrFriendsOutOfWhiteBoard.RemoveAll();
		
		/// ����Ĭ��
		ResetTools();
		ResetWidth();
		m_emTool		= PENCIL;
		m_nWidth		= WHITEBOARD_WIDTH_1;
		m_clDrawColor	= DEFAULT_WHITEBOARD_TOOL_COLOR;


		/// �������
		m_staticCanvas.ClearCanvas();
		DestroyWindow();
	}
}

void CWhiteBoardServerDlg::OnOk() 
{	
}

void CWhiteBoardServerDlg::OnPaint() 
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

BOOL CWhiteBoardServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/// ��ʼ��
	Init();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/// ��ʼ��
void CWhiteBoardServerDlg::Init()
{
	/// ��ȡ���Ի���
	m_pMainDlg = ( CInstantMessagingDlg * )::AfxGetMainWnd();
	
	/// ���������ϵ�˵�ͼ��
	m_btnAddFriends.setBmp( IDB_CHATROOM_ADD_NORMAL, 
		IDB_CHATROOM_ADD_HOVER, 
		IDB_CHATROOM_ADD_DOWN, 
		IDB_CHATROOM_ADD_DISABLE );
	
	/// ����ɾ����ϵ�˵�ͼ��
	m_btnDelFriends.setBmp( IDB_CHATROOM_DEL_NORMAL, 
		IDB_CHATROOM_DEL_HOVER, 
		IDB_CHATROOM_DEL_DOWN, 
		IDB_CHATROOM_DEL_DISABLE );

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
	m_btnRectangelEmpty.setBmp(	IDB_WHITEBOARD_RECTANGLE_EMPTY_NORMAL,
		IDB_WHITEBOARD_RECTANGLE_EMPTY_HOVER,
		IDB_WHITEBOARD_RECTANGLE_EMPTY_DOWN,
		IDB_WHITEBOARD_RECTANGLE_EMPTY_DISABLE );

	/// ����ʵ�ľ��ε�ͼ��
	m_btnRectangle.setBmp( IDB_WHITEBOARD_RECTANGLE_NORMAL,
		IDB_WHITEBOARD_RECTANGLE_HOVER,
		IDB_WHITEBOARD_RECTANGLE_DOWN,
		IDB_WHITEBOARD_RECTANGLE_DISABLE );

	/// ���ÿ�����Բ��ͼ��
	m_btnElipseEmpty.setBmp( IDB_WHITEBOARD_ELLIPSE_EMPTY_NORMAL,
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
	
	/// �����û��б���ͼ��
	m_listCtrlInWhiteBoard.SetImageList( &m_imageList, LVSIL_SMALL );
	m_listCtrlInWhiteBoard.SetExtendedStyle( m_listCtrlInWhiteBoard.GetExtendedStyle() | 
		LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	m_listCtrlInWhiteBoard.InsertColumn( 0, "" );
	
	m_listCtrlOutOfWhiteBoard.SetImageList( &m_imageList, LVSIL_SMALL );
	m_listCtrlOutOfWhiteBoard.SetExtendedStyle( m_listCtrlOutOfWhiteBoard.GetExtendedStyle() | 
		LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	m_listCtrlOutOfWhiteBoard.InsertColumn( 0, "" );
	
	/// ���ò��ڰװ�ĺ��ѣ���ȫ��ѡ��
	m_pMainDlg->GetFriendsArray( m_arrFriendsOutOfWhiteBoard );
	for( int nIndex = 0; nIndex < m_arrFriendsOutOfWhiteBoard.GetSize(); nIndex++ )
	{
		USER user = m_arrFriendsOutOfWhiteBoard.GetAt( nIndex );
		CString strText;
		strText.Format( "%s(%s)", user.strName, user.strIP );
		m_listCtrlOutOfWhiteBoard.InsertItem( nIndex, strText, user.nFace );
		m_listCtrlOutOfWhiteBoard.SetCheck( nIndex );
		m_listCtrlOutOfWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	}
	
	
	/// ���á���ӡ��͡�ɾ������ť��״̬
	if( 0 == m_arrFriendsOutOfWhiteBoard.GetSize() )
	{
		m_btnAddFriends.EnableWindow( FALSE );	
	}
	else
	{	
		m_btnAddFriends.SetNormal();
	}
	m_btnDelFriends.EnableWindow( FALSE );
	
	// ��ʼ��UDP�׽���
	m_pWBSSocket = new CWhiteBoardServerSocket( this );
	m_pWBSSocket->Create( WHITEBOARD_SERVER_PORT, SOCK_DGRAM );

	/// �������ؼ���λ��
	SetPosition();

	/// ���û����ĳ�ʼ״̬
	m_staticCanvas.SetTool( m_emTool );
	m_staticCanvas.SetWidth( m_nWidth );
	m_staticCanvas.SetColor( m_clDrawColor );

	/// ���û����ĸ��Ի���
	m_staticCanvas.SetParent( this );
}

/// ��ָ��IP��PORT����ָ������
void CWhiteBoardServerDlg::SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT uPort, const USER *pUser )
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
	m_pWBSSocket->SendTo( pSendData, uDataLength, uPort, szIP );
	
	delete pSendData;
}

/// ��UDP�н�������
void CWhiteBoardServerDlg::OnReceive()
{
	/// ��������
	CString strIP;
	UINT uPort;
	char szReceive[ MAXDATAPACKETLENGTH ];
	m_pWBSSocket->ReceiveFrom( szReceive, MAXDATAPACKETLENGTH, strIP, uPort );
	
	/// װ�����ݰ�
	DATAPACKET dataPacket;
	memcpy( &dataPacket, szReceive, sizeof( DATAPACKET ) );
	USER user;
	
	/// ����������в�ͬ�Ĵ���
	switch( dataPacket.command )
	{
	case WHITEBOARD_ADDREQUEST_DENY:							/// �ͻ��˾ܾ�����װ������
		break;
	case WHITEBOARD_ADDREQUEST_ALLOW:							/// �ͻ����������װ������
		memcpy( &user, szReceive + sizeof( DATAPACKET ), sizeof( USER ) );
		memcpy( user.strIP, strIP.GetBuffer( 16 ), 16 );
		strIP.ReleaseBuffer();
		AddUserToWhiteBoard( user );
		break;
	case WHITEBOARD_CLIENT_COLSED:								/// �ͻ��˹رհװ�
		memcpy( &user, szReceive + sizeof( DATAPACKET ), sizeof( USER ) );
		memcpy( user.strIP, strIP.GetBuffer( 16 ), 16 );
		strIP.ReleaseBuffer();
		DelUserFromWhiteBoard( user );
		break;
	case WHITEBOARD_DRAWINFO:
		memcpy( &user, szReceive + sizeof( DATAPACKET ), sizeof( USER ) );
		memcpy( user.strIP, strIP.GetBuffer( 16 ), 16 );
		strIP.ReleaseBuffer();
		ReceiveClientDrawInfo( szReceive, user );
		break;
	default:
		break;
	}
}

void CWhiteBoardServerDlg::OnAddfriends() 
{
	/// ����װ������USER
	USER userSelf;
	userSelf.bHasCamera	= m_pMainDlg->HasCamera();
	userSelf.nFace		= m_pMainDlg->GetFace();
	memcpy( userSelf.strName, m_pMainDlg->GetNickName().GetBuffer( MAXNICKNAMELENGTH + 1 ), MAXNICKNAMELENGTH + 1 );
	
	for( int nIndex = 0; nIndex < m_listCtrlOutOfWhiteBoard.GetItemCount(); nIndex++ )
	{
		if( m_listCtrlOutOfWhiteBoard.GetCheck( nIndex ) )
		{
			USER user = m_arrFriendsOutOfWhiteBoard.GetAt( nIndex );
			SendUserCommandToIP( WHITEBOARD_ADDREQUEST, user.strIP, DEFAULT_PORT, &userSelf );
		}
	}
}

void CWhiteBoardServerDlg::OnDelfriends() 
{
	static BOOL bFirst = TRUE;
	for( int nIndex = 0; nIndex < m_listCtrlInWhiteBoard.GetItemCount(); nIndex++ )
	{
		if( m_listCtrlInWhiteBoard.GetCheck( nIndex ) )
		{
			if( bFirst == TRUE )
			{
				/// ��ʾ
				CString strPrompt;
				strPrompt.Format( "���Ƿ�Ҫ�Ӱװ�ɾ����Щ���ѣ�" );
				
				/// �ܾ�����
				if( IDCANCEL == MessageBox( strPrompt, "�װ�", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON1 ) )
				{
					return;
				}
				bFirst = FALSE;
			}
			
			USER user = m_arrFriendsInWhiteBoard.GetAt( nIndex );
			
			/// ��ɾ���û�����ɾ����Ϣ
			Sleep( 10 );
			SendUserCommandToIP( WHITEBOARD_CLIENT_KICKED, user.strIP, WHITEBOARD_CLIENT_PORT, NULL );
			
			m_arrFriendsOutOfWhiteBoard.Add( user );
			CString strText;
			strText.Format( "%s(%s)", user.strName, user.strIP );
			m_listCtrlOutOfWhiteBoard.InsertItem( m_listCtrlOutOfWhiteBoard.GetItemCount(), strText, user.nFace );
			m_listCtrlOutOfWhiteBoard.SetCheck( m_listCtrlOutOfWhiteBoard.GetItemCount() - 1, TRUE );
			m_listCtrlOutOfWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			m_arrFriendsInWhiteBoard.RemoveAt( nIndex );
			m_listCtrlInWhiteBoard.DeleteItem( nIndex );
			m_listCtrlInWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			nIndex--;
		}
	}
	bFirst = TRUE;
	
	UpdateButtons();
}

/// ������Ӻ�ɾ����ť
void CWhiteBoardServerDlg::UpdateButtons()
{
	/// ������Ӻ��ѵİ�ť״̬
	if( 0 != m_arrFriendsInWhiteBoard.GetSize() )
	{
		m_btnDelFriends.SetNormal();
	}
	else
	{
		m_btnDelFriends.EnableWindow( FALSE );
	}
	
	/// ����ɾ�����ѵİ�ť
	if( 0 != m_arrFriendsOutOfWhiteBoard.GetSize() )
	{
		m_btnAddFriends.SetNormal();
	}
	else
	{
		m_btnAddFriends.EnableWindow( FALSE );
	}
}

/// ��װ�����û�
void CWhiteBoardServerDlg::AddUserToWhiteBoard( const USER user )
{
	for( int nIndex = 0; nIndex < m_arrFriendsOutOfWhiteBoard.GetSize(); nIndex++ )
	{
		USER userNew = m_arrFriendsOutOfWhiteBoard.GetAt( nIndex );
		if( 0 == strcmp(user.strIP, userNew.strIP ) )
		{	
			/// ���°װ��б�
			m_arrFriendsInWhiteBoard.Add( userNew );
			
			CString strText;
			strText.Format( "%s(%s)", userNew.strName, userNew.strIP );
			m_listCtrlInWhiteBoard.InsertItem( m_listCtrlInWhiteBoard.GetItemCount(), strText, userNew.nFace );
			m_listCtrlInWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			m_arrFriendsOutOfWhiteBoard.RemoveAt( nIndex );
			m_listCtrlOutOfWhiteBoard.DeleteItem( nIndex );
			m_listCtrlOutOfWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			break;
		}
	}
	
	UpdateButtons();
}

/// �Ƴ��û�
void CWhiteBoardServerDlg::DelUserFromWhiteBoard( const USER user )
{	
	for( int nIndex = 0; nIndex < m_arrFriendsInWhiteBoard.GetSize(); nIndex++ )
	{
		USER userDel = m_arrFriendsInWhiteBoard.GetAt( nIndex );
		if( 0 == strcmp(user.strIP, userDel.strIP ) )
		{	
			m_arrFriendsOutOfWhiteBoard.Add( userDel );
			CString strText;
			strText.Format( "%s(%s)", userDel.strName, userDel.strIP );
			m_listCtrlOutOfWhiteBoard.InsertItem( m_listCtrlOutOfWhiteBoard.GetItemCount(), strText, userDel.nFace );
			m_listCtrlOutOfWhiteBoard.SetCheck( m_listCtrlOutOfWhiteBoard.GetItemCount() - 1, TRUE );
			m_listCtrlOutOfWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			m_arrFriendsInWhiteBoard.RemoveAt( nIndex );
			m_listCtrlInWhiteBoard.DeleteItem( nIndex );
			m_listCtrlInWhiteBoard.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			break;
		}
	}
	
	UpdateButtons();
}


/// �������ؼ���λ��
void CWhiteBoardServerDlg::SetPosition()
{
	/// �ؼ�δ����������
	if( !m_listCtrlInWhiteBoard
		|| !m_listCtrlOutOfWhiteBoard
		|| !m_btnAddFriends
		|| !m_btnDelFriends
		|| !m_staticCanvas )
	{
		return ;
	}
	CRect rect;
	GetClientRect( &rect );

	CRect rectInWB;
	CRect rectOutOfWB;
	CRect rectAddFriends;
	CRect rectDelFriends;
	CRect rectCanvas;
	
	m_listCtrlInWhiteBoard.GetWindowRect( &rectInWB );
	m_listCtrlOutOfWhiteBoard.GetWindowRect( &rectOutOfWB );
	m_btnAddFriends.GetWindowRect( &rectAddFriends );
	m_btnDelFriends.GetWindowRect( &rectDelFriends );
	m_staticCanvas.GetWindowRect( &rectCanvas );
	ScreenToClient( &rectCanvas );
	
	/// �����б��֮ǰ�ľ���
	int nLen = 30;

	/// �б��Ŀ��
	int nWidth	= 191;

	/// ����λ��
	rectInWB.right	= rect.right - 8;
	rectInWB.top	= rect.top + 8;
	rectInWB.left	= rectInWB.right - nWidth;
	rectInWB.bottom	= rectInWB.top + ( rect.Height() - 8 * 2 - nLen ) / 2;
	m_listCtrlInWhiteBoard.MoveWindow( rectInWB );
	
	rectOutOfWB.right	= rectInWB.right;
	rectOutOfWB.left	= rectInWB.left;
	rectOutOfWB.top		= rectInWB.bottom + nLen;
	rectOutOfWB.bottom	= rect.bottom - 8;
	m_listCtrlOutOfWhiteBoard.MoveWindow( rectOutOfWB );

	int nBtnWidth	= rectAddFriends.Width();
	int nBtnHeigth	= rectAddFriends.Height();

	rectAddFriends.top		= rectInWB.bottom + 3;
	rectAddFriends.left		= rectInWB.left + 50;
	rectAddFriends.right	= rectAddFriends.left + nBtnWidth;
	rectAddFriends.bottom	= rectAddFriends.top + nBtnHeigth;
	m_btnAddFriends.MoveWindow( rectAddFriends );

	rectDelFriends.right	= rectInWB.right - 50;
	rectDelFriends.left		= rectDelFriends.right - nBtnWidth;
	rectDelFriends.top		= rectInWB.bottom + 3;
	rectDelFriends.bottom	= rectDelFriends.top + nBtnHeigth;
	m_btnDelFriends.MoveWindow( rectDelFriends );

	rectCanvas.right	= rectOutOfWB.left - 8;
	rectCanvas.bottom	= rect.bottom - 8;
	m_staticCanvas.MoveWindow( rectCanvas );
}	

void CWhiteBoardServerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	SetPosition();
}

void CWhiteBoardServerDlg::OnColor() 
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

void CWhiteBoardServerDlg::OnEraser() 
{
	ResetTools();
	m_emTool = ERASER;
	m_btnEraser.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnPencil() 
{
	ResetTools();
	m_emTool = PENCIL;
	m_btnPencil.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnLine() 
{
	ResetTools();
	m_emTool = LINE;
	m_btnLine.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnRectangle() 
{
	ResetTools();
	m_emTool = RECTANGLE;
	m_btnRectangle.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnRectangleEmpty() 
{
	ResetTools();
	m_emTool = RECTANGLE_EMPTY;
	m_btnRectangelEmpty.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnEllipseEmpty() 
{
	ResetTools();
	m_emTool = ELLIPSE_EMPTY;
	m_btnElipseEmpty.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnEllipse()
{
	ResetTools();
	m_emTool = ELLIPSE;
	m_btnEllipse.EnableWindow( FALSE );
	m_staticCanvas.SetTool( m_emTool );
}

void CWhiteBoardServerDlg::OnWidth1() 
{
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_1;
	m_btnWidth1.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

void CWhiteBoardServerDlg::OnWidth2() 
{	
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_2;
	m_btnWidth2.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

void CWhiteBoardServerDlg::OnWidth3() 
{
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_3;
	m_btnWidth3.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

void CWhiteBoardServerDlg::OnWidth4() 
{	
	ResetWidth();
	m_nWidth = WHITEBOARD_WIDTH_4;
	m_btnWidth4.EnableWindow( FALSE );
	m_staticCanvas.SetWidth( m_nWidth );
}

/// �ָ����ߵ�״̬
void CWhiteBoardServerDlg::ResetTools()
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
		m_btnRectangelEmpty.SetNormal();
		break;
	case RECTANGLE:
		m_btnRectangle.SetNormal();
		break;
	case ELLIPSE_EMPTY:
		m_btnElipseEmpty.SetNormal();
		break;
	case ELLIPSE:
		m_btnEllipse.SetNormal();
		break;
	default:
		break;
	}
}

///	�ָ��߿�
void CWhiteBoardServerDlg::ResetWidth()
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

void CWhiteBoardServerDlg::OnSizing(UINT fwSide, LPRECT pRect) 
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

/// �����Լ��Ļ滭���ݸ����пͻ���
void CWhiteBoardServerDlg::SendDrawInfoToAllUser( const CPoint ptStart, const CPoint ptEnd )
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

	for( int nIndex = 0; nIndex < m_arrFriendsInWhiteBoard.GetSize(); nIndex++ )
	{
		/// ��������
		m_pWBSSocket->SendTo( pSendData, uDataLength, WHITEBOARD_CLIENT_PORT, 
			m_arrFriendsInWhiteBoard.GetAt( nIndex ).strIP );
	}

	delete [] pSendData;
}

/// ���տͻ��˵Ļ�ͼ��Ϣ
void CWhiteBoardServerDlg::ReceiveClientDrawInfo( char *pReceivedData, const USER user )
{
	/// �õ���ͼ��Ϣ
	DRAWINFO drawInfo;
	memcpy( &drawInfo, pReceivedData + sizeof( DATAPACKET ), sizeof( DRAWINFO ) );

	/// ��������ͼ
	m_staticCanvas.Draw( drawInfo );

	/// ����ͼ��Ϣ���͸��ͻ���
	for( int nIndex = 0; nIndex < m_arrFriendsInWhiteBoard.GetSize(); nIndex++ )
	{
		if( 0 != strcmp( user.strIP, m_arrFriendsInWhiteBoard.GetAt( nIndex ).strIP ) )
		{
			/// ��������
			m_pWBSSocket->SendTo( pReceivedData, sizeof( DATAPACKET ) + sizeof( DRAWINFO ), 
				WHITEBOARD_CLIENT_PORT, 
				m_arrFriendsInWhiteBoard.GetAt( nIndex ).strIP );
		}
	}
}

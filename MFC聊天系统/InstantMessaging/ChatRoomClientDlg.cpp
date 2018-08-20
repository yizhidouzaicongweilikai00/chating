// ChatRoomClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "instantmessaging.h"
#include "ChatRoomClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatRoomClientDlg dialog

#include "InstantMessagingDlg.h"
#include "ChatRoomClientSocket.h"

CChatRoomClientDlg::CChatRoomClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatRoomClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatRoomClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/// ����ͼ���б�����32*32��ͼ����СΪ16*16
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
}


void CChatRoomClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatRoomClientDlg)
	DDX_Control(pDX, IDE_SEND, m_editSend);
	DDX_Control(pDX, IDE_RECEIVED, m_editReceived);
	DDX_Control(pDX, IDC_LIST_INCHAT, m_listCtrlInChat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatRoomClientDlg, CDialog)
	//{{AFX_MSG_MAP(CChatRoomClientDlg)
	ON_BN_CLICKED(IDB_CLOSE, OnClose)
	ON_BN_CLICKED(IDB_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatRoomClientDlg message handlers

BOOL CChatRoomClientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/// ���г�ʼ������
	Init();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChatRoomClientDlg::OnOk() 
{
}

void CChatRoomClientDlg::OnCancel() 
{	
	if( IDOK == MessageBox( "Ҫ�ر���������", "�ر�", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2 ) )
	{
		if( 0 == m_arrFriendsInChat.GetSize() )
		{
			/// �ر�UDP�׽���
			m_pCRCSocket->Close();
			delete [] m_pCRCSocket;
			m_pCRCSocket = NULL;
			DestroyWindow();
			delete this;
			return;
		}
		USER user = m_arrFriendsInChat.GetAt( 0 );
		
		/// �ر������ҵ�USER
		USER userSelf;
		userSelf.bHasCamera	= m_pMainDlg->HasCamera();
		userSelf.nFace		= m_pMainDlg->GetFace();
		memcpy( userSelf.strName, m_pMainDlg->GetNickName().GetBuffer( MAXNICKNAMELENGTH + 1 ), MAXNICKNAMELENGTH + 1 );
		
		SendUserCommandToIP( CHATROOM_CLIENT_CLOSED, user.strIP, CHATROOM_SERVER_PORT, &userSelf );
		m_pMainDlg->DelCRClientFromList( user.strIP );

		/// �ر�UDP�׽���
		m_pCRCSocket->Close();
		delete [] m_pCRCSocket;
		m_pCRCSocket = NULL;
		DestroyWindow();
		delete this;
	}
}

void CChatRoomClientDlg::Init()
{
	/// ��ȡ���Ի���
	m_pMainDlg = ( CInstantMessagingDlg * )::AfxGetMainWnd();

	/// �����û��б���ͼ��
	m_listCtrlInChat.SetImageList( &m_imageList, LVSIL_SMALL );
	m_listCtrlInChat.SetExtendedStyle( m_listCtrlInChat.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
	m_listCtrlInChat.InsertColumn( 0, "" );

	/// ��ʼ��UDP�׽���
	m_pCRCSocket = new CChatRoomClientSocket( this );
	m_pCRCSocket->Create( CHATROOM_CLIENT_PORT, SOCK_DGRAM );

	/// �޶�������Ϣ���������
	m_editSend.SetLimitText( MAXDATAPACKETLENGTH - sizeof( DATAPACKET ) - sizeof( CHATROOMMESSAGEINFO ) );
}

/// ��UDP�н�������
void CChatRoomClientDlg::OnReceive()
{
	/// ��������
	CString strIP;
	UINT uPort;
	char szReceive[ MAXDATAPACKETLENGTH ];
	m_pCRCSocket->ReceiveFrom( szReceive, MAXDATAPACKETLENGTH, strIP, uPort );

	/// װ�����ݰ�
	DATAPACKET dataPacket;
	memcpy( &dataPacket, szReceive, sizeof( DATAPACKET ) );
	USER user;
	CHATROOMMESSAGEINFO CRMInfo;

	/// ����������в�ͬ�Ĵ���
	switch( dataPacket.command )
	{
	case CHATROOM_SERVER_CLOSED:							/// �����ҷ������ر�
		ServerClosed();
		break;
	case CHATROOM_CLIENT_KICKED:							/// �ͻ��˱���
		Kicked();
		break;
	case CHATROOM_CLIENT_ADDUSER:							/// ��ͻ�������û�
		memcpy( &user, szReceive + sizeof( DATAPACKET ), sizeof( USER ) );
		AddUserToChatRoom( user );
		break;
	case CHATROOM_CLIENT_DELDUSER:							/// �ӿͻ���ɾ���û�
		memcpy( &user, szReceive + sizeof( DATAPACKET ), sizeof( USER ) );
		DelUserFromChatRoom( user );
		break;
	case CHATROOM_TEXT:										/// ���յ��ı���Ϣ
		memcpy( &CRMInfo, szReceive + sizeof( DATAPACKET ), sizeof( CHATROOMMESSAGEINFO ) );
		if( 0 == strcmp( CRMInfo.szSrcIP, "" ) )
		{
			memcpy( CRMInfo.szSrcIP, strIP.GetBuffer( 16 ), 16 );
			strIP.ReleaseBuffer();
		}
		ReceiveText( CRMInfo, 
			szReceive + sizeof( DATAPACKET ) + sizeof( CHATROOMMESSAGEINFO ), 
			dataPacket.dwLength );
		break;
	default:
		break;
	}
}

/// ��ָ��IP��PORT����ָ������
void CChatRoomClientDlg::SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT uPort, const USER *pUser )
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
	m_pCRCSocket->SendTo( pSendData, uDataLength, uPort, szIP );

	delete pSendData;
}

/// ���ض�Ӧ�ķ�����IP
CString CChatRoomClientDlg::GetServerIP()
{
	CString strServerIP;
	if( 0 != m_arrFriendsInChat.GetSize() )
	{
		memcpy( strServerIP.GetBuffer( 16 ), m_arrFriendsInChat.GetAt( 0 ).strIP, 16 );
		strServerIP.ReleaseBuffer( -1 );
	}
	return strServerIP;
}

/// ��ӷ��������Լ����û��б��
void CChatRoomClientDlg::AcceptAddRequest(  const USER userServer )
{
	/// �Լ���USER
	USER userSelf;
	userSelf.bHasCamera	= m_pMainDlg->HasCamera();
	userSelf.nFace		= m_pMainDlg->GetFace();
	memcpy( userSelf.strName, m_pMainDlg->GetNickName().GetBuffer( MAXNICKNAMELENGTH + 1 ), MAXNICKNAMELENGTH + 1 );

	CString strText;
	strText.Format( "%s(������)", userServer.strName );
	m_listCtrlInChat.InsertItem( 0, strText, userServer.nFace );
	m_listCtrlInChat.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_arrFriendsInChat.Add( userServer );

	strText.Format( "%s(�Լ�)", userSelf.strName );
	m_listCtrlInChat.InsertItem( 1, strText, userSelf.nFace );
	m_listCtrlInChat.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_arrFriendsInChat.Add( userSelf );

	/// �����������������
	SendUserCommandToIP(CHATROOM_ADDREQUEST_ALLOW, userServer.strIP, CHATROOM_SERVER_PORT, &userSelf );
}

/// ���߳�������
void CChatRoomClientDlg::Kicked()
{
	/// �ر�UDP�׽���
	m_pCRCSocket->Close();

	/// �ӿͻ����б���ɾ��
	USER user = m_arrFriendsInChat.GetAt( 0 );
	m_pMainDlg->DelCRClientFromList( user.strIP );

	/// ����û��б��
	m_arrFriendsInChat.RemoveAll();
	m_listCtrlInChat.DeleteAllItems();

	/// �رհ�ť�ͱ༭�����
	GetDlgItem( IDB_SEND )->EnableWindow( FALSE );
	GetDlgItem( IDE_SEND )->EnableWindow( FALSE );

	/// ��ʾ
	MessageBox( "�㱻�������߳����������ң�", "������" );
}

/// �������ر�
void CChatRoomClientDlg::ServerClosed()
{
	/// �ر�UDP�׽���
	m_pCRCSocket->Close();

	/// �ӿͻ����б���ɾ��
	USER user = m_arrFriendsInChat.GetAt( 0 );
	m_pMainDlg->DelCRClientFromList( user.strIP );

	/// ����û��б��
	m_arrFriendsInChat.RemoveAll();
	m_listCtrlInChat.DeleteAllItems();

	/// �رհ�ť�ͱ༭�����
	GetDlgItem( IDB_SEND )->EnableWindow( FALSE );
	GetDlgItem( IDE_SEND )->EnableWindow( FALSE );

	/// ��ʾ
	MessageBox( "�����߹ر��������ң�", "������" );
}

/// ������������û�
void CChatRoomClientDlg::AddUserToChatRoom( const USER user )
{
	/// ���½��ձ༭��
	CString strDelText;
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format( "%H:%M:%S" );
	strDelText.Format( "%s(%s) %s �����������ң�\r\n\r\n", 
		user.strName, user.strIP, strTime.GetBuffer( 10 ) );
	strTime.ReleaseBuffer( -1 );
	CString strReceiveText;
	m_editReceived.GetWindowText( strReceiveText );
	m_editReceived.SetWindowText( strReceiveText + strDelText );
	m_editReceived.LineScroll( m_editReceived.GetLineCount() );

	m_arrFriendsInChat.Add( user );
	CString strText;
	strText.Format( "%s(%s)", user.strName, user.strIP );
	m_listCtrlInChat.InsertItem( m_listCtrlInChat.GetItemCount(), strText, user.nFace );
	m_listCtrlInChat.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
}

/// �Ƴ��û�
void CChatRoomClientDlg::DelUserFromChatRoom( const USER user )
{
	for( int nIndex = 2; nIndex < m_listCtrlInChat.GetItemCount(); nIndex++ )
	{
		USER userDel = m_arrFriendsInChat.GetAt( nIndex );
		if( 0 == strcmp( user.strIP, userDel.strIP ) )
		{
			/// ���½��ձ༭��
			CString strDelText;
			CTime time = CTime::GetCurrentTime();
			CString strTime = time.Format( "%H:%M:%S" );
			strDelText.Format( "%s(%s) %s �뿪�������ң�\r\n\r\n", 
				userDel.strName, userDel.strIP, strTime.GetBuffer( 10 ) );
			strTime.ReleaseBuffer( -1 );
			CString strReceiveText;
			m_editReceived.GetWindowText( strReceiveText );
			m_editReceived.SetWindowText( strReceiveText + strDelText );
			m_editReceived.LineScroll( m_editReceived.GetLineCount() );

			m_arrFriendsInChat.RemoveAt( nIndex );
			m_listCtrlInChat.DeleteItem( nIndex );
			m_listCtrlInChat.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
			break;
		}
	}
}

void CChatRoomClientDlg::OnClose() 
{
	OnCancel();
}

/// ��ָ��IP��PORT������Ϣ����
void CChatRoomClientDlg::SendTextToIP( LPCSTR szDesIP, UINT uPort, CString strSendText )
{
	DATAPACKET dataPacket;
	dataPacket.command	= CHATROOM_TEXT;
	dataPacket.dwLength = strSendText.GetLength();
	
	CHATROOMMESSAGEINFO chatRoomMessage;
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format( "%H:%M:%S" );
	memcpy( chatRoomMessage.szTime, strTime.GetBuffer( 10 ), 10 );
	strTime.ReleaseBuffer( -1 );

	/// ����ռ�
	UINT uDataLength = sizeof( DATAPACKET ) + sizeof( CHATROOMMESSAGEINFO ) + strSendText.GetLength();
	BYTE *pSendData = new BYTE[ uDataLength ];
	memcpy( pSendData, &dataPacket, sizeof( DATAPACKET ) );
	memcpy( pSendData + sizeof( DATAPACKET ), &chatRoomMessage, sizeof( CHATROOMMESSAGEINFO ) );
	memcpy( pSendData + sizeof( DATAPACKET ) + sizeof( CHATROOMMESSAGEINFO ), 
		strSendText.GetBuffer( strSendText.GetLength() ), strSendText.GetLength() );
	strSendText.ReleaseBuffer( -1 );
	
	/// ��������
	m_pCRCSocket->SendTo( pSendData, uDataLength, uPort, szDesIP );

	delete pSendData;
}


void CChatRoomClientDlg::OnSend() 
{
	CString strSend;
	m_editSend.GetWindowText( strSend );
	if( strSend.IsEmpty() )
	{
		return;
	}
	
	/// ���½��ձ༭��
	CString strText;
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format( "%H:%M:%S" );
	strText.Format( "%s %s\r\n  %s\r\n\r\n", 
		m_pMainDlg->GetNickName().GetBuffer( MAXNICKNAMELENGTH ), 
		strTime.GetBuffer( 10 ),
		strSend.GetBuffer( strSend.GetLength() ) );
	strSend.ReleaseBuffer( -1 );
	strTime.ReleaseBuffer( -1 );
	CString strReceiveText;
	m_editReceived.GetWindowText( strReceiveText );
	m_editReceived.SetWindowText( strReceiveText + strText );
	m_editReceived.LineScroll( m_editReceived.GetLineCount() );

	m_editSend.SetWindowText( "" );
	m_editSend.SetFocus();
	
	/// �������������Ϣ
	USER user = m_arrFriendsInChat.GetAt( 0 );
	SendTextToIP( user.strIP, CHATROOM_SERVER_PORT, strSend );
}

/// �����ı���Ϣ
void CChatRoomClientDlg::ReceiveText( CHATROOMMESSAGEINFO CRMInfo,char *pData, DWORD dwTextLength )
{
	pData[ dwTextLength ] = 0;
	for( int nIndex = 0; nIndex < m_arrFriendsInChat.GetSize(); nIndex++ )
	{
		USER userSrc = m_arrFriendsInChat.GetAt( nIndex );
		if( 0 == strcmp( userSrc.strIP, CRMInfo.szSrcIP ) )
		{
			CString strText;
			strText.Format( "%s(%s) %s\r\n  %s\r\n\r\n",
				userSrc.strName,
				userSrc.strIP,
				CRMInfo.szTime,
				pData );
			CString strReceiveText;
			m_editReceived.GetWindowText( strReceiveText );
			m_editReceived.SetWindowText( strReceiveText + strText );
			m_editReceived.LineScroll( m_editReceived.GetLineCount() );
			if( 0 != strcmp( CRMInfo.szSrcIP, userSrc.strIP ) )
			{
				FlashWindow( TRUE );
			}
			break;
		}
	}	
}

BOOL CChatRoomClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	/// ��Ctrl+Enter������Ϣ
	if( pMsg->hwnd == m_editSend.m_hWnd && pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )       
		{                         
        case VK_RETURN:                                 //�س���Ϣ 
			if( ::GetKeyState(VK_CONTROL) < 0 )			//�ж��Ƿ�����Control
			{
				OnSend();
				return TRUE;
			}
			break;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

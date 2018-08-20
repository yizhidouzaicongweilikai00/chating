// ChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "instantmessaging.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

#include "InstantMessagingDlg.h"

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_RICHTEXT_RECEIVED, m_rtReceived);
	DDX_Control(pDX, IDC_RICHTEXT_SEND, m_rtSend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDB_CLOSE, OnClose)
	ON_BN_CLICKED(IDB_SEND, OnSend)
	ON_BN_CLICKED(IDB_LOADFILE, OnLoadfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers

BOOL CChatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/// ��ʼ������
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChatDlg::OnOk() 
{
}

/// ��ʼ������
void CChatDlg::Init()
{
	/// ��ȡ���Ի���
	m_pMainDlg = ( CInstantMessagingDlg * )::AfxGetMainWnd();

	/// ���÷��ͱ༭�����󳤶�
	m_rtSend.SetMaxLength( MAXCHATLENGTH );
}

void CChatDlg::OnCancel() 
{	
	/// �����Ի���������б���ɾ��
	m_pMainDlg->OnCloseChatDlg( m_userChat.strIP );

	/// �Ƿ񱣴������¼
	if( ( ( CButton * )GetDlgItem( IDC_SAVEFILE ) )->GetCheck() )
	{
		VARIANT v; 
		memset( &v, 0, sizeof( v ) );
		v.vt	= VT_I4;           
		v.iVal	= 0;
		CString strPath;
		strPath = m_strFilePath.Left( m_strFilePath.ReverseFind( '\\' ) ) + "\\";
		CFileFind fileFind;

		/// û��HISTORY�ļ���
		if( !fileFind.FindFile( strPath ) )
		{
			::CreateDirectory( strPath, NULL );
		} 
		fileFind.Close();
		m_rtReceived.SaveFile( m_strFilePath, v );
	}
	CDialog::OnCancel();

	delete this;
}


/// ��������Ķ���
void CChatDlg::SetChatUser( const USER userChat )
{
	memcpy( &m_userChat, &userChat, sizeof( USER ) );
	
	/// ���ñ���
	CString strTitle;
	strTitle.Format( "���� - %s", m_userChat.strName );
	SetWindowText( strTitle );

	/// ���öԻ���
	GetDlgItem( IDS_NICKNAME )->SetWindowText( m_userChat.strName );
	GetDlgItem( IDS_IP )->SetWindowText( m_userChat.strIP );

	m_bmpFace.DeleteObject();
	m_bmpFace.LoadBitmap( IDB_BITMAP1 + m_userChat.nFace );
	( ( CStatic * )GetDlgItem( IDS_FACE ) )->SetBitmap( ( HBITMAP )m_bmpFace );

	/// ���ñ��������¼���ļ�·��
	char path[ MAX_PATH ];
	::GetModuleFileName( NULL, path, MAX_PATH );
	CString strTmp( path );
	m_strFilePath = strTmp.Left( strTmp.ReverseFind( '\\' ) + 1 );
	m_strFilePath += HISTORYPATH;
	CTime time = CTime::GetCurrentTime();
	CString file;
	file.Format( "%s(%s)%s.rtf", 
		time.Format( "%Y-%m-%d.%H%M%S" ).GetBuffer(time.Format( "%Y-%m-%d.%H%M%S" ).GetLength() ),
		m_userChat.strIP,
		m_userChat.strName );
	m_strFilePath += "\\" + file;
}


void CChatDlg::OnClose() 
{
	OnCancel();
}

void CChatDlg::OnLoadfile() 
{
	CFileDialog fileDlg( TRUE, ".rtf", NULL, NULL, "*.rtf|*.rtf||", NULL );
	if( IDOK == fileDlg.DoModal() )
	{
		CString strPath = fileDlg.GetFileName();
		VARIANT v; 
		memset( &v, 0, sizeof( v ) );
		v.vt	= VT_I4;           
		v.iVal	= 0;
		m_rtReceived.LoadFile( strPath, v );
	}
}

BOOL CChatDlg::PreTranslateMessage(MSG* pMsg) 
{
	/// ��Ctrl+Enter������Ϣ
	if( pMsg->hwnd == m_rtSend.m_hWnd && pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )       
		{                         
        case VK_RETURN:                                 //�س���Ϣ 
			if( ::GetKeyState(VK_CONTROL) < 0 )			//�ж��Ƿ�����Control
			{
				OnSend();
				return TRUE;
			}
			else
			{
				/// ���س���ʵ�ֻس�Ч��
				m_rtSend.SetSelText( "\r\n" );
			}
			break;
		default:
			break;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}


void CChatDlg::OnSend() 
{
	/// ���������ϢΪ��
	if( 153 == m_rtSend.GetTextRTF().GetLength() )
	{
		return ;
	}
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format( "%H:%M:%S" );

	CString strText;
	strText.Format( "%s %s\r\n  ",m_pMainDlg->GetNickName(), strTime );

	/// ���ý��տ�
	CString strSend = m_rtSend.GetTextRTF();
	if( strSend.GetLength() > 555000 )
	{
		return;
	}

	m_rtReceived.SetSelStart( m_rtReceived.GetTextRTF().GetLength() );
	m_rtReceived.SetSelText( strText );
	m_rtReceived.SetSelRTF( strSend );
	m_rtReceived.SetSelStart( m_rtReceived.GetTextRTF().GetLength() );
	strText.Format( "\r\n" );
	m_rtReceived.SetSelText( strText );
	m_rtReceived.SetSelStart( m_rtReceived.GetTextRTF().GetLength() );

	/// ���÷��Ϳ�
	m_rtSend.SetText( "" );
	m_rtSend.SetFocus();

	/// ����������Ϣ
	m_pMainDlg->SendPreChatMessage( m_userChat, strTime, strSend );
}

/// ������Ϣ
void CChatDlg::ReceiveMessage( LPCSTR szTime, LPCSTR szMessage )
{
	CString strText;
	strText.Format( "%s(%s) %s\r\n  ", 
		m_userChat.strName,
		m_userChat.strIP,
		szTime );

	/// ���ý��տ�
	m_rtReceived.SetSelStart( m_rtReceived.GetTextRTF().GetLength() );
	m_rtReceived.SetSelText( strText );
	m_rtReceived.SetSelRTF( szMessage );
	m_rtReceived.SetSelStart( m_rtReceived.GetTextRTF().GetLength() );
	strText.Format( "\r\n" );
	m_rtReceived.SetSelText( strText );
	m_rtReceived.SetSelStart( m_rtReceived.GetTextRTF().GetLength() );
}

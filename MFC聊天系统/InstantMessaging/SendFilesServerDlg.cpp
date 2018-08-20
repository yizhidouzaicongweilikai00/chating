// SendFilesServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "instantmessaging.h"
#include "SendFilesServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendFilesServerDlg dialog

#include "SendFilesServerThread.h"
#include "InstantMessagingDlg.h"
#include "SendFilesServerSocket.h"
#include "SendFilesSocket.h"
#include "SendFilesReceiveUsersDlg.h"

CSendFilesServerDlg::CSendFilesServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendFilesServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendFilesServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSendFilesServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendFilesServerDlg)
	DDX_Control(pDX, IDC_LIST_SEND, m_listCtrlSend);
	DDX_Control(pDX, IDB_DELETE, m_btnDelete);
	DDX_Control(pDX, IDB_ADD, m_btnAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendFilesServerDlg, CDialog)
	//{{AFX_MSG_MAP(CSendFilesServerDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDB_ADD, OnAdd)
	ON_BN_CLICKED(IDB_DELETE, OnDelete)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendFilesServerDlg message handlers

BOOL CSendFilesServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/// ��ʼ��
	Init();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSendFilesServerDlg::OnOk() 
{
}

void CSendFilesServerDlg::OnCancel() 
{	
	ShowWindow( SW_HIDE );
}

/// ����Ҫ���յ��û�
void CSendFilesServerDlg::SetReceivedUser( CArray< USER, USER > &arrUserSend )
{
	m_arrUserSend.Copy( arrUserSend );
}

/// ��ʼ��
void CSendFilesServerDlg::Init()
{
	/// ��ȡ���Ի���
	m_pMainDlg = ( CInstantMessagingDlg * )::AfxGetMainWnd();

	/// ��ʼ�������ļ��б��
	m_listCtrlSend.SetExtendedStyle( m_listCtrlSend.GetExtendedStyle() | 
		LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	m_listCtrlSend.InsertColumn( 0, "�ļ�", LVCFMT_LEFT, 275 );
	m_listCtrlSend.InsertColumn( 1, "��С", LVCFMT_LEFT, 70 );
	m_listCtrlSend.InsertColumn( 2, "�����", LVCFMT_LEFT, 70 );
	m_listCtrlSend.InsertColumn( 3, "�ٶ�", LVCFMT_LEFT, 80 );
	m_listCtrlSend.InsertColumn( 4, "����", LVCFMT_LEFT, 40 );
	m_listCtrlSend.InsertColumn( 5, "������", LVCFMT_LEFT, 130 );
	
	/// ��������socket
	m_pSFServerSocket = new CSendFilesServerSocket( this );
	m_pSFServerSocket->Create( SENDFILESSERVER_PORT );
	m_pSFServerSocket->Listen();

	/// ��ȡʱ��
	m_ulTime = ::GetTickCount();

	/// ����ˢ���ļ��б��Ķ�ʱ��
	SetTimer( TIMER_SENDFILES_REFRESH, 1000, NULL );
}

void CSendFilesServerDlg::OnAdd() 
{
	CFileDialog fileDlg( TRUE, "*.*", NULL, NULL, "�����ļ� *.*|*.*||", NULL );
	fileDlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	if( IDCANCEL == fileDlg.DoModal() )
	{
		return;
	}
	m_arrSendFiles.RemoveAll();
	POSITION pos = fileDlg.GetStartPosition();
	while( NULL != pos )
	{
		CString strPath = fileDlg.GetNextPathName( pos );
		m_arrSendFiles.Add( strPath );
	}
	
	m_arrUserSend.RemoveAll();

	/// ѡ��Ҫ���ͺ���
	CSendFilesReceiveUsersDlg sfruDlg;
	sfruDlg.SetSFSDlg( this );
	if( IDCANCEL == sfruDlg.DoModal() )
	{
		return;
	}

	/// û��ѡ���͵��û���ֱ�ӷ���
	if( m_arrUserSend.GetSize() == 0 )
	{
		return;
	}
	
	/// ��������Ϣ��ӽ��б�򲢷���֪ͨ�������û�
	for( int nIndexFiles = 0; nIndexFiles < m_arrSendFiles.GetSize(); nIndexFiles++ )
	{
		for( int nIndexUsers = 0; nIndexUsers < m_arrUserSend.GetSize(); nIndexUsers++ )
		{
			for( int nIndex = 0; nIndex < m_listCtrlSend.GetItemCount(); nIndex++ )
			{
				CString strFilePathTmp	= m_listCtrlSend.GetItemText( nIndex, 0 );
				CString strReceiveIP	= m_listCtrlSend.GetItemText( nIndex, 5 );
				strReceiveIP			= strReceiveIP.Right( strReceiveIP.GetLength() - 1 
					- strReceiveIP.ReverseFind( '(' ) );
				strReceiveIP			= strReceiveIP.Left( strReceiveIP.GetLength() - 1 );

				if( 0 == strcmp( strFilePathTmp, m_arrSendFiles.GetAt( nIndexFiles ) ) 
					&& 0 == strcmp( strReceiveIP, m_arrUserSend.GetAt( nIndexUsers ).strIP ) )
				{
					break;
				}
			}

			/// �Ѿ�������
			if( nIndex != m_listCtrlSend.GetItemCount() )
			{
				continue;	
			}

			CFile file( m_arrSendFiles.GetAt( nIndexFiles ), CFile::typeBinary | CFile::modeRead);
			DWORD dwLength = file.GetLength();
			file.Close();


			CString strReceiveUser;
			strReceiveUser.Format( "%s(%s)",
				m_arrUserSend.GetAt( nIndexUsers ).strName,
				m_arrUserSend.GetAt( nIndexUsers ).strIP );

			m_listCtrlSend.InsertItem( m_listCtrlSend.GetItemCount(), m_arrSendFiles.GetAt( nIndexFiles ), 0 );

			CString strLength;
			if( dwLength > pow( 1024, 3 ) )
			{
				float fLength = dwLength / pow( 1024, 3 );
				strLength.Format( "%.2fGB", fLength );
			}
			else if( dwLength > pow( 1024, 2 ) )
			{
				float fLength = dwLength / pow( 1024, 2 );
				strLength.Format( "%.2fMB", fLength );
			}
			else if( dwLength > 1024 )
			{
				float fLength = dwLength / 1024;
				strLength.Format( "%.2fKB", fLength );
			}
			else
			{
				strLength.Format( "%dB", dwLength );
			}

			m_listCtrlSend.SetItemText( m_listCtrlSend.GetItemCount() - 1, 1, strLength );
			m_listCtrlSend.SetItemText( m_listCtrlSend.GetItemCount() - 1, 2, "0" );
			m_listCtrlSend.SetItemText( m_listCtrlSend.GetItemCount() - 1, 3, "?" );
			m_listCtrlSend.SetItemText( m_listCtrlSend.GetItemCount() - 1, 4, "0%" );
			m_listCtrlSend.SetItemText( m_listCtrlSend.GetItemCount() - 1, 5, strReceiveUser );

			/// �����ļ�ǰ֪ͨ�Է�
			m_pMainDlg->SendFilesNotify( m_arrUserSend.GetAt( nIndexUsers ).strIP, 
				m_arrSendFiles.GetAt( nIndexFiles ), dwLength );
		}
	}
}

/// ��������
void CSendFilesServerDlg::OnAccept()
{
	CSendFilesSocket sfSocket;
	m_pSFServerSocket->Accept( sfSocket );

	CSendFilesServerThread *pSFSThread = ( CSendFilesServerThread * )AfxBeginThread( RUNTIME_CLASS( CSendFilesServerThread ),
		0,
		0,
		CREATE_SUSPENDED,
		NULL );
	
	pSFSThread->SetSendFilesServerDlg( this );
	pSFSThread->AttachSocket( sfSocket.Detach() );
	pSFSThread->ResumeThread();

	m_arrSendThread.Add( pSFSThread );
}

	/// �ܾ������ļ�
void CSendFilesServerDlg::DenyReceiveFile( CString strFilePath, LPCSTR szIP )
{
	for( int nIndex = 0; nIndex < m_listCtrlSend.GetItemCount(); nIndex++ )
	{
		CString strFilePathTmp	= m_listCtrlSend.GetItemText( nIndex, 0 );
		CString strReceiveIP	= m_listCtrlSend.GetItemText( nIndex, 5 );
		strReceiveIP			= strReceiveIP.Right( strReceiveIP.GetLength() 
			- 1 
			- strReceiveIP.ReverseFind( '(' ) );
		strReceiveIP			= strReceiveIP.Left( strReceiveIP.GetLength() - 1 );
		CString strSpeed		= m_listCtrlSend.GetItemText( nIndex, 3 );
		if( strFilePathTmp == strFilePath && strReceiveIP == szIP && strSpeed != "�ܾ�" )
		{
			m_listCtrlSend.SetItemText( nIndex, 3, "�ܾ�" );
			break;
		}
	}
}

void CSendFilesServerDlg::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
	case TIMER_SENDFILES_REFRESH:								/// �����ļ��б��
		RefreshListBox();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

/// �����б��
void CSendFilesServerDlg::RefreshListBox()
{
	unsigned long ulTime	= ::GetTickCount();
	unsigned long ulErased	= ulTime - m_ulTime;
	m_ulTime = ulTime;

	for( int nThreadIndex = 0; nThreadIndex < m_arrSendThread.GetSize(); nThreadIndex++ )
	{
		CSendFilesServerThread * pSFCThread = ( CSendFilesServerThread * )m_arrSendThread.GetAt( nThreadIndex );
		CString strFilePath = pSFCThread->GetFilePath();
		for( int nListFileIndex = 0; nListFileIndex < m_listCtrlSend.GetItemCount(); nListFileIndex++ )
		{
			CString strFilePathTmp	= m_listCtrlSend.GetItemText( nListFileIndex, 0 );
			CString strReceiveIP	= m_listCtrlSend.GetItemText( nListFileIndex, 5 );
			strReceiveIP			= strReceiveIP.Right( strReceiveIP.GetLength() 
				- 1 
				- strReceiveIP.ReverseFind( '(' ) );
			strReceiveIP			= strReceiveIP.Left( strReceiveIP.GetLength() - 1 );
			CString strSpeed		= m_listCtrlSend.GetItemText( nListFileIndex, 3 );

			if( strFilePathTmp == strFilePath && strReceiveIP == pSFCThread->GetIP() && strSpeed != "�ܾ�" )
			{	
				/// ���߳�ֹͣ�����ļ�
				if( pSFCThread->GetStop() )
				{
					m_listCtrlSend.SetItemText( nListFileIndex, 3, "ֹͣ" );
					
					/// ���߳��б���ɾ��
					m_arrSendThread.RemoveAt( nThreadIndex );
					break;
				}

				/// ���´��е������
				CString strSended;
				DWORD dwSended = pSFCThread->GetSended();
				if( dwSended > pow( 1024, 3 ) )
				{
					float fSended = dwSended / pow( 1024, 3 );
					strSended.Format( "%.2fGB", fSended );
				}
				else if( dwSended > pow( 1024, 2 ) )
				{
					float fSended = dwSended / pow( 1024, 2 );
					strSended.Format( "%.2fMB", fSended );
				}
				else if( dwSended > 1024 )
				{
					float fSended = dwSended / 1024;
					strSended.Format( "%.2fKB", fSended );
				}
				else
				{
					strSended.Format( "%dB", dwSended );
				}
				m_listCtrlSend.SetItemText( nListFileIndex, 2, strSended );

				/// �����ٶ�
				DWORD dwSendCount	= pSFCThread->GetSendCount();
				float fSpeed		= dwSendCount * 1000 / ulErased;
				CString strSpeed;
				if( fSpeed > pow( 1024, 2 ) )
				{
					float fSpeed1 = fSpeed / pow( 1024, 2 );
					strSended.Format( "%.2fMB/S", fSpeed1 );
				}
				else if( fSpeed > 1024 )
				{
					float fSpeed1 = fSpeed / 1024;
					strSended.Format( "%.2fKB/S", fSpeed1 );
				}
				else
				{
					strSended.Format( "%.2fB/S", fSpeed );
				}
				m_listCtrlSend.SetItemText( nListFileIndex, 3, strSended );

				/// ���½���
				CString strPercent;
				int nPercent = 100;
				if( 0 != pSFCThread->GetLength() / 100 )
				{
					nPercent = 100 * ( pSFCThread->GetSended() / 100 ) / ( pSFCThread->GetLength() / 100 ) ;
				}
				strPercent.Format( "%d%%", nPercent );
				m_listCtrlSend.SetItemText( nListFileIndex, 4, strPercent );

				/// ����ɴ���
				if( pSFCThread->GetSended() == pSFCThread->GetLength() )
				{
					m_listCtrlSend.SetItemText( nListFileIndex, 3, "���" );
					m_listCtrlSend.SetItemText( nListFileIndex, 4, "100%" );

					/// ���߳��б���ɾ��
					m_arrSendThread.RemoveAt( nThreadIndex );
				}

				break;
			}
		}
	}
}

void CSendFilesServerDlg::OnDelete() 
{
	/// ��ʾ
	if( IDCANCEL == MessageBox( "��ȷ��Ҫֹͣ������Щ�ļ���", "ֹͣ����", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2 ) )
	{
		return;
	}

	for( int nListFileIndex = 0; nListFileIndex < m_listCtrlSend.GetItemCount(); nListFileIndex++ )
	{
		/// û��ѡ������
		if( !m_listCtrlSend.GetCheck( nListFileIndex ) )
		{
			continue;
		}

		/// �ܾ����յ�ֱ�Ӵ��б���ɾ��
		if( m_listCtrlSend.GetItemText( nListFileIndex, 3 ) == "�ܾ�" 
			|| m_listCtrlSend.GetItemText( nListFileIndex, 3 ) == "���"
			|| m_listCtrlSend.GetItemText( nListFileIndex, 3 ) == "ֹͣ")
		{
			m_listCtrlSend.DeleteItem( nListFileIndex-- );
			continue;
		}

		/// ������ѡ�����ڴ�����߳�
		for( int nThreadIndex = 0; nThreadIndex < m_arrSendThread.GetSize(); nThreadIndex++ )
		{
			CSendFilesServerThread * pSFCThread = ( CSendFilesServerThread * )m_arrSendThread.GetAt( nThreadIndex );
			CString strFilePath = pSFCThread->GetFilePath();
			
			CString strFilePathTmp	= m_listCtrlSend.GetItemText( nListFileIndex, 0 );
			CString strReceiveIP	= m_listCtrlSend.GetItemText( nListFileIndex, 5 );
			strReceiveIP			= strReceiveIP.Right( strReceiveIP.GetLength() 
				- 1 
				- strReceiveIP.ReverseFind( '(' ) );
			strReceiveIP			= strReceiveIP.Left( strReceiveIP.GetLength() - 1 );
			
			/// �߳����б��е������
			if( strFilePathTmp == strFilePath && strReceiveIP == pSFCThread->GetIP() )
			{	
				pSFCThread->StopSend();
				m_arrSendThread.RemoveAt( nThreadIndex-- );
				m_listCtrlSend.DeleteItem( nListFileIndex-- );
				break;
			} 
		}
	}
}

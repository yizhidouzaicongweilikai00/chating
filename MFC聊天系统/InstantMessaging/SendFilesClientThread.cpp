// SendFilesClientThread.cpp : implementation file
//

#include "stdafx.h"
#include "instantmessaging.h"
#include "SendFilesClientThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendFilesClientThread

IMPLEMENT_DYNCREATE(CSendFilesClientThread, CWinThread)

CSendFilesClientThread::CSendFilesClientThread()
{
	m_bStop				= FALSE;
	m_bRun				= TRUE;
	m_dwReceiveCount	= 0;
}

CSendFilesClientThread::~CSendFilesClientThread()
{
}

BOOL CSendFilesClientThread::InitInstance()
{
	/// ������socket��Ӧ���߳�	
	m_receiveFilesSocket.SetSendFileClientThread( this );

	/// �����ļ����״��жϣ������½��ջ��Ǽ�������
	CFileFind fileFind;
	
	/// �����ļ����ڣ���ȡ��Ϣ
	/// �����ļ����ݣ�MAX_PATH������·����+DWORD�����ȣ�+DWORD���ѽ��ճ��ȣ�
	if( fileFind.FindFile( m_strConfig ) )
	{
		CFile fileCfg( m_strConfig, CFile::modeRead | CFile::typeBinary );
		char szData[ MAX_PATH + 2 * sizeof( DWORD ) ];
		fileCfg.Read( szData, MAX_PATH + 2 * sizeof( DWORD ) );
		fileCfg.Close();

		/// ��ȡ��Ϣ
		CString strDesPath;
		DWORD dwLength;
		DWORD dwReceived;
		memcpy( strDesPath.GetBuffer( MAX_PATH ), szData, MAX_PATH );
		strDesPath.ReleaseBuffer( -1 );
		memcpy( &dwLength, szData + MAX_PATH, sizeof( DWORD ) );
		memcpy( &dwReceived, szData + MAX_PATH + sizeof( DWORD ), sizeof( DWORD ) );

		/// �����ѽ����ļ��Ƿ���ڣ����Ƿ�����ѽ��յ��ĳ���
		CFileFind fileFind1;
		if( fileFind1.FindFile( m_strDesPath ) 
			&& m_dwLength == dwLength 
			&& m_strDesPath == strDesPath )
		{
			CFile file( m_strDesPath, CFile::modeRead | CFile::typeBinary );
			
			/// �ļ��ĳ��ȴ��ڻ���������ļ��н��յ����ļ�����
			if( file.GetLength() >= dwReceived )
			{
				m_dwReceived = dwReceived;
			}
			file.Close();
		}
		fileFind1.Close();
	}
	fileFind.Close();

	/// ���ļ������ó��ȣ���λ��д��λ��
	m_fileSave.Open( m_strDesPath, CFile::modeWrite | CFile::typeBinary | CFile::modeNoTruncate | CFile::modeCreate );
	m_fileSave.SetLength( m_dwReceived );
	m_fileSave.Seek( m_dwReceived, CFile::begin );

	/// �ҿ������ļ�
	m_fileCfg.Open( m_strConfig, CFile::modeWrite | CFile::typeBinary | CFile::modeNoTruncate | CFile::modeCreate );

	/// ����SENDFILES_FILEINFO��������
	DATAPACKET dataPacket;
	dataPacket.command = SENDFILES_FILEINFO;
	UINT uDataLength = sizeof( DATAPACKET ) + MAX_PATH + 2 * sizeof( DWORD );
	BYTE *pSendData = new BYTE[ uDataLength ];
	memcpy( pSendData, &dataPacket, sizeof( DATAPACKET ) );
	memcpy( pSendData + sizeof( DATAPACKET ), m_strSourcePath.GetBuffer( MAX_PATH ), MAX_PATH );
	m_strSourcePath.ReleaseBuffer( -1 );
	memcpy( pSendData + sizeof( DATAPACKET ) + MAX_PATH, &m_dwLength, sizeof( DWORD ) );
	memcpy( pSendData + sizeof( DATAPACKET ) + MAX_PATH + sizeof( DWORD ), &m_dwReceived, sizeof( DWORD ) );
	m_receiveFilesSocket.Send( pSendData, uDataLength );
	delete pSendData;

	while( m_bRun )
	{
		Sleep( 1 );
	}
	return TRUE;
}

int CSendFilesClientThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendFilesClientThread, CWinThread)
	//{{AFX_MSG_MAP(CSendFilesClientThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendFilesClientThread message handlers

/// �����׽���
void CSendFilesClientThread::AttachSocket( SOCKET hSocket )
{
	m_receiveFilesSocket.Attach( hSocket );
}

/// ������������
void CSendFilesClientThread::OnReceive()
{
	char szReceive[ MAXDATAPACKETLENGTH ];
	m_receiveFilesSocket.Receive( szReceive, MAXDATAPACKETLENGTH );

	/// װ�����ݰ�
	DATAPACKET dataPacket;
	memcpy( &dataPacket, szReceive, sizeof( DATAPACKET ) );
	
	switch( dataPacket.command )
	{
	case SENDFILES_RESPONSE:									/// ��������
		ReceiveData( szReceive );
		break;
	case SENDFILES_BEGIN:										/// ֪ͨ�����߿�ʼ���ͽ�����Ϣ
		SendReceiveMessage( m_dwReceived );
		break;
	default:
		break;
	}
}

/// ���������ļ�����Ϣ
void CSendFilesClientThread::SendReceiveMessage( DWORD dwReceived )
{
	DATAPACKET dataPacket;

	/// ����յ��ĳ������ܳ�����ȣ���������
	if( dwReceived == m_dwLength )
	{
		m_fileSave.Close();
		m_fileCfg.Close();
		CFile::Remove( m_strConfig );
		dataPacket.command = SENDFIELS_DONE;
		UINT uDataLength = sizeof( DATAPACKET );
		BYTE *pSendData = new BYTE[ uDataLength ];
		memcpy( pSendData, &dataPacket, sizeof( DATAPACKET ) );
		m_receiveFilesSocket.Send( pSendData, uDataLength );
		delete pSendData;
		m_receiveFilesSocket.Close();
		m_pDlgSendFilesClient->RefreshListBox();
		m_bRun = FALSE;
		return;
	}

	/// ��δ�������
	dataPacket.command = SENDFILES_REQUEST;
	UINT uDataLength = sizeof( DATAPACKET ) + sizeof( DWORD );
	BYTE *pSendData = new BYTE[ uDataLength ];
	memcpy( pSendData, &dataPacket, sizeof( DATAPACKET ) );
	memcpy( pSendData + sizeof( DATAPACKET ), &dwReceived, sizeof( DWORD ) );
	m_receiveFilesSocket.Send( pSendData, uDataLength );
	delete pSendData;
}

/// ��������
void CSendFilesClientThread::ReceiveData( LPCSTR szReceive )
{
	DATAPACKET dataPacket;
	memcpy( &dataPacket, szReceive, sizeof( DATAPACKET ) );

	BYTE * pCompressData	= new BYTE[ dataPacket.dwLength ];
	memcpy( pCompressData, szReceive + sizeof( DATAPACKET ), dataPacket.dwLength );
	
	m_fileSave.Write( pCompressData, dataPacket.dwLength );
	m_dwReceiveCount += dataPacket.dwLength;

	m_dwReceived += dataPacket.dwLength;

	BYTE * pData = new BYTE[ MAX_PATH + 2 * sizeof( DWORD ) ];
	memcpy( pData, m_strDesPath.GetBuffer( MAX_PATH ), MAX_PATH );
	m_strDesPath.ReleaseBuffer( -1 );
	memcpy( pData + MAX_PATH, &m_dwLength, sizeof( DWORD ) );
	memcpy( pData + MAX_PATH + sizeof( DWORD ), &m_dwReceived, sizeof( DWORD ) );

	m_fileCfg.SeekToBegin();
	m_fileCfg.Write( pData, MAX_PATH + 2 * sizeof( DWORD ) );

	delete pData;

	delete pCompressData;

	/// ���ͼ����������ݵ���Ϣ
	SendReceiveMessage( m_dwReceived );
}

/// ɾ�������߳�
void CSendFilesClientThread::StopReceive()
{
	m_receiveFilesSocket.Close();
	m_fileCfg.Close();
	m_fileSave.Close();
	m_bRun = FALSE;
}

/// �ر�����
void CSendFilesClientThread::OnClose()
{
	m_receiveFilesSocket.Close();
	m_fileCfg.Close(); 
	m_fileSave.Close();
	m_bStop = TRUE;
	m_pDlgSendFilesClient->RefreshListBox();
	m_bRun	= FALSE;
}
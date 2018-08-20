#if !defined(AFX_SENDFILESCLIENTTHREAD_H__B50835DE_3B5E_4D21_B116_E482C7130E1D__INCLUDED_)
#define AFX_SENDFILESCLIENTTHREAD_H__B50835DE_3B5E_4D21_B116_E482C7130E1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendFilesClientThread.h : header file
//

#include "ReceiveFilesSocket.h"
#include "SendFilesClientDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSendFilesClientThread thread

class CSendFilesClientThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendFilesClientThread)
protected:
	CSendFilesClientThread();           // protected constructor used by dynamic creation

// Attributes
private:
	CReceiveFilesSocket		m_receiveFilesSocket;				/// ����߳���ص�socket
	CSendFilesClientDlg		*m_pDlgSendFilesClient;				/// �����ļ��Ի���ָ��
	DWORD					m_dwLength;							/// Ҫ�����ļ����ܳ���
	DWORD					m_dwReceived;						/// �Ѿ����յĳ���
	CString					m_strSourcePath;					/// �����ļ���Դ·��
	char					m_szSoureceIP[ 16 ];				/// �����ļ���ԴIP
	CString					m_strDesPath;						/// �����·��
	CString					m_strConfig;						/// �����ļ���·��
	BOOL					m_bRun;								/// ����������еı��

	CFile					m_fileSave;							/// ������ļ���
	CFile					m_fileCfg;							/// ���õ��ļ��ࡡ

	DWORD					m_dwReceiveCount;					/// ��timer�ڽ��յĳ���

	BOOL					m_bStop;							/// �Ƿ�ֹͣ�����ļ�

// Operations
public:
	/// �����׽���
	void AttachSocket( SOCKET hSocket );

	/// ������������
	void OnReceive();

	/// �����ļ��Ի���ָ��
	void SetSendFilesClientDlg( CSendFilesClientDlg *pDlgSendFilesClient )
	{
		m_pDlgSendFilesClient = pDlgSendFilesClient;
	}

	/// ����
	void SetInfo( CString strSourcePath, CString strDesPath, DWORD dwLength, LPCSTR szSourceIP )
	{
		m_strSourcePath = strSourcePath;
		m_strDesPath	= strDesPath;
		m_strConfig		= m_strDesPath.Left( m_strDesPath.ReverseFind( '\\' ) + 1 ) 
			+ m_strDesPath.Right( m_strDesPath.GetLength() - 1 - m_strDesPath.ReverseFind( '\\' ) )
			+ "." + SENDFILES_CONFIG_SUB;
		m_dwLength		= dwLength;
		m_dwReceived	= 0;
		memcpy( m_szSoureceIP, szSourceIP, 16 );
	}

	/// ���������ļ�����Ϣ
	void SendReceiveMessage( DWORD dwReceived );

	/// ��������
	void ReceiveData( LPCSTR szReceive );
	
	/// ɾ�������߳�
	void StopReceive();

	/// �ر�����
	void OnClose();

	CString GetSourceFilePath(){ return m_strSourcePath; }
	CString GetDesFilePath(){ return m_strDesPath; }
	LPCSTR	GetIP(){ return m_szSoureceIP; }
	DWORD	GetLength(){ return m_dwLength; }
	DWORD	GetReceived(){ return m_dwReceived; }
	BOOL	GetStop(){ return m_bStop ; }
	DWORD	GetReceiveCount(){ DWORD dwReceiveCount = m_dwReceiveCount; m_dwReceiveCount = 0; return dwReceiveCount; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendFilesClientThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSendFilesClientThread();

	// Generated message map functions
	//{{AFX_MSG(CSendFilesClientThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDFILESCLIENTTHREAD_H__B50835DE_3B5E_4D21_B116_E482C7130E1D__INCLUDED_)

#if !defined(AFX_SendFilesServerThread_H__3B5C4F9F_1FE0_4882_BEA2_970801DAB869__INCLUDED_)
#define AFX_SendFilesServerThread_H__3B5C4F9F_1FE0_4882_BEA2_970801DAB869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendFilesServerThread.h : header file
//

#include "SendFilesSocket.h"
#include "SendFilesServerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSendFilesServerThread thread

class CSendFilesServerThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendFilesServerThread)
protected:
	CSendFilesServerThread();           // protected constructor used by dynamic creation

// Attributes
private:
	CSendFilesSocket		m_sendFilesSocket;				/// ����߳���ص�socket
	CSendFilesServerDlg		*m_pDlgSendFileServer;			/// �����ļ��������Ի���ָ��
	DWORD					m_dwLength;						/// Ҫ�����ļ����ܳ���
	DWORD					m_dwSended;						/// �Ѿ�����ĳ���
	CString					m_strSourcePath;				/// �����ļ���Դ·��
	char					m_szDesIP[ 16 ];				/// ���յ�IP
	BOOL					m_bRun;							/// ����������еı��

	CFile					m_fileSend;						/// �����ļ���

	DWORD					m_dwSendCount;					/// ��timer�ڷ��͵ĳ���

	BOOL					m_bStop;						/// �Ƿ�ֹͣ�����ļ�

// Operations
public:
	/// �����׽���
	void AttachSocket( SOCKET hSocket );

	/// ������������
	void OnReceive();

	/// ���÷����ļ��������Ի����ָ��
	void SetSendFilesServerDlg( CSendFilesServerDlg *pDlgSendFileServer )
	{
		m_pDlgSendFileServer = pDlgSendFileServer;
	}

	CString GetFilePath(){ return m_strSourcePath; }
	LPCSTR	GetIP(){ return m_szDesIP; }
	DWORD	GetLength(){ return m_dwLength; }
	DWORD	GetSended(){ return m_dwSended; }
	BOOL	GetStop(){ return m_bStop ; }
	DWORD	GetSendCount(){ DWORD dwSendCount = m_dwSendCount; m_dwSendCount = 0; return dwSendCount; }

	/// ɾ�������߳�
	void StopSend();

	/// �ر�����
	void OnClose();

private:
	/// ����
	void SetInfo( LPCSTR szReceive );

	/// ��������
	void SendData( LPCSTR szReceive );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendFilesServerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSendFilesServerThread();

	// Generated message map functions
	//{{AFX_MSG(CSendFilesServerThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SendFilesServerThread_H__3B5C4F9F_1FE0_4882_BEA2_970801DAB869__INCLUDED_)

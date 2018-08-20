#if !defined(AFX_SENDFILESCLIENTDLG_H__31959843_17BF_45BC_9ABF_530E453AE1B2__INCLUDED_)
#define AFX_SENDFILESCLIENTDLG_H__31959843_17BF_45BC_9ABF_530E453AE1B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendFilesClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSendFilesClientDlg dialog

class CInstantMessagingDlg;
class CReceiveFilesSocket;

class CSendFilesClientDlg : public CDialog
{
// Construction
public:
	CSendFilesClientDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSendFilesClientDlg)
	enum { IDD = IDD_SENDFILES_CLIENT };
	CListCtrl	m_listCtrlReceive;
	//}}AFX_DATA
	
private:
	/// ��ʼ������
	void Init();

public:
	/// ��ָ��IP��������
	void ReceiveFileFromIP( CString strFilePath, LPCSTR szIP, DWORD dwLength );

	/// ���ݽ�����Ϸ���Ϣ
	void ReceivedDone();

	/// �����б��
	void RefreshListBox();
	
private:
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���

	CObArray				m_arrReceiveThread;	/// �����ļ����߳��б�

	CArray< USER, USER >	m_arrUserSend;		/// Ҫ�������û�

	unsigned long			m_ulTime;			/// ��¼ʱ��


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendFilesClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendFilesClientDlg)
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDFILESCLIENTDLG_H__31959843_17BF_45BC_9ABF_530E453AE1B2__INCLUDED_)

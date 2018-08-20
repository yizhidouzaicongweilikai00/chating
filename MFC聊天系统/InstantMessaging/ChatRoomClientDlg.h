#if !defined(AFX_CHATROOMCLIENTDLG_H__FF08831A_5EAE_4394_8072_7B29D03BA943__INCLUDED_)
#define AFX_CHATROOMCLIENTDLG_H__FF08831A_5EAE_4394_8072_7B29D03BA943__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatRoomClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatRoomClientDlg dialog

class CInstantMessagingDlg;
class CChatRoomClientSocket;

class CChatRoomClientDlg : public CDialog
{
// Construction
public:
	CChatRoomClientDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChatRoomClientDlg)
	enum { IDD = IDD_CHATROOM_CLIENT };
	CEdit	m_editSend;
	CEdit	m_editReceived;
	CListCtrl	m_listCtrlInChat;
	//}}AFX_DATA

private:
	/// ��ʼ������
	void Init();

	/// ��ָ��IP��PORT����ָ���û�������
	void SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT uPort, const USER *pUser );

	/// ��ָ��IP��PORT������Ϣ����
	void SendTextToIP( LPCSTR szDesIP, UINT uPort, CString strSendText );

	/// ������������û�
	void AddUserToChatRoom( const USER user );

	/// �Ƴ��û�
	void DelUserFromChatRoom( const USER user );

	/// �������ر�
	void ServerClosed();

	/// ���߳�������
	void Kicked();

	/// �����ı���Ϣ
	void ReceiveText( CHATROOMMESSAGEINFO CRMInfo, char *pData, DWORD dwTextLength ); 

private:
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���
	CImageList				m_imageList;		/// ͷ��ͼ���б�
	CArray< USER, USER >	m_arrFriendsInChat;	/// �������еĺ����б�
	CChatRoomClientSocket	*m_pCRCSocket;		/// �����ҿͻ���socket

public:
	/// ������������
	void OnReceive();

	/// ���ض�Ӧ�ķ�����IP
	CString GetServerIP();

	/// ��ӷ��������Լ����û��б��
	void AcceptAddRequest( const USER userServer );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatRoomClientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChatRoomClientDlg)
	afx_msg void OnOk();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATROOMCLIENTDLG_H__FF08831A_5EAE_4394_8072_7B29D03BA943__INCLUDED_)

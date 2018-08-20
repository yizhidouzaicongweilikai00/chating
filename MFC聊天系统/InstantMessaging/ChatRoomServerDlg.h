#if !defined(AFX_CHATROOMSERVERDLG_H__15DBAC8F_1F81_4EB6_95F3_C543D7BEAADB__INCLUDED_)
#define AFX_CHATROOMSERVERDLG_H__15DBAC8F_1F81_4EB6_95F3_C543D7BEAADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatRoomServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatRoomServerDlg dialog

#include "AdvButton.h"

class CInstantMessagingDlg;
class CChatRoomServerSocket;

class CChatRoomServerDlg : public CDialog
{
// Construction
public:
	CChatRoomServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChatRoomServerDlg)
	enum { IDD = IDD_CHATROOM_SERVER };
	CEdit	m_editSend;
	CEdit	m_editReceived;
	CListCtrl	m_listCtrlOutOfChat;
	CListCtrl	m_listCtrlInChat;
	CAdvButton	m_btnDelFriends;
	CAdvButton	m_btnAddFriends;
	//}}AFX_DATA

private:
	/// ��ʼ������
	void Init();

	/// ��ָ��IP��PORT����ָ���û�������
	void SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT nPort, const USER *pUser );

	/// ��ָ��IP��PORT������Ϣ����
	void SendTextToIP( LPCSTR szDesIP, UINT uPort, CString strSendText, LPCSTR szTextIP );

	/// ������������û�
	void AddUserToChatRoom( const USER user );

	/// �Ƴ��û�
	void DelUserFromChatRoom( const USER user );

	/// ������Ӻ�ɾ����ť
	void UpdateButtons();

	/// �����ı���Ϣ
	void ReceiveText( CHATROOMMESSAGEINFO CRMInfo, char *pData, DWORD dwTextLength ); 

private:
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���
	CImageList				m_imageList;		/// ͷ��ͼ���б�
	CArray< USER, USER >	m_arrFriendsInChat;	/// �������еĺ����б�
	CArray< USER, USER >	m_arrFriendsOutOfChat;
												/// �����������еĺ����б�
	CChatRoomServerSocket	*m_pCRSSocket;		/// �����ҷ�����socket

public:
	/// ������������
	void OnReceive();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatRoomServerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChatRoomServerDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnColse();
	afx_msg void OnSend();
	afx_msg void OnAddfriends();
	afx_msg void OnDelfriends();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATROOMSERVERDLG_H__15DBAC8F_1F81_4EB6_95F3_C543D7BEAADB__INCLUDED_)

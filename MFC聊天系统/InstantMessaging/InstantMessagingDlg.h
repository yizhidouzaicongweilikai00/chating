// InstantMessagingDlg.h : header file
//

#if !defined(AFX_INSTANTMESSAGINGDLG_H__7D41D06C_C73A_4AE1_9284_440FC1479084__INCLUDED_)
#define AFX_INSTANTMESSAGINGDLG_H__7D41D06C_C73A_4AE1_9284_440FC1479084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInstantMessagingDlg dialog

#include "AdvButton.h"
#include "DialogSetting.h"
#include "FriendsListCtrl.h"
#include "ChatRoomServerDlg.h"
#include "ChatRoomClientDlg.h"
#include "ChatDlg.h"
#include "SendFilesServerDlg.h"
#include "SendFilesClientDlg.h"
#include "WhiteBoardServerDlg.h"
#include "WhiteBoardClientDlg.h"

class CListeningSocket;

LRESULT WINAPI EncodeCallback( HWND hWnd, LPVIDEOHDR lpVHdr );
BYTE _clip255( LONG v );
void YUY2_RGB( BYTE *YUY2buff, BYTE *RGBbuff, DWORD dwSize );

class CInstantMessagingDlg : public CDialog
{
// Construction
public:
	CInstantMessagingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInstantMessagingDlg)
	enum { IDD = IDD_INSTANTMESSAGING_DIALOG };
	CComboBox	m_comboxIP;
	CFriendsListCtrl	m_listCtrlFriends;
	CStatic	m_staticFace;
	CEdit	m_editNickName;
	CComboBox	m_comboxState;
	CComboBox	m_comboxFriend;
	CAdvButton	m_btnWhiteBoard;
	CAdvButton	m_btnSendFiles;
	CAdvButton	m_btnChatRoom;
	CAdvButton	m_btnAddFriend;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstantMessagingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON					m_hIcon;
	NOTIFYICONDATA			m_nid;				/// ��С������������ṹ��
	CBitmap					m_bmpFace;			/// ͷ��
	WORD					m_wVirtualKeyCode;	/// �ȼ���������
	WORD					m_wModifiers;		/// �ȼ��޸�ֵ
	WORD					m_wFace;			/// ͷ��
	WORD					m_wState;			/// ״̬
	CString					m_strNickName;		/// �ǳ�
	BOOL					m_bCamera;			/// ����ͷ
	BOOL					m_bAutoAddFriends;	/// �Զ���Ϊ����
	CStringList				m_strlstIP;			/// �Լ���IP�б�

	CStringList				m_strlstRequest;	/// �����Ϊ���ѵ�IP�б�

	CStringList				m_strlstKeepConnent;
												/// �������ӵ�IP�б�
	CStringList				m_strlstChatRoomRequest;	
												/// ������������ҵ�IP�б�
	CListeningSocket		*m_pLisSocket;		/// ����socket
	
	CArray< USER, USER >	m_arrFriends;		/// �����б�

	CChatRoomServerDlg		m_dlgCRServer;		/// �����ҷ�����

	CArray< CChatRoomClientDlg *, CChatRoomClientDlg * >
							m_arrCRClient;		/// �����ҵĿͻ����б�

	CArray< CChatDlg *, CChatDlg *>
							m_arrChatDlg;		/// ����Ի�����б�

	CMapStringToPtr			m_mapIPToData;		/// ����ip�����ݵ�map

	CMapStringToString		m_mapIPToTime;		/// ����ip��ʱ���map

	CMapStringToOb			m_mapIPToChat;		/// ����IP�����죨ʱ������ݣ���map

	CMapStringToString		m_mapSendChatMessage;
												/// ����������Ϣ��map

	CArray< int, int >		m_arrMessageArrive;	/// ��������Ϣ������û����

	CImageList				m_imageListMA;		/// ��������Ϣ������û�ͷ��

	int						m_nTrayShowIndex;	/// ������ʾ�����

	CSendFilesServerDlg		m_dlgSendFilesServer;
												/// �����ļ��������Ի���
	
	CSendFilesClientDlg		m_dlgSendFilesClient;
												/// �����ļ��������Ի���

	CWhiteBoardServerDlg	m_dlgWhiteBoardServer;
												/// �װ�������Ի���
	CStringList				m_strlstWhiteBoardRequest;	
												/// �������װ��IP�б�
	CWhiteBoardClientDlg	m_dlgWhiteBoardClient;
												/// �װ�ͻ��˶Ի���


	int						m_nMaxUDPDG;		/// UDP��֧�ֵ��������ֵ

	HWAVEIN					m_hWaveIn;
	PWAVEHDR				m_pWaveHdr1;
	PWAVEHDR				m_pWaveHdr2;
	char					*m_pBuffer1;
	char					*m_pBuffer2;
	WAVEFORMATEX			m_WaveForm;



public:
	CSocket					m_pSocket;			/// ��������û���socket



	
private:
	/// ��ʼ������
	void Init();

	/// ���������������Ϣ��Ӧ����
	void OnShellNotifyProc( WPARAM wParam, LPARAM lParam );

	/// �ȼ���Ӧ����
	void OnHotkey( WPARAM wParam, LPARAM lParam );

	void SetState();

	void SendOffLineMessage();

	void AddRequest( const USER user );
	void AddFriend( const USER user );
	void DeleteFriend( const USER user );
	void KeepConnect( CString strIP );

	/// ��Ӧ���������ҵ�����
	void OnAddToChatRoomRequest( const USER userServer );

	/// ��ָ��IP��PORT����ָ���û�������
	void SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT nPort, const USER *pUser );

	///	Ϊ����������Ϣ��׼��
	void PrepareChatMessage( LPCSTR szReceive, LPCSTR szIP );

	/// ����������Ϣ
	void ReceiveChatMessage( LPCSTR szReceive, LPCSTR szIP );

	/// ����ʷ��¼��ɾ��ָ����¼
	void DeleteHistory( int nIndex, const CString strHistory );

	/// ��ʼ��������
	void SendChatMessage( LPCSTR szReceive, LPCSTR szIP );

	/// ��ָ��ip���û���ͷ����뵽���̶�̬��ʾ
	void AddTrayIcon( LPCSTR szIP );

	/// ��Ӧ�Ƿ���մ�����ļ�
	void SendFilesResponse( LPCSTR szReceive, LPCSTR szIP );

	/// ��Ӧ����װ������
	void OnAddToWhiteBoardRequest( const USER userServer );



public:
	WORD GetFace(){ return m_wFace; }
	CString GetNickName(){ return m_strNickName; }
	BOOL HasCamera(){ return m_bCamera; }
	WORD GetVirtualKeyCode(){ return m_wVirtualKeyCode; }
	WORD GetModifiers(){ return m_wModifiers; }
	BOOL GetAutoAddFriends(){ return m_bAutoAddFriends; }
	int  GetMaxUDPDG(){ return m_nMaxUDPDG; }

	void SetFace( WORD wFace );
	void SetNickName( const CString strNickName );
	void SetHotKey( WORD wVirtualKeyCode, WORD wModifiers );
	void SetAutoAddFriends( BOOL bAutoAddFriends );
	void SetMaxUDPDG( int nMaxUDPDG ){ m_nMaxUDPDG = nMaxUDPDG; }

	void OnListeningReceive();
	
	void DeleteFriend( int nIndex );

	void GetFriendsArray( CArray< USER, USER > &arrFriends ){ arrFriends.Copy( m_arrFriends ); }

	void SendFriendMessage( const USER user, CONTROLCOMMAND command );

	/// ���б���ɾ�������ҿͻ���
	void DelCRClientFromList( LPCSTR szIP );

	/// �͵�nIndex����������
	void OnChat( UINT nIndex );

	/// �ر����촰��
	void OnCloseChatDlg( LPCSTR szIP );

	/// ����������Ϣ
	void SendPreChatMessage( const USER userChat, CString strTime, CString strSend );

	/// ��ָ��IP�û����ʹ����ļ���֪ͨ
	void SendFilesNotify( LPCSTR szIP, CString strFile, DWORD dwLength );

	/// �ܾ������ļ�
	void DenyReceiveFile( CString strFilePath, LPCSTR szIP );

	/// ��IP�õ�USER
	void GetUserFromIP( LPCSTR szIP, USER &user );

	/// �õ��Լ�IP�б���IP
	CString GetComboBoxIP();

	// Generated message map functions
	//{{AFX_MSG(CInstantMessagingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExit();
	afx_msg void OnSetting();
	afx_msg void OnSelchangeComboState();
	afx_msg void OnState();
	afx_msg void OnBtnAddfriend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnChatroom();
	afx_msg void OnChatroom();
	afx_msg void OnBtnSendfiles();
	afx_msg void OnSendfiles();
	afx_msg void OnReceivefiles();
	afx_msg void OnAddSection();
	afx_msg void OnBtnWhiteboard();
	afx_msg void OnWhiteboard();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTANTMESSAGINGDLG_H__7D41D06C_C73A_4AE1_9284_440FC1479084__INCLUDED_)

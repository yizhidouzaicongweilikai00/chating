#if !defined(AFX_WHITEBOARDSERVERDLG_H__A7CE5BCC_B96F_4147_81BC_358F4A515984__INCLUDED_)
#define AFX_WHITEBOARDSERVERDLG_H__A7CE5BCC_B96F_4147_81BC_358F4A515984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhiteBoardServerDlg.h : header file
//

#include "AdvButton.h"
#include "CanvasStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CWhiteBoardServerDlg dialog

class CInstantMessagingDlg;
class CWhiteBoardServerSocket;

class CWhiteBoardServerDlg : public CDialog
{
// Construction
public:
	CWhiteBoardServerDlg(CWnd* pParent = NULL);   // standard constructor
	
	/// ֧��RUNTIME_CLASS
	DECLARE_DYNAMIC( CWhiteBoardServerDlg );

private:
	/// ��ʼ������
	void Init();
	
	/// ��ָ��IP��PORT����ָ���û�������
	void SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT nPort, const USER *pUser );
	
	/// ��װ�����û�
	void AddUserToWhiteBoard( const USER user );
	
	/// �Ƴ��û�
	void DelUserFromWhiteBoard( const USER user );
	
	/// ������Ӻ�ɾ����ť
	void UpdateButtons();

	/// �������ؼ���λ��
	void SetPosition();

	/// �ָ����ߵ�״̬
	void ResetTools();

	///	�ָ��߿�
	void ResetWidth();

	/// ���տͻ��˵Ļ�ͼ��Ϣ
	void ReceiveClientDrawInfo( char *pReceivedData, const USER user );
	
private:
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���
	CImageList				m_imageList;		/// ͷ��ͼ���б�
	CArray< USER, USER >	m_arrFriendsInWhiteBoard;	
												/// �װ��еĺ����б�
	CArray< USER, USER >	m_arrFriendsOutOfWhiteBoard;
												/// ���ڰװ��еĺ����б�
	CWhiteBoardServerSocket	
							*m_pWBSSocket;		/// �װ������socket

	TOOL					m_emTool;			/// ����
	int						m_nWidth;			/// �߿�
	COLORREF				m_clDrawColor;		/// ��ɫ
	
public:
	/// ������������
	void OnReceive();

	/// �����Լ��Ļ滭���ݸ����пͻ���
	void SendDrawInfoToAllUser( const CPoint ptStart, const CPoint ptEnd );

// Dialog Data
	//{{AFX_DATA(CWhiteBoardServerDlg)
	enum { IDD = IDD_WHITEBOARD_SERVER };
	CCanvasStatic	m_staticCanvas;
	CAdvButton	m_btnWidth4;
	CAdvButton	m_btnWidth3;
	CAdvButton	m_btnWidth2;
	CAdvButton	m_btnWidth1;
	CStatic		m_staticColor;
	CListCtrl	m_listCtrlOutOfWhiteBoard;
	CListCtrl	m_listCtrlInWhiteBoard;
	CAdvButton	m_btnRectangelEmpty;
	CAdvButton	m_btnRectangle;
	CAdvButton	m_btnPencil;
	CAdvButton	m_btnLine;
	CAdvButton	m_btnEraser;
	CAdvButton	m_btnElipseEmpty;
	CAdvButton	m_btnEllipse;
	CAdvButton	m_btnDelFriends;
	CAdvButton	m_btnAddFriends;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhiteBoardServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhiteBoardServerDlg)
	virtual void OnCancel();
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnAddfriends();
	afx_msg void OnDelfriends();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColor();
	afx_msg void OnEraser();
	afx_msg void OnPencil();
	afx_msg void OnLine();
	afx_msg void OnRectangle();
	afx_msg void OnRectangleEmpty();
	afx_msg void OnEllipseEmpty();
	afx_msg void OnEllipse();
	afx_msg void OnWidth1();
	afx_msg void OnWidth2();
	afx_msg void OnWidth3();
	afx_msg void OnWidth4();
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHITEBOARDSERVERDLG_H__A7CE5BCC_B96F_4147_81BC_358F4A515984__INCLUDED_)

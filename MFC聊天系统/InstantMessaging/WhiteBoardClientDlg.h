#if !defined(AFX_WHITEBOARDCLIENTDLG_H__2874C727_04E1_4EC2_91F9_6AC913C09F07__INCLUDED_)
#define AFX_WHITEBOARDCLIENTDLG_H__2874C727_04E1_4EC2_91F9_6AC913C09F07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "AdvButton.h"
#include "CanvasStatic.h"


class CInstantMessagingDlg;
class CWhiteBoardClientSocket;

class CWhiteBoardClientDlg : public CDialog
{
// Construction
public:
	CWhiteBoardClientDlg(CWnd* pParent = NULL);   // standard constructor

	/// ֧��RUNTIME_CLASS
	DECLARE_DYNAMIC( CWhiteBoardClientDlg );

private:
	/// ��ʼ������
	void Init();
	
	/// ��ָ��IP��PORT����ָ���û�������
	void SendUserCommandToIP( CONTROLCOMMAND command, LPCSTR szIP, UINT nPort, const USER *pUser );
	
	/// �������ؼ���λ��
	void SetPosition();
	
	/// �ָ����ߵ�״̬
	void ResetTools();
	
	///	�ָ��߿�
	void ResetWidth();

	/// �������ر�
	void ServerClosed();

	/// ���߳�������Ļ
	void Kicked();

	/// ���ջ滭����
	void ReceiveDrawData( char *pReceivedData );

private:
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���
	CImageList				m_imageList;		/// ͷ��ͼ���б�
	CString					m_strServerIP;		/// ������IP
	CWhiteBoardClientSocket	
							*m_pWBCSocket;		/// �װ�ͻ���socket
	
	TOOL					m_emTool;			/// ����
	int						m_nWidth;			/// �߿�
	COLORREF				m_clDrawColor;		/// ��ɫ
	
public:
	/// ������������
	void OnReceive();

	/// ���ض�Ӧ�ķ�����IP
	CString GetServerIP(){ return m_strServerIP; }

	/// ͬ���������
	void AcceptAddRequest( const USER userServer );

	/// ����ͼ��Ϣ���͵�������
	void SendDrawInfoToServer( const CPoint ptStart, const CPoint ptEnd );

// Dialog Data
	//{{AFX_DATA(CWhiteBoardClientDlg)
	enum { IDD = IDD_WHITEBOARD_CLIENT };
	CStatic	m_staticColor;
	CCanvasStatic	m_staticCanvas;
	CAdvButton	m_btnWidth4;
	CAdvButton	m_btnWidth3;
	CAdvButton	m_btnWidth2;
	CAdvButton	m_btnWidth1;
	CAdvButton	m_btnRectangleEmpty;
	CAdvButton	m_btnRectangle;
	CAdvButton	m_btnPencil;
	CAdvButton	m_btnLine;
	CAdvButton	m_btnEraser;
	CAdvButton	m_btnEllipseEmpty;
	CAdvButton	m_btnEllipse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhiteBoardClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhiteBoardClientDlg)
	afx_msg void OnOk();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEraser();
	afx_msg void OnPencil();
	afx_msg void OnLine();
	afx_msg void OnRectangleEmpty();
	afx_msg void OnRectangle();
	afx_msg void OnEllipseEmpty();
	afx_msg void OnEllipse();
	afx_msg void OnColor();
	afx_msg void OnWidth1();
	afx_msg void OnWidth2();
	afx_msg void OnWidth3();
	afx_msg void OnWidth4();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHITEBOARDCLIENTDLG_H__2874C727_04E1_4EC2_91F9_6AC913C09F07__INCLUDED_)

#if !defined(AFX_FRIENDSLISTCTRL_H__2EE393C8_2A1D_43E5_AA13_296614EB5AA5__INCLUDED_)
#define AFX_FRIENDSLISTCTRL_H__2EE393C8_2A1D_43E5_AA13_296614EB5AA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FriendsListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFriendsListCtrl window

class CInstantMessagingDlg;

class CFriendsListCtrl : public CListCtrl
{

// Construction
public:
	CFriendsListCtrl();

// Attributes
private:
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���
	CImageList				m_imageList;		/// ͷ��ͼ���б�
	int						m_nCamera;			/// ����ͷ��������
	int						m_nDelIcon;			/// ɾ����ť��������
	int						m_nCurSel;			/// ��ǰѡ���û����

// Operations
public:
	int AddUser( const USER user );
	void DeleteUser( int nIndex );
	void SetMainDlg( CInstantMessagingDlg *pMainDlg );
	void DeleteAllUsers();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFriendsListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFriendsListCtrl();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

	// Generated message map functions
protected:
	//{{AFX_MSG(CFriendsListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave( WPARAM wParam, LPARAM lParam );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnDeleteuser();
	afx_msg void OnSendmessage();
	afx_msg void OnVideochat();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRIENDSLISTCTRL_H__2EE393C8_2A1D_43E5_AA13_296614EB5AA5__INCLUDED_)

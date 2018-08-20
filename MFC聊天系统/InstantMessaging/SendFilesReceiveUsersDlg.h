#if !defined(AFX_SENDFILESRECEIVEUSERSDLG_H__EF3978CB_D9B9_4688_9F91_642265ADA878__INCLUDED_)
#define AFX_SENDFILESRECEIVEUSERSDLG_H__EF3978CB_D9B9_4688_9F91_642265ADA878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendFilesReceiveUsersDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSendFilesReceiveUsersDlg dialog

class CInstantMessagingDlg;
class CSendFilesServerDlg;

class CSendFilesReceiveUsersDlg : public CDialog
{
// Construction
public:
	CSendFilesReceiveUsersDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSendFilesReceiveUsersDlg)
	enum { IDD = IDD_SENDFILES_USER };
	CListCtrl	m_listCtrlUser;
	//}}AFX_DATA
private:
	/// ��ʼ������
	void Init();
	
private:
	CSendFilesServerDlg		*m_pSFSDlg;			/// �����ļ��������Ի���
	CInstantMessagingDlg	*m_pMainDlg;		/// ���Ի���
	CImageList				m_imageList;		/// ͷ��ͼ���б�
	CArray< USER, USER >	m_arrFriends;		/// �������еĺ����б�

public:
	/// ���÷����ļ��������Ի����ָ��
	void SetSFSDlg( CSendFilesServerDlg *pSFSDlg ){ m_pSFSDlg = pSFSDlg; }
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendFilesReceiveUsersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendFilesReceiveUsersDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDFILESRECEIVEUSERSDLG_H__EF3978CB_D9B9_4688_9F91_642265ADA878__INCLUDED_)

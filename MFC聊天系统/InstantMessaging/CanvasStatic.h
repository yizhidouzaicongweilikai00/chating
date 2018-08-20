#if !defined(AFX_CANVASSTATIC_H__9C2685EC_A4D6_40E2_9974_E602AF4EE2D9__INCLUDED_)
#define AFX_CANVASSTATIC_H__9C2685EC_A4D6_40E2_9974_E602AF4EE2D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanvasStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCanvasStatic window

class CCanvasStatic : public CStatic
{
// Construction
public:
	CCanvasStatic();

// Attributes
private:
	CDC						m_dcMem;			/// ��ͼ���ڴ�dc
	CBitmap					m_bitmap;			/// ��ͼ���ڴ�bitmap

	CDC						m_dcOld;			/// ����ԭ����dc
	CBitmap					m_bitmapOld;		/// ����ԭ����bitmap

	TOOL					m_emTool;			/// ����
	int						m_nWidth;			/// �߿�
	COLORREF				m_clDrawColor;		/// ��ɫ

	HCURSOR					m_hCursor;			/// ���ָ��

	CPoint					m_ptStart;			/// ��ʼ��
	CPoint					m_ptEnd;			/// ������

	BOOL					m_bDrawing;			/// �Ƿ��ڻ�ͼ��
	BOOL					m_bOwnerDraw;		/// �Ƿ����Լ���ͼ���Ǳ��˻�ͼ

// Operations
public:
	void SetTool( TOOL emTool ){ m_emTool = emTool; SetCursorByTool(); }
	void SetWidth( int nWidth ){ m_nWidth = nWidth; SetCursorByTool(); }
	void SetColor( COLORREF cl ){ m_clDrawColor = cl; }

	/// ���ݻ�ͼ��Ϣ��ͼ
	void Draw( DRAWINFO drawInfo );

	/// ��ͼ
	void Draw();

	/// �������
	void ClearCanvas();

private:
	/// ���»���
	void UpdataCanvas();

	/// �������ָ��
	void SetCursorByTool();

	/// ����
	void DrawLine();

	/// ����
	void Erase();

	/// ����
	void DrawPoint();

	/// �����ľ���
	void DrawEmptyRectangle();

	/// ������
	void DrawRectangle();

	/// ��������Բ
	void DrawEmptyEllipse();

	/// ����Բ
	void DrawEllipse();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvasStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCanvasStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCanvasStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseLeave( WPARAM wParam, LPARAM lParam );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASSTATIC_H__9C2685EC_A4D6_40E2_9974_E602AF4EE2D9__INCLUDED_)

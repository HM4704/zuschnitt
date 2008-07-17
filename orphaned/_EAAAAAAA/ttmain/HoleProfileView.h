#if !defined(AFX_HOLEPROFILEVIEW_H__25572C95_1A7C_408F_96C8_142C0DE3A016__INCLUDED_)
#define AFX_HOLEPROFILEVIEW_H__25572C95_1A7C_408F_96C8_142C0DE3A016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoleProfileView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileView view

class CHoleProfileView : public CScrollView
{
protected:
	CHoleProfileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHoleProfileView)

// Attributes
public:
	CFont m_stdFont;
	CFont m_boldFont;
	CFont m_thinFont;
	CFont m_smallFont;
	CPen  m_orgPen;
	CPen  m_bPen;

private:
    int     m_iPageLen;
    int     m_iPageHeight;
    int     m_iLeftX;           // linkX
    int     m_iTopY;            // startY

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoleProfileView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHoleProfileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHoleProfileView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHoleProfileAendern();
	afx_msg void OnUpdateHoleProfileAendern(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
    void  getFont(CFont* pFont, int height, int width, int orient, int weight);
    void  DrawAll(CDC* pDC);
    void  DrawHeader(CDC* pDC);
    void  DrawGrid(CDC* pDC);
    void  DrawData(CDC* pDC);
    void  PrintCutList(CDC* pDC, CPtrArray* pList, int& iY, int iX, int iWidth);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLEPROFILEVIEW_H__25572C95_1A7C_408F_96C8_142C0DE3A016__INCLUDED_)

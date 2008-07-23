// ttmainView.h : interface of the CTtmainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTMAINVIEW_H__141C612F_1243_11D4_8E7B_F82D876B953C__INCLUDED_)
#define AFX_TTMAINVIEW_H__141C612F_1243_11D4_8E7B_F82D876B953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define squareW   140
#define squareH   60
#define spaceSquare 20  //Abstand von Tuer und Rechteck

#define SCR_XUNIT  8
#define SCR_YUNIT  15
#define SCR_XRANGE 80
#define SCR_YRANGE 20
#define MIN_Y_EINTRAG (DINA4WID/2)   // min. Höhe eines Eintrags für ein Tor



#define VIEW_MAX_TORE   5       // max. Anzahl Tore, die im Fenster darstellbar sind

#define classZeichnung "Zeichnung"
#define classBestellung   "Bestellung"



class CTorDoor;

class CTtmainView : public CScrollView
{
protected: // create from serialization only
	CTtmainView();
	DECLARE_DYNCREATE(CTtmainView)

// Attributes
public:
	CTtmainDoc* GetDocument();

// Varaibles
public:
	//CTorDoor* m_pTor;
	CPtrArray m_pTore;

	int actY;
	BOOL m_bFirstShow;

	HFONT m_stdFont;
	HFONT m_boldFont;
	HFONT m_thinFont;
	HFONT m_smallFont;
	HPEN m_orgPen;
	HPEN m_bPen;

private:
    int     m_iPageLen;
    int     m_iPageHeight;
    int     m_iLeftX;           // linkX
    int     m_iTopY;            // startY
    int     m_iAryTorHoehe[VIEW_MAX_TORE];    // Höhe der Spalte für jedes Tor

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTtmainView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	CTorDoor* getSelectedTor(void);
    int getSelectedTorNr(void);
	int editTorDoor(int iTorNr);
	int editProfiles(int iTorNr);
	int saveTorDoor(int iTorNr, char* strFileName);
	int openTorDoor(CString& strFileName);
	virtual ~CTtmainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	HFONT getFont(int height, int width, int orient, int weight);
	void PaintTT(HDC hdc, int Fact = 1);
	void drawGrid(HDC hdc, int fact);
	void fillGrid(HDC hdc, int fact);
    void drawHeader(HDC hdc, int Fact);
	void stretchGrid(HDC hdc, int beginY, int endY, int fact);
	void getRealKoord(CPoint* pt);
	int addTorDoor();
	int  getTorCount();
    CTorDoor* removeTor(int iTorNr);
    void computeAndDraw(void);
    BOOL canAddTor(void);

protected:

// Generated message map functions
protected:
    void AppendTestsystemMenu(void);

	//{{AFX_MSG(CTtmainView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPuTorAendern();
	afx_msg void OnUpdatePuTorAendern(CCmdUI* pCmdUI);
    afx_msg void OnZuschnittregelFluegel(UINT nId);
	afx_msg void OnUpdateZuschnittregelFluegel(CCmdUI* pCmdUI);
	afx_msg void OnPuProfileBearbeiten();
	afx_msg void OnUpdatePuProfileBearbeiten(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ttmainView.cpp
inline CTtmainDoc* CTtmainView::GetDocument()
   { return (CTtmainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTMAINVIEW_H__141C612F_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

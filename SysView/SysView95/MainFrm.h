// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CAD19EA4_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)
#define AFX_MAINFRM_H__CAD19EA4_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ChildView.h"
#include "SysSheet.h"


///////////////////////////////////////////
// defines
#define  WS_MY_TOOLBAR_VISIBLE       0x00010000
#define  WS_MY_STATUSBAR_VISIBLE     0x00020000



class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

    CStatusBar& GetStatusBar(void) {return m_wndStatusBar;};
// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
//	CChildView    m_wndView;
    CSysSheet  *m_pSheet;
    LONG        m_lWndStyles;
    DWORD       m_dwWndExStyles;
    CRegistryItemDword      m_riPosX, m_riPosY;
    CRegistryItemDword      m_riSizeX, m_riSizeY;
    CRegistryItemDword      m_dwMinimized;
    CRegistryItemDword      m_ridWindowStyles;

// Generated message map functions
protected:
	BOOL AdjustSize(int cx, int cy);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSpawnProcess();
	afx_msg void OnUpdateSpawnProcess(CCmdUI* pCmdUI);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnForeground();
	afx_msg void OnUpdateForeground(CCmdUI* pCmdUI);
	afx_msg void OnMinimizeAtStart();
	afx_msg void OnUpdateMinimizeAtStart(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar();
	afx_msg void OnViewStatusBar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    BOOL CMainFrame::SetupRegistryValues();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CAD19EA4_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)

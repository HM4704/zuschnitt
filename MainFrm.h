// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__141C6129_1243_11D4_8E7B_F82D876B953C__INCLUDED_)
#define AFX_MAINFRM_H__141C6129_1243_11D4_8E7B_F82D876B953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Defines
#define WM_BERECHNE_ZUSCNITTE       (WM_USER+1)       // Botschaft von Zuschnittregeln
                                                      // dialog
#define WM_SPLIT_SHEET              (WM_USER+2)       // Teile aktuelles Blatt auf zwei
#define WM_ZR_SHEET_CLOSED          (WM_USER+8)       // Sheet wurde geschlossen


const char szZuschnittHilfe[] = "Zuschnitt_hilfe.htm";

class CZRSheet;
class CTTTestManager;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
    void       SplitSheet(void);
    CTorDoor*  GetSplitTor(void) { return m_pSplitTor; };
    void       ClearSplitTor(void) { m_pSplitTor = NULL; };
	CZRSheet* CreateZuschnittDlg(void);
    void       LoadToolBar(UINT nId);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
//	CStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;
    CRegistryItemDword      m_riPosX, m_riPosY;
    CRegistryItemDword      m_riSizeX, m_riSizeY;
    CRegistryItemDword      m_ridWindowStyles;
    CRegistryItemDword      m_dwMinimized;

    CTorDoor*   m_pSplitTor;

    CTTTestManager* m_pTestMan;
    CMenu       m_menuTest;

protected:  // Eingebettete Member der Steuerleiste
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// Generated message map functions
protected:
	BOOL SetupRegistryValues(void);
    void AppendTestsystemMenu(void);

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTorHinzu();
	afx_msg void OnUpdateTorHinzu(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnKreisberechnung();
	afx_msg void OnUpdateKreisberechnung(CCmdUI* pCmdUI);
	afx_msg void OnHilfeInhalt();
	afx_msg void OnUpdateHilfeInhalt(CCmdUI* pCmdUI);
	afx_msg void OnTorAendern();
	afx_msg void OnUpdateTorAendern(CCmdUI* pCmdUI);
	afx_msg void OnRefTorSpeichern();
	afx_msg void OnUpdateRefTorSpeichern(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRefTorLaden(CCmdUI* pCmdUI);
	afx_msg void OnZuschnittregelnAnzeigen();
	afx_msg void OnUpdateZuschnittregelnAnzeigen(CCmdUI* pCmdUI);
	afx_msg void OnTestStart();
	afx_msg void OnUpdateTestStart(CCmdUI* pCmdUI);
    afx_msg LRESULT OnZuschnitteBerechnen(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnZRSheetClosed(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSplitSheet(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTestEinstellungen();
	afx_msg void OnUpdateTestEinstellungen(CCmdUI* pCmdUI);
	afx_msg void OnProfileBearbeiten();
	afx_msg void OnUpdateProfileBearbeiten(CCmdUI* pCmdUI);
	afx_msg void OnBhTest();
	afx_msg LRESULT OnResetMenu(WPARAM,LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnWhsprofile();
public:
    afx_msg void OnTestPs312();
public:
    afx_msg void OnExtrasStartPs312();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__141C6129_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

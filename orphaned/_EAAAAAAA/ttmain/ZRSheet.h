#if !defined(AFX_ZRSHEET_H__DDC883A8_B5A3_11D5_8E7C_96319B2B0854__INCLUDED_)
#define AFX_ZRSHEET_H__DDC883A8_B5A3_11D5_8E7C_96319B2B0854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Defines

// Windows Messages
#define     ID_ZR_SHEET_SAVE            (1001)
#define     ID_ZR_SHEET_NEW             (1000)
#define     WM_ZR_SHEET_SET_BUTTONS     (WM_USER+3)     // Buttons setzen
#define     WM_ZR_SHEET_NEW_TREE        (WM_USER+4)     // Button Neu gedrückt
#define     WM_ZR_SHEET_PAGED_CHANGED   (WM_USER+5)     // Baum hat sich geändet
#define     WM_ZR_SHEET_SHOW_RULES      (WM_USER+6)     // alle Regeln anzeigen
            // Format: WPARAM: CTorDoor*, LPARAM: CFlParam*
#define     WM_ZR_SHEET_SHOW_NODE       (WM_USER+7)     // an Pages, zeige die Node an
#define     WM_ZR_SHEET_CLOSED          (WM_USER+8)     // Sheet wurde geschlossen

const char szZuschnittProfileHilfe[] = "zuschnittregeln_dlg_hilfe.htm";
//const char szZuschnittSchiebefensterHilfe[] = "zuschnitt_schiebefenster_hilfe.htm";

/////////////////////////////////////////////////////////////////////////////
// CZRSheet
class CZRProfilePage;
class CZRSFPage;

class CZRSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CZRSheet)

// Construction
public:
	CZRSheet(UINT nIDCaption, CZuschnittManager* pMan, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CZRSheet(LPCTSTR pszCaption, CZuschnittManager* pMan, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

private:
	BOOL SetTabState(int iIndex, DWORD dwState);
	void SetButton(CButton& button, DWORD dwState);
	BOOL ResizeButtons(LPRECT rectPage);
	BOOL ResizePages(LPRECT rect);
	BOOL AddPages(void);
    void StartAllPages(void);


    CRect                   m_rectSize;
    CRect                   m_rectPage;
    CZuschnittManager*      m_pMan;
    CZRProfilePage*         m_pPPSenk;
    CZRProfilePage*         m_pPPQuer;
    CZRProfilePage*         m_pPPOber;
    CZRProfilePage*         m_pPPUnter;
    CZRSFPage*              m_pPPSf;

    CButton m_ButtonApply;
    CButton m_ButtonSave;
    CButton m_ButtonCancel;
    CButton m_ButtonHelp;
    CButton m_ButtonNew;
    CToolTipCtrl            m_toolTip;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZRSheet)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZRSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CZRSheet)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnSetButtons(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPageChanged(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnGetDefId(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShowRules(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnNewTree();
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnHelp();
    afx_msg void OnSave();
	afx_msg void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRSHEET_H__DDC883A8_B5A3_11D5_8E7C_96319B2B0854__INCLUDED_)

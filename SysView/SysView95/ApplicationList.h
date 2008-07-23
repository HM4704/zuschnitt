#if !defined(AFX_APPLICATIONLIST_H__AA6F1C41_FB9E_11D4_8E7C_AD23B374CB37__INCLUDED_)
#define AFX_APPLICATIONLIST_H__AA6F1C41_FB9E_11D4_8E7C_AD23B374CB37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApplicationList.h : header file
//

#include "ToolHelp.h"
#undef MAX_COLUMNS
#define MAX_COLUMNS   2
#include "ColumnList.h"

///////////////////////////////////////////
// defines
#define MAX_APPS       50      // fits dynamically when bigger

// private Flags
#define F_EXIST         0x01
#define F_POS_CHANGED   0x02

// Zeiten in ms fuer Refreshrate
#define RR_HIGH     1000
#define RR_MID      4000
#define RR_LOW      8000
#define RR_MANUAL      0

#define REFRESH_APPLIST_TIMER  11
///////////////////////////////////////////
// const
const char szApplication[]      =   _T("Task");
const char szStatus[]           =   _T("Status");
const char szStatusRuns[]       =   _T("Wird ausgeführt");
const char szStatusHangs[]      =   _T("reagiert nicht");


/////////////////////////////////////////////////////////////////////////////
// CApplicationList window

class CApplicationList : public CListCtrl
{
// Construction
public:
	BOOL Create(const CRect &rect, CWnd* pWnd, UINT uiId);
	CApplicationList();

	BOOL UpdateList(void);

// Attributes
public:

private:
	inline const char* StatusToStr(BOOL bStatus);
	inline BOOL StrToStatus(char *strStatus);
	void RefreshItem(int iItem, APPLICATION *pApp);
    CToolHelp       m_toolHelp;
    BOOL            m_bColumnsCreated;
    APPLICATION     m_taskEntryList[MAX_APPS];
    DWORD           m_dwExistApps;
    BYTE            *m_pListField;          // fuer Listenverwaltung
    BOOL            m_bMustRefreshItems;
    BOOL            m_bTimerCreated;
    int             m_iStrlenStatusRuns;
    HICON           m_hDefaultIcon;
    int             m_iSelectedItem;
    CRegistryItemDword           m_ridRefreshTime;

protected:
	CImageList      *m_pImageListSmall;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplicationList)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetRefreshTime(DWORD dwTime);
	DWORD GetRefreshTime(void);
	BOOL ActivateTask(void);
	BOOL EndTask(void);
	void StopApplicationWalk(void);
	void StartApplicationWalk(void);
    BOOL GetSelectedItem(int &iItem);

	virtual ~CApplicationList();

	// Generated message map functions
protected:
	void UpdateImageList(void);
	BOOL CreateColumns(void);
	//{{AFX_MSG(CApplicationList)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSwitchto();
	afx_msg void OnUpdateSwitchto(CCmdUI* pCmdUI);
	afx_msg void OnKillTask();
	afx_msg void OnUpdateKillTask(CCmdUI* pCmdUI);
	afx_msg void OnSwitchtoProcess();
	afx_msg void OnUpdateSwitchtoProcess(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLICATIONLIST_H__AA6F1C41_FB9E_11D4_8E7C_AD23B374CB37__INCLUDED_)

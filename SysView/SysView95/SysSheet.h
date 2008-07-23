#if !defined(AFX_SYSSHEET_H__CAD19EAD_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)
#define AFX_SYSSHEET_H__CAD19EAD_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSheet.h : header file
//

#include "ProcessPg.h"
#include "PerformancePg.h"
#include "ApplicationPg.h"


#define WM_RESIZEPAGE WM_USER + 111
/////////////////////////////////////////////////////////////////////////////
// CSysSheet

class CSysSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSysSheet)

// Construction
public:
//    CSysSheet();
	CSysSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSysSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

private:
    CProcessPg              *m_pProcessPg;
    CPerformancePg          *m_pPerformancePg;
    CApplicationPg         *m_pApplicationPg;
    CRect                   m_rectSize;
    CRect                   m_rectPage;
    CRegistryItemDword      m_riActivePage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void StartAllPages(void);
	virtual ~CSysSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSysSheet)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();
    afx_msg LRESULT  OnSwitchToProcess(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    afx_msg LRESULT OnResizePage(WPARAM wParam, LPARAM lParam);
private:
	BOOL SetupRegistryValues(void);
	BOOL ResizePages(LPRECT rect);
	BOOL RemovePages(void);
	BOOL AddPages(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSHEET_H__CAD19EAD_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)

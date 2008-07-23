#if !defined(AFX_TASKPG_H__F47ECE24_F16C_11D4_8E7B_CB3E523F3393__INCLUDED_)
#define AFX_TASKPG_H__F47ECE24_F16C_11D4_8E7B_CB3E523F3393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApplicationPg.h : header file
//

#include "SysBasePage.h"
#include "ApplicationList.h"

/////////////////////////////////////////////////////////////////////////////
// CApplicationPg dialog

class CApplicationPg : public CSysBasePage
{
	DECLARE_DYNCREATE(CApplicationPg)

// Construction
public:
	CApplicationPg();
	~CApplicationPg();

// Dialog Data
	//{{AFX_DATA(CApplicationPg)
	enum { IDD = IDD_TASK_PG };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

private:
    CApplicationList    m_AppList;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CApplicationPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL PlaceItems(CRect &rect);
	BOOL InitializeList(UINT dwIDC_Frame);

	// Generated message map functions
	//{{AFX_MSG(CApplicationPg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg void OnStartAppwalk();
	afx_msg void OnUpdateStartAppwalk(CCmdUI* pCmdUI);
	afx_msg void OnStopAppwalk();
	afx_msg void OnUpdateStopAppwalk(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnEndTask();
	afx_msg void OnActivateTask();
	afx_msg void OnHook();
	afx_msg void OnUpdateHook(CCmdUI* pCmdUI);
	afx_msg void OnUnhook();
	afx_msg void OnUpdateUnhook(CCmdUI* pCmdUI);
    afx_msg LRESULT  OnSwitchToProcess(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSetActive();
	afx_msg void OnRefreshHigh();
	afx_msg void OnUpdateRefreshHigh(CCmdUI* pCmdUI);
	afx_msg void OnRefreshLow();
	afx_msg void OnUpdateRefreshLow(CCmdUI* pCmdUI);
	afx_msg void OnRefreshMid();
	afx_msg void OnUpdateRefreshMid(CCmdUI* pCmdUI);
	afx_msg void OnOpenWintop();
	afx_msg void OnUpdateOpenWintop(CCmdUI* pCmdUI);
	afx_msg void OnStartWintop();
	afx_msg void OnUpdateStartWintop(CCmdUI* pCmdUI);
	afx_msg void OnMemoryWintop();
	afx_msg void OnUpdateMemoryWintop(CCmdUI* pCmdUI);
	afx_msg void OnTimesWintop();
	afx_msg void OnUpdateTimesWintop(CCmdUI* pCmdUI);
	afx_msg void OnRefreshManual();
	afx_msg void OnUpdateRefreshManual(CCmdUI* pCmdUI);
	afx_msg void OnDebug1();
	afx_msg void OnUpdateDebug1(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKPG_H__F47ECE24_F16C_11D4_8E7B_CB3E523F3393__INCLUDED_)

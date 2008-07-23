#if !defined(AFX_PROCESSPG_H__CAD19EAE_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)
#define AFX_PROCESSPG_H__CAD19EAE_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessPg.h : header file
//

#include "ProcessList.h"
#include "SysBasePage.h"

/////////////////////////////////////////////////////////////////////////////
// CProcessPg dialog

class CProcessPg : public CSysBasePage
{
	DECLARE_DYNCREATE(CProcessPg)

// Construction
public:
	CProcessPg();
	~CProcessPg();

// Dialog Data
	//{{AFX_DATA(CProcessPg)
	enum { IDD = IDD_PROCESS_PG };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


private:
    CProcessList    m_ProcList;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProcessPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL InitializeList(UINT dwIDC_Frame);
	virtual BOOL PlaceItems(CRect &rect);

	// Generated message map functions
	//{{AFX_MSG(CProcessPg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg void OnRefreshHigh();
	afx_msg void OnUpdateRefreshHigh(CCmdUI* pCmdUI);
	afx_msg void OnRefreshLow();
	afx_msg void OnUpdateRefreshLow(CCmdUI* pCmdUI);
	afx_msg void OnRefreshMid();
	afx_msg void OnUpdateRefreshMid(CCmdUI* pCmdUI);
	afx_msg void OnPid();
	afx_msg void OnUpdatePid(CCmdUI* pCmdUI);
	afx_msg void OnCntusage();
	afx_msg void OnUpdateCntusage(CCmdUI* pCmdUI);
	afx_msg void OnModuleid();
	afx_msg void OnUpdateModuleid(CCmdUI* pCmdUI);
	afx_msg void OnHeapid();
	afx_msg void OnUpdateHeapid(CCmdUI* pCmdUI);
	afx_msg void OnCntthreads();
	afx_msg void OnUpdateCntthreads(CCmdUI* pCmdUI);
	afx_msg void OnParentPid();
	afx_msg void OnUpdateParentPid(CCmdUI* pCmdUI);
	afx_msg void OnPriority();
	afx_msg void OnUpdatePriority(CCmdUI* pCmdUI);
	afx_msg void OnFlags();
	afx_msg void OnUpdateFlags(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT  OnSwitchToProcess(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSetActive();
	afx_msg void OnPtDlg();
	afx_msg void OnUpdatePtDlg(CCmdUI* pCmdUI);
	afx_msg void OnCpuTime();
	afx_msg void OnUpdateCpuTime(CCmdUI* pCmdUI);
	afx_msg void OnCpuPercent();
	afx_msg void OnUpdateCpuPercent(CCmdUI* pCmdUI);
	afx_msg void OnRefreshManual();
	afx_msg void OnUpdateRefreshManual(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSPG_H__CAD19EAE_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)

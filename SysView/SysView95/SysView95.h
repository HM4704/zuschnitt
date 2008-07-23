// SysView95.h : main header file for the SYSVIEW95 application
//

#if !defined(AFX_SYSVIEW95_H__CAD19EA0_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)
#define AFX_SYSVIEW95_H__CAD19EA0_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "RegistryManager.h"


/////////////////////////////////////////////////////////////////////////////
// Global defines
// 
#define  SYSVIEW95_CLASS   _T("Sysview95Wnd")   // our Class

#define MSG_SWITCH_TO_PROCESS   (WM_USER+100)

#define SYS_INTEGRATION_MENU    1       // Aufruf über Taskleisten-Menü
#define SYS_INTEGRATION_AUTO    2       // Einrichten bei jedem Boot

/////////////////////////////////////////////////////////////////////////////
// CSysView95App:
// See SysView95.cpp for the implementation of this class
//

class CSysView95App : public CWinApp
{
public:
	CSysView95App();

public:

private:
    CRegistryManager    m_regManager;
    CRegistryItemString m_riAppPath;
    CRegistryItemDword  m_ridIntegration;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysView95App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
    BOOL FirstInstance();

public:
	CRegistryManager* GetRegistryManager(void);
	//{{AFX_MSG(CSysView95App)
	afx_msg void OnAppAbout();
	afx_msg void OnSysIntegration();
	afx_msg void OnUpdateSysIntegration(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSVIEW95_H__CAD19EA0_D985_11D4_8E7B_BC5ECE72CA32__INCLUDED_)

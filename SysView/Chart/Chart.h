#if !defined(AFX_CHART_H__71862334_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED_)
#define AFX_CHART_H__71862334_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Chart.h : main header file for CHART.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartApp : See Chart.cpp for implementation.

class CChartApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHART_H__71862334_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED)

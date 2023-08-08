// RegMan.h : main header file for the REGMAN DLL
//

#if !defined(AFX_REGMAN_H__D27BC6CB_BA43_11D5_8E7C_FD8C227A7B54__INCLUDED_)
#define AFX_REGMAN_H__D27BC6CB_BA43_11D5_8E7C_FD8C227A7B54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegManApp
// See RegMan.cpp for the implementation of this class
//

class CRegManApp : public CWinApp
{
public:
	CRegManApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegManApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRegManApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMAN_H__D27BC6CB_BA43_11D5_8E7C_FD8C227A7B54__INCLUDED_)

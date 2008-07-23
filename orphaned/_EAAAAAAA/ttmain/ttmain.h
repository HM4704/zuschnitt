// ttmain.h : main header file for the TTMAIN application
//

#if !defined(AFX_TTMAIN_H__141C6125_1243_11D4_8E7B_F82D876B953C__INCLUDED_)
#define AFX_TTMAIN_H__141C6125_1243_11D4_8E7B_F82D876B953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTtmainApp:
// See ttmain.cpp for the implementation of this class
//
#include "HtmlHelper.h"

#define HILFE_VERZEICHNIS  "Hilfe"

class CTtmainApp : public CWinApp
{
public:
	CTtmainApp();

    BOOL ShowHelp(const char* strHmtlFile, PLACING place, RECT* pRect = NULL)
    { return m_Help.OpenDoc(strHmtlFile, place, pRect); };


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTtmainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTtmainApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CHtmlHelper     m_Help;

public:
	void SetLandscape();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTMAIN_H__141C6125_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

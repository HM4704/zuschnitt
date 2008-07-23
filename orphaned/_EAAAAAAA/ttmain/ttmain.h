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
#include "RegistryManager.h"

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
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTtmainApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnZuschnittdateiAuslesen();
	afx_msg void OnUpdateZuschnittdateiAuslesen(CCmdUI* pCmdUI);
	afx_msg void OnZuschnittregelnSpeichern();
	afx_msg void OnUpdateZuschnittregelnSpeichern(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CHtmlHelper     m_Help;
    CZuschnittManager   m_ZuschnittMan;
    CString         m_strZuschnittDatei;        // enthält Name der Zuschnitt-Datei
    CRegistryManager    m_regManager;

public:
	void SetLandscape();
    CZuschnittManager* GetZuschnittManager(void) { return &m_ZuschnittMan; };
    CRegistryManager* GetRegistryManager(void) { return &m_regManager; };
protected:
	BOOL CheckDirectory(CString strFileName);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTMAIN_H__141C6125_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

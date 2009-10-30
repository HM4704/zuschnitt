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



/////// einige Defines fuer Ouput
#define DINA4LEN  2750//900  //770
#define DINA4WID  1800//600
#define linksX 0
#define startY 130//70
#define spaceX    10   //Abstand zwischen Text und Linie

#define FONTHEIGTH  50  //15
#define FONTWIDTH   20  //7
#define THINFONTHEIGTH  50  //15
#define THINFONTWIDTH   17  //7

#define MM_PER_INCH     25.4f

#define rowH      70


typedef struct {
    int bSize;
    char string[50];
    } sGridCont;


/////////////////////////////////////////////////////////////////////////////
// CTtmainApp:
// See ttmain.cpp for the implementation of this class
//
#include "HtmlHelper.h"
#include "RegistryManager.h"

#define HILFE_VERZEICHNIS  "Hilfe"

class CTtmainApp : public CWinAppEx
{
public:
	CTtmainApp();

    BOOL ShowHelp(const char* strHmtlFile, PLACING place, RECT* pRect = NULL)
    { return m_Help.OpenDoc(strHmtlFile, place, pRect); };


    int             m_iLastMode;            // mit welchem Dialog als letztes gearbeitet wurde

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
	afx_msg void OnFileOpen();
	afx_msg void OnZuschnittdateiAuslesen();
	afx_msg void OnUpdateZuschnittdateiAuslesen(CCmdUI* pCmdUI);
	afx_msg void OnZuschnittregelnSpeichern();
	afx_msg void OnUpdateZuschnittregelnSpeichern(CCmdUI* pCmdUI);
	afx_msg void OnHoleProfileNew();
	afx_msg void OnUpdateHoleProfileNew(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CHtmlHelper     m_Help;
    CZuschnittManager   m_ZuschnittMan;
    CString         m_strZuschnittDatei;        // enthält Name der Zuschnitt-Datei
    CRegistryManager    m_regManager;
    BOOL            m_bTestMode;
    CString         m_strAppPath;

public:
	void SetPrintOrientation(short dmOrientation);
    CZuschnittManager* GetZuschnittManager(void) { return &m_ZuschnittMan; };
    CRegistryManager* GetRegistryManager(void) { return &m_regManager; };
    BOOL IsInTestMode(void) { return m_bTestMode; };   // bei release auf FALSE!!!
    CString& GetAppPath(void) { return m_strAppPath; };
protected:
	BOOL CheckDirectory(CString strFileName);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTMAIN_H__141C6125_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

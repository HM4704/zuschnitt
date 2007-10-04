// TTTestManager.h: interface for the CTTTestManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTTESTMANAGER_H__8E8BE7F1_8B9D_4B1D_A3D9_ABD8BC70739B__INCLUDED_)
#define AFX_TTTESTMANAGER_H__8E8BE7F1_8B9D_4B1D_A3D9_ABD8BC70739B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegistryItem.h"

//////////////////////////////////////////////////////////////////////////
///  Defines
#define     TT_TEST_VERSION  100

// Config Flags
#define     CM_MANUAL          0x00000001
#define     CM_SHOW_CFG_D      0x00000002
#define     CM_MSG_ON_ERR      0x00000004
#define     CM_LOG_WARNING     0x00000008
#define     CM_SHOW_LOG        0x00000010


typedef union
{
    struct {
        DWORD bManual            : 1;       // automatischer Durchlauf oder nicht
        DWORD bShowCfgDlg        : 1;       // bei jedem Start Configure-Dlg anzeigen?
        DWORD bBreakOnError      : 1;       // nach Auftritt von Fehler anhalten
        DWORD bMessageBoxOnError : 1;       // nach Auftritt von Fehler MessageBox
        DWORD bLogWarnings       : 1;       // Warnings in Log-Datei eintragen
        DWORD bShowLogFile       : 1;       // Log-Datei automatisch am Ende anzeigen
    } bf;
    DWORD dwConfig;
}
BF_CONFIG_FLAGS;

typedef union
{
    struct {
        DWORD bProfile           : 1;       // Profile überprüfen
        DWORD bFuellung          : 1;       // Fuellung überprüfen
    } bf;
    DWORD dwTest;
}
BF_TEST_FLAGS;

//////////////////////////////////////////////////////////////////////////
///  Structs
typedef struct _TTM_CONFIG
{
    int  iShowTime;             // Länge in ms, während der das Fenster angezeigt wird
    CString  strTestFile;       // Name der Test-Datei
    CString  strLogFile;        // Name der Log-Datei
    CString  strBaseTestDir;    // Basis-Directory für Tests

    BF_CONFIG_FLAGS   ribfConfig;
    BF_TEST_FLAGS     ribfCheck[2];    // Typ 400 + Typ 600
}
TTM_CONFIG;


class CRegistryManager;

//class CRegistryItemDword;

class CTTTestManager  
{
    typedef struct _TTM_STATISTIC
    {
        DWORD dwCntCompares;
        DWORD dwErrors;
        DWORD dwWarnings;
        DWORD dwRemarks;
    }
    TTM_STATISTIC;

    CFrameWnd*     m_pFrmWnd;
    TTM_CONFIG     m_Config;
    TTM_STATISTIC  m_Statistic;
    ifstream       m_isTestFile;
    ofstream       m_osLogFile;
    CRegistryItemDword m_riConfig;
    CRegistryManager*  m_pRegMan;


public:
	CTTTestManager();
	virtual ~CTTTestManager();

    BOOL Initialize(CFrameWnd* pFrmWnd, CRegistryManager* pRegMan = NULL);
    BOOL ConfigureDlg(void);
    BOOL OpenTestFile(void);
    BOOL CloseTestFile(void);

    BOOL GetNextSet(char* strRefFile, CTorDoor** ppTor);
    BOOL CompareSet(CTorDoor* pRefTor, CTorDoor* pTor, char* strRefFile = NULL);
    

    BOOL DoTest(void);

protected:
    BOOL OpenLogFile(void);
    BOOL CloseLogFile(void);
    void LogError(const char *pFormat, ...);
    void LogRemark(const char *pFormat, ...);
    void DumpTore(CTorDoor* pRefTor, CTorDoor* pTor, char* strRefFile = NULL);
    void DumpTor(CTorDoor* pTor);
    void DumpFluegel(CFlParam* pFl);
    void DumpProfil(CProfileElem* pE, char* strName);
    void DumpFuellung(CHolzElement* pE, char* strName);
    void DumpRiegel(CRiegelElement* pE);
    void DumpStatistic(void);
    void ShowLogFile(void);
};

#endif // !defined(AFX_TTTESTMANAGER_H__8E8BE7F1_8B9D_4B1D_A3D9_ABD8BC70739B__INCLUDED_)

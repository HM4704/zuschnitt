// ProcessList.h: interface for the CProcessList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSLIST_H__84E02564_D5EB_11D4_8E7B_D75CE8BADB7C__INCLUDED_)
#define AFX_PROCESSLIST_H__84E02564_D5EB_11D4_8E7B_D75CE8BADB7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ToolHelp.h"
#include "WinTop.h"
#include "RegKeyEx.h"

#undef MAX_COLUMNS
#define MAX_COLUMNS   11       //?? zu untersuchen: wenn auf 11 dann Absturz
#include "ColumnList.h"


///////////////////////////////////////////
// defines
#define MAX_PROCESSES       50      // fits dynamically when bigger


// Zeiten in ms fuer Refreshrate
#define RR_HIGH     1000
#define RR_MID      4000
#define RR_LOW      8000
#define RR_MANUAL   0

#define REFRESH_TIMER_ID     10

// private Flags
#define F_EXIST     0x01


///////////////////////////////////////////
// const
const char szProcess[]      =   _T("Prozess");
const char szPID[]          =   _T("PID");
const char szCntThreads[]   =   _T("Threads");
const char szCntUsage[]     =   _T("Usage");
const char szModId[]        =   _T("Module-ID");
const char szHeapId[]       =   _T("Heap-ID");
const char szParentPid[]    =   _T("Parent-PID");
const char szPrioClass[]    =   _T("Priority");
const char szCPUTime[]      =   _T("CPU Time");
const char szCPUPercent[]   =   _T("% CPU");
const char szFlags[]        =   _T("Flags");

const char szABOVE_NORMAL_PRIORITY_CLASS[]  = "ABOVE_NORMAL";
const char szBELOW_NORMAL_PRIORITY_CLASS[]  = "BELOW_NORMAL";
const char szHIGH_PRIORITY_CLASS[]          = "HIGH";
const char szIDLE_PRIORITY_CLASS[]          = "IDLE";
const char szNORMAL_PRIORITY_CLASS[]        = "NORMAL";
const char szREALTIME_PRIORITY_CLASS[]      = "REALTIME";
const char szUNKOWN_PRIORITY_CLASS[]        = "UNKOWN";


const char  szAppPath[]  = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msdev.exe";

///////////////////////////////////////////
// enums
enum ListItems
{
    process         = 0x0000,
    pid             = 0x0001,
    cntUsage        = 0x0004,
    moduleId        = 0x0008,
    defaultHeapId   = 0x0010,
    cntThreads      = 0x0020,
    parentPid       = 0x0040,
    prioClassBase   = 0x0080,
    cpuTime         = 0x0100,
    cpuPercent      = 0x0200,
    flags           = 0x0400
};

///////////////////////////////////////////
// structs
typedef struct tagPLPROCENTRY
{
    char        szExeFile[MAX_PATH];        // 260
    char        szCntUsage[5];              // 265
    char        szProcessID[20];            // 285
    char        szDefaultHeapID[20];        // 305
    char        szModuleID[20];             // 325
    char        szCntThreads[5];            // 330
    char        szParentProcessID[20];      // 350
    char        szPriClassBase[20];         // 370
    char        szFlags[5];                 // 375
    char        szCPUTime[20];
    char        szCPUPercent[4];
    DWORD       dwFlags;                // Flags fuer Listen-Verwaltung
} PLPROCENTRY;


///////////////////////////////////////////
// Class
class CProcessList : public CListCtrl  
{
public:
	BOOL Create(const CRect &rect, CWnd* pWnd, UINT uiId);
	CProcessList();
	virtual ~CProcessList();

// Operations
public:
	BOOL RegisterStatusBarInfo(CStatusBar* pBar, int iStrId);
	BOOL ClearAllSelections(void);
	BOOL RefreshColSubItems(int iSubItem, COLUMNLISTENTRY *pCol);
	BOOL GetColumnProperties(DWORD dwId, COLUMNLISTENTRY *pCol);
	BOOL AddColumn(DWORD dwBfCol);
	BOOL RemoveColumn(DWORD dwBfCol);
	DWORD GetColumnField(void);
	BOOL SetRefreshTime(DWORD dwTime);
	DWORD GetRefreshTime(void);
	BOOL GetSelectedItem(int& iItem);
	BOOL UpdateList(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessList)
	public:
	//}}AFX_VIRTUAL

private:
	BOOL CreateColumns(void);
	BOOL IsDebuggerInstalled(void);
	void ConvertProcEntriesToListEntries(PLPROCENTRY *pPE, PROCESSENTRY32 *m_pPL, 
        CPU_USAGE* pCuBuf, DWORD dwEntries);

private:
	BOOL SetupRegistryValues(void);
    CToolHelp       m_toolHelp;
    PLPROCENTRY    *m_pProcList;            // fuer ListCtrl
    PROCESSENTRY32 *m_pProcEntries;         // Win Struktur
    CPU_USAGE      *m_pCuEntries;           // für Prozess-Tickcounts
    DWORD           m_dwMaxProcs;
    DWORD           m_dwExistProcs;
    CRegistryItemDword  m_ridRefreshTime;        // Refresh-Rate in ms
    BOOL            m_bColumnsCreated;
    BYTE            *m_pListField;          // fuer Listenverwaltung

    CColumnList     m_ColsList;             // fuer Offset-Verwaltung
    DWORD           m_dwFieldsToShow;       // Bitfeld fuer Anzeige
    DWORD           m_dwCntColumns;         // Anzahl der Spalten
    int             m_iSelectedItem;        // ausgewählter Eintrag
    BOOL            m_bMustRefreshItems;    // Flag, ob Refresh nötig
    BOOL            m_bTimerCreated;        // Flag, ob Timer gestartet

    CRegKeyEx       m_Reg;                  // zur Abfrage der Registry
    BOOL            m_bMsdevInstalled;      // msdev.exe für Debuggen
    char            m_strMsdev[MAX_PATH];   // Pfad auf Msdev
    CRegistryItemDword  m_riFieldsToShow;   // Bitfeld in Registry

    // for status bar view
    CStatusBar     *m_pStatusBar;
    int             m_iStatusIndex;
    char            m_strStatusText[MAX_PATH];
    int             m_iStatusStrLen;

    HANDLE          m_hWintop;              // Handle auf Wintop-Treiber


	// Generated message map functions
protected:
	void UpdateStatusBar(void);
	BOOL SelectProcess(DWORD dwProcessId);
	inline void RefreshItem(int iItem);
	//{{AFX_MSG(CProcessList)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTerminate();
	afx_msg void OnUpdateTerminate(CCmdUI* pCmdUI);
	afx_msg void OnDebug();
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
    afx_msg LRESULT  OnSwitchToProcess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProcMemusage();
	afx_msg void OnUpdateProcMemusage(CCmdUI* pCmdUI);
	//}}AFX_MSG
    afx_msg void OnChangePriority(UINT nId);

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_PROCESSLIST_H__84E02564_D5EB_11D4_8E7B_D75CE8BADB7C__INCLUDED_)

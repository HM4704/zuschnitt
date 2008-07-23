// ToolHelp.h: interface for the CToolHelp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLHELP_H__948B40A4_D43B_11D4_8E7B_C96792A9F233__INCLUDED_)
#define AFX_TOOLHELP_H__948B40A4_D43B_11D4_8E7B_C96792A9F233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tlhelp32.h>


///////////////////////////////////////////
// defines
#define TA_FAILED           0   
#define TA_SUCCESS_CLEAN    1   
#define TA_SUCCESS_KILL     2
#define TA_SUCCESS_16       3

#define PRIO_NORMAL         8
#define PRIO_REALTIME       24
#define PRIO_HIGH           13
#define PRIO_IDLE           4

// Request Flags
#define RQU_HEAPSIZE        1
///////////////////////////////////////////
// structs
typedef struct tagAPPLICATION
{
    HWND        hWnd;
    char        strTitle[MAX_PATH];
    HINSTANCE   hInstance;
    HICON       hSmallIcon;
    BOOL        bResponding;
    DWORD       dwFlags;
} APPLICATION;


///////////////////////////////////////////
// callbacks
UINT ApplicationWalkThread( LPVOID pParam );


class CToolHelp : public CObject  
{
public:
	CToolHelp();
	virtual ~CToolHelp();

public:
    APPLICATION     *m_pApplications;       // Zeiger auf Speicher für Application-Scan
    DWORD           m_dwCurrentApp;         // aktuelle Position für App
    DWORD           m_dwMaxApps;            // maximale Größe für Application-Scan
    DWORD           m_dwExistApps;          // aktuelle Anzahl an Applications
    HANDLE          m_hExitAppWalk;
    HANDLE          m_hAppWalkThreadExited;
    HICON           m_hDefaultIcon;
    HICON           m_hDosIcon;
    HICON           m_hDefaultAppIcon;

private:
    CWinThread     *m_pApplicationThread;   // Thread für Task-Polling

public:
	BOOL GetProcessHeapsSize(DWORD dwPID, DWORD dwHeapId, DWORD *pdwSizes);
	BOOL ActivateApp(HWND hWnd);
	BOOL StartApplicationWalk(APPLICATION *pAppBuffer, DWORD dwMaxApps, 
        HICON hDefaultIcon = NULL);
	BOOL StopApplicationWalk(void);
	BOOL GetApplicationList(DWORD dwMaxEntries, APPLICATION *pApplications, DWORD &dwEntries);

	BOOL SetProcessPriority(DWORD dwPID, DWORD dwNewPriority);
	DWORD TerminateApp( DWORD dwPID, DWORD dwTimeout, HWND hWnd = NULL);
	BOOL RefreshThreadList (DWORD dwOwnerPID);

	BOOL GetProcessModule(DWORD dwPID, DWORD dwModuleID,      LPMODULEENTRY32 lpMe32, DWORD cbMe32);
	BOOL GetProcessList(DWORD dwMaxEntries, PROCESSENTRY32 *pProcEntry, DWORD &dwEntries, DWORD dwRequestFlags = 0);
	BOOL GetProcessInformation(PROCESSENTRY32 *pProcEntry);
};

#endif // !defined(AFX_TOOLHELP_H__948B40A4_D43B_11D4_8E7B_C96792A9F233__INCLUDED_)

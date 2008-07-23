// ToolHelp.cpp: implementation of the CToolHelp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolHelp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolHelp::CToolHelp()
{
    m_pApplicationThread = NULL;

    m_hDefaultIcon = NULL;

    m_hExitAppWalk = CreateEvent(NULL,
      FALSE,  // flag for manual-reset event
      FALSE, // flag for initial state
      NULL      // pointer to event-object name
      );
    
    m_hAppWalkThreadExited = CreateEvent(NULL,
      FALSE,  // flag for manual-reset event
      FALSE, // flag for initial state
      NULL      // pointer to event-object name
      );

    m_hDosIcon = NULL;
    ExtractIconEx("Pifmgr.dll", 0, NULL, &m_hDosIcon, 1);
    m_hDefaultAppIcon = NULL;
    ExtractIconEx("Shell32.dll", 2, NULL, &m_hDefaultAppIcon, 1);
}

CToolHelp::~CToolHelp()
{
}

BOOL CToolHelp::GetProcessList(DWORD dwMaxEntries, PROCESSENTRY32 *pProcEntry, DWORD &dwEntries,
                               DWORD dwRequestFlags)
{
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE;     

    dwEntries = 0;

    //  Take a snapshot of all processes in the system. 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == (HANDLE)-1)         
        return (FALSE);  

    //  Fill in the size of the structure before using it. 
    pProcEntry[dwEntries].dwSize = sizeof(PROCESSENTRY32);

    bRet = TRUE;     // Suppose, everything functions

    //  Walk the snapshot of the processes, and for each process, 
    //  display information.     
    if (Process32First(hProcessSnap, &pProcEntry[dwEntries]))     
    { 
        do         
        { 
            if (dwRequestFlags & RQU_HEAPSIZE)
            {
                pProcEntry[dwEntries].dwSize = 0;
                GetProcessHeapsSize(pProcEntry[dwEntries].th32ProcessID,
                    pProcEntry[dwEntries].th32DefaultHeapID,
                    &pProcEntry[dwEntries].dwSize);
                pProcEntry[dwEntries].dwSize = sizeof(PROCESSENTRY32);
            }
            else
            {
            }
            dwEntries++;
            pProcEntry[dwEntries].dwSize = sizeof(PROCESSENTRY32);
            // folgende Zeile wird gebraucht, sonst Fehler ERROR_INVALID_DATA
            pProcEntry[dwEntries].th32ProcessID = pProcEntry[dwEntries-1].th32ProcessID;
        }         
        while (Process32Next(hProcessSnap, &pProcEntry[dwEntries]) && 
            (dwEntries < dwMaxEntries)); 
        if (dwEntries >= dwMaxEntries)
        {
            PROCESSENTRY32 pe32      = {0};
            pe32.dwSize = sizeof(PROCESSENTRY32);  
            // Scan only for Count of Processes
            while (Process32Next(hProcessSnap, &pe32)) 
            { 
                    dwEntries++;        // Count for Num Processes
            }         
        }
    }     
    else 
        bRet = FALSE;    // could not walk the list of processes  

    // Do not forget to clean up the snapshot object. 
    CloseHandle (hProcessSnap);     
    return (bRet); 
}

BOOL CToolHelp::GetProcessModule(DWORD dwPID, DWORD dwModuleID, LPMODULEENTRY32 lpMe32, DWORD cbMe32)
{
    BOOL          bRet        = FALSE;     
    BOOL          bFound      = FALSE; 
    HANDLE        hModuleSnap = NULL;     
    MODULEENTRY32 me32        = {0};  

    // Take a snapshot of all modules in the specified process. 
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 

    if (hModuleSnap == (HANDLE)-1)         
        return (FALSE);  

    // Fill the size of the structure before using it. 
    me32.dwSize = sizeof(MODULEENTRY32);  
    // Walk the module list of the process, and find the module of 
    // interest. Then copy the information to the buffer pointed 
    // to by lpMe32 so that it can be returned to the caller. 
    if (Module32First(hModuleSnap, &me32))     
    {         
        do         
        { 
            if (me32.th32ModuleID == dwModuleID)             
            { 
                CopyMemory (lpMe32, &me32, cbMe32); 
                bFound = TRUE;             
            }         
        } 
        while (!bFound && Module32Next(hModuleSnap, &me32));  

        bRet = bFound;   // if this sets bRet to FALSE, dwModuleID 
                         // no longer exists in specified process     
    } 
    else         
        bRet = FALSE;           // could not walk module list  

    // Do not forget to clean up the snapshot object. 
    CloseHandle (hModuleSnap);      
    return (bRet); 
}

BOOL CToolHelp::GetProcessHeapsSize(DWORD dwPID, DWORD dwHeapId,
                                    DWORD *pdwSizes)
{
    HEAPENTRY32 entry;
    entry.dwSize = sizeof(entry);
    if (Heap32First(&entry, dwPID, dwHeapId) == FALSE)
        return FALSE;
    *pdwSizes = entry.dwBlockSize;
    while (Heap32Next(&entry))
    {
        *pdwSizes += entry.dwBlockSize;
    }
    if (GetLastError() == ERROR_NO_MORE_FILES)
        return TRUE;
    else
        return FALSE;
}

BOOL CToolHelp::RefreshThreadList(DWORD dwOwnerPID)
{     
    HANDLE        hThreadSnap = NULL;     
    BOOL          bRet        = FALSE; 
    THREADENTRY32 te32        = {0};  

    // Take a snapshot of all threads currently in the system. 
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == (HANDLE)-1)         
        return (FALSE);  

    // Fill in the size of the structure before using it. 
    te32.dwSize = sizeof(THREADENTRY32);  

    // Walk the thread snapshot to find all threads of the process. 
    // If the thread belongs to the process, add its information 
    // to the display list.     
    if (Thread32First(hThreadSnap, &te32))     
    { 
        do         
        {             
            if (te32.th32OwnerProcessID == dwOwnerPID) 
            {                 
                printf( "\nTID\t\t%d\n", te32.th32ThreadID); 
                printf( "Owner PID\t%d\n", te32.th32OwnerProcessID); 
                printf( "Delta Priority\t%d\n", te32.tpDeltaPri); 
                printf( "Base Priority\t%d\n", te32.tpBasePri);             
            } 
        }         
        while (Thread32Next(hThreadSnap, &te32)); 
        bRet = TRUE;     
    }     
    else 
        bRet = FALSE;          // could not walk the list of threads  

    // Do not forget to clean up the snapshot object. 
    CloseHandle (hThreadSnap);      
    return (bRet); 
}

BOOL CToolHelp::GetProcessInformation(PROCESSENTRY32 *pProcEntry)
{
    DWORD         dwPriorityClass; 
    BOOL          bGotModule = FALSE; 
    MODULEENTRY32 me32       = {0};          

    bGotModule = GetProcessModule(pProcEntry->th32ProcessID, 
    pProcEntry->th32ModuleID, &me32, sizeof(MODULEENTRY32)); 

    if (bGotModule)             
    {                 
        HANDLE hProcess;  

        // Get the actual priority class. 
        hProcess = OpenProcess (PROCESS_ALL_ACCESS, 
            FALSE, pProcEntry->th32ProcessID); 
        dwPriorityClass = GetPriorityClass (hProcess); 
        CloseHandle (hProcess); 

        strcpy(pProcEntry->szExeFile, me32.szModule);
    }
    else
        return FALSE;

    return TRUE;
}

BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )   
{      
    DWORD dwID ;
      
    GetWindowThreadProcessId(hwnd, &dwID) ;      
    if(dwID == (DWORD)lParam)
    {         
        PostMessage(hwnd, WM_CLOSE, 0, 0) ;      
    }      
    
    return TRUE ;
}

DWORD CToolHelp::TerminateApp(DWORD dwPID, DWORD dwTimeout, HWND hWnd)
{
    HANDLE   hProc ;      
    DWORD   dwRet ;
    DWORD   dwId;
     
    if (hWnd != NULL)
    {
        GetWindowThreadProcessId(hWnd, &dwId);      

        // If we can't open the process with PROCESS_TERMINATE rights,      
        // then we give up immediately.
        hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwId);
      
        if(hProc == NULL)      
        {         
            return TA_FAILED ;      
        }

        PostMessage(hWnd, WM_CLOSE, 0, 0);      
    }
    else
    {
        // If we can't open the process with PROCESS_TERMINATE rights,      
        // then we give up immediately.
        hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwPID);
      
        if(hProc == NULL)      
        {         
            return TA_FAILED ;      
        }
        // TerminateAppEnum() posts WM_CLOSE to all windows whose PID      
        // matches your process's.
        EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM)dwPID);
    }

    // Wait on the handle. If it signals, great. If it times out,
    // then you kill it.
    if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
         dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);      
    else
         dwRet = TA_SUCCESS_CLEAN ;      

    CloseHandle(hProc);
    
    return dwRet ;
}

BOOL CToolHelp::ActivateApp(HWND hWnd)
{
    BOOL bRet;
#if 1    
    HWND hCurrWnd = GetActiveWindow();
    if (hWnd == hCurrWnd)
        return TRUE;
    bRet = PostMessage(hCurrWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    bRet = PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    SetForegroundWindow(hWnd);
#else
    LPARAM lParam = MAKELONG(HTCAPTION, WM_MOUSEACTIVATE);
    bRet = SendMessage(hWnd, WM_MOUSEACTIVATE, (WPARAM)hWnd, lParam);
    //MA_ACTIVATEANDEAT
#endif
    return bRet;
}


BOOL CToolHelp::SetProcessPriority(DWORD dwPID, DWORD dwNewPriority)
{
    HANDLE   hProc ;  
    BOOL     bRet = FALSE;
      
    hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
      
    if(hProc == NULL)      
    {         
        return FALSE;      
    }

    bRet = SetPriorityClass(hProc, dwNewPriority);
    if (bRet == FALSE)
    {
    }
    CloseHandle(hProc);
    return bRet;
}

BOOL CALLBACK ListExistWindows(  HWND hwnd, LPARAM lParam )
{
#define CONSOLE_CLASS_NAME "tty"

    LONG lStyle, lStyleEx;
    char* strTitle;
    char  strName[MAX_PATH];
    CToolHelp* pToolHelp = (CToolHelp*)lParam;
    
    lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyleEx = GetWindowLong(hwnd, GWL_EXSTYLE);
    if ((lStyle & (WS_OVERLAPPED | WS_OVERLAPPEDWINDOW))
        && !(lStyleEx & WS_EX_TOOLWINDOW))
    {
        // is it a main window
        if (lStyle & WS_VISIBLE)
        {
            // is it visible

            HINSTANCE   hInstance;
            HICON hSmallIcon;
            if ((hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE)) == NULL)
                return TRUE;

            if ((hSmallIcon = (HICON)GetClassLong(hwnd, GCL_HICONSM)) == NULL)
            {
                if (GetClassName(hwnd, strName, MAX_PATH) != (int)strlen(CONSOLE_CLASS_NAME))
                    hSmallIcon = pToolHelp->m_hDefaultAppIcon;
                else
                    if (strcmp(strName, CONSOLE_CLASS_NAME))
                        hSmallIcon = pToolHelp->m_hDefaultAppIcon;
                    else
                        // console app
                        hSmallIcon = pToolHelp->m_hDosIcon;
            }

            pToolHelp->m_pApplications[pToolHelp->m_dwCurrentApp].hInstance = hInstance;
            pToolHelp->m_pApplications[pToolHelp->m_dwCurrentApp].hSmallIcon = hSmallIcon;
            pToolHelp->m_pApplications[pToolHelp->m_dwCurrentApp].hWnd = hwnd;
            GetWindowText(hwnd, pToolHelp->m_pApplications[pToolHelp->m_dwCurrentApp].strTitle,
                MAX_PATH);
            strTitle = pToolHelp->m_pApplications[pToolHelp->m_dwCurrentApp].strTitle;
            pToolHelp->m_pApplications[pToolHelp->m_dwCurrentApp].dwFlags = 0;
            pToolHelp->m_dwCurrentApp++;
            if (pToolHelp->m_dwCurrentApp >= pToolHelp->m_dwMaxApps)
                return FALSE;  // stop enum
        }
    }
    return TRUE;
}

BOOL CToolHelp::GetApplicationList(DWORD dwMaxEntries, APPLICATION *pApplications, DWORD &dwExistEntries)
{
    pApplications; dwMaxEntries;  // to avoid warnings

    if (m_pApplicationThread == NULL)
        return FALSE;

    dwExistEntries = m_dwExistApps;

    return TRUE;
}

BOOL CToolHelp::StartApplicationWalk(APPLICATION *pAppBuffer, DWORD dwMaxApps, 
                                     HICON hDefaultIcon)
{
    if (m_pApplicationThread != NULL)
        return TRUE;

    m_pApplications = pAppBuffer;
    m_dwMaxApps = dwMaxApps;
    m_dwExistApps = m_dwCurrentApp = 0;
    m_hDefaultIcon = hDefaultIcon;
    if (m_hDefaultAppIcon == NULL)
        m_hDefaultAppIcon = hDefaultIcon;
    if (m_hDosIcon == NULL)
        m_hDosIcon = hDefaultIcon;

    ResetEvent(m_hAppWalkThreadExited);

    if (m_hExitAppWalk == NULL || m_hAppWalkThreadExited == NULL)
        return FALSE;

    if ((m_pApplicationThread = AfxBeginThread(ApplicationWalkThread, (LPVOID) this, THREAD_PRIORITY_LOWEST,
        0, 0, NULL)) == NULL)
        return FALSE;

    // eventuell warten, bis erster Durchlauf abgeschlossen

    return TRUE;
}

BOOL CToolHelp::StopApplicationWalk()
{
    if (m_pApplicationThread== NULL)
        return TRUE;

    SetEvent(m_hExitAppWalk);

    WaitForSingleObject(m_hAppWalkThreadExited, INFINITE);

    m_pApplicationThread = NULL;

    ResetEvent(m_hAppWalkThreadExited);
    ResetEvent(m_hExitAppWalk);

    return TRUE;
}

UINT ApplicationWalkThread( LPVOID pParam )
{
    BOOL bMustExit;
    DWORD dwTimeout;
    CToolHelp* pToolHelp = (CToolHelp*)pParam;

    bMustExit = FALSE;
    do
    {
        dwTimeout = 5000;

        // nach Apps suchen
        pToolHelp->m_dwCurrentApp = 0;
        EnumWindows(ListExistWindows, (LPARAM)pToolHelp);
        pToolHelp->m_dwExistApps = pToolHelp->m_dwCurrentApp;

        // jede App testen
        DWORD dwResult;
        for (DWORD i = 0; i < pToolHelp->m_dwExistApps; i++)
        {
            // bResponding is TRUE if the thread is responding and 
            // FALSE if not.
             if ((pToolHelp->m_pApplications[i].bResponding = 
                 SendMessageTimeout(pToolHelp->m_pApplications[i].hWnd, WM_NULL, 0, 0, 
                 SMTO_ABORTIFHUNG, 5000, &dwResult)) == 0)
             {
                 if (GetLastError() == 0)
                     // 0 == Timeout
                     dwTimeout = 0;
             }
        }

        if (WaitForSingleObject(pToolHelp->m_hExitAppWalk, dwTimeout) == WAIT_OBJECT_0)
            bMustExit = TRUE;
    }
    while (bMustExit == FALSE);

    HANDLE hThread = GetCurrentThread();
    SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
    SetEvent(pToolHelp->m_hAppWalkThreadExited);

    AfxEndThread(0);

    return 0;
}

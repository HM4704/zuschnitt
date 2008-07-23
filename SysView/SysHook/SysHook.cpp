// SysHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "math.h"
#include "SysHook.h"

///////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
// This function forces the debugger to be invoked
void ForceDebugBreak() {
   __try { DebugBreak(); }
   __except(UnhandledExceptionFilter(GetExceptionInformation())) { }
}
#else
#define ForceDebugBreak()
#endif


///////////////////////////////////////////////////////////////////////////////

#define  ID_START_APP              0xff00
#define  MENUITEM_NAME             "Taskmanager..."

#define  WIN9X_SHELLTRAY_CLASS       "Shell_TrayWnd" //"#32770"   
#define  WIN9X_SHELLTRAY_WINDOW      ""             //"ToolSim"  
#define  WIN9X_TASKBAR_CLASS         "MSTaskSwWClass"
#define  WIN9X_TASKBAR_WINDOW        ""



// Instruct the compiler to put the g_hhook data variable in 
// its own data section called Shared. We then instruct the 
// linker that we want to share the data in this section 
// with all instances of this application.

#pragma data_seg("Shared")
char  g_strAppToStart[MAX_PATH] = {0};
char  g_strAppPath[MAX_PATH] = {0};
int   g_iItemNum = 0;
HWND g_hTaskBar = NULL;
HWND g_hShellTray = NULL;
HHOOK g_hCallProcRetHook = NULL;
HHOOK g_hMsgFilterHook = NULL;
#pragma data_seg()


// Instruct the linker to make the Shared section
// readable, writable, and shared.
#pragma comment(linker, "/section:Shared,rwskpe")


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

char* GetCurrDir(char* strApp)
{
    char *pSlash;
    strcpy(g_strAppPath, strApp);
    pSlash = strrchr(g_strAppPath, '\\');
    if (pSlash)
    {
        *pSlash = 0;
        return g_strAppPath;
    }

    return NULL;
}
        
void StartTaskmanager(void)
{
    STARTUPINFO startUpInfo;
    PROCESS_INFORMATION processInfo;
    memset(&startUpInfo, 0, sizeof(startUpInfo));
    startUpInfo.cb = sizeof(startUpInfo);
    startUpInfo.dwFlags = 0;

    if ( CreateProcess(NULL, g_strAppToStart, NULL, NULL,
        FALSE, 0, NULL, NULL, /*GetCurrDir(g_strAppToStart),*/ 
        &startUpInfo, &processInfo) == FALSE )
    {
        DWORD dwErr = GetLastError();
    }
}

BOOL InsertTaskmanagerItem(HMENU hmenuPopup, int iPos)
{
    BOOL bRet = TRUE;

    MENUITEMINFO mInfo;
    mInfo.cbSize = sizeof(MENUITEMINFO);
    mInfo.fMask = MIIM_TYPE | MIIM_ID;
    mInfo.fType = MFT_STRING;
    mInfo.dwTypeData = (char*)MENUITEM_NAME;
    mInfo.cch = strlen(MENUITEM_NAME);
    mInfo.wID = ID_START_APP;


    bRet = InsertMenuItem(hmenuPopup, iPos /*item*/,  TRUE /*Position*/, 
            &mInfo);
    if (bRet)
    {
        // Insert separator
        mInfo.fMask = MIIM_TYPE;
        mInfo.fType = MFT_SEPARATOR;
        bRet = InsertMenuItem(hmenuPopup, iPos+1 /*item*/,  TRUE /*Position*/, 
                &mInfo);
    }

    return bRet;
}


BOOL IsTaskbarMenu(HMENU hmenuPopup, int iLastItem)
{
#define PROP_GERMAN "E&igensch"
#define PROP_ENGLISH "P&rop"

    MENUITEMINFO mInfo;
    char cBuf[MAX_PATH];

    mInfo.cbSize = sizeof(MENUITEMINFO);
    mInfo.fMask = MIIM_TYPE;
    mInfo.fType = MFT_STRING;
    mInfo.dwTypeData = (char*)cBuf;
    mInfo.cch = sizeof(cBuf);

    if (GetMenuItemInfo(hmenuPopup, iLastItem, TRUE, &mInfo))
    {
        if ((strncmp(cBuf, PROP_GERMAN, strlen(PROP_GERMAN)) == 0)
            || (strncmp(cBuf, PROP_ENGLISH, strlen(PROP_ENGLISH)) == 0))
            return TRUE;
    }

    return FALSE;
}


// installiert
LRESULT CALLBACK MsgFilterProc(  int code,/* hook code */
                        WPARAM wParam, LPARAM lParam)
{
    static RECT itemRect;
    static BOOL bMenuSelected = FALSE;

    MSG *msg = (MSG*) lParam;
    if ((code < HC_ACTION) || (msg == NULL))
    {
        if (g_hMsgFilterHook)
            return CallNextHookEx(g_hMsgFilterHook, code, wParam, lParam);
        else
            return 0L;
    }

    switch (msg->message)
    {
    case WM_LBUTTONUP:
        // hier kommt rein bei Menuselect
//        ForceDebugBreak();
        if (bMenuSelected == TRUE)
        {
            StartTaskmanager();
            bMenuSelected = FALSE;
        }
        break;
    case WM_MENUSELECT :
        if (ID_START_APP == LOWORD(msg->wParam))
        {
            bMenuSelected = TRUE;
            if (GetMenuItemRect(msg->hwnd, (HMENU)msg->lParam, g_iItemNum, &itemRect) == FALSE)
            {
                DWORD dwErr = GetLastError();
            }
        }
        else
            bMenuSelected = FALSE;
        break;
    default:
        break;
    }

    if (g_hMsgFilterHook)
        return CallNextHookEx(g_hMsgFilterHook, code, wParam, lParam);
    return 
        0L;
}


// installiert
LRESULT CALLBACK CallWndRetProc(  int code, WPARAM wParam, LPARAM lParam)
{
    CWPRETSTRUCT *msg = (CWPRETSTRUCT*) lParam;
    if ((code != HC_ACTION) || (msg == NULL))
    {
        if (g_hCallProcRetHook)
            return CallNextHookEx(g_hCallProcRetHook, code, wParam, lParam);
        else
            return 0L;
    }

    switch (msg->message)
    {
    case WM_INITMENUPOPUP:
        {
            BOOL bSystemMenu = (BOOL) HIWORD(msg->lParam);
            if (bSystemMenu == FALSE)
            {
//                ForceDebugBreak();
                HMENU hmenuPopup = (HMENU) msg->wParam;
                if (hmenuPopup != NULL)
                {
                    UINT uPos = (UINT) LOWORD(msg->lParam);
                    g_iItemNum= GetMenuItemCount(hmenuPopup);
                    g_iItemNum--;
                    if (IsTaskbarMenu(hmenuPopup, g_iItemNum))
                    {
                        // insert before Eigenschaften
                        InsertTaskmanagerItem(hmenuPopup, g_iItemNum);
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    if (g_hCallProcRetHook)
        return CallNextHookEx(g_hCallProcRetHook, code, wParam, lParam);
    else
        return 0L;
}



SYSHOOK_API BOOL InstallHooks(char* strAppToStart)
{
    HINSTANCE   hMod;
    DWORD       dwThreadId, dwShellTrayThreadId;

    if (g_hMsgFilterHook != NULL || g_hCallProcRetHook != NULL)
        return FALSE;

    strcpy(g_strAppToStart, strAppToStart);

    if ((g_hShellTray = FindWindow(WIN9X_SHELLTRAY_CLASS, WIN9X_SHELLTRAY_WINDOW)) 
        == NULL)
        return FALSE;
    
    if ((g_hTaskBar = FindWindowEx(g_hShellTray, NULL, WIN9X_TASKBAR_CLASS, WIN9X_TASKBAR_WINDOW))
        == NULL)
        g_hTaskBar = g_hShellTray;
 
    if ((hMod = GetModuleHandle("SysHook.dll")) == NULL)
        return FALSE;

    dwThreadId = GetWindowThreadProcessId(g_hTaskBar, NULL);
    dwShellTrayThreadId = GetWindowThreadProcessId(g_hShellTray, NULL);

    if ((g_hCallProcRetHook = SetWindowsHookEx(WH_CALLWNDPROCRET,
                    CallWndRetProc,     // pointer to hook procedure
                    hMod,    // handle to application instance
                    dwThreadId   // identity of thread to install hook for
                    )) == NULL)
    {
        DWORD dwErr = GetLastError();
        return FALSE;
    }


    if ((g_hMsgFilterHook = SetWindowsHookEx(WH_MSGFILTER,
                    MsgFilterProc,     // pointer to hook procedure
                    hMod,    // handle to application instance
                    dwThreadId   // identity of thread to install hook for
                    )) == NULL)
    {
        DWORD dwErr = GetLastError();
        return FALSE;
    }

	return TRUE;
}


SYSHOOK_API BOOL UnInstallHooks(void)
{
    BOOL bRet;
    if (g_hMsgFilterHook!= NULL)
    {
        bRet = UnhookWindowsHookEx(g_hMsgFilterHook);
        g_hMsgFilterHook = NULL;
    }

    if (g_hCallProcRetHook!= NULL)
    {
        bRet = UnhookWindowsHookEx(g_hCallProcRetHook);
        g_hCallProcRetHook = NULL;
    }

    return bRet;
}

SYSHOOK_API BOOL IsHookInstalled(void)
{
    return ((g_hMsgFilterHook!= NULL) && (g_hCallProcRetHook!= NULL));
}

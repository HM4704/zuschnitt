// HtmlHelper.cpp: implementation of the CHtmlHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HtmlHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static RECT g_Rect;
static BOOL g_bWndFound;

CHtmlHelper::CHtmlHelper()
{

}

CHtmlHelper::~CHtmlHelper()
{

}

CHtmlHelper::CHtmlHelper(const char* strFileDirectory)
{
    strcpy(cFileDirectory, strFileDirectory);
}

BOOL CHtmlHelper::OpenDoc(const char *strHtmlFile, PLACING place, RECT *pRect)
{
    CString strFile = cFileDirectory;

    strFile += '\\';
    strFile += strHtmlFile;

    SHELLEXECUTEINFO ExecInfo;
    memset(&ExecInfo, 0, sizeof(SHELLEXECUTEINFO));
    ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
    ExecInfo.lpVerb = "open";
    ExecInfo.lpFile = strFile;
    ExecInfo.lpDirectory = cFileDirectory;
    if (ShellExecuteEx(&ExecInfo) == TRUE)
    {
        if (place == PLACE_NOMATTER)
            return TRUE;
        if (place == PLACE_POS && pRect)
        {
            g_Rect = *pRect;
            // Move Browser
            PositionWindow(pRect, strHtmlFile, ExecInfo.hProcess);
        }
        else
        {
            // Not yet implemented
        }

    }
    else
        return FALSE;

    return TRUE;
}

static BOOL CALLBACK PosWindow( HWND hwnd, LPARAM lParam )   
{      
    char cBuf[1000];
    
    GetWindowText(hwnd, cBuf, 1000);
    char* szTitle = (char*) lParam;
    if(strstr(cBuf, szTitle) != NULL)
    {         
/*
        CRect rect;
        int width = ::GetSystemMetrics(SM_CXSCREEN); 
        int height = ::GetSystemMetrics(SM_CYSCREEN);
        rect.left = width/2;
        rect.right = width-10;
        rect.top = 10;
        rect.bottom = (int)((float)height*8/9);
*/
        int iWidth = g_Rect.right-g_Rect.left;
        int iHeight = g_Rect.bottom - g_Rect.top;
        ::MoveWindow(hwnd, g_Rect.left, g_Rect.top, iWidth, 
            iHeight, TRUE);
        g_bWndFound = TRUE;
    }      
    
    return TRUE ;
}

BOOL CHtmlHelper::PositionWindow(RECT *pRect, const char *strWndNamePart,
                                 HANDLE hWndProcess)
{
    if (hWndProcess != NULL)
    {
        WaitForInputIdle(hWndProcess, 20000);
    }
    g_Rect = *pRect;
    g_bWndFound = FALSE;
    int i = 0;
    do 
    {
        EnumWindows(PosWindow, (LPARAM)strWndNamePart);
        if (g_bWndFound == FALSE)
            Sleep(1000);
        i++;
    }
    while (i<10 && g_bWndFound == FALSE);

    return g_bWndFound;
}

void CHtmlHelper::SetFileDirectory(const char* strNewDir)
{
    strcpy(cFileDirectory, strNewDir);
}

char* CHtmlHelper::GetFileDirectory()
{
    return cFileDirectory;
}

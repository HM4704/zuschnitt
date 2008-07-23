// ToolHelp.cpp: implementation of the CToolHelp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "TrayIcon.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon()
{
    m_bCreated = FALSE;
    memset(&m_notIconData, 0, sizeof(m_notIconData));
}


CTrayIcon::~CTrayIcon()
{
    if (m_bCreated == TRUE)
        Close();
}


BOOL CTrayIcon::Create(CWnd *pWnd, UINT uCallbMsg, UINT uId, 
                       HICON hIcon, char *strTip)
{
    m_notIconData.cbSize = sizeof(NOTIFYICONDATA);
    m_notIconData.hWnd = pWnd->GetSafeHwnd();
    m_notIconData.uCallbackMessage = uCallbMsg;
    m_notIconData.uID = uId;
    m_notIconData.hIcon = hIcon;
    lstrcpyn(m_notIconData.szTip, strTip, sizeof(m_notIconData.szTip));
    m_notIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

    m_bCreated = Shell_NotifyIcon(NIM_ADD, &m_notIconData);
    return m_bCreated;
}


BOOL CTrayIcon::Modify(HICON hIcon, char *strTip)
{

    m_notIconData.hIcon = hIcon;
    lstrcpyn(m_notIconData.szTip, strTip, sizeof(m_notIconData.szTip));
    m_notIconData.uFlags = NIF_ICON | NIF_TIP;

    return Shell_NotifyIcon(NIM_MODIFY, &m_notIconData);
}


BOOL CTrayIcon::Close()
{
    if (Shell_NotifyIcon(NIM_DELETE, &m_notIconData) == TRUE)
        m_bCreated = FALSE;

    return !m_bCreated;
}

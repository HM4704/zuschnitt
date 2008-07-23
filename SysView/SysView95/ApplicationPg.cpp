// TaskPg.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "Wintop.h"
#include "ApplicationPg.h"
#include "..\SysHook\SysHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApplicationPg property page

IMPLEMENT_DYNCREATE(CApplicationPg, CSysBasePage)

CApplicationPg::CApplicationPg() : CSysBasePage(IDR_TASK_PAGE, CApplicationPg::IDD)
{
	//{{AFX_DATA_INIT(CApplicationPg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CApplicationPg::~CApplicationPg()
{
//    UnInstallHooks();
}

void CApplicationPg::DoDataExchange(CDataExchange* pDX)
{
	CSysBasePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicationPg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplicationPg, CSysBasePage)
	//{{AFX_MSG_MAP(CApplicationPg)
	ON_WM_SIZE()
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateRefresh)
	ON_COMMAND(ID_START_APPWALK, OnStartAppwalk)
	ON_UPDATE_COMMAND_UI(ID_START_APPWALK, OnUpdateStartAppwalk)
	ON_COMMAND(ID_STOP_APPWALK, OnStopAppwalk)
	ON_UPDATE_COMMAND_UI(ID_STOP_APPWALK, OnUpdateStopAppwalk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_END_TASK, OnEndTask)
	ON_BN_CLICKED(IDC_ACTIVATE_TASK, OnActivateTask)
	ON_COMMAND(ID_HOOK, OnHook)
	ON_UPDATE_COMMAND_UI(ID_HOOK, OnUpdateHook)
	ON_COMMAND(ID_UNHOOK, OnUnhook)
	ON_UPDATE_COMMAND_UI(ID_UNHOOK, OnUpdateUnhook)
	ON_MESSAGE(MSG_SWITCH_TO_PROCESS, OnSwitchToProcess)
	ON_COMMAND(ID_RR_HIGH, OnRefreshHigh)
	ON_UPDATE_COMMAND_UI(ID_RR_HIGH, OnUpdateRefreshHigh)
	ON_COMMAND(ID_RR_LOW, OnRefreshLow)
	ON_UPDATE_COMMAND_UI(ID_RR_LOW, OnUpdateRefreshLow)
	ON_COMMAND(ID_RR_MID, OnRefreshMid)
	ON_UPDATE_COMMAND_UI(ID_RR_MID, OnUpdateRefreshMid)
	ON_COMMAND(ID_OPEN_WINTOP, OnOpenWintop)
	ON_UPDATE_COMMAND_UI(ID_OPEN_WINTOP, OnUpdateOpenWintop)
	ON_COMMAND(ID_START_WINTOP, OnStartWintop)
	ON_UPDATE_COMMAND_UI(ID_START_WINTOP, OnUpdateStartWintop)
	ON_COMMAND(ID_DATA_WINTOP, OnMemoryWintop)
	ON_UPDATE_COMMAND_UI(ID_DATA_WINTOP, OnUpdateMemoryWintop)
	ON_COMMAND(ID_ALL_WINTOP, OnTimesWintop)
	ON_UPDATE_COMMAND_UI(ID_ALL_WINTOP, OnUpdateTimesWintop)
	ON_COMMAND(ID_MANUAL, OnRefreshManual)
	ON_UPDATE_COMMAND_UI(ID_MANUAL, OnUpdateRefreshManual)
	ON_COMMAND(ID_DEBUG1, OnDebug1)
	ON_UPDATE_COMMAND_UI(ID_DEBUG1, OnUpdateDebug1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplicationPg message handlers

BOOL CApplicationPg::InitializeList(UINT dwIDC_Frame)
{
    CRect rect;

    GetDlgItem(dwIDC_Frame)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    rect.left += 3;
    rect.top += 3;
    rect.right -= 3;
    rect.bottom -= 3;
    return m_AppList.Create(rect, this, dwIDC_Frame);
}

BOOL CApplicationPg::OnInitDialog() 
{
	CSysBasePage::OnInitDialog();
	
	InitializeList(IDC_TASKLISTFRAME);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CApplicationPg::OnSize(UINT nType, int cx, int cy) 
{
	CSysBasePage::OnSize(nType, cx, cy);
	
	if (nType == SIZE_RESTORED)
    {
        CRect rectClient;
        GetWindowRect(&rectClient);
        ScreenToClient(&rectClient);
        PlaceItems(rectClient);    
    }	
}

BOOL CApplicationPg::PlaceItems(CRect &rect)
{
    if (CSysBasePage::PlaceItems(rect) == FALSE)
        return FALSE;

    CRect rectEndTask, rectActTask, rc;
    CWnd *pWndFrame, *pWndEndTask, *pWndActTask;

    pWndEndTask = GetDlgItem(IDC_END_TASK);
    if (pWndEndTask)
    {
        pWndEndTask->GetWindowRect(&rectEndTask);
        ScreenToClient(&rectEndTask);
    }
    pWndActTask = GetDlgItem(IDC_ACTIVATE_TASK);
    if (pWndActTask)
    {
        pWndActTask->GetWindowRect(&rectActTask);
        ScreenToClient(&rectActTask);
    }

    rc = rectActTask;
    rc.bottom = rect.bottom-10; rc.top = rc.bottom-rectActTask.Height();  // nach oben verschieben
    rc.right = rect.right - 20; rc.left = rc.right - rectActTask.Width(); // am rechten Rand aligned
    pWndActTask->MoveWindow(&rc);

    rc.left -= 10 + rectEndTask.Width(); rc.right = rc.left+rectEndTask.Width();
    pWndEndTask->MoveWindow(&rc);

    pWndFrame = GetDlgItem(IDC_TASKLISTFRAME);
    
    rect.left += 20;
    rect.top += 20;
    rect.right -= 20;
    rect.bottom -= (rc.Height() + 20);
//    rect.bottom -= 20;
    pWndFrame->MoveWindow(&rect);

    rect.left += 3;
    rect.top += 3;
    rect.right -= 3;
    rect.bottom -= 3;
    m_AppList.MoveWindow(&rect);

    return TRUE;
}

void CApplicationPg::OnRefresh() 
{
    m_AppList.UpdateList();
}

void CApplicationPg::OnUpdateRefresh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CApplicationPg::OnStartAppwalk() 
{
	m_AppList.StartApplicationWalk();
}

void CApplicationPg::OnUpdateStartAppwalk(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CApplicationPg::OnStopAppwalk() 
{
	m_AppList.StopApplicationWalk();	
}

void CApplicationPg::OnUpdateStopAppwalk(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CApplicationPg::OnDestroy() 
{
	CSysBasePage::OnDestroy();	
}

void CApplicationPg::OnEndTask() 
{
	m_AppList.EndTask();
}

void CApplicationPg::OnActivateTask() 
{
    m_AppList.ActivateTask();	
}

void CApplicationPg::OnHook() 
{
    char cBufDir[MAX_PATH];

    if (GetModuleFileName(NULL, cBufDir, sizeof(cBufDir)) != 0)
    {
        InstallHooks(cBufDir);	
    }
}

void CApplicationPg::OnUpdateHook(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();		
}

void CApplicationPg::OnUnhook() 
{
    UnInstallHooks();
}

void CApplicationPg::OnUpdateUnhook(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();			
}

LRESULT  CApplicationPg::OnSwitchToProcess(WPARAM wParam, LPARAM lParam)
{
    CWnd* pWnd = GetParent();
    if (pWnd)
    {
        pWnd->PostMessage(MSG_SWITCH_TO_PROCESS,
            wParam, lParam);
    }
    
    return 0L;
}

BOOL CApplicationPg::OnSetActive() 
{
    /* Force select */
    m_AppList.PostMessage(WM_MBUTTONDOWN , MK_MBUTTON, 0);
    m_AppList.PostMessage(WM_MBUTTONUP, 0, 0);
	return CSysBasePage::OnSetActive();
}


void CApplicationPg::OnRefreshHigh() 
{
    if (m_AppList.GetRefreshTime() != RR_HIGH)
        m_AppList.SetRefreshTime(RR_HIGH);		
}

void CApplicationPg::OnUpdateRefreshHigh(CCmdUI* pCmdUI) 
{
    if (m_AppList.GetRefreshTime() == RR_HIGH)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);		
}

void CApplicationPg::OnRefreshLow() 
{
    if (m_AppList.GetRefreshTime() != RR_LOW)
        m_AppList.SetRefreshTime(RR_LOW);			
}

void CApplicationPg::OnUpdateRefreshLow(CCmdUI* pCmdUI) 
{
    if (m_AppList.GetRefreshTime() == RR_LOW)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);			
}

void CApplicationPg::OnRefreshMid() 
{
    if (m_AppList.GetRefreshTime() != RR_MID)
        m_AppList.SetRefreshTime(RR_MID);			
}

void CApplicationPg::OnUpdateRefreshMid(CCmdUI* pCmdUI) 
{
    if (m_AppList.GetRefreshTime() == RR_MID)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);			
}

void CApplicationPg::OnRefreshManual() 
{
    if (m_AppList.GetRefreshTime() != RR_MANUAL)
        m_AppList.SetRefreshTime(RR_MANUAL);				
}

void CApplicationPg::OnUpdateRefreshManual(CCmdUI* pCmdUI) 
{
    if (m_AppList.GetRefreshTime() == RR_MANUAL)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);				
}


HANDLE  g_hDevice;
DWORD   g_dwProcessId;

static BOOL InitGetProcessTimes(HANDLE hDevice)
{
    DWORD dwBytesRet;

    g_dwProcessId = 0;

    if (DeviceIoControl(hDevice, 2, NULL, 0, &g_dwProcessId, 4,
        &dwBytesRet, NULL) == FALSE)
    {
        DWORD dwErr = GetLastError(); dwErr;
        return FALSE;
    }

    DWORD dwCurrentProcess = GetCurrentProcessId();
    g_dwProcessId ^= dwCurrentProcess;

    return TRUE;
}

static BOOL GetProcessTimes(void)
{
#define PT_MAX_ENTRIES   0x400

    PROCESS_TIME pt[PT_MAX_ENTRIES];

    DWORD dwBytesRet;

    if (DeviceIoControl(g_hDevice, 1, NULL, 0, pt, 
        PT_MAX_ENTRIES*sizeof(PROCESS_TIME), &dwBytesRet, NULL) == FALSE)
    {
        DWORD dwErr = GetLastError(); dwErr;
    }    	

    // richtige Ids formen
    int iCnt = dwBytesRet/sizeof(PROCESS_TIME);
    int i = 0;
    while (iCnt)
    {
        if ((pt[i].dwProcessId>>16))
        {
            // größer 0xffff, also Prozess
            pt[i].dwProcessId ^= g_dwProcessId;
            if ((pt[i].dwUnkown1 == 0) || (pt[i].byFlags[0] & 2))
            {
                // warum???
                // vielleicht diesen Prozess nicht weiter verarbeiten
                pt[i].dwProcessId = 0;
            }
            else
            {
            }
        }
        else
        {
            // kleiner, also Hwnd
            HWND hWnd;
            if ((hWnd = GetWindow((HWND)pt[i].dwProcessId, GW_CHILD)) == NULL)
            {
                hWnd = (HWND)pt[i].dwProcessId;
            }
            else
            {
            }
            GetWindowThreadProcessId(hWnd, &pt[i].dwProcessId);
        }
        iCnt--;
        i++;
    }
    return TRUE;
}

void CApplicationPg::OnOpenWintop() 
{
}

void CApplicationPg::OnUpdateOpenWintop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CApplicationPg::OnStartWintop() 
{
    InitGetProcessTimes(g_hDevice);
}

void CApplicationPg::OnUpdateStartWintop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CApplicationPg::OnMemoryWintop() 
{
#define PACK    0x1c
    BYTE byData[PACK];
    DWORD dwBytesRet;
    DWORD dwId = 0;

    if (DeviceIoControl(g_hDevice, 3, &dwId, 4, byData, PACK,
        &dwBytesRet, NULL) == FALSE)
    {
        DWORD dwErr = GetLastError(); dwErr;
    }    	
}

void CApplicationPg::OnUpdateMemoryWintop(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}

void CApplicationPg::OnTimesWintop() 
{
    GetProcessTimes();
}

void CApplicationPg::OnUpdateTimesWintop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CApplicationPg::OnDebug1() 
{
}

void CApplicationPg::OnUpdateDebug1(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(FALSE);
}

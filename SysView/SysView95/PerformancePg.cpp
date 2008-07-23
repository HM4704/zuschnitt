// PerformancePg.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "MainFrm.h"
#include "PerformancePg.h"
#include "ActiveXUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#undef _TEST

/////////////////////////////////////////////////////////////////////////////
// CPerformancePg property page

BYTE g_byField[10];
BYTE* g_pbyField;

IMPLEMENT_DYNCREATE(CPerformancePg, CSysBasePage)

CPerformancePg::CPerformancePg() : CSysBasePage(IDR_PERFORMANCE_PAGE,
                                      CPerformancePg::IDD)
{
	//{{AFX_DATA_INIT(CPerformancePg)
	m_dwCpuUsage = 0;
	m_dwMemUsage = 0;
	//}}AFX_DATA_INIT

    m_bTimerStarted = FALSE;
    m_bStop = FALSE;

    for (int i=0; i < 10; i++)
    {
        if (i<5)
            g_byField[i] = 1;
        else
            g_byField[i] = 100;
    }
    g_pbyField = &g_byField[0];

    BOOL bCreated;
    m_ridUpdateTime = UPDATE_HIGH;
    CSysView95App *pApp = (CSysView95App*) AfxGetApp(); 
    if (pApp)
    {
        CString strSubKey, strValue;
        strSubKey = _T("Performance");
        strValue = _T("REFRESH");
        if (m_ridUpdateTime.Connect(pApp->GetRegistryManager(),
            strSubKey, strValue, bCreated))
        {
            if (bCreated) m_ridUpdateTime = UPDATE_HIGH;
        }
        else
            m_ridUpdateTime = UPDATE_HIGH;
    }

    m_pStatusBar = NULL;
}

CPerformancePg::~CPerformancePg()
{
	m_StatData.StopStatistics();
    m_ridUpdateTime.Update();
}

void CPerformancePg::DoDataExchange(CDataExchange* pDX)
{
	CSysBasePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPerformancePg)
	DDX_Control(pDX, IDC_MEMUSAGE, m_ctrlMemUsage);
	DDX_Control(pDX, IDC_STATIC_MEMUSAGE, m_ctrlStMemUsage);
	DDX_Control(pDX, IDC_STATIC_CPUUSAGE, m_ctrlStCpuUsage);
	DDX_Control(pDX, IDC_MEMUSGAE_FRAME, m_ctrlFrMemUsage);
	DDX_Control(pDX, IDC_CPUUSGAE_FRAME, m_ctrlFrCpuUsage);
	DDX_Control(pDX, IDC_CPUUSAGE, m_ctrlCpuUsage);
	DDX_Text(pDX, IDC_CPUUSAGE, m_dwCpuUsage);
	DDX_Text(pDX, IDC_MEMUSAGE, m_dwMemUsage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPerformancePg, CSysBasePage)
	//{{AFX_MSG_MAP(CPerformancePg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_RR_HIGH, OnUpdateHigh)
	ON_UPDATE_COMMAND_UI(ID_RR_HIGH, OnUpdateUpdateHigh)
	ON_COMMAND(ID_RR_LOW, OnUpdateLow)
	ON_UPDATE_COMMAND_UI(ID_RR_LOW, OnUpdateUpdateLow)
	ON_COMMAND(ID_RR_MID, OnUpdateMid)
	ON_UPDATE_COMMAND_UI(ID_RR_MID, OnUpdateUpdateMid)
	ON_WM_SIZE()
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateRefresh)
    ON_MESSAGE(WM_PERF_NOTIFYICON, OnNotifyIcon)
	ON_COMMAND(ID_MANUAL, OnUpdateManual)
	ON_UPDATE_COMMAND_UI(ID_MANUAL, OnUpdateUpdateManual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPerformancePg message handlers

int CPerformancePg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

    if (m_ChartCpuUsage.Create(NULL, WS_VISIBLE, CRect(0,0,1,1),
        this, 0) == FALSE)
    {
        CActiveXUtils help;
        if (help.RegisterControl("chart.ocx") == FALSE)
            return -1;
        if (m_ChartCpuUsage.Create(NULL, WS_VISIBLE, CRect(0,0,1,1),
            this, 0) == FALSE)
            return -1;
    }

    if (m_ChartMemUsage.Create(NULL, WS_VISIBLE, CRect(0,0,1,1),
        this, 0) == FALSE)
    {
        return -1;
    }

    return 0;
}


BOOL CPerformancePg::PlaceItems(CRect &rect)
{
    static bFirst = TRUE;
    if (CSysBasePage::PlaceItems(rect) == FALSE)
        return FALSE;

    CRect rcStatic;
    rcStatic.top = rect.top+10;
    rcStatic.bottom = rect.Height()/2 - 5;
    rcStatic.left = rect.left + 10;
    rcStatic.right = rect.right - 10;
    m_ctrlStCpuUsage.MoveWindow(&rcStatic);

    CRect rc;
    m_ctrlCpuUsage.GetWindowRect(&rc);
    ScreenToClient(&rc);
    int iHeight = rc.Height(); int iWidth = rc.Width();
    rc.left = rcStatic.left + 20;
    rc.top = rcStatic.top+rcStatic.Height()/2 - iHeight/2;
    rc.right = rc.left + iWidth;
    rc.bottom = rc.top + iHeight;
    m_ctrlCpuUsage.MoveWindow(&rc);

    rc.left = rc.right + 20;
    rc.top = rcStatic.top + 20;
    rc.right = rcStatic.right - 10;
    rc.bottom = rcStatic.bottom - 10;
    m_ctrlFrCpuUsage.MoveWindow(&rc);

    PlaceChartCtrl(m_ChartCpuUsage, IDC_CPUUSGAE_FRAME);

    rcStatic.top = rcStatic.bottom + 5;
    rcStatic.bottom = rect.bottom - 10;
    rcStatic.left = rect.left + 10;
    rcStatic.right = rect.right - 10;
    m_ctrlStMemUsage.MoveWindow(&rcStatic);

    m_ctrlMemUsage.GetWindowRect(&rc);
    ScreenToClient(&rc);
    iHeight = rc.Height(); iWidth = rc.Width();
    rc.left = rcStatic.left + 20;
    rc.top = rcStatic.top+rcStatic.Height()/2 - iHeight/2;
    rc.right = rc.left + iWidth;
    rc.bottom = rc.top + iHeight;
    m_ctrlMemUsage.MoveWindow(&rc);

    rc.left = rc.right + 20;
    rc.top = rcStatic.top + 20;
    rc.right = rcStatic.right - 10;
    rc.bottom = rcStatic.bottom - 10;
    m_ctrlFrMemUsage.MoveWindow(&rc);

    PlaceChartCtrl(m_ChartMemUsage, IDC_MEMUSGAE_FRAME);

    return TRUE;
}

BOOL CPerformancePg::OnInitDialog() 
{
	CSysBasePage::OnInitDialog();

    m_hInstance = AfxGetInstanceHandle();

    PlaceChartCtrl(m_ChartCpuUsage, IDC_CPUUSGAE_FRAME);

    PlaceChartCtrl(m_ChartMemUsage, IDC_MEMUSGAE_FRAME);

    // set registry manager
    CSysView95App *pApp = (CSysView95App*) AfxGetApp();
    if (pApp)
    {
        m_ChartCpuUsage.SetControlInfo("Performance\\CpuUsage", (IUnknown*)pApp->GetRegistryManager());
        m_ChartMemUsage.SetControlInfo("Performance\\MemUsage", (IUnknown*)pApp->GetRegistryManager());
    }

    GetWindowRect(&m_rectClient);
    ScreenToClient(&m_rectClient);
    PlaceItems(m_rectClient);
    // Statistik in Registry starten
    m_StatData.StartStatistics();

    SetUpdateRate(m_ridUpdateTime.GetVal());

    LoadIcons();

    m_trayCpuUsage.Create(this, WM_PERF_NOTIFYICON, 1, 
        m_hArTrayIcons[0], "0 %");

    RegisterStatusBarInfo(ID_CPUUSAGE, ID_MEMUSAGE);

    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPerformancePg::DestroyWindow() 
{
	KillTimer(m_dwTimerCPUUsage);
	return CSysBasePage::DestroyWindow();
}

void CPerformancePg::OnTimer(UINT nIDEvent) 
{
    DWORD  dwOldValue = m_dwCpuUsage;
	if (nIDEvent == m_dwTimerCPUUsage)
    {
        DWORD dwValue;

        if (m_StatData.GetCPUUsage(&dwValue) == FALSE)
        {
            return;
        }

        m_dwCpuUsage = dwValue;

        if (m_StatData.GetMemoryUsage(&dwValue) == FALSE)
        {
            return;
        }

        m_dwMemUsage = dwValue;

        UpdateData(FALSE);

        // update the chart
        if (m_bStop == FALSE)
        {
#ifndef _TEST
            m_ChartCpuUsage.Update(m_dwCpuUsage);
            m_ChartMemUsage.Update(m_dwMemUsage);

            if ((dwOldValue/10) != (m_dwCpuUsage/10))
            {
                char cBuf[100];
                sprintf(cBuf, "%d %%", m_dwCpuUsage);
                m_trayCpuUsage.Modify(GetPerformanceIcon(m_dwCpuUsage), cBuf);
            }

            if (m_pStatusBar)
                UpdateStatusBar();
#else
            m_ChartCpuUsage.Update((DWORD)((char)*g_pbyField));
            if (g_pbyField == &g_byField[9])
                g_pbyField = &g_byField[0];
            else
                g_pbyField++;
#endif
        }
    }

	CSysBasePage::OnTimer(nIDEvent);
}

BOOL CPerformancePg::PlaceChartCtrl(CChart &chart, UINT IdFrame)
{
    CRect rectFrame;
    GetDlgItem(IdFrame)->GetWindowRect(&rectFrame);
    ScreenToClient(&rectFrame);
    rectFrame.left += 3;
    rectFrame.top += 3;
    rectFrame.right -= 3;
    rectFrame.bottom -= 3;
    chart.MoveWindow(&rectFrame, TRUE);

    return TRUE;
}


void CPerformancePg::OnUpdateHigh() 
{
    if (m_ridUpdateTime.GetVal() != UPDATE_HIGH)
        SetUpdateRate(UPDATE_HIGH);        
}

void CPerformancePg::OnUpdateUpdateHigh(CCmdUI* pCmdUI) 
{
    if (m_ridUpdateTime.GetVal() == UPDATE_HIGH)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);			
}

void CPerformancePg::OnUpdateLow() 
{
    if (m_ridUpdateTime.GetVal() != UPDATE_LOW)
        SetUpdateRate(UPDATE_LOW);	
}

void CPerformancePg::OnUpdateUpdateLow(CCmdUI* pCmdUI) 
{
    if (m_ridUpdateTime.GetVal() == UPDATE_LOW)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);			
}

void CPerformancePg::OnUpdateMid() 
{
    if (m_ridUpdateTime.GetVal() != UPDATE_MID)
        SetUpdateRate(UPDATE_MID);
}

void CPerformancePg::OnUpdateUpdateMid(CCmdUI* pCmdUI) 
{
    if (m_ridUpdateTime.GetVal() == UPDATE_MID)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);			
}

void CPerformancePg::OnUpdateManual() 
{
    if (m_ridUpdateTime.GetVal() != UPDATE_MANUAL)
        SetUpdateRate(UPDATE_MANUAL);	
}

void CPerformancePg::OnUpdateUpdateManual(CCmdUI* pCmdUI) 
{
    if (m_ridUpdateTime.GetVal() == UPDATE_MANUAL)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);				
}

BOOL CPerformancePg::SetUpdateRate(DWORD dwUpdateTime)
{
    if (m_bTimerStarted == TRUE)
        KillTimer(m_dwTimerCPUUsage);

    m_ridUpdateTime = dwUpdateTime;
    if (m_ridUpdateTime.GetVal() != UPDATE_MANUAL)
    {
        if ((m_dwTimerCPUUsage = SetTimer(TIMERCPUUSAGE, m_ridUpdateTime.GetVal(), NULL)) != 0)
            m_bTimerStarted = TRUE;
        else
            m_bTimerStarted = FALSE;
    }
    else
    {
        m_bTimerStarted = FALSE;
        return TRUE;
    }

    return m_bTimerStarted;
}

void CPerformancePg::OnSize(UINT nType, int cx, int cy) 
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

void CPerformancePg::LoadIcons()
{
    for (int i = IDI_PART_0; i <= IDI_PART_9; i++)
    {
        m_hArTrayIcons[i-IDI_PART_0] = 
          (HICON) LoadImage(m_hInstance, MAKEINTRESOURCE(i), IMAGE_ICON, 
          16, 16, 0);
    }
}

void CPerformancePg::UnloadIcons()
{
    for (int i = 0; i <= 9; i++)
    {
        DestroyIcon(m_hArTrayIcons[i]);
    }
}

inline HICON CPerformancePg::GetPerformanceIcon(DWORD dwValue)
{
    return m_hArTrayIcons[__min((dwValue/10)+1, 9)];
}

void CPerformancePg::OnRefresh() 
{
	OnTimer(m_dwTimerCPUUsage);
}

void CPerformancePg::OnUpdateRefresh(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();    	
}

BOOL CPerformancePg::RegisterStatusBarInfo(int iStrIdCpuUsage, int iStrIdMemUsage)
{
    BOOL bRet;

    CMainFrame* pFrm = (CMainFrame*) AfxGetMainWnd();
    if (!pFrm)
        return FALSE;

    m_pStatusBar= &pFrm->GetStatusBar();

    m_iIndexCpuUsage = m_pStatusBar->CommandToIndex(iStrIdCpuUsage);
    m_iIndexMemUsage = m_pStatusBar->CommandToIndex(iStrIdMemUsage);

    // get string from resource
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(pFrm->GetSafeHwnd(), GWL_HINSTANCE);
    if (hInstance)
    {
       m_iStrLenCpuUsage = LoadString(hInstance, iStrIdCpuUsage, m_strTextCpuUsage, MAX_PATH);
       m_iStrLenMemUsage = LoadString(hInstance, iStrIdMemUsage, m_strTextMemUsage, MAX_PATH);
       bRet = (BOOL)(m_iStrLenCpuUsage & m_iStrLenMemUsage);
    }
    else
    {
       strcpy(m_strTextCpuUsage, _T("???"));
       m_iStrLenCpuUsage = strlen(m_strTextCpuUsage);
       strcpy(m_strTextMemUsage, _T("???"));
       m_iStrLenMemUsage = strlen(m_strTextMemUsage);
       bRet = FALSE;
    }

    UINT nID, nStyle; int cxWidth ;
    m_pStatusBar->GetPaneInfo(m_iIndexCpuUsage, nID, nStyle, cxWidth); 
    cxWidth += 30;
    m_pStatusBar->SetPaneInfo(m_iIndexCpuUsage, nID, nStyle, cxWidth); 
    m_pStatusBar->GetPaneInfo(m_iIndexMemUsage, nID, nStyle, cxWidth); 
    cxWidth += 30;
    m_pStatusBar->SetPaneInfo(m_iIndexMemUsage, nID, nStyle, cxWidth); 
    return bRet;    
}

void CPerformancePg::UpdateStatusBar()
{
    itoa(m_dwCpuUsage, &m_strTextCpuUsage[m_iStrLenCpuUsage], 10);
    strcat(&m_strTextCpuUsage[m_iStrLenCpuUsage], " %");
    m_pStatusBar->SetPaneText(m_iIndexCpuUsage, m_strTextCpuUsage, TRUE);
    itoa(m_dwMemUsage, &m_strTextMemUsage[m_iStrLenMemUsage], 10);
    strcat(&m_strTextMemUsage[m_iStrLenMemUsage], " %");
    m_pStatusBar->SetPaneText(m_iIndexMemUsage, m_strTextMemUsage, TRUE);
}

LRESULT CPerformancePg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
    switch(lParam)
    {
    case WM_RBUTTONDOWN:
        {
            wParam; // to avoid warning

            CRect rect;
            if (SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0))
            {
                CMenu menu;
                POINT pt;

                GetCursorPos(&pt);

                CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

                pFrm->SetForegroundWindow();

                if (menu.LoadMenu(IDR_TRAYMENU) && pFrm)
                {
                    CMenu *pMenu = menu.GetSubMenu(0);
                    if (::TrackPopupMenuEx(pMenu->GetSafeHmenu(), TPM_RIGHTALIGN|TPM_BOTTOMALIGN, 
                        pt.x, pt.y, pFrm->GetSafeHwnd(), NULL) == FALSE)
                    {
//                        DWORD dwErr = GetLastError();
                    }
                    return 0L;
                }
            }
            break;
        }
    case WM_LBUTTONDBLCLK:
        {
            CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
            if (pFrm)
            {
                CRect rect;
                pFrm->GetWindowRect(&rect);
                AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_RESTORE, 
                    MAKELPARAM(rect.Width(), rect.Height()));
            }
            return 0L;
            break;
        }
    default:
        break;
    }
    return 1L;
}

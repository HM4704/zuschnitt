// ProcessPg.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "MainFrm.h"
#include "ProcessPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessPg property page

IMPLEMENT_DYNCREATE(CProcessPg, CSysBasePage)

CProcessPg::CProcessPg() : CSysBasePage(IDR_PROCESSLIST_PAGE,
                                  CProcessPg::IDD)
{
	//{{AFX_DATA_INIT(CProcessPg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_bDlgInitDone = FALSE;
}

CProcessPg::~CProcessPg()
{
}

void CProcessPg::DoDataExchange(CDataExchange* pDX)
{
	CSysBasePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessPg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessPg, CSysBasePage)
	//{{AFX_MSG_MAP(CProcessPg)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateRefresh)
	ON_COMMAND(ID_RR_HIGH, OnRefreshHigh)
	ON_UPDATE_COMMAND_UI(ID_RR_HIGH, OnUpdateRefreshHigh)
	ON_COMMAND(ID_RR_LOW, OnRefreshLow)
	ON_UPDATE_COMMAND_UI(ID_RR_LOW, OnUpdateRefreshLow)
	ON_COMMAND(ID_RR_MID, OnRefreshMid)
	ON_UPDATE_COMMAND_UI(ID_RR_MID, OnUpdateRefreshMid)
	ON_COMMAND(ID_PID, OnPid)
	ON_UPDATE_COMMAND_UI(ID_PID, OnUpdatePid)
	ON_COMMAND(ID_CNTUSAGE, OnCntusage)
	ON_UPDATE_COMMAND_UI(ID_CNTUSAGE, OnUpdateCntusage)
	ON_COMMAND(ID_MODULEID, OnModuleid)
	ON_UPDATE_COMMAND_UI(ID_MODULEID, OnUpdateModuleid)
	ON_COMMAND(ID_HEAPID, OnHeapid)
	ON_UPDATE_COMMAND_UI(ID_HEAPID, OnUpdateHeapid)
	ON_COMMAND(ID_CNTTHREADS, OnCntthreads)
	ON_UPDATE_COMMAND_UI(ID_CNTTHREADS, OnUpdateCntthreads)
	ON_COMMAND(ID_PARENT_PID, OnParentPid)
	ON_UPDATE_COMMAND_UI(ID_PARENT_PID, OnUpdateParentPid)
	ON_COMMAND(ID_PRIORITY, OnPriority)
	ON_UPDATE_COMMAND_UI(ID_PRIORITY, OnUpdatePriority)
	ON_COMMAND(ID_FLAGS, OnFlags)
	ON_UPDATE_COMMAND_UI(ID_FLAGS, OnUpdateFlags)
	ON_WM_SIZE()
	ON_MESSAGE(MSG_SWITCH_TO_PROCESS, OnSwitchToProcess)
	ON_COMMAND(ID_PT_DLG, OnPtDlg)
	ON_UPDATE_COMMAND_UI(ID_PT_DLG, OnUpdatePtDlg)
	ON_COMMAND(ID_CPU_TIME, OnCpuTime)
	ON_UPDATE_COMMAND_UI(ID_CPU_TIME, OnUpdateCpuTime)
	ON_COMMAND(ID_CPU_PERCENT, OnCpuPercent)
	ON_UPDATE_COMMAND_UI(ID_CPU_PERCENT, OnUpdateCpuPercent)
	ON_COMMAND(ID_MANUAL, OnRefreshManual)
	ON_UPDATE_COMMAND_UI(ID_MANUAL, OnUpdateRefreshManual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessPg message handlers

BOOL CProcessPg::InitializeList(UINT dwIDC_Frame)
{
    CRect rect;

    GetDlgItem(dwIDC_Frame)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    rect.left += 3;
    rect.top += 3;
    rect.right -= 3;
    rect.bottom -= 3;
    return m_ProcList.Create(rect, this, 1022);
}

BOOL CProcessPg::PlaceItems(CRect &rect)
{
    if (CSysBasePage::PlaceItems(rect) == FALSE)
        return FALSE;

    CWnd* pWndFrame = GetDlgItem(IDC_PROCESSLISTFRAME);
    
    rect.left += 20;
    rect.top += 20;
    rect.right -= 20;
    rect.bottom -= 20;
    pWndFrame->MoveWindow(&rect);

    rect.left += 3;
    rect.top += 3;
    rect.right -= 3;
    rect.bottom -= 3;
    m_ProcList.MoveWindow(&rect);

    return TRUE;
}

BOOL CProcessPg::OnInitDialog() 
{
	CSysBasePage::OnInitDialog();
	
	InitializeList(IDC_PROCESSLISTFRAME);
    
    m_bDlgInitDone = TRUE;

    CRect rectClient;
    GetWindowRect(&rectClient);
    ScreenToClient(&rectClient);
    PlaceItems(rectClient);

    CMainFrame* pFrm = (CMainFrame*) AfxGetMainWnd();
    m_ProcList.RegisterStatusBarInfo(&pFrm->GetStatusBar(), ID_PROCESSES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessPg::OnRefresh() 
{
	m_ProcList.UpdateList();
}

void CProcessPg::OnUpdateRefresh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CProcessPg::OnRefreshHigh() 
{
    if (m_ProcList.GetRefreshTime() != RR_HIGH)
        m_ProcList.SetRefreshTime(RR_HIGH);
}

void CProcessPg::OnUpdateRefreshHigh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
    if (m_ProcList.GetRefreshTime() == RR_HIGH)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);	
}

void CProcessPg::OnRefreshLow() 
{
    if (m_ProcList.GetRefreshTime() != RR_LOW)
        m_ProcList.SetRefreshTime(RR_LOW);	
}

void CProcessPg::OnUpdateRefreshLow(CCmdUI* pCmdUI) 
{
    if (m_ProcList.GetRefreshTime() == RR_LOW)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);	
}

void CProcessPg::OnRefreshMid() 
{
    if (m_ProcList.GetRefreshTime() != RR_MID)
        m_ProcList.SetRefreshTime(RR_MID);	
}

void CProcessPg::OnUpdateRefreshMid(CCmdUI* pCmdUI) 
{
    if (m_ProcList.GetRefreshTime() == RR_MID)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);	
}

void CProcessPg::OnRefreshManual() 
{
    if (m_ProcList.GetRefreshTime() != RR_MANUAL)
        m_ProcList.SetRefreshTime(RR_MANUAL);		
}

void CProcessPg::OnUpdateRefreshManual(CCmdUI* pCmdUI) 
{
    if (m_ProcList.GetRefreshTime() == RR_MANUAL)
        pCmdUI->SetCheck(1);	
    else
        pCmdUI->SetCheck(0);		
}

void CProcessPg::OnPid() 
{
	if (m_ProcList.GetColumnField() & pid)
        m_ProcList.RemoveColumn(pid);
    else
        m_ProcList.AddColumn(pid);
}

void CProcessPg::OnUpdatePid(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & pid)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);
}

void CProcessPg::OnCntusage() 
{
	if (m_ProcList.GetColumnField() & cntUsage)
        m_ProcList.RemoveColumn(cntUsage);	
    else
        m_ProcList.AddColumn(cntUsage);
}

void CProcessPg::OnUpdateCntusage(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & cntUsage)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);	
}

void CProcessPg::OnModuleid() 
{
	if (m_ProcList.GetColumnField() & moduleId)
        m_ProcList.RemoveColumn(moduleId);	
    else
        m_ProcList.AddColumn(moduleId);
}

void CProcessPg::OnUpdateModuleid(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & moduleId)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);		
}

void CProcessPg::OnHeapid() 
{
	if (m_ProcList.GetColumnField() & defaultHeapId)
        m_ProcList.RemoveColumn(defaultHeapId);		
    else
        m_ProcList.AddColumn(defaultHeapId);
}

void CProcessPg::OnUpdateHeapid(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & defaultHeapId)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);			
}

void CProcessPg::OnCntthreads() 
{
	if (m_ProcList.GetColumnField() & cntThreads)
        m_ProcList.RemoveColumn(cntThreads);
    else
        m_ProcList.AddColumn(cntThreads);
}

void CProcessPg::OnUpdateCntthreads(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & cntThreads)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);			
}

void CProcessPg::OnParentPid() 
{
	if (m_ProcList.GetColumnField() & parentPid)
        m_ProcList.RemoveColumn(parentPid);				
    else
        m_ProcList.AddColumn(parentPid);
}

void CProcessPg::OnUpdateParentPid(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & parentPid)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);			
}

void CProcessPg::OnPriority() 
{
	if (m_ProcList.GetColumnField() & prioClassBase)
        m_ProcList.RemoveColumn(prioClassBase);				
    else
        m_ProcList.AddColumn(prioClassBase);
}

void CProcessPg::OnUpdatePriority(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & prioClassBase)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);				
}

void CProcessPg::OnCpuTime() 
{
	if (m_ProcList.GetColumnField() & cpuTime)
        m_ProcList.RemoveColumn(cpuTime);					
    else
        m_ProcList.AddColumn(cpuTime);    	
}

void CProcessPg::OnUpdateCpuTime(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & cpuTime)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);						
}

void CProcessPg::OnCpuPercent() 
{
	if (m_ProcList.GetColumnField() & cpuPercent)
        m_ProcList.RemoveColumn(cpuPercent);					
    else
        m_ProcList.AddColumn(cpuPercent);    		
}

void CProcessPg::OnUpdateCpuPercent(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & cpuPercent)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);							
}

void CProcessPg::OnFlags() 
{
	if (m_ProcList.GetColumnField() & flags)
        m_ProcList.RemoveColumn(flags);					
    else
        m_ProcList.AddColumn(flags);
}

void CProcessPg::OnUpdateFlags(CCmdUI* pCmdUI) 
{
	if (m_ProcList.GetColumnField() & flags)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);					
}

void CProcessPg::OnSize(UINT nType, int cx, int cy) 
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

LRESULT  CProcessPg::OnSwitchToProcess(WPARAM wParam, LPARAM lParam)
{
    if (m_ProcList)
    {
        return m_ProcList.PostMessage(MSG_SWITCH_TO_PROCESS,
            wParam, lParam);
    }
    
    return 0L;
}


BOOL CProcessPg::OnSetActive() 
{
    /* Force select */
    m_ProcList.PostMessage(WM_MBUTTONDOWN , MK_MBUTTON, 0);
    m_ProcList.PostMessage(WM_MBUTTONUP, 0, 0);
	return CSysBasePage::OnSetActive();
}

void CProcessPg::OnPtDlg() 
{
}

void CProcessPg::OnUpdatePtDlg(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(FALSE);
}

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SysView95.h"

#include "SpawnProcessDlg.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_SPAWN_PROCESS, OnSpawnProcess)
	ON_UPDATE_COMMAND_UI(ID_SPAWN_PROCESS, OnUpdateSpawnProcess)
	ON_WM_MOVE()
	ON_COMMAND(ID_FOREGROUND, OnForeground)
	ON_UPDATE_COMMAND_UI(ID_FOREGROUND, OnUpdateForeground)
	ON_COMMAND(ID_MINIMIZE_AT_START, OnMinimizeAtStart)
	ON_UPDATE_COMMAND_UI(ID_MINIMIZE_AT_START, OnUpdateMinimizeAtStart)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_PROCESSES,
	ID_CPUUSAGE,
	ID_MEMUSAGE,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pSheet = NULL;
    SetupRegistryValues();
}

CMainFrame::~CMainFrame()
{
    if (m_dwMinimized.GetVal() == FALSE)
    {
        m_riPosX.Update();
        m_riPosY.Update();
    }
    m_riSizeX.Update();
    m_riSizeY.Update();
    m_ridWindowStyles.Update();

    if (m_pSheet != NULL)
    {
        delete m_pSheet;
        m_pSheet = NULL;
    }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*
	if (!m_wndToolBar.CreateEx(this, m_wndToolBar.GetBarStyle(), WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

    if (!(m_ridWindowStyles.GetVal() & WS_MY_TOOLBAR_VISIBLE))
        m_wndToolBar.ShowWindow(SW_HIDE);
    if (!(m_ridWindowStyles.GetVal() & WS_MY_STATUSBAR_VISIBLE))
        m_wndStatusBar.ShowWindow(SW_HIDE);



    // create Property-Sheet
    m_pSheet = new CSysSheet(_T(""), this);

	// create a modeless property page
	if (!m_pSheet->Create(this,
			DS_CONTEXTHELP | DS_SETFONT | WS_CHILD | WS_VISIBLE))
	{
		DestroyWindow();
		return -1;
	}
    

	m_pSheet->SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

	// we use the style from the template - but make sure that
	// the WS_BORDER bit is correct.
	// the WS_BORDER bit will be whatever is in dwRequestedStyle

	m_pSheet->ModifyStyle(WS_BORDER,
        WS_BORDER);
  
    if (m_ridWindowStyles.GetVal() & WS_MINIMIZE)
        ShowWindow(SW_SHOWMINIMIZED);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Use the specific class name we established earlier
    cs.lpszClass = SYSVIEW95_CLASS;
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    CRect rect;
    rect.top = m_riPosY.GetVal();
    rect.left = m_riPosX.GetVal();
    rect.bottom = rect.top+m_riSizeY.GetVal();
    rect.right = rect.left+m_riSizeX.GetVal(); 
    cs.x = rect.left;
    cs.y = rect.top;
    cs.cx = rect.Width();
    cs.cy = rect.Height();
    cs.style &= ~(/*WS_THICKFRAME|*/WS_MAXIMIZEBOX);
    if (m_ridWindowStyles.GetVal() & WS_MINIMIZE)
        cs.style |= WS_MINIMIZE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    if (m_ridWindowStyles.GetVal() & WS_EX_TOPMOST)
        cs.dwExStyle |= WS_EX_TOPMOST;

//    cs.lpszClass = AfxRegisterWndClass(0);
    m_lWndStyles = cs.style;
    m_dwWndExStyles = cs.dwExStyle;

	return TRUE;
}

BOOL CMainFrame::SetupRegistryValues()
{
    BOOL bCreated;
    CSysView95App *pApp = (CSysView95App*) AfxGetApp();
    if (pApp)
    {
        m_riPosX.Connect(pApp->GetRegistryManager(), "",
            "POS_X", bCreated);
        if (bCreated == TRUE) m_riPosX = 10;

        m_riPosY.Connect(pApp->GetRegistryManager(), "",
            "POS_Y", bCreated);
        if (bCreated == TRUE) m_riPosY = 10;

        m_riSizeX.Connect(pApp->GetRegistryManager(), "",
            "SIZE_X", bCreated);
        if (bCreated == TRUE) m_riSizeX = 480;

        m_riSizeY.Connect(pApp->GetRegistryManager(), "",
            "SIZE_Y", bCreated);
        if (bCreated == TRUE) m_riSizeY = 550;

        m_ridWindowStyles.Connect(pApp->GetRegistryManager(), "",
            "WINDOW_STYLES", bCreated);
        if (bCreated == TRUE) m_ridWindowStyles = 0;
        m_dwMinimized = FALSE;
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
    pOldWnd;  // to avoid warning

	// forward focus to the view window
	m_pSheet->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command

    if (m_pSheet != NULL)
    {
        if (m_pSheet->GetSafeHwnd() != NULL)
        {
	        if (m_pSheet->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		        return TRUE;
        }
    }

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
    
    if (nType == SIZE_RESTORED)
    {
#if 0
        m_dwMinimized = FALSE;
        m_riSizeX = cx+8;   //->Frame links+rechts
        m_riSizeY = cy+56;  // Frame unten + System-Leiste oben

	    CRect rectToolBar, rectStatusBar, rectSize;

        if (m_wndToolBar.IsWindowVisible())
            m_wndToolBar.GetWindowRect(&rectToolBar);
        else
            rectToolBar.SetRectEmpty();

        // gehört vielleicht nicht zum Client-Bereich???
        m_wndStatusBar.GetWindowRect(&rectStatusBar);

        GetClientRect(&rectSize);

        rectSize.top += rectToolBar.Height();

        rectSize.bottom -= rectStatusBar.Height();

        m_pSheet->MoveWindow(&rectSize);
#else
        AdjustSize(cx, cy);
#endif
    }	
    else
        if (nType == SIZE_MINIMIZED)
            m_dwMinimized = TRUE;
}


void CMainFrame::OnSpawnProcess() 
{
    CSpawnProcessDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        DWORD dwFlags = 0;//CREATE_DEFAULT_ERROR_MODE|NORMAL_PRIORITY_CLASS;
        STARTUPINFO startUpInfo;
        PROCESS_INFORMATION processInfo;
        char cBuf[MAX_PATH];

        strcpy(cBuf, dlg.m_strProcessName);
        memset(&startUpInfo, 0, sizeof(startUpInfo));
        startUpInfo.cb = sizeof(startUpInfo);
        startUpInfo.dwFlags = 0;

        if ( CreateProcess(NULL, cBuf, NULL, NULL,
            FALSE, dwFlags, NULL, NULL, &startUpInfo, 
            &processInfo) == FALSE )
        {
            // Error
        }
    }
}

void CMainFrame::OnUpdateSpawnProcess(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
    m_riPosX = x-4;
    m_riPosY = y-52;
}

void CMainFrame::OnForeground() 
{
    if (m_ridWindowStyles.GetVal() & WS_EX_TOPMOST)
    {
        // is topmost
        m_ridWindowStyles = m_ridWindowStyles.GetVal() & ~WS_EX_TOPMOST;
        ::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, 
            SWP_NOSIZE|SWP_NOMOVE);        
    }
    else
    {
        // no, set topmost
        m_ridWindowStyles = m_ridWindowStyles.GetVal() | WS_EX_TOPMOST;
        ::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, 
            SWP_NOSIZE|SWP_NOMOVE);
    }
}

void CMainFrame::OnUpdateForeground(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_ridWindowStyles.GetVal() & WS_EX_TOPMOST);	
}

void CMainFrame::OnMinimizeAtStart() 
{
    m_ridWindowStyles =  m_ridWindowStyles.GetVal() ^ WS_MINIMIZE;
}

void CMainFrame::OnUpdateMinimizeAtStart(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_ridWindowStyles.GetVal() & WS_MINIMIZE);	    	
}

void CMainFrame::OnViewToolbar() 
{
    if (m_wndToolBar.IsWindowVisible())
    {
        m_ridWindowStyles = m_ridWindowStyles.GetVal() & (~WS_MY_TOOLBAR_VISIBLE);
        m_wndToolBar.ShowWindow(SW_HIDE); 
    }
    else
    {
        m_ridWindowStyles = m_ridWindowStyles.GetVal() | WS_MY_TOOLBAR_VISIBLE;
        m_wndToolBar.ShowWindow(SW_SHOW);  
    }
    OnSize(SIZE_RESTORED, m_riSizeX.GetVal()-8, m_riSizeY.GetVal()-56);
}

void CMainFrame::OnViewStatusBar() 
{
    if (m_wndStatusBar.IsWindowVisible())
    {
        m_ridWindowStyles = m_ridWindowStyles.GetVal() & (~WS_MY_STATUSBAR_VISIBLE);
        m_wndStatusBar.ShowWindow(SW_HIDE); 
    }
    else
    {
        m_ridWindowStyles = m_ridWindowStyles.GetVal() | WS_MY_STATUSBAR_VISIBLE;
        m_wndStatusBar.ShowWindow(SW_SHOW);  
    }
    OnSize(SIZE_RESTORED, m_riSizeX.GetVal()-8, m_riSizeY.GetVal()-56);	
}

BOOL CMainFrame::AdjustSize(int cx, int cy)
{
    m_dwMinimized = FALSE;
    m_riSizeX = cx+8;   //->Frame links+rechts
    m_riSizeY = cy+56;  // Frame unten + System-Leiste oben

    CRect rectToolBar, rectStatusBar, rectSize;

    if (m_wndToolBar.IsWindowVisible())
        m_wndToolBar.GetWindowRect(&rectToolBar);
    else
        rectToolBar.SetRectEmpty();

    if (m_wndStatusBar.IsWindowVisible())
        m_wndStatusBar.GetWindowRect(&rectStatusBar);
    else
        rectStatusBar.SetRectEmpty();

    GetClientRect(&rectSize);

    rectSize.top += rectToolBar.Height();

    rectSize.bottom -= rectStatusBar.Height();

    m_pSheet->MoveWindow(&rectSize);

    return TRUE;
}

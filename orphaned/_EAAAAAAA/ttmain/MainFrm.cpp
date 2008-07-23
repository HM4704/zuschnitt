// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#include "TTData.h"
#include "Tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "MainFrm.h"
#include "AuswDlg.h"
#include "TorwahlDlg.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "KreisBerechnungDlg.h"
#include "ZRTreeCtrl.h"
#include "ZRSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOR_HINZU, OnTorHinzu)
	ON_UPDATE_COMMAND_UI(ID_TOR_HINZU, OnUpdateTorHinzu)
	ON_WM_PAINT()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_KREISBERECHNUNG, OnKreisberechnung)
	ON_UPDATE_COMMAND_UI(ID_KREISBERECHNUNG, OnUpdateKreisberechnung)
	ON_COMMAND(ID_INHALT, OnHilfeInhalt)
	ON_UPDATE_COMMAND_UI(ID_INHALT, OnUpdateHilfeInhalt)
	ON_COMMAND(ID_TOR_AENDERN, OnTorAendern)
	ON_UPDATE_COMMAND_UI(ID_TOR_AENDERN, OnUpdateTorAendern)
	ON_COMMAND(ID_ZUSCHNITTREGELN_ANZEIGEN, OnZuschnittregelnAnzeigen)
	ON_UPDATE_COMMAND_UI(ID_ZUSCHNITTREGELN_ANZEIGEN, OnUpdateZuschnittregelnAnzeigen)
    ON_MESSAGE(WM_BERECHNE_ZUSCNITTE, OnZuschnitteBerechnen)
    ON_MESSAGE(WM_ZR_SHEET_CLOSED, OnZRSheetClosed)
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
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
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
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

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

    if (m_ridWindowStyles.GetVal() & WS_MINIMIZE)
        ShowWindow(SW_SHOWMINIMIZED);

    if (m_ridWindowStyles.GetVal() & WS_MAXIMIZE)
    {
        AfxGetApp()->m_nCmdShow = SW_SHOWMAXIMIZED;
    }
//        ShowWindow(SW_SHOWMAXIMIZED);    

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

    CRect rect;
    rect.top = m_riPosY.GetVal();
    rect.left = m_riPosX.GetVal();
    rect.bottom = rect.top+m_riSizeY.GetVal();
    rect.right = rect.left+m_riSizeX.GetVal(); 
    cs.x = rect.left;
    cs.y = rect.top;
    cs.cx = rect.Width();
    cs.cy = rect.Height();
    if (m_ridWindowStyles.GetVal() & WS_MINIMIZE)
        cs.style |= WS_MINIMIZE;


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnTorHinzu() 
{
	CMDIChildWnd* pChild =  MDIGetActive();
	CTtmainView* pView = (CTtmainView*)pChild->GetActiveView();
	pView->addTorDoor();
	pView->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateTorHinzu(CCmdUI* pCmdUI) 
{
	CMDIChildWnd* pChild =  MDIGetActive();
	if (pChild != NULL)
	{
		CTtmainView* pView = (CTtmainView*)pChild->GetActiveView();		
		if (pView != NULL)
		{
			pCmdUI->Enable((pView->getTorCount() < 2));
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}


void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CMDIFrameWnd::OnPaint() for painting messages
}

void CMainFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnKreisberechnung() 
{
    CKreisBerechnungDlg* pDlg = new CKreisBerechnungDlg(this);
	pDlg->Create();
	pDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateKreisberechnung(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}


void CMainFrame::OnHilfeInhalt() 
{
    CTtmainApp* pApp = (CTtmainApp*) AfxGetApp();
    if (pApp)
    {
        CRect rect;
        int width = ::GetSystemMetrics(SM_CXSCREEN); 
        int height = ::GetSystemMetrics(SM_CYSCREEN);
        rect.left = width/2;
        rect.right = width-10;
        rect.top = 10;
        rect.bottom = (int)((float)height*8/9);

        pApp->ShowHelp(szZuschnittHilfe, PLACE_POS, &rect);
    }
}

void CMainFrame::OnUpdateHilfeInhalt(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}

void CMainFrame::OnTorAendern() 
{
	CMDIChildWnd* pChild =  MDIGetActive();
	if (pChild != NULL)
	{
		CTtmainView* pView = (CTtmainView*)pChild->GetActiveView();		
		if (pView != NULL)
		{
            int iTorZahl = pView->getTorCount();
            if (iTorZahl > 1)
            {
                CTorwahlDlg dlg(this, iTorZahl);
                if (dlg.DoModal() == IDOK)
                {
                    int iTor = dlg.GewaehltesTor();
                    pView->editTorDoor(iTor);
                }
            }
            else
			    pView->editTorDoor(0);
		}
	}	
}

void CMainFrame::OnUpdateTorAendern(CCmdUI* pCmdUI) 
{
	CMDIChildWnd* pChild =  MDIGetActive();
	if (pChild != NULL)
	{
		CTtmainView* pView = (CTtmainView*)pChild->GetActiveView();		
		if (pView != NULL)
		{
			pCmdUI->Enable((pView->getTorCount() > 0));
			return;
		}
	}
	pCmdUI->Enable(FALSE);			
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnZuschnittregelnAnzeigen() 
{
    CreateZuschnittDlg();
}

void CMainFrame::OnUpdateZuschnittregelnAnzeigen(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();
}

LRESULT CMainFrame::OnZuschnitteBerechnen(WPARAM wParam, LPARAM lParam)
{
    CMDIChildWnd *pChild = (CMDIChildWnd *)GetActiveFrame();
    if (pChild != NULL)
    {
        CTtmainView* pView = (CTtmainView*)pChild->GetActiveView();
        if (pView != NULL)
        {
            pView->computeAndDraw();
        }
    }
    return 0L;
}


BOOL CMainFrame::SetupRegistryValues()
{
    BOOL bCreated;
    CString strSubKey, strValue;
    CTtmainApp *pApp = (CTtmainApp*) AfxGetApp();
    if (pApp)
    {
        strSubKey = _T("");
        strValue = _T("POS_X");
        m_riPosX.Connect(pApp->GetRegistryManager(), strSubKey,
            strValue, bCreated);
        if (bCreated == TRUE) m_riPosX = 20;

        strValue = _T("POS_Y");
        m_riPosY.Connect(pApp->GetRegistryManager(), strSubKey,
            strValue, bCreated);
        if (bCreated == TRUE) m_riPosY = 40;

        strValue = _T("SIZE_X");
        m_riSizeX.Connect(pApp->GetRegistryManager(), strSubKey,
            strValue, bCreated);
        if (bCreated == TRUE) m_riSizeX = 600;

        strValue = _T("SIZE_Y");
        m_riSizeY.Connect(pApp->GetRegistryManager(), strSubKey,
            strValue, bCreated);
        if (bCreated == TRUE) m_riSizeY = 600;

        strValue = _T("WINDOW_STYLES");
        m_ridWindowStyles.Connect(pApp->GetRegistryManager(), strSubKey,
            strValue, bCreated);
        if (bCreated == TRUE) m_ridWindowStyles = 0;
        m_dwMinimized = FALSE;
    }

    return TRUE;
}

void CMainFrame::OnMove(int x, int y) 
{
    CRect rc;

	CMDIFrameWnd::OnMove(x, y);
	
//    m_riPosX = x-4;
//    m_riPosY = y-50; //42;   //52;        //??	
    GetWindowRect(&rc);
    m_riPosX = rc.left;
    m_riPosY = rc.top;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
    if (nType == SIZE_RESTORED)
    {
        CRect rc;
        GetWindowRect(&rc);

        m_dwMinimized = FALSE;
        m_riSizeX = rc.Width();   //->Frame links+rechts
        m_riSizeY = rc.Height();  //??56;  // Frame unten + System-Leiste oben
        m_ridWindowStyles = SW_SHOW;
    }
    else
        if (nType == SIZE_MINIMIZED)
        {
            m_ridWindowStyles = WS_MINIMIZE;
            m_dwMinimized = TRUE;	
        }
        else if (nType == SIZE_MAXIMIZED)
        {
            m_ridWindowStyles = WS_MAXIMIZE;
        }
}

static CZRSheet* g_pSheet = NULL;

CZRSheet* CMainFrame::CreateZuschnittDlg()
{
    CTtmainApp* pApp = (CTtmainApp*)AfxGetApp();
    if (pApp)
    {
        for (int i=0; i<=1; i++)
        {
            // 2 Versuche
            if (g_pSheet == NULL)
            {
                // es gibt noch kein sheet
                g_pSheet = new CZRSheet(_T(""), pApp->GetZuschnittManager(), 
                    this);  // Sheet gibt sich selbst wieder frei

	            // create a modeless property page
	            if (!g_pSheet->Create(this, WS_VISIBLE | WS_OVERLAPPED | WS_THICKFRAME|
                    WS_DLGFRAME | WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU|DS_CONTEXTHELP|
                        WS_CLIPCHILDREN, WS_EX_OVERLAPPEDWINDOW | WS_EX_MDICHILD 
		           ))
	            {
                    delete g_pSheet;
                    g_pSheet = NULL;
		            return NULL;
	            }
                else
                {
                    return g_pSheet;
                }
            }
            else
            {
#if 0
                // sheet lˆschen und neu aufbauen
                CWnd* pWnd = g_pSheet;
                g_pSheet = NULL;    // anzeigen: neu erzeugen
                pWnd->SendMessage(WM_CLOSE, 0, 0);
#else
                // nicht neu erzeugen
                return g_pSheet;
#endif
            }
        }
    }

    return NULL;
}

LRESULT CMainFrame::OnZRSheetClosed(WPARAM wParam, LPARAM lParam)
{
    if (g_pSheet != NULL)
    {
        g_pSheet = NULL;
    }
    else
    {
        // Sheet muﬂ neu erzeugt werden!!!
    }
    return 0L;
}

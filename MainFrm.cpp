// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#include "TTData.h"
#include "Tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "Testsystem\TTTestManager.h"
#include "ttmain.h"
#include "MainFrm.h"
#include "AuswDlg.h"
#include "TorwahlDlg.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "KreisBerechnungDlg.h"
#include "ZRTreeCtrl.h"
#include "ZRSheet.h"
#include "BHTestDlg.h"
#include "WhsProfilDlg.h"
#include "AnsteuerungPS312\PS312TestDlg.h"
#include "AnsteuerungPS312\PS312CtrlDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
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
	ON_COMMAND(ID_REFTOR_SPEICHERN, OnRefTorSpeichern)
	ON_UPDATE_COMMAND_UI(ID_REFTOR_SPEICHERN, OnUpdateRefTorSpeichern)
	ON_UPDATE_COMMAND_UI(ID_REFTOR_LADEN, OnUpdateRefTorLaden)
	ON_COMMAND(ID_ZUSCHNITTREGELN_ANZEIGEN, OnZuschnittregelnAnzeigen)
	ON_UPDATE_COMMAND_UI(ID_ZUSCHNITTREGELN_ANZEIGEN, OnUpdateZuschnittregelnAnzeigen)
	ON_COMMAND(ID_TEST_START, OnTestStart)
	ON_UPDATE_COMMAND_UI(ID_TEST_START, OnUpdateTestStart)
    ON_MESSAGE(WM_BERECHNE_ZUSCNITTE, OnZuschnitteBerechnen)
    ON_MESSAGE(WM_ZR_SHEET_CLOSED, OnZRSheetClosed)
    ON_MESSAGE(WM_SPLIT_SHEET, OnSplitSheet)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TEST_EINSTELLUNGEN, OnTestEinstellungen)
	ON_UPDATE_COMMAND_UI(ID_TEST_EINSTELLUNGEN, OnUpdateTestEinstellungen)
	ON_COMMAND(IDC_PROFILE_BEARBEITEN, OnProfileBearbeiten)
	ON_UPDATE_COMMAND_UI(IDC_PROFILE_BEARBEITEN, OnUpdateProfileBearbeiten)
	ON_WM_INITMENU()
	ON_COMMAND(ID_BH_TEST, OnBhTest)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_WHSPROFILE, &CMainFrame::OnWhsprofile)
    ON_COMMAND(ID_EXTRAS_TEST_PS312, &CMainFrame::OnTestPs312)
    ON_COMMAND(ID_EXTRAS_START_PS312, &CMainFrame::OnExtrasStartPs312)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETMENU, OnResetMenu)
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
    m_pSplitTor = NULL;
    SetupRegistryValues();

    m_pTestMan = NULL;
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

    if (m_pTestMan)
    {
        delete m_pTestMan;
        m_pTestMan = NULL;
    }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#if 1
    if (-1 == __super::OnCreate(lpCreateStruct))
    {
        return -1;
    }
#else
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
#endif // 1
	
	// Visuellen Manager zum Zeichnen aller Benutzeroberflächenelemente festlegen
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));

#if 1
	CMDITabInfo mdiTabParams;
    memset(&mdiTabParams, 0, sizeof(mdiTabParams));
#if 1
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // Weitere Stile verfügbar...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // Auf "FALSE" festlegen, um die Schaltfläche "Schließen" rechts auf der Registerkarte zu platzieren
	mdiTabParams.m_bTabIcons = FALSE;    // Auf "TRUE" festlegen, um Dokumentsymbole auf MDI-Registerkarten zu aktivieren
	mdiTabParams.m_bAutoColor = TRUE;    // Auf "FALSE" festlegen, um automatische Farben für MDI-Registerkarten zu deaktivieren
	mdiTabParams.m_bDocumentMenu = TRUE; // Dokumentmenü am rechten Rand des Registerkartenbereichs aktivieren
#endif
	//EnableMDITabbedGroups(TRUE, mdiTabParams);
    EnableMDITabs(TRUE, TRUE, CMFCTabCtrl::LOCATION_TOP, TRUE, CMFCTabCtrl::STYLE_3D_ONENOTE, FALSE, TRUE);
//    EnableMDITabs(BOOL bEnable/* = TRUE*/, BOOL bIcons/* = TRUE*/, CMFCTabCtrl::Location tabLocation /* = CMFCTabCtrl::LOCATION_BOTTOM*/,
//        BOOL bTabCloseButton/* = FALSE*/, CMFCTabCtrl::Style style/* = CMFCTabCtrl::STYLE_3D_SCROLLED*/, BOOL bTabCustomTooltips/* = FALSE*/, BOOL bActiveTabCloseButton/* = FALSE*/)

#else
    CMDITabInfo params;

    params.m_tabLocation = theApp.m_Options.m_bTabsOnTop ? CMFCTabCtrl::LOCATION_TOP : CMFCTabCtrl::LOCATION_BOTTOM;
    params.m_style = theApp.m_Options.m_nTabsStyle;
    params.m_bTabCloseButton = !theApp.m_Options.m_bActiveTabCloseButton;
    params.m_bActiveTabCloseButton = theApp.m_Options.m_bActiveTabCloseButton;
    params.m_bAutoColor = theApp.m_Options.m_bTabsAutoColor;
    params.m_bDocumentMenu = theApp.m_Options.m_bMDITabsDocMenu;
    params.m_bEnableTabSwap = theApp.m_Options.m_bDragMDITabs;
    params.m_nTabBorderSize = theApp.m_Options.m_nMDITabsBorderSize;
    params.m_bTabIcons = theApp.m_Options.m_bMDITabsIcons;
    params.m_bFlatFrame = theApp.m_Options.m_bFlatFrame;
    params.m_bTabCustomTooltips = theApp.m_Options.m_bCustomTooltips;

    EnableMDITabbedGroups(TRUE, params);
#endif
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Fehler beim Erstellen der Menüleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

    m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC/* | CBRS_TOOLTIPS | CBRS_FLYBY*/);

	//------------- Example --------------------//
	// do not allow modification of the dummy item in customize mode
	CList<UINT, UINT> lstProtectedCmds;
	lstProtectedCmds.AddTail (ID_REFTOR_SPEICHERN);
	CMFCToolBarButton::SetProtectedCommands (lstProtectedCmds);

    // Verhindern, dass die Menüleiste beim Aktivieren den Fokus erhält
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Fehler beim Erstellen der Symbolleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	CString strToolBarName = _T("Standard");
//	strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	//ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


    if (m_ridWindowStyles.GetVal() & WS_MINIMIZE)
        ShowWindow(SW_SHOWMINIMIZED);

    if (m_ridWindowStyles.GetVal() & WS_MAXIMIZE)
    {
        AfxGetApp()->m_nCmdShow = SW_SHOWMAXIMIZED;
    }
//        ShowWindow(SW_SHOWMAXIMIZED);    

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// Andockfensterverhalten wie in Visual Studio 2005 aktivieren
	CDockingManager::SetDockingMode(DT_SMART);
	// Automatisches Ausblenden von Andockfenstern wie in Visual Studio 2005 aktivieren
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Dialogfeld für erweiterte Fensterverwaltung aktivieren
//	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Umpositionieren des Menüs für Symbolleisten und Andockfenster aktivieren
//	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// Schnelles Anpassen von Symbolleisten mit Alt+Ziehen aktivieren
//	CMFCToolBar::EnableQuickCustomization();

#if 1 //???
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);
#endif // 0
    if (m_pTestMan == NULL)
    {
        m_pTestMan = new CTTTestManager;
    }
#if 1
    if (m_pTestMan)
    {
        CTtmainApp *pApp = (CTtmainApp*) AfxGetApp();
        m_pTestMan->Initialize(this, pApp->GetRegistryManager());
        if (pApp->IsInTestMode() == TRUE)
            // Testsystem Menu hinzu
            AppendTestsystemMenu();
    }
#endif // 0

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
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
#if 0
    if (m_ridWindowStyles.GetVal() & WS_MINIMIZE)
        cs.style |= WS_MINIMIZE;
#endif // 0


	return TRUE;
}


void CMainFrame::LoadToolBar(UINT nId)
{
    m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
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
	if (pView->addTorDoor() == -1)
    {
        // Tor passt nicht mehr auf Blatt, neues Blatt erzeugen
        SplitSheet();
    }

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
			pCmdUI->Enable(pView->canAddTor());
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
        CScrollView* pView = (CScrollView*)pChild->GetActiveView();
        if ( pView->IsKindOf( RUNTIME_CLASS(CTtmainView) ) )
        {
			pCmdUI->Enable((((CTtmainView*)pView)->getTorCount() > 0));
			return;
        }
        else
            pCmdUI->Enable(FALSE);
	}
	pCmdUI->Enable(FALSE);			
}


void CMainFrame::OnRefTorSpeichern()
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
                    pView->saveTorDoor(iTor, NULL);
                }
            }
            else
			    pView->saveTorDoor(0, NULL);
		}
	}	
}

void CMainFrame::OnUpdateRefTorSpeichern(CCmdUI* pCmdUI)
{
    OnUpdateTorAendern(pCmdUI);
}

void CMainFrame::OnUpdateRefTorLaden(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);			
}


void CMainFrame::OnTestStart()
{
    if (m_pTestMan)
        m_pTestMan->DoTest();
}

void CMainFrame::OnUpdateTestStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);			
}

void CMainFrame::OnTestEinstellungen() 
{
	m_pTestMan->ConfigureDlg();
}

void CMainFrame::OnUpdateTestEinstellungen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CMDIFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnZuschnittregelnAnzeigen() 
{
    CreateZuschnittDlg();
}

void CMainFrame::OnUpdateZuschnittregelnAnzeigen(CCmdUI* pCmdUI) 
{
    CMDIChildWnd *pChild = (CMDIChildWnd *)GetActiveFrame();
    CScrollView* pView = (CScrollView*)pChild->GetActiveView();
    if (pView != NULL)
    {
        if ( pView->IsKindOf( RUNTIME_CLASS(CTtmainView) ) )
            pCmdUI->Enable();
        else
            pCmdUI->Enable(FALSE);
    }
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

	CMDIFrameWndEx::OnMove(x, y);
	
//    m_riPosX = x-4;
//    m_riPosY = y-50; //42;   //52;        //??	
    GetWindowRect(&rc);
    m_riPosX = rc.left;
    m_riPosY = rc.top;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);
	
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
                // sheet löschen und neu aufbauen
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
        // Sheet muß neu erzeugt werden!!!
    }
    return 0L;
}


// Function name	: OnSplitSheet
// Description	    : aufgerufen von View, wenn Blatt aufgeteilt werden soll
// Return type		: LRESULT 
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CMainFrame::OnSplitSheet(WPARAM wParam, LPARAM lParam)
{
    SplitSheet();
    return 0L;
}

// Function name	: CMainFrame::SplitSheet
// Description	    : teilt ein Blatt in zwei auf
// Return type		: void 
// Argument         : void
void CMainFrame::SplitSheet(void)
{
	CMDIChildWnd* pChild =  MDIGetActive();
	CTtmainView* pView = (CTtmainView*)pChild->GetActiveView();		
	if (pView != NULL)
	{
		if (pView->getTorCount() > 1)
        {
            CTorDoor* pTor = pView->removeTor(1);
            if (pTor)
            {
                m_pSplitTor = pTor;
                CTtmainApp* pApp = (CTtmainApp*)AfxGetApp();
                if (pApp)
                {
                    MessageBox("Die zwei Tore passen nicht mehr auf ein Blatt. Das Tor wird auf ein eigenes Blatt gelegt!", 
                        "Info", MB_OK);
                    pApp->OnFileNew();
//                    pT->AddDocument(pDoc);
//                    pApp->OnFileNew();
                }
            }
        }
	}    
}


void CMainFrame::AppendTestsystemMenu(void)
{
#if 0
    CMenu* pMenu = GetMenu();

    m_menuTest.LoadMenu(IDM_TESTSYSTEM);

    HMENU hTestMenu = m_menuTest.m_hMenu;

    if (m_wndMenuBar.AppendMenu(MF_POPUP, (UINT)hTestMenu, "&Testsystem") == FALSE)
    {
        int i=0;
    }
#else
    CMenu menu;
    menu.LoadMenu(IDM_TESTSYSTEM);

	m_wndMenuBar.InsertButton (CMFCToolBarMenuButton (0, menu, -1, _T ("&Test")));
	m_wndMenuBar.AdjustLayout();
	m_wndMenuBar.AdjustSizeImmediate ();
#endif // 0
}

void CMainFrame::OnProfileBearbeiten() 
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
                    pView->editProfiles(iTor);
                }
            }
            else
			    pView->editProfiles(0);
		}
	}		
}

void CMainFrame::OnUpdateProfileBearbeiten(CCmdUI* pCmdUI) 
{
    OnUpdateTorAendern(pCmdUI);
}

void CMainFrame::OnBhTest() 
{
    CBHTestDlg dlg;
   	dlg.DoModal();
}

void CMainFrame::OnWhsprofile()
{
    // TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
    CWhsProfilDlg dlg;

    dlg.DoModal();

    CTtmainApp* pApp = (CTtmainApp*)AfxGetApp();
    if (pApp)
        pApp->m_iLastMode = 1;
}

void CMainFrame::OnTestPs312()
{
    CPS312TestDlg dlg;

    dlg.DoModal();
}

void CMainFrame::OnExtrasStartPs312()
{
    CPS312CtrlDlg dlg;

    dlg.DoModal();
}

LRESULT CMainFrame::OnResetMenu(WPARAM,LPARAM)
{
    if (m_pTestMan)
    {
        CTtmainApp *pApp = (CTtmainApp*) AfxGetApp();
        m_pTestMan->Initialize(this, pApp->GetRegistryManager());
        if (pApp->IsInTestMode() == TRUE)
            // Testsystem Menu hinzu
            AppendTestsystemMenu();
    }
	return 0;
}

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ttmain.h"

#include "MainFrm.h"

#include "TTData.h"
#include "Tordoor.h"
#include "AuswDlg.h"
#include "TorwahlDlg.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "KreisBerechnungDlg.h"

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
}

CMainFrame::~CMainFrame()
{
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

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	
    cs.cy = ::GetSystemMetrics(SM_CYSCREEN) - 35; 
    cs.cx = ::GetSystemMetrics(SM_CXSCREEN) - 35; 
    cs.y = 0;//35 / 2;     
	cs.x = 35 / 2;	
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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
	// TODO: Add your command handler code here
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

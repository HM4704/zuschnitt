// SysBasePage.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "SysBasePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysBasePage property page

IMPLEMENT_DYNCREATE(CSysBasePage, CPropertyPage)

// nur für IMPLEMENT_DYNCREATE == Serializing!!!
CSysBasePage::CSysBasePage() 
            : CPropertyPage()
{
    m_bDlgInitDone = FALSE;
}

CSysBasePage::CSysBasePage(UINT nIdMenu, UINT nIdDlg) 
            : CPropertyPage(nIdDlg)
{
	//{{AFX_DATA_INIT(CSysBasePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_Menu.LoadMenu(nIdMenu);
    m_bDlgInitDone = FALSE;
}

CSysBasePage::~CSysBasePage()
{
    m_Menu.DestroyMenu();
}

void CSysBasePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysBasePage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysBasePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSysBasePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysBasePage message handlers

BOOL CSysBasePage::OnSetActive() 
{
    CWnd* pFrameWnd = AfxGetMainWnd();

    if (pFrameWnd)
    {
        CMenu *pMenu = pFrameWnd->GetMenu();
        if (pMenu != &m_Menu)
        {
            pFrameWnd->SetMenu(&m_Menu);
        }
    }
    
	return CPropertyPage::OnSetActive();
}

BOOL CSysBasePage::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    // Problem: default ruft Parent auf -> rekursiver Aufruf, Wie anders lösen????
    // kopiert aus mfc\src\dlgcore.cpp
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if ((nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI) ||
			/*!IS_COMMAND_ID(nID) ||*/ nID >= 0xf000)
	{
		// control notification or non-command button or system command
		return FALSE;       // not routed any further
	}

	// last crack goes to the current CWinThread object
	CWinThread* pThread = AfxGetThread();
	if (pThread != NULL)
	{
#ifdef _DEBUG
		if (afxTraceFlags & traceCmdRouting)
			TRACE1("Routing command id 0x%04X to app.\n", nID);
#endif
		if (pThread->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

#ifdef _DEBUG
	if (afxTraceFlags & traceCmdRouting)
	{
		TRACE2("IGNORING command id 0x%04X sent to %hs dialog.\n", nID,
				GetRuntimeClass()->m_lpszClassName);
	}
#endif
	return FALSE;
}

BOOL CSysBasePage::PlaceItems(CRect &rect)
{
    rect; // to avoid warning

    if (m_bDlgInitDone == FALSE)
        return FALSE;

    return TRUE;
}

BOOL CSysBasePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_bDlgInitDone = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

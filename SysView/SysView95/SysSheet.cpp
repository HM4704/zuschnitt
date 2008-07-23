// SysSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "SysSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSheet

IMPLEMENT_DYNAMIC(CSysSheet, CPropertySheet)

CSysSheet::CSysSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_rectSize =0;    
    SetupRegistryValues();
    AddPages();
}

CSysSheet::CSysSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    SetupRegistryValues();
    AddPages();
}

CSysSheet::~CSysSheet()
{
    RemovePages();
}


BEGIN_MESSAGE_MAP(CSysSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSysSheet)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_SWITCH_TO_PROCESS, OnSwitchToProcess)
	//}}AFX_MSG_MAP
    ON_MESSAGE (WM_RESIZEPAGE, OnResizePage)
END_MESSAGE_MAP()


BOOL CSysSheet::SetupRegistryValues()
{
    BOOL bCreated;
    CSysView95App *pApp = (CSysView95App*) AfxGetApp();
    m_riActivePage = 0;
    if (pApp)
    {
        return m_riActivePage.Connect(pApp->GetRegistryManager(), "", 
            "ACT_PAGE", bCreated);
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CSysSheet message handlers

BOOL CSysSheet::AddPages()
{

    m_pApplicationPg = new CApplicationPg;
    if (m_pApplicationPg)
        AddPage(m_pApplicationPg);

    m_pProcessPg = new CProcessPg;
    if (m_pProcessPg)
        AddPage(m_pProcessPg);

    m_pPerformancePg = new CPerformancePg;
    if (m_pPerformancePg)
        AddPage(m_pPerformancePg);

    return TRUE;
}

BOOL CSysSheet::RemovePages()
{
    if (m_pApplicationPg != NULL)
    {
        RemovePage(m_pApplicationPg);
        delete m_pApplicationPg;
        m_pApplicationPg = NULL;
    }

    if (m_pProcessPg != NULL)
    {
        RemovePage(m_pProcessPg);
        delete m_pProcessPg;
        m_pProcessPg = NULL;
    }

    if (m_pPerformancePg != NULL)
    {
        RemovePage(m_pPerformancePg);
        delete m_pPerformancePg;
        m_pPerformancePg = NULL;
    }

    return TRUE;
}

void CSysSheet::OnSize(UINT nType, int cx, int cy) 
{
    static bFirst = TRUE;
	CPropertySheet::OnSize(nType, cx, cy);
	
    if (nType == SIZE_RESTORED)
    {
        if (bFirst == TRUE)
        {
            m_rectSize.left = 0; m_rectSize.top = 0;
            m_rectSize.right = cx; m_rectSize.bottom = cy;
        }

        // resize Property Page
        // 
	    CRect rectSize;
        rectSize.top = 0;
        rectSize.left = 0;
        rectSize.right = cx-15;     // <- muß größer sein als bei TabCtrl
        rectSize.bottom = cy-15;    // <- muß größer sein als bei TabCtrl
        ResizePages(&rectSize);

        // resize the CTabCtrl
        RECT rc;
        CTabCtrl* pTab = GetTabControl ();     
        if (pTab == NULL)
            return;
        ASSERT (pTab);
        pTab->GetWindowRect (&rc);     
        ScreenToClient (&rc);     
        rc.right = cx-10;           // <- muß kleiner sein als bei Page
        rc.bottom = cy-10;          // <- muß kleiner sein als bei Page
        pTab->MoveWindow (&rc);

        bFirst = FALSE;
    }		
}

BOOL CSysSheet::ResizePages(LPRECT rect)
{
    CPropertyPage* pPage = GetActivePage ();     
    if (pPage == NULL)
        return TRUE;
    if (pPage->GetSafeHwnd() == NULL)
        return TRUE;
    // store page size in m_PageRect     
    pPage->GetWindowRect (&m_rectPage);
    ScreenToClient (&m_rectPage);     
    m_rectPage.right = rect->right;
    m_rectPage.bottom = rect->bottom;     
    pPage->MoveWindow (&m_rectPage);
    return TRUE;
}

LONG CSysSheet::OnResizePage(UINT, LONG) 
{
     // resize the page using m_PageRect which was set in OnInitDialog()
     CPropertyPage* pPage = GetActivePage ();     
     if (pPage == NULL)
         return 0;
     pPage->MoveWindow (&m_rectPage);     
     return 0; 
}

void CSysSheet::StartAllPages()
{
    for (int i = 0; i < GetPageCount(); i++)
    {
        SetActivePage(i);
    }

    SetActivePage(m_riActivePage.GetVal());
}

BOOL CSysSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: Add your specialized code here
    StartAllPages();	
	return bResult;
}

BOOL CSysSheet::PreTranslateMessage(MSG* pMsg) 
{
	// since CPropertySheet::PreTranslateMessage will eat frame window
	// accelerators, we call all frame windows' PreTranslateMessage first.
	CFrameWnd* pFrameWnd = GetParentFrame();   // start with first parent frame
	while (pFrameWnd != NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pFrameWnd = pFrameWnd->GetParentFrame();
	}
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}

BOOL CSysSheet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    // Problem: default ruft Parent auf -> rekursiver Aufruf, Wie anders lösen????
    // kopiert aus mfc\src\dlgprop.cpp
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if ((nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI) ||
			/*!IS_COMMAND_ID(nID) ||*/ nID >= 0xf000)
	{
		// control notification or non-command button or system command
		return FALSE;       // not routed any further
	}

    // pages inform
    CPropertyPage* pPage = GetActivePage();
    if (pPage != NULL && pPage->GetSafeHwnd() != NULL)
    {
        if (pPage->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
            return TRUE;
    }

    return FALSE;
//	return CPropertySheet::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CSysSheet::OnMove(int x, int y) 
{
	CPropertySheet::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

BOOL CSysSheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    NMHDR* pnmh = (LPNMHDR) lParam;
    // the sheet resizes the page whenever it is activated
    // so we need to resize it to what we want 
    if (TCN_SELCHANGE == pnmh->code)
         // user-defined message needs to be posted because page must
         // be resized after TCN_SELCHANGE has been processed
         PostMessage (WM_RESIZEPAGE);

	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}


void CSysSheet::OnDestroy() 
{
	CPropertySheet::OnDestroy();
	
    m_riActivePage = GetActiveIndex();
    m_riActivePage.Update();
}

LRESULT  CSysSheet::OnSwitchToProcess(WPARAM wParam, LPARAM lParam)
{
    if (m_pProcessPg)
    {
        if (SetActivePage(m_pProcessPg) == TRUE)
        {
            return m_pProcessPg->PostMessage(MSG_SWITCH_TO_PROCESS,
                wParam, lParam);
        }
        else
            return 1L;
    }

    return 0L;
}

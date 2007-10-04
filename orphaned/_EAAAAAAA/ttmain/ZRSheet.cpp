// ZRSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Macros.h"
#include "Ttdata.h"
#include "tordoor.h"
#include "Node.h"
//#include "Testsystem\TTTestManager.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRTreeCtrl.h"
#include "ZRSheet.h"
#include "ZRBasePage.h"
#include "ZRSFPage.h"
#include "ZRProfilePage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Defines
// Button Defines
#define     SHEET_BUTTON_WIDTH      100
#define     SHEET_BUTTON_HEIGHT     30
#define     SHEET_BUTTON_SPACE_Y    10

// Tab states
#define     TS_CHANGED              0x00000001

/////////////////////////////////////////////////////////////////////////////
// CZRSheet

IMPLEMENT_DYNAMIC(CZRSheet, CPropertySheet)

CZRSheet::CZRSheet(UINT nIDCaption, CZuschnittManager* pMan, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_pMan = pMan;
    AddPages();
}

CZRSheet::CZRSheet(LPCTSTR pszCaption, CZuschnittManager* pMan, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_pMan = pMan;
    AddPages();
}

CZRSheet::~CZRSheet()
{
    SAFE_DELETE(m_pPPSenk)
    SAFE_DELETE(m_pPPQuer)
    SAFE_DELETE(m_pPPOber)
    SAFE_DELETE(m_pPPUnter)
    SAFE_DELETE(m_pPPSf)
}


BEGIN_MESSAGE_MAP(CZRSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CZRSheet)
	ON_WM_SIZE()
	ON_WM_CREATE()
    ON_MESSAGE(WM_ZR_SHEET_SET_BUTTONS, OnSetButtons)
    ON_MESSAGE(WM_ZR_SHEET_PAGED_CHANGED, OnPageChanged)
    ON_MESSAGE(DM_GETDEFID, OnGetDefId)
    ON_MESSAGE(WM_ZR_SHEET_SHOW_RULES, OnShowRules)
	ON_WM_PAINT()
    ON_BN_CLICKED(ID_ZR_SHEET_NEW, OnNewTree)
    ON_BN_CLICKED(IDOK, OnApply)
    ON_BN_CLICKED(IDHELP, OnHelp)
    ON_BN_CLICKED(ID_ZR_SHEET_SAVE, OnSave)
	ON_WM_SYSDEADCHAR()
	ON_WM_SYSCHAR()
	ON_WM_CHAR()
	ON_WM_DEADCHAR()
	ON_WM_KEYDOWN()
	ON_WM_SYSCOMMAND()
    ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRSheet message handlers

BOOL CZRSheet::AddPages()
{
    m_pPPSenk = new CZRProfilePage(_T("Senkrecht"), m_pMan->BaumAnfordern(SENK));
    if (m_pPPSenk)
        AddPage(m_pPPSenk);
    m_pPPQuer = new CZRProfilePage(_T("Quer"), m_pMan->BaumAnfordern(QUER));
    if (m_pPPQuer)
        AddPage(m_pPPQuer);
    m_pPPOber = new CZRProfilePage(_T("Oberteil"), m_pMan->BaumAnfordern(OBER));
    if (m_pPPOber)
        AddPage(m_pPPOber);
    m_pPPUnter = new CZRProfilePage(_T("Unterteil"), m_pMan->BaumAnfordern(UNTER));
    if (m_pPPUnter)
        AddPage(m_pPPUnter);
    
    m_pPPSf = new CZRSFPage(_T("Schiebefenster"), m_pMan);
    if (m_pPPSf)
        AddPage(m_pPPSf);

    return TRUE;
}

void CZRSheet::OnSize(UINT nType, int cx, int cy) 
{
    static BOOL bFirst = TRUE;
	CPropertySheet::OnSize(nType, cx, cy);

    if (nType == SIZE_RESTORED)
    {
        // resize the CTabCtrl
        RECT rc;
        CTabCtrl* pTab = GetTabControl ();     
        if (pTab == NULL)
            return;
        ASSERT (pTab);
        pTab->GetWindowRect (&rc);     
        ScreenToClient (&rc);     
        rc.right = cx-SHEET_BUTTON_WIDTH-10;           // <- muß kleiner sein als bei Page
        rc.bottom = cy-10;          // <- muß kleiner sein als bei Page
        pTab->MoveWindow (&rc);

        // resize Property Page
        // 
        rc.left += 10;
        rc.right -= 4;
        rc.top += 10;
        rc.bottom -= 4;
        ResizePages(&rc);

        ResizeButtons(&rc);
    }		

}

BOOL CZRSheet::ResizePages(LPRECT rect)
{
    for (int i=0; i<GetPageCount(); i++)
    {
        CPropertyPage* pPage = GetPage(i);     
        if (pPage == NULL)
            continue;
        if (pPage->GetSafeHwnd() == NULL)
            continue;

        // store page size in m_PageRect     
        pPage->GetWindowRect (&m_rectPage);
        ScreenToClient (&m_rectPage);     
        m_rectPage.right = rect->right;
        m_rectPage.bottom = rect->bottom;     
        pPage->MoveWindow (&m_rectPage);
    }
    return TRUE;
}

void CZRSheet::PostNcDestroy() 
{
	CPropertySheet::PostNcDestroy();
	delete this;
}


BOOL CZRSheet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
#if 0	
	return CPropertySheet::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
#else
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
#endif
}

BOOL CZRSheet::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CPropertySheet::PreCreateWindow(cs);
}

int CZRSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    CWnd* pWnd = AfxGetMainWnd();
    if (pWnd != NULL)
    {
        CRect rect;
        pWnd->GetClientRect(&rect);
//        ClientToScreen(&rect);
        int iWidth = rect.Width()/3;
        int iHeight = rect.Height()/2;
        lpCreateStruct->x  = 0;//rect.right - iWidth;
        lpCreateStruct->y  = rect.bottom - iHeight; 
        lpCreateStruct->cx = iWidth;
        lpCreateStruct->cy = iHeight;
    }

	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CZRSheet::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CZRSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

    // richtiges Icon anzeigen
    CWinApp* pApp = AfxGetApp();
    if (pApp != NULL)
    {
        HICON hIcon = pApp->LoadIcon(IDR_MAINFRAME);
        SendMessage(WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
    // Plazieren:
    CRect rect, dlgRect;
    CWnd* pWnd = AfxGetMainWnd();
    if (pWnd != NULL)
    {
        pWnd->GetClientRect(&rect);
        pWnd->ClientToScreen(&rect);
        int iWidth = rect.Width()*2/5;
        int iHeight = rect.Height()*3/4;
        dlgRect.left  = rect.right - iWidth - SHEET_BUTTON_WIDTH - 30;
        dlgRect.top   = rect.top + 50;
        dlgRect.right = dlgRect.left + iWidth;
        dlgRect.bottom = rect.top+iHeight;

        MoveWindow(&dlgRect);
        GetWindowRect(&dlgRect);
    }
//    g_DlgRect = rect;

	
    // Title setzen
    SetWindowText(_T("Regeln für Zugabewerte"));

    // Größe für Buttons verändern
    CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width()+SHEET_BUTTON_WIDTH+20,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    // Buttons erzeugen
    m_ButtonApply.Create("Anwenden", WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON, rect, this,
        IDOK);
    m_ButtonSave.Create("Speichern", WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON, rect, this, 
        ID_ZR_SHEET_SAVE);
    m_ButtonCancel.Create("Beenden", WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON, rect, this, 
        IDCANCEL);
    m_ButtonHelp.Create("Hilfe", WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON, rect, this, 
        IDHELP);
    m_ButtonNew.Create("Neue Regel", WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON, rect, this, 
        ID_ZR_SHEET_NEW);

    // Font für Buttons
    m_ButtonApply.SetFont(GetFont());
    m_ButtonSave.SetFont(GetFont());
    m_ButtonCancel.SetFont(GetFont());
    m_ButtonHelp.SetFont(GetFont());
    m_ButtonNew.SetFont(GetFont());

    CZRBasePage* pPage = (CZRBasePage*)GetActivePage();
    if (pPage)
    {
        SetButtonsStruct buttons;
        pPage->GetButtonStates(&buttons);
        OnSetButtons(0, (LPARAM)&buttons);
    }

    // alle Pages fertig initialisieren
    StartAllPages();

    m_toolTip.Create(this, 0);
    CRect tTrect(0, 0, 300, 200);
    m_toolTip.AddTool(&m_ButtonApply, _T("Regeln auf Tor anwenden"), &tTrect, IDOK);
    m_toolTip.AddTool(&m_ButtonSave, _T("Regeländerungen abspeichern"), &tTrect, ID_ZR_SHEET_SAVE);
    m_toolTip.AddTool(&m_ButtonCancel, _T("Diesen Dialog beenden"), &tTrect, IDCANCEL);
    m_toolTip.AddTool(&m_ButtonNew, _T("Neue Regel anlegen"), &tTrect, ID_ZR_SHEET_NEW);
    m_toolTip.AddTool(&m_ButtonHelp, _T("Hilfe anzeigen"), &tTrect, IDHELP);
    m_toolTip.Activate(TRUE);
	return bResult;
}

BOOL CZRSheet::ResizeButtons(LPRECT rectPage)
{
    CRect rect, clientRect;

    GetClientRect(&clientRect);
    rect.left = rectPage->right+10;
    rect.right = clientRect.right-10;
    rect.bottom = rectPage->bottom;
    rect.top = rect.bottom - SHEET_BUTTON_HEIGHT;
    if (m_ButtonHelp.GetSafeHwnd() != NULL)
    {
        m_ButtonHelp.MoveWindow(&rect);
    }
    rect.bottom = rect.top-2*SHEET_BUTTON_SPACE_Y;
    rect.top = rect.bottom-SHEET_BUTTON_HEIGHT;
    if (m_ButtonCancel.GetSafeHwnd() != NULL)
    {
        m_ButtonCancel.MoveWindow(&rect);
    }
    rect.bottom = rect.top-SHEET_BUTTON_SPACE_Y;
    rect.top = rect.bottom-SHEET_BUTTON_HEIGHT;
    if (m_ButtonSave.GetSafeHwnd() != NULL)
    {
        m_ButtonSave.MoveWindow(&rect);
    }
    rect.bottom = rect.top-SHEET_BUTTON_SPACE_Y;
    rect.top = rect.bottom-SHEET_BUTTON_HEIGHT;
    if (m_ButtonApply.GetSafeHwnd() != NULL)
    {
        m_ButtonApply.MoveWindow(&rect);
    }
    rect.bottom = rect.top-2*SHEET_BUTTON_SPACE_Y;
    rect.top = rect.bottom-SHEET_BUTTON_HEIGHT;
    if (m_ButtonNew.GetSafeHwnd() != NULL)
    {
        m_ButtonNew.MoveWindow(&rect);
    }
//    rect.top = rect.bottom+SHEET_BUTTON_SPACE_Y;
//    rect.bottom = rect.top+SHEET_BUTTON_HEIGHT;

    return TRUE;
}

LRESULT CZRSheet::OnSetButtons(WPARAM wParam, LPARAM lParam)
{
    SetButtonsStruct* pSetButtons = (SetButtonsStruct*)lParam;

    if (pSetButtons)
    {
        SetButton(m_ButtonApply, pSetButtons->dwStateButtonApply);
        SetButton(m_ButtonSave, pSetButtons->dwStateButtonSave);
        SetButton(m_ButtonCancel, pSetButtons->dwStateButtonCancel);
        SetButton(m_ButtonHelp, pSetButtons->dwStateButtonHelp);
        SetButton(m_ButtonNew, pSetButtons->dwStateButtonNew);
    }
    OnPaint();
    return 0L;
}

LRESULT CZRSheet::OnPageChanged(WPARAM wParam, LPARAM lParam)
{
    SetButtonsStruct* pSetButtons = (SetButtonsStruct*)lParam;

    if (pSetButtons)
    {
        SetButton(m_ButtonApply, pSetButtons->dwStateButtonApply);
        SetButton(m_ButtonSave, pSetButtons->dwStateButtonSave);
        SetButton(m_ButtonCancel, pSetButtons->dwStateButtonCancel);
        SetButton(m_ButtonHelp, pSetButtons->dwStateButtonHelp);
        SetButton(m_ButtonNew, pSetButtons->dwStateButtonNew);
    }

    if (pSetButtons->dwStateButtonSave & WS_VISIBLE)
    {
        // etwas im Baum hat sich geändert, das auch in der Titelleiste anzeigen
        SetTabState(GetActiveIndex(), TS_CHANGED);
    }

    OnPaint();

    return 0L;
}

void CZRSheet::SetButton(CButton &button, DWORD dwState)
{
    if (button.GetSafeHwnd() == NULL)
        return; 

    DWORD dwStyle = button.GetStyle();
    if (dwState & WS_VISIBLE)
    {
        if (dwStyle &  WS_DISABLED)
        {
            // Button enablen
            dwStyle &= ~WS_DISABLED;
            ::SetWindowLong(button.GetSafeHwnd(), GWL_STYLE, dwStyle);
        }
        button.ShowWindow(SW_SHOW);
    }
    else
    if (dwState & WS_DISABLED)
    {
        dwStyle |= WS_DISABLED;
        ::SetWindowLong(button.GetSafeHwnd(), GWL_STYLE, dwStyle);
    }
    else
    {
        button.ShowWindow(SW_HIDE);
    }
}

void CZRSheet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    if (m_ButtonApply.GetSafeHwnd() != NULL)
        m_ButtonApply.RedrawWindow();
    if (m_ButtonSave.GetSafeHwnd() != NULL)
        m_ButtonSave.RedrawWindow();
    if (m_ButtonCancel.GetSafeHwnd() != NULL)
        m_ButtonCancel.RedrawWindow();
    if (m_ButtonHelp.GetSafeHwnd() != NULL)
        m_ButtonHelp.RedrawWindow();
    if (m_ButtonNew.GetSafeHwnd() != NULL)
        m_ButtonNew.RedrawWindow();
    
	// Do not call CPropertySheet::OnPaint() for painting messages
}

void CZRSheet::OnNewTree()
{
    CWnd* pWnd = (CWnd*)GetActivePage();
    if (pWnd)
    {
        pWnd->PostMessage(WM_ZR_SHEET_NEW_TREE, 0, 0);
    }
}

void CZRSheet::StartAllPages()
{
    for (int i = 0; i < GetPageCount(); i++)
    {
        SetActivePage(i);
        SetTabState(i, 0);
    }
    SetActivePage(0);
}

void CZRSheet::OnApply()
{
    // sendet eine Botschaft an das Child-Fenster, um die Zuschnittwerte 
    // neu zu berechnen
    
    CWnd* pWnd = AfxGetMainWnd();
    if (pWnd != NULL)
    {
        pWnd->PostMessage(WM_BERECHNE_ZUSCNITTE, 0, 0);
    }
    else
    {
        //?? Fehler ausgeben
    }
}

void CZRSheet::OnCancel()
{
    BOOL bClose = TRUE;
    // Abfragen, ob alle Pages mit Schliessen einverstanden sind
    for (int i=0; i<GetPageCount(); i++)
    {
        CPropertyPage* pPage = GetPage(i);
        if (pPage)
        {
            if (pPage->OnQueryCancel() == FALSE)
            {
                // noch nicht schliessen
                bClose = FALSE;
                break;
            }
        }
    }
    if (bClose == FALSE)
    {
        int iRet;
        iRet = MessageBox("Sollen die Regel-Änderungen gespeichert werden?",
            "Achtung", MB_ICONQUESTION|MB_YESNOCANCEL);

        if (iRet == IDYES)
        {
            // ja, abspeichern und beenden
            OnSave();
        }
        else
        if (iRet == IDNO)
        {
            // nein, Zuschnitt-Datei neu laden, so daß die Änderungen verloren gehen
            m_pMan->ZuschnittDateiAuswerten(NULL);
        }
        else
        {
            // cancel, zurück zu Dialog
            return;
        }
    }
    OnApply();      // Änderungen anwenden

    CWnd* pWnd = AfxGetMainWnd();
    if (pWnd != NULL)
        pWnd->SetForegroundWindow();

    CWnd* pParent = (CWnd*)AfxGetMainWnd();
    if (pParent)
    {
        // Nachricht an Frame
        pParent->SendMessage(WM_ZR_SHEET_CLOSED, 0, 0);
    }
    DestroyWindow();
}

void CZRSheet::OnHelp()
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

        pApp->ShowHelp(szZuschnittProfileHilfe, PLACE_POS, &rect);
//        MoveWindow(g_DlgRect);
   }
}

void CZRSheet::OnSave()
{
    int iRet;

    iRet = MessageBox("Wollen Sie die Änderungen wirklich übernehmen?",
        "Achtung", MB_ICONQUESTION|MB_OKCANCEL);
    if (iRet == IDOK)
    {
        if (m_pMan->ZuschnittDateiAbspeichern(NULL) == TRUE)
        {
            // alle Tab states rücksetzen
            for (int i=0; i<GetPageCount(); i++)
            {
                SetTabState(i, 0);
                CZRBasePage* pPage = (CZRBasePage*)GetPage(i);
                if (pPage != NULL)
                {
                    if (pPage != m_pPPSf)
                    {
                        // Profile Page
                        ((CZRProfilePage*)(pPage))->m_treeZR.ResetChanged();
                    }
                    else
                    {
                        // SF Page
                        ((CZRSFPage*)(pPage))->ResetChanged();
                    }
                }
            }
            m_ButtonSave.EnableWindow(FALSE);
            CZRBasePage* pPage = (CZRBasePage*)GetActivePage();
            if (pPage != NULL && pPage != m_pPPSf)
            {
                ((CZRProfilePage*)(pPage))->m_treeZR.SetFocus();
            }
        }
        else
        {
            //?? Fehler ausgeben!!!!
        }
    }
}


BOOL CZRSheet::SetTabState(int iIndex, DWORD dwState)
{
    TCITEM item;
    CString strTitle = "";
    CTabCtrl* pTab = GetTabControl();
    if (pTab == NULL)
        return FALSE;

    CZRBasePage* pPage = (CZRBasePage*)GetPage(iIndex);
    if (pPage == NULL)
        return FALSE;

    if (dwState & TS_CHANGED)
    {
        strTitle = pPage->GetTitle();
        strTitle += " *";
        item.mask = TCIF_TEXT ;
        item.pszText = strTitle.GetBuffer(strTitle.GetLength());
        pTab->SetItem(iIndex, &item);
        strTitle.ReleaseBuffer();
    }
    else
    {
        strTitle = pPage->GetTitle();
        item.mask = TCIF_TEXT ;
        item.pszText = strTitle.GetBuffer(strTitle.GetLength());
        pTab->SetItem(iIndex, &item);
        strTitle.ReleaseBuffer();
    }
    return TRUE;
}

void CZRSheet::OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnSysDeadChar(nChar, nRepCnt, nFlags);
}

void CZRSheet::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnSysChar(nChar, nRepCnt, nFlags);
}

void CZRSheet::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnChar(nChar, nRepCnt, nFlags);
}

void CZRSheet::OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnDeadChar(nChar, nRepCnt, nFlags);
}

void CZRSheet::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CZRSheet::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnSysCommand(nID, lParam);
}

LRESULT CZRSheet::OnGetDefId(WPARAM wParam, LPARAM lParam)
{
    CZRBasePage* pPage = (CZRBasePage*)GetActivePage();
    if (pPage != NULL && pPage != m_pPPSf)
    {
        ((CZRProfilePage*)(pPage))->m_treeZR.ReturnWasPressed();
    }
    return 0L;
}

void CZRSheet::OnClose() 
{
    OnCancel();
//	CPropertySheet::OnClose();
}


// Function name	: OnShowRules
// Description	    : zeigt die Regeln für ein Tor bzw. Flügel an
// Return type		: LRESULT 
// Argument         : WPARAM wParam     CTorDoor*
// Argument         : LPARAM lParam     CFlParam*
LRESULT CZRSheet::OnShowRules(WPARAM wParam, LPARAM lParam)
{
    // Nodes holen und anzeigen
    BOOL    bRuleFound = FALSE;
    CNode *pSenk, *pQuer, *pOber, *pUnter;

    pSenk = pQuer = pOber = pUnter = NULL;
    CTorDoor* pTor = (CTorDoor*)wParam;
    CFlParam* pFl = (CFlParam*)lParam;

    if (pTor != NULL && pFl != NULL)
    {
        if (pFl->Geteilt == TRUE || pFl->OArt != LEER)
        {
            // irgendein Oberteil
            pOber = m_pMan->ZugabeNodeHolen(OBER, pTor, pFl);
            pUnter = m_pMan->ZugabeNodeHolen(UNTER, pTor, pFl);
        }
        else
        {
            // nur ein senkrechtes Teil
            pSenk = m_pMan->ZugabeNodeHolen(SENK, pTor, pFl);
        }
        pQuer = m_pMan->ZugabeNodeHolen(QUER, pTor, pFl);
        CZRBasePage* pPage = (CZRBasePage*)GetPage(0);
        if (pPage != NULL && pSenk != NULL)
        {
            if (pPage->SendMessage(WM_ZR_SHEET_SHOW_NODE, 0, (LPARAM)pSenk)
                == TRUE)
            {
                // in dieser Page ist eine Regel
                bRuleFound = TRUE;
                SetActivePage(pPage);
            }
        }            
        pPage = (CZRBasePage*)GetPage(1);
        if (pPage != NULL && pQuer != NULL)
        {
            if ((pPage->SendMessage(WM_ZR_SHEET_SHOW_NODE, 0, (LPARAM)pQuer)
                == TRUE) && (bRuleFound == FALSE)) 
            {
                // in dieser Page ist eine Regel
                bRuleFound = TRUE;
                SetActivePage(pPage);
            }
        }            
        pPage = (CZRBasePage*)GetPage(2);
        if (pPage != NULL && pOber != NULL)
        {
            if ((pPage->SendMessage(WM_ZR_SHEET_SHOW_NODE, 0, (LPARAM)pOber)
                == TRUE) && (bRuleFound == FALSE)) 
            {
                // in dieser Page ist eine Regel
                bRuleFound = TRUE;
                SetActivePage(pPage);
            }
        }            
        pPage = (CZRBasePage*)GetPage(3);
        if (pPage != NULL && pUnter != NULL)
        {
            if ((pPage->SendMessage(WM_ZR_SHEET_SHOW_NODE, 0, (LPARAM)pUnter)
                == TRUE) && (bRuleFound == FALSE)) 
            {
                // in dieser Page ist eine Regel
                bRuleFound = TRUE;
                SetActivePage(pPage);
            }
        }            
    }
    if (bRuleFound == FALSE)
    {
        CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
        if (pFrm != NULL)
        {
            pFrm->MessageBox("Es wurde keine Regel gefunden. Die Regeln für diesen Flügel sind im Programm eingebaut.", 
                "Schade", MB_OK | MB_ICONASTERISK);
        }
    }

    return 0L;
}

BOOL CZRSheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (m_toolTip.GetSafeHwnd() != NULL)
        m_toolTip.RelayEvent(pMsg);
	return CPropertySheet::PreTranslateMessage(pMsg);
}

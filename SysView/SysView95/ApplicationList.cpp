// ApplicationList.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "ApplicationList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SAFE_DELETE_ARR(PDATA)  if (PDATA != NULL)      \
                                {                       \
                                    delete [] PDATA;       \
                                    PDATA = NULL;       \
                                }  
                     
#define SAFE_DELETE(PDATA)      if (PDATA != NULL)      \
                                {                       \
                                    delete PDATA;       \
                                    PDATA = NULL;       \
                                }  

/////////////////////////////////////////////////////////////////////////////
// CApplicationList

CApplicationList::CApplicationList()
{
    m_bColumnsCreated = FALSE;
    m_pListField = NULL;
    m_bMustRefreshItems = TRUE;

	m_pImageListSmall = NULL;
    m_iSelectedItem = -1;
    
    m_ridRefreshTime = RR_LOW;
    CSysView95App *pApp = (CSysView95App*) AfxGetApp();
    if (pApp)
    {
        BOOL bCreated;
        CString strTask = _T("Tasks");
        CString strRefresh = _T("REFRESH");
        if (m_ridRefreshTime.Connect(pApp->GetRegistryManager(),
            strTask, strRefresh, bCreated))
        {
//            if (bCreated) m_ridRefreshTime = RR_LOW;
        }
    }
}

CApplicationList::~CApplicationList()
{
    m_ridRefreshTime.Update();
    SAFE_DELETE_ARR(m_pListField)
    SAFE_DELETE(m_pImageListSmall);
}


BEGIN_MESSAGE_MAP(CApplicationList, CListCtrl)
	//{{AFX_MSG_MAP(CApplicationList)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SWITCHTO, OnSwitchto)
	ON_UPDATE_COMMAND_UI(ID_SWITCHTO, OnUpdateSwitchto)
	ON_COMMAND(ID_KILL_TASK, OnKillTask)
	ON_UPDATE_COMMAND_UI(ID_KILL_TASK, OnUpdateKillTask)
	ON_COMMAND(ID_SWITCHTO_PROCESS, OnSwitchtoProcess)
	ON_UPDATE_COMMAND_UI(ID_SWITCHTO_PROCESS, OnUpdateSwitchtoProcess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplicationList message handlers

BOOL CApplicationList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CApplicationList::Create(const CRect &rect, CWnd* pWnd, UINT uiId)
{
    BOOL bRet = CListCtrl::Create(LVS_REPORT, rect, pWnd, uiId);
    bRet = ShowWindow(SW_SHOW);

    SetRefreshTime(m_ridRefreshTime.GetVal());

    return bRet;
}

void CApplicationList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CListCtrl::OnShowWindow(bShow, nStatus);
	
	if (bShow == TRUE)
    {
        if (m_bColumnsCreated == FALSE)
        {
            ICONINFO iconInfo;

            GetIconInfo((HICON)IDI_ASTERISK, &iconInfo);
            DWORD dwErr = GetLastError();
            m_hDefaultIcon = CreateIconIndirect(&iconInfo);
            dwErr = GetLastError();
            if (m_hDefaultIcon == NULL)
                m_hDefaultIcon = AfxGetApp()->LoadIcon(IDI_DEFAULT);

            m_toolHelp.StartApplicationWalk(m_taskEntryList, 
                MAX_APPS, m_hDefaultIcon);

            CreateColumns();
            m_bColumnsCreated = TRUE;

            m_pImageListSmall   = new CImageList;

	        if (m_pImageListSmall != NULL)
                m_pImageListSmall->Create(16, 16, TRUE, 0, MAX_APPS);

            SetImageList(m_pImageListSmall, LVSIL_SMALL);
        }
//        UpdateList();
    }	
}

BOOL CApplicationList::CreateColumns()
{
    if (m_bColumnsCreated == FALSE)
    {
        CPaintDC dc(this);
        CRect rect;
        GetWindowRect(&rect);
        CSize size = dc.GetTextExtent(szStatusRuns, strlen(szStatusRuns));
        m_iStrlenStatusRuns = size.cx;
	    InsertColumn(0, szApplication, LVCFMT_LEFT, 
            rect.Width()-(size.cx+20), 0);

        LVCOLUMN    col;
        col.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
        col.pszText = (char*) szStatus;
        col.iSubItem = 1;
        col.iOrder = 1;
        col.fmt = LVCFMT_LEFT;
        col.cx = size.cx;
	    InsertColumn(1, &col);

//	    InsertColumn(1, szStatus, LVCFMT_LEFT, size.cx, 1);
    }
    return TRUE;
}

BOOL CApplicationList::UpdateList()
{
    LVITEM  lvitem;
    int     iSubItem;
    int     iActualItem = -1;
    int     iCount;
    DWORD   dwItemData;
    int     iIndex;
    static int cntOldItems = 0;
    BOOL    bMustInsertNewApp = FALSE;
    BOOL    bPosChanged = FALSE;

    m_toolHelp.GetApplicationList(MAX_APPS, m_taskEntryList, m_dwExistApps);

    iCount = GetItemCount();
    if (iCount > cntOldItems)
    {
        SAFE_DELETE_ARR(m_pListField);
        m_pListField = new BYTE[iCount];
        cntOldItems = iCount;
    }

    memset(m_pListField, 0, iCount);
	for (int iItem = 0; iItem < (int)m_dwExistApps; iItem++)  // insert the items and subitems into the list view.
    {
        if (iItem < iCount)
        {
            if ((dwItemData = GetItemData(iItem)) != 0)
            {
                if (dwItemData == (DWORD)m_taskEntryList[iItem].hWnd)
                {
                    // Prozess schon eingetragen
                    m_pListField[iItem] |= F_EXIST;
                    m_taskEntryList[iItem].dwFlags |= F_EXIST;
                    continue;
                }
                else
                {
                    // nach Eintrag suchen
                    LVFINDINFO info;
                    info.flags = LVFI_PARAM;
                    info.lParam = (LONG)m_taskEntryList[iItem].hWnd;
                    if ((iIndex = FindItem(&info, -1)) != -1)
                    {
                        // Prozess schon eingetragen
                        m_pListField[iIndex] |= F_EXIST;
                        m_taskEntryList[iItem].dwFlags |= F_POS_CHANGED;
                        bPosChanged = TRUE;
                        continue;
                    }
                }
            }
        }
        bMustInsertNewApp = TRUE;
    }

    // alle überfüssigen Einträge löschen
    BOOL  bDelete = FALSE;
    for (iItem = 0; iItem < iCount; iItem++)
    {
        if (m_pListField[iItem] != F_EXIST)
        {
            DeleteItem(iItem);
            bDelete = TRUE;
        }
    }

    BOOL bMustCreateNew = FALSE;
    if (bMustInsertNewApp == TRUE || bDelete == TRUE || bPosChanged == TRUE)
    {
        DeleteAllItems();
        UpdateImageList();
        bMustCreateNew = TRUE;
    }

    // neue Items eintragen
    if (bMustCreateNew == TRUE || m_bMustRefreshItems == TRUE)
    {
        for (iItem = 0; iItem < (int)m_dwExistApps; iItem++)  // insert the items and subitems into the list view.
        {
            if (bMustCreateNew == FALSE)
            {
                m_taskEntryList[iItem].dwFlags = 0;  // Eintrag fuer nächstes Mal löschen

                RefreshItem(iItem, &m_taskEntryList[iItem]);
                continue;
            }
		    for (iSubItem = 0; iSubItem < 2; iSubItem++)
		    {
                if (iSubItem == 0)
                {
			        // calculate the main and sub-item strings for the current item
    			    lvitem.iItem = iItem;
                    lvitem.iImage = iItem;
			        lvitem.pszText= m_taskEntryList[iItem].strTitle;
                    lvitem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    			    lvitem.iSubItem = 0;
                    lvitem.lParam = (LONG)m_taskEntryList[iItem].hWnd;
			        iActualItem = InsertItem(&lvitem); // insert new item
                }
                else
                {
			        // calculate the main and sub-item strings for the current item
                    if (m_taskEntryList[iItem].bResponding == TRUE)
			            lvitem.pszText = (char*)szStatusRuns; 
                    else
			            lvitem.pszText = (char*)szStatusHangs; 

    			    lvitem.iSubItem = iSubItem;
                    lvitem.mask = LVIF_TEXT; //| LVIF_PARAM;
    			    lvitem.iItem = iActualItem;
                    lvitem.lParam = (LONG)m_taskEntryList[iItem].bResponding;
				    SetItem(&lvitem); // modify existing item (the sub-item text)
                }
		    }
        }  
    }

    return TRUE;
}

inline void CApplicationList::RefreshItem(int iItem, APPLICATION *pApp)
{
    LVITEM item;
    char cBuf[MAX_PATH];

    // get item text
    item.mask = LVIF_TEXT;
    item.cchTextMax = sizeof(cBuf);
    item.pszText = cBuf;
    GetItem(&item);
    if (strcmp(cBuf, pApp->strTitle) != 0)
    {
        item.mask = LVIF_TEXT;
        item.pszText = pApp->strTitle;
        item.iItem = iItem;
        item.iSubItem = 0;
        SetItem(&item);
    }

    // subitem: Achtung: Subitems können keine Parameter haben???
    item.mask = LVIF_TEXT;
    item.cchTextMax = sizeof(cBuf);
    item.pszText = cBuf;
    item.iItem = iItem;
    item.iSubItem = 1;
    GetItem(&item);
    BOOL bStatus = StrToStatus(cBuf);
    if (bStatus != pApp->bResponding)
    {
        if (pApp->bResponding == TRUE)
	        item.pszText = (char*)szStatusRuns; 
        else
	        item.pszText = (char*)szStatusHangs; 

        item.iSubItem = 1;
        item.mask = LVIF_TEXT;
        item.iItem = iItem;
        SetItem(&item); // modify existing item (the sub-item text)
    }
}

void CApplicationList::OnTimer(UINT nIDEvent) 
{
#if 1
	if (nIDEvent == REFRESH_APPLIST_TIMER)
        UpdateList();
#endif
//	CListCtrl::OnTimer(nIDEvent);
}


void CApplicationList::UpdateImageList(void)
{
    int iApp;
    
    m_pImageListSmall->DeleteImageList();

    m_pImageListSmall->Create(16, 16, TRUE, 0, MAX_APPS);
    SetImageList(m_pImageListSmall, LVSIL_SMALL);

    for (iApp = 0; iApp < (int)m_dwExistApps; iApp++)  // insert the items and subitems into the list view.
    {
        // get Icon
        HICON hSmallIcon = m_taskEntryList[iApp].hSmallIcon;
        m_pImageListSmall->Add(hSmallIcon);
    }
}

BOOL CApplicationList::GetSelectedItem(int &iItem)
{
    POSITION pos = GetFirstSelectedItemPosition();
    if (pos == NULL)
        return FALSE;
    else
    {   
        iItem = GetNextSelectedItem(pos);
    }

    return TRUE;
}

void CApplicationList::OnDestroy() 
{
    m_toolHelp.StopApplicationWalk();

	CListCtrl::OnDestroy();	
}

void CApplicationList::StartApplicationWalk()
{
    m_toolHelp.StartApplicationWalk(m_taskEntryList, MAX_APPS, NULL);
}

void CApplicationList::StopApplicationWalk()
{
    m_toolHelp.StopApplicationWalk();
}

inline BOOL CApplicationList::StrToStatus(char *strStatus)
{
    if (strcmp(szStatusRuns, strStatus) == 0)
        return TRUE;
    else
        return FALSE;
}

inline const char* CApplicationList::StatusToStr(BOOL bStatus)
{
    if (bStatus == TRUE)
        return szStatusRuns;
    else
        return szStatusHangs;
}

BOOL CApplicationList::EndTask()
{
    int iSelItem;
    if (GetSelectedItem(iSelItem) == TRUE)
    {
        m_toolHelp.TerminateApp(0,3000, m_taskEntryList[iSelItem].hWnd);
        return TRUE;
    }

    return FALSE;
}

BOOL CApplicationList::ActivateTask()
{
    int iSelItem;
    if (GetSelectedItem(iSelItem) == TRUE)
    {
        m_toolHelp.ActivateApp(m_taskEntryList[iSelItem].hWnd);
        return TRUE;
    }

    return FALSE;
}

void CApplicationList::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
    if (nType == SIZE_RESTORED)
    {

        CHeaderCtrl* pHCtrl = GetHeaderCtrl();
        if (pHCtrl)
        {
            HDITEM  item;
            item.cxy = cx-(m_iStrlenStatusRuns+10);
            item.mask = HDI_WIDTH;
            pHCtrl->SetItem(0, &item);
        }

    }
}


void CApplicationList::OnRButtonDown(UINT nFlags, CPoint point) 
{
    nFlags;  // to avoid warning

    CMenu menu;

    if (GetSelectedItem(m_iSelectedItem) == TRUE)
    {
        menu.LoadMenu(IDR_TASKMENU);

        CMenu *pMenu = menu.GetSubMenu(0);

        if (pMenu)
        {
            ClientToScreen(&point);
            pMenu->TrackPopupMenu(/*TPM_LEFTALIGN|TPM_RIGHTBUTTON*/0, 
                point.x, point.y, this, NULL);
        }
    }	
	
//	CListCtrl::OnRButtonDown(nFlags, point);
}

void CApplicationList::OnSwitchto() 
{
    ActivateTask();	
}

void CApplicationList::OnUpdateSwitchto(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}

void CApplicationList::OnKillTask() 
{
	EndTask();
}

void CApplicationList::OnUpdateKillTask(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}

void CApplicationList::OnSwitchtoProcess() 
{
    if (GetSelectedItem(m_iSelectedItem) == TRUE)
    {
        CWnd* pWnd = GetParent();
        if (pWnd)
        {
            pWnd->PostMessage(MSG_SWITCH_TO_PROCESS,
                (WPARAM)m_taskEntryList[m_iSelectedItem].hWnd, 0);
        }
    }    	
}

void CApplicationList::OnUpdateSwitchtoProcess(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}

DWORD CApplicationList::GetRefreshTime()
{
    return m_ridRefreshTime.GetVal();
}

BOOL CApplicationList::SetRefreshTime(DWORD dwTime)
{
    if (m_bTimerCreated == TRUE)
    {
        KillTimer(REFRESH_APPLIST_TIMER);
        m_bTimerCreated = FALSE;
    }

    m_ridRefreshTime = dwTime;
    if (m_ridRefreshTime.GetVal() != RR_MANUAL)
    {
        m_bTimerCreated = (SetTimer(REFRESH_APPLIST_TIMER, m_ridRefreshTime.GetVal(), NULL) != 0);
        return m_bTimerCreated;
    }
    else
        return TRUE;
}

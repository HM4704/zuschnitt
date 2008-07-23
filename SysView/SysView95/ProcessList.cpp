// ProcessList.cpp: implementation of the CProcessList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysView95.h"
#include "Wintop.h"
#include "MemUsageDlg.h"
#include "ProcessList.h"

#ifdef _DEBUG
//#define _OUTPUT
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcessList::CProcessList()
{
    m_pProcList     = new PLPROCENTRY[MAX_PROCESSES];
    for (int i = 0; i < MAX_PROCESSES; i++)
        m_pProcList[i].dwFlags = 0;
    m_pProcEntries  = new PROCESSENTRY32[MAX_PROCESSES];

    m_dwMaxProcs    = MAX_PROCESSES;
    m_dwExistProcs  = 0;

    m_bColumnsCreated = FALSE;
    m_pListField = NULL;

    m_iSelectedItem = -1;
    m_bMustRefreshItems = TRUE;
    m_bTimerCreated = FALSE;

    SetupRegistryValues();

    m_pStatusBar = NULL;

    // init Wintop
    if ((m_hWintop = WintopOpenDevice()) != INVALID_HANDLE_VALUE)
    {
        WintopInit(m_hWintop);
        m_pCuEntries = WintopInitCuBuffer(MAX_PROCESSES);
    }
    else
    {
        //??
    }
}

CProcessList::~CProcessList()
{
    SAFE_DELETE_ARR(m_pProcList)
    SAFE_DELETE_ARR(m_pProcEntries)    
    SAFE_DELETE_ARR(m_pCuEntries)
    SAFE_DELETE_ARR(m_pListField)
}

BEGIN_MESSAGE_MAP(CProcessList, CListCtrl)
	//{{AFX_MSG_MAP(CProcessList)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TERMINATE, OnTerminate)
	ON_UPDATE_COMMAND_UI(ID_TERMINATE, OnUpdateTerminate)
	ON_COMMAND(ID_DEBUG, OnDebug)
	ON_UPDATE_COMMAND_UI(ID_DEBUG, OnUpdateDebug)
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_SWITCH_TO_PROCESS, OnSwitchToProcess)
	ON_COMMAND(ID_PROC_MEMUSAGE, OnProcMemusage)
	ON_UPDATE_COMMAND_UI(ID_PROC_MEMUSAGE, OnUpdateProcMemusage)
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE(ID_REALTIME, ID_IDLE, OnChangePriority)
END_MESSAGE_MAP()


BOOL CProcessList::SetupRegistryValues()
{
    BOOL bCreated;
    CSysView95App *pApp = (CSysView95App*) AfxGetApp();

    m_dwCntColumns = 1;     // 1 == Prozessname

    if (pApp)
    {
        // default
        m_dwFieldsToShow = pid;
        m_dwCntColumns++;
        m_dwFieldsToShow |= cntThreads;
        m_dwCntColumns++;
        m_dwFieldsToShow |= prioClassBase;
        m_dwCntColumns++;
        if (m_riFieldsToShow.Connect(pApp->GetRegistryManager(), 
            "Prozesse", "DETAILS", bCreated) == TRUE)
        {
            if (bCreated == FALSE)
            {
                // aus Registry ausgelesen
                DWORD   dwBitMask = 0x00000001;
                for (int i=0; i<32; i++)
                {
                    if (m_riFieldsToShow.GetVal() & dwBitMask)
                        m_dwCntColumns++;
                    dwBitMask <<= 1;
                }
                m_dwFieldsToShow = m_riFieldsToShow.GetVal();
            }
            else
                m_riFieldsToShow = m_dwFieldsToShow;
        }
        else
            m_riFieldsToShow = m_dwFieldsToShow;

        m_ridRefreshTime = RR_LOW;
        m_ridRefreshTime.Connect(pApp->GetRegistryManager(),
            "Prozesse", "REFRESH", bCreated);
    }
    return TRUE;
}

BOOL CProcessList::Create(const CRect &rect, CWnd* pWnd, UINT uiId)
{
    BOOL bRet = CListCtrl::Create(LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL, rect, pWnd, uiId);
    bRet = ShowWindow(SW_SHOW);

    SetRefreshTime(m_ridRefreshTime.GetVal());

    return bRet;
}

void CProcessList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CListCtrl::OnShowWindow(bShow, nStatus);
	
	if (bShow == TRUE)
    {
        if (m_bColumnsCreated == FALSE)
        {
            CreateColumns();
            m_bColumnsCreated = TRUE;

            // Abfrage der Registry auf msdev.exe
            m_bMsdevInstalled = IsDebuggerInstalled();
        }

        UpdateList();
    }
}

BOOL CProcessList::UpdateList()
{
    BOOL    bRet;
    LVITEM  lvitem;
    int     iSubItem;
    int     iActualItem = -1;
    int     iCount;
    DWORD   dwItemData;
    int     iIndex;
    static int cntOldItems = 0;
    BOOL    bMustInsertNewProc = FALSE;

	bRet = m_toolHelp.GetProcessList(m_dwMaxProcs, 
                m_pProcEntries, m_dwExistProcs, /*RQU_HEAPSIZE*/0);  
    while (m_dwExistProcs > m_dwMaxProcs)
    {
        m_dwMaxProcs = m_dwExistProcs+5;

        SAFE_DELETE_ARR(m_pProcEntries);
        SAFE_DELETE_ARR(m_pProcList);
        SAFE_DELETE_ARR(m_pCuEntries);

        m_pProcEntries = new PROCESSENTRY32[m_dwMaxProcs];
        m_pProcList = new PLPROCENTRY[m_dwMaxProcs];
        m_pCuEntries = WintopInitCuBuffer(MAX_PROCESSES);

        for (int i = 0; i < (int)m_dwMaxProcs; i++)
            m_pProcList[i].dwFlags = 0;

	    bRet = m_toolHelp.GetProcessList(m_dwMaxProcs, 
            (PROCESSENTRY32*)m_pProcEntries, m_dwExistProcs, 0/*RQU_HEAPSIZE*/);

    }
    
    iCount = GetItemCount();
    if (iCount > cntOldItems)
    {
        SAFE_DELETE_ARR(m_pListField);
        m_pListField = new BYTE[iCount];
        cntOldItems = iCount;
    }
    memset(m_pListField, 0, iCount);

	for (int iItem = 0; iItem < (int)m_dwExistProcs; iItem++)  // insert the items and subitems into the list view.
    {
        if (iItem < iCount)
        {
            if ((dwItemData = GetItemData(iItem)) != 0)
            {
                if (dwItemData == m_pProcEntries[iItem].th32ProcessID)
                {
                    // Prozess schon eingetragen
                    m_pListField[iItem] |= F_EXIST;
                    m_pProcEntries[iItem].dwFlags |= F_EXIST;
                    continue;
                }
                else
                {
                    // nach Eintrag suchen
                    LVFINDINFO info;
                    info.flags = LVFI_PARAM;
                    info.lParam = m_pProcEntries[iItem].th32ProcessID;
                    if ((iIndex = FindItem(&info, -1)) != -1)
                    {
                        // Prozess schon eingetragen
                        m_pListField[iIndex] |= F_EXIST;
                        m_pProcEntries[iItem].dwFlags |= F_EXIST;
                        continue;
                    }
                }
            }
        }
        bMustInsertNewProc = TRUE;
    }


    // alle überfüssigen Einträge löschen
    for (iItem = 0; iItem < iCount; iItem++)
    {
        if (m_pListField[iItem] != F_EXIST)
        {
            DeleteItem(iItem);
            bMustInsertNewProc = TRUE;
        }
    }

    WintopGetProcessTimes(m_hWintop, m_pCuEntries, m_dwMaxProcs, m_dwExistProcs, bMustInsertNewProc);
    ConvertProcEntriesToListEntries(m_pProcList, m_pProcEntries, m_pCuEntries, m_dwExistProcs);

    // Refresh
//    RedrawItems(0, iCount);
//    UpdateWindow();

	// neue Items eintragen
    if (bMustInsertNewProc == TRUE || m_bMustRefreshItems == TRUE)
    {
        for (iItem = 0; iItem < (int)m_dwExistProcs; iItem++)  // insert the items and subitems into the list view.
        {
            if (m_pProcEntries[iItem].dwFlags == F_EXIST)
            {
                m_pProcEntries[iItem].dwFlags = 0;  // Eintrag fuer nächstes Mal löschen

                RefreshItem(iItem);
                continue;
            }
            m_ColsList.BeginFromEnd();
		    for (iSubItem = process; iSubItem < (int)m_ColsList.GetColsUsed()+1; iSubItem++)
		    {
                if (ListItems(iSubItem) == process)
                {
			        // calculate the main and sub-item strings for the current item
    			    lvitem.iItem = iItem;
			        lvitem.pszText= m_pProcList[iItem].szExeFile; // + m_ColsList.GetNext();
                    lvitem.mask = LVIF_TEXT | LVIF_PARAM;
    			    lvitem.iSubItem = 0;
                    lvitem.lParam = m_pProcEntries[iItem].th32ProcessID;
			        iActualItem = InsertItem(&lvitem); // insert new item
                }
                else
                {
			        // calculate the main and sub-item strings for the current item
			        lvitem.pszText= m_pProcList[iItem].szExeFile + m_ColsList.GetNext();
    			    lvitem.iSubItem = iSubItem;
                    lvitem.mask = LVIF_TEXT;
    			    lvitem.iItem = iActualItem;
				    SetItem(&lvitem); // modify existing item (the sub-item text)
                }
			    

		    }
        }  
    }
    return bRet;
}

BOOL CProcessList::CreateColumns()
{
    int   nSubItem = 0;

    m_ColsList.Reset();
  
    if (m_bColumnsCreated == FALSE)
    {
        COLUMNLISTENTRY colEntry;
        GetColumnProperties(process, &colEntry);
	    InsertColumn(0, colEntry.pstText, LVCFMT_LEFT, colEntry.cx, nSubItem);
    }

    if (m_dwFieldsToShow & pid)
    {
        AddColumn(pid);
    }
    if (m_dwFieldsToShow & cntThreads)
    {
        AddColumn(cntThreads);
    }
    if (m_dwFieldsToShow & cntUsage)
    {
        AddColumn(cntUsage);
    }
    if (m_dwFieldsToShow & moduleId)
    {
        AddColumn(moduleId);
    }
    if (m_dwFieldsToShow & defaultHeapId)
    {
        AddColumn(defaultHeapId);
    }
    if (m_dwFieldsToShow & parentPid)
    {
        AddColumn(parentPid);
    }
    if (m_dwFieldsToShow & prioClassBase)
    {
        AddColumn(prioClassBase);
    }
    if (m_dwFieldsToShow & cpuTime)
    {
        AddColumn(cpuTime);
    }
    if (m_dwFieldsToShow & cpuPercent)
    {
        AddColumn(cpuPercent);
    }
    if (m_dwFieldsToShow & flags)
    {
        AddColumn(flags);
    }    
    return TRUE;
}

void CProcessList::ConvertProcEntriesToListEntries(PLPROCENTRY *pPL, 
                    PROCESSENTRY32 *pPE, CPU_USAGE* pCuBuf, DWORD dwEntries)
{
    for (DWORD i = 0; i < dwEntries; i++)
    {
        // copy only file-name
        char* pFileName = strrchr(pPE[i].szExeFile, '\\')+1;
        strcpy(pPL[i].szExeFile, pFileName);
        itoa(pPE[i].cntThreads, pPL[i].szCntThreads, 10);
        itoa(pPE[i].dwFlags, pPL[i].szFlags, 16);
        itoa(pPE[i].cntUsage, pPL[i].szCntUsage, 10);
        itoa(pPE[i].th32DefaultHeapID, pPL[i].szDefaultHeapID, 16);
        itoa(pPE[i].th32ModuleID, pPL[i].szModuleID, 16);
        itoa(pPE[i].th32ParentProcessID, pPL[i].szParentProcessID, 16);
        itoa(pPE[i].th32ProcessID, pPL[i].szProcessID, 16);

        switch (pPE[i].pcPriClassBase)
        {
        case PRIO_NORMAL:
            strcpy(pPL[i].szPriClassBase, szNORMAL_PRIORITY_CLASS);
            break;
        case PRIO_HIGH:
            strcpy(pPL[i].szPriClassBase, szHIGH_PRIORITY_CLASS);
            break;
        case PRIO_IDLE:
            strcpy(pPL[i].szPriClassBase, szIDLE_PRIORITY_CLASS);
            break;
#ifdef WIN2000
        case ABOVE_NORMAL_PRIORITY_CLASS:
            strcpy(pPL[i].szPriClassBase, szABOVE_NORMAL_PRIORITY_CLASS);
            break;
        case BELOW_NORMAL_PRIORITY_CLASS:
            strcpy(pPL[i].szPriClassBase, szBELOW_NORMAL_PRIORITY_CLASS);
            break;
#endif
        case PRIO_REALTIME:
            strcpy(pPL[i].szPriClassBase, szREALTIME_PRIORITY_CLASS);
            break;
        default:
            strcpy(pPL[i].szPriClassBase, szUNKOWN_PRIORITY_CLASS);
            break;
        }

        for (DWORD x = 0; x < dwEntries; x++)
        {
            if (pPE[i].th32ProcessID == pCuBuf[x].dwProcessId)
            {
                // CPU Percent
                itoa(pCuBuf[x].dwCpuPercent, pPL[i].szCPUPercent, 10);

                // Run Time
                int iSec = (int)((double)pCuBuf[x].dwTickCountPrev/(double)1000);
                int iMin = (int)((double)iSec/(double)60);
                iSec = iSec%60;
                sprintf(pPL[i].szCPUTime, "%d:%02d", iMin, iSec);
            }
        }

    }
}

void CProcessList::OnTimer(UINT nIDEvent) 
{

    if (nIDEvent == 10)
    {
#ifdef _OUTPUT
    char cBuf[200];
    sprintf(cBuf, "OnTimer fuer UpdateList()\n");
    OutputDebugString(cBuf);    
#endif
        UpdateList();
        if (m_pStatusBar)
            UpdateStatusBar();
    }
//	CListCtrl::OnTimer(nIDEvent);
}

void CProcessList::OnRButtonDown(UINT nFlags, CPoint point) 
{
    nFlags;// to avoid warning

    CMenu menu;
    if (GetSelectedItem(m_iSelectedItem) == TRUE)
    {
        menu.LoadMenu(IDR_PROCMENU);

        CMenu *pMenu = menu.GetSubMenu(0);

        /* check right menu item */
        switch (m_pProcEntries[m_iSelectedItem].pcPriClassBase)
        {
        case PRIO_NORMAL:
            pMenu->CheckMenuItem(ID_NORMAL, MF_BYCOMMAND|MF_CHECKED);
            break;
        case PRIO_HIGH:
            pMenu->CheckMenuItem(ID_HIGH, MF_BYCOMMAND|MF_CHECKED);
            break;
        case PRIO_IDLE:
            pMenu->CheckMenuItem(ID_IDLE, MF_BYCOMMAND|MF_CHECKED);
            break;
        case PRIO_REALTIME:
            pMenu->CheckMenuItem(ID_REALTIME, MF_BYCOMMAND|MF_CHECKED);
            break;
        default:
            break;
        }

        if (pMenu)
        {
            ClientToScreen(&point);
            pMenu->TrackPopupMenu(/*TPM_LEFTALIGN|TPM_RIGHTBUTTON*/0, 
                point.x, point.y, this, NULL);
        }
    }	
//	CListCtrl::OnRButtonDown(nFlags, point);
}

#define TERMINATE_TIMEOUT   1000        // 1 sec
void CProcessList::OnTerminate() 
{    
    m_toolHelp.TerminateApp(m_pProcEntries[m_iSelectedItem].th32ProcessID
        , TERMINATE_TIMEOUT);	

    UpdateList();
}

void CProcessList::OnUpdateTerminate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}

void CProcessList::OnChangePriority(UINT nId)
{
    DWORD dwPriority, dwDesiredPrio;
    dwPriority = m_pProcEntries[m_iSelectedItem].pcPriClassBase;

    switch (nId)
    {
    case ID_REALTIME: 
        dwDesiredPrio = REALTIME_PRIORITY_CLASS;
        break;
    case ID_HIGH:
        dwDesiredPrio = HIGH_PRIORITY_CLASS;
        break;
    case ID_NORMAL:
        dwDesiredPrio = NORMAL_PRIORITY_CLASS;
        break;
    case ID_IDLE:
        dwDesiredPrio = IDLE_PRIORITY_CLASS;
        break;
    default:
        return;
    }

    if (dwDesiredPrio != dwPriority)
    {
        m_toolHelp.SetProcessPriority(
            m_pProcEntries[m_iSelectedItem].th32ProcessID
            , dwDesiredPrio);

        UpdateList();
    }
}

BOOL CProcessList::GetSelectedItem(int &iItem)
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

inline void CProcessList::RefreshItem(int iItem)
{
    int iSubItem;
    int iOffset;
    LVITEM lvitem;
    char cBuf[200];

    m_ColsList.BeginFromEnd();
    for (iSubItem = 1; iSubItem < (int)m_ColsList.GetColsUsed()+1; iSubItem++)
    {
        if (m_ColsList.GetNextIsChangeable(&iOffset) == FALSE)
            continue;

    	lvitem.iSubItem = iSubItem;
    	lvitem.iItem = iItem;
        lvitem.mask = LVIF_TEXT;
        lvitem.pszText = cBuf;
        lvitem.cchTextMax = 200;
        GetItem(&lvitem);
        if (!strcmp(lvitem.pszText, m_pProcList[iItem].szExeFile + iOffset))
            continue;

		// calculate the main and sub-item strings for the current item
		lvitem.pszText= m_pProcList[iItem].szExeFile + iOffset;
		SetItem(&lvitem); // modify existing item (the sub-item text)
    }
}

DWORD CProcessList::GetRefreshTime()
{
    return m_ridRefreshTime.GetVal();
}

BOOL CProcessList::SetRefreshTime(DWORD dwTime)
{
    if (m_bTimerCreated == TRUE)
    {
        KillTimer(REFRESH_TIMER_ID);
        m_bTimerCreated = FALSE;
    }

    m_ridRefreshTime = dwTime;
    if (m_ridRefreshTime.GetVal() != RR_MANUAL)
    {
        m_bTimerCreated = (SetTimer(REFRESH_TIMER_ID, m_ridRefreshTime.GetVal(), NULL) != 0);
        return m_bTimerCreated;
    }
    else
        return TRUE;
}

DWORD CProcessList::GetColumnField()
{
    return m_dwFieldsToShow;
}

BOOL CProcessList::RemoveColumn(DWORD dwBfCol)
{
    int iCol;

    if ((m_dwFieldsToShow & dwBfCol) == 0)
        return FALSE;

    if ((iCol = m_ColsList.GetIndexFromId(dwBfCol)) == -1)
        return FALSE;

    if (DeleteColumn(iCol+1) == FALSE)
        return FALSE;

    m_ColsList.RemoveEntry(iCol);

    m_dwCntColumns--;
    m_dwFieldsToShow &= ~dwBfCol;

    return TRUE;
}

BOOL CProcessList::AddColumn(DWORD dwBfCol)
{
    LVCOLUMN    col;
    COLUMNLISTENTRY colEntry;

    GetColumnProperties(dwBfCol, &colEntry);

    CHeaderCtrl* pHCtrl = GetHeaderCtrl();

    int iSubItem = pHCtrl->GetItemCount();
    col.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
    col.pszText = colEntry.pstText;
    col.iSubItem = iSubItem;
    col.iOrder = iSubItem;
    col.fmt = colEntry.iFmt;
    col.cx = colEntry.cx;
	InsertColumn(iSubItem, &col);

    m_ColsList.InsertAt(iSubItem-1, &colEntry);

    m_dwCntColumns++;
    m_dwFieldsToShow |= dwBfCol;

    RefreshColSubItems(iSubItem, &colEntry);

    return TRUE;
}

BOOL CProcessList::GetColumnProperties(DWORD dwId, COLUMNLISTENTRY *pCol)
{
#define ID_PERCENT      0.7
#define NUMBER_PERCENT     0.55
#define PRIORITY_PERCENT    0.8

    PLPROCENTRY ListEntry;      // fuer Offset-Berechnung
    CDC* pDC;
    static CSize size;

    pCol->dwId = dwId;

    switch (dwId)
    {
    case process:
        pDC = GetDC();
        size = pDC->GetTextExtent("kernel32.dllMM");
        ReleaseDC(pDC);
        pCol->bChangeable = FALSE;
        pCol->iColumnItemOffset = ListEntry.szExeFile-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szProcess;
        pCol->cx = size.cx;
        break;
    case pid:
        pCol->bChangeable = FALSE;
        pCol->iColumnItemOffset = ListEntry.szProcessID-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szPID;
        pCol->cx = (int)((double)size.cx*ID_PERCENT);
        break;
    case cntUsage:
        pCol->bChangeable = TRUE;
        pCol->iColumnItemOffset = ListEntry.szCntUsage-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_CENTER;
        pCol->pstText = (char*)szCntUsage;
        pCol->cx = (int)((float)size.cx*NUMBER_PERCENT);
        break;
    case moduleId:
        pCol->bChangeable = FALSE;
        pCol->iColumnItemOffset = ListEntry.szModuleID-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szModId;
        pCol->cx = (int)((float)size.cx*ID_PERCENT);
        break;
    case defaultHeapId:
        pCol->bChangeable = FALSE;
        pCol->iColumnItemOffset = ListEntry.szDefaultHeapID-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szHeapId;
        pCol->cx = (int)((float)size.cx*ID_PERCENT);
        break;
    case cntThreads:
        pCol->bChangeable = TRUE;
        pCol->iColumnItemOffset = ListEntry.szCntThreads-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_CENTER;
        pCol->pstText = (char*)szCntThreads;
        pCol->cx = (int)((float)size.cx*NUMBER_PERCENT);
        break;
    case parentPid:
        pCol->bChangeable = FALSE;
        pCol->iColumnItemOffset = ListEntry.szParentProcessID-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szParentPid;
        pCol->cx = (int)((float)size.cx*ID_PERCENT);
        break;
    case prioClassBase:
        pCol->bChangeable = TRUE;
        pCol->iColumnItemOffset = ListEntry.szPriClassBase-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szPrioClass;
        pCol->cx = (int)((float)size.cx*PRIORITY_PERCENT);
        break;
    case cpuTime:
        pCol->bChangeable = TRUE;
        pCol->iColumnItemOffset = ListEntry.szCPUTime-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_CENTER;
        pCol->pstText = (char*)szCPUTime;
        pCol->cx = (int)((float)size.cx*PRIORITY_PERCENT);
        break;
    case cpuPercent:
        pCol->bChangeable = TRUE;
        pCol->iColumnItemOffset = ListEntry.szCPUPercent-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_CENTER;
        pCol->pstText = (char*)szCPUPercent;
        pCol->cx = (int)((float)size.cx*NUMBER_PERCENT);
        break;
    case flags:
        pCol->bChangeable = FALSE;
        pCol->iColumnItemOffset = ListEntry.szFlags-ListEntry.szExeFile;
        pCol->iFmt = LVCFMT_LEFT;
        pCol->pstText = (char*)szFlags;
        pCol->cx = (int)((float)size.cx*ID_PERCENT);
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

// Vorsicht: vielleicht concurrent mit UpdateList()!!!
BOOL CProcessList::RefreshColSubItems(int iSubItem, COLUMNLISTENTRY *pCol)
{
    LVITEM lvitem;
    char cBuf[200];

    for (int iItem = 0; iItem < (int)m_dwExistProcs; iItem++)
    {
    	lvitem.iSubItem = iSubItem;
    	lvitem.iItem = iItem;
        lvitem.mask = LVIF_TEXT;
        lvitem.pszText = cBuf;
        lvitem.cchTextMax = 200;
        GetItem(&lvitem);
        if (!strcmp(lvitem.pszText, m_pProcList[iItem].szExeFile + pCol->iColumnItemOffset))
            continue;

		// calculate the main and sub-item strings for the current item
		lvitem.pszText= m_pProcList[iItem].szExeFile + pCol->iColumnItemOffset;
		SetItem(&lvitem); // modify existing item (the sub-item text)
    }

    return TRUE;
}

void CProcessList::OnDebug() 
{
    char strDebug[300];
    char strFormat[100];
    DWORD dwProcId = m_pProcEntries[m_iSelectedItem].th32ProcessID;
    STARTUPINFO startUpInfo;
    PROCESS_INFORMATION processInfo;
    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE;
   	
    strcpy(strDebug, m_strMsdev);
    sprintf(strFormat, " -p %ld", dwProcId); 

    memset(&startUpInfo, 0, sizeof(startUpInfo));
    startUpInfo.cb = sizeof(startUpInfo);
    startUpInfo.dwFlags = 0;

    if ( CreateProcess(strDebug, strFormat, NULL, NULL,
        FALSE, dwFlags, NULL, NULL, &startUpInfo, 
        &processInfo) == FALSE )
    {
        // Error
    }
    
}

void CProcessList::OnUpdateDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMsdevInstalled);	
}

BOOL CProcessList::IsDebuggerInstalled()
{
    DWORD dwLen = MAX_PATH;

    if (m_Reg.GetRegistryValue(HKEY_LOCAL_MACHINE, (char*)szAppPath, 
        "", m_strMsdev, &dwLen ))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void CProcessList::OnProcMemusage() 
{
    DWORD dwPoolId = 0;

    DWORD dwPid = m_pProcEntries[m_iSelectedItem].th32ProcessID;

    // search for Pool-Id
    for (DWORD i=0; i<m_dwExistProcs; i++)
    {
        if (m_pCuEntries[i].dwProcessId == dwPid)
            dwPoolId = m_pCuEntries[i].dwMemPoolId;
    }
    CMemUsageDlg dlg(m_hWintop, m_pProcList[m_iSelectedItem].szExeFile, dwPoolId);
    dlg.DoModal();
}

void CProcessList::OnUpdateProcMemusage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CProcessList::OnDestroy() 
{
	CListCtrl::OnDestroy();
	
    m_riFieldsToShow = m_dwFieldsToShow;
    m_riFieldsToShow.Update();
    m_ridRefreshTime.Update();
}

LRESULT  CProcessList::OnSwitchToProcess(WPARAM wParam, LPARAM lParam)
{
    lParam; //to avoid warning

    DWORD dwProcessId;
    HWND hWnd = (HWND)wParam;
    if (hWnd)
    {
        GetWindowThreadProcessId(hWnd, &dwProcessId);
        SelectProcess(dwProcessId);
    }

    return 0L;
}

BOOL CProcessList::SelectProcess(DWORD dwProcessId)
{
    for (DWORD i=0; i<m_dwExistProcs; i++)
    {
        if (m_pProcEntries[i].th32ProcessID == dwProcessId)
        {
            UINT nState;
            ClearAllSelections();
            EnsureVisible(i, FALSE);
            nState = GetItemState(i, LVIS_SELECTED);
            nState |= LVIS_SELECTED;
            return SetItemState(i, nState, LVIS_SELECTED);
        }
    }
    return TRUE;
}

BOOL CProcessList::ClearAllSelections()
{
    if (GetSelectedCount())
    {
        int iItem;
        POSITION pos = GetFirstSelectedItemPosition();
        if (pos == NULL)
            return FALSE;
        else
        {   
            while (pos)
            {
                iItem = GetNextSelectedItem(pos);
                // deselect
                SetItemState(iItem, 0, LVIS_SELECTED);
            }
        }
    }
    return TRUE;
}

BOOL CProcessList::RegisterStatusBarInfo(CStatusBar *pBar, int iStrId)
{
    BOOL bRet = FALSE;

    m_pStatusBar = pBar;
    m_iStatusIndex = m_pStatusBar->CommandToIndex(iStrId);
    // get strin from resource
    CFrameWnd* pFrm = (CFrameWnd*) AfxGetMainWnd();
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(pFrm->GetSafeHwnd(), GWL_HINSTANCE);
    if (hInstance)
    {
       m_iStatusStrLen = LoadString(hInstance, iStrId, m_strStatusText, MAX_PATH);
       bRet = (BOOL)m_iStatusStrLen;
    }
    else
    {
       strcpy(m_strStatusText, _T("???"));
       m_iStatusStrLen = strlen(m_strStatusText);
    }
    UINT nID, nStyle; int cxWidth ;
    m_pStatusBar->GetPaneInfo(m_iStatusIndex, nID, nStyle, cxWidth); 
    cxWidth += 30;
    m_pStatusBar->SetPaneInfo(m_iStatusIndex, nID, nStyle, cxWidth); 
    return bRet;    
}

void CProcessList::UpdateStatusBar()
{
//    sprintf(&m_strStatusText[m_iStatusStrLen-2], &m_strStatusText[m_iStatusStrLen-2]);
    itoa(m_dwExistProcs, &m_strStatusText[m_iStatusStrLen], 10);
    m_pStatusBar->SetPaneText(m_iStatusIndex, m_strStatusText, TRUE);
}

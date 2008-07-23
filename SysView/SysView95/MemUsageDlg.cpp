// MemUsageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sysview95.h"
#include "Wintop.h"
#include "MemUsageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemUsageDlg dialog


CMemUsageDlg::CMemUsageDlg(HANDLE hDevice, char* strProcName, 
                           DWORD dwPoolId, CWnd* pParent /*=NULL*/)
	: CDialog(CMemUsageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemUsageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_hWintop = hDevice;
    strcpy(m_strProcName, strProcName);
    m_dwPoolId = dwPoolId;
}


void CMemUsageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemUsageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemUsageDlg, CDialog)
	//{{AFX_MSG_MAP(CMemUsageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemUsageDlg message handlers

BOOL CMemUsageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    // set window title
    char cBuf[MAX_PATH], cTrgtBuf[MAX_PATH];
    GetWindowText(cBuf, sizeof(cBuf));
    sprintf(cTrgtBuf, cBuf, m_strProcName);
    SetWindowText(cTrgtBuf);

    // set dialog contents
    MEM_USAGE mu;
    WintopGetProcessMemoryUsage(m_hWintop, m_dwPoolId, &mu);
    SetMemUsage(&mu);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMemUsageDlg::SetMemUsage(MEM_USAGE *pMemUsage)
{
    char cBuf[100];

    // Code allocated
    CWnd *pWnd = GetDlgItem(IDC_CODE_ALLOCATED);
    if (pWnd)
    {
        sprintf(cBuf, "%dK", pMemUsage->dwCodeAllocated*4);
        pWnd->SetWindowText(cBuf);
    }

    // Code in memory
    pWnd = GetDlgItem(IDC_CODE_IN_MEM);
    if (pWnd)
    {
        sprintf(cBuf, "%dK", pMemUsage->dwCodeInMemory*4);
        pWnd->SetWindowText(cBuf);
    }

    // Code in use
    pWnd = GetDlgItem(IDC_CODE_IN_USE);
    if (pWnd)
    {
        sprintf(cBuf, "%dK", pMemUsage->dwCodeXXX*4);
        pWnd->SetWindowText(cBuf);
    }

    // Data allocated
    pWnd = GetDlgItem(IDC_DATA_ALLOCATED);
    if (pWnd)
    {
        sprintf(cBuf, "%dK", pMemUsage->dwDataAllocated*4);
        pWnd->SetWindowText(cBuf);
    }
    // Data in memory
    pWnd = GetDlgItem(IDC_DATA_IN_MEM);
    if (pWnd)
    {
        sprintf(cBuf, "%dK", pMemUsage->dwDataInMemory*4);
        pWnd->SetWindowText(cBuf);
    }
    // Data in use
    pWnd = GetDlgItem(IDC_DATA_IN_USE);
    if (pWnd)
    {
        sprintf(cBuf, "%dK", pMemUsage->dwDataXXX*4);
        pWnd->SetWindowText(cBuf);
    }

    pWnd = GetDlgItem(IDC_MEM_USAGE_TABLES_NP);
    if (pWnd)
    {
        if (pMemUsage->dwPageTablesNotPresent)
        {
                pWnd->GetWindowText(cBuf, sizeof(cBuf));
                sprintf(cBuf, cBuf, pMemUsage->dwPageTablesNotPresent,
                    pMemUsage->dwPageTablesNotPresent*4);
                pWnd->SetWindowText(cBuf);
        }
        else
            pWnd->ShowWindow(SW_HIDE);
    }

    return TRUE;
}

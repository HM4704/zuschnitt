// SysIntegrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sysview95.h"
#include "SysIntegrationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysIntegrationDlg dialog
CSysIntegrationDlg::CSysIntegrationDlg(CWnd* pParent /*= NULL*/)
	: CDialog(CSysIntegrationDlg::IDD, pParent)
{
    m_dwIntegration = 0;
}

CSysIntegrationDlg::CSysIntegrationDlg(DWORD dwIntegrationFlags, CWnd* pParent /*= NULL*/)
	: CDialog(CSysIntegrationDlg::IDD, pParent)
{
    m_dwIntegration = dwIntegrationFlags;
}

void CSysIntegrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysIntegrationDlg)
	DDX_Control(pDX, IDC_SYS_INT_MENU, m_ctrlIntMenu);
	DDX_Control(pDX, IDC_SYS_INT_AUTO, m_ctrlIntAuto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysIntegrationDlg, CDialog)
	//{{AFX_MSG_MAP(CSysIntegrationDlg)
	ON_BN_CLICKED(IDC_SYS_INT_MENU, OnSysIntMenu)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysIntegrationDlg message handlers

BOOL CSysIntegrationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ctrlIntMenu.SetCheck(m_dwIntegration & SYS_INTEGRATION_MENU);
    m_ctrlIntAuto.SetCheck(m_dwIntegration & SYS_INTEGRATION_AUTO);

    EnableControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSysIntegrationDlg::EnableControls()
{
    if (m_ctrlIntMenu.GetCheck())
    {
        m_ctrlIntAuto.EnableWindow(TRUE);
    }
    else
    {
        m_ctrlIntAuto.SetCheck(0);
        m_ctrlIntAuto.EnableWindow(FALSE);
    }

    return TRUE;
}


void CSysIntegrationDlg::OnSysIntMenu() 
{
	EnableControls();
}

void CSysIntegrationDlg::OnOK() 
{
    m_dwIntegration = 0;
	if (m_ctrlIntMenu.GetCheck())
        m_dwIntegration |= SYS_INTEGRATION_MENU;

	if (m_ctrlIntAuto.GetCheck())
        m_dwIntegration |= SYS_INTEGRATION_AUTO;

	CDialog::OnOK();
}

BOOL CSysIntegrationDlg::GetSettings(DWORD &dwIntegration)
{
    dwIntegration = m_dwIntegration;
    return TRUE;
}

void CSysIntegrationDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
}

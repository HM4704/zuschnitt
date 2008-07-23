// SpawnProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysView95.h"
#include "SpawnProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpawnProcessDlg dialog


CSpawnProcessDlg::CSpawnProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpawnProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpawnProcessDlg)
	m_strProcessName = _T("");
	//}}AFX_DATA_INIT
}


void CSpawnProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpawnProcessDlg)
	DDX_Text(pDX, IDC_PROCESSNAME, m_strProcessName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpawnProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CSpawnProcessDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpawnProcessDlg message handlers

BOOL CSpawnProcessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CWnd *pEdit;
	pEdit = GetDlgItem(IDC_PROCESSNAME);
    if (pEdit)
        pEdit->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpawnProcessDlg::OnBrowse() 
{
static char BASED_CODE szFilter[] = 
    "Programme|*.exe; *.com; *.bat; *.lnk|Alle Dateien|*.*||";
    CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
        szFilter, this);

    if (dlg.DoModal() == IDOK)
    {
        CWnd *pEdit;
	    pEdit = GetDlgItem(IDC_PROCESSNAME);
        if (pEdit)
        {
            pEdit->SetWindowText(dlg.GetPathName());
        }    
    }
}

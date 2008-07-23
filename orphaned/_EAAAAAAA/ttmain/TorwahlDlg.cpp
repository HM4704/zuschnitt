// TorwahlDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "ttmain.h"
#include "resource.h"
#include "TorDoor.h"
#include "TorwahlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTorwahlDlg dialog


CTorwahlDlg::CTorwahlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTorwahlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTorwahlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_iTorZahl = 1;
    m_iGewaehltesTor = 0;
}

CTorwahlDlg::CTorwahlDlg(CWnd* pParent, int iTorZahl)
    : CDialog(CTorwahlDlg::IDD, pParent)
{
    m_iTorZahl = iTorZahl;
    m_iGewaehltesTor = 0;
}

void CTorwahlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTorwahlDlg)
	DDX_Control(pDX, IDC_TORLIST, m_ctrTorList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTorwahlDlg, CDialog)
	//{{AFX_MSG_MAP(CTorwahlDlg)
	ON_LBN_DBLCLK(IDC_TORLIST, OnDblclkTorlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTorwahlDlg message handlers

BOOL CTorwahlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    char cBuf[MAX_PATH], cTorBuf[MAX_PATH];
    char cZahl[10];

    strcpy(cTorBuf, "Tor ");
	
    for (int i = 0;	i < m_iTorZahl; i++)
    {
        itoa(i+1, cZahl, 10);
        strcpy(cBuf, cTorBuf);
        strcat(cBuf, cZahl);
        m_ctrTorList.AddString(cBuf);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CTorwahlDlg::GewaehltesTor()
{
    return m_iGewaehltesTor;
}


void CTorwahlDlg::OnOK() 
{
    if ((m_iGewaehltesTor = m_ctrTorList.GetCurSel()) == LB_ERR)
    {
        return;
    }


	CDialog::OnOK();
}

void CTorwahlDlg::OnDblclkTorlist() 
{
    OnOK();	
}

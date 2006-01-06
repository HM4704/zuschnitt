// HoleProfilEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "HoleProfileDoc.h"
#include "HoleProfilEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoleProfilEditDlg dialog


CHoleProfilEditDlg::CHoleProfilEditDlg(CHoleProfile* pProfile, CWnd* pParent /*=NULL*/)
	: CDialog(CHoleProfilEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHoleProfilEditDlg)
	m_iCount = 0;
	m_iLength = 0;
	//}}AFX_DATA_INIT

    m_pProfile = pProfile;
}


void CHoleProfilEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoleProfilEditDlg)
	DDX_Text(pDX, IDC_BH_PROFILE_ELEM_COUNT, m_iCount);
	DDX_Text(pDX, IDC_BH_PROFILE_ELEM_LENGTH, m_iLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoleProfilEditDlg, CDialog)
	//{{AFX_MSG_MAP(CHoleProfilEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoleProfilEditDlg message handlers


BOOL CHoleProfilEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CopyValues(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHoleProfilEditDlg::CopyValues(BOOL bLoad)
{
    if (bLoad == TRUE)
    {
        // in Dialog-Elemente laden
        m_iCount = m_pProfile->m_dwCount;
        m_iLength = m_pProfile->m_dwLength;

        UpdateData(FALSE);
    }
    else
    {
        // aus Dialog-Elementen holenn
        UpdateData(TRUE);

        m_pProfile->m_dwCount = m_iCount;
        m_pProfile->m_dwLength = m_iLength;
    }
}

void CHoleProfilEditDlg::OnOK() 
{
	CopyValues(FALSE);

	CDialog::OnOK();
}

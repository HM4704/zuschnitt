// ProfilPropDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ProfilPropDlg.h"


// CProfilPropDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CProfilPropDlg, CDialog)

CProfilPropDlg::CProfilPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfilPropDlg::IDD, pParent),
    m_iCount(1)
	, m_iLength(1000)

{

}

CProfilPropDlg::CProfilPropDlg(CProfilProp& profilProp, CWnd* pParent)
	: CDialog(CProfilPropDlg::IDD, pParent)
{
    m_iCount  = profilProp.m_iCount;
    m_iLength = profilProp.m_iLength;
}

CProfilPropDlg::~CProfilPropDlg()
{
}

void CProfilPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BH_PROFILE_ELEM_COUNT, m_iCount);
	DDX_Text(pDX, IDC_BH_PROFILE_ELEM_LENGTH, m_iLength);
}


BEGIN_MESSAGE_MAP(CProfilPropDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CProfilPropDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProfilPropDlg-Meldungshandler

BOOL CProfilPropDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CProfilPropDlg::OnBnClickedOk()
{
    UpdateData(TRUE);

    if ((m_iCount > 0) && (m_iLength > 0))
        OnOK();
    else
        AfxMessageBox("Bitte gültige Werte für Anzahl und Länge eingeben!", MB_OK|MB_ICONSTOP);
}

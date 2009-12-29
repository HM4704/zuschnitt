// PS312DSetEditDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "..\Node.h"
#include "..\ZuschnittManager.h"
#include "..\ttmain.h"
#include "PS312DSetEditDlg.h"


// CPS312DSetEditDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CPS312DSetEditDlg, CDialog)

CPS312DSetEditDlg::CPS312DSetEditDlg(CPS312DSetEditData* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CPS312DSetEditDlg::IDD, pParent)
    , m_bMassAbsolut(false)
    , m_iCount(0)
    , m_iLength(0)
{
    m_pData = pData;
}

CPS312DSetEditDlg::~CPS312DSetEditDlg()
{
}

void CPS312DSetEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PS312_DSET_EDIT_COUNT, m_iCount);
    DDV_MinMaxInt(pDX, m_iCount, 1, 1000000);
    DDX_Text(pDX, IDC_PS312_DEST_EDIT_LENGTH, m_iLength);
}


BEGIN_MESSAGE_MAP(CPS312DSetEditDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CPS312DSetEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPS312DSetEditDlg-Meldungshandler

BOOL CPS312DSetEditDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  hier zusätzliche Initialisierung hinzufügen.

    if (m_pData != NULL)
    {
        CButton* pRB = (CButton*)GetDlgItem(IDC_PS312_DSET_EDIT_MASS_ABS);
        if (pRB)
        {
            if (m_pData->m_bMassAbsolut == true)
            {
                pRB->SetCheck(1);
            }
            else
            {
                pRB = (CButton*)GetDlgItem(IDC_PS312_DSET_EDIT_MASS_KETTE);
                if (pRB)
                    pRB->SetCheck(1);
            }
        }
        m_iCount  = m_pData->m_iCount;
        m_iLength = m_pData->m_iLength;

        UpdateData(FALSE);
    }


    return TRUE;  // return TRUE unless you set the focus to a control
    // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CPS312DSetEditDlg::OnBnClickedOk()
{
    if (m_pData)
    {
        UpdateData(TRUE);

        CButton* pRB = (CButton*)GetDlgItem(IDC_PS312_DSET_EDIT_MASS_ABS);
        if (pRB)
        {
            if (pRB->GetCheck() == 1)
                m_pData->m_bMassAbsolut = true;
            else
                m_pData->m_bMassAbsolut = false;
        }
        m_pData->m_iCount = m_iCount;
        m_pData->m_iLength = m_iLength;
    }

    OnOK();
}

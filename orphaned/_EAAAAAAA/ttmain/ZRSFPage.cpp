// ZRSFPage.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRBasePage.h"
#include "ZRSFPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZRSFPage property page

IMPLEMENT_DYNCREATE(CZRSFPage, CPropertyPage)

CZRSFPage::CZRSFPage() : CZRBasePage("", CZRSFPage::IDD)
{
	//{{AFX_DATA_INIT(CZRSFPage)
	m_strSFQuer = _T("");
	m_strSFSenk = _T("");
	m_strSF2Quer = _T("");
	m_strSF2Senk = _T("");
	m_strSFQuer400 = _T("");
	m_strSFSenk400 = _T("");
	m_strSF2Quer400 = _T("");
	m_strSF2Senk400 = _T("");
	//}}AFX_DATA_INIT
    m_pMan = NULL;
    m_bChanged = FALSE;
}

CZRSFPage::CZRSFPage(char* strTitle, CZuschnittManager* pMan) : CZRBasePage(strTitle, CZRSFPage::IDD)
{
    m_pMan = pMan;
    m_bChanged = FALSE;
}

CZRSFPage::~CZRSFPage()
{
}

void CZRSFPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZRSFPage)
	DDX_Control(pDX, IDC_ZR_SF2_SENK_400, m_ctrlSF2Senk400);
	DDX_Control(pDX, IDC_ZR_SF2_QUER_400, m_ctrlSF2Quer400);
	DDX_Control(pDX, IDC_ZR_SF_SENK_400, m_ctrlSFSenk400);
	DDX_Control(pDX, IDC_ZR_SF_QUER_400, m_ctrlSFQuer400);
	DDX_Control(pDX, IDC_ZR_SF2_SENK, m_ctrlSF2Senk);
	DDX_Control(pDX, IDC_ZR_SF2_QUER, m_ctrlSF2Quer);
	DDX_Control(pDX, IDC_ZR_SF_SENK, m_ctrlSFSenk);
	DDX_Control(pDX, IDC_ZR_SF_QUER, m_ctrlSFQuer);
	DDX_Text(pDX, IDC_ZR_SF_QUER, m_strSFQuer);
	DDX_Text(pDX, IDC_ZR_SF_SENK, m_strSFSenk);
	DDX_Text(pDX, IDC_ZR_SF2_QUER, m_strSF2Quer);
	DDX_Text(pDX, IDC_ZR_SF2_SENK, m_strSF2Senk);
	DDX_Text(pDX, IDC_ZR_SF_QUER_400, m_strSFQuer400);
	DDX_Text(pDX, IDC_ZR_SF_SENK_400, m_strSFSenk400);
	DDX_Text(pDX, IDC_ZR_SF2_QUER_400, m_strSF2Quer400);
	DDX_Text(pDX, IDC_ZR_SF2_SENK_400, m_strSF2Senk400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZRSFPage, CPropertyPage)
	//{{AFX_MSG_MAP(CZRSFPage)
	ON_EN_CHANGE(IDC_ZR_SF_QUER, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF_SENK, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF2_QUER, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF2_SENK, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF_QUER_400, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF_SENK_400, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF2_QUER_400, OnChangePage)
	ON_EN_CHANGE(IDC_ZR_SF2_SENK_400, OnChangePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRSFPage message handlers

void CZRSFPage::GetButtonStates(SetButtonsStruct* pButtons)
{
    // Button-Zustände ermitteln
    // Apply nur, wenn sich im TreeCtrl etwas geändert hat
    pButtons->dwStateButtonApply = WS_VISIBLE;
    // immer sichbar
    pButtons->dwStateButtonCancel = WS_VISIBLE;
    // immer sichbar
    pButtons->dwStateButtonHelp = WS_VISIBLE;
    // Neuer Baum immer unsichtbar
    pButtons->dwStateButtonNew = 0;

    // Apply nur, wenn sich etwas geändert hat
    if (m_bChanged)
        pButtons->dwStateButtonSave = WS_VISIBLE;
    else
        pButtons->dwStateButtonSave = WS_DISABLED;
}


BOOL CZRSFPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CZRBasePage::OnSetActive();
}

void CZRSFPage::OnChangePage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	

    char strTemp[MAX_PATH];
	m_bChanged = TRUE;
    SignalPageChanged();
    if (m_pMan->m_pSF == NULL)
    {
        // Felder neu erzeugen und updaten
        if (m_ctrlSFQuer.GetModify() == TRUE || m_ctrlSFSenk.GetModify() == TRUE
            || m_ctrlSFQuer400.GetModify() == TRUE || m_ctrlSFSenk400.GetModify() == TRUE)
        {
            // neuen Element anlegen
            m_pMan->m_pSF = new CSchiebeFensterZugaben;
            if (m_pMan->m_pSF != NULL)
            {
                if (m_ctrlSFQuer.GetModify())
                {
                    m_ctrlSFQuer.GetLine(0, strTemp);
                    m_pMan->m_pSF->m_iQuer = atoi(strTemp);
                    m_ctrlSFSenk.SetWindowText(_T("0"));
                    m_ctrlSFQuer400.SetWindowText(_T("0"));
                    m_ctrlSFSenk400.SetWindowText(_T("0"));
                }
                if (m_ctrlSFSenk.GetModify())
                {
                    m_ctrlSFSenk.GetLine(0, strTemp);
                    m_pMan->m_pSF->m_iSenkrecht = atoi(strTemp);
                    m_ctrlSFQuer.SetWindowText(_T("0"));
                    m_ctrlSFQuer400.SetWindowText(_T("0"));
                    m_ctrlSFSenk400.SetWindowText(_T("0"));
                }
                if (m_ctrlSFQuer400.GetModify())
                {
                    m_ctrlSFQuer400.GetLine(0, strTemp);
                    m_pMan->m_pSF->m_iQuer_400 = atoi(strTemp);
                    m_ctrlSFSenk.SetWindowText(_T("0"));
                    m_ctrlSFQuer.SetWindowText(_T("0"));
                    m_ctrlSFSenk400.SetWindowText(_T("0"));
                }
                if (m_ctrlSFSenk400.GetModify())
                {
                    m_ctrlSFSenk400.GetLine(0, strTemp);
                    m_pMan->m_pSF->m_iSenkrecht_400 = atoi(strTemp);
                    m_ctrlSFQuer.SetWindowText(_T("0"));
                    m_ctrlSFSenk.SetWindowText(_T("0"));
                    m_ctrlSFQuer400.SetWindowText(_T("0"));
                }
                m_ctrlSFQuer.SetModify(FALSE);
                m_ctrlSFSenk.SetModify(FALSE);
                m_ctrlSFQuer400.SetModify(FALSE);
                m_ctrlSFSenk400.SetModify(FALSE);
            }
        }
    }
    if (m_pMan->m_pSF2 == NULL)
    {
        // Felder neu erzeugen und updaten
        if (m_ctrlSF2Quer.GetModify() == TRUE || m_ctrlSF2Senk.GetModify() == TRUE
            || m_ctrlSF2Quer400.GetModify() == TRUE || m_ctrlSF2Senk400.GetModify() == TRUE)
        {
            // neuen Element anlegen
            m_pMan->m_pSF2 = new CSchiebeFensterZugaben;
            if (m_pMan->m_pSF2 != NULL)
            {
                if (m_ctrlSF2Quer.GetModify())
                {
                    m_ctrlSF2Quer.GetLine(0, strTemp);
                    m_pMan->m_pSF2->m_iQuer = atoi(strTemp);
                    m_ctrlSF2Senk.SetWindowText(_T("0"));
                    m_ctrlSF2Quer400.SetWindowText(_T("0"));
                    m_ctrlSF2Senk400.SetWindowText(_T("0"));
                }
                if (m_ctrlSF2Senk.GetModify())
                {
                    m_ctrlSF2Senk.GetLine(0, strTemp);
                    m_pMan->m_pSF2->m_iSenkrecht = atoi(strTemp);
                    m_ctrlSF2Quer.SetWindowText(_T("0"));
                    m_ctrlSF2Quer400.SetWindowText(_T("0"));
                    m_ctrlSF2Senk400.SetWindowText(_T("0"));
                }
                if (m_ctrlSF2Quer400.GetModify())
                {
                    m_ctrlSF2Quer400.GetLine(0, strTemp);
                    m_pMan->m_pSF2->m_iQuer_400 = atoi(strTemp);
                    m_ctrlSF2Quer.SetWindowText(_T("0"));
                    m_ctrlSF2Senk.SetWindowText(_T("0"));
                    m_ctrlSF2Senk400.SetWindowText(_T("0"));
                }
                if (m_ctrlSF2Senk400.GetModify())
                {
                    m_ctrlSF2Senk400.GetLine(0, strTemp);
                    m_pMan->m_pSF2->m_iSenkrecht_400 = atoi(strTemp);
                    m_ctrlSF2Quer.SetWindowText(_T("0"));
                    m_ctrlSF2Senk.SetWindowText(_T("0"));
                    m_ctrlSF2Quer400.SetWindowText(_T("0"));
                }
                m_ctrlSF2Quer.SetModify(FALSE);
                m_ctrlSF2Senk.SetModify(FALSE);
                m_ctrlSF2Quer400.SetModify(FALSE);
                m_ctrlSF2Senk400.SetModify(FALSE);
            }
        }
    }
    UpdateData(TRUE);  // data retrieved from box

    // in eigenes Objekt übernehmen
    if (m_pMan->m_pSF != NULL)
    {
        m_pMan->m_pSF->m_iQuer = atoi(m_strSFQuer);
        m_pMan->m_pSF->m_iSenkrecht = atoi(m_strSFSenk);
        m_pMan->m_pSF->m_iQuer_400 = atoi(m_strSFQuer400);
        m_pMan->m_pSF->m_iSenkrecht_400 = atoi(m_strSFSenk400);
    }
    if (m_pMan->m_pSF2 != NULL)
    {
        m_pMan->m_pSF2->m_iQuer = atoi(m_strSF2Quer);
        m_pMan->m_pSF2->m_iSenkrecht = atoi(m_strSF2Senk);
        m_pMan->m_pSF2->m_iQuer_400 = atoi(m_strSF2Quer400);
        m_pMan->m_pSF2->m_iSenkrecht_400 = atoi(m_strSF2Senk400);
    }
}

void CZRSFPage::ResetChanged()
{
    m_bChanged = FALSE;
}

BOOL CZRSFPage::OnQueryCancel() 
{
    if (m_bChanged == TRUE)
        return FALSE;
	return CPropertyPage::OnQueryCancel();
}

BOOL CZRSFPage::OnKillActive() 
{
    UpdateData(TRUE);
    
	return CPropertyPage::OnKillActive();
}

BOOL CZRSFPage::OnInitDialog() 
{
    char cBuf[250];

	CPropertyPage::OnInitDialog();
#if 0	
	m_ctrlSF2Senk.SubclassDlgItem(IDC_ZR_SF2_SENK, this);
	m_ctrlSF2Quer.SubclassDlgItem(IDC_ZR_SF2_QUER, this);
	m_ctrlSFSenk.SubclassDlgItem(IDC_ZR_SF_SENK, this);
	m_ctrlSFQuer.SubclassDlgItem(IDC_ZR_SF_QUER, this);
#endif
    if (m_pMan != NULL)
    {
        if (m_pMan->m_pSF != NULL)
        {
            itoa(m_pMan->m_pSF->m_iQuer, cBuf, 10);
            m_strSFQuer = cBuf;
            itoa(m_pMan->m_pSF->m_iSenkrecht, cBuf, 10);
            m_strSFSenk = cBuf;
            itoa(m_pMan->m_pSF->m_iQuer_400, cBuf, 10);
            m_strSFQuer400 = cBuf;
            itoa(m_pMan->m_pSF->m_iSenkrecht_400, cBuf, 10);
            m_strSFSenk400 = cBuf;
            UpdateData(FALSE);
        }
        if (m_pMan->m_pSF2 != NULL)
        {
            itoa(m_pMan->m_pSF2->m_iQuer, cBuf, 10);
            m_strSF2Quer = cBuf;
            itoa(m_pMan->m_pSF2->m_iSenkrecht, cBuf, 10);
            m_strSF2Senk = cBuf;
            itoa(m_pMan->m_pSF2->m_iQuer_400, cBuf, 10);
            m_strSF2Quer400 = cBuf;
            itoa(m_pMan->m_pSF2->m_iSenkrecht_400, cBuf, 10);
            m_strSF2Senk400 = cBuf;
            UpdateData(FALSE);
        }
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

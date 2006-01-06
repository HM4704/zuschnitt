// HoleProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "HoleProfileDoc.h"
#include "HoleProfileDlg.h"
#include "HoleProfilEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDlg dialog


CHoleProfileDlg::CHoleProfileDlg(CPtrArray* pContainer, CWnd* pParent /*=NULL*/)
	: CDialog(CHoleProfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHoleProfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_pContainer = pContainer;
}


void CHoleProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoleProfileDlg)
	DDX_Control(pDX, IDC_BH_PROFIL_EDIT, m_btProfilEdit);
	DDX_Control(pDX, IDC_BH_PROFIL_DEL, m_btProfilDel);
	DDX_Control(pDX, IDC_BH_PROFIL_LIST, m_lcProfilList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoleProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CHoleProfileDlg)
	ON_BN_CLICKED(IDC_BH_PROFIL_NEW, OnBhProfilNew)
	ON_BN_CLICKED(IDC_BH_PROFIL_EDIT, OnBhProfilEdit)
	ON_BN_CLICKED(IDC_BH_PROFIL_DEL, OnBhProfilDel)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_PROFIL_LIST, OnItemActivateProfilList)
	ON_NOTIFY(NM_CLICK, IDC_BH_PROFIL_LIST, OnClickProfilList)
	ON_NOTIFY(NM_DBLCLK, IDC_BH_PROFIL_LIST, OnDblclkProfilList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDlg message handlers
BOOL CHoleProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitProfilList();

    FillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CHoleProfileDlg::InitProfilList(void)
{
    // set ex styles
    ListView_SetExtendedListViewStyleEx(
        m_lcProfilList.GetSafeHwnd(),
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE,
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE
    );

    // create columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Anzahl";
	lvc.cx = m_lcProfilList.GetStringWidth(lvc.pszText) + 20;
    lvc.iImage = -1;
	m_lcProfilList.InsertColumn(0,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) "Länge";
	lvc.cx = m_lcProfilList.GetStringWidth(lvc.pszText) + 60;
    lvc.iSubItem = 1;
	m_lcProfilList.InsertColumn(1,&lvc);

}

void CHoleProfileDlg::FillList(void)
{
    m_lcProfilList.DeleteAllItems();
    for (int i=0; i<m_pContainer->GetSize(); i++)
    {
        CHoleProfile* pProfil = (CHoleProfile*)m_pContainer->GetAt(i);
        if (pProfil)
            InsertProfil(pProfil);
    }
    UpdateButtons();
}


void CHoleProfileDlg::InsertProfil(CHoleProfile* pProfil)
{
	LV_ITEM lvItem;
    char szBuf[20];
    int iItem = m_lcProfilList.GetItemCount();

    // Anzahl
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
    sprintf(szBuf, "%ld", pProfil->m_dwCount);
	lvItem.pszText = (LPTSTR) szBuf;
	lvItem.iImage = -1;
	lvItem.lParam = (long)pProfil;
	m_lcProfilList.InsertItem(&lvItem);

    // Laenge
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 1;
    sprintf(szBuf, "%ld", pProfil->m_dwLength);
	lvItem.pszText = (LPTSTR) szBuf;
	m_lcProfilList.SetItem(&lvItem);
}

void CHoleProfileDlg::UpdateButtons(void)
{
#if 1
    BOOL bEnable = (m_lcProfilList.GetSelectedCount() == 1);
    m_btProfilDel.EnableWindow(bEnable);
    m_btProfilEdit.EnableWindow(bEnable);
#endif // 0
}

void CHoleProfileDlg::OnBhProfilNew() 
{
    CHoleProfile* pProfil = new CHoleProfile;

    CHoleProfilEditDlg dlg(pProfil);

    if (dlg.DoModal() == IDOK)
    {
        m_pContainer->Add(pProfil);
        FillList();
        return;
    }

    delete pProfil;
}

void CHoleProfileDlg::OnOK() 
{
	CDialog::OnOK();
}

void CHoleProfileDlg::OnBhProfilEdit() 
{
    // Profil finden
    CHoleProfile* pPrfl = NULL;
    if (m_lcProfilList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lcProfilList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        pPrfl = (CHoleProfile*)m_lcProfilList.GetItemData(iItem);
    }

    // Dialog starten
    if (pPrfl == NULL)
    {
        AfxMessageBox("Bitte ein Profil auswählen", MB_OK, 0);
        return;
    }

    CHoleProfilEditDlg dlg(pPrfl);
    if (dlg.DoModal() == IDOK)
    {
        FillList();
    }
    UpdateButtons();
}

void CHoleProfileDlg::OnBhProfilDel() 
{
    if (m_lcProfilList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lcProfilList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        CHoleProfile* pPrfl = (CHoleProfile*)m_lcProfilList.GetItemData(iItem);
        // richtigen Eintrag suchen
        for (int i=0; i<m_pContainer->GetSize(); i++)
        {
            CHoleProfile* pF = (CHoleProfile*)m_pContainer->GetAt(i);
            if (pF == pPrfl)
            {
			    delete pF;
                m_pContainer->RemoveAt(i);
                break;
            }
        }        

        FillList();
    }
    else
    {
        MessageBox("Bitte das zu löschende Profil auswählen", "Info");
    }	
}

void CHoleProfileDlg::OnClickProfilList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UpdateButtons();
	
	*pResult = 0;
}

void CHoleProfileDlg::OnDblclkProfilList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_lcProfilList.GetSelectedCount() > 0)
        OnBhProfilEdit();

    UpdateButtons();

	*pResult = 0;
}

void CHoleProfileDlg::OnItemActivateProfilList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    UpdateButtons();

	*pResult = 0;
}

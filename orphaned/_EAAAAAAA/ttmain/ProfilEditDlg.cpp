// ProfilEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Macros.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ZRTreeCtrl.h"
#include "ttmain.h"
#include "ProfileElemDlg.h"
#include "ProfilEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CImageListEx::Init(int cx, int cy, UINT nFlags, int nInitial, int nGrow)
{
    memset(m_imgList, 0, sizeof(m_imgList));

    Create(cx, cy, nFlags, nInitial, nGrow);
}

void CImageListEx::InsertImage(char* szName, CBitmap* pbmImage, COLORREF crMask)
{
    for (int i=0; i<IMG_LIST_SIZE; i++)
    {
        if (m_imgList[i].bUsed == FALSE)
        {
            int iIdx = Add(pbmImage, crMask);
            if (iIdx != -1)
            {
                m_imgList[i].iImgIdx = iIdx;
                strncpy(m_imgList[i].szImgName, szName, sizeof(m_imgList[i].szImgName));
                m_imgList[i].bUsed = TRUE;
                return;
            }
        }
    }
}

int CImageListEx::FindImageWithName(char* szName)
{
    for (int i=0; i<IMG_LIST_SIZE; i++)
    {
        if (m_imgList[i].bUsed == TRUE)
        {
            if (strncmp(m_imgList[i].szImgName, szName, sizeof(m_imgList[i].szImgName)) == 0)
                return m_imgList[i].iImgIdx;
        }
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CProfilEditDlg dialog


CProfilEditDlg::CProfilEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfilEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfilEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_pImgList = NULL;
}

CProfilEditDlg::CProfilEditDlg(CWnd* pParent, CTorDoor* pTor)
	: CDialog(CProfilEditDlg::IDD, pParent)
{
    m_pTor = pTor;
    m_pImgList = NULL;
}

CProfilEditDlg::~CProfilEditDlg(void)
{
    DeleteProfiles(&m_aProfile);
    SAFE_DELETE(m_pImgList);
}

void CProfilEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfilEditDlg)
	DDX_Control(pDX, IDC_PROFIL_LIST, m_lvProfilList);
	DDX_Control(pDX, IDC_PROFIL_EDIT, m_btProfilEdit);
	DDX_Control(pDX, IDC_PROFIL_DEL, m_btProfilDel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfilEditDlg, CDialog)
	//{{AFX_MSG_MAP(CProfilEditDlg)
	ON_BN_CLICKED(IDC_PROFIL_DEL, OnProfilDel)
	ON_BN_CLICKED(IDC_PROFIL_NEW, OnProfilNew)
	ON_BN_CLICKED(IDC_PROFIL_EDIT, OnProfilEdit)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_PROFIL_LIST, OnItemActivateProfilList)
	ON_NOTIFY(NM_DBLCLK, IDC_PROFIL_LIST, OnDblclkProfilList)
	ON_NOTIFY(NM_CLICK, IDC_PROFIL_LIST, OnClickProfilList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfilEditDlg message handlers

BOOL CProfilEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    // Profile lokal kopieren zum Bearbeiten
    CopyProfiles(m_pTor->Profile, &m_aProfile);

    InitProfilList();

    FillList();

    UpdateButtons();

    // alle Profile in Liste eintragen
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProfilEditDlg::OnProfilDel() 
{
    if (m_lvProfilList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lvProfilList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        CProfileElem* pPrfl = (CProfileElem*)m_lvProfilList.GetItemData(iItem);
        // richtigen Eintrag suchen
        for (int i=0; i<m_aProfile.GetSize(); i++)
        {
            CProfileElem* pF = (CProfileElem*)m_aProfile.GetAt(i);
            if (pF == pPrfl)
            {
			    delete pF;
                m_aProfile.RemoveAt(i);
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


void CProfilEditDlg::FillList(void)
{
    m_lvProfilList.DeleteAllItems();
    for (int i=0; i<m_aProfile.GetSize(); i++)
    {
        CProfileElem* pF = (CProfileElem*)m_aProfile.GetAt(i);
        if (pF)
            InsertProfil(pF);
    }
    UpdateButtons();
}


void CProfilEditDlg::CopyProfiles(CPtrArray* pArrSrc, CPtrArray* pArrDest)
{
    DeleteProfiles(pArrDest);

    for (int i=0; i<pArrSrc->GetSize(); i++)
    {
        CProfileElem* pF = (CProfileElem*)pArrSrc->GetAt(i);
        if (pF)
        {
            CProfileElem* pNew = new CProfileElem(pF->Anzahl, pF->Laenge, pF->Direction,
     				         pF->uTyp, pF->Profil, pF->iWinkel1, pF->iWinkel2);
            pArrDest->Add(pNew);
        }
    }
}

void CProfilEditDlg::DeleteProfiles(CPtrArray* pArrSrc)
{
	int count = pArrSrc->GetSize();
	for (int i=0; i<count; i++)
	{
        CProfileElem* pF = (CProfileElem*)pArrSrc->GetAt(i);
		delete pF;
	}
	pArrSrc->RemoveAll();
}

void CProfilEditDlg::OnOK() 
{
    // Änderungen übernehmen
    CopyProfiles(&m_aProfile, m_pTor->Profile);        

	CDialog::OnOK();
}

void CProfilEditDlg::OnProfilNew() 
{
    // Dialog starten
    CProfileElemDlg dlg(this, m_pTor);
    if (dlg.DoModal() == IDOK)
    {
        CProfileElem* pPrfl = new CProfileElem;
        if (m_pTor->Typ != 4)
            // Typ 600, 800, RP6_8 Vorrauswahl
            pPrfl->Profil = RP6_8;
        dlg.GetProfile(pPrfl);
//        pPrfl->Direction = SENKRECHT;
        m_aProfile.Add(pPrfl);
        FillList();
    }
}

void CProfilEditDlg::OnProfilEdit() 
{
    // Profil finden
    CProfileElem* pPrfl = NULL;
    if (m_lvProfilList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lvProfilList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        pPrfl = (CProfileElem*)m_lvProfilList.GetItemData(iItem);
    }

    // Dialog starten
    if (pPrfl == NULL)
    {
        AfxMessageBox("Bitte ein Profil auswählen", MB_OK, 0);
        return;
    }

    CProfileElemDlg dlg(this, m_pTor, pPrfl);
    if (dlg.DoModal() == IDOK)
    {
        dlg.GetProfile(pPrfl);
        FillList();
    }
    UpdateButtons();
}

void CProfilEditDlg::UpdateButtons(void)
{
    BOOL bEnable = (m_lvProfilList.GetSelectedCount() == 1);
    m_btProfilDel.EnableWindow(bEnable);
    m_btProfilEdit.EnableWindow(bEnable);
}


void CProfilEditDlg::InitProfilList(void)
{
    // set ex styles
    ListView_SetExtendedListViewStyleEx(
        m_lvProfilList.GetSafeHwnd(),
        LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE,
        LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE
    );

    // create columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Anzahl";
	lvc.cx = m_lvProfilList.GetStringWidth(lvc.pszText) + 50;
    lvc.iImage = -1;
	m_lvProfilList.InsertColumn(0,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) "Typ";
	lvc.cx = m_lvProfilList.GetStringWidth(lvc.pszText) + 35;
    lvc.iSubItem = 1;
	m_lvProfilList.InsertColumn(1,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) "Länge";
	lvc.cx = m_lvProfilList.GetStringWidth(lvc.pszText) + 20;
    lvc.iSubItem = 2;
	m_lvProfilList.InsertColumn(2,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) "Richtung";
	lvc.cx = m_lvProfilList.GetStringWidth(lvc.pszText) + 15;
    lvc.iSubItem = 3;
	m_lvProfilList.InsertColumn(3,&lvc);

    // create and assoc. image list
    TDataScan ds;
	m_pImgList = new CImageListEx();
    ASSERT(m_pImgList);
	m_pImgList->Init(36, 21, ILC_COLOR8, 2, 2);
	CBitmap             bitmap;
	bitmap.LoadBitmap(IDB_RPRR);
	m_pImgList->InsertImage(ds.getNameForProfil(RPRR), &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1087);
	m_pImgList->InsertImage(ds.getNameForProfil(RP1087), &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1093);
	m_pImgList->InsertImage(ds.getNameForProfil(RP1093), &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1094);
	m_pImgList->InsertImage(ds.getNameForProfil(RP1094), &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1348);
	m_pImgList->InsertImage(ds.getNameForProfil(RP1348), &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP6_8);
	m_pImgList->InsertImage(ds.getNameForProfil(RP6_8), &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
    // Richtung
	bitmap.LoadBitmap(IDB_PROFIL_WAAG);
	m_pImgList->InsertImage("WAAGRECHT", &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_PROFIL_SENK);
	m_pImgList->InsertImage("SENKRECHT", &bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
    m_lvProfilList.SetImageList(m_pImgList, LVSIL_SMALL);
}


void CProfilEditDlg::InsertProfil(CProfileElem* pProfil)
{
	LV_ITEM lvItem;
    char szBuf[20];
    TDataScan ds;
    int iItem = m_lvProfilList.GetItemCount();

    // Anzahl
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
    sprintf(szBuf, "%ld St.", pProfil->Anzahl);
	lvItem.pszText = (LPTSTR) szBuf;
	lvItem.iImage = -1;
	lvItem.lParam = (long)pProfil;
	m_lvProfilList.InsertItem(&lvItem);

    // Profil-Typ
	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 1;
	lvItem.iImage = m_pImgList->FindImageWithName(ds.getNameForProfil(pProfil->Profil));
	int iRet = m_lvProfilList.SetItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 2;
    sprintf(szBuf, "%ld", pProfil->Laenge);
	lvItem.pszText = (LPTSTR) szBuf;
	m_lvProfilList.SetItem(&lvItem);

	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 3;
    if (pProfil->Direction == WAAGRECHT)
	    lvItem.iImage = m_pImgList->FindImageWithName("WAAGRECHT");
    else
	    lvItem.iImage = m_pImgList->FindImageWithName("SENKRECHT");
	m_lvProfilList.SetItem(&lvItem);
}


void CProfilEditDlg::OnItemActivateProfilList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    UpdateButtons();

	*pResult = 0;
}

void CProfilEditDlg::OnDblclkProfilList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_lvProfilList.GetSelectedCount() > 0)
        OnProfilEdit();

    UpdateButtons();

	*pResult = 0;
}

void CProfilEditDlg::OnClickProfilList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UpdateButtons();
	
	*pResult = 0;
}

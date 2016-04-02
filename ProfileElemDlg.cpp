// ProfileElemDlg.cpp : implementation file
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileElemDlg dialog


CProfileElemDlg::CProfileElemDlg(CWnd* pParent, CTorDoor* pTor)
	: CDialog(CProfileElemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfileElemDlg)
	m_iProfileElemCount = 0;
	m_iProfileElemLength = 0;
	m_strProfileElemType = _T("");
	//}}AFX_DATA_INIT
    m_pTor = pTor;
    if (m_pTor->Typ != 4)
        m_Prfl.Profil = RP6_8;
}

CProfileElemDlg::CProfileElemDlg(CWnd* pParent, CTorDoor* pTor, CProfileElem* pPrfl)
	: CDialog(CProfileElemDlg::IDD, pParent)
{
    m_pTor = pTor;
    m_Prfl = *pPrfl;
}

void CProfileElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileElemDlg)
	DDX_Control(pDX, IDC_PROFILE_ORIENTATION, m_cbProfileOrientation);
	DDX_Control(pDX, IDC_PROFILE_ELEM_TYPE, m_cbProfileType);
	DDX_Text(pDX, IDC_PROFILE_ELEM_COUNT, m_iProfileElemCount);
	DDX_Text(pDX, IDC_PROFILE_ELEM_LENGTH, m_iProfileElemLength);
	DDX_CBString(pDX, IDC_PROFILE_ELEM_TYPE, m_strProfileElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileElemDlg, CDialog)
	//{{AFX_MSG_MAP(CProfileElemDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileElemDlg message handlers

BOOL CProfileElemDlg::OnInitDialog() 
{

	CDialog::OnInitDialog();
	
    // lade Profil Typ Combo-Box
    TDataScan ds;

    CImageList          *pImageList = NULL;
	pImageList = new CImageList();
	pImageList->Create(36, 21, ILC_COLOR8, 2, 2);
	CBitmap             bitmap;
	bitmap.LoadBitmap(IDB_RPRR);
	pImageList->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1087);
	pImageList->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1093);
	pImageList->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1094);
	pImageList->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP1348);
	pImageList->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_RP6_8);
	pImageList->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
    m_cbProfileType.SetImageList(pImageList );

    COMBOBOXEXITEM cbi;
    cbi.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 0;
    cbi.pszText = ds.getNameForProfil(RPRR);
    cbi.cchTextMax = 20;  //??
    cbi.iImage = 0;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = RPRR;
    m_cbProfileType.InsertItem(&cbi);
    cbi.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 1;
    cbi.pszText = ds.getNameForProfil(RP1087);
    cbi.cchTextMax = 20;  //??
    cbi.iImage = 1;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = RP1087;
    m_cbProfileType.InsertItem(&cbi);
    cbi.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 2;
    cbi.pszText = ds.getNameForProfil(RP1093);
    cbi.cchTextMax = 20;  //??
    cbi.iImage = 2;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = RP1093;
    m_cbProfileType.InsertItem(&cbi);
    cbi.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 3;
    cbi.pszText = ds.getNameForProfil(RP1094);
    cbi.cchTextMax = 20;  //??
    cbi.iImage = 3;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = RP1094;
    m_cbProfileType.InsertItem(&cbi);
    cbi.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 4;
    cbi.pszText = ds.getNameForProfil(RP1348);
    cbi.cchTextMax = 20;  //??
    cbi.iImage = 4;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = RP1348;
    m_cbProfileType.InsertItem(&cbi);
    if (m_pTor->Typ != 4)
    {
        cbi.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
        cbi.iItem = 5;
        cbi.iImage = 5;
        cbi.iSelectedImage = cbi.iImage;
        cbi.lParam = RP6_8;
        m_cbProfileType.InsertItem(&cbi);
    }

    // lade Profil Orientierungs Combo
    CImageList          *pImageListO = NULL;
	pImageListO = new CImageList();
	pImageListO->Create(36, 21, ILC_COLOR8, 2, 2);
	bitmap.LoadBitmap(IDB_PROFIL_WAAG);
	pImageListO->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_PROFIL_SENK);
	pImageListO->Add(&bitmap, (COLORREF)0x00000000);
	bitmap.DeleteObject();
    m_cbProfileOrientation.SetImageList(pImageListO );

    cbi.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 0;
    cbi.iImage = 0;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = WAAGRECHT;
    m_cbProfileOrientation.InsertItem(&cbi);
    cbi.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
    cbi.iItem = 1;
    cbi.iImage = 1;
    cbi.iSelectedImage = cbi.iImage;
    cbi.lParam = SENKRECHT;
    m_cbProfileOrientation.InsertItem(&cbi);

    CopyValues(TRUE);

    if (m_pTor->Typ != 4)
    {
        // Typ 600 +800, nur ein Profil
        m_cbProfileType.EnableWindow(FALSE);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CProfileElemDlg::CopyValues(BOOL bLoad)
{
    COMBOBOXEXITEM cbi;
    int i;
    if (bLoad == TRUE)
    {
        // in Dialog-Elemente laden
        m_iProfileElemCount = m_Prfl.Anzahl;
        m_iProfileElemLength = m_Prfl.Laenge;
//        m_strProfileElemType = ds.getNameForProfil(m_Prfl.Profil);
//        int iRet = m_cbProfileType.SelectString(-1, ds.getNameForProfil(m_Prfl.Profil));
        for (i=0; i<m_cbProfileType.GetCount(); i++)
        {
            cbi.mask = CBEIF_LPARAM;
            cbi.iItem = i;
            m_cbProfileType.GetItem(&cbi);
            if (cbi.lParam == m_Prfl.Profil)
            {
                m_cbProfileType.SetCurSel(i);
                break;
            }
        }
        for (i=0; i<m_cbProfileOrientation.GetCount(); i++)
        {
            cbi.mask = CBEIF_LPARAM;
            cbi.iItem = i;
            m_cbProfileOrientation.GetItem(&cbi);
            if (cbi.lParam == m_Prfl.Direction)
            {
                m_cbProfileOrientation.SetCurSel(i);
                break;
            }
        }

        UpdateData(FALSE);
    }
    else
    {
        // aus Dialog-Elementen holenn
        UpdateData(TRUE);

        m_Prfl.Anzahl = m_iProfileElemCount;
        m_Prfl.Laenge = m_iProfileElemLength;
        if (m_cbProfileType.GetCurSel() != CB_ERR)
        {
            cbi.mask = CBEIF_LPARAM;
            cbi.iItem = m_cbProfileType.GetCurSel();
            m_cbProfileType.GetItem(&cbi);
            m_Prfl.Profil = (tProfil)cbi.lParam;
        }
        if (m_cbProfileOrientation.GetCurSel() != CB_ERR)
        {
            cbi.mask = CBEIF_LPARAM;
            cbi.iItem = m_cbProfileOrientation.GetCurSel();
            m_cbProfileOrientation.GetItem(&cbi);
            m_Prfl.Direction = (tDirect)cbi.lParam;
        }
    }
}


void CProfileElemDlg::GetProfile(CProfileElem* pPrfl)
{
    *pPrfl = m_Prfl;
}

void CProfileElemDlg::OnOK() 
{
    CopyValues(FALSE);

    // Tests
    BOOL bOk = TRUE;
    if (m_Prfl.Anzahl <= 0)
        bOk = FALSE;
    if (m_Prfl.Laenge <= 0)
        bOk = FALSE;
    if (m_Prfl.Profil == NP)
        bOk = FALSE;

    if (bOk == FALSE)
    {
        AfxMessageBox("Bitte korrekte Werte eingeben!", MB_OK, 0);
        return;
    }

	CDialog::OnOK();
}

void CProfileElemDlg::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class	
	CDialog::PreSubclassWindow();

}

BOOL CProfileElemDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreCreateWindow(cs);
}

void CProfileElemDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
    CImageList* pIL = m_cbProfileType.GetImageList();	
    SAFE_DELETE(pIL);
    pIL = m_cbProfileOrientation.GetImageList();	
    SAFE_DELETE(pIL);
}

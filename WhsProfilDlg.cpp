// WhsProfilDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "PrintEx\\PrintPreviewDialog.h"
#include "AnsteuerungPS312\\PS312CtrlDlg.h"
#include "CutlistSettDlg.h"
#include "ProfilPropDlg.h"
#include "WhsProfilDlg.h"


#define WSH_PROF_INI_SECTION  "WSHProfile"

// CWhsProfilDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CWhsProfilDlg, CDialog)

CWhsProfilDlg::CWhsProfilDlg(CWnd* pParent /*=NULL*/)
	: CPrintPreviewDialog(CWhsProfilDlg::IDD, pParent)
    , m_fEticetLen(48.5)
    , m_fEticetHeight(25.4)
    , m_iEticetLeft(2)
    , m_iEticetTop(17)
    , m_strListName(_T(""))
    , m_bRoundUpProfileCount(TRUE)
    , m_iProfileCount(0)
    , m_iBladeWidth(3)
    , m_iProfileLen(5600)
    , m_bPrintProfMark(FALSE)
{
    getFont(&m_font, /*FONTHEIGTH*/ 160, /*FONTWIDTH*/ 60, 0, 550);
    getFont(&m_fontPageNo, /*FONTHEIGTH*/ 80, /*FONTWIDTH*/ 30, 0, 200);
    
    m_MarkPen.CreatePen(PS_SOLID, 5,  RGB(0,0,0));

    m_fEticetLen    = ((float)AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "EttikettenBreite", 485))/10.0f;
    m_fEticetHeight = ((float)AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "EttikettenHoehe", 254))/10.0f;
    m_iEticetLeft   = AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "RandLinks", 8);
    m_iEticetTop    = AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "RandOben", 10);
    m_bRoundUpProfileCount = AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "ProfileAufrunden", 1);
    m_iBladeWidth   = AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "SaegeblattDicke", 3);
    m_iProfileLen   = AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "ProfilLaenge", 5600);
    m_bPrintProfMark = AfxGetApp()->GetProfileInt(WSH_PROF_INI_SECTION, "MarkProfAnfang", 0);

    m_pWHSProfile = new CWHSProfilZuschnitt(m_iProfileLen, m_iBladeWidth, m_bRoundUpProfileCount);
    m_rcEticet.left = m_rcEticet.top = m_rcEticet.right = m_rcEticet.bottom = 0;
}

CWhsProfilDlg::~CWhsProfilDlg()
{
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "EttikettenBreite", (int)(m_fEticetLen*10.0f + 0.0001f));
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "EttikettenHoehe", (int)(m_fEticetHeight*10.0f + 0.0001f));
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "RandLinks", m_iEticetLeft);
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "RandOben", m_iEticetTop);
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "ProfileAufrunden", m_bRoundUpProfileCount);
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "SaegeblattDicke", m_iBladeWidth);
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "ProfilLaenge", m_iProfileLen);
    AfxGetApp()->WriteProfileInt(WSH_PROF_INI_SECTION, "MarkProfAnfang", m_bPrintProfMark);

    if (m_pWHSProfile)
    {
        delete m_pWHSProfile;
        m_pWHSProfile = NULL;
    }
	DeleteObject(m_font);
	DeleteObject(m_fontPageNo);
	DeleteObject(m_MarkPen);
}

void CWhsProfilDlg::DoDataExchange(CDataExchange* pDX)
{
    CPrintPreviewDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_WHS_ETICET_LEN, m_fEticetLen);
    DDX_Text(pDX, IDC_WHS_ETICET_HEIGHT, m_fEticetHeight);
    DDX_Text(pDX, IDC_WHS_ETICET_LEFT, m_iEticetLeft);
    DDX_Text(pDX, IDC_WHS_ETICET_TOP, m_iEticetTop);
    DDX_Text(pDX, IDC_WHS_PROFIL_CNT, m_iProfileCount);
    DDX_Text(pDX, IDC_WHS_LIST_NAME, m_strListName);
    DDX_Check(pDX, IDC_WHS_ROUND_UP_P, m_bRoundUpProfileCount);
    DDX_Control(pDX, IDC_WHS_PROFIL_LIST, m_lcWhsProfilList);
    DDX_Text(pDX, IDC_WHS_BLADE_WIDTH, m_iBladeWidth);
    DDV_MinMaxInt(pDX, m_iBladeWidth, 0, 100);
    DDX_Text(pDX, IDC_WHS_PROFILE_LEN, m_iProfileLen);
    DDV_MinMaxInt(pDX, m_iProfileLen, 1, 100000);
    DDX_Check(pDX, IDC_WH_PRINT_PROF_MARK, m_bPrintProfMark);
    DDX_Control(pDX, ID_FILE_PRINT_PREVIEW, m_btPrintPreview);
    DDX_Control(pDX, ID_FILE_PRINT, m_btPrint);
    DDX_Control(pDX, IDC_WHS_SAVE_CUTLIST, m_btSaveCutlist);
    DDX_Control(pDX, IDC_START_SAW_DLG, m_btStartSawDlg);
    DDX_Control(pDX, ID_WHS_VERIFY, m_btVerify);
    DDX_Control(pDX, IDC_WHS_PROFIL_NEW, m_btProfilNew);
    DDX_Control(pDX, IDC_WHS_PROFIL_EDIT, m_btProfilEdit);
    DDX_Control(pDX, IDC_WHS_PROFIL_DEL, m_btProfilDel);
}


BEGIN_MESSAGE_MAP(CWhsProfilDlg, CPrintPreviewDialog)
    ON_BN_CLICKED(IDC_WHS_PROFIL_OPEN, &CWhsProfilDlg::OnBnClickedWhsProfilOpen)
    ON_EN_CHANGE(IDC_WHS_BLADE_WIDTH, &CWhsProfilDlg::OnEnChangeBladeWidth)
    ON_EN_CHANGE(IDC_WHS_PROFILE_LEN, &CWhsProfilDlg::OnEnChangeProfileLen)
    ON_BN_CLICKED(IDC_WHS_ROUND_UP_P, &CWhsProfilDlg::OnBnClickedRoundUpP)
    ON_BN_CLICKED(IDC_WHS_SAVE_CUTLIST, &CWhsProfilDlg::OnClickedSaveCutlist)
    ON_BN_CLICKED(IDC_START_SAW_DLG, &CWhsProfilDlg::OnClickedStartSawDlg)
    ON_BN_CLICKED(IDC_WHS_EDIT_CUTLIST_SETT, &CWhsProfilDlg::OnClickedWhsEditCutlistSett)
    ON_BN_CLICKED(ID_WHS_VERIFY, &CWhsProfilDlg::OnClickedWhsVerify)
    ON_BN_CLICKED(IDC_WHS_PROFIL_NEW, &CWhsProfilDlg::OnBnClickedWhsProfilNew)
    ON_BN_CLICKED(IDC_WHS_PROFIL_EDIT, &CWhsProfilDlg::OnBnClickedWhsProfilEdit)
    ON_BN_CLICKED(IDC_WHS_PROFIL_DEL, &CWhsProfilDlg::OnBnClickedWhsProfilDel)
    ON_NOTIFY(NM_CLICK, IDC_WHS_PROFIL_LIST, &CWhsProfilDlg::OnNMClickWhsProfilList)
    ON_NOTIFY(NM_DBLCLK, IDC_WHS_PROFIL_LIST, &CWhsProfilDlg::OnNMDblclkWhsProfilList)
END_MESSAGE_MAP()

void CWhsProfilDlg::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	ASSERT(pDC) ;
	ASSERT(pInfo) ;
	// do some default output onto the page
    int iCount, i, iProfile,iX, iY, iLength, iProfCount, iLastLen;
    bool bNewProfile;
    char szBuf[20];

    pDC->SetMapMode(MM_LOMETRIC);
    CFont* pFont =  pDC->SelectObject(&m_font);
    CPen* orgPen = (CPen*)pDC->SelectObject(&m_MarkPen);
    if (m_iPageCount == 0)
        return;
    if (pInfo->m_nCurPage == m_iPageCount)
    {
        // letzte Seite
        iCount = m_iProfileCount%m_iProfilesPerPage;
    }
    else
    {
        iCount = m_iProfilesPerPage;
    }

    iLength = 0;
    iLastLen = 0;
    bNewProfile = true;
    if (m_bPrintProfMark)
    {
        // Marke für Profilanfang drucken
        // auf aktuelles Profil positionieren
        // es kommen 
        for (int ii=0; ii < (pInfo->m_nCurPage-1)*m_iProfilesPerPage; ii++)
        {
            if (iLastLen != m_PrintList[ii])
            {
                iLength += m_PrintList[ii];
                iLastLen = m_PrintList[ii];
                if (iLength > m_iProfileLen)
                {
                    // neues Profil
                    iLength = m_PrintList[ii] + m_iBladeWidth;
                    bNewProfile = true;
                }
                else
                {
                    iLength += m_iBladeWidth;  // Verlust durch Schneiden
                    bNewProfile = false;
                }
            }
            else
            {
                // es ist ein gleiches Profil gekommen,das nächste auf alle Fälle wieder zählen
                iLastLen = 0;
            }
        }
    }
    iX = (m_iEticetLeft*10); 
    iY = -(m_iEticetTop*10);

    for (i = 0; i < iCount; i++)
    {
        iProfile = m_PrintList[((pInfo->m_nCurPage-1)*m_iProfilesPerPage)+i];

        if (m_bPrintProfMark)
        {
            if (iLastLen != m_PrintList[((pInfo->m_nCurPage-1)*m_iProfilesPerPage)+i])
            {
                iLength += m_PrintList[((pInfo->m_nCurPage-1)*m_iProfilesPerPage)+i];
                iLastLen = m_PrintList[((pInfo->m_nCurPage-1)*m_iProfilesPerPage)+i];
                if (iLength > m_iProfileLen)
                {
                    // neues Profil
                    iLength = m_PrintList[((pInfo->m_nCurPage-1)*m_iProfilesPerPage)+i] + m_iBladeWidth;
                    bNewProfile = true;
                }
                else
                {
                    iLength += m_iBladeWidth;  // Verlust durch Schneiden
                    bNewProfile = false;
                }
            }
            else
            {
                // es ist ein gleiches Profil gekommen,das nächste auf alle Fälle wieder zählen
                iLastLen = 0;
            }
            if (bNewProfile == true)
            {
                pDC->Rectangle(iX+50, iY-50, iX+55, iY-m_rcEticet.Height()+50);
                bNewProfile = false;
            }
        }

        sprintf(szBuf, "%d", iProfile);
        // center String
        CSize sExt = pDC->GetTextExtent(szBuf, strlen(szBuf));
        pDC->TextOut(iX+(m_rcEticet.Width()/2-sExt.cx/2), iY-(m_rcEticet.Height()/2-sExt.cy/2), szBuf, strlen(szBuf));
        if ((i%4) == 3)
        {
            // wrap around
            iX  = (m_iEticetLeft*10);
            iY -= m_rcEticet.Height();
        }
        else
        {
            iX += m_rcEticet.Width();
        }

        // 
    }
    
    // print current Page Nr
    pDC->SelectObject(&m_fontPageNo);
    CSize sExt = pDC->GetTextExtent(szBuf, strlen(szBuf));
    sprintf(szBuf, "%d/%d", pInfo->m_nCurPage, m_iPageCount);
    iX = m_iPageLen/2;
    iX -= sExt.cx/2;
    iY = -m_iPageHeight + sExt.cy + 30;
    pDC->TextOut(iX, iY-30, szBuf, strlen(szBuf));

    pDC->SelectObject(pFont);
    pDC->SelectObject(orgPen);
}

void CWhsProfilDlg::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    CPrintPreviewDialog::OnPrepareDC(pDC, pInfo);
}

BOOL CWhsProfilDlg::OnPreparePrinting(CPrintInfo* pInfo)
{
	ASSERT(pInfo) ;

    //?? wird schon beim Öffnen gemacht
    m_iProfileCount = m_pWHSProfile->GetProfileCount();
    m_pWHSProfile->GetCutListForPrint(m_PrintList);
    ASSERT(m_PrintList.size() == m_iProfileCount);
    // von A4 ausgehen
    // Einstellungen einlesen
    UpdateData(TRUE);
    m_rcEticet.right = (m_fEticetLen*10.0f + 0.0001f);
    m_rcEticet.bottom = (m_fEticetHeight*10.0f + 0.0001f);  //??? es kommt immer .999 raus

    m_iProfilesPerPage = ((2100 - 2*(m_iEticetLeft*10))/m_rcEticet.Width()) * ((2970 - 2*(m_iEticetTop*10))/m_rcEticet.Height());
    if (!m_iProfilesPerPage)
        return FALSE;
    m_iPageCount = m_iProfileCount/m_iProfilesPerPage;
    if (m_iProfileCount%m_iProfilesPerPage) m_iPageCount++;
	pInfo->SetMaxPage(m_iPageCount) ;
	// must call the base class to get correct operation
    CTtmainApp* pApp = (CTtmainApp*)AfxGetApp();
    if (pApp)
        pApp->SetPrintOrientation(DMORIENT_PORTRAIT);

	return CPrintPreviewDialog::OnPreparePrinting(pInfo);
}

void CWhsProfilDlg::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    int iDpiHorz = pDC->GetDeviceCaps(LOGPIXELSX);
    int iDpiVert = pDC->GetDeviceCaps(LOGPIXELSY);
    m_iPageLen = 10*MM_PER_INCH*pDC->GetDeviceCaps(HORZRES)/iDpiHorz;
    m_iPageHeight = 10*MM_PER_INCH*pDC->GetDeviceCaps(VERTRES)/iDpiVert;

    return CPrintPreviewDialog::OnBeginPrinting(pDC, pInfo);
}

void CWhsProfilDlg::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    CTtmainApp* pApp = (CTtmainApp*)AfxGetApp();
    if (pApp)
        pApp->SetPrintOrientation(DMORIENT_LANDSCAPE);
    return CPrintPreviewDialog::OnEndPrinting(pDC, pInfo);
}

// CWhsProfilDlg-Meldungshandler
void CWhsProfilDlg::OnBnClickedWhsProfilOpen()
{
//    CFileDialog dlg(TRUE, "csv", "*.csv");
    CFileDialog dlg(TRUE, "csv;xls", "*.csv;*.xls");

    if (dlg.DoModal() == IDOK)
    {
        CString szFile = dlg.GetPathName();
        
        if (szFile.GetLength() > 0)
        {
            BeginWaitCursor();
            char szError[MAX_PATH];
            sprintf(szError, "Fehler in Datei gefunden!");
            if (m_pWHSProfile->ReadProfileList(szFile.GetBuffer(szFile.GetLength()), szError) == false)
            {
                m_strListName = "";
                m_pWHSProfile->GenerateCutList();
                FillList();
                EndWaitCursor();
                // rausschreiben
                UpdateData(FALSE);
            
                EnableButtons();
                AfxMessageBox(szError, MB_ICONSTOP|MB_OK);
                return;
            }
            m_pWHSProfile->GenerateCutList();
            EndWaitCursor();

            m_strListName = szFile;
//            m_iProfileCount = m_pWHSProfile->GetProfileCount();
//            m_pWHSProfile->GetCutListForPrint(m_PrintList);
//            ASSERT(m_PrintList.size() == m_iProfileCount);

            FillList();

            // rausschreiben
            UpdateData(FALSE);
        
            EnableButtons();
        }
    }

}

void CWhsProfilDlg::InitProfilList(void)
{
    // set ex styles
    ListView_SetExtendedListViewStyleEx(
        m_lcWhsProfilList.GetSafeHwnd(),
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE,
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE
    );

    // create columns
	LV_COLUMN lvc;
    // dummy column um erste column rechts zu alignen
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "dummy";
	lvc.cx = m_lcWhsProfilList.GetStringWidth(lvc.pszText) + 20;
    lvc.iImage = -1;
	m_lcWhsProfilList.InsertColumn(0,&lvc);

	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Anzahl";
	lvc.cx = m_lcWhsProfilList.GetStringWidth(lvc.pszText) + 20;
    lvc.iImage = -1;
	m_lcWhsProfilList.InsertColumn(1,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Länge";
	lvc.cx = m_lcWhsProfilList.GetStringWidth(lvc.pszText) + 60;
    lvc.iSubItem = 1;
	m_lcWhsProfilList.InsertColumn(2,&lvc);

    // dummy column entfernen
    m_lcWhsProfilList.DeleteColumn(0);

}

void CWhsProfilDlg::FillList(void)
{
    MIIter   iter;
    InputMap InputMap;

    m_iProfileCount = m_pWHSProfile->GetProfileCount();
    m_pWHSProfile->GetProfileList(InputMap);

    m_lcWhsProfilList.DeleteAllItems();

    for (iter = InputMap.begin(); iter != InputMap.end(); iter++)
    {
        InsertProfil(iter->second, iter->first);
    }
#if 0
    for (int i=0; i<m_pContainer->GetSize(); i++)
    {
        CHoleProfile* pProfil = (CHoleProfile*)m_pContainer->GetAt(i);
        if (pProfil)
            InsertProfil(pProfil);
    }
#endif // 0
}


void CWhsProfilDlg::InsertProfil(int iCount, int iLength)
{
	LV_ITEM lvItem;
    char szBuf[20];
    int iItem = m_lcWhsProfilList.GetItemCount();

    // Anzahl
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
    sprintf(szBuf, "%ld", iCount);
	lvItem.pszText = (LPTSTR) szBuf;
	lvItem.iImage = -1;
	lvItem.lParam = (long)iLength;
	m_lcWhsProfilList.InsertItem(&lvItem);

    // Laenge
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 1;
    sprintf(szBuf, "%ld", iLength);
	lvItem.pszText = (LPTSTR) szBuf;
	m_lcWhsProfilList.SetItem(&lvItem);
}

void CWhsProfilDlg::getFont(CFont* pFont, int height, int width, int orient, int weight)
{
  LOGFONT MainFontRec;
  MainFontRec.lfHeight = height; //*15
  MainFontRec.lfWidth = width;   //*4
  MainFontRec.lfEscapement = orient;
  MainFontRec.lfOrientation = 0;
  MainFontRec.lfWeight = weight;//400;
  MainFontRec.lfItalic = 0;
  MainFontRec.lfUnderline = 0;
  MainFontRec.lfStrikeOut = 0;
  MainFontRec.lfCharSet = ANSI_CHARSET;
  MainFontRec.lfOutPrecision = OUT_DEFAULT_PRECIS;
  MainFontRec.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  MainFontRec.lfQuality = PROOF_QUALITY;
  MainFontRec.lfPitchAndFamily = DEFAULT_PITCH;
  strcpy(MainFontRec.lfFaceName, "Arial");   //Roman

  pFont->CreateFontIndirect(&MainFontRec);
}

BOOL CWhsProfilDlg::OnInitDialog()
{
    CPrintPreviewDialog::OnInitDialog();

    InitProfilList();

    EnableButtons();

    return TRUE;  // return TRUE unless you set the focus to a control
    // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CWhsProfilDlg::OnEnChangeBladeWidth()
{
    // TODO:  Falls dies ein RICHEDIT-Steuerelement ist, wird das Kontrollelement
    // diese Benachrichtigung nicht senden, es sei denn, Sie setzen den CPrintPreviewDialog::OnInitDialog() außer Kraft.
    // Funktion und Aufruf CRichEditCtrl().SetEventMask()
    // mit dem ENM_CHANGE-Flag ORed in der Eingabe.

    // TODO:  Fügen Sie hier Ihren Code für die Kontrollbenachrichtigungsbehandlung ein.
}

void CWhsProfilDlg::OnEnChangeProfileLen()
{
    // TODO:  Falls dies ein RICHEDIT-Steuerelement ist, wird das Kontrollelement
    // diese Benachrichtigung nicht senden, es sei denn, Sie setzen den CPrintPreviewDialog::OnInitDialog() außer Kraft.
    // Funktion und Aufruf CRichEditCtrl().SetEventMask()
    // mit dem ENM_CHANGE-Flag ORed in der Eingabe.

    // TODO:  Fügen Sie hier Ihren Code für die Kontrollbenachrichtigungsbehandlung ein.
}

void CWhsProfilDlg::OnBnClickedRoundUpP()
{
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
}

bool CWhsProfilDlg::BuildCutList(CXmlDocumentWrapper& cutList)
{
    vector<CUT_LIST_ELEM> sawList;
    vector<CUT_LIST_ELEM>::iterator iter;
    char szNode[20];

    if (cutList.LoadXML("<Schnittliste></Schnittliste>") == FALSE)
        return false;

    m_pWHSProfile->GetCutListForSaw(sawList);

    CXmlNodeWrapper rootNode(cutList.AsNode());
//    rootNode.SetText("Schnittliste für Profile");
    
    rootNode.SetValue("Version", 100);
    rootNode.SetValue("Profillaenge", m_iProfileLen);
    rootNode.SetValue("Saegeblattdicke", m_iBladeWidth);

    CXmlNodeWrapper node;

    int i=1;
    for (iter = sawList.begin(); iter != sawList.end( ); iter++)
    {
        sprintf(szNode, "PN-%04d", i);
        node = rootNode.InsertNode(i, szNode);
        node.SetValue("Anzahl", iter->iCount);
        node.SetValue("Laenge", iter->iLength);

        i++;
        node.Detach();
    }

    return true;
}

void CWhsProfilDlg::EnableButtons()
{
    BOOL bEnable;
    if (m_iProfileCount > 0)
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }
    m_btPrintPreview.EnableWindow(bEnable);
    m_btPrint.EnableWindow(bEnable);
    m_btSaveCutlist.EnableWindow(bEnable);
    m_btStartSawDlg.EnableWindow(bEnable);

    CTtmainApp *pApp = (CTtmainApp*) AfxGetApp();
    if (pApp)
    {
        if (pApp->IsInTestMode())
            m_btVerify.ShowWindow(SW_SHOW);
    }

    bEnable = (m_lcWhsProfilList.GetSelectedCount() == 1);
    m_btProfilDel.EnableWindow(bEnable);
    m_btProfilEdit.EnableWindow(bEnable);
}

void CWhsProfilDlg::OnClickedSaveCutlist()
{
    CFileDialog dlg(FALSE, "psl", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Profil Schnittliste (*.psl)|*.psl|Alle Dateien (*.*)|*.*||");
    
    if (dlg.DoModal() == IDOK)
    {
        CString strFile = dlg.GetPathName();

        CXmlDocumentWrapper xCutList;

        if (BuildCutList(xCutList) == true)
        {
            if (xCutList.Save(strFile.GetBuffer(strFile.GetLength())) == FALSE)
            {
                AfxMessageBox("Fehler beim Speichern der Datei!", MB_OK|MB_ICONSTOP);
            }
        }
        else
        {
            AfxMessageBox("Fehler beim Erzeugen der Schnittliste!", MB_OK|MB_ICONSTOP);
        }

    }
}

void CWhsProfilDlg::OnClickedStartSawDlg()
{
    CXmlDocumentWrapper xCutList;
    if (BuildCutList(xCutList) == true)
    {
        CPS312CtrlDlg dlg(&xCutList);

        dlg.DoModal();
    }
}

void CWhsProfilDlg::OnClickedWhsEditCutlistSett()
{
    CCutlistSettDlg dlg;
    dlg.m_iProfileLen = m_iProfileLen;
    dlg.m_iBladeWidth = m_iBladeWidth;
    dlg.m_bRoundUp    = m_bRoundUpProfileCount;
    if (dlg.DoModal() == IDOK)
    {
        m_iProfileLen = dlg.m_iProfileLen;
        m_iBladeWidth = dlg.m_iBladeWidth;
        m_bRoundUpProfileCount = dlg.m_bRoundUp;

        UpdateData(FALSE);

        // Schnittliste neu berechnen
        if (m_pWHSProfile->UpdateSettings(m_iProfileLen, m_iBladeWidth, m_bRoundUpProfileCount) == false)
        {
            AfxMessageBox("Fehler in der Berechnung der Schnittliste!", MB_OK|MB_ICONSTOP);
            return;
        }
//        m_pWHSProfile->GetCutListForPrint(m_PrintList);
//        ASSERT(m_PrintList.size() == m_iProfileCount);
    }
}

void CWhsProfilDlg::OnClickedWhsVerify()
{
    CFileDialog dlg(TRUE, "txt", "*.txt");

    if (dlg.DoModal() == IDOK)
    {
        CString szFile = dlg.GetPathName();
        
        if (szFile.GetLength() > 0)
        {
            char szError[MAX_PATH];
            if (m_pWHSProfile->Compare(szFile.GetBuffer(szFile.GetLength()), TRUE, FALSE, szError) == false)
            {
                AfxMessageBox(szError, MB_OK|MB_ICONSTOP);
            }
            else
            {
                AfxMessageBox("Kein Fehler gefunden", MB_OK|MB_ICONINFORMATION);
            }
        }
    }
}

void CWhsProfilDlg::OnBnClickedWhsProfilNew()
{
    CProfilPropDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        m_pWHSProfile->AddProfil(dlg.m_iLength, dlg.m_iCount);

        FillList();
        // rausschreiben
        UpdateData(FALSE);
    
        EnableButtons();
    }
}

void CWhsProfilDlg::OnBnClickedWhsProfilEdit()
{
    int iLength, iCount;
    BOOL bRedraw = FALSE;

    if (m_lcWhsProfilList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lcWhsProfilList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        iLength = (int)m_lcWhsProfilList.GetItemData(iItem);
        iCount = m_pWHSProfile->GetCountForProfil(iLength);
        
        if (iCount > 0)
        {
            CProfilPropDlg dlg(CProfilProp(iLength, iCount));
            if (dlg.DoModal() == IDOK)
            {
                if (dlg.m_iLength != iLength)
                {
                    // Profillänge hat sich geändert, erst mal löschen
                    m_pWHSProfile->DeleteProfil(iLength);
                    m_pWHSProfile->AddProfil(dlg.m_iLength, dlg.m_iCount);
                    bRedraw = TRUE;
                }
                else
                {
                    if (dlg.m_iCount != iCount)
                    {
                        // Anzahl hat sich geaendert
                        m_pWHSProfile->UpdateProfil(dlg.m_iLength, dlg.m_iCount);
                        bRedraw = TRUE;
                   }
                }

                if (bRedraw)
                {
                    FillList();
                    // rausschreiben
                    UpdateData(FALSE);
                
                    EnableButtons();
                }
            }
        }
    }

}

void CWhsProfilDlg::OnBnClickedWhsProfilDel()
{
    int iLength;

    if (m_lcWhsProfilList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lcWhsProfilList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        iLength = (int)m_lcWhsProfilList.GetItemData(iItem);
        m_pWHSProfile->DeleteProfil(iLength);
        
        FillList();
        // rausschreiben
        UpdateData(FALSE);
    
        EnableButtons();
    }
}

void CWhsProfilDlg::OnNMClickWhsProfilList(NMHDR *pNMHDR, LRESULT *pResult)
{
    EnableButtons();
    *pResult = 0;
}

void CWhsProfilDlg::OnNMDblclkWhsProfilList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_lcWhsProfilList.GetSelectedCount() > 0)
        OnBnClickedWhsProfilEdit();

    EnableButtons();

    *pResult = 0;
}

// AuswDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TTData.h"
#include "Tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "AuswDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAuswDlg dialog

static const char* szBetoColors[] = 
{
	"braun",
	"gelb"
};

static const char* szSchlossArt[] =
{
    "",
    "PZ",
    "PZ/GS"
};


CAuswDlg::CAuswDlg(CPersistenceManager* persMan, CWnd* pParent /*=NULL*/)
	: CDialog(CAuswDlg::IDD, pParent)
    , m_edPosition(_T("1"))
    , m_edHolzInnen(_T(""))
{
    m_iCurFlCnt = 0;
    m_persMan = persMan;
}

CAuswDlg::	CAuswDlg(CWnd* pParent, char* strKName, CTorDoor* pT, CPersistenceManager* persMan, 
		BOOL modi) : CDialog(CAuswDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAuswDlg)
	m_edKunde = _T("");
	m_edKommission = _T("");
	m_edHoehe = _T("");
	m_edOberHoehe = _T("");
	m_edFuellung = _T("");
	m_edAnzahl = _T("");
	m_edBreite = _T("");
	m_edSehnenHoehe = _T("");
	m_edProfilDicke = _T("");
	m_strBreiteFl1 = _T("");
    m_edPosition = _T("1");
	//}}AFX_DATA_INIT

	m_bModify = modi;
	m_pTor     = pT;
	m_strKundName = strKName;
	m_cTorTyp = ' ';
    m_iCurFlCnt = 0;
    m_persMan = persMan;
}

CAuswDlg::~CAuswDlg()
{
}

void CAuswDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAuswDlg)
    DDX_Control(pDX, IDC_PROFILMASS, m_cbProfilMass);
    DDX_Control(pDX, IDC_400R, m_ck400R);
    DDX_Control(pDX, IDC_BREITE_FL1, m_edBreiteFl1);
    DDX_Control(pDX, IDC_BOGEN, m_ckBogen);
    DDX_Control(pDX, IDC_OKF, m_cckOKF);
    DDX_Control(pDX, IDS_FUELLUNG, m_cedFuellung);
    DDX_Control(pDX, IDC_FUELLUNG, m_cbFuellung);
    DDX_Control(pDX, IDS_TTORTYP, m_cedTorTyp);
    DDX_Control(pDX, IDC_TTORTYP, m_cbTTorTyp);
    DDX_Text(pDX, IDC_KUNDE, m_edKunde);
    DDX_Text(pDX, IDC_KOMMISSION, m_edKommission);
    DDX_Text(pDX, IDC_HOEHE, m_edHoehe);
    DDX_Text(pDX, IDC_OBERHOEHE, m_edOberHoehe);
    DDX_Text(pDX, IDS_FUELLUNG, m_edFuellung);
    DDX_Text(pDX, IDC_ANZAHL, m_edAnzahl);
    DDX_Text(pDX, IDC_BREITE, m_edBreite);
    DDX_Text(pDX, IDC_SEHNENHOEHE, m_edSehnenHoehe);
    DDX_Text(pDX, IDC_PROFILDICKE, m_edProfilDicke);
    DDX_Text(pDX, IDC_BREITE_FL1, m_strBreiteFl1);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_BETO_COLOR, m_ctrlBetoColor);
    DDX_Control(pDX, IDC_KOMMISSION, m_cbKommission);
    DDX_Control(pDX, IDC_KUNDE, m_cbKunde);
    DDX_Control(pDX, IDC_POSITION, m_cbPosition);
    DDX_CBString(pDX, IDC_POSITION, m_edPosition);
    DDV_MaxChars(pDX, m_edPosition, 2);
    DDX_Control(pDX, IDC_HOLZ_INNEN, m_cbHolzInnen);
    DDX_CBString(pDX, IDC_HOLZ_INNEN, m_edHolzInnen);
    DDX_Control(pDX, IDC_HOLZ_AUSSEN, m_cbHolzAussen);
    DDX_Control(pDX, IDC_SCHLOSS_ART, m_ctrlSchlossArt);
    DDX_Control(pDX, IDC_STYROPOR, m_ctrlStyropor);
    DDX_Control(pDX, IDC_TEB_LINKS, m_ctrlTEBLinks);
    DDX_Control(pDX, IDC_TEB_RECHTS, m_ctrlTEBRechts);
    DDX_Control(pDX, IDC_TEB_OBEN, m_ctrlTEBOben);
    DDX_Control(pDX, IDC_TEB_UNTEN, m_ctrlTEBUnten);
    DDX_Control(pDX, IDC_TOR_FEST_LINKS, m_ctrlTorFestLinks);
    DDX_Control(pDX, IDC_TOR_FEST_RECHTS, m_ctrlTorFestRechts);
    DDX_Control(pDX, IDC_Z_WIDTH, m_cbZWidth);
}


BEGIN_MESSAGE_MAP(CAuswDlg, CDialog)
	//{{AFX_MSG_MAP(CAuswDlg)
	ON_CBN_SELCHANGE(IDC_TTORTYP, OnSelchangeTtortyp)
	ON_CBN_SELCHANGE(IDC_FUELLUNG, OnSelchangeFuellung)
	ON_EN_CHANGE(IDS_FUELLUNG, OnChangeFuellungStr)
	ON_BN_CLICKED(IDC_BOGEN, OnBogen)
	ON_EN_CHANGE(IDC_BREITE, OnChangeBreite)
	ON_BN_CLICKED(IDC_400R, OnClicked400R)
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_BETO_COLOR, &CAuswDlg::OnCbnSelchangeBetoColor)
    ON_CBN_SELCHANGE(IDC_HOLZ_AUSSEN, &CAuswDlg::OnCbnSelchangeHolzAussen)
    ON_CBN_SELCHANGE(IDC_HOLZ_INNEN, &CAuswDlg::OnCbnSelchangeHolzInnen)
    ON_BN_CLICKED(IDC_STYROPOR, &CAuswDlg::OnBnClickedStyropor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuswDlg message handlers

BOOL CAuswDlg::OnInitDialog() 
{
    char ctemp[10];

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	TDataScan dataScan;
    CStringArray list;
	CRect rect;
	memset(&rect, 0, sizeof(CRect));

    if (m_bModify == TRUE)
      dataScan.getSpecTT(&list, (tART)m_pTor->Art);
    else 
      dataScan.getAllTT(&list);

    int count = list.GetSize();
    if (count >0)
    {
      for (int i=0; i<count; i++)
           m_cbTTorTyp.AddString(list.GetAt(i));
    }
	list.RemoveAll();

	GetFluegelItems();
	for (int i=0; i<5; i++)
	{
		//wichtig!! Eintrag in Reihenfolge der Fensterart
		//(siehe TTdata.h)!!!
		m_cbOberTeil[i]->AddString("Normal");
		m_cbOberTeil[i]->AddString("z. Öffnen");
		m_cbOberTeil[i]->AddString("i. Fluegel");
		m_cbOberTeil[i]->SetCurSel(0);
     
		m_cbFenster[i]->AddString("Ohne");
		m_cbFenster[i]->AddString("Oberlicht");
		m_cbFenster[i]->AddString("Schiebe");
		m_cbFenster[i]->AddString("Kipp");
		m_cbFenster[i]->AddString("Schiebe Typ2");
		m_cbFenster[i]->SetCurSel(0);

		m_cbGlasart[i]->AddString("VSG");
		m_cbGlasart[i]->AddString("Plexi4");
		m_cbGlasart[i]->AddString("Plexi5");
		m_cbGlasart[i]->AddString("Plexi6");
		m_cbGlasart[i]->AddString("SDP10");
		m_cbGlasart[i]->AddString("SDP16");
		m_cbGlasart[i]->AddString("SDP20");
		m_cbGlasart[i]->AddString("ISO");
		m_cbGlasart[i]->AddString("Klar");
		m_cbGlasart[i]->AddString("Draht");
		m_cbGlasart[i]->AddString("SAN4");
		m_cbGlasart[i]->AddString("SAN5");
		m_cbGlasart[i]->AddString("");

		m_cbSprosse[i]->AddString("");
		m_cbSprosse[i]->AddString("U");
		m_cbSprosse[i]->AddString("RP");
		m_cbSprosse[i]->SetCurSel(0);

	}
    m_cbProfilMass.AddString("70/40");
    m_cbProfilMass.AddString("60/40");
    m_cbProfilMass.SetItemData(0, PD_70_40);
    m_cbProfilMass.SetItemData(1, PD_60_40);
    m_cbProfilMass.SetCurSel(-1);
    m_cbProfilMass.EnableWindow(FALSE);

	for (int i = 0; i < 2; i++)
		m_ctrlBetoColor.AddString(szBetoColors[i]);

	for (int i = 0; i < 11; i++)
		m_cbHolzAussen.AddString(dataScan.getNameForFuellung(i));
	for (int i = 0; i < 11; i++)
		m_cbHolzInnen.AddString(dataScan.getNameForFuellung(i));

	m_ctrlBetoColor.SetCurSel(0);
    for (int i = 0; i < 2; i++)
    {
        if (strstr(m_pTor->strFuellung, szBetoColors[i]) != NULL)
        {
	        m_ctrlBetoColor.SetCurSel(i);
        }
    }

    for (int i = 1; i < 21; i++)
    {
        m_cbPosition.AddString(itoa(i, ctemp, 10));
    }

    // Schlossart
	for (int i = 0; i < 3; i++)
    {
	    m_ctrlSchlossArt.AddString(szSchlossArt[i]);
    }
    m_ctrlSchlossArt.SetCurSel(m_pTor->PzGs);

    // Toreinbau
	for (int teb = TEB_LEER; teb < TEB_MAX; teb++)
    {
	    m_ctrlTEBLinks.AddString(dataScan.getTorEinbau(teb));
	    m_ctrlTEBRechts.AddString(dataScan.getTorEinbau(teb));
	    m_ctrlTEBOben.AddString(dataScan.getTorEinbau(teb));
	    m_ctrlTEBUnten.AddString(dataScan.getTorEinbau(teb));
    }
    m_ctrlTEBLinks.SetCurSel(m_pTor->TorEinbau[TS_LINKS]);
    m_ctrlTEBRechts.SetCurSel(m_pTor->TorEinbau[TS_RECHTS]);
    m_ctrlTEBOben.SetCurSel(m_pTor->TorEinbau[TS_OBEN]);
    m_ctrlTEBUnten.SetCurSel(m_pTor->TorEinbau[TS_UNTEN]);

    // Torfeststellung
	for (int tfs = TFS_LEER; tfs < TFS_MAX; tfs++)
    {
	    m_ctrlTorFestLinks.AddString(dataScan.getTorFeststellung(tfs));
	    m_ctrlTorFestRechts.AddString(dataScan.getTorFeststellung(tfs));
    }
    m_ctrlTorFestLinks.SetCurSel(m_pTor->TorFeststellung[TS_LINKS]);
    m_ctrlTorFestRechts.SetCurSel(m_pTor->TorFeststellung[TS_RECHTS]);

    // Z Staerke
    m_cbZWidth.AddString("");
    m_cbZWidth.AddString("3");
    m_cbZWidth.AddString("4");
    m_cbZWidth.SetCurSel(m_pTor->ZWidth - 2);

	if (m_bModify)
	{
        char kommission[MAX_PATH], pos[MAX_PATH];
		m_edKunde = m_pTor->Kunde;
        m_pTor->GetPositionKommission(kommission, pos);
		m_edKommission = kommission;
        m_edPosition = pos;

		m_cbTTorTyp.SelectString(-1, m_pTor->sArtikel);
		SetFuellung(m_pTor->sArtikel);
		EnableControls(m_pTor->sArtikel);

		m_edFuellung = m_pTor->strFuellung;
		if (m_pTor->Fuellung != -1)
		{
            if (m_pTor->Fuellung > 4000)
            {
			    itoa(m_pTor->Fuellung, ctemp, 10);
			    m_cbFuellung.SelectString(-1, ctemp);
            }
            else
            {
                m_cbHolzAussen.SetCurSel(m_pTor->Fuellung&0x0f);
                m_cbHolzInnen.SetCurSel((m_pTor->Fuellung&0xf0)>>4);
            }
		}

		itoa(m_pTor->StueckZahl, ctemp, 10);
		m_edAnzahl = ctemp;

		itoa(m_pTor->Size.Breite, ctemp, 10); 
		m_edBreite = ctemp;

		itoa(m_pTor->Size.Hoehe, ctemp, 10);
		m_edHoehe = ctemp;

		itoa(m_pTor->Size.ObtHoehe, ctemp, 10);
		m_edOberHoehe = ctemp;

		m_cckOKF.SetCheck(m_pTor->OKF);

		m_ck400R.SetCheck(m_pTor->b400R);

		m_ckBogen.SetCheck(m_pTor->bBogen);
        if (m_pTor->bBogen == TRUE)
        {
		    itoa(m_pTor->Size.SehnenHoehe, ctemp, 10);
		    m_edSehnenHoehe = ctemp;
            GetDlgItem(IDC_SEHNENHOEHE)->EnableWindow(TRUE);
		    itoa(m_pTor->Size.iProfilDicke, ctemp, 10);
		    m_edProfilDicke = ctemp;
            GetDlgItem(IDC_PROFILDICKE)->EnableWindow(TRUE);
        }
        if (m_pTor->b400R == TRUE)
        {
            m_cbProfilMass.EnableWindow(TRUE);
            for (int i=0; i<m_cbProfilMass.GetCount(); i++)
                if (m_cbProfilMass.GetItemData(i) == (DWORD)m_pTor->ProfilMass)
                {
                    m_cbProfilMass.SetCurSel(i);
                    break;
                }
        }
		for (int i=0; i<m_pTor->FluegelAnz; i++)
		{
			CFlParam* pFl = (CFlParam*)m_pTor->FlParam->GetAt(i);
			m_ckGeteilt[i]->SetCheck(pFl->Geteilt);
			m_cbFenster[i]->SetCurSel(pFl->FArt);
			m_cbOberTeil[i]->SetCurSel(pFl->OArt);
			m_cbSprosse[i]->SetCurSel(pFl->Sprosse);
			if (pFl->FArt == OlF || pFl->FArt == KF || pFl->FArt == SF
				|| pFl->FArt == SF2)
				m_cbGlasart[i]->SetCurSel(pFl->GArt);

            if ((i == 0) && (m_pTor->FluegelAnz == 2))
            {
                // 1. Fulegel und Tor
		        itoa(pFl->iBreite, ctemp, 10); 
		        m_strBreiteFl1 = ctemp;
            }
		}

		UpdateData(FALSE);

        m_ctrlStyropor.SetCheck(FuellungMitStyropor());
	}
	else
	{
		CString str;
		str.Empty();
		EnableControls(str);
	}
    for( POSITION pos = m_persMan->GetListKunde().GetHeadPosition(); pos != NULL; )
    {
        m_cbKunde.AddString(m_persMan->GetListKunde().GetNext( pos ));
    }
    for( POSITION pos = m_persMan->GetListKommission().GetHeadPosition(); pos != NULL; )
    {
        m_cbKommission.AddString(m_persMan->GetListKommission().GetNext( pos ));
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAuswDlg::OnSelchangeTtortyp() 
{
	// TODO: Add your control notification handler code here
	CString str;
	char cBuffer[255];
	int iCurSel;

	if (((iCurSel = m_cbTTorTyp.GetCurSel()) != CB_ERR)
		&& (m_cbTTorTyp.GetLBText(iCurSel, cBuffer) != CB_ERR))
	{
		str = cBuffer;
		SetFuellung(str);
		EnableControls(str);
	}
}

static bool newWordPos(char c)
{
    if (c == '-')
        return false;
    else
        return true;
}

static bool insertWord(char* sentence, const char* wordBefore, const char* word)
{
    char* pos;
    int lenBefore = strlen(wordBefore);
    int lenToInsert = strlen(word) + 1;
    if ((pos = strstr(sentence, wordBefore)) != NULL)
    {
        pos += lenBefore;
        int lenEnd = strlen(pos);
        if (newWordPos(*pos))
        {
            memcpy(pos + lenToInsert, pos, lenEnd);
            *pos = ' ';
            pos++;
            memcpy(pos, word, lenToInsert - 1);
            *(pos + lenToInsert - 1 + lenEnd) = 0;
            return true;
        }
    }

    return false;
}

static bool cutoutStringSequence(char* sentence, const char* sequence)
{
    char* pos;
    int lenSentence = strlen(sentence);
    int lenToCut = strlen(sequence);
    if ((pos = strstr(sentence, sequence)) != NULL)
    {
        int lenToEnd = lenSentence - ((pos - sentence) + lenToCut);
        strncpy(pos, pos + lenToCut, lenToEnd);
        *(pos + lenToEnd) = 0;
        return true;
    }
    return false;
}

#define NR_BETO_TEXT 3
static const char betoSearchTexts[NR_BETO_TEXT][50] =
{
    "Beto-Plan-Füllung",
    "Beto-Plan",
    "Beto"
//    "PP-Füllung",
//    "PP"
};

typedef struct _HOLZ_ADD_TEXTS
{
    char searchText[60];
    char addText[60];
} HOLZ_ADD_TEXTS;

#define NR_HOLZ_AUSSEN_TEXT  10
static const HOLZ_ADD_TEXTS holzAussenSearchTexts[NR_HOLZ_AUSSEN_TEXT] =
{
    { "doppelw. Holzfüllung", "außen " },
    { "einw. Holzfüllung", "" },
    { "außen Holz", "" },
    { "doppelw. Holz", "außen " },
    { "doppelw. Holz (2 x 14 mm)", "außen " },
    { "einwandig Holz (1 x 16 mm)", "" },
    { "einwandig Holz (1 x 16 mm)", "" },
    { "m. einw. Holz", "" },
    { "doppelw. Holz", "außen " },
    { "m. einw. Holzfüllung", "" }
    
};

#define NR_HOLZ_INNEN_TEXT  6
static const HOLZ_ADD_TEXTS holzInnenSearchTexts[NR_HOLZ_INNEN_TEXT] =
{
    { "doppelw. Holzfüllung", "innen " },
    { "innen Holz", "" },
    { "doppelw. Holz", "innen " },
    { "doppelw. Holz (2 x 14 mm)", "innen " },
    { "doppelw. Holz", "innen " },
    { "doppelw. Holz", "innen " }    
};

void CAuswDlg::AddBetoColor(const char* strFuellung, const char* szColor)
{
    char szBuf[300];
    
//    m_cedFuellung.GetWindowText(szBuf);
    strncpy(szBuf, strFuellung, sizeof(szBuf));
    
    for (int i = 0; i < NR_BETO_TEXT; i++)
    {
        if (insertWord(szBuf, betoSearchTexts[i], szColor) == true)
        {
	        m_cedFuellung.SetWindowText(szBuf);
            break;
        }
    }

}

const char* szMitStyropor = " m. Styropor";
void CAuswDlg::AddRemoveStyropor(bool add, int aNr)
{
    char szBuf[300];
    bool setText = false;
    
    m_cedFuellung.GetWindowText(szBuf, sizeof(szBuf));
    
    if (add == true)
    {
        strcat(szBuf, szMitStyropor);
        setText = true;
    }
    else
    {
        setText = cutoutStringSequence(szBuf, szMitStyropor);
    }
    if (setText)
    {
        m_cedFuellung.SetWindowText(szBuf);
    }
}

bool CAuswDlg::FuellungMitStyropor(void)
{
    char szBuf[300];
    
    m_cedFuellung.GetWindowText(szBuf, sizeof(szBuf));

    return (strstr(szBuf, szMitStyropor) != NULL);
}

void CAuswDlg::AddHolzArt(char* szBuf, BOOL outside, char* szHolz)
{
    
//    m_cedFuellung.GetWindowText(szBuf);
//    strncpy(szBuf, dataScan.getBezeich(aNr), sizeof(szBuf));
    char szWord[200];

    const HOLZ_ADD_TEXTS* holzAddTexts;
    int Len;
    
    if (outside)
    {
        Len = NR_HOLZ_AUSSEN_TEXT;
        holzAddTexts = holzAussenSearchTexts;
    }
    else
    {
        Len = NR_HOLZ_INNEN_TEXT;
        holzAddTexts = holzInnenSearchTexts;        
    }
    for (int i = 0; i < Len; i++)
    {
        strncpy(szWord, holzAddTexts[i].addText, sizeof(szWord));
        strncat(szWord, szHolz, sizeof(szWord));
        if (insertWord(szBuf, holzAddTexts[i].searchText, szWord) == true)
        {
	        m_cedFuellung.SetWindowText(szBuf);
            break;
        }
    }

}

void CAuswDlg::OnCbnSelchangeBetoColor()
{
    OnSelchangeCustomFuellung();
}

void CAuswDlg::OnSelchangeFuellung() 
{
	TDataScan dataScan;
    int aNr;
	char cBuffer[255];
	int iCurSel;

	if (((iCurSel = m_cbFuellung.GetCurSel()) != CB_ERR)
		&& (m_cbFuellung.GetLBText(iCurSel, cBuffer) != CB_ERR))
    {
		tFUELLUNG outside, inside;

        m_cbHolzAussen.SetCurSel(-1);
        m_cbHolzInnen.SetCurSel(-1);

		aNr = atoi(cBuffer);
		m_cedFuellung.SetWindowText(dataScan.getBezeich(aNr));
		dataScan.getFuellung(aNr, &outside, &inside);
		if ((outside == F_BETOPLAN || inside == F_BETOPLAN) && (strstr(dataScan.getBezeich(aNr), " PP") == NULL))
		{
            AddBetoColor(dataScan.getBezeich(aNr), szBetoColors[m_ctrlBetoColor.GetCurSel()]);
		}
        AddRemoveStyropor(m_ctrlStyropor.GetCheck() == 1, 0);
	}
}

void CAuswDlg::OnSelchangeCustomFuellung() 
{
	TDataScan dataScan;
	char cBuffer[255];

    cBuffer[0] = 0;
    int fuellungAussen = m_cbHolzAussen.GetCurSel();
    int fuellungInnen = m_cbHolzInnen.GetCurSel();
    if (fuellungAussen > 0 || fuellungInnen > 0)
    {
        m_cbFuellung.SetCurSel(-1);
        if (fuellungAussen > 0)
        {
            strncat(cBuffer, "bandseitig ", strlen("bandseitig "));
            strcat(cBuffer, dataScan.getNameForFuellung(fuellungAussen));
        }
        if (fuellungInnen > 0)
        {
            if (strlen(cBuffer) > 0)
            {
                strcat(cBuffer, ", ");
            }
            strncat(cBuffer, "bandgegenseitig ", strlen("bandgegenseitig "));
            strcat(cBuffer, dataScan.getNameForFuellung(fuellungInnen));
        }
        m_cedFuellung.SetWindowText(cBuffer);

        AddBetoColor(cBuffer, szBetoColors[m_ctrlBetoColor.GetCurSel()]);
        AddRemoveStyropor(m_ctrlStyropor.GetCheck() == 1, 0);
    }
    else
    {
        m_cedFuellung.SetWindowText(cBuffer);
    }
}

void CAuswDlg::OnChangeFuellungStr() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
#if 0  //?? Fuellung gilt weiterhin, auch wenn geändert wurde
	m_cbFuellung.SetCurSel(-1);	
#endif // 0
}

void CAuswDlg::EnableControls(CString strTyp)
{
	TDataScan dataScan;

	if (strTyp.IsEmpty() == FALSE)
	{
		CTorDoor* pTtemp = new CTorDoor();
		dataScan.getTTDaten(pTtemp, strTyp);

        m_iCurFlCnt = pTtemp->FluegelAnz;

		for (int i=0; i<5; i++)
		{
			BOOL enable = (i<pTtemp->FluegelAnz);
			m_ckGeteilt[i]->EnableWindow(enable);
			m_cbFenster[i]->EnableWindow(enable);
			m_cbGlasart[i]->EnableWindow(enable);
			m_cbOberTeil[i]->EnableWindow(enable);
            if (pTtemp->Typ == 4)
            {
                // Typ 400
                enable = FALSE;
    			m_cbSprosse[i]->SetCurSel(0);
            }
            else
            {
                // Typ 600 + 800
                if (m_cbSprosse[i]->GetCurSel() == 0)
    			    m_cbSprosse[i]->SetCurSel(1);  // default: U-Sprosse wählen
            }
			m_cbSprosse[i]->EnableWindow(enable);
		}
        m_edBreiteFl1.EnableWindow(pTtemp->FluegelAnz == 2);

        // Typ 400R
        m_ck400R.EnableWindow(pTtemp->Typ == 4);
        if (pTtemp->Typ != 4)
            m_ck400R.SetCheck(0);

		delete pTtemp;
	}
	else
	{
        m_iCurFlCnt = 0;
		for (int i=0; i<5; i++)
		{
			m_ckGeteilt[i]->EnableWindow(FALSE);
			m_cbFenster[i]->EnableWindow(FALSE);
			m_cbGlasart[i]->EnableWindow(FALSE);
			m_cbOberTeil[i]->EnableWindow(FALSE);
			m_cbSprosse[i]->EnableWindow(FALSE);
            m_edBreiteFl1.EnableWindow(FALSE);
		}
	}
}

void CAuswDlg::SetFuellung(CString str)
{
	TDataScan dataScan;

	int aNr = atoi(str);
	m_cedTorTyp.SetWindowText(dataScan.getBezeich(aNr));


	if (m_cTorTyp != str[0])
    {
		m_cTorTyp = str[0];
		CStringArray* list = new CStringArray;
		dataScan.getListArtNr(list, FUELLUNG, str);

		int count = list->GetSize();
		if (count >0)
		{
			m_cbFuellung.ResetContent();
			for (int i=0; i<count; i++)
				m_cbFuellung.AddString(list->GetAt(i));
		}
		m_cedFuellung.SetWindowText("");
		list->RemoveAll();
		delete list;
    }
}

void CAuswDlg::GetFluegelItems()
{
	for (int i=0; i<5; i++)
	{
		m_ckGeteilt[i] = (CButton*)GetDlgItem(IDC_GETEILT1+i*2);
		m_cbOberTeil[i] = (CComboBox*)GetDlgItem(IDC_GETEILT1+1+i*2);
		m_cbGlasart[i] = (CComboBox*)GetDlgItem(IDC_GLASART1+i);
		m_cbFenster[i] = (CComboBox*)GetDlgItem(IDC_FENSTER1+i);
		m_cbSprosse[i] = (CComboBox*)GetDlgItem(IDC_SPROSSE1+i);
	}
}

#define BF_CHECKED 1
void CAuswDlg::OnOK( )
{
	TDataScan dataScan;
	CString str;
	char cBuffer[255];
	int iCurSel;
    int iBreiteFl1 = 0;
	BOOL fensterOK = TRUE, obertOK = TRUE;

	UpdateData(TRUE);

	GetFluegelItems();

	strcpy(m_pTor->Kunde, m_edKunde);

    // Kommission + Position
	strcpy(m_pTor->Kommission, m_edKommission);
    strcat(m_pTor->Kommission, POSITION_SIGN);
    strcat(m_pTor->Kommission, m_edPosition);


	if (((iCurSel = m_cbTTorTyp.GetCurSel()) != CB_ERR)
		&& (m_cbTTorTyp.GetLBText(iCurSel, cBuffer) != CB_ERR))
	{
		CString str = cBuffer;
		dataScan.getTTDaten(m_pTor, str);
		strcpy(m_pTor->sArtikel, str);
	}
	else
	{
		MessageBox("Bitte Tor-Typ auswählen");
		return;
	}

#if 0       // immer Text von Fuellung merken
	if (((iCurSel = m_cbFuellung.GetCurSel()) != CB_ERR)
		&& (m_cbFuellung.GetLBText(iCurSel, cBuffer) != CB_ERR))
		m_pTor->Fuellung = atoi(cBuffer);
	else
	{
		m_pTor->Fuellung = -1;
		strcpy(m_pTor->strFuellung, m_edFuellung);
	}
#else
    strcpy(m_pTor->strFuellung, m_edFuellung);
	if (((iCurSel = m_cbFuellung.GetCurSel()) != CB_ERR)
		&& (m_cbFuellung.GetLBText(iCurSel, cBuffer) != CB_ERR))
		m_pTor->Fuellung = atoi(cBuffer);
	else
    {
        int fuellungAussen = m_cbHolzAussen.GetCurSel();
        int fuellungInnen = m_cbHolzInnen.GetCurSel();
        if (fuellungAussen > 0 || fuellungInnen > 0)
        {
		    m_pTor->Fuellung = 0;
            if (fuellungAussen > 0)
            {
                m_pTor->Fuellung |= fuellungAussen;
            }
            if (fuellungInnen > 0)
            {
                m_pTor->Fuellung |= (fuellungInnen<<4);
            }
        }
        else
		    m_pTor->Fuellung = -1;
    }
#endif   //0

	m_pTor->StueckZahl = atoi(m_edAnzahl);
	m_pTor->Size.Hoehe = atoi(m_edHoehe);
	m_pTor->Size.Breite = atoi(m_edBreite);
	m_pTor->Size.ObtHoehe = atoi(m_edOberHoehe);
    
    if (m_pTor->StueckZahl == 0)
    {
        MessageBox("Bitte Stückzahl eingeben");
        return;
    }
    if (m_pTor->Size.Hoehe == 0 || m_pTor->Size.Breite == 0)
    {
        MessageBox("Bitte Maße eingeben");
        return;
    }
	m_pTor->OKF = (m_cckOKF.GetCheck() == BF_CHECKED);
    if (m_pTor->Typ == 4)
	    m_pTor->b400R = (m_ck400R.GetCheck() == BF_CHECKED);
    else
        m_pTor->b400R = FALSE;
    if (m_pTor->b400R == TRUE)
    {
        m_pTor->ProfilMass = PD_70_40;  // default
        int iSel = m_cbProfilMass.GetCurSel();
        if (iSel != -1)
            m_pTor->ProfilMass = (tProfilMass)m_cbProfilMass.GetItemData(iSel);
    }

    m_pTor->bBogen = (m_ckBogen.GetCheck() == BF_CHECKED);
    if (m_pTor->bBogen == TRUE)
    {
    	m_pTor->Size.SehnenHoehe = atoi(m_edSehnenHoehe);
    	m_pTor->Size.iProfilDicke = atoi(m_edProfilDicke);
        if (m_pTor->Size.SehnenHoehe == 0)
        {
            MessageBox("Bitte Sehnenhöhe eingeben");
            return;
        }
        if (m_pTor->Size.iProfilDicke == 0)
        {
            MessageBox("Bitte Profildicke eingeben");
            return;
        }
    }
    else
    {
        m_pTor->Size.SehnenHoehe = 0;
        m_pTor->Size.iProfilDicke = 0;
    }

    m_pTor->PzGs = (tPzGs)m_ctrlSchlossArt.GetCurSel();

    // Toreinbau
    m_pTor->TorEinbau[TS_LINKS]  = (tTorEinbau)m_ctrlTEBLinks.GetCurSel();
    m_pTor->TorEinbau[TS_RECHTS] = (tTorEinbau)m_ctrlTEBRechts.GetCurSel();
    m_pTor->TorEinbau[TS_OBEN]   = (tTorEinbau)m_ctrlTEBOben.GetCurSel();
    m_pTor->TorEinbau[TS_UNTEN]  = (tTorEinbau)m_ctrlTEBUnten.GetCurSel();

    // Torfeststellung
    m_pTor->TorFeststellung[TS_LINKS]  = (tTorFeststellung)m_ctrlTorFestLinks.GetCurSel();
    m_pTor->TorFeststellung[TS_RECHTS] = (tTorFeststellung)m_ctrlTorFestRechts.GetCurSel();

    // Z Staerke
    m_pTor->ZWidth = m_cbZWidth.GetCurSel() + 2;

	for (int i=0; i<m_pTor->FluegelAnz; i++)
	{
		CFlParam* FlP;
		if (m_bModify == FALSE)
		{
			FlP = new CFlParam;
			FlP->Geteilt = (m_ckGeteilt[i]->GetCheck() == BF_CHECKED);
			FlP->FArt = m_cbFenster[i]->GetCurSel();
			FlP->OArt = m_cbOberTeil[i]->GetCurSel();
			FlP->Sprosse = (tSprosse)m_cbSprosse[i]->GetCurSel();
            if (m_pTor->Typ != 4 && FlP->Sprosse == NA)
                FlP->Sprosse = U;       // auf Default U setzen
			if (FlP->FArt == OlF || FlP->FArt == KF || FlP->FArt == SF
				|| FlP->FArt == SF2)
				FlP->GArt = m_cbGlasart[i]->GetCurSel();
			FlP->ObtHoehe = m_pTor->Size.ObtHoehe;
		}
		else
		{
			FlP = (CFlParam*)m_pTor->FlParam->GetAt(i);
			FlP->Geteilt = (m_ckGeteilt[i]->GetCheck() == BF_CHECKED);
			FlP->FArt = m_cbFenster[i]->GetCurSel();
			FlP->OArt = m_cbOberTeil[i]->GetCurSel();
			FlP->Sprosse = (tSprosse)m_cbSprosse[i]->GetCurSel();
            if (m_pTor->Typ != 4 && FlP->Sprosse == NA)
                FlP->Sprosse = U;       // auf Default U setzen
			if (FlP->FArt == OlF || FlP->FArt == KF || FlP->FArt == SF
				|| FlP->FArt == SF2)
				FlP->GArt = m_cbGlasart[i]->GetCurSel();
			FlP->ObtHoehe = m_pTor->Size.ObtHoehe;
		}

        FlP->iBreite = 10*m_pTor->Size.Breite/m_pTor->FluegelAnz;
        if ((i == 0) && (m_pTor->FluegelAnz == 2))
        {
            // 1.Fluegel und Tor
            if (m_strBreiteFl1.IsEmpty() == FALSE)
            {   
                FlP->iBreite = atoi(m_strBreiteFl1);
                iBreiteFl1 = FlP->iBreite; 
            }
        }
        if ((i == 1) && (m_pTor->FluegelAnz == 2))
        {
            // 2.Fluegel und Tor
            if ((m_strBreiteFl1.IsEmpty()) == FALSE && (iBreiteFl1 != 0))
            {   
                // Breite ist Tor-Breite - Breite Fl1
                FlP->iBreite = m_pTor->Size.Breite*10 - iBreiteFl1;
//                FlP->iBreite *= 10;
            }
        }

		if (m_bModify == FALSE)
		{
			if (fensterOK && obertOK)
				m_pTor->FlParam->Add(FlP);
			else
				delete FlP;
		}
	}

    m_persMan->AddKundeToList(m_edKunde);       
    m_persMan->AddKommssionToList(m_edKommission);       

	m_pTor->updateValues();
	CDialog::OnOK();
}


void CAuswDlg::OnBogen() 
{
    if (m_ckBogen.GetCheck() == BF_CHECKED)
    {
        GetDlgItem(IDC_SEHNENHOEHE)->EnableWindow(TRUE);
        GetDlgItem(IDC_PROFILDICKE)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_SEHNENHOEHE)->EnableWindow(FALSE);
        GetDlgItem(IDC_PROFILDICKE)->EnableWindow(FALSE);
    }
}

void CAuswDlg::OnChangeBreite() 
{
#if 0  //?? macht alles zu kompliziert
    int iFBreite;
    char szFBreite[20];

    if (m_iCurFlCnt > 0)
    {
        UpdateData(TRUE);

        iFBreite = atoi(m_edBreite);
        iFBreite /= m_iCurFlCnt;
        itoa(iFBreite, szFBreite, 10);
        m_strBreiteFl1 = szFBreite;

        UpdateData(FALSE);
    }
#endif

}

void CAuswDlg::OnClicked400R() 
{
    if (m_ck400R.GetCheck() == 0)
    {
        // unchecked
        m_cbProfilMass.SetCurSel(-1);
        m_cbProfilMass.EnableWindow(FALSE);
    }
    else
    {
        tProfilMass pm;
        m_cbProfilMass.EnableWindow(TRUE);
        if (m_pTor->b400R == TRUE)
        {
            pm = m_pTor->ProfilMass;
        }
        else
        {
            TDataScan ds;
		    CTorDoor tt;
            int iCurSel;
            char szBuf[100];

	        if (((iCurSel = m_cbTTorTyp.GetCurSel()) != CB_ERR)
		        && (m_cbTTorTyp.GetLBText(iCurSel, szBuf) != CB_ERR))
	        {
		        ds.getTTDaten(&tt, szBuf);

                if ((tt.Art == ATUER) || (tt.Art == STUER))
                    pm = PD_60_40;
                else
                    pm = PD_70_40;
	        }
        }
        for (int i=0; i<m_cbProfilMass.GetCount(); i++)
            if (m_cbProfilMass.GetItemData(i) == (DWORD)pm)
            {
                m_cbProfilMass.SetCurSel(i);
                break;
            }
    }
}

void CAuswDlg::OnCbnSelchangeHolzAussen()
{
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
    OnSelchangeCustomFuellung();
}

void CAuswDlg::OnCbnSelchangeHolzInnen()
{
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
    OnSelchangeCustomFuellung();
}

void CAuswDlg::OnBnClickedStyropor()
{
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
    AddRemoveStyropor(m_ctrlStyropor.GetCheck() == 1, 0);
}

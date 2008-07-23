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


CAuswDlg::CAuswDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuswDlg::IDD, pParent)
{
}

CAuswDlg::	CAuswDlg(CWnd* pParent, char* strKName, CTorDoor* pT, 
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
	//}}AFX_DATA_INIT

	m_bModify = modi;
	m_pTor     = pT;
	m_strKundName = strKName;
	m_cTorTyp = ' ';
}

CAuswDlg::~CAuswDlg()
{
}

void CAuswDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAuswDlg)
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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAuswDlg, CDialog)
	//{{AFX_MSG_MAP(CAuswDlg)
	ON_CBN_SELCHANGE(IDC_TTORTYP, OnSelchangeTtortyp)
	ON_CBN_SELCHANGE(IDC_FUELLUNG, OnSelchangeFuellung)
	ON_EN_CHANGE(IDS_FUELLUNG, OnChangeFuellungStr)
	ON_BN_CLICKED(IDC_BOGEN, OnBogen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuswDlg message handlers

BOOL CAuswDlg::OnInitDialog() 
{
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

		m_cbGlasart[i]->AddString("Plexi4");
		m_cbGlasart[i]->AddString("Plexi5");
		m_cbGlasart[i]->AddString("SDP10");
		m_cbGlasart[i]->AddString("SDP16");
		m_cbGlasart[i]->AddString("SDP20");
		m_cbGlasart[i]->AddString("ISO");
		m_cbGlasart[i]->AddString("Klar");
		m_cbGlasart[i]->AddString("Draht");
		m_cbGlasart[i]->AddString("SAN4");
		m_cbGlasart[i]->AddString("SAN5");
		m_cbGlasart[i]->AddString("");
	}

	if (m_bModify)
	{

		char ctemp[10];
		m_edKunde = m_pTor->Kunde;
		m_edKommission = m_pTor->Kommission;

		m_cbTTorTyp.SelectString(-1, m_pTor->sArtikel);
		SetFuellung(m_pTor->sArtikel);
		EnableControls(m_pTor->sArtikel);

		if (m_pTor->Fuellung != -1)
		{
			itoa(m_pTor->Fuellung, ctemp, 10);
			m_cbFuellung.SelectString(-1, ctemp);
			m_edFuellung = dataScan.getBezeich(m_pTor->Fuellung);
		}
		else
			m_edFuellung = m_pTor->strFuellung;

		itoa(m_pTor->StueckZahl, ctemp, 10);
		m_edAnzahl = ctemp;

		itoa(m_pTor->Size.Breite, ctemp, 10); 
		m_edBreite = ctemp;

		itoa(m_pTor->Size.Hoehe, ctemp, 10);
		m_edHoehe = ctemp;

		itoa(m_pTor->Size.ObtHoehe, ctemp, 10);
		m_edOberHoehe = ctemp;

		m_cckOKF.SetCheck(m_pTor->OKF);

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

		for (int i=0; i<m_pTor->FluegelAnz; i++)
		{
			CFlParam* pFl = (CFlParam*)m_pTor->FlParam->GetAt(i);
			m_ckGeteilt[i]->SetCheck(pFl->Geteilt);
			m_cbFenster[i]->SetCurSel(pFl->FArt);
			m_cbOberTeil[i]->SetCurSel(pFl->OArt);
			if (pFl->FArt == OlF || pFl->FArt == KF || pFl->FArt == SF
				|| pFl->FArt == SF2)
				m_cbGlasart[i]->SetCurSel(pFl->GArt);
		}

		UpdateData(FALSE);

	}
	else
	{
		CString str;
		str.Empty();
		EnableControls(str);
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

void CAuswDlg::OnSelchangeFuellung() 
{
	// TODO: Add your control notification handler code here
	TDataScan dataScan;
    int aNr;
	char cBuffer[255];
	int iCurSel;

	if (((iCurSel = m_cbFuellung.GetCurSel()) != CB_ERR)
		&& (m_cbFuellung.GetLBText(iCurSel, cBuffer) != CB_ERR))
    {
		aNr = atoi(cBuffer);
		m_cedFuellung.SetWindowText(dataScan.getBezeich(aNr));
	}
}

void CAuswDlg::OnChangeFuellungStr() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_cbFuellung.SetCurSel(-1);	
}

void CAuswDlg::EnableControls(CString strTyp)
{
	TDataScan dataScan;

	if (strTyp.IsEmpty() == FALSE)
	{
		CTorDoor* pTtemp = new CTorDoor();
		dataScan.getTTDaten(pTtemp, strTyp);

		for (int i=0; i<5; i++)
		{
			BOOL enable = (i<pTtemp->FluegelAnz);
			m_ckGeteilt[i]->EnableWindow(enable);
			m_cbFenster[i]->EnableWindow(enable);
			m_cbGlasart[i]->EnableWindow(enable);
			m_cbOberTeil[i]->EnableWindow(enable);
		}
		delete pTtemp;
	}
	else
	{
		for (int i=0; i<5; i++)
		{
			m_ckGeteilt[i]->EnableWindow(FALSE);
			m_cbFenster[i]->EnableWindow(FALSE);
			m_cbGlasart[i]->EnableWindow(FALSE);
			m_cbOberTeil[i]->EnableWindow(FALSE);
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
	}
}

#define BF_CHECKED 1
void CAuswDlg::OnOK( )
{
	TDataScan dataScan;
	CString str;
	char cBuffer[255];
	int iCurSel;
	BOOL fensterOK = TRUE, obertOK = TRUE;

	UpdateData(TRUE);

	GetFluegelItems();

	strcpy(m_pTor->Kunde, m_edKunde);

	strcpy(m_pTor->Kommission, m_edKommission);

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


	if (((iCurSel = m_cbFuellung.GetCurSel()) != CB_ERR)
		&& (m_cbFuellung.GetLBText(iCurSel, cBuffer) != CB_ERR))
		m_pTor->Fuellung = atoi(cBuffer);
	else
	{
		m_pTor->Fuellung = -1;
		strcpy(m_pTor->strFuellung, m_edFuellung);
	}

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

	for (int i=0; i<m_pTor->FluegelAnz; i++)
	{
		CFlParam* FlP;
		if (m_bModify == FALSE)
		{
			FlP = new CFlParam;
			FlP->Geteilt = (m_ckGeteilt[i]->GetCheck() == BF_CHECKED);
			FlP->FArt = m_cbFenster[i]->GetCurSel();
			FlP->OArt = m_cbOberTeil[i]->GetCurSel();
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
			if (FlP->FArt == OlF || FlP->FArt == KF || FlP->FArt == SF
				|| FlP->FArt == SF2)
				FlP->GArt = m_cbGlasart[i]->GetCurSel();
			FlP->ObtHoehe = m_pTor->Size.ObtHoehe;
		}
/*
if ((FlP->OArt != LEER  || FlP->FArt != KEIN ||
FlP->Geteilt == TRUE) && FlP->ObtHoehe == 0)
{
  obertOK = FALSE;
  MessageBox(HWindow, "Bitte Oberteil-Höhe eingeben!", "", MB_OK);
}
if (FlP->FArt != KEIN && FlP->GArt < 0)
{
  fensterOK = FALSE;
  MessageBox(HWindow, "Bitte Glasart auswählen!", "", MB_OK);
}
*/
		if (m_bModify == FALSE)
		{
			if (fensterOK && obertOK)
				m_pTor->FlParam->Add(FlP);
			else
				delete FlP;
		}
	}

	m_pTor->updateValues();
	CDialog::OnOK();
}



void CAuswDlg::OnBogen() 
{
	// TODO: Add your control notification handler code here
    if (m_ckBogen.GetCheck() == BF_CHECKED)
    {
        GetDlgItem(IDC_SEHNENHOEHE)->EnableWindow(TRUE);
        GetDlgItem(IDC_PROFILDICKE)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_SEHNENHOEHE)->EnableWindow(TRUE);
        GetDlgItem(IDC_PROFILDICKE)->EnableWindow(FALSE);
    }
}

// PS312TestDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "..\Node.h"
#include "..\ZuschnittManager.h"
#include "..\ttmain.h"
//#include "..\HoleProfileDoc.h"
//#include "..\HoleProfilEditDlg.h"
#include "PS312DSetEditDlg.h"
#include "PS312TestDlg.h"
#include "sim\PS312Simul.h"
#include "ComSetDlg.h"


// CPS312TestDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CPS312TestDlg, CDialog)

CPS312TestDlg::CPS312TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPS312TestDlg::IDD, pParent)
    , m_strPS312Status(_T(""))
    , m_iPs312Cmd(128)
    , m_iPS312Level(20)
    , m_iPS312ParamNr(0)
    , m_iPS312ParamVal(0)
    , m_iProgNr(5)
{
    m_strCom = _T("COM1");
//    m_strComNr = m_strCom;
    m_pContainer = new CPtrArray();
}

CPS312TestDlg::~CPS312TestDlg()
{
    for (int i=0; i<m_pContainer->GetSize(); i++)
    {
        PS312_DATA_SET* pProfil = (PS312_DATA_SET*)m_pContainer->GetAt(i);
        if (pProfil)
            delete pProfil;
    }

    delete m_pContainer;
}

void CPS312TestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_TEST_PS312_STATUS, m_strPS312Status);
    DDX_Control(pDX, IDC_TEST_PS312_STATUS_AKT, m_btStatusAkt);
    DDX_Text(pDX, IDC_TEST_PS312_CMD, m_iPs312Cmd);
    DDX_Text(pDX, IDC_TEST_PS312_LEVEL, m_iPS312Level);
    DDX_Text(pDX, IDC_TEST_PS312_PARAM_NR, m_iPS312ParamNr);
    DDV_MinMaxInt(pDX, m_iPS312ParamNr, 0, 99);
    //??    DDV_MinMaxInt(pDX, m_iPS312Level, 20, 21);
    DDX_Text(pDX, IDC_TEST_PS312_PARAM_VAL, m_iPS312ParamVal);
    DDX_Control(pDX, IDC_TEST_PS312_KOMM_TRACE, m_edPS312KommTrace);
    DDX_Control(pDX, IDC_TEST_PS312_PROG_LIST, m_lcProgList);
    DDX_Control(pDX, IDC_TEST_PS312_CHANGE_PROFIL, m_btProfilEdit);
    DDX_Control(pDX, IDC_TEST_PS312_DEL_PROFIL, m_btProfilDel);
    DDX_Text(pDX, IDC_TEST_PS312_PROG_NR, m_iProgNr);
	DDV_MinMaxInt(pDX, m_iProgNr, 1, 99);
}


BEGIN_MESSAGE_MAP(CPS312TestDlg, CDialog)
    ON_BN_CLICKED(IDC_TEST_PS312_STATUS_AKT, &CPS312TestDlg::OnClickedTestPs312StatusAkt)
    ON_BN_CLICKED(IDC_TEST_PS312_CMD_SEND, &CPS312TestDlg::OnClickedTestPs312CmdSend)
    ON_BN_CLICKED(IDC_TEST_PS312_PARAM_READ, &CPS312TestDlg::OnClickedTestPs312ParamRead)
    ON_BN_CLICKED(IDC_TEST_PS312_PARAM_SET, &CPS312TestDlg::OnClickedTestPs312ParamSet)
    ON_BN_CLICKED(IDC_TEST_PS312_COM_SETT, &CPS312TestDlg::OnClickedTestPs312ComSett)
    ON_BN_CLICKED(IDC_TEST_PS312_NEW_PROFIL, &CPS312TestDlg::OnClickedNewProfil)
    ON_BN_CLICKED(IDC_TEST_PS312_CHANGE_PROFIL, &CPS312TestDlg::OnClickedChangeProfil)
    ON_BN_CLICKED(IDC_TEST_PS312_DEL_PROFIL, &CPS312TestDlg::OnClickedDelProfil)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_TEST_PS312_PROG_LIST, &CPS312TestDlg::OnLvnItemActivateProgList)
    ON_NOTIFY(NM_DBLCLK, IDC_TEST_PS312_PROG_LIST, &CPS312TestDlg::OnDblclkProgList)
    ON_NOTIFY(NM_CLICK, IDC_TEST_PS312_PROG_LIST, &CPS312TestDlg::OnClickProgList)
    ON_BN_CLICKED(IDC_TEST_PS312_PROG_READ, &CPS312TestDlg::OnClickedProgRead)
    ON_BN_CLICKED(IDC_TEST_PS312_PROG_SEND, &CPS312TestDlg::OnClickedProgSend)
    ON_BN_CLICKED(IDC_TEST_PS312_PROG_START, &CPS312TestDlg::OnClickedProgStart)
END_MESSAGE_MAP()


// CPS312TestDlg-Meldungshandler

BOOL CPS312TestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Verbindung zum Gerät oder Simulation aufnehmen
    // erstmal immer von Simul ausgehen

    if (m_Com.AttachToDevice(PS312_SIMUL_PIPE) == false)
    {
        // keine Pipe, Com1 aufmachen
        int i=1;
        while (m_strCom != _T("COM5"))
        {
            if (m_Com.AttachToDevice(m_strCom.GetBuffer(m_strCom.GetLength())) == true)
            {
                // Fehlermeldung, alle Buttons deaktivieren
                //AfxMessageBox(
                DCB dcb;
                m_Com.GetCommDcb(&dcb);
                // Einstellungen: 7 Bit, 9600 B, 1 Stop, 1 Start, 1 Parity(even), kein Handshake
                dcb.BaudRate   = CBR_9600;
                dcb.fParity    = TRUE;
                dcb.ByteSize   = 7;
                dcb.Parity     = EVENPARITY;
                dcb.StopBits   = ONESTOPBIT;
                dcb.fOutxCtsFlow = FALSE;
                dcb.fOutxDsrFlow = FALSE;
                dcb.fDtrControl  = DTR_CONTROL_DISABLE;
                dcb.fDsrSensitivity  = FALSE;
                dcb.fOutX            = FALSE;
                dcb.fInX             = FALSE;
                dcb.fRtsControl      = RTS_CONTROL_DISABLE;
                m_Com.SetCommDcb(&dcb);
                break;
            }
            else
            {
                char szBuf[100];
                i++;
                ShowError("COMx kann nicht geöffnet werden!");
                m_strCom = _T("COM") + CString(itoa(i, szBuf, 10));
            }
        }

    }

    m_edPS312KommTrace.FmtLines(TRUE);
    m_Com.SetTraceWindow(&m_edPS312KommTrace);

    m_PS312.SetCom(&m_Com);

#ifdef _DEBUG  //?? wieder raus
//    m_PS312.SetTimeout(-1);
#endif // _DEBUG
    // Test ob Verbindung klappt
    //?? OnClickedTestPs312StatusAkt();
    
    InitProfilList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CPS312TestDlg::OnClickedTestPs312StatusAkt()
{
    int iStatus, iRet;
    char szBuf[20];

    m_strPS312Status = _T("Status anforden...");
    UpdateData(FALSE);
    m_btStatusAkt.EnableWindow(FALSE);
    BeginWaitCursor();
    if ((iRet = m_PS312.GetStatus(PS312_LEVEL_AXIS1, &iStatus)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler ausgeben und im Statusfenster anzeigen
        m_strPS312Status = _T("keine Verbindung");
        ShowError(iRet);
    }
    else
    {
        // in hex String wandeln
        sprintf(szBuf, "0x%03x", iStatus);
        m_strPS312Status = szBuf;
    }
    EndWaitCursor();
    m_btStatusAkt.EnableWindow(TRUE);
    UpdateData(FALSE);


}

void CPS312TestDlg::OnClickedTestPs312CmdSend()
{
    int iRet;

    UpdateData(TRUE);
//    m_btStatusAkt.EnableWindow(FALSE);
    BeginWaitCursor();
    if ((iRet = m_PS312.SetCmd(PS312_LEVEL_AXIS1, m_iPs312Cmd)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler ausgeben und im Statusfenster anzeigen
        ShowError(iRet);
    }
    else
    {
        // in hex String wandeln
    }
    EndWaitCursor();
    
    // Status holen
    OnClickedTestPs312StatusAkt();
}

void CPS312TestDlg::OnClickedTestPs312ParamRead()
{
    int iRet;

    UpdateData(TRUE);
    if ((iRet = m_PS312.GetParam((byte)m_iPS312Level, m_iPS312ParamNr, &m_iPS312ParamVal)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler ausgeben und im Statusfenster anzeigen
        ShowError(iRet);
    }
    else
    {
        // ok
        UpdateData(FALSE);
    }

}

void CPS312TestDlg::OnClickedTestPs312ParamSet()
{
    int iRet;

    UpdateData(TRUE);
    if ((iRet = m_PS312.SetParam((byte)m_iPS312Level, m_iPS312ParamNr, m_iPS312ParamVal)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler ausgeben und im Statusfenster anzeigen
        ShowError(iRet);
    }
    else
    {
        // ok
    }    
}

void CPS312TestDlg::ShowError(int iError)
{
    char szBuf[MAX_PATH];

    sprintf(szBuf, "Fehler: %s", m_PS312.GetErrorString(iError));
    AfxMessageBox(szBuf, MB_ICONSTOP);
}

void CPS312TestDlg::ShowError(char* szError)
{
    char szBuf[MAX_PATH];

    sprintf(szBuf, "Fehler: %s", szError);
    AfxMessageBox(szBuf, MB_ICONSTOP);
}

void CPS312TestDlg::OnClickedTestPs312ComSett()
{
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
#if 0
    CComSetDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        CString strSet;
        dlg.GetComSettings(strSet);
    }
#else
    COMMCONFIG cc;
    DCB dcb;
    dcb.DCBlength = sizeof(dcb);
    m_Com.GetCommDcb(&dcb);
    cc.dcb = dcb;
    if (CommConfigDialog(m_strCom.GetBuffer(m_strCom.GetLength()), GetSafeHwnd(), &cc) == FALSE)
    {
        ShowError("Einstellungen nicht möglich!");
        return;
    }
    dcb = cc.dcb;
    m_Com.SetCommDcb(&dcb);
#endif // 0
}

void CPS312TestDlg::InitProfilList(void)
{
    // set ex styles
    ListView_SetExtendedListViewStyleEx(
        m_lcProgList.GetSafeHwnd(),
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE,
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE
    );

    // create columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;

	lvc.pszText = (LPTSTR) "Anzahl";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 20;
    lvc.iImage = -1;
	m_lcProgList.InsertColumn(0,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;

	lvc.pszText = (LPTSTR) "Länge";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 60;
    lvc.iSubItem = 1;
	m_lcProgList.InsertColumn(1,&lvc);

	lvc.pszText = (LPTSTR) "Mass";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 60;
    lvc.iSubItem = -1;
	m_lcProgList.InsertColumn(2,&lvc);

    UpdateButtons();
}

void CPS312TestDlg::FillList(void)
{
    m_lcProgList.DeleteAllItems();
    for (int i=0; i<m_pContainer->GetSize(); i++)
    {
        PS312_DATA_SET* pProfil = (PS312_DATA_SET*)m_pContainer->GetAt(i);
        if (pProfil)
            InsertProfil(pProfil);
    }
    UpdateButtons();
}


void CPS312TestDlg::InsertProfil(PS312_DATA_SET* pProfil)
{
	LV_ITEM lvItem;
    char szBuf[20];
    int iItem = m_lcProgList.GetItemCount();

    // Anzahl
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
    sprintf(szBuf, "%ld", pProfil->iCount);
	lvItem.pszText = (LPTSTR) szBuf;
	lvItem.iImage = -1;
	lvItem.lParam = (long)pProfil;
	m_lcProgList.InsertItem(&lvItem);

    // Laenge
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 1;
    sprintf(szBuf, "%ld", pProfil->iLength);
	lvItem.pszText = (LPTSTR) szBuf;
	m_lcProgList.SetItem(&lvItem);

    // Mass
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 2;
    if (pProfil->mass == MASS_ABSOLUT)
        strcpy(szBuf, "Absolut");
    else
        strcpy(szBuf, "Kette");
    lvItem.pszText = (LPTSTR) szBuf;
	m_lcProgList.SetItem(&lvItem);
}

void CPS312TestDlg::UpdateButtons(void)
{
#if 1
    BOOL bEnable = (m_lcProgList.GetSelectedCount() == 1);
    m_btProfilDel.EnableWindow(bEnable);
    m_btProfilEdit.EnableWindow(bEnable);
#endif // 0
}

void CPS312TestDlg::OnClickedNewProfil()
{
    CPS312DSetEditData data;

    CPS312DSetEditDlg dlg(&data);

    if (dlg.DoModal() == IDOK)
    {
        PS312_DATA_SET* pP = new PS312_DATA_SET;
        pP->iCount = data.m_iCount;
        pP->iLength = data.m_iLength;
        if (data.m_bMassAbsolut == true)
            pP->mass    = MASS_ABSOLUT;
        else
            pP->mass    = MASS_KETTE;
        m_pContainer->Add(pP);
        FillList();

        return;
    }
}

void CPS312TestDlg::OnClickedChangeProfil()
{
    // Profil finden
    PS312_DATA_SET* pPrfl = NULL;
    if (m_lcProgList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lcProgList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        pPrfl = (PS312_DATA_SET*)m_lcProgList.GetItemData(iItem);
    }

    // Dialog starten
    if (pPrfl == NULL)
    {
        AfxMessageBox("Bitte ein Profil auswählen", MB_OK, 0);
        return;
    }

    CPS312DSetEditData data;

    data.m_iCount = pPrfl->iCount;
    data.m_iLength = pPrfl->iLength;
    CPS312DSetEditDlg dlg(&data);
    if (dlg.DoModal() == IDOK)
    {
        pPrfl->iCount = data.m_iCount;
        pPrfl->iLength = data.m_iLength;
        if (data.m_bMassAbsolut == true)
            pPrfl->mass    = MASS_ABSOLUT;
        else
            pPrfl->mass    = MASS_KETTE;
        FillList();
    }
    UpdateButtons();
}

void CPS312TestDlg::OnClickedDelProfil()
{
    if (m_lcProgList.GetSelectedCount() > 0)
    {
        int iItem = -1;
        iItem = m_lcProgList.GetNextItem(iItem, LVNI_SELECTED);
        ASSERT(iItem != -1);
        PS312_DATA_SET* pPrfl = (PS312_DATA_SET*)m_lcProgList.GetItemData(iItem);
        // richtigen Eintrag suchen
        for (int i=0; i<m_pContainer->GetSize(); i++)
        {
            PS312_DATA_SET* pF = (PS312_DATA_SET*)m_pContainer->GetAt(i);
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

void CPS312TestDlg::OnLvnItemActivateProgList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    UpdateButtons();

    *pResult = 0;
}

void CPS312TestDlg::OnDblclkProgList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_lcProgList.GetSelectedCount() > 0)
        OnClickedChangeProfil();

    UpdateButtons();

	*pResult = 0;
}

void CPS312TestDlg::OnClickProgList(NMHDR *pNMHDR, LRESULT *pResult)
{
    UpdateButtons();

    *pResult = 0;
}

void CPS312TestDlg::OnClickedProgRead()
{
    int iCount, iRet, iMaxCount, i;

    UpdateData(TRUE);

    if ((iRet = m_PS312.GetDataSetsPerProg(&iCount)) != PS312_CTRL_NO_ERR)
    {
        ShowError(iRet);
        return;
    }

    iCount += 5;  //??
    // Anzahl Data Sets allokieren
    PS312_DATA_SET* pSets = new PS312_DATA_SET[iCount];
    if (pSets == NULL)
    {
        //?? Fehler
        return;
    }

    iMaxCount = iCount;
    
    if ((iRet = m_PS312.ReadProgram(m_iProgNr, pSets, iMaxCount, &iCount)) != PS312_CTRL_NO_ERR)
    {
        delete [] pSets;
        ShowError(iRet);
        return;
    }

    // anzeigen
    for (i = 0; i < m_pContainer->GetCount(); i++)
    {
        PS312_DATA_SET* pP = (PS312_DATA_SET*)m_pContainer->GetAt(i);
        if (pP)
            delete pP;
    }
    m_pContainer->RemoveAll();

    for (i = 0; i < iCount; i++)
    {
        PS312_DATA_SET* pSet = new PS312_DATA_SET;
        *pSet = pSets[i];
        m_pContainer->Add(pSet);
    }
    delete [] pSets;

    FillList();
}

void CPS312TestDlg::OnClickedProgSend()
{
    int iRet, iCount, i;

    UpdateData(TRUE);

    iCount = m_pContainer->GetCount();

    PS312_DATA_SET* pSets = new PS312_DATA_SET[iCount];

    for (i = 0; i < iCount; i++)
    {
        PS312_DATA_SET* pSet = (PS312_DATA_SET*)m_pContainer->GetAt(i);
        if (pSet)
        {
            pSets[i] = *pSet;
        }
        else
        {
            //?? Fehler
            ShowError("Fehler in m_pContainer!!!");
            delete [] pSets;
            return;
        }
    }

    if ((iRet = m_PS312.LoadProgram(m_iProgNr, pSets, iCount)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler
        ShowError(iRet);
        delete [] pSets;
        return;
    }

    AfxMessageBox("Programm übertragen");  
}

void CPS312TestDlg::OnClickedProgStart()
{
    int iRet;

    UpdateData(TRUE);

    if ((iRet = m_PS312.StartProgram(m_iProgNr)) != PS312_CTRL_NO_ERR)
    {
        ShowError(iRet);
        return;
    }
}

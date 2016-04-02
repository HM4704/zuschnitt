// PS312CtrlDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "..\Node.h"
#include "..\ZuschnittManager.h"
#include "..\ttmain.h"
#include "sim\PS312Simul.h"
#include "PS312TestDlg.h"
#include "PS312CtrlDlg.h"

#pragma warning(disable : 4996)


#define PS312_STATUS_TIMER_PERIOD  3000
#define PS312_STATUS_TIMER_ID      312

#define PS312_STAT_TO_BITPOS(stat, pos) \
        _BitScanForward(&pos, stat);

static bool bFirstCall = true;


#define PS312_CTRL_SECTION   "PS312Ctrl"
// CPS312CtrlDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CPS312CtrlDlg, CDialog)

CPS312CtrlDlg::CPS312CtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPS312CtrlDlg::IDD, pParent)
    , m_strSawWidth(_T(""))
    , m_strProfileLength(_T(""))
    , m_iDataSetCount(0)
    , m_iDSetPerProg(0)
    , m_iActDataSetNr(1)
    , m_iMaxDataSetNr(1)
    , m_bErrorDetected(false)
    , m_hTraceFile(INVALID_HANDLE_VALUE)
    , m_iLastProgUsed(0)
{
    m_strCom = _T("COM1");
    m_pCutListDoc = NULL;
    m_iStatusPS312 = -1;
    m_bDelCutList = true;
}

CPS312CtrlDlg::CPS312CtrlDlg(CXmlDocumentWrapper* pCutList, CWnd* pParent /*=NULL*/)
	: CDialog(CPS312CtrlDlg::IDD, pParent)
    , m_strSawWidth(_T(""))
    , m_strProfileLength(_T(""))
    , m_iDataSetCount(0)
    , m_iDSetPerProg(0)
    , m_iActDataSetNr(1)
    , m_iMaxDataSetNr(1)
    , m_bErrorDetected(false)
    , m_hTraceFile(INVALID_HANDLE_VALUE)
    , m_iLastProgUsed(0)
{
    m_strCom = _T("COM1");
    m_pCutListDoc = pCutList;
    m_iStatusPS312 = -1;
    m_bDelCutList = false;
}

CPS312CtrlDlg::~CPS312CtrlDlg()
{
    if ((m_pCutListDoc != NULL) && (m_bDelCutList == true))
        delete m_pCutListDoc;

    if (m_iLastProgUsed != 0)
        AfxGetApp()->WriteProfileInt(PS312_CTRL_SECTION, "LastProgUsed", m_iLastProgUsed);
    ::CloseHandle(m_hTraceFile);
}

void CPS312CtrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEST_PS312_KOMM_TRACE, m_edPS312KommTrace);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_A_NOT_READY, m_statusLeds[0]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_A_READY, m_statusLeds[1]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_A_MOVING, m_statusLeds[2]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_A_IN_POS, m_statusLeds[3]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_REF_ACTIVE, m_statusLeds[4]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_CURRENT_MAX, m_statusLeds[5]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_KURZSCHLUSS, m_statusLeds[6]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_MEAS_ERR, m_statusLeds[7]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_ENDL_UEBERS, m_statusLeds[8]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_PROG_RUN, m_statusLeds[9]);
    DDX_Control(pDX, IDC_PS312_CTRL_ST_NOT_CONNECT, m_statusLeds[PS312_CTRL_DLG_MAX_STATUS_LEDS-1]);
    DDX_Control(pDX, IDC_PS312_CTRL_PROG_LIST, m_lcProgList);
    DDX_Text(pDX, IDC_PS312_CTRL_SAW_WIDTH, m_strSawWidth);
    DDX_Text(pDX, IDC_PS312_CTRL_PROFILE_LENGTH, m_strProfileLength);
    DDX_Text(pDX, IDC_PS312_CTRL_DSET_COUNT, m_iDataSetCount);
    DDX_Text(pDX, IDC_PS312_CTRL_DSET_PROG_COUNT, m_iDSetPerProg);
    DDX_Control(pDX, IDC_PS312_CTRL_PROG_NR, m_cbProgNr);
    DDX_Control(pDX, IDC_PS312_CTRL_PROG_SEND, m_btProgSend);
    DDX_Control(pDX, IDC_PS312_CTRL_RETRY_CONNECT, m_btRetryConnect);
    DDX_Control(pDX, IDC_PS312_CTRL_START_TEST_DLG, m_btStartTest);
}


BEGIN_MESSAGE_MAP(CPS312CtrlDlg, CDialog)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_PS312_CTRL_PROG_LOAD, &CPS312CtrlDlg::OnClickedPs312CtrlProgLoad)
    ON_BN_CLICKED(IDC_PS312_CTRL_PROG_SEND, &CPS312CtrlDlg::OnClickedPs312CtrlProgSend)
    ON_BN_CLICKED(IDC_PS312_CTRL_RETRY_CONNECT, &CPS312CtrlDlg::OnClickedPs312CtrlRetryConnect)
    ON_BN_CLICKED(IDC_PS312_CTRL_START_TEST_DLG, &CPS312CtrlDlg::OnClickedStartTestDlg)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPS312CtrlDlg-Meldungshandler

BOOL CPS312CtrlDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Verbindung zum Gerät oder Simulation aufnehmen
    // erstmal immer von Simul ausgehen
    m_iLastProgUsed  = AfxGetApp()->GetProfileInt(PS312_CTRL_SECTION, "LastProgUsed", 5);

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
    m_edPS312KommTrace.SetLimitText(1*0x100000);  // 1 MB Tracespeicher
    m_Com.SetTraceWindow(&m_edPS312KommTrace);

    m_PS312.SetCom(&m_Com);
    m_Com.ClearRecv();


    for(int ledIndex = 0; ledIndex < PS312_CTRL_DLG_MAX_STATUS_LEDS; ledIndex++)
    {
        if ((ledIndex < 5) || (ledIndex == 9))
            // kein Fehler
            m_statusLeds[ledIndex].SetIcons(IDI_LED_NONE, IDI_LED_GREEN);
        else
            m_statusLeds[ledIndex].SetIcons(IDI_LED_NONE, IDI_LED_RED);
    }


    InitProfilList();

    LoadList(m_pCutListDoc);

    bFirstCall = true;

    ManagePS312State();
//    GetDeviceParameters();
    InitTrace();

#ifdef _DEBUG  //?? wieder raus
    m_PS312.SetTimeout(-1);
#endif // _DEBUG

    return TRUE;  // return TRUE unless you set the focus to a control
    // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CPS312CtrlDlg::InitProfilList(void)
{
    // set ex styles
    ListView_SetExtendedListViewStyleEx(
        m_lcProgList.GetSafeHwnd(),
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE,
        LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE
    );

    // create columns
	LV_COLUMN lvc;
    // dummy column um erste column rechts zu alignen
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "dummy";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 20;
    lvc.iImage = -1;
	m_lcProgList.InsertColumn(0,&lvc);

	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM/* | LVCF_IMAGE */;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Anzahl";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 20;
    lvc.iImage = -1;
	m_lcProgList.InsertColumn(1,&lvc);
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Länge";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 30;
    lvc.iSubItem = 1;
	m_lcProgList.InsertColumn(2,&lvc);

	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) "Status";
	lvc.cx = m_lcProgList.GetStringWidth(lvc.pszText) + 80;
    lvc.iSubItem = 2;
	m_lcProgList.InsertColumn(3,&lvc);

    // dummy column entfernen
    m_lcProgList.DeleteColumn(0);
}

void CPS312CtrlDlg::FillList(void)
{
    vector<CTRL_DATA_SET>::iterator iter;

    m_lcProgList.DeleteAllItems();

    for (iter = m_CutList.begin(); iter != m_CutList.end(); iter++)
    {
        InsertProfil(iter);
    }
}

BOOL CPS312CtrlDlg::LoadList(CXmlDocumentWrapper* pCutListDoc)
{
    CTRL_DATA_SET set;
    char szNode[30];
    CXmlNodeWrapper node;

    if (pCutListDoc == NULL)
        return FALSE;

    m_CutList.clear();
    m_iActDataSetNr = 1;
    m_iMaxDataSetNr = 1;

    // Sägeblattdicke überprüfen
    CXmlNodeWrapper rootNode(pCutListDoc->AsNode());
    if (rootNode.IsValid() == FALSE)
    {
        ShowError("Fehler beim Laden der Schnittliste!");
        return FALSE;
    }

    m_strProfileLength = rootNode.GetValue("Profillaenge");
    m_strSawWidth      = rootNode.GetValue("Saegeblattdicke");

    int i=1;

    // erstes Mass ist absolut
    set.ds.mass = MASS_ABSOLUT;

    while (TRUE)
    {
        sprintf(szNode, "PN-%04d", i);
        CXmlNodeWrapper node(rootNode.FindNode(szNode));
        if (node.IsValid() == TRUE)
        {
            CString strCount  = node.GetValue("Anzahl");
            CString strLength = node.GetValue("Laenge");

            set.byStatus = 0;  // offen
            set.ds.iCount  = atoi(strCount.GetBuffer(strCount.GetLength()));
            set.ds.iLength = atoi(strLength.GetBuffer(strLength.GetLength()));
            set.iDataSetNr = 1; // default bis die Geräte Parameter vorhanden sind

            m_CutList.push_back(set);

            set.ds.mass = MASS_KETTE;
        }
        else
            break;

        i++;
        node.Detach();
    }

    m_iDataSetCount = m_CutList.size();
    FillList();

    Trace("\r\nSchnittliste geladen: Anzahl=%ld, Profillänge=%s, Sägeblatt=%s.\r\n", m_iDataSetCount, m_strProfileLength, m_strSawWidth);

    UpdateData(FALSE);

    return true;
}

void CPS312CtrlDlg::RecalculateDataSets(void)
{
    int iDataSet, iCurrentLength, iCurCutLength, iEffProfileLength, iDataSetPart, iSaveCnt, iSaveIdx, iBladeWidth;
    vector<CTRL_DATA_SET> newCutList, tempList/*, debugList*/;
    vector<CTRL_DATA_SET>::iterator iter, saveIter, tempIter;
    InputMap orgMap, newMap;
    InputMap::iterator orgIter, newIter;
    bool bError = false;

    CTRL_DATA_SET  newDataSet;

    if (m_iDSetPerProg == 0)
        // noch keine Daten von der Säge
        return;

    Trace("\r\n****Recalculating data sets******\r\n");                 

    CreateInputList(orgMap, m_CutList);

    iEffProfileLength = atoi(m_strProfileLength.GetBuffer(m_strProfileLength.GetLength()));
    iBladeWidth = atoi(m_strSawWidth.GetBuffer(m_strSawWidth.GetLength()));
    ASSERT(iBladeWidth >= 0);
    iCurrentLength = 0;
    iDataSet = 0;
    iCurCutLength = 0;
    iDataSetPart = 1;
//    debugList.assign(m_CutList.begin(), m_CutList.end());
    for (iter = m_CutList.begin(); iter != m_CutList.end(); iter++)
    {
        iter->iDataSetNr = iDataSetPart;
        newCutList.push_back(*iter);
        if (iDataSet < m_iDSetPerProg)
        {
            // Data set passt rein
            iDataSet++;
            for (int i = 0; i < iter->ds.iCount; i++)
            {
                if ((iCurCutLength + (iter->ds.iLength + iBladeWidth)) > iEffProfileLength)
                {
                    // neues Profile anfangen
                    iCurCutLength = iter->ds.iLength;
                    iSaveCnt = i;
//                    saveIter = newCutList.end();
                    iSaveIdx = newCutList.size();
                    TRACE("neues Profil(%ld,%ld,%ld)\n", iter->ds.iLength, i, iSaveIdx);                 
                    Trace("neues Profil(%ld,%ld,%ld)\r\n", iter->ds.iLength, i, iSaveIdx);                 
                }
                else
                {
                    iCurCutLength += iter->ds.iLength;
                    iCurCutLength += iBladeWidth;
                }
            }
        }
        else
        {
            // iDataSet == m_iDSetPerProg
            // ein Datensatz passt noch rein
            // die Datasets ab dem letzten Profilanfang auf neue Datset Nr setzen

            // richtigen Iterator finden
            int i = 0;
            for (saveIter = newCutList.begin(); saveIter != newCutList.end(); saveIter++)
            {
                if (i == iSaveIdx)
                    break;
                i++;
            }
            if (saveIter == newCutList.end())
            {
                ShowError("Es ist ein Fehler beim Aufteilen der Datensätze aufgetreten.\nErhöhen Sie bitte die Anzahl der Speicherplätze pro Programm im Steuergerät.\n");
                m_bErrorDetected = true;
                EnableButtons();
                return;
            }

            iDataSetPart++;
            iDataSet = 0;
            // alle Datasets im neuen Vector auf neue Dataset Nr setzen bis zum letzten Profilwechsel
            tempList.assign(saveIter, newCutList.end());
            // debug
            for (tempIter = tempList.begin(); tempIter != tempList.end(); tempIter++)
            {
                TRACE("gespeicherter Datensatz(%ld,%ld)\n", tempIter->ds.iLength, tempIter->ds.iCount);
                Trace("gespeicherter Datensatz(%ld,%ld)\r\n", tempIter->ds.iLength, tempIter->ds.iCount);
            }
            newCutList.erase(saveIter, newCutList.end());
            // alten Dataset aufteilen
            CTRL_DATA_SET &dataSet = newCutList.back();
            int iOldCount = dataSet.ds.iCount;
            if (iSaveCnt == 0)
            {
                // Profilwechsel war bei Datensatzbeginn, gesamten Datensatz hochzählen, es muss kein neuer Datensatz
                // erzeugt werden
                dataSet.iDataSetNr = iDataSetPart;
                iDataSet = 1;  // schon ein Profil im nächsten Teil drin
            }
            else
            {
                // datensatz aufteilen
                dataSet.ds.iCount = iSaveCnt;
                // neuen Dataset erzeugen
                newDataSet.byStatus   = 0;
                newDataSet.iDataSetNr = iDataSetPart;
                newDataSet.ds.iCount  = iOldCount - dataSet.ds.iCount;
                newDataSet.ds.iLength = dataSet.ds.iLength;
                newDataSet.ds.mass    = MASS_ABSOLUT;
                iDataSet = 1;
                if (newDataSet.ds.iCount == 0)
                {
                    // Profilwechsel war am Ende des Datensatzes, kein neuer Datensatz nötig
                }
                else
                    newCutList.push_back(newDataSet);
            }
            for (tempIter = tempList.begin(); tempIter != tempList.end(); tempIter++)
            {
                // allen folgenden Datensätzen  neue Nummer geben
                tempIter->iDataSetNr = iDataSetPart;
                newCutList.push_back(*tempIter);
                iDataSet++;
            }
        }
    }
    m_iMaxDataSetNr = iDataSetPart;

    CreateInputList(newMap, newCutList);

    m_CutList.swap(newCutList);
    m_iDataSetCount = m_CutList.size();
//    tempIter = debugList.begin();
//    iter = m_CutList.begin();
#if 0
    while (TRUE)
    {
        if (tempIter != debugList.end() && iter != m_CutList.end())
        {
            if ((iter->ds.iCount != tempIter->ds.iCount) || (iter->ds.iLength != tempIter->ds.iLength))
            {
                TRACE("found diff in data set: org(%ld, %ld) new(%ld, %ld)\n", tempIter->ds.iCount, tempIter->ds.iLength,
                    iter->ds.iCount, iter->ds.iLength);
                Trace("\r\nfound diff in data set: org(%ld, %ld) new(%ld, %ld)\r\n", tempIter->ds.iCount, tempIter->ds.iLength,
                    iter->ds.iCount, iter->ds.iLength);
                bError = true;
            }
            else
            {
                //TRACE("identical data set: org(%ld, %ld) new(%ld, %ld)\n", tempIter->ds.iCount, tempIter->ds.iLength,
                //       iter->ds.iCount, iter->ds.iLength);
            }
        }
        else
        {
            // eine Liste ist schon am Ende
            if (tempIter != debugList.end())
            {
                TRACE("one more profile in org(%ld, %ld)\n", tempIter->ds.iCount, tempIter->ds.iLength);
                Trace("\r\none more profile in org(%ld, %ld)\r\n", tempIter->ds.iCount, tempIter->ds.iLength);
                bError = true;
            }
            else
            if (iter != m_CutList.end())
            {
                TRACE("one more profile in new(%ld, %ld)\n", iter->ds.iCount, iter->ds.iLength);
                Trace("\r\none more profile in new(%ld, %ld)\r\n", iter->ds.iCount, iter->ds.iLength);
                bError = true;
            }
        }
        if (tempIter != debugList.end())
            tempIter++;
        if (iter != m_CutList.end())
            iter++;
        if (tempIter == debugList.end() && iter == m_CutList.end())
            break;
    }
#else
    orgIter = orgMap.begin();
    newIter = newMap.begin();
    while (TRUE)
    {
        if (newIter != newMap.end() && orgIter != orgMap.end())
        {
            if ((orgIter->first != newIter->first) || (orgIter->second != newIter->second))
            {
                TRACE("found diff in data set: org(%ld, %ld) new(%ld, %ld)\n", orgIter->second, orgIter->first,
                    newIter->second, newIter->first);
                Trace("\r\nfound diff in data set: org(%ld, %ld) new(%ld, %ld)\r\n", orgIter->second, orgIter->first,
                    newIter->second, newIter->first);
                bError = true;
            }
            else
            {
                TRACE("new(%ld,%ld)  org(%ld,%ld)\n", newIter->second, newIter->first, orgIter->second, orgIter->first);
            }
        }
        else
        {
            // eine Liste ist schon am Ende
            if (orgIter != orgMap.end())
            {
                TRACE("one more profile in org(%ld, %ld)\n", orgIter->second, orgIter->first);
                Trace("\r\none more profile in org(%ld, %ld)\r\n", orgIter->second, orgIter->first);
                bError = true;
            }
            else
            if (newIter != newMap.end())
            {
                TRACE("one more profile in new(%ld, %ld)\n", newIter->second, newIter->first);
                Trace("\r\none more profile in new(%ld, %ld)\r\n", newIter->second, newIter->first);
                bError = true;
            }
        }
        if (newIter != newMap.end())
            newIter++;
        if (orgIter != orgMap.end())
            orgIter++;
        if (newIter == newMap.end() && orgIter == orgMap.end())
            break;
    }

    // Ueberpruefung auf Einhaltung der maximalen Anzahl an Datensätzen pro Programmierung
    iDataSetPart = 0;
    iDataSet = 0;
    for (iter = m_CutList.begin(); iter != m_CutList.end(); iter++)
    {
        if (iDataSetPart == iter->iDataSetNr)
        {
            iDataSet++;
        }
        else
        {
            // neuer Datensatz Teil
            if (iDataSet >= m_iDSetPerProg)
            {
                Trace("\r\nRecalc: zuviele Datensätze(%ld) für einen Programmierschritt!\r\n", iDataSet);
                bError = true;
                break;
            }
            else
            {
                iDataSet = 0;
                iDataSetPart++;
            }
        }
    }

#endif
    if (bError == true)
    {
        ShowError("Es ist ein Fehler beim Aufteilen der Datensätze aufgetreten.\nErhöhen Sie bitte die Anzahl der Speicherplätze pro Programm im Steuergerät.\n");
        m_bErrorDetected = true;
        EnableButtons();
    }
    else
        Trace("\r\n****Recalculating data sets END******\r\n");
}

void CPS312CtrlDlg::CreateInputList(InputMap& inputMap, vector<CTRL_DATA_SET>& cList)
{
    vector<CTRL_DATA_SET>::iterator iter;
    pair< MIIter, bool > pr;

    inputMap.clear();

    for (iter = cList.begin(); iter != cList.end(); iter++)
    {
        pr = inputMap.insert ( Int_Pair ( iter->ds.iLength, iter->ds.iCount) );
        if( pr.second == true ) 
        {
            // The element was inserted successfully
        }
        else 
        {
            // Key number already exists, add it
            ( pr.first )->second += iter->ds.iCount;  //?? es werden immer zwei geschnitten
        }
    }
}

void CPS312CtrlDlg::InsertProfil(vector<CTRL_DATA_SET>::iterator iter)
{
    char szBuf[30];
	LV_ITEM lvItem;
    int iItem = m_lcProgList.GetItemCount();

    // Anzahl
	lvItem.mask = LVIF_TEXT/* | LVIF_PARAM*/;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
    itoa(iter->ds.iCount, szBuf, 10);
	lvItem.pszText = (LPTSTR) szBuf;
	lvItem.iImage = -1;
//	lvItem.lParam = (long)pProfil;
	m_lcProgList.InsertItem(&lvItem);

    // Laenge
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 1;
    itoa(iter->ds.iLength, szBuf, 10);
	lvItem.pszText = (LPTSTR) szBuf;
	m_lcProgList.SetItem(&lvItem);

    // Status
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 2;
    if (iter->byStatus == 0)
        strncpy(szBuf, "offen", sizeof(szBuf));
    else
        strncpy(szBuf, "programmiert", sizeof(szBuf));
	lvItem.pszText = (LPTSTR) szBuf;
	m_lcProgList.SetItem(&lvItem);
}

void CPS312CtrlDlg::ManagePS312State(void)
{
    int iState;

    int iRet;

    iState = m_iStatusPS312;
    if (bFirstCall == false)
    {
        // Status abfragen
        if ((iRet = m_PS312.GetStatus(PS312_LEVEL_AXIS1, &m_iStatusPS312)) == PS312_CTRL_NO_ERR)
        {
            if (iState == -1)
            {
                // neue Verbindung, Parameter abfragen
                GetDeviceParameters();
            }
        }
        else
        {
            m_iStatusPS312 = -1;
            KillTimer(PS312_STATUS_TIMER_ID);
            if (iState == -1)
            {
                // Verbindungsaufbau nicht erfolgreich
                ShowError("Verbindung zur Säge kann nicht aufgebaut werden!");
            }
            else
            {
                ShowError("Verbindung zur Säge verloren!");
            }
        }
    }
    else
    {
        bFirstCall = false;
        SetTimer(PS312_STATUS_TIMER_ID, PS312_STATUS_TIMER_PERIOD, NULL);
        EnableButtons();
        SetLedsStatus();
        return;
    }


    // Timer wieder starten, wenn Verbindung neu aufgebaut wurde
    if ((m_iStatusPS312 != -1) && (iState == -1))
        SetTimer(PS312_STATUS_TIMER_ID, PS312_STATUS_TIMER_PERIOD, NULL);

    EnableButtons();

    SetLedsStatus();
}

void CPS312CtrlDlg::SetLedsStatus(void)
{
    int i;
    int iStat[PS312_CTRL_DLG_MAX_STATUS_LEDS];

    // alle erstmal ausschalten
    memset(iStat, LED_BUTTON_STATE_OFF, sizeof(iStat));

    if (m_iStatusPS312 == -1)
    {
        // keine Verbindung
        iStat[PS312_CTRL_DLG_MAX_STATUS_LEDS-1] = LED_BUTTON_STATE_ON;
    }
    else
    {
        if (m_iStatusPS312 == 0)
        {
            iStat[0] = LED_BUTTON_STATE_ON;
        }
        else
        {
            int iMask = 1;
            for (i = 1; i < PS312_CTRL_DLG_MAX_STATUS_LEDS; i++)
            {
                if ((m_iStatusPS312&iMask) != 0)
                {
                    iStat[i] = LED_BUTTON_STATE_ON;;
                }
                iMask <<= 1;
            }
        }
    }

    for (i = 0; i < PS312_CTRL_DLG_MAX_STATUS_LEDS; i++)
    {
        m_statusLeds[i].SetLedState(iStat[i]);
    }
}

void CPS312CtrlDlg::EnableButtons(void)
{
    if (m_iStatusPS312 == -1)
    {
        // keine Verbindung
        m_btRetryConnect.EnableWindow(TRUE);
        m_btProgSend.EnableWindow(FALSE);
    }
    else
    {
        m_btRetryConnect.EnableWindow(FALSE);
        m_btProgSend.EnableWindow(TRUE);
    }
    if (m_iActDataSetNr > m_iMaxDataSetNr)
    {
        if (m_btProgSend.IsWindowEnabled())
            m_btProgSend.EnableWindow(FALSE);
    }
    if (m_iDataSetCount == 0)
    {
        if (m_btProgSend.IsWindowEnabled())
            m_btProgSend.EnableWindow(FALSE);        
    }

    if (m_bErrorDetected == true)
    {
        // Fehler steht an Programmieren nicht möglich
        if (m_btProgSend.IsWindowEnabled())
            m_btProgSend.EnableWindow(FALSE);
    }

    CTtmainApp *pApp = (CTtmainApp*) AfxGetApp();
    if (pApp)
    {
        if (pApp->IsInTestMode())
            m_btStartTest.ShowWindow(SW_SHOW);
    }
}

void CPS312CtrlDlg::GetDeviceParameters(void)
{
    int iRet;

    // Anzahl Datensätze pro Prog holen
    if ((iRet = m_PS312.GetDataSetsPerProg(&m_iDSetPerProg)) != PS312_CTRL_NO_ERR)
    {
        m_iDSetPerProg = 0;
        ShowError(iRet);
        return;
    }
    m_iDSetPerProg--; //?? fuer Gesamtmass am Anfang

    // Anzahl Programme holen
    if ((iRet = m_PS312.GetParam(PS312_LEVEL_ALL, PS312_PARAM_COUNT_PROGS, &m_iCountProgs)) != PS312_CTRL_NO_ERR)
    {
        m_iCountProgs = 0;
        ShowError(iRet);
        return;
    }

    // Sägeblattstärke


    // setze die Programmnummern
    CString str;
    for (int i = 1; i <= m_iCountProgs; i++)
    {
        str.Format(_T("%d"), i);
        m_cbProgNr.AddString( str );
    }

    if (m_iLastProgUsed < m_iCountProgs)
        m_cbProgNr.SetCurSel((m_iLastProgUsed-1));
    else
        m_cbProgNr.SetCurSel(5);

    Trace("\r\nGeräte Parameter: Anzahl Programme=%ld, Datensätz/Programm=%ld\r\n", m_iCountProgs, m_iDSetPerProg);

    if (m_iDSetPerProg < m_iDataSetCount)
    {
        CDialog dlg(IDD_PS312_DSET_INFO_DLG);

        // mehr Datensätze als Speicher auf dem Gerät vorhanden
        RecalculateDataSets();
        // Msg box anzeigen
        dlg.DoModal();
//        AfxMessageBox("Die Profilliste besteht aus mehr Datensätze, als in das Programm passen. Daher muss die Liste in mehreren Schritten abgearbeitet werden!",
//            MB_ICONEXCLAMATION|MB_OK);

        FillList();
        UpdateData(FALSE);
    }

    UpdateData(FALSE);
}

void CPS312CtrlDlg::ShowError(int iError)
{
    char szBuf[MAX_PATH];

    sprintf(szBuf, "Fehler: %s", m_PS312.GetErrorString(iError));
    AfxMessageBox(szBuf, MB_ICONSTOP);
    Trace("\r\nFehler: %s\r\n", m_PS312.GetErrorString(iError));
}

void CPS312CtrlDlg::ShowError(char* szError)
{
    char szBuf[MAX_PATH];

    sprintf(szBuf, "Fehler: %s", szError);
    AfxMessageBox(szBuf, MB_ICONSTOP);
    Trace("\r\nFehler: %s\r\n", szError);
}

void CPS312CtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == PS312_STATUS_TIMER_ID)
        ManagePS312State();

    CDialog::OnTimer(nIDEvent);
}

void CPS312CtrlDlg::OnClickedPs312CtrlProgLoad()
{
    CFileDialog dlg(TRUE, "psl", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Profil Schnittliste (*.psl)|*.psl|Alle Dateien (*.*)|*.*||");

    if (dlg.DoModal() == IDOK)
    {
        CString szFile = dlg.GetPathName();
        if (szFile.GetLength() > 0)
        {
            if (m_bDelCutList == true)
                delete m_pCutListDoc;
            m_pCutListDoc = new CXmlDocumentWrapper();
            m_bDelCutList = true;
            if (m_pCutListDoc)
            {
                if (m_pCutListDoc->Load(szFile.GetBuffer(szFile.GetLength())) == TRUE)
                {
                    LoadList(m_pCutListDoc);
                    if (m_iDSetPerProg < m_iDataSetCount)
                    {
                        CDialog dlg(IDD_PS312_DSET_INFO_DLG);

                        // mehr Datensätze als Speicher auf dem Gerät vorhanden
                        RecalculateDataSets();
                        // Msg box anzeigen
                        dlg.DoModal();

                        FillList();
                        UpdateData(FALSE);
                    }
                    EnableButtons();
                    return;
                }
            }
            ShowError("Fehler beim Öffnen der Datei");
        }
    }
}

void CPS312CtrlDlg::OnClickedPs312CtrlProgSend()
{
    int iRet, iCount, iProgNr;
    vector<CTRL_DATA_SET>::iterator iter;
//    char cBuf[20];

    UpdateData(TRUE);


    if (m_iActDataSetNr > 1)
    {
        // nächster Teil wird programmiert, Warnung ausgeben
        CDialog dlg(IDD_PS312_NEXT_DSET_INFO_DLG);
        if (dlg.DoModal() != IDOK)
            return;
    }


    iCount = m_CutList.size();
    iProgNr = m_cbProgNr.GetCurSel();
    if (iProgNr == CB_ERR)
    {
        ShowError("Bitte Programm Nr. auswählen!");
        return;
    }
    Trace("\r\n****Start Programmierung in Prog %ld\r\n", iProgNr);


    iProgNr++;
    m_iLastProgUsed = iProgNr;

    iCount = 0;
    PS312_DATA_SET* pSets = new PS312_DATA_SET[m_iDSetPerProg+2 /* fuer eventuelles ABSOLUT_MASS */];
    memset(pSets, 0, (m_iDSetPerProg+1)*sizeof(PS312_DATA_SET));

    //?? erster Datensatz ist absolute Laenge
    pSets[iCount].iLength = atoi(m_strProfileLength.GetBuffer(m_strProfileLength.GetLength())) + 10 /*??*/;
    pSets[iCount].iCount = 1;
    pSets[iCount].mass = MASS_ABSOLUT;
    iCount++;

    for (iter = m_CutList.begin(); iter != m_CutList.end(); iter++)
    {
        if (m_iActDataSetNr == iter->iDataSetNr)
        {
            if (iCount == 0)
            {
                if ((iter->ds.mass == MASS_KETTE) || (iter->ds.iCount > 1))
                {
                    // Sonderbehandlung fuer erstes Profil
                    if (iter->ds.iCount > 1)
                    {
                        // Profile auspalten
                        pSets[iCount] = iter->ds;
                        pSets[iCount].iCount = 1;
                        pSets[iCount].mass = MASS_ABSOLUT;
                        iCount++;
                        pSets[iCount] = iter->ds;
                        pSets[iCount].iCount -= 1;
                        pSets[iCount].mass = MASS_KETTE;
                        iCount++;
                    }
                    else
                    {
                        // Anzahl 1 aber KETTE
                        pSets[iCount] = iter->ds;
                        pSets[iCount].iLength *= -1;  //?? negativ
                        pSets[iCount].mass = MASS_ABSOLUT;
                        iCount++;
                    }
                }
                else
                {
                    // Anzahl 1 und ABSOLUT_MASS nichts zu tun
                    pSets[iCount] = iter->ds;
                    pSets[iCount].iLength *= -1;  //?? negativ
                    pSets[iCount].mass = MASS_KETTE;
                    iCount++;
                }
            }
            else
            {
                pSets[iCount] = iter->ds;
                pSets[iCount].iLength *= -1;  //?? negativ
                pSets[iCount].mass = MASS_KETTE;
                iCount++;
            }
        }
    }

    BeginWaitCursor();
    m_btProgSend.EnableWindow(FALSE);
    if ((iRet = m_PS312.LoadProgram(iProgNr, pSets, iCount)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler
        ShowError("Fehler beim Laden des Programms!");
        ShowError(iRet);
        delete [] pSets;
        m_btProgSend.EnableWindow(TRUE);
        EndWaitCursor();
        return;
    }

    delete [] pSets;

    for (iter = m_CutList.begin(); iter != m_CutList.end(); iter++)
    {
        if (m_iActDataSetNr == iter->iDataSetNr)
        {
            iter->byStatus = 1;
        }
    }
    
    m_iActDataSetNr++;

    FillList();
    UpdateData(FALSE);

    EndWaitCursor();
    AfxMessageBox("Programm wurde erfolgreich übertragen", MB_OK|MB_ICONINFORMATION);  
    Trace("\r\n****Ende Programmierung\r\n");
    //m_btProgSend.EnableWindow(TRUE);
    EnableButtons();
}

void CPS312CtrlDlg::Trace(char* szFormat,...)
{
    char szBuf[MAX_PATH];
    va_list argptr;

   va_start (argptr, szFormat);
   vsprintf (szBuf, szFormat, argptr);
   va_end   (argptr);

    m_edPS312KommTrace.ReplaceSel(szBuf, 0);
}

void CPS312CtrlDlg::OnClickedPs312CtrlRetryConnect()
{
    m_Com.ClearDevice();

    ManagePS312State();
}

void CPS312CtrlDlg::OnClickedStartTestDlg()
{
    int iRet;
#if 0
    CPS312TestDlg dlg;

    dlg.DoModal();    
#else
    if ((iRet = m_PS312.StartProgram(m_iLastProgUsed)) != PS312_CTRL_NO_ERR)
    {
        //?? Fehler
        ShowError("Fehler beim Starten des Programms!");
        ShowError(iRet);
        return;
    }
#endif //
}

#define MAX_TRACE_FILES         20
void CPS312CtrlDlg::InitTrace(void)
{
    int i;
    char szFile[MAX_PATH], szPath[MAX_PATH];

    CTtmainApp* pApp = (CTtmainApp*) AfxGetApp();
    if (pApp)
    {
        strcpy(szPath, pApp->GetAppPath().GetBuffer(pApp->GetAppPath().GetLength()));
    }
    else
    {
        memset(szPath, 0, sizeof(szPath));
    }

    // nachschauen ob Verzeichnis existiert, wenn nicht anlegen
    strcpy(szFile, szPath);
    strcat(szPath,  "\\Trace");
    if (::CreateDirectory(szPath, NULL) == FALSE)
    {
        DWORD dwErr = ::GetLastError();
        if (dwErr == ERROR_ALREADY_EXISTS)
        {
            // ok
        }
        else
        {
            //?? Fehler
            return;
        }
    }

    // Namen der nächsten Trace Datei bestimmen
    for (i = 0; i < MAX_TRACE_FILES; i++)
    {
        sprintf(szFile, "%s\\PS312CtrlTrace%02d.txt", szPath, (i+1));

        if ((m_hTraceFile = ::CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
        {
            // File existiert bereits, nächsten Namen probieren
        }
        else
        {
            break;
        }
    }

    if (i == MAX_TRACE_FILES)
    {
        // Überlauf, bei erstem wieder anfangen und nächstes löschen
        i = 0;
        sprintf(szFile, "%s\\PS312CtrlTrace%02d.txt", szPath, (i+1));
        if ((m_hTraceFile = ::CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
        {
            //?? Fehler
        }
        else
        {
            // ok
        }
    }

    // nächste Datei löschen
    if (i == (MAX_TRACE_FILES-1))
        i = 0;
    else
        i++;

        sprintf(szFile, "%s\\PS312CtrlTrace%02d.txt", szPath, (i+1));
    ::DeleteFile(szFile);
}
void CPS312CtrlDlg::OnDestroy()
{
    DWORD dwWritten;

    CDialog::OnDestroy();

    if (m_hTraceFile != INVALID_HANDLE_VALUE)
    {
        HLOCAL h = m_edPS312KommTrace.GetHandle();
        if (h != NULL)
        {
            LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);

            if (::WriteFile(m_hTraceFile, lpszText, strlen(lpszText), &dwWritten, NULL) == FALSE)
            {
                // Fehler
            }
        }
    }
}

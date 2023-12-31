// ttmain.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ZRTreeCtrl.h"
#include "ttmain.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "HoleProfileFrame.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "HoleProfileDoc.h"
#include "HoleProfileView.h"
#include "winspool.h"
#include "StatLink.h"
#include "MyException.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTtmainApp

BEGIN_MESSAGE_MAP(CTtmainApp, CWinAppEx)
	//{{AFX_MSG_MAP(CTtmainApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_ZUSCHNITTDATEI_AUSLESEN, OnZuschnittdateiAuslesen)
	ON_UPDATE_COMMAND_UI(ID_ZUSCHNITTDATEI_AUSLESEN, OnUpdateZuschnittdateiAuslesen)
	ON_COMMAND(ID_ZUSCHNITTREGELN_SPEICHERN, OnZuschnittregelnSpeichern)
	ON_UPDATE_COMMAND_UI(ID_ZUSCHNITTREGELN_SPEICHERN, OnUpdateZuschnittregelnSpeichern)
	ON_COMMAND(ID_HOLE_PROFILE_NEW, OnHoleProfileNew)
	ON_UPDATE_COMMAND_UI(ID_HOLE_PROFILE_NEW, OnUpdateHoleProfileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, /*CWinApp::OnFileNew*/OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_REFTOR_LADEN, OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTtmainApp construction

CTtmainApp::CTtmainApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    char cBuf[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, cBuf);
    CString strHilfeVerz = cBuf;
    strHilfeVerz += "\\";
    strHilfeVerz += HILFE_VERZEICHNIS;

    m_Help.SetFileDirectory((LPCTSTR)strHilfeVerz);

//    m_strZuschnittDatei = "Zuschnittregeln\\Zuschnitt-Regeln.txt";

    m_bTestMode = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTtmainApp object

CTtmainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTtmainApp initialization

BOOL CTtmainApp::InitInstance()
{
//    SetRegistryKey("Moosburger\\Zuschnitt");

	// InitCommonControlsEx() ist f�r Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder h�her zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden m�chten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

    VERIFY(InitContextMenuManager());

	AfxEnableControlContainer();

	InitKeyboardManager();

//    int i = 0;
//    while (i == 0)
//        Sleep(100);
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

    // SEH fuer C++ aktivieren
    MyExceptInit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CoInitialize(NULL);

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Moosburger"));

    LoadStdProfileSettings();  // Load standard INI file options (including MRU)

    CString strProg = _T("Moosburger\\Zuschnitt");
    m_regManager.Open(strProg, 30, TRUE);

    // Zuschnittregel einlesen
    char path[MAX_PATH];
    ::GetModuleFileName(AfxGetInstanceHandle(), path, sizeof(path));
    char* exe = strrchr(path, '\\');
    if (exe != NULL)
        *exe = 0;       // remove exe name
    m_strZuschnittDatei = path;
    m_strZuschnittDatei += "\\Zuschnittregeln\\Zuschnitt-Regeln.txt";
    CheckDirectory(m_strZuschnittDatei);  // erzeugt die n�tigen Directories
    m_ZuschnittMan.ZuschnittDateiSetzen(m_strZuschnittDatei);
    OnZuschnittdateiAuslesen();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TTMAINTYPE,
		RUNTIME_CLASS(CTtmainDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTtmainView));
	AddDocTemplate(pDocTemplate);

    CMultiDocTemplate* pHPTemplate;
    pHPTemplate = new CMultiDocTemplate(
      IDR_HOLE_PROFILE,
      RUNTIME_CLASS(CHoleProfileDoc),
      RUNTIME_CLASS(/*CHoleProfileFrame*/CMDIChildWndEx),        // standard MDI child frame
      RUNTIME_CLASS(CHoleProfileView));
    AddDocTemplate(pHPTemplate);

    RegisterShellFileTypes(FALSE);
	EnableShellOpen();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
//	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

    if (cmdInfo.m_strFileName == "test")
        m_bTestMode = TRUE;

	LoadCustomState ();

    char szPath[MAX_PATH];
    ::GetCurrentDirectory(MAX_PATH, szPath);
    m_strAppPath = szPath;

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//Druckereinstellungen auf LANDSCAPE
	SetPrintOrientation(DMORIENT_LANDSCAPE);

    m_iLastMode = AfxGetApp()->GetProfileInt("Settings", "LastStartedMode", 0);
#if 0
	CWinApp::OnFileNew();
#else
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew || 
        cmdInfo.m_nShellCommand == -1)
    {
        if (m_iLastMode == 0)
            pDocTemplate->OpenDocumentFile(NULL);
        else
            m_pMainWnd->PostMessage(WM_COMMAND, ID_WHSPROFILE, NULL);
    }
    else
    {
		ProcessShellCommand(cmdInfo);
		//if (!ProcessShellCommand(cmdInfo))
		//	return FALSE;
    }
#endif //

	return TRUE;
}

void CTtmainApp::OnFileNew()
{
#if 0
	CWinApp::OnFileNew();
#else
    POSITION pos = GetFirstDocTemplatePosition();
    CDocTemplate* pTmplt = GetNextDocTemplate(pos);
    if (pTmplt != NULL)
        pTmplt->OpenDocumentFile(NULL);    

    m_iLastMode = 0;
#endif // 0
}

void CTtmainApp::OnFileOpen()
{
	CWinAppEx::OnFileOpen();
}

void CTtmainApp::SetPrintOrientation(short dmOrientation)
{
    // Get default printer settings.
    PRINTDLG   pd;

    pd.lStructSize = (DWORD) sizeof(PRINTDLG);
    if (GetPrinterDeviceDefaults(&pd))
    {
        // Lock memory handle.
        DEVMODE FAR* pDevMode =
            (DEVMODE FAR*)::GlobalLock(m_hDevMode);
        LPDEVNAMES lpDevNames;
        LPTSTR lpszDriverName, lpszDeviceName, lpszPortName;
        HANDLE hPrinter;


		if (pDevMode)
        {
            // Change printer settings in here.
            pDevMode->dmOrientation = dmOrientation; //DMORIENT_LANDSCAPE;
			// Unlock memory handle.
			lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
			lpszDriverName = (LPTSTR )lpDevNames + lpDevNames->wDriverOffset;
			lpszDeviceName = (LPTSTR )lpDevNames + lpDevNames->wDeviceOffset;
			lpszPortName   = (LPTSTR )lpDevNames + lpDevNames->wOutputOffset;

			::OpenPrinter(lpszDeviceName, &hPrinter, NULL);
			::DocumentProperties(NULL,hPrinter,lpszDeviceName,pDevMode,
                           pDevMode, DM_IN_BUFFER|DM_OUT_BUFFER);

			// Sync the pDevMode.
			// See SDK help for DocumentProperties for more info.
			::ClosePrinter(hPrinter);
			::GlobalUnlock(m_hDevNames);
			::GlobalUnlock(m_hDevMode);
		}
    }
} 
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

    CStaticLink m_linkMail;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTtmainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_linkMail.SubclassDlgItem(IDC_LINK_MAIL, this);
    m_linkMail.m_link = _T("mailto:hermann.moosburger@gmx.de");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CTtmainApp message handlers


void CTtmainApp::OnZuschnittdateiAuslesen() 
{
    FILE* hFile;
//    char szDir[MAX_PATH];

    // current directory setzen
#if 0
    if (GetModuleFileName(
      NULL,    // handle to module, current
      szDir,  // path buffer
      MAX_PATH         // maximum characters to retrieve
    ) != 0)
    {
        char* pC = strrchr(szDir, '\\');
        if (pC)
        {
            *pC = 0;
        }
        SetCurrentDirectory(szDir);
    }
#endif

    // ist Zuschnitt-Datei da?
    if ((hFile = fopen(m_strZuschnittDatei, "r")) == NULL)
    {
        char szMsg[10*MAX_PATH];

//        GetCurrentDirectory(MAX_PATH, szDir);
//        strcat(szDir, "\\");
//        strcat(szDir, m_strZuschnittDatei);
        sprintf(szMsg, "WARNUNG!!!!\nDie Datei mit den Zuschnittregeln kann nicht ge�ffnet werden!\nZuschnittregeln k�nnen damit falsch sein.\nBitte �berpr�fen Sie die Datei %s.",
            m_strZuschnittDatei);
        // Nein, muss von Hand kopiert werden aus Sicherheitsgr�nden
        AfxMessageBox(szMsg, MB_OK|MB_ICONWARNING, 0);
        return;
    }
    else
        fclose(hFile);
    m_ZuschnittMan.ZugabeWerteRuecksetzen();
    m_ZuschnittMan.ZuschnittDateiAuswerten(m_strZuschnittDatei.GetBuffer(m_strZuschnittDatei.GetLength()));	
    m_strZuschnittDatei.ReleaseBuffer();
}

void CTtmainApp::OnUpdateZuschnittdateiAuslesen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


BOOL CTtmainApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CWinAppEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CTtmainApp::OnZuschnittregelnSpeichern() 
{
	m_ZuschnittMan.ZuschnittDateiAbspeichern(m_strZuschnittDatei.GetBuffer(m_strZuschnittDatei.GetLength()));
    m_strZuschnittDatei.ReleaseBuffer();
}

void CTtmainApp::OnUpdateZuschnittregelnSpeichern(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

int CTtmainApp::ExitInstance() 
{
	m_regManager.Close();

    AfxGetApp()->WriteProfileInt("Settings", "LastStartedMode", m_iLastMode);
	
	return CWinAppEx::ExitInstance();
}


// Function name	: CTtmainApp::CheckDirectory
// Description	    : durchsucht den String und erzeugt, falls n�tig die 
//                    Verzeichnisse, die er enth�lt
// Return type		: BOOL 
// Argument         : CString strFileName
BOOL CTtmainApp::CheckDirectory(CString strFileName)
{
    CString strDirectory;
    strDirectory = strFileName.Left(strFileName.ReverseFind('\\'));

    return CreateDirectory(strDirectory,  NULL);
}

void CTtmainApp::OnHoleProfileNew() 
{
    // Loch Profil Doc ist letztes
    POSITION pos = GetFirstDocTemplatePosition();
    CDocTemplate* pTmplt = GetNextDocTemplate(pos);
    if (pos != NULL)
    {
        pTmplt = GetNextDocTemplate(pos);
        if (pTmplt != NULL)
            pTmplt->OpenDocumentFile(NULL);        	
    }
}

void CTtmainApp::OnUpdateHoleProfileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

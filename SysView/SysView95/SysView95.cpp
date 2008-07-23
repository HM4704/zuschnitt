// SysView95.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SysView95.h"
#include "SysIntegrationDlg.h"
#include "MainFrm.h"
#include "StatLink.h"
#include "SysHook\SysHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSysView95App

BEGIN_MESSAGE_MAP(CSysView95App, CWinApp)
	//{{AFX_MSG_MAP(CSysView95App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SYSINTEGRATION, OnSysIntegration)
	ON_UPDATE_COMMAND_UI(ID_SYSINTEGRATION, OnUpdateSysIntegration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysView95App construction

CSysView95App::CSysView95App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    m_ridIntegration = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysView95App object

CSysView95App theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysView95App initialization

// Add a static BOOL that indicates whether the class was
// registered so that we can unregister it in ExitInstance
static BOOL bClassRegistered = FALSE;

BOOL CSysView95App::InitInstance()
{
    char cBufDir[MAX_PATH];

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    // limit our window on one instance
    if(!FirstInstance())            
      return FALSE;

    // Register our unique class name that we wish to use
    WNDCLASS wndcls;
    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
    wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wndcls.lpfnWndProc = ::DefWindowProc;
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different icon
    wndcls.hCursor = LoadCursor( IDC_ARROW );
    wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wndcls.lpszMenuName = NULL;
    // Specify our own class name for using FindWindow later
    wndcls.lpszClassName = SYSVIEW95_CLASS;
    // Register new class and exit if it fails
    if(!AfxRegisterClass(&wndcls))          
    {
        return FALSE;
    }          
    bClassRegistered = TRUE;


	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
//	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    m_regManager.Open(_T("Moosburger\\Sysview95"), 30, TRUE);


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

    if (GetModuleFileName(NULL, cBufDir, sizeof(cBufDir)) != 0)
    {
        BOOL bCreated;
        if (m_riAppPath.Connect(&m_regManager, _T(""), 
            _T("APP_PATH"), bCreated))
        {
            m_riAppPath.Update(FALSE);
            if (strcmp(cBufDir, m_riAppPath.GetVal()) != 0)
            {
                m_riAppPath = cBufDir;
                m_riAppPath.Update();
            }
        }
        if (m_ridIntegration.Connect(&m_regManager, _T(""), 
            _T("SYSTEM_INTEGRATION"), bCreated))
        {
            if (bCreated == TRUE) m_ridIntegration = 0;
            else
            {
                if (m_ridIntegration.GetVal() & SYS_INTEGRATION_MENU)
                {
                    if (!IsHookInstalled())
                    {
                        CString str = m_riAppPath.GetVal(); 
                        InstallHooks(str.GetBuffer(str.GetLength()));
                        str.ReleaseBuffer();
                    }
                }
            }
        }

        if (m_lpCmdLine)
        {
            if (strcmp(m_lpCmdLine, _T("HOOK")) == 0)
            {
                // Hook action
                InstallHooks(cBufDir);
                return FALSE;
            }
        }
    }
	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

BOOL CSysView95App::FirstInstance()      
{
    CWnd *pWndPrev, *pWndChild;

    // Determine if another window with our class name exists...
    if ((pWndPrev = CWnd::FindWindow(SYSVIEW95_CLASS,NULL)) != NULL)        
    {
        // if so, does it have any popups?
        pWndChild = pWndPrev->GetLastActivePopup();
        // If iconic, restore the main window
        if (pWndPrev->IsIconic())
         pWndPrev->ShowWindow(SW_RESTORE);
        // Bring the main window or its popup to          
        // the foreground
        pWndChild->SetForegroundWindow();
        // and we are done activating the previous one.          
        return FALSE;
    }        
    // First instance. Proceed as normal.        
    else      
        return TRUE;      
}
/////////////////////////////////////////////////////////////////////////////
// CSysView95App message handlers





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

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CStaticLink m_linkMail;

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

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_linkMail.SubclassDlgItem(IDC_LINK_MAIL, this);
    m_linkMail.m_link = _T("mailto:hermann.moosburger@ops.de");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CSysView95App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSysView95App message handlers


int CSysView95App::ExitInstance() 
{
    m_ridIntegration.Update();  // hier gehts anscheinend nicht!!!
	m_regManager.Close();

    if(bClassRegistered)
        ::UnregisterClass(SYSVIEW95_CLASS, AfxGetInstanceHandle());	

    return CWinApp::ExitInstance();
}

CRegistryManager* CSysView95App::GetRegistryManager()
{
    return &m_regManager;
}

void CSysView95App::OnSysIntegration() 
{
    DWORD dwIntegration = 0;
    CSysIntegrationDlg dlg(m_ridIntegration.GetVal());
    if (dlg.DoModal() == IDOK)
    {
        dlg.GetSettings(dwIntegration);
        if ((DWORD)m_ridIntegration.GetVal() != dwIntegration)
        {
            if (dwIntegration & SYS_INTEGRATION_MENU)
            {
                // es muß Hook installiert sein
                if (!IsHookInstalled())
                {
                    CString str = m_riAppPath.GetVal(); 
                    InstallHooks(str.GetBuffer(str.GetLength()));
                    str.ReleaseBuffer();
                }
            }
            else
            {
                if (IsHookInstalled())
                {
                    UnInstallHooks();
                }
            }
            if (dwIntegration & SYS_INTEGRATION_AUTO)
            {
                CString strCmdLine = m_riAppPath.GetVal();
                strCmdLine += _T(" HOOK");
                // es muß beim Boot eingestellt werden
                m_regManager.RegisterRunItem(_T("SysHook"), strCmdLine);
            }
            else
            {
                m_regManager.DeleteRunItem(_T("SysHook"));
            }
            m_ridIntegration = dwIntegration;
            m_ridIntegration.Update();
        }
    }
}

void CSysView95App::OnUpdateSysIntegration(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();	
}

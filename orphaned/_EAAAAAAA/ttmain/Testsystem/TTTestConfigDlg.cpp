// TTTestConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include <fstream.h>
#include "..\datatype.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "TTTestManager.h"
#include "TTTestConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTTestConfigDlg dialog


CTTTestConfigDlg::CTTTestConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTTestConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTTTestConfigDlg)
	m_bAlwaysConfig = FALSE;
	m_bBreakOnError = FALSE;
	m_strLogFile = _T("");
	m_bManual = FALSE;
	m_bShowLog = FALSE;
	m_strTestDir = _T("");
	m_strTestFile = _T("");
	//}}AFX_DATA_INIT
}

CTTTestConfigDlg::CTTTestConfigDlg(TTM_CONFIG* pConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CTTTestConfigDlg::IDD, pParent)
{
    m_Config = *pConfig;
}


void CTTTestConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTTTestConfigDlg)
	DDX_Check(pDX, IDC_ALWAYS_CONFIG, m_bAlwaysConfig);
	DDX_Check(pDX, IDC_BREAK_ON_ERROR, m_bBreakOnError);
	DDX_Text(pDX, IDC_LOG_FILE, m_strLogFile);
	DDX_Check(pDX, IDC_MANUAL, m_bManual);
	DDX_Check(pDX, IDC_SHOW_LOG, m_bShowLog);
	DDX_Text(pDX, IDC_TEST_DIR, m_strTestDir);
	DDX_Text(pDX, IDC_TEST_FILE, m_strTestFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTTTestConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CTTTestConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTTestConfigDlg message handlers

BOOL CTTTestConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CopyValues(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTTTestConfigDlg::CopyValues(BOOL bLoad)
{
    if (bLoad == TRUE)
    {
        // in Dialog-Elemente laden
	    m_bAlwaysConfig = m_Config.ribfConfig.bf.bShowCfgDlg;
	    m_bBreakOnError = m_Config.ribfConfig.bf.bBreakOnError;
	    m_strLogFile = m_Config.strLogFile;
	    m_bManual = m_Config.ribfConfig.bf.bManual;
	    m_bShowLog = m_Config.ribfConfig.bf.bShowLogFile;
	    m_strTestDir = m_Config.strBaseTestDir;
	    m_strTestFile = m_Config.strTestFile;

        UpdateData(FALSE);
    }
    else
    {
        // aus Dialog-Elementen holenn
        UpdateData(TRUE);

	    m_Config.ribfConfig.bf.bShowCfgDlg = m_bAlwaysConfig;
	    m_Config.ribfConfig.bf.bBreakOnError = m_bBreakOnError;
	    m_Config.strLogFile = m_strLogFile;
	    m_Config.ribfConfig.bf.bManual = m_bManual;
	    m_Config.ribfConfig.bf.bShowLogFile = m_bShowLog;
	    m_Config.strBaseTestDir = m_strTestDir;
	    m_Config.strTestFile = m_strTestFile;
    }

}

void CTTTestConfigDlg::OnOK() 
{
    CopyValues(FALSE);
	
	CDialog::OnOK();
}

void CTTTestConfigDlg::GetConfigData(TTM_CONFIG* pConfig)
{
    *pConfig = m_Config;
}
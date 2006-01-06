#if !defined(AFX_TTTESTCONFIGDLG_H__732C708C_1DBC_41C7_89BC_659289F3901C__INCLUDED_)
#define AFX_TTTESTCONFIGDLG_H__732C708C_1DBC_41C7_89BC_659289F3901C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTTestConfigDlg.h : header file
//

#include "..\resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTTTestConfigDlg dialog

class CTTTestConfigDlg : public CDialog
{

    TTM_CONFIG m_Config;
// Construction
public:
	CTTTestConfigDlg(CWnd* pParent = NULL);   // standard constructor
    CTTTestConfigDlg(TTM_CONFIG* pConfig, CWnd* pParent /*=NULL*/);

    void GetConfigData(TTM_CONFIG* pConfig);
    
// Dialog Data
	//{{AFX_DATA(CTTTestConfigDlg)
	enum { IDD = IDD_TEST_CONFIG_DLG };
	BOOL	m_bAlwaysConfig;
	BOOL	m_bBreakOnError;
	CString	m_strLogFile;
	BOOL	m_bManual;
	BOOL	m_bShowLog;
	CString	m_strTestDir;
	CString	m_strTestFile;
	BOOL	m_bTestFuellung;
	BOOL	m_bTestProfil;
	BOOL	m_bTestFuellung600;
	BOOL	m_bTestProfil600;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTTestConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTTTestConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void CopyValues(BOOL bLoad);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTTESTCONFIGDLG_H__732C708C_1DBC_41C7_89BC_659289F3901C__INCLUDED_)

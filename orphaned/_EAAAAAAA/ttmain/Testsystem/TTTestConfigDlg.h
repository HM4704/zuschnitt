#if !defined(AFX_TTTESTCONFIGDLG_H__732C708C_1DBC_41C7_89BC_659289F3901C__INCLUDED_)
#define AFX_TTTESTCONFIGDLG_H__732C708C_1DBC_41C7_89BC_659289F3901C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTTestConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTTestConfigDlg dialog

class CTTTestConfigDlg : public CDialog
{
// Construction
public:
	CTTTestConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTTTestConfigDlg)
	enum { IDD = IDD_TEST_CONFIG_DLG };
		// NOTE: the ClassWizard will add data members here
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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTTESTCONFIGDLG_H__732C708C_1DBC_41C7_89BC_659289F3901C__INCLUDED_)

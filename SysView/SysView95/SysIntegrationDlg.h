#if !defined(AFX_SYSINTEGRATIONDLG_H__094A5B25_22C4_11D5_8E7C_B443017A4925__INCLUDED_)
#define AFX_SYSINTEGRATIONDLG_H__094A5B25_22C4_11D5_8E7C_B443017A4925__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysIntegrationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysIntegrationDlg dialog

class CSysIntegrationDlg : public CDialog
{
// Construction
public:
	BOOL GetSettings(DWORD& dwIntegration);
	CSysIntegrationDlg(CWnd* pParent = NULL);
	CSysIntegrationDlg(DWORD dwIntegrationFlags, 
        CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CSysIntegrationDlg)
	enum { IDD = IDD_SYS_INTEGRATION };
	CButton	m_ctrlIntMenu;
	CButton	m_ctrlIntAuto;
	//}}AFX_DATA

private:
	BOOL EnableControls(void);
    DWORD m_dwIntegration;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysIntegrationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysIntegrationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysIntMenu();
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSINTEGRATIONDLG_H__094A5B25_22C4_11D5_8E7C_B443017A4925__INCLUDED_)

#if !defined(AFX_ZRTREENEWDLG_H__CC31B141_B03D_11D5_8E7C_F4422503B455__INCLUDED_)
#define AFX_ZRTREENEWDLG_H__CC31B141_B03D_11D5_8E7C_F4422503B455__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRTreeNewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZRTreeNewDlg dialog

class CZRTreeNewDlg : public CDialog
{
// Construction
public:
	CZRTreeNewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CZRTreeNewDlg)
	enum { IDD = IDD_ZR_NEU_DLG };
	int		m_rbNewProperty;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZRTreeNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZRTreeNewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRTREENEWDLG_H__CC31B141_B03D_11D5_8E7C_F4422503B455__INCLUDED_)

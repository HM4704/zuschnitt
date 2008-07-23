#if !defined(AFX_SPAWNPROCESSDLG_H__F47ECE25_F16C_11D4_8E7B_CB3E523F3393__INCLUDED_)
#define AFX_SPAWNPROCESSDLG_H__F47ECE25_F16C_11D4_8E7B_CB3E523F3393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpawnProcessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpawnProcessDlg dialog

class CSpawnProcessDlg : public CDialog
{
// Construction
public:
	CSpawnProcessDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpawnProcessDlg)
	enum { IDD = IDD_SPAWN_PROCESS };
	CString	m_strProcessName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpawnProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpawnProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPAWNPROCESSDLG_H__F47ECE25_F16C_11D4_8E7B_CB3E523F3393__INCLUDED_)

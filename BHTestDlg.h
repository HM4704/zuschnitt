#if !defined(AFX_BHTESTDLG_H__3CECFF9B_316E_40C8_8847_C7FD9F7901D2__INCLUDED_)
#define AFX_BHTESTDLG_H__3CECFF9B_316E_40C8_8847_C7FD9F7901D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BHTestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBHTestDlg dialog

class CBHTestDlg : public CDialog
{
// Construction
public:
	CBHTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBHTestDlg)
	enum { IDD = IDD_BH_TEST };
	CString	m_strNewOffs2Hole;
	CString	m_strOffs2Hole;
	CString	m_strProfilLength;
	CString	m_strResult;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBHTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	int	m_iNewOffs2Hole;
	int	m_iOffs2Hole;
	int	m_iProfilLength;
	int	m_iResult;

// Implementation
    void AllMembersToValues(void);
    void AllMembersToStrings(void);

protected:

	// Generated message map functions
	//{{AFX_MSG(CBHTestDlg)
	afx_msg void OnBhTestCompute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BHTESTDLG_H__3CECFF9B_316E_40C8_8847_C7FD9F7901D2__INCLUDED_)

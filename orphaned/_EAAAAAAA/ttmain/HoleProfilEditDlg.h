#if !defined(AFX_HOLEPROFILEDITDLG_H__1304A551_40F2_4926_8A77_88FCCC8C3A63__INCLUDED_)
#define AFX_HOLEPROFILEDITDLG_H__1304A551_40F2_4926_8A77_88FCCC8C3A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoleProfilEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoleProfilEditDlg dialog

class CHoleProfilEditDlg : public CDialog
{
// Construction
public:
	CHoleProfilEditDlg(CHoleProfile* pProfile, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHoleProfilEditDlg)
	enum { IDD = IDD_BH_PROFIL_EDIT_DLG };
	int		m_iCount;
	int		m_iLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoleProfilEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    CHoleProfile* m_pProfile;

    void CopyValues(BOOL bLoad);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHoleProfilEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLEPROFILEDITDLG_H__1304A551_40F2_4926_8A77_88FCCC8C3A63__INCLUDED_)

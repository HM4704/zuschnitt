#if !defined(AFX_HOLEPROFILEDLG_H__F7977F9E_8640_4D21_AC6D_602A2E383BF1__INCLUDED_)
#define AFX_HOLEPROFILEDLG_H__F7977F9E_8640_4D21_AC6D_602A2E383BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoleProfileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDlg dialog

class CHoleProfileDlg : public CDialog
{
// Construction
public:
	CHoleProfileDlg(CPtrArray* pContainer, HP_OPT_SETTINGS* pSetsOpt, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHoleProfileDlg)
	enum { IDD = IDD_BH_PROFILE_DLG };
	CButton	m_btProfilEdit;
	CButton	m_btProfilDel;
	CListCtrl	m_lcProfilList;
	int		m_iOptTime;
	BOOL	m_bDoProfileOpt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoleProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    CPtrArray* m_pContainer;
    HP_OPT_SETTINGS* m_pSetsOpt;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHoleProfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBhProfilNew();
	virtual void OnOK();
	afx_msg void OnBhProfilEdit();
	afx_msg void OnBhProfilDel();
	afx_msg void OnItemActivateProfilList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickProfilList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkProfilList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void InitProfilList(void);
    void FillList(void);
    void InsertProfil(CHoleProfile* pProfil);
    void UpdateButtons(void);
    void CopyOptSettings(BOOL bLoad);



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLEPROFILEDLG_H__F7977F9E_8640_4D21_AC6D_602A2E383BF1__INCLUDED_)

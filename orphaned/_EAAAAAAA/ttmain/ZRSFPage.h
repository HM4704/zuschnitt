#if !defined(AFX_ZRSFPAGE_H__67ED68E5_B9BC_11D5_8E7C_BB7956C7DF54__INCLUDED_)
#define AFX_ZRSFPAGE_H__67ED68E5_B9BC_11D5_8E7C_BB7956C7DF54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRSFPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZRSFPage dialog

class CZRSFPage : public CZRBasePage
{
	DECLARE_DYNCREATE(CZRSFPage)

private:
    CZuschnittManager*  m_pMan;
    BOOL                m_bChanged;

// Construction
public:
	void ResetChanged(void);
    CZRSFPage();
    CZRSFPage(char* strTitle, CZuschnittManager* pMan);
	~CZRSFPage();

// Dialog Data
	//{{AFX_DATA(CZRSFPage)
	enum { IDD = IDD_ZR_SF_DLG };
	CEdit	m_ctrlSF2Senk400;
	CEdit	m_ctrlSF2Quer400;
	CEdit	m_ctrlSFSenk400;
	CEdit	m_ctrlSFQuer400;
	CEdit	m_ctrlSF2Senk;
	CEdit	m_ctrlSF2Quer;
	CEdit	m_ctrlSFSenk;
	CEdit	m_ctrlSFQuer;
	CString	m_strSFQuer;
	CString	m_strSFSenk;
	CString	m_strSF2Quer;
	CString	m_strSF2Senk;
	CString	m_strSFQuer400;
	CString	m_strSFSenk400;
	CString	m_strSF2Quer400;
	CString	m_strSF2Senk400;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CZRSFPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnQueryCancel();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void GetButtonStates(SetButtonsStruct* pButtons);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CZRSFPage)
	afx_msg void OnChangePage();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRSFPAGE_H__67ED68E5_B9BC_11D5_8E7C_BB7956C7DF54__INCLUDED_)

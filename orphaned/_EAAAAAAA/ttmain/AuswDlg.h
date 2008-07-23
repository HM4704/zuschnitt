#if !defined(AFX_AUSWDLG_H__141C613C_1243_11D4_8E7B_F82D876B953C__INCLUDED_)
#define AFX_AUSWDLG_H__141C613C_1243_11D4_8E7B_F82D876B953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AuswDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAuswDlg dialog

class CAuswDlg : public CDialog
{
// Construction
public:
	CAuswDlg(CWnd* pParent = NULL);   // standard constructor
	CAuswDlg(CWnd* pParent, char*, CTorDoor*, BOOL mod = FALSE);
	~CAuswDlg();
// Dialog Data
	char* m_strKundName;
	CTorDoor* m_pTor;
	char m_cTorTyp;
	BOOL m_bModify;

	//{{AFX_DATA(CAuswDlg)
	enum { IDD = IDD_TTAUSWDLG };
	CButton	m_ckBogen;
	CButton	m_cckOKF;
	CEdit	m_cedFuellung;
	CComboBox	m_cbFuellung;
	CStatic	m_cedTorTyp;
	CComboBox	m_cbTTorTyp;
	CString	m_edKunde;
	CString	m_edKommission;
	CString	m_edHoehe;
	CString	m_edOberHoehe;
	CString	m_edFuellung;
	CString	m_edAnzahl;
	CString	m_edBreite;
	CString	m_edSehnenHoehe;
	CString	m_edProfilDicke;
	//}}AFX_DATA

	CButton*   m_ckGeteilt[5];
	CComboBox*   m_cbFenster[5];
	CComboBox*	m_cbGlasart[5];
	CComboBox*	m_cbOberTeil[5];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuswDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK( );
	//}}AFX_VIRTUAL

// Implementation
	void EnableControls(CString str);
	void SetFuellung(CString str);
	void GetFluegelItems();
protected:

	// Generated message map functions
	//{{AFX_MSG(CAuswDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTtortyp();
	afx_msg void OnSelchangeFuellung();
	afx_msg void OnChangeFuellungStr();
	afx_msg void OnBogen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUSWDLG_H__141C613C_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

#include "afxwin.h"
#if !defined(AFX_AUSWDLG_H__141C613C_1243_11D4_8E7B_F82D876B953C__INCLUDED_)
#define AFX_AUSWDLG_H__141C613C_1243_11D4_8E7B_F82D876B953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AuswDlg.h : header file
//
#include "PersistenceManager.h"

/////////////////////////////////////////////////////////////////////////////
// CAuswDlg dialog

class CAuswDlg : public CDialog
{
// Construction
    CPersistenceManager* m_persMan;
public:
	CAuswDlg(CPersistenceManager* persMan, CWnd* pParent = NULL);   // standard constructor
	CAuswDlg(CWnd* pParent, char*, CTorDoor*, CPersistenceManager* persMan, BOOL mod = FALSE);
	~CAuswDlg();
// Dialog Data
	char* m_strKundName;
	CTorDoor* m_pTor;
	char m_cTorTyp;
	BOOL m_bModify;
    int  m_iCurFlCnt;       // aktuelle Fluegel-Anzahl

	//{{AFX_DATA(CAuswDlg)
	enum { IDD = IDD_TTAUSWDLG };
	CComboBox	m_cbProfilMass;
	CButton	m_ck400R;
	CEdit	m_edBreiteFl1;
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
	CString	m_strBreiteFl1;
	//}}AFX_DATA

	CButton*   m_ckGeteilt[5];
	CComboBox*  m_cbFenster[5];
	CComboBox*	m_cbGlasart[5];
	CComboBox*	m_cbOberTeil[5];
	CComboBox*	m_cbSprosse[5];

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
    void AddBetoColor(const char* strFuellung, const char* szColor);
    void AddHolzArt(char* szBuf, BOOL outside, char* szHolz);
    void AddRemoveStyropor(bool add, int aNr);
    bool FuellungMitStyropor(void);

protected:

	// Generated message map functions
	//{{AFX_MSG(CAuswDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTtortyp();
	afx_msg void OnSelchangeFuellung();
	afx_msg void OnSelchangeCustomFuellung();
	afx_msg void OnChangeFuellungStr();
	afx_msg void OnBogen();
	afx_msg void OnChangeBreite();
	afx_msg void OnClicked400R();
    afx_msg void OnCbnSelchangeHolzAussen();
    afx_msg void OnCbnSelchangeHolzInnen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CComboBox m_ctrlBetoColor;
public:
    afx_msg void OnCbnSelchangeBetoColor();
    CComboBox m_cbKommission;
    CComboBox m_cbKunde;
private:
    CComboBox m_cbPosition;
public:
    CString m_edPosition;

private:
public:
    CComboBox m_cbHolzInnen;
    CString m_edHolzInnen;
    CComboBox m_cbHolzAussen;
    CComboBox m_ctrlSchlossArt;
    afx_msg void OnBnClickedStyropor();
    CButton m_ctrlStyropor;
    CComboBox m_ctrlTEBLinks;
    CComboBox m_ctrlTEBRechts;
    CComboBox m_ctrlTEBOben;
    CComboBox m_ctrlTEBUnten;
    CComboBox m_ctrlTorFestLinks;
    CComboBox m_ctrlTorFestRechts;
    CComboBox m_cbZWidth;
    CComboBox m_cbBand;
    CString m_edTextUnten;
    CComboBox m_cbKlappgriff;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUSWDLG_H__141C613C_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

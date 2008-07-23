#if !defined(AFX_KREISBERECHNUNGDLG_H__BAAE0F21_DE7B_11D4_8E7B_ABD7149E7437__INCLUDED_)
#define AFX_KREISBERECHNUNGDLG_H__BAAE0F21_DE7B_11D4_8E7B_ABD7149E7437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KreisBerechnungDlg.h : header file
//

const char szKreisDlgHilfe[] = "kreisdlg_hilfe.htm";

/////////////////////////////////////////////////////////////////////////////
// CKreisBerechnung dialog

#define BERECHNUNG_RADIUS       0
#define BERECHNUNG_SEHNE        1
#define BERECHNUNG_SEHNENHOEHE  2

class CKreisBerechnungDlg : public CDialog
{
// Construction
public:
	BOOL Create();

	BOOL AllMembersToValues(void);
	BOOL BerechneSchnittwinkel(void);
	HFONT CreateFont(void);
    void OnBerechnen(); 
	BOOL SetzeBerechnung(int iWert);
	BOOL BerechneTangentenWinkel(void);
	BOOL BerechneSehnenHoehe(void);
	BOOL BerechneSehne(void);
	BOOL BerechneBogenlaenge(void);
	BOOL BerechneRadius(void);
	CKreisBerechnungDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKreisBerechnungDlg)
	enum { IDD = IDD_KREISBERECHNUNG };
	CButton	m_RadioBerechnen;
	CString	m_strRadius;
	CString	m_strBogenlaenge;
	CString	m_strBWinkelAussen;
	CString	m_strBWinkelMitte;
	CString	m_strProfilDicke;
	CString	m_strSehne;
	CString	m_strSehnenHoehe;
	CString	m_strSWinkelAussen;
	//}}AFX_DATA

private:
	CWnd* m_pParent;
	int m_nID;

    int     m_iBerechneterWert;
    CFont   m_fontEdit;

    double  m_dRadius;
	double	m_dSehne;
	double	m_dSehnenHoehe;
	double	m_dBogenlaenge;
	double	m_dProfilDicke;
	double	m_dBWinkelAussen;
	double	m_dBWinkelMitte;
	double	m_dSWinkelAussen;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKreisBerechnungDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL AllMembersToStrings(void);
	BOOL SendFontToEdits(CFont &font);
	BOOL EnableEdits(int iState);

	// Generated message map functions
	//{{AFX_MSG(CKreisBerechnungDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioRadius();
	afx_msg void OnRadioSehne();
	afx_msg void OnRadioSehnenhoehe();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBerechnewinkel();
	afx_msg void OnHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KREISBERECHNUNGDLG_H__BAAE0F21_DE7B_11D4_8E7B_ABD7149E7437__INCLUDED_)

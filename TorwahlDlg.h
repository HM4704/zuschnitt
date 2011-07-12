#if !defined(AFX_TORWAHLDLG_H__9DDA63A4_F7BA_11D4_8E7C_DD0314AEAC30__INCLUDED_)
#define AFX_TORWAHLDLG_H__9DDA63A4_F7BA_11D4_8E7C_DD0314AEAC30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TorwahlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTorwahlDlg dialog

class CTorwahlDlg : public CDialog
{
// Construction
public:
	int GewaehltesTor(void);
	CTorwahlDlg(CWnd* pParent = NULL);   // standard constructor
	CTorwahlDlg(CWnd* pParent, int iTorZahl);

// Dialog Data
	//{{AFX_DATA(CTorwahlDlg)
	enum { IDD = IDD_TORWAHL_DLG };
	CListBox	m_ctrTorList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTorwahlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
    int m_iTorZahl;
    int m_iGewaehltesTor;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTorwahlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkTorlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORWAHLDLG_H__9DDA63A4_F7BA_11D4_8E7C_DD0314AEAC30__INCLUDED_)

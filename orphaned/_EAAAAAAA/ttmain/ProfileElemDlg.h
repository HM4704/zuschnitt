#if !defined(AFX_PROFILEELEMDLG_H__A43E06FA_BBD9_4673_93BC_3997377778F5__INCLUDED_)
#define AFX_PROFILEELEMDLG_H__A43E06FA_BBD9_4673_93BC_3997377778F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileElemDlg.h : header file
//

#include "CBProfile.h"


/////////////////////////////////////////////////////////////////////////////
// CProfileElemDlg dialog

class CProfileElemDlg : public CDialog
{
    CProfileElem  m_Prfl;
    CTorDoor*     m_pTor;
// Construction
public:
	CProfileElemDlg(CWnd* pParent, CTorDoor* pTor);   // standard constructor
	CProfileElemDlg(CWnd* pParent, CTorDoor* pTor, CProfileElem* pPrfl);

// Dialog Data
	//{{AFX_DATA(CProfileElemDlg)
	enum { IDD = IDD_PROFILE_ELEM_DLG };
	CComboBoxEx	m_cbProfileOrientation;
	CComboBoxEx	m_cbProfileType;
	int		m_iProfileElemCount;
	int		m_iProfileElemLength;
	CString	m_strProfileElemType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileElemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
    void GetProfile(CProfileElem* pPrfl);

protected:
    void CopyValues(BOOL bLoad);

	// Generated message map functions
	//{{AFX_MSG(CProfileElemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEELEMDLG_H__A43E06FA_BBD9_4673_93BC_3997377778F5__INCLUDED_)

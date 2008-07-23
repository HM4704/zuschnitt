#if !defined(AFX_ZRTREEEDIT_H__DD0AB205_B8FD_11D5_8E7C_8E7A03DA9A55__INCLUDED_)
#define AFX_ZRTREEEDIT_H__DD0AB205_B8FD_11D5_8E7C_8E7A03DA9A55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRTreeEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZRTreeEdit window

class CZRTreeEdit : public CEdit
{
// Construction
public:
	CZRTreeEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZRTreeEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZRTreeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CZRTreeEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRTREEEDIT_H__DD0AB205_B8FD_11D5_8E7C_8E7A03DA9A55__INCLUDED_)

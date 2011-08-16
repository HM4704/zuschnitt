// MyPreviewView.h : header file
//

#include "afxpriv.h"

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView view
#define MAX_DLLS			50

class CPrintPreviewDialog;

class CMyPreviewView : public CPreviewView
{
protected:
	CMyPreviewView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyPreviewView)

// Attributes
public:

protected:
// Operations
	CPrintPreviewDialog*	m_pDialog;
public:
// Overrides
	void	SetDialogPointer(CPrintPreviewDialog* pDialog);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPreviewView)
protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyPreviewView();
#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPreviewView)
	//}}AFX_MSG
	afx_msg void	OnPreviewPrint();
	afx_msg void    OnUpdatePrevPage(CCmdUI* pCmdUI);
	afx_msg void    OnNextPage();
	afx_msg void    OnPrevPage();
	afx_msg void    OnUpdateNextPage(CCmdUI* pCmdUI);
	afx_msg void    OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void    OnUpdateZoomOut(CCmdUI* pCmdUI);

protected:
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

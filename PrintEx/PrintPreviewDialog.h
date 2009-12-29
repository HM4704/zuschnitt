#if !defined(AFX_PRINTPREVIEWDIALOG_H__9AB25DD7_06B1_11D7_ADB6_00B0D0652E95__INCLUDED_)
#define AFX_PRINTPREVIEWDIALOG_H__9AB25DD7_06B1_11D7_ADB6_00B0D0652E95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintPreviewDialog.h : header file
//
// Note that this is a base class, not meant to be built itself.
// You should inherit from it

/////////////////////////////////////////////////////////////////////////////
// CPrintPreviewDialog dialog
class CDummyView;

class _PE_API CPrintPreviewDialog : public CDialog
{
// Construction
public:
	CPrintPreviewDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) : CDialog(lpszTemplateName, pParentWnd)
	{
		m_pTemplate = NULL;
		m_bPrintPreview = false;
		m_pView = NULL;
	};
	CPrintPreviewDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) : CDialog(nIDTemplate, pParentWnd)
	{
		m_pTemplate = NULL;
		m_bPrintPreview = false;
		m_pView = NULL;
	};
	virtual ~CPrintPreviewDialog()
	{
		delete m_pTemplate;
		m_pTemplate = NULL;
	};
	void EndPreviewMode();

// Dialog Data
	//{{AFX_DATA(CPrintPreviewDialog)
	//}}AFX_DATA
private:
	bool		m_bPrintPreview;
	CDummyView* m_pView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintPreviewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
//	virtual BOOL DoPrintPreview(UINT nIDResource, CView* pPrintView, CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
protected:
	CSingleDocTemplate*	m_pTemplate;
	// Generated message map functions
	//{{AFX_MSG(CPrintPreviewDialog)
	afx_msg void OnDialogPrint();
	afx_msg void OnDialogPrintPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTPREVIEWDIALOG_H__9AB25DD7_06B1_11D7_ADB6_00B0D0652E95__INCLUDED_)

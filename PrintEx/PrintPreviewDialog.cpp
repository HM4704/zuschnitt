// PrintPreviewDialog.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#include "PrintPreviewDialog.h"
#include "DummyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintPreviewDialog dialog

void CPrintPreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintPreviewDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintPreviewDialog, CDialog)
	//{{AFX_MSG_MAP(CPrintPreviewDialog)
	ON_COMMAND(ID_FILE_PRINT, OnDialogPrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnDialogPrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintPreviewDialog message handlers
BOOL CPrintPreviewDialog::OnPreparePrinting(CPrintInfo* pInfo)
{
	return m_pView->DoPreparePrinting(pInfo);
}

void CPrintPreviewDialog::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	UNUSED_PARAMETER(pDC);
	UNUSED_PARAMETER(pInfo);
}

void CPrintPreviewDialog::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	UNUSED_PARAMETER(pDC);
	UNUSED_PARAMETER(pInfo);
}

void CPrintPreviewDialog::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	UNUSED_PARAMETER(pDC);
	UNUSED_PARAMETER(pInfo);
}

void CPrintPreviewDialog::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	UNUSED_PARAMETER(pDC);
	UNUSED_PARAMETER(pInfo);
}

void CPrintPreviewDialog::OnDialogPrint()
{
	CDummyView	view;

	view.SetPrintDialog(this);
	view.OnDoFilePrint();
}

void CPrintPreviewDialog::OnDialogPrintPreview()
{
	CFrameWnd* pOldFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	//pOldFrame->ShowWindow(SW_HIDE);
	if (!m_pTemplate)
	{
		m_pTemplate = new CSingleDocTemplate(
			128,					// so load fram is happy. This matches the IDR_MAINFRAME value from your application!
			NULL,
			RUNTIME_CLASS(CFrameWnd),
			RUNTIME_CLASS(CDummyView));
//		AfxGetApp()->AddDocTemplate(m_pTemplate);
	}
	CFrameWnd* pFrameWnd = m_pTemplate->CreateNewFrame(NULL, NULL);
    HWND hWnd = pFrameWnd->GetSafeHwnd();
    LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
    lStyle &= ~WS_SYSMENU ;
    ::SetWindowLong(hWnd, GWL_STYLE, lStyle);

	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
#if 0
	CPrintPreviewState* pState = new CPrintPreviewState;
	UNUSED_PARAMETER(pState);
#endif // 0
	m_bPrintPreview = true;
	m_pTemplate->InitialUpdateFrame(pFrameWnd, NULL, FALSE);
	m_pView = (CDummyView*)pFrameWnd->GetActiveView();
	AfxGetApp()->m_pMainWnd = pFrameWnd;
	pFrameWnd->SetWindowText(_T("Druckvorschau"));
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pView->SetPrintDialog(this);
	m_pView->m_pOldFrame = pOldFrame;
	m_pView->OnDoPrintPreview();
}

void CPrintPreviewDialog::EndPreviewMode()
{
	m_bPrintPreview = false;
}

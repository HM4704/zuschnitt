#include "stdafx.h"
#include "MyPreviewView.h"
#include "DummyView.h"

IMPLEMENT_DYNCREATE(CMyPreviewView, CPreviewView)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView

CMyPreviewView::CMyPreviewView()
{
	m_pDialog = NULL;
}

CMyPreviewView::~CMyPreviewView()
{
}

BEGIN_MESSAGE_MAP(CMyPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CMyPreviewView)
	//}}AFX_MSG_MAP
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_COMMAND(AFX_ID_PREVIEW_NEXT,  OnNextPage)
	ON_COMMAND(AFX_ID_PREVIEW_PREV,  OnPrevPage)

	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NEXT, OnUpdateNextPage)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_PREV, OnUpdatePrevPage)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMIN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMOUT, OnUpdateZoomOut)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView drawing

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView diagnostics
#ifdef _DEBUG
void CMyPreviewView::AssertValid() const
{
	// Bug nearby Article ID: Q192853 
	//	CPreviewView::AssertValid();
}

void CMyPreviewView::Dump(CDumpContext& dc) const
{
	CPreviewView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView message handlers

void CMyPreviewView::OnPreviewPrint()
{
	if (m_pDialog != NULL)
	{
		m_pDialog->PostMessage(WM_COMMAND, ID_FILE_PRINT);
	}
	else
	{
		// the view created for the dialog needs to set the m_pDialog pointer
		ASSERT(FALSE);
	}
	OnPreviewClose();               // force close of Preview
}

void CMyPreviewView::OnNextPage()
{
 	SetCurrentPage(m_nCurrentPage + 1, TRUE);

    m_pToolBar->PostMessage(WM_PAINT, 0, 0);
}

void CMyPreviewView::OnPrevPage()
{
    if (m_nCurrentPage > 0)
	    SetCurrentPage(m_nCurrentPage - 1, TRUE);
}

void CMyPreviewView::OnUpdateNextPage(CCmdUI* pCmdUI)
{
	// enable if not showing last page
	pCmdUI->Enable(m_nCurrentPage+m_nPages-1 < m_pPreviewInfo->GetMaxPage());
}

void CMyPreviewView::OnUpdatePrevPage(CCmdUI* pCmdUI)
{
	// enable if not showing First page
	pCmdUI->Enable(/*m_nCurrentPage > m_pPreviewInfo->GetMinPage()*/TRUE);
}

void CMyPreviewView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(/*m_nZoomState != ZOOM_IN*/TRUE);
}

void CMyPreviewView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(/*m_nZoomState != ZOOM_OUT*/TRUE);
}

void CMyPreviewView::SetDialogPointer(CPrintPreviewDialog* pDialog)
{
	m_pDialog = pDialog;
}



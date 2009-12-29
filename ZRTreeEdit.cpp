// ZRTreeEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRTreeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZRTreeEdit

CZRTreeEdit::CZRTreeEdit()
{
}

CZRTreeEdit::~CZRTreeEdit()
{
}


BEGIN_MESSAGE_MAP(CZRTreeEdit, CEdit)
	//{{AFX_MSG_MAP(CZRTreeEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRTreeEdit message handlers

HBRUSH CZRTreeEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetStyle()|
        ES_WANTRETURN|ES_MULTILINE|ES_AUTOVSCROLL/*|ES_NUMBER*/);
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CZRTreeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == '\n')
    {
        // return was pressed, close this window
        PostMessage(WM_CLOSE, 0, 0);
    }
    else
    {
        // nur Zahlen auch negativ zulassen
        if ((nChar >= '0' && nChar <= '9') || (nChar == '-') || (nChar == '\b'))
	        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}

void CZRTreeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

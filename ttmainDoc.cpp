// ttmainDoc.cpp : implementation of the CTtmainDoc class
//

#include "stdafx.h"
//#include "ttmain.h"
#include "ttmainDoc.h"
#include "ttmainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTtmainDoc

IMPLEMENT_DYNCREATE(CTtmainDoc, CDocument)

BEGIN_MESSAGE_MAP(CTtmainDoc, CDocument)
	//{{AFX_MSG_MAP(CTtmainDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTtmainDoc construction/destruction

CTtmainDoc::CTtmainDoc()
{
	// TODO: add one-time construction code here

}

CTtmainDoc::~CTtmainDoc()
{
}

BOOL CTtmainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTtmainDoc serialization

void CTtmainDoc::Serialize(CArchive& ar)
{
    POSITION pos = GetFirstViewPosition();
    CTtmainView* pView = (CTtmainView*)GetNextView(pos);
    if (pView)
    {
	    if (ar.IsStoring())
	    {
		    // TODO: add storing code here
            pView->saveAll(ar);
	    }
	    else
	    {
		    // TODO: add loading code here
	    }
    }
    else
    {

    }
}

/////////////////////////////////////////////////////////////////////////////
// CTtmainDoc diagnostics

#ifdef _DEBUG
void CTtmainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTtmainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTtmainDoc commands

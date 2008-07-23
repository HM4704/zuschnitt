// ZRBasePage.cpp : implementation file
//

#include "stdafx.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRSheet.h"
#include "ZRBasePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZRBasePage property page

IMPLEMENT_DYNCREATE(CZRBasePage, CPropertyPage)

CZRBasePage::CZRBasePage(char* strTitle, UINT nId) : CPropertyPage(nId)
{
	//{{AFX_DATA_INIT(CZRBasePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_strTitle = strTitle;
}

CZRBasePage::~CZRBasePage()
{
}

void CZRBasePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZRBasePage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZRBasePage, CPropertyPage)
	//{{AFX_MSG_MAP(CZRBasePage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRBasePage message handlers

void CZRBasePage::SendButtonStates()
{
    CWnd* pParent = GetParent();
    if (pParent != NULL)
    {
        // Button-Zustände ermitteln und an Sheet senden
        SetButtonsStruct buttons;
        GetButtonStates(&buttons);
        pParent->SendMessage(WM_ZR_SHEET_SET_BUTTONS, 0, (LPARAM)&buttons);
    }
}

BOOL CZRBasePage::OnSetActive() 
{
    SendButtonStates();
    CWnd* pParent = GetParent();
    if (pParent)
        pParent->SetFocus();
    Invalidate(TRUE);
	
	return CPropertyPage::OnSetActive();
}

LONG CZRBasePage::SignalPageChanged()
{
    CWnd* pParent = GetParent();
    if (pParent)
    {
        SetButtonsStruct buttons;
        GetButtonStates(&buttons);
        pParent->SendMessage(WM_ZR_SHEET_PAGED_CHANGED, 0, (LPARAM)&buttons);
    }
    return 0L;
}

// TTTestConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttmain.h"
#include "TTTestConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTTestConfigDlg dialog


CTTTestConfigDlg::CTTTestConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTTestConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTTTestConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTTTestConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTTTestConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTTTestConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CTTTestConfigDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTTestConfigDlg message handlers

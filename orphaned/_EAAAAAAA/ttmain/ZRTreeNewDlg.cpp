// ZRTreeNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRTreeNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZRTreeNewDlg dialog


CZRTreeNewDlg::CZRTreeNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZRTreeNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZRTreeNewDlg)
	m_rbNewProperty = 0;
	//}}AFX_DATA_INIT
}


void CZRTreeNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZRTreeNewDlg)
	DDX_Radio(pDX, IDC_ZR_NEW_PROPERTY, m_rbNewProperty);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZRTreeNewDlg, CDialog)
	//{{AFX_MSG_MAP(CZRTreeNewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRTreeNewDlg message handlers

BOOL CZRTreeNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

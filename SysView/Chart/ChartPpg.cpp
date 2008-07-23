// ChartPpg.cpp : Implementation of the CChartPropPage property page class.

#include "stdafx.h"
#include "Chart.h"
#include "ChartPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CChartPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CChartPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CChartPropPage)
	ON_BN_CLICKED(IDC_CHANGEGRIDCOLOR, OnChangegridcolor)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHANGELINECOLOR, OnChangelinecolor)
	ON_BN_CLICKED(IDC_CHANGEBGCOLOR, OnChangebgcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CChartPropPage, "CHART.ChartPropPage.1",
	0x7186232f, 0xceac, 0x11d4, 0x8e, 0x7b, 0xc3, 0xbd, 0xf0, 0xd3, 0x8d, 0x3c)


/////////////////////////////////////////////////////////////////////////////
// CChartPropPage::CChartPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CChartPropPage

BOOL CChartPropPage::CChartPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CHART_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CChartPropPage::CChartPropPage - Constructor

CChartPropPage::CChartPropPage() :
	COlePropertyPage(IDD, IDS_CHART_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CChartPropPage)
	m_refreshTime = 0;
	m_gridColor = 0;
	m_lineColor = 0;
	m_bgColor = 0;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CChartPropPage::DoDataExchange - Moves data between page and properties

void CChartPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CChartPropPage)
	DDP_Text(pDX, IDC_REFRESH, m_refreshTime, _T("RefreshTime") );
	DDX_Text(pDX, IDC_REFRESH, m_refreshTime);
	DDP_Text(pDX, IDC_GRIDCOLOR, m_gridColor, _T("GridColor") );
	DDX_Text(pDX, IDC_GRIDCOLOR, m_gridColor);
	DDP_Text(pDX, IDC_LINECOLOR, m_lineColor, _T("LineColor") );
	DDX_Text(pDX, IDC_LINECOLOR, m_lineColor);
	DDP_Text(pDX, IDC_BGCOLOR, m_bgColor, _T("BgColor") );
	DDX_Text(pDX, IDC_BGCOLOR, m_bgColor);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CChartPropPage message handlers

BOOL CChartPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChartPropPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    FillFrame(&dc, m_gridColor, IDC_GRIDCOLORFRAME);
	FillFrame(&dc, m_lineColor, IDC_LINECOLORFRAME);
    FillFrame(&dc, m_bgColor, IDC_BGCOLORFRAME);
	// Do not call COlePropertyPage::OnPaint() for painting messages
}

void CChartPropPage::OnChangegridcolor() 
{
    DoColorDialog(m_gridColor, IDC_GRIDCOLOR);
}

void CChartPropPage::OnChangelinecolor() 
{
    DoColorDialog(m_lineColor, IDC_LINECOLOR);	
}

void CChartPropPage::OnChangebgcolor() 
{
    DoColorDialog(m_bgColor, IDC_BGCOLOR);
}

void CChartPropPage::FillFrame(CDC* pDC, UINT dwColor, int nId)
{
    CBrush Brush(dwColor);
    CRect rect;
    GetDlgItem(nId)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    pDC->FillRect(rect, &Brush);
}

void CChartPropPage::DoColorDialog(UINT &dwColor, int nId)
{
    char cBuf[100];

    CColorDialog dlg( dwColor, CC_ANYCOLOR, this);
    
    dlg.DoModal();
    dwColor = dlg.GetColor();
    itoa(dwColor, cBuf, 10);
    GetDlgItem(nId)->SetWindowText(cBuf);
    InvalidateRect(NULL, FALSE);
}

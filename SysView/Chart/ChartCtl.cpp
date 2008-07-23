// ChartCtl.cpp : Implementation of the CChartCtrl ActiveX Control class.

#include "stdafx.h"
#include "Chart.h"
#include "ChartCtl.h"
#include "ChartPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _OUTPUT 

#define SAFE_DELETE(PDATA)      if (PDATA != NULL)      \
                                {                       \
                                    delete PDATA;       \
                                    PDATA = NULL;       \
                                }                       

#define SAFE_DELETE_ARR(PDATA)  if (PDATA != NULL)      \
                                {                       \
                                    delete [] PDATA;       \
                                    PDATA = NULL;       \
                                }                       

#define ALIGN_BUFFER(POS, SIZE)  if (POS >= SIZE)   \
                                 {                  \
                                    POS = POS-SIZE; \
                                 }

IMPLEMENT_DYNCREATE(CChartCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CChartCtrl, COleControl)
	//{{AFX_MSG_MAP(CChartCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_OPTIONS, OnOptions)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS, OnUpdateOptions)
	ON_COMMAND(ID_RESET, OnReset)
	ON_UPDATE_COMMAND_UI(ID_RESET, OnUpdateReset)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CChartCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CChartCtrl)
	DISP_PROPERTY_NOTIFY(CChartCtrl, "GridColor", m_gridColor, OnGridColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CChartCtrl, "LineColor", m_lineColor, OnLineColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CChartCtrl, "RefreshTime", m_refreshTime, OnRefreshTimeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CChartCtrl, "BgColor", m_bgColor, OnBgColorChanged, VT_COLOR)
	DISP_FUNCTION(CChartCtrl, "MinMax", MinMax, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CChartCtrl, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CChartCtrl, "Update", Update, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CChartCtrl, "SetControlInfo", SetControlInfo, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_STOCKPROP_BACKCOLOR()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CChartCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CChartCtrl, COleControl)
	//{{AFX_EVENT_MAP(CChartCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CChartCtrl, 1)
	PROPPAGEID(CChartPropPage::guid)
END_PROPPAGEIDS(CChartCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CChartCtrl, "CHART.ChartCtrl.1",
	0x7186232e, 0xceac, 0x11d4, 0x8e, 0x7b, 0xc3, 0xbd, 0xf0, 0xd3, 0x8d, 0x3c)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CChartCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DChart =
		{ 0x7186232c, 0xceac, 0x11d4, { 0x8e, 0x7b, 0xc3, 0xbd, 0xf0, 0xd3, 0x8d, 0x3c } };
const IID BASED_CODE IID_DChartEvents =
		{ 0x7186232d, 0xceac, 0x11d4, { 0x8e, 0x7b, 0xc3, 0xbd, 0xf0, 0xd3, 0x8d, 0x3c } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwChartOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CChartCtrl, IDS_CHART, _dwChartOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::CChartCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CChartCtrl

BOOL CChartCtrl::CChartCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CHART,
			IDB_CHART,
			afxRegApartmentThreading,
			_dwChartOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::CChartCtrl - Constructor

CChartCtrl::CChartCtrl()
{
	InitializeIIDs(&IID_DChart, &IID_DChartEvents);

    CreateChart(RGB(0, 255, 0), RGB(255, 0, 0));
    MinMax(0, 100);

    m_sizeControl.cx = 0; m_sizeControl.cy = 0;

    m_Menu.LoadMenu(IDR_CHARTMENU);

    m_pManager = NULL;
}

CChartCtrl::CreateChart(COLORREF rgbLine, COLORREF rgbGrid)
{
    int cx, cy;
    GetControlSize(&cx, &cy);    
    m_rectChart.left = 0;
    m_rectChart.right = cx;
    m_rectChart.top = 0;
    m_rectChart.bottom = cy;

    InitializeCriticalSection(&ChartCritSect);

    m_lineColor = TranslateColor(rgbLine);
    m_gridColor = TranslateColor(rgbGrid);
    m_bgColor   = TranslateColor(RGB(0, 0, 0));
    m_brBackground.CreateSolidBrush(m_bgColor);
    m_penLine.CreatePen(PS_SOLID, 1, m_lineColor);
    m_penBgLine.CreatePen(PS_SOLID, 1, m_gridColor);

    m_pmtField = NULL;
    m_pBitmap = NULL;
    m_pBgBitmap = NULL;

    Reset();    
}

/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::~CChartCtrl - Destructor

CChartCtrl::~CChartCtrl()
{
    // update registry
    if (m_pManager)
    {
        m_riBgColor.Update();
        m_riLineColor.Update();
        m_riGridColor.Update();
    }

    m_pBitmap->DeleteObject();
    SAFE_DELETE(m_pBitmap)

    SAFE_DELETE_ARR(m_pmtField)

    m_pBgBitmap->DeleteObject();
    SAFE_DELETE(m_pBgBitmap)

    m_Menu.DestroyMenu();
}


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::OnDraw - Drawing function

void CChartCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{

    CDC MemDC;
    MemDC.CreateCompatibleDC(pdc);
    CDC BgDC;
    BgDC.CreateCompatibleDC(pdc);

    m_pOldBitmap = MemDC.SelectObject(m_pBitmap);  
    m_pOldBgBmp = BgDC.SelectObject(m_pBgBitmap);  

    pdc->BitBlt(m_rectChart.left, m_rectChart.top, m_rectChart.Width(), 
        m_rectChart.Height(), &MemDC, 0, 0, SRCCOPY);

    MemDC.SelectObject(m_pOldBitmap);  
    BgDC.SelectObject(m_pOldBgBmp);  
}


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::DoPropExchange - Persistence support

void CChartCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

    PX_ULong(pPX, _T("LineColor"), m_lineColor, 
        TranslateColor(RGB(0, 255, 0)));
    PX_ULong(pPX, _T("GridColor"), m_gridColor, 
        TranslateColor(RGB(255, 0, 0)));
    PX_ULong(pPX, _T("BgColor"), m_bgColor, 
        TranslateColor(RGB(0, 0, 0)));
    PX_Long(pPX, _T("RefreshTime"), m_refreshTime, 2000);
}


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::OnResetState - Reset control to default state

void CChartCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl::AboutBox - Display an "About" box to the user

void CChartCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CHART);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl message handlers
void CChartCtrl::Start(CDC* pMemDC, CDC* pBgDC)
{
    CPaintDC    dc(this);

    if (m_pmtField == NULL)
    {
        m_pmtField = new DWORD[FIELD_SIZE];
    }

    if (m_pBitmap == NULL)
    {
        m_pBitmap = new CBitmap;
        m_pBitmap->CreateCompatibleBitmap(&dc, m_rectChart.Width(), 
            m_rectChart.Height());

    }
    m_pOldBitmap = pMemDC->SelectObject(m_pBitmap);  

    // Background-Bitmap erzeugen
    if (m_pBgBitmap == NULL)
    {
        m_pBgBitmap = new CBitmap;
        m_pBgBitmap->CreateCompatibleBitmap(&dc, m_rectChart.Width(), 
            m_rectChart.Height());
    }
    m_pOldBgBmp = pBgDC->SelectObject(m_pBgBitmap);  

    CreateBackground(pBgDC);

    Clear();

    pMemDC->SelectObject(m_pOldBitmap);  
    pBgDC->SelectObject(m_pOldBgBmp);  
}

void CChartCtrl::Restart(CDC *pMemDC, CDC *pBgDC, BOOLEAN bDraw)
{
    CPaintDC    dc(this);

    if (m_pmtField == NULL)
    {
        m_pmtField = new DWORD[FIELD_SIZE];
    }

    if (m_pBitmap == NULL)
    {
        m_pBitmap = new CBitmap;
        m_pBitmap->CreateCompatibleBitmap(&dc, m_rectChart.Width(), 
            m_rectChart.Height());

    }
    m_pOldBitmap = pMemDC->SelectObject(m_pBitmap);  

    // Background-Bitmap erzeugen
    if (m_pBgBitmap == NULL)
    {
        m_pBgBitmap = new CBitmap;
        m_pBgBitmap->CreateCompatibleBitmap(&dc, m_rectChart.Width(), 
            m_rectChart.Height());
    }
    m_pOldBgBmp = pBgDC->SelectObject(m_pBgBitmap);  

    CreateBackground(pBgDC);

    // Hintergrund einkopieren
    pMemDC->BitBlt(m_rectChart.left, m_rectChart.top, m_rectChart.Width(), 
        m_rectChart.Height(), pBgDC, 0, 0, SRCCOPY);

    if (bDraw == TRUE)
    {
        EnterCriticalSection(&ChartCritSect);

        if (m_dwValid == FIELD_SIZE)
            m_dwPosDraw = m_dwX+1;
        else
            m_dwPosDraw = 0;
        LeaveCriticalSection(&ChartCritSect);
        DrawChart(pMemDC, pBgDC);
    }

    pMemDC->SelectObject(m_pOldBitmap);  
    pBgDC->SelectObject(m_pOldBgBmp);  
}

void CChartCtrl::MinMax(long ulMin, long ulMax) 
{
    m_MinMax.iMin = ulMin;
    m_MinMax.iMax = ulMax;

    m_dwDelta = m_MinMax.iMax - m_MinMax.iMin;
}

void CChartCtrl::Reset(void)
{
    m_bRun = FALSE;

    m_dwX = m_dwValid = m_dwPosDraw = 0;
}

inline int CChartCtrl::MapRange(DWORD val)
{
  return (int)( ((float)((float)(m_MinMax.iMax - val)/(float)(m_MinMax.iMax - m_MinMax.iMin))
      *(m_rectChart.Height()-1)) );
}

BOOL CChartCtrl::DrawChart(CDC* pMemDC, CDC* pBgDC)
{
    BOOLEAN bOverflow = FALSE;
    EnterCriticalSection(&ChartCritSect);
    if (m_dwPosDraw == m_dwX)
    {
        return FALSE;
    }
     
    // Anzeige nach hinten kopieren
    if (m_dwX < m_dwPosDraw)
    {
        // Pufferüberlauf
        pMemDC->BitBlt(0, 0, m_rectChart.Width()-(FIELD_SIZE-m_dwPosDraw),
            m_rectChart.Height(), pMemDC, (FIELD_SIZE-m_dwPosDraw), 0, SRCCOPY);

        pMemDC->MoveTo(m_rectChart.Width()-(FIELD_SIZE-m_dwPosDraw)-1, MapRange(m_pmtField[m_dwPosDraw]));

        DrawFromTo(pMemDC, pBgDC, m_dwPosDraw, FIELD_SIZE);
        m_dwPosDraw = 0;
        bOverflow = TRUE;
        pMemDC->BitBlt(0, 0, m_rectChart.Width()-(m_dwX-m_dwPosDraw),
            m_rectChart.Height(), pMemDC, m_dwX-m_dwPosDraw, 0, SRCCOPY);
        pMemDC->MoveTo(m_rectChart.Width()-(m_dwX-m_dwPosDraw)-1, MapRange(m_pmtField[29]));
    }

    int iY;
    if (bOverflow == FALSE)
    {
        int iX = m_dwPosDraw-1;
        if (iX < 0 )
            iX = FIELD_SIZE-1;
        iY = m_pmtField[iX];
#ifdef _OUTPUT
        char cBuf[200];
        sprintf(cBuf, "\n***MoveTo: x=%d, y=%d\n", m_rectChart.Width()-(m_dwX-m_dwPosDraw)-1, iY);
        OutputDebugString(cBuf);  
#endif
        pMemDC->BitBlt(0, 0, m_rectChart.Width()-(m_dwX-m_dwPosDraw),
            m_rectChart.Height(), pMemDC, m_dwX-m_dwPosDraw, 0, SRCCOPY);
        pMemDC->MoveTo(m_rectChart.Width()-(m_dwX-m_dwPosDraw)-1, MapRange(iY));
    }

    DrawFromTo(pMemDC, pBgDC, m_dwPosDraw, m_dwX);

    m_dwPosDraw = m_dwX;

    LeaveCriticalSection(&ChartCritSect);

    return TRUE;
}

inline BOOL CChartCtrl::DrawFromTo(CDC* pMemDC, CDC* pBgDC, 
                                   DWORD dwFrom, DWORD dwTo)
{
    if (dwFrom == dwTo)
        return FALSE;

#ifdef _OUTPUT
    char cBuf[200];
    sprintf(cBuf, "\n***DrawFromTo: from=%d, to=%d\nChartRect: %d\n\n", dwFrom, dwTo,
        m_rectChart.Width());
    OutputDebugString(cBuf);  
#endif

    m_pOldPen = pMemDC->SelectObject(&m_penLine);

    // Löschen der vorherigen Anzeige
    int iFill;
    int iWidth = m_rectChart.Width();
    int iLength = dwTo - dwFrom;
    for (int i = iLength; i > 0; i -= iFill)
    {
        iFill = __min(iLength, m_rectChart.Width()-m_dwPosGrid);
        pMemDC->BitBlt(m_rectChart.Width()-(dwTo-dwFrom), 0, iFill+1, 
            m_rectChart.Height(), pBgDC, m_dwPosGrid, 
            0, SRCCOPY);
        m_dwPosGrid += iFill;
        if (m_dwPosGrid >= m_rectChart.Width())
            m_dwPosGrid = m_rectChart.Width()-2*m_dwXSpace+1;
    }

    for (i = dwFrom; i < dwTo; i++)
    {
#ifdef _OUTPUT
        char cBuf[200];
        CPoint point = pMemDC->GetCurrentPosition( );
        sprintf(cBuf, "Current Pos: x=%d, y=%d\n", point.x, point.y);
//        OutputDebugString(cBuf);  
        sprintf(cBuf, "LineTo: x=%d, y=%d\n", m_rectChart.Width()-dwTo+i, m_pmtField[i]);
        OutputDebugString(cBuf);  
#endif
        pMemDC->LineTo(m_rectChart.Width()-dwTo+i, MapRange(m_pmtField[i]));
    }

    pMemDC->SelectObject(m_pOldPen);
    return TRUE;
}


int CChartCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    CPaintDC dc(this);

    CDC* pMemDC = new CDC;
    pMemDC->CreateCompatibleDC(&dc);
    CDC* pBgDC = new CDC;
    pBgDC->CreateCompatibleDC(&dc);

	Start(pMemDC, pBgDC);
#if 0
    SetTimer( 1, m_refreshTime, NULL );
#endif
    delete pMemDC;
    delete pBgDC;
	return 0;
}

void CChartCtrl::Clear() 
{
    // Chart löschen
    CRect rect;

    CPaintDC dc(this);
    CDC* pMemDC = new CDC;
    pMemDC->CreateCompatibleDC(&dc);
    CDC* pBgDC = new CDC;
    pBgDC->CreateCompatibleDC(&dc);

    m_pOldBitmap = pMemDC->SelectObject(m_pBitmap);  
    m_pOldBgBmp = pBgDC->SelectObject(m_pBgBitmap);  

    EnterCriticalSection(&ChartCritSect);

    m_dwX = m_dwValid = m_dwPosDraw = 0;

    rect.left = rect.top = 0;
    rect.right = m_rectChart.Width();
    rect.bottom = m_rectChart.Height();

    m_dwPosGrid = m_rectChart.Width() - 2*m_dwXSpace+1;

    pMemDC->BitBlt(m_rectChart.left, m_rectChart.top, m_rectChart.Width(), 
        m_rectChart.Height(), pBgDC, 0, 0, SRCCOPY);


    LeaveCriticalSection(&ChartCritSect);

    InvalidateControl();
    pMemDC->SelectObject(m_pOldBitmap);  
    pBgDC->SelectObject(m_pOldBgBmp);  
    delete pMemDC;
    delete pBgDC;
}

void CChartCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);

    if (nType == SIZE_RESTORED)
    {
        if ((abs(m_sizeControl.cx-cx) > 1) || (abs(m_sizeControl.cy-cy) > 1))
        {
            m_sizeControl.cx = cx;
            m_sizeControl.cy = cy;

            m_rectChart.right = cx;
            m_rectChart.bottom = cy;

            SAFE_DELETE(m_pBitmap)


            SAFE_DELETE(m_pBgBitmap)

            CPaintDC dc(this);

            CDC* pMemDC = new CDC;
            pMemDC->CreateCompatibleDC(&dc);
            CDC* pBgDC = new CDC;
            pBgDC->CreateCompatibleDC(&dc);

//	        Restart(pMemDC, pBgDC);
            Redraw(pMemDC, pBgDC);
            InvalidateControl();
            delete pMemDC;
            delete pBgDC;
        }
    }
}

BOOL CChartCtrl::Update(long dwValue) 
{
    EnterCriticalSection(&ChartCritSect);
    m_pmtField[m_dwX] = dwValue;
#ifdef _OUTPUT
    char cBuf[200];
    sprintf(cBuf, "Update: x=%d, y=%d\n", m_dwX, dwValue);
    OutputDebugString(cBuf); 
#endif
    m_dwX++;
    if (m_dwX >= FIELD_SIZE)
        m_dwX = 0;
    if (m_dwValid < FIELD_SIZE)
        m_dwValid++;
    LeaveCriticalSection(&ChartCritSect);

    CPaintDC dc(this);
    CDC MemDC;
    MemDC.CreateCompatibleDC(&dc);
    CDC BgDC;
    BgDC.CreateCompatibleDC(&dc);

    m_pOldBitmap = MemDC.SelectObject(m_pBitmap);  
    m_pOldBgBmp = BgDC.SelectObject(m_pBgBitmap);  

    DrawChart(&MemDC, &BgDC);

    MemDC.SelectObject(m_pOldBitmap);  
    BgDC.SelectObject(m_pOldBgBmp);  

    InvalidateControl();
	return TRUE;
}

void CChartCtrl::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == 1)
    {
//        InvalidateControl();
    }
	
	COleControl::OnTimer(nIDEvent);
}

void CChartCtrl::CreateBackground(CDC* pBgDC)
{
#define MIN_SPACE_X     2      // minmaler Pixelabstand in X
#define MIN_SPACE_Y     2      // minmaler Pixelabstand in

    // Hintergrund ausmalen
    CRect rect;

    rect.left = rect.top = 0;
    rect.right = m_rectChart.Width();
    rect.bottom = m_rectChart.Height();

    pBgDC->FillRect(rect, &m_brBackground);

    // Raster erzeugen
    int x, y;
    m_dwXSpace = COLUMN_WIDTH;
    m_dwYSpace = m_rectChart.Height()/5;
    m_pOldBgBrush = pBgDC->SelectObject(&m_brBackground);
    m_pOldBgPen = pBgDC->SelectObject(&m_penBgLine);

    for (x = m_rectChart.Width()-1, y = 0; x >= 0; x -= m_dwXSpace)
    {
        pBgDC->MoveTo(x, y);
        pBgDC->LineTo(x, y + m_rectChart.Height());
    }

    for (y = MapRange(m_MinMax.iMin), x = 0; y > 0; y -= m_dwYSpace)
    {
        pBgDC->MoveTo(x, y);
        pBgDC->LineTo(x + m_rectChart.Width(), y);
    }
    m_dwPosGrid = m_rectChart.Width() - 2*m_dwXSpace+1;
    pBgDC->SelectObject(m_pOldBgBrush);
    pBgDC->SelectObject(m_pOldBgPen);
}


void CChartCtrl::OnLineColorChanged() 
{
    m_penLine.DeleteObject();
    m_penLine.CreatePen(PS_SOLID, 1, m_lineColor);

    ControlColorChanged();

//	SetModifiedFlag();
}

void CChartCtrl::OnGridColorChanged() 
{
    m_penBgLine.DeleteObject();
    m_penBgLine.CreatePen(PS_SOLID, 1, m_gridColor);

    ControlColorChanged();
    
//    SetModifiedFlag();
}

void CChartCtrl::OnBgColorChanged() 
{
    m_brBackground.DeleteObject();
    m_brBackground.CreateSolidBrush(m_bgColor);

    ControlColorChanged();

//	SetModifiedFlag();
}

BOOL CChartCtrl::ControlColorChanged(void)
{
    CPaintDC dc(this);

    CDC MemDC;
    MemDC.CreateCompatibleDC(&dc);
    CDC BgDC;
    BgDC.CreateCompatibleDC(&dc);

//    Restart(&MemDC, &BgDC);
    Redraw(&MemDC, &BgDC);
    InvalidateControl();

    SetModifiedFlag();

    // Update registry variables
    m_riBgColor = (DWORD) m_bgColor;
    m_riLineColor = (DWORD) m_lineColor;
    m_riGridColor = (DWORD) m_gridColor;

    return TRUE;
}


void CChartCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
    CMenu *pMenu = m_Menu.GetSubMenu(0);
    if (pMenu)
    {
        ClientToScreen(&point);
        pMenu->TrackPopupMenu(/*TPM_LEFTALIGN|TPM_RIGHTBUTTON*/0, 
            point.x, point.y, this, NULL);
    }

    // wenn drin, dann Probleme ????
//	COleControl::OnRButtonDown(nFlags, point);
}

void CChartCtrl::OnRefreshTimeChanged() 
{
#if 0
    KillTimer(1);
    SetTimer( 1, m_refreshTime, NULL );
#endif
	SetModifiedFlag();
}

void CChartCtrl::OnBackColorChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COleControl::OnBackColorChanged();
}

void CChartCtrl::OnOptions() 
{
    CRect rect;
    GetRectInContainer(&rect);
    OnProperties(NULL, GetParent()->m_hWnd, &rect);    	    	
}

void CChartCtrl::OnUpdateOptions(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(TRUE);	
}

void CChartCtrl::OnReset() 
{
	Clear();
}

void CChartCtrl::OnUpdateReset(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(TRUE);		
}


void CChartCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	COleControl::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

void CChartCtrl::Redraw(CDC *pMemDC, CDC *pBgDC)
{
    Restart(pMemDC, pBgDC, FALSE);

    m_pOldBitmap = pMemDC->SelectObject(m_pBitmap);  
    m_pOldPen = pMemDC->SelectObject(&m_penLine);

    EnterCriticalSection(&ChartCritSect);

    int iLength;
    if (m_dwValid == FIELD_SIZE)
    {
        m_dwPosDraw = m_dwX+1;
        iLength = FIELD_SIZE;
    }
    else
    {
        m_dwPosDraw = 0;
        iLength = m_dwX - m_dwPosDraw;
    }
    
    if (m_rectChart.Width() < iLength)
    {
        m_dwPosDraw += iLength - m_rectChart.Width();
        if (m_dwPosDraw >= FIELD_SIZE)
        {
            m_dwPosDraw -= FIELD_SIZE;
        }
        if (m_dwPosDraw < m_dwX)
            iLength = m_dwX - m_dwPosDraw;
        else
            iLength = FIELD_SIZE - m_dwPosDraw + m_dwX;
    }

    pMemDC->MoveTo(m_rectChart.Width() - iLength, MapRange(m_pmtField[m_dwPosDraw]));
    if (m_dwPosDraw < m_dwX)
    {
        for (int i = 0; i < iLength; i++)
            pMemDC->LineTo(m_rectChart.Width()-iLength+i, MapRange(m_pmtField[m_dwPosDraw+i]));
    }
    else
    {
        // Überlauf
        for (int i = 0; i < FIELD_SIZE-m_dwPosDraw; i++)
            pMemDC->LineTo(m_rectChart.Width()-iLength+i, MapRange(m_pmtField[m_dwPosDraw+i]));
        iLength -= i;
        for (i = 0; i <= m_dwX; i++)
            pMemDC->LineTo(m_rectChart.Width()-iLength+i, MapRange(m_pmtField[i]));
    }

    m_dwPosDraw = m_dwX;
    LeaveCriticalSection(&ChartCritSect);

    pMemDC->SelectObject(m_pOldPen);
    pMemDC->SelectObject(m_pOldBitmap);  
}

void CChartCtrl::SetControlInfo(LPCTSTR strControlName, LPUNKNOWN pManager) 
{
    m_strName = strControlName;
    m_pManager = (CRegistryManager*) pManager;
    SetupRegistryValues();
}

BOOL CChartCtrl::SetupRegistryValues()
{
    BOOL bCreated;
    BOOL bColorChanged = FALSE;

    if (m_pManager)
    {
        CString strSubKey, strValue;

        strValue = _T("BackgroundColor");
        m_riBgColor.Connect(m_pManager, m_strName,
            strValue, bCreated);
        if (bCreated == TRUE) m_riBgColor = 
            (DWORD)m_bgColor;
        else
        {
            m_bgColor = (OLE_COLOR) m_riBgColor.GetVal();
            m_brBackground.DeleteObject();
            m_brBackground.CreateSolidBrush(m_bgColor);
            bColorChanged = TRUE;
        }

        strValue = _T("GridColor");
        m_riGridColor.Connect(m_pManager, m_strName,
            strValue, bCreated);
        if (bCreated == TRUE) 
            m_riBgColor = (DWORD)m_gridColor;
        else
        {
            m_gridColor = (OLE_COLOR) m_riGridColor.GetVal();
            m_penBgLine.DeleteObject();
            m_penBgLine.CreatePen(PS_SOLID, 1, (OLE_COLOR) m_gridColor);

            bColorChanged = TRUE;
        }
        strValue = _T("LineColor");
        m_riLineColor.Connect(m_pManager, m_strName,
            strValue, bCreated);
        if (bCreated == TRUE) 
            m_riLineColor = (DWORD)m_lineColor;
        else
        {
            m_lineColor = (OLE_COLOR) m_riLineColor.GetVal();
            m_penLine.DeleteObject();
            m_penLine.CreatePen(PS_SOLID, 1, m_lineColor);

            bColorChanged = TRUE;
        }
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////// 
// CStaticLink 1997 Microsoft Systems Journal. 
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// CStaticLink implements a static control that's a hyperlink
// to any file on your desktop or web. You can use it in dialog boxes
// to create hyperlinks to web sites. When clicked, opens the file/URL
//

#include "StdAfx.h"
#include "resource.h"       // main symbols
#include "StatLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CStaticLink, CStatic)

BEGIN_MESSAGE_MAP(CStaticLink, CStatic)
	//{{AFX_MSG_MAP(CStaticLink)
	ON_WM_SETCURSOR()
    ON_WM_CTLCOLOR_REFLECT()    
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////
// Constructor sets default colors = blue/purple.
//
CStaticLink::CStaticLink()
{
    m_colorUnvisited = RGB(0,0,255);       // blue
    m_colorVisited   = RGB(128,0,128);     // purple
    m_bVisited       = FALSE;              // not visited yet
    m_bCaptured      = FALSE;
    m_hCursOrg       = NULL;
    m_hCursLink      = NULL;
}

//////////////////// Handle reflected WM_CTLCOLOR to set custom control color.
// For a text control, use visited/unvisited colors and underline font.
// For non-text controls, do nothing. Also ensures SS_NOTIFY is on.
//
HBRUSH CStaticLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    
    nCtlColor;   // only for avoiding warning

    ASSERT(nCtlColor == CTLCOLOR_STATIC);    
    DWORD dwStyle = GetStyle();
    if (!(dwStyle & SS_NOTIFY)) 
    {
        // Turn on notify flag to get mouse messages and STN_CLICKED.
        // Otherwise, I'll never get any mouse clicks!
        ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);    
    }    
    HBRUSH hbr = NULL;    
    if ((dwStyle & 0xFF) <= SS_RIGHT) 
    {
        // this is a text control: set up font and colors
        if (!(HFONT)m_font) 
        {            // first time init: create font
            LOGFONT lf;            
            GetFont()->GetObject(sizeof(lf), &lf);
            lf.lfUnderline = TRUE;            
            m_font.CreateFontIndirect(&lf);

            // load cursor
            CWinApp* pApp = AfxGetApp();
            if (pApp)
            {
                m_hCursLink=pApp->LoadCursor(IDC_HAND);
            }

        }        
        // use underline font and visited/unvisited colors
        pDC->SelectObject(&m_font);
        pDC->SetTextColor(m_bVisited ? m_colorVisited : m_colorUnvisited);
        pDC->SetBkMode(TRANSPARENT);
        // return hollow brush to preserve parent background color
        hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);    
    }    
    return hbr;
}

/////////////////// Handle mouse click: open URL/file.
//
void CStaticLink::OnClicked()
{
    ReleaseMouse();
    if (m_link.IsEmpty())         // if URL/filename not set..
        GetWindowText(m_link);    // ..get it from window text

    // Call ShellExecute to run the file.
    // For an URL, this means opening it in the browser.    
    //
    HINSTANCE h = ShellExecute(NULL, "open", m_link, NULL, NULL, SW_SHOWNORMAL);
    if ((UINT)h > 32) 
    {
        m_bVisited = TRUE;       // (not really--might not have found link)
        Invalidate();            // repaint to show visited color    
    } else 
    {
        MessageBeep(0);          // unable to execute file!
    }
}

BOOL CStaticLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

int CStaticLink::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CStaticLink::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (m_bCaptured == FALSE)
    {
        SetCapture();
        m_bCaptured = TRUE;
        // load cursor
        m_hCursOrg = ::SetCursor(m_hCursLink);
    }
    else
    {
        CRect rect;
        GetClientRect(&rect);
        if (!PtInRect(&rect, point))
        {
            ReleaseMouse();
        }
    }
	CStatic::OnMouseMove(nFlags, point);
}

BOOL CStaticLink::ReleaseMouse()
{
    ReleaseCapture();
    m_bCaptured = FALSE; 
    
    ::SetCursor(m_hCursOrg);

    return m_bCaptured;
}

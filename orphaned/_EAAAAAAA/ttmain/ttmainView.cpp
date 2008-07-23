// ttmainView.cpp : implementation of the CTtmainView class
//

#include "stdafx.h"

#include "ttmainDoc.h"
#include "ttmainView.h"
#include "Datatype.h"
#include "TTData.h"
#include "Tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"

#include "ZRSheet.h"
#include "RegistryManager.h"
#include "MainFrm.h"
#include "ttmain.h"
#include "TuerN.h"
#include "TorN.h"
#include "FTor.h"
#include "AuswDlg.h"
#include "winspool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define bKunde   350//120//90
#define bStueck  200//70//60
#define bBreite  150//50
#define bHoehe   150//70//50
#define bDIN     90//30
#define bObert   200//70//50
#define bStarr   200//70//50
#define bKipp    200//70//50
#define bSchieb  200//70//50
#define bNr	 80//30
#define bSchwei	 170//50
#define bFuell	 600//60
#define pTor bKunde+bStueck+bBreite+bHoehe+bDIN+bObert+\
	     bStarr+bKipp+bSchieb+bNr+bSchwei//+bFuell

#define hEintrag 900//250   //Höhe eines Eintrags für ein Tor

#define GridSize  12

#define MM_PER_INCH     25.4f

sGridCont Grid[GridSize] =
{  {bKunde, "Kunde"},
   {bStueck, "Stück"},
   {bBreite, "Breite"},
   {bHoehe, "Höhe"},
   {bDIN, "DIN"},
   {bObert, "Obert."},
   {bStarr, "starr"},
   {bKipp, "kipp"},
   {bSchieb, "schieb"},
   {bNr, "Nr."},
   {bSchwei, "Schwei."},
   {bFuell, "Füllung/Sonstiges"},
}; 


static CPoint mousePoint;

/////////////////////////////////////////////////////////////////////////////
// CTtmainView

IMPLEMENT_DYNCREATE(CTtmainView, CScrollView)

BEGIN_MESSAGE_MAP(CTtmainView, CScrollView)
	//{{AFX_MSG_MAP(CTtmainView)
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PU_TOR_AENDERN, OnPuTorAendern)
	ON_UPDATE_COMMAND_UI(ID_PU_TOR_AENDERN, OnUpdatePuTorAendern)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_ZUSCHNITTREGEL_FLUEGEL1, ID_ZUSCHNITTREGEL_FLUEGEL5, 
                     OnZuschnittregelFluegel)
    ON_UPDATE_COMMAND_UI_RANGE(ID_ZUSCHNITTREGEL_FLUEGEL1, ID_ZUSCHNITTREGEL_FLUEGEL5,
                     OnUpdateZuschnittregelFluegel)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTtmainView construction/destruction

CTtmainView::CTtmainView()
{
	// TODO: add construction code here
	m_bFirstShow = TRUE;
	if (addTorDoor() != IDOK)
		m_bShutDown = TRUE;
	else
		m_bShutDown = FALSE;

	m_stdFont = getFont(FONTHEIGTH, FONTWIDTH, 0, 400);
	m_boldFont = getFont(FONTHEIGTH, FONTWIDTH, 0, 700);
	m_thinFont = getFont(THINFONTHEIGTH, THINFONTWIDTH, 0, 400);
	m_bPen = CreatePen(PS_SOLID, 2, 0x00000000);

    m_iPageLen = DINA4LEN;
    m_iPageHeight = DINA4WID;
    m_iLeftX = linksX;
    m_iTopY = startY;
}

CTtmainView::~CTtmainView()
{
/*
	if (m_pTor != NULL)
	{
		delete m_pTor;
		m_pTor = NULL;
	}
*/
	int count = m_pTore.GetSize();
	for (int i=0; i<count; i++)
	{
		CTorDoor* pT = (CTorDoor*)m_pTore.GetAt(i);
		delete pT;
	}
	m_pTore.RemoveAll();

	DeleteObject(m_bPen);
	DeleteObject(m_stdFont);
	DeleteObject(m_boldFont);
	DeleteObject(m_thinFont);
}


BOOL CTtmainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTtmainView drawing

void CTtmainView::OnDraw(CDC* pDC)
{
	CTtmainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if ((m_bFirstShow == TRUE))
	{
		m_bFirstShow = FALSE;
		if (m_pTore.GetSize() > 0)
		{
			CTorDoor* pTTor = (CTorDoor*)m_pTore.GetAt(0);
			CDocument* pDoc = GetDocument();
			pDoc->SetTitle(pTTor->Kunde);
		}
	}
    if (pDC->IsPrinting())
    {
        // Drucken
        int iDpiHorz = pDC->GetDeviceCaps(LOGPIXELSX);
        int iDpiVert = pDC->GetDeviceCaps(LOGPIXELSY);
        m_iPageLen = 10*MM_PER_INCH*pDC->GetDeviceCaps(HORZRES)/iDpiHorz;
        m_iPageHeight = 10*MM_PER_INCH*pDC->GetDeviceCaps(VERTRES)/iDpiVert;
        m_iLeftX = linksX; //10*MM_PER_INCH*pDC->GetDeviceCaps(PHYSICALOFFSETX)/iDpiHorz;//linksX;
        m_iTopY = startY;//+10*MM_PER_INCH*pDC->GetDeviceCaps(PHYSICALOFFSETY)/iDpiVert;//startY;
    }
    else
    {
        // Anzeigen am Bildschirm
        m_iPageLen = DINA4LEN-1;
        m_iPageHeight = DINA4WID;
        m_iLeftX = linksX;
        m_iTopY = startY;
    }   

	HDC hdc = pDC->GetSafeHdc();

	PaintTT(hdc, 1);
}

void CTtmainView::PaintTT(HDC hdc, int Fact)
{
  ::SetMapMode(hdc, MM_LOMETRIC);

  ::SelectObject(hdc, m_stdFont);
  drawGrid(hdc, Fact);
  fillGrid(hdc, Fact);
}

void CTtmainView::drawHeader(HDC hdc, int Fact)
{
  char buf[50];
  SIZE size;

  m_orgPen = (HPEN)SelectObject(hdc, m_bPen);

  CTime t = CTime::GetCurrentTime();
  sprintf(buf, "Datum:   %d.%d.%d", t.GetDay(), t.GetMonth(),
		t.GetYear()); 
  
  ::GetTextExtentPoint32(hdc, buf, strlen(buf), &size);

  Rectangle(hdc, m_iLeftX, -(m_iTopY-rowH-size.cy-10), 
      m_iPageLen, -(m_iTopY-rowH));

  ::TextOut(hdc, (m_iPageLen-size.cx-10), -(m_iTopY-rowH-size.cy-5),
	  buf, strlen(buf));

  MoveToEx(hdc, (m_iPageLen-size.cx-60), -(m_iTopY-rowH-size.cy-10), NULL);
  LineTo(hdc, (m_iPageLen-size.cx-60), -(m_iTopY-rowH));

}


void CTtmainView::drawGrid(HDC hdc, int fact)
{
  drawHeader(hdc, fact);

  int actX = m_iLeftX;

  MoveToEx(hdc, m_iLeftX, -(m_iTopY-10), NULL);
  LineTo(hdc, (m_iLeftX+m_iPageLen), -(m_iTopY-10));
  SelectObject(hdc, m_boldFont);

  // zeichne 1. Linie
  MoveToEx(hdc, actX, -(m_iTopY-rowH), NULL);
  LineTo(hdc, actX, -(m_iTopY+hEintrag));

  for (int i=0; i<GridSize; i++)
  {
    MoveToEx(hdc, (actX+Grid[i].bSize), -(m_iTopY-rowH), NULL);
    if (i<GridSize-1)
      LineTo(hdc, (actX+Grid[i].bSize), -(m_iTopY+hEintrag));

    TextOut(hdc, (actX+spaceX), -(m_iTopY-rowH+6),
	    Grid[i].string, strlen(Grid[i].string));
    actX += Grid[i].bSize;
  }
  // zeichne hinterste Linie
  MoveToEx(hdc, m_iLeftX+m_iPageLen-1, -(m_iTopY-rowH), NULL);
  LineTo(hdc, m_iLeftX+m_iPageLen-1, -(m_iTopY+hEintrag));
/*  SelectObject(hdc, orgPen);
  MoveTo(hdc, m_iPageLen, 0);
  LineTo(hdc, DINA4LEN, -m_iPageHeight);*/
  SelectObject(hdc, m_stdFont);
}

void CTtmainView::stretchGrid(HDC hdc, int beginY, int endY, int fact)
{
  int actX = m_iLeftX;
  m_orgPen = (HPEN)SelectObject(hdc, m_bPen);

  // Trennstrich
  MoveToEx(hdc, m_iLeftX, -beginY, NULL);
  LineTo(hdc, (m_iLeftX+m_iPageLen), -beginY);

  // zeichne 1. Linie
  MoveToEx(hdc, actX, -beginY, NULL);
  LineTo(hdc, actX, -endY);

  for (int i=0; i<GridSize; i++)
  {
    MoveToEx(hdc, (actX+Grid[i].bSize), -beginY, NULL);
    if (i<GridSize-1)
      LineTo(hdc, (actX+Grid[i].bSize), -endY);
    actX += Grid[i].bSize;
  }

  // zeichne hinterste Linie
  MoveToEx(hdc, m_iLeftX+m_iPageLen-1, -beginY, NULL);
  LineTo(hdc, m_iLeftX+m_iPageLen-1, -endY);

  SelectObject(hdc, m_orgPen);
}


void CTtmainView::fillGrid(HDC hdc, int fact)
{
  int actX;
  //Tor zeichnen als 1. aufrufen!!!!
  int count = m_pTore.GetSize();
  actY = m_iTopY;
  for (int i=0; i<count; i++)
  {
    if (i > 0)
       stretchGrid(hdc, actY, actY+hEintrag, 1);

    actX = m_iLeftX+spaceX;
	CTorDoor* pTTor = (CTorDoor*)m_pTore.GetAt(i);

    int posX = pTor+m_iLeftX+squareW+3*spaceSquare;
    pTTor->drawTT(hdc, posX, actY+2*(rowH+spaceSquare+squareH),
                m_iPageLen-posX-squareW-3*spaceSquare,
		hEintrag-2*(rowH+2*(spaceSquare+squareH)));
    pTTor->drawSquares(hdc, posX, actY+2*(rowH+spaceSquare+squareH));

    //Kunde
    pTTor->printKundKomm(hdc, actX, (actY+10),
       (actX+bKunde), m_boldFont);
    //Stueck
    actX += bKunde;
    pTTor->printStueck(hdc, actX, (actY+10),
		   (actX+bStueck), m_boldFont);
    //Breite
    actX += bStueck;
    pTTor->printBreite(hdc, actX, (actY+10),
		   (actX+bBreite), m_boldFont, m_thinFont);
    //Hoehe
    m_orgPen = (HPEN)SelectObject(hdc, m_bPen);
    actX += bBreite;
    pTTor->printHoehe(hdc, actX, (actY+10), actX+bHoehe,
		   (actX+bHoehe), m_boldFont);
    SelectObject(hdc, m_orgPen);

    //DIN
    actX += bHoehe;
    pTTor->printDIN(hdc, actX,  (actY+10),
		    (actX+bDIN), m_boldFont);
    //Obert
    actX += bDIN;
    pTTor->printObert(hdc,  actX,  (actY+10),
		    (actX+bObert), m_boldFont);
    //Fenster
    actX += bObert;
    pTTor->printFenster(hdc,  (actY+10),  actX,
		    (actX+bStarr),  (actX+bStarr+bKipp),
		    (actX+bStarr+bKipp+bSchieb), m_boldFont);
    //Nr.
    actX += bStarr+bKipp+bSchieb;
    //Schwei.
    actX += bNr;
    //Fuellung
    actX += bSchwei;
    pTTor->printFuellung(hdc, (actX),  (actY+10),
		    (actX+bDIN), m_boldFont);
    actY += hEintrag;
  }
  // untere Begrenzung
  MoveToEx(hdc, m_iLeftX, -actY, NULL);
  LineTo(hdc, m_iLeftX+m_iPageLen-1, -actY);
}

void CTtmainView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = m_iPageLen;
	sizeTotal.cy = m_iPageHeight+100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
	SetScrollSizes(MM_LOMETRIC, sizeTotal);

	if (m_bShutDown == TRUE)
	{
		CWnd* pWnd = GetParent();
		pWnd->PostMessage(WM_CLOSE);
		return;
	}
	m_bShutDown = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CTtmainView printing

BOOL CTtmainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	if (pInfo->m_bPreview == FALSE)
		pInfo->m_bPreview = TRUE;
	return DoPreparePrinting(pInfo);
}

void CTtmainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
}

void CTtmainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTtmainView diagnostics

#ifdef _DEBUG
void CTtmainView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTtmainView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTtmainDoc* CTtmainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTtmainDoc)));
	return (CTtmainDoc*)m_pDocument;
}
#endif //_DEBUG

HFONT CTtmainView::getFont(int height, int width, int orient, int weight)
{
  LOGFONT MainFontRec;
  MainFontRec.lfHeight = height; //*15
  MainFontRec.lfWidth = width;   //*4
  MainFontRec.lfEscapement = orient;
  MainFontRec.lfOrientation = 0;
  MainFontRec.lfWeight = weight;//400;
  MainFontRec.lfItalic = 0;
  MainFontRec.lfUnderline = 0;
  MainFontRec.lfStrikeOut = 0;
  MainFontRec.lfCharSet = ANSI_CHARSET;
  MainFontRec.lfOutPrecision = OUT_DEFAULT_PRECIS;
  MainFontRec.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  MainFontRec.lfQuality = PROOF_QUALITY;
  MainFontRec.lfPitchAndFamily = DEFAULT_PITCH;
  strcpy(MainFontRec.lfFaceName, "Arial");   //Roman

  return CreateFontIndirect(&MainFontRec);
/*  return CreateFont(height, width, orient, 0, 400, 0, 0, 0,
	 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	 PROOF_QUALITY, FIXED_PITCH, NULL);
*/
}

void CTtmainView::getRealKoord(CPoint* pt)
{
  CPoint tmpPoint;
  tmpPoint.x = pt->x;
  tmpPoint.y = pt->y;

  CDC* pDC = GetDC();
  pDC->SetMapMode(MM_LOMETRIC);
  pDC->LPtoDP(pt, 1);
  ReleaseDC(pDC);

  float fFactor = (float)((float)tmpPoint.x/(float)pt->x);
  pt->x = (long)rnd(fFactor*(float)tmpPoint.x);
  pt->y = (long)rnd(fFactor*(float)tmpPoint.y);
}

int CTtmainView::addTorDoor()
{
	int iRet;
	CTorDoor* pT = new CTorDoor;
	CTorDoor* pTTor;
	CAuswDlg dlg(NULL, "Kunde", pT);
	if ((iRet=dlg.DoModal()) == IDOK)
	{
		switch(pT->Art)
		{
			case ATOR:
				pTTor = new CTorN(pT);
			break;
			case ATUER:
				pTTor = new CTuerN(pT);
			break;
			case FT3:
			case FT4:
			case FT5:
				pTTor = new CFTor(pT);
			break;
            case STUER:
                pTTor = new CSchiebeTuer(pT);
                break;
            case STOR:
                pTTor = new CSchiebeTor(pT);
                break;
			default:
				pTTor = new CTorDoor(pT);
			break;
		}

		pT->FlParam = NULL;
		pT->Profile = NULL;
		pT->GlasProfile = NULL;
		m_pTore.Add(pTTor);
	}
	delete pT;	

	if (m_pTore.GetSize() > 1)
	{
		POINT pt;
		pt.x = 0;//m_iPageLen/2;
		pt.y = -(m_iPageHeight+100)/2;
		ScrollToPosition(pt);
	}

	return iRet;
}

int  CTtmainView::getTorCount()
{
	return m_pTore.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// CTtmainView message handlers

void CTtmainView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int posX = pTor+m_iLeftX+squareW+3*spaceSquare;

	CPoint scrPoint = GetScrollPosition();
	scrPoint.y = -scrPoint.y;
	CDC* pDC = GetDC();
	pDC->SetMapMode(MM_LOMETRIC);
	pDC->DPtoLP(&point, 1);
	ReleaseDC(pDC);
	point.y = -point.y;
	point.x += scrPoint.x;
	point.y += scrPoint.y;

	int mouse_x = point.x;
	int mouse_y = point.y;

	int actY = m_iTopY+2*(rowH+spaceSquare+squareH);
	if (posX < mouse_x)
	{
		int count = m_pTore.GetSize();
		for (int i=0; i<count; i++)
		{
			CTorDoor* pTT = (CTorDoor*)m_pTore.GetAt(i);
			if (actY < mouse_y  &&
				pTT->inBounds(posX, actY, mouse_x, mouse_y) == TRUE)
			{
				CAuswDlg dlg(this, "Kunde", pTT, TRUE);
				if (dlg.DoModal() == IDOK)
				{
					InvalidateRect(NULL, TRUE);
				}
				goto Ende;
			}
			actY += hEintrag;
		}
	}
	
Ende:
  CScrollView::OnLButtonDown(nFlags, point);
}

int CTtmainView::editTorDoor(int iTorNr)
{
    int iRet = 0;
    CTorDoor* pTT = (CTorDoor*)m_pTore.GetAt(iTorNr);
    if (pTT)
    {
		CAuswDlg dlg(this, "Kunde", pTT, TRUE);
		if ((iRet = dlg.DoModal()) == IDOK)
		{
			InvalidateRect(NULL, TRUE);
		}
    }

    return iRet;
}

void CTtmainView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
    InvalidateRect(NULL, FALSE);
}

void CTtmainView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
    InvalidateRect(NULL, FALSE);
}

void CTtmainView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    CMenu menu;

    menu.LoadMenu(IDR_TORVIEWMENU);

    CMenu *pMenu = menu.GetSubMenu(0);


    if (pMenu)
    {
        mousePoint = point;
        ClientToScreen(&point);

        pMenu->TrackPopupMenu(/*TPM_LEFTALIGN|TPM_RIGHTBUTTON*/0, 
            point.x, point.y, AfxGetMainWnd(), NULL);
    }
	
	CScrollView::OnRButtonDown(nFlags, point);
}

void CTtmainView::OnPuTorAendern() 
{
#if 1
    CTorDoor* pTT = getSelectedTor();
    if (pTT != NULL)
    {
		CAuswDlg dlg(this, "Kunde", pTT, TRUE);
		if (dlg.DoModal() == IDOK)
		{
			InvalidateRect(NULL, TRUE);
		}
    }
#else
    CPoint point = mousePoint;
	CPoint scrPoint = GetScrollPosition();

    // ummappen in Device Units
	CDC* pDC = GetDC();
	pDC->SetMapMode(MM_LOMETRIC);
	pDC->DPtoLP(&point, 1);
	ReleaseDC(pDC);

	point.y = -point.y;
	scrPoint.y = -scrPoint.y;
	point.y += scrPoint.y;

	int actY = m_iTopY+hEintrag;

	int count = m_pTore.GetSize();
	for (int i=0; i<count; i++)
	{
		CTorDoor* pTT = (CTorDoor*)m_pTore.GetAt(i);
		if (point.y < actY)
		{
			CAuswDlg dlg(this, "Kunde", pTT, TRUE);
			if (dlg.DoModal() == IDOK)
			{
				InvalidateRect(NULL, TRUE);
			}
            break;
		}
		actY += hEintrag;
	}
#endif
}

void CTtmainView::OnUpdatePuTorAendern(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();
}

void CTtmainView::computeAndDraw()
{
	for (int i=0; i<m_pTore.GetSize(); i++)
	{
		CTorDoor* pTT = (CTorDoor*)m_pTore.GetAt(i);
        pTT->updateValues();
    }

    Invalidate();
}



void CTtmainView::OnZuschnittregelFluegel(UINT nId) 
{
    // richtigen Flügel ermitteln
    CFlParam* pFluegel;
    CTorDoor* pTT = getSelectedTor();
    if (pTT != NULL)
    {
        if (pTT->FlParam->GetSize() > (int)(nId-ID_ZUSCHNITTREGEL_FLUEGEL1))
        {
            pFluegel = (CFlParam*)pTT->FlParam->GetAt(nId-ID_ZUSCHNITTREGEL_FLUEGEL1);

            CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
            if (pFrm != NULL && pFluegel != NULL)
            {
                CZRSheet* pSheet =  pFrm->CreateZuschnittDlg();
                if (pSheet != NULL)
                {
                    // Message senden
                    while (TRUE)
                    {
                        if (pSheet->GetSafeHwnd() != NULL)
                        {
                            pSheet->SendMessage(WM_ZR_SHEET_SHOW_RULES, (WPARAM)pTT, 
                                (LPARAM)pFluegel);
                            break;
                        }
                        Sleep(100);
                    }
                }
            }
        }
    }
}

void CTtmainView::OnUpdateZuschnittregelFluegel(CCmdUI* pCmdUI) 
{
    CTorDoor* pTT = getSelectedTor();
    if (pTT != NULL)
    {
        if (pTT->FlParam->GetSize() > (int)(pCmdUI->m_nID-ID_ZUSCHNITTREGEL_FLUEGEL1))
        {
            pCmdUI->Enable(TRUE);	
            return;
        }
    }
    pCmdUI->Enable(FALSE);
}

CTorDoor* CTtmainView::getSelectedTor()
{
    CPoint point = mousePoint;
	CPoint scrPoint = GetScrollPosition();

    // ummappen in Device Units
	CDC* pDC = GetDC();
	pDC->SetMapMode(MM_LOMETRIC);
	pDC->DPtoLP(&point, 1);
	ReleaseDC(pDC);

	point.y = -point.y;
	scrPoint.y = -scrPoint.y;
	point.y += scrPoint.y;

	int actY = m_iTopY+hEintrag;

	int count = m_pTore.GetSize();
	for (int i=0; i<count; i++)
	{
		CTorDoor* pTT = (CTorDoor*)m_pTore.GetAt(i);
		if (point.y < actY)
		{
            return pTT;
		}
		actY += hEintrag;
	}
    return NULL;
}

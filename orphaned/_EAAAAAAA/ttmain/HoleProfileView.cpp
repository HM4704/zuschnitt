// HoleProfileView.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ZRSheet.h"
#include "RegistryManager.h"
#include "MainFrm.h"
#include "ttmain.h"
#include "HoleProfileDoc.h"
#include "HoleProfileView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static sGridCont Grid[] =
{  
    {300, "Typ"},
    {300, "Stück"},
    {300, "Länge"},
    {700, "Schnittliste"},
}; 

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileView

IMPLEMENT_DYNCREATE(CHoleProfileView, CScrollView)

CHoleProfileView::CHoleProfileView()
{
    m_iPageLen = DINA4LEN;
    m_iPageHeight = DINA4WID;
    m_iLeftX = linksX;
    m_iTopY = startY;

	getFont(&m_stdFont, FONTHEIGTH, FONTWIDTH, 0, 400);
	getFont(&m_boldFont, FONTHEIGTH, FONTWIDTH, 0, 700);
	getFont(&m_thinFont, THINFONTHEIGTH, THINFONTWIDTH, 0, 400);
    getFont(&m_smallFont, 45, 17, 0, 400);
	m_bPen.CreatePen(PS_SOLID, 2,  RGB(0,0,0));
}

CHoleProfileView::~CHoleProfileView()
{
	DeleteObject(m_bPen);
	DeleteObject(m_stdFont);
	DeleteObject(m_boldFont);
	DeleteObject(m_thinFont);
	DeleteObject(m_smallFont);
}


BEGIN_MESSAGE_MAP(CHoleProfileView, CScrollView)
	//{{AFX_MSG_MAP(CHoleProfileView)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_HP_AENDERN, OnHoleProfileAendern)
	ON_UPDATE_COMMAND_UI(ID_HP_AENDERN, OnUpdateHoleProfileAendern)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileView drawing

void CHoleProfileView::getFont(CFont* pFont, int height, int width, int orient, int weight)
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

  pFont->CreateFontIndirect(&MainFontRec);
}

void CHoleProfileView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = m_iPageLen;
	sizeTotal.cy = m_iPageHeight+100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
	SetScrollSizes(MM_LOMETRIC, sizeTotal);
}

void CHoleProfileView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

    if (pDC->IsPrinting())
    {
        // Drucken
        int iDpiHorz = pDC->GetDeviceCaps(LOGPIXELSX);
        int iDpiVert = pDC->GetDeviceCaps(LOGPIXELSY);
        m_iPageLen = (int)(10*MM_PER_INCH*pDC->GetDeviceCaps(HORZRES)/iDpiHorz);
        m_iPageHeight = (int)(10*MM_PER_INCH*pDC->GetDeviceCaps(VERTRES)/iDpiVert);
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

    DrawAll(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileView diagnostics

#ifdef _DEBUG
void CHoleProfileView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CHoleProfileView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileView message handlers
#if 0
void CHoleProfileView::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	
}

void CHoleProfileView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	
}
#endif

void CHoleProfileView::DrawAll(CDC* pDC)
{
    ::SetMapMode(pDC->GetSafeHdc(), MM_LOMETRIC);
//    pDC->SetMapMode(MM_LOMETRIC);

    ::SelectObject(pDC->GetSafeHdc(), m_stdFont);

    DrawHeader(pDC);
    DrawGrid(pDC);
    DrawData(pDC);
}


void CHoleProfileView::DrawHeader(CDC* pDC)
{
  char buf[150];
  CSize size;

//  m_orgPen = (HPEN)pDC->SelectObject(m_bPen);

  CTime t = CTime::GetCurrentTime();
  sprintf(buf, "  %d.%d.%d   ", t.GetDay(), t.GetMonth(),
		t.GetYear()); 
  
  size = pDC->GetTextExtent(buf, strlen(buf));

  pDC->Rectangle(m_iLeftX, -(m_iTopY-rowH-size.cy-10), 
      m_iPageLen, -(m_iTopY-rowH));

  pDC->TextOut((m_iPageLen-size.cx-10), -(m_iTopY-rowH-size.cy-5),
	  buf, strlen(buf));

  pDC->MoveTo((m_iPageLen-size.cx-60), -(m_iTopY-rowH-size.cy-10));
  pDC->LineTo((m_iPageLen-size.cx-60), -(m_iTopY-rowH));

  // Bulthaup Zuschnitt
  ::SelectObject(pDC->GetSafeHdc(), m_boldFont);
  strcpy(buf, "Bulthaup Zuschnitt");
  pDC->TextOut(m_iLeftX+2*spaceX, -(m_iTopY-rowH-size.cy-5),
	  buf, strlen(buf));
  ::SelectObject(pDC->GetSafeHdc(), m_stdFont);
}


void CHoleProfileView::DrawGrid(CDC* pDC)
{
  int actX = m_iLeftX;
  int actY = -m_iTopY+rowH;
  int GridSize = sizeof(Grid)/sizeof(sGridCont);

  pDC->MoveTo(m_iLeftX, -(m_iTopY-10));

  // Seite einfassen
  pDC->Rectangle(m_iLeftX, -(m_iTopY + 2*rowH - 10), m_iLeftX+m_iPageLen-1, -(m_iTopY+m_iPageHeight));
//  pDC->LineTo((m_iLeftX+m_iPageLen), -(m_iTopY-10));
  ::SelectObject(pDC->GetSafeHdc(), m_boldFont);

  // TP
  actY -= 2*rowH;
  pDC->Rectangle(m_iLeftX, -(m_iTopY - rowH), m_iLeftX+m_iPageLen-1, actY);
  actX = Grid[0].bSize + Grid[1].bSize + Grid[2].bSize;
  pDC->MoveTo(actX, -(m_iTopY - rowH));
  pDC->LineTo(actX, actY);
  ::TextOut(pDC->GetSafeHdc(), (actX+2*spaceX), /*-(m_iTopY-rowH+6)*/-(m_iTopY - rowH + 6),
	"TP", strlen("TP"));
  ::TextOut(pDC->GetSafeHdc(), (actX+2*spaceX), /*-(m_iTopY-rowH+6)*/-(m_iTopY + 6),
	"TP", strlen("TP"));
  // waggrecht teilen
  pDC->MoveTo(actX, -m_iTopY);
  pDC->LineTo(m_iLeftX+m_iPageLen-1, -m_iTopY);
  // senkrecht teilen
  pDC->MoveTo(actX + 200, -(m_iTopY - rowH));
  pDC->LineTo(actX + 200, actY);



  // zeichne 1. Linie
  actX = m_iLeftX;
  pDC->MoveTo(actX, actY);
  pDC->LineTo(actX, -(m_iTopY+m_iPageHeight));


  for (int i=0; i<GridSize; i++)
  {
    pDC->MoveTo((actX+Grid[i].bSize), actY);
    if (i<GridSize-1)
      pDC->LineTo((actX+Grid[i].bSize), -(m_iTopY+m_iPageHeight));

    ::TextOut(pDC->GetSafeHdc(), (actX+2*spaceX), /*-(m_iTopY-rowH+6)*/actY-6,
	    Grid[i].string, strlen(Grid[i].string));
    actX += Grid[i].bSize;
  }
  // zeichne hinterste Linie
  pDC->MoveTo(m_iLeftX+m_iPageLen-1, actY);
  pDC->LineTo(m_iLeftX+m_iPageLen-1, -(m_iTopY+m_iPageHeight));
/*  SelectObject(hdc, orgPen);
  MoveTo(hdc, m_iPageLen, 0);
  LineTo(hdc, DINA4LEN, -m_iPageHeight);*/
  ::SelectObject(pDC->GetSafeHdc(), m_stdFont);
}

void CHoleProfileView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{	
	CScrollView::OnBeginPrinting(pDC, pInfo);
}

BOOL CHoleProfileView::OnPreparePrinting(CPrintInfo* pInfo) 
{
//	if (pInfo->m_bPreview == FALSE)
//		pInfo->m_bPreview = TRUE;
	return DoPreparePrinting(pInfo);
}

void CHoleProfileView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnEndPrinting(pDC, pInfo);
}


void CHoleProfileView::DrawData(CDC* pDC)
{
    char szTemp[2000];
    int iActY = m_iTopY + 10 + 2*rowH;
    int iActX;
    int iSpaceCutList;

    CHoleProfileDoc* pDoc = (CHoleProfileDoc*) GetDocument();

    if (pDoc == NULL)
        return;

    for (int i = 0; i < pDoc->m_Profiles.GetSize(); i++)
    {
        CHoleProfile* pPrf = (CHoleProfile*)pDoc->m_Profiles.GetAt(i);
        if (pPrf)
        {
            iActX = m_iLeftX + 2*spaceX;
            // Typ
            strcpy(szTemp, "50 HP xx");
            ::TextOut(pDC->GetSafeHdc(), iActX, -iActY, szTemp, strlen(szTemp));
            iActX += Grid[0].bSize;
            // Stueckzahl
            sprintf(szTemp, "%ld", pPrf->m_dwCount);
            ::TextOut(pDC->GetSafeHdc(), iActX, -iActY, szTemp, strlen(szTemp));
            iActX += Grid[1].bSize;
            // Laenge
            sprintf(szTemp, "%ld", pPrf->m_dwLength);
            ::TextOut(pDC->GetSafeHdc(), iActX, -iActY, szTemp, strlen(szTemp));
            iActX += Grid[2].bSize;

            // Schnittliste
            CPtrArray* p = (CPtrArray*)pDoc->m_CutList.GetAt(i);
            if (p)
            {
                szTemp[0] = 0;
                for (int c = 0; c < p->GetSize(); c++)
                {
                    char szCut[100];
                    CHoleProfileCut* pCut = (CHoleProfileCut*)p->GetAt(c);
                    if (pCut)
                    {
                        if (pCut->m_eAction == caCut)
                        {
                            sprintf(szCut, "%ld mm, ", pCut->m_dwLength);
                            strcat(szTemp, szCut);
                        }
                        if (pCut->m_eAction == caNew)
                        {
                            strcat(szTemp, "neues Profil, ");
                        }
                    }
                }
                // letztes ", " wieder weg
                if (strlen(szTemp) > 2)
                {
                    szTemp[strlen(szTemp) - 2] = 0;
                }
                iSpaceCutList = m_iPageLen - iActX - 100;

                // Zeilenweise ausgeben
                TEXTMETRIC mtrc;
                pDC->GetTextMetrics(&mtrc);
                SIZE size;
                size.cx = mtrc.tmAveCharWidth; size.cy = mtrc.tmHeight;
                //?? pDC->LPtoDP(&size);
                int iAvCharCnt = iSpaceCutList/size.cx - 7;
                int iTextLen = strlen(szTemp);
                char* pNextLine, *pOut;
                pOut = szTemp;
                pNextLine = szTemp;
                while (TRUE)
                {
                    CSize TextSize = pDC->GetTextExtent(pOut, strlen(pOut));
                    if (TextSize.cx > iSpaceCutList)
                    {
                        char* pC;
                        char c = pOut[iAvCharCnt];
                        pOut[iAvCharCnt] = 0;
                        // nach letztem "," suchen
                        pC = strrchr(pOut, ',');
                        if (pC)
                        {
                            pC[1] = 0;  // pC[1] muss Blank sein!!!
                            pNextLine = &(pC[2]);
                            iTextLen--;    // Zeichen wird nicht gedruckt
                        }
                        else
                        {
                            //?? was jetzt
                        }
                        if (&pC[1] != &pOut[iAvCharCnt])
                            pOut[iAvCharCnt] = c;
                    }
                    ::TextOut(pDC->GetSafeHdc(), iActX, -iActY, pOut, strlen(pOut));
                    iActY += rowH;
                    iTextLen -= strlen(pOut);
                    if (iTextLen < 1)
                        break;
                    pOut = pNextLine;
                }
            }
            // --> Schnitliste Ende
            iActY += rowH;
        }
    }
}


void CHoleProfileView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CHoleProfileView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    CMenu menu;

    menu.LoadMenu(IDR_HP_POPUP);

    CMenu *pMenu = menu.GetSubMenu(0);


    if (pMenu)
    {
        ClientToScreen(&point);

        pMenu->TrackPopupMenu(/*TPM_LEFTALIGN|TPM_RIGHTBUTTON*/0, 
            point.x, point.y, AfxGetMainWnd(), NULL);
    }
		
	CScrollView::OnRButtonDown(nFlags, point);
}

void CHoleProfileView::OnHoleProfileAendern() 
{
	// TODO: Add your command handler code here
    CHoleProfileDoc* pDoc = (CHoleProfileDoc*)GetDocument();
    if (pDoc)
    {
	    pDoc->EditProfiles();
		InvalidateRect(NULL, TRUE);
    }
}

void CHoleProfileView::OnUpdateHoleProfileAendern(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// KreisBerechnungDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttmain.h"
#include "EvaluateCircle.h"
#include "EvaluateDoubleCircle.h"
#include "KreisBerechnungDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FORMAT_DOUBLE_STRING    "%1.3f"

#define SETREADONLY(id, bSetReadOnly)   { pEdit = (CEdit*) GetDlgItem(id); \
                                          pEdit->SetReadOnly(bSetReadOnly); }

#define SENDMSGTO(id, msg, wParam, lParam)   \
                            { pWnd = (CWnd*) GetDlgItem(id); \
                              pWnd->SendMessage(msg, wParam, lParam); }

/////////////////////////////////////////////////////////////////////////////
// CKreisBerechnung dialog


volatile BOOL g_bExplorerFound;
CRect  g_DlgRect;

CKreisBerechnungDlg::CKreisBerechnungDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKreisBerechnungDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKreisBerechnungDlg)
	m_strRadius = _T("0.0");
	m_strBogenlaenge = _T("0.0");
	m_strBWinkelAussen = _T("0.0");
	m_strBWinkelMitte = _T("0.0");
	m_strProfilDicke = _T("0.0");
	m_strSehne = _T("0.0");
	m_strSehnenHoehe = _T("0.0");
	m_strSWinkelAussen = _T("0.0");
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CKreisBerechnungDlg::IDD;
}


void CKreisBerechnungDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKreisBerechnungDlg)
	DDX_Control(pDX, IDC_RADIORADIUS, m_RadioBerechnen);
	DDX_Text(pDX, IDC_RADIUS, m_strRadius);
	DDX_Text(pDX, IDC_BOGENLAENGE, m_strBogenlaenge);
	DDX_Text(pDX, IDC_BWINKEL_AUSSEN, m_strBWinkelAussen);
	DDX_Text(pDX, IDC_BWINKEL_MITTE, m_strBWinkelMitte);
	DDX_Text(pDX, IDC_PROFILDICKE, m_strProfilDicke);
	DDX_Text(pDX, IDC_SEHNE, m_strSehne);
	DDX_Text(pDX, IDC_SEHNENHOEHE, m_strSehnenHoehe);
	DDX_Text(pDX, IDC_SWINKEL_AUSSEN, m_strSWinkelAussen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKreisBerechnungDlg, CDialog)
	//{{AFX_MSG_MAP(CKreisBerechnungDlg)
	ON_BN_CLICKED(IDC_RADIORADIUS, OnRadioRadius)
	ON_BN_CLICKED(IDC_RADIOSEHNE, OnRadioSehne)
	ON_BN_CLICKED(IDC_RADIOSEHNENHOEHE, OnRadioSehnenhoehe)
	ON_BN_CLICKED(IDC_BERECHNEWINKEL, OnBerechnewinkel)
	ON_BN_CLICKED(IDC_CIRCLE_HELP, OnHelp)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKreisBerechnung message handlers

void CKreisBerechnungDlg::OnBerechnen() 
{
    UpdateData(TRUE);
    AllMembersToValues();

    if (m_iBerechneterWert == BERECHNUNG_RADIUS)
        BerechneRadius();
    else
        if (m_iBerechneterWert == BERECHNUNG_SEHNE)
            BerechneSehne();
        else
            BerechneSehnenHoehe();

    BerechneBogenlaenge();
//    BerechneTangentenWinkel();
    BerechneSchnittwinkel();

    AllMembersToStrings();
    UpdateData(FALSE);
}

BOOL CKreisBerechnungDlg::BerechneRadius()
{
    CEvaluateCircle circMath;
//    UpdateData(TRUE);
    
    if (!m_dSehne || !m_dSehnenHoehe)
        return  FALSE;

    m_dRadius = circMath.EvalRadius(m_dSehne, m_dSehnenHoehe);
//    UpdateData(FALSE);

    return TRUE;
}

BOOL CKreisBerechnungDlg::BerechneBogenlaenge()
{
//    UpdateData(TRUE);

    if (!m_dRadius || !m_dSehne)
        return  FALSE;

    CEvaluateDoubleCircle circMath;
    CEvaluateCircle circMath1;
    m_dBogenlaenge = circMath1.EvalArcLength(m_dRadius, m_dSehne);
    m_dBogenlaenge = circMath.EvalArcLength(m_dRadius, m_dSehne, m_dProfilDicke);
//    UpdateData(FALSE);

    return TRUE;
}

BOOL CKreisBerechnungDlg::BerechneSehne()
{
//    UpdateData(TRUE);

    if (!m_dRadius || !m_dSehnenHoehe)
        return  FALSE;

    CEvaluateCircle circMath;

    m_dSehne = circMath.EvalChordLength(m_dRadius, m_dSehnenHoehe);
//    UpdateData(FALSE);

    return TRUE;
}

BOOL CKreisBerechnungDlg::BerechneSehnenHoehe()
{
//    UpdateData(TRUE);
    
    if (!m_dRadius || !m_dSehne)
        return  FALSE;

    CEvaluateCircle circMath;
    m_dSehnenHoehe = circMath.EvalChordHeight(m_dRadius, m_dSehne);
//    UpdateData(FALSE);

    return TRUE;
}

BOOL CKreisBerechnungDlg::BerechneTangentenWinkel()
{
//    UpdateData(TRUE);
    if (!m_dRadius || !m_dSehne)
        return  FALSE;

    CEvaluateCircle circMath;
    m_dBWinkelMitte = circMath.EvalTangentAngle(m_dRadius, m_dSehne);

    // Winkel zwischen Rahmen und Bogen
    m_dBWinkelMitte = 90+m_dBWinkelMitte;

//  UpdateData(FALSE);
    return TRUE;
}

BOOL CKreisBerechnungDlg::BerechneSchnittwinkel()
{
/* neue Formel: tan a = b / (r+-sqrt(r*r-2b*r))
           wobei:   b = Dicke des Bogens entspricht Dicke des Profils
                    r = Innenradius des Bogens
*/

//    CEvaluateCircle circMath;
    CEvaluateDoubleCircle math;

/*  Bogenmitte */
//    UpdateData(TRUE);

    if (!m_dRadius)
        return  FALSE;

#if 0
    double dAngle = m_dProfilDicke / (m_dRadius-/*+??*/
                    sqrt(m_dRadius*m_dRadius-2*m_dRadius*m_dProfilDicke));

    m_dBWinkelMitte = circMath.RadToDeg(atan(dAngle));
#endif
    m_dBWinkelMitte = math.EvaluateAngleMid(m_dRadius, m_dProfilDicke);

//    UpdateData(FALSE);

/*  Bogenseiten */
/* Ausgangspunkt: linke Enke im oberen Kreisbogen */

//    UpdateData(TRUE);
    math.EvalAngleWithFrame(m_dRadius, m_dSehne, m_dSehnenHoehe, m_dProfilDicke,
        &m_dSWinkelAussen, &m_dBWinkelAussen);
//    UpdateData(FALSE);
    return TRUE;
}

BOOL CKreisBerechnungDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CreateFont();

    m_RadioBerechnen.SetCheck(1);

    SetzeBerechnung(BERECHNUNG_RADIUS);

    // Plazieren:
    CRect rect, dlgRect;
    GetParent()->GetClientRect(&rect);
    ClientToScreen(&rect);
    GetWindowRect(&dlgRect);
    rect.left += 8;
    rect.top += 8;
    rect.right = dlgRect.Width()+rect.left;
    rect.bottom = dlgRect.Height()+rect.top;
    MoveWindow(&rect);
    g_DlgRect = rect;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKreisBerechnungDlg::OnRadioRadius() 
{
    SetzeBerechnung(BERECHNUNG_RADIUS);
}

void CKreisBerechnungDlg::OnRadioSehne() 
{
    SetzeBerechnung(BERECHNUNG_SEHNE);
}

void CKreisBerechnungDlg::OnRadioSehnenhoehe() 
{
    SetzeBerechnung(BERECHNUNG_SEHNENHOEHE);
}


BOOL CKreisBerechnungDlg::EnableEdits(int iState)
{
    CEdit *pEdit;

    if (iState == BERECHNUNG_RADIUS)
    {
        // Radius berechnen
        SETREADONLY(IDC_RADIUS, TRUE)
        // ModifyStyle(Remove, Add, Flags)
        pEdit->ModifyStyle(WS_TABSTOP, 0);
        SETREADONLY(IDC_SEHNE, FALSE)
        pEdit->ModifyStyle(0, WS_TABSTOP);
//        pEdit->SetFocus();
        SETREADONLY(IDC_SEHNENHOEHE, FALSE)
        pEdit->ModifyStyle(0, WS_TABSTOP);
    }
    else
        if (iState == BERECHNUNG_SEHNE)
        {
            // Sehne berechnen
            SETREADONLY(IDC_RADIUS, FALSE)
            pEdit->ModifyStyle(0, WS_TABSTOP);
//            pEdit->SetFocus();
            SETREADONLY(IDC_SEHNE, TRUE)
            pEdit->ModifyStyle(WS_TABSTOP, 0);
            SETREADONLY(IDC_SEHNENHOEHE, FALSE)
            pEdit->ModifyStyle(0, WS_TABSTOP);
        }
        else
        {
            // Sehnenhoehe berechnen
            SETREADONLY(IDC_RADIUS, FALSE)
            pEdit->ModifyStyle(0, WS_TABSTOP);
//            pEdit->SetFocus();
            SETREADONLY(IDC_SEHNE, FALSE)
            pEdit->ModifyStyle(0, WS_TABSTOP);
            SETREADONLY(IDC_SEHNENHOEHE, TRUE)
            pEdit->ModifyStyle(WS_TABSTOP, 0);
        }

    return TRUE;
}

BOOL CKreisBerechnungDlg::SetzeBerechnung(int iWert)
{
    m_iBerechneterWert = iWert;

    EnableEdits(m_iBerechneterWert);

    return TRUE;
}

void CKreisBerechnungDlg::OnOK() 
{
    OnBerechnen();	
//	CDialog::OnOK();
}

BOOL CALLBACK CloseWindow( HWND hwnd, LPARAM lParam )   
{
    char cBuf[1000];
    
    GetWindowText(hwnd, cBuf, 1000);
    char* szTitle = (char*) lParam;
    if(strstr(cBuf, szTitle) != NULL)
    {         
        ::PostMessage(hwnd, WM_CLOSE, 0, 0);
    }      
    
    return TRUE ;
}

void CKreisBerechnungDlg::OnCancel() 
{
    char* szWndTitle = (char*)szKreisDlgHilfe;
    EnumWindows(::CloseWindow, (LPARAM) szWndTitle);
//	CDialog::OnCancel();
    //?? zu tun: MainWindow unterrichten, daß Dialog beendet
//	((CMainDlg*)m_pParent)->BoxDone();
    DestroyWindow();
}

HFONT CKreisBerechnungDlg::CreateFont()
{
    CWnd* pEdit;
    CFont* pFont;
    LOGFONT logFont;
    
    pEdit = GetDlgItem(IDC_RADIUS);

    pFont = pEdit->GetFont();
    pFont->GetLogFont(&logFont);

    logFont.lfHeight = -17;
    logFont.lfWeight = 400;

    m_fontEdit.CreateFontIndirect(&logFont);

//    SendFontToEdits(m_fontEdit);

    return m_fontEdit;
}

BOOL CKreisBerechnungDlg::SendFontToEdits(CFont &font)
{
    BOOL fRedraw = TRUE;
    HFONT hfont = font;
    WPARAM wParam = (WPARAM) hfont;            // handle of font 
    LPARAM lParam = MAKELPARAM(fRedraw, 0);    

    CWnd* pWnd;
    SENDMSGTO(IDC_RADIUS, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_SEHNE, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_SEHNENHOEHE, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_BWINKEL_MITTE, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_BOGENLAENGE, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_PROFILDICKE, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_BWINKEL_AUSSEN, WM_SETFONT, wParam, lParam);
    SENDMSGTO(IDC_SWINKEL_AUSSEN, WM_SETFONT, wParam, lParam);

    return TRUE;
}


void CKreisBerechnungDlg::OnBerechnewinkel() 
{
    BerechneSchnittwinkel();
}


void CKreisBerechnungDlg::OnHelp() 
{
    CTtmainApp* pApp = (CTtmainApp*) AfxGetApp();
    if (pApp)
    {
        CRect rect;
        int width = ::GetSystemMetrics(SM_CXSCREEN); 
        int height = ::GetSystemMetrics(SM_CYSCREEN);
        rect.left = width/2;
        rect.right = width-10;
        rect.top = 10;
        rect.bottom = (int)((float)height*8/9);

        pApp->ShowHelp(szKreisDlgHilfe, PLACE_POS, &rect);
        MoveWindow(g_DlgRect);
   }
}

BOOL CKreisBerechnungDlg::AllMembersToStrings()
{
    char cBuf[100];

    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dRadius);
    m_strRadius = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dSehne);
    m_strSehne = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dSehnenHoehe);
    m_strSehnenHoehe = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dBogenlaenge);
    m_strBogenlaenge = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dProfilDicke);
    m_strProfilDicke = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dBWinkelAussen);
    m_strBWinkelAussen = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dBWinkelMitte);
    m_strBWinkelMitte = cBuf;
    sprintf(cBuf, FORMAT_DOUBLE_STRING, m_dSWinkelAussen);
    m_strSWinkelAussen = cBuf;

//    UpdateData(FALSE);
    return TRUE;
}

BOOL CKreisBerechnungDlg::AllMembersToValues()
{
    m_dRadius = atof(m_strRadius);
    m_dSehne = atof(m_strSehne);
    m_dSehnenHoehe = atof(m_strSehnenHoehe);
    m_dBogenlaenge = atof(m_strBogenlaenge);
    m_dProfilDicke = atof(m_strProfilDicke);
    m_dBWinkelAussen = atof(m_strBWinkelAussen);
    m_dBWinkelMitte = atof(m_strBWinkelMitte);
    m_dSWinkelAussen = atof(m_strSWinkelAussen);
    return TRUE;
}

BOOL CKreisBerechnungDlg::Create(void) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(m_nID, m_pParent);
}

void CKreisBerechnungDlg::PostNcDestroy() 
{
	delete this;
}

void CKreisBerechnungDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
    if (nType == SIZE_MINIMIZED)
    {
        if (m_pParent)
        {
            CRect rect;
            m_pParent->GetClientRect(&rect);

//            MoveWindow(rect.left, rect.bottom-cy, cx, cy);
        }
    }
}

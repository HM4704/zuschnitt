// BHTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TTData.h"
#include "Tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "BHTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBHTestDlg dialog


CBHTestDlg::CBHTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBHTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBHTestDlg)
	m_strNewOffs2Hole = _T("0");
	m_strOffs2Hole = _T("0");
	m_strProfilLength = _T("0");
	m_strResult = _T("0");
	//}}AFX_DATA_INIT
}


void CBHTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBHTestDlg)
	DDX_Text(pDX, IDC_BH_TEST_NEW_OFFSET_TO_HOLE, m_strNewOffs2Hole);
	DDX_Text(pDX, IDC_BH_TEST_OFFSET_TO_HOLE, m_strOffs2Hole);
	DDX_Text(pDX, IDC_BH_TEST_PROFIL_LENGTH, m_strProfilLength);
	DDX_Text(pDX, IDC_BH_TEST_RESULT, m_strResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBHTestDlg, CDialog)
	//{{AFX_MSG_MAP(CBHTestDlg)
	ON_BN_CLICKED(ID_BH_TEST_COMPUTE, OnBhTestCompute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBHTestDlg message handlers

void CBHTestDlg::AllMembersToValues(void)
{
    UpdateData(TRUE);
    m_iNewOffs2Hole = atoi(m_strNewOffs2Hole);
    m_iOffs2Hole    = atoi(m_strOffs2Hole);
    m_iProfilLength = atoi(m_strProfilLength);
    m_iResult       = atoi(m_strResult);
}

void CBHTestDlg::AllMembersToStrings(void)
{
    char cBuf[100];

    sprintf(cBuf, "%ld", m_iNewOffs2Hole);
    m_strNewOffs2Hole   = cBuf;
    sprintf(cBuf, "%ld", m_iOffs2Hole);
    m_strOffs2Hole      = cBuf;
    sprintf(cBuf, "%ld", m_iProfilLength);
    m_strProfilLength   = cBuf;
    sprintf(cBuf, "%ld", m_iResult);
    m_strResult         = cBuf;
    UpdateData(FALSE);
}


// Function name	: ComputeBHProfil
// Description	    : berechnet Zuschnitt fuer gelochte Profile (links und rechts neben Profile mind. 20 mm
//                    Abstand
// Return type		: BOOL
// Argument         : int iOffs2Hole        Offset zum naechsten Loch
// Argument         : int iLength           gewuenschte Laenge des Profils
// Argument         : int& iFrontCut        Laenge abzuschneiden, um Bedingung zu erfuellen
// Argument         : int& iNewOffs2Hole    neuer Offset zum naechsten Loch
BOOL ComputeBHProfil(int iOffs2Hole, int iLength, int& iFrontCut, int& iNewOffs2Hole)
{
    const int c_iSpace = 133;
    const int c_iHole  = 60;
    const int c_iMin   = 20;

    int iTmp;
    // 1. Test,ob Offset zum Loch >= 20mm ist
    if (iOffs2Hole < c_iMin)
    {
        // nein, Offset bis nach dem Loch abschneiden
        iFrontCut = iOffs2Hole + c_iHole;
        iTmp = c_iSpace;         // neuer Abstand zum naechsten Loch
    }
    else
    {
        iFrontCut = 0;
        iTmp = iOffs2Hole;
    }

    // 2. Berechnen, wieviel Loecher in die Laenge passen
    // Formel: [Rest + ] iHoles*60 + (iHoles-1)*133 = iLength
    int iHoles = (iLength + c_iSpace)/(c_iHole+c_iSpace);
    if (iHoles <= 0)
        // Fehler, darf nicht vorkommen!!!!
        return FALSE;

    // 3. Berechnen des Rests
    int iRest = iLength + c_iSpace - iHoles*(c_iHole + c_iSpace);
#if 0
    if (iRest > c_iSpace)
    {
        iRest = c_iSpace - iRest;
    }
#endif
        
    if ((iRest - iTmp) > c_iSpace)
    {
        // reicht in neues Loch rein, ein Loch dazu
        iHoles++;
        iRest = iLength + c_iSpace - iHoles*(c_iHole + c_iSpace);
    }

    // 4. Testen ob nach letztem Loch noch mindestens 20 mm Abstand ist
    int iOffsAfterHole = iRest - iTmp;
    if (iOffsAfterHole == -1*c_iHole)
        // liegt genau am Beginn des Lochs, ok.
        iOffsAfterHole = c_iSpace;
    else
    if (iOffsAfterHole < -1*c_iHole)
        // liegt links vom letzten Loch
        iOffsAfterHole = c_iSpace + (c_iHole + iOffsAfterHole);
    else
    if (iOffsAfterHole < c_iMin)
    {
        // nein, entsprechend vorn mehr wegschneiden
//        if (iOffsAfterHole < 0)
//            iFrontCut += (c_iMin - iOffsAfterHole) + (c_iHole + iOffsAfterHole);
//        else
        iFrontCut += (c_iMin - iOffsAfterHole);

        iOffsAfterHole = c_iMin;
    }

    if ((iTmp - iFrontCut) < c_iMin)
    {
        int iFC;
        BOOL bRet = ComputeBHProfil((iTmp - iFrontCut), iLength, iFC, iNewOffs2Hole);
        iFrontCut += iFC;
        // Fehler, darf nicht vorkommen!!!!!
        return bRet;
    }

    
    iNewOffs2Hole = c_iSpace - iOffsAfterHole;

    return TRUE;
}

void CBHTestDlg::OnBhTestCompute() 
{
    AllMembersToValues();
    if (ComputeBHProfil(m_iOffs2Hole, m_iProfilLength, m_iResult, m_iNewOffs2Hole) == FALSE)
    {
        MessageBox("Fehler in Berechnung!!!");
        return;
    }

    AllMembersToStrings();
}

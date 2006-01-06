// HoleProfileDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "HoleProfileDoc.h"
#include "HoleProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CHoleProfileCompute


// Function name	: ComputeBHProfil
// Description	    : berechnet Zuschnitt fuer gelochte Profile (links und rechts neben Profile mind. 20 mm
//                    Abstand
// Return type		: BOOL
// Argument         : int iOffs2Hole        Offset zum naechsten Loch
// Argument         : int iLength           gewuenschte Laenge des Profils
// Argument         : int& iFrontCut        Laenge abzuschneiden, um Bedingung zu erfuellen
// Argument         : int& iNewOffs2Hole    neuer Offset zum naechsten Loch
BOOL CHoleProfileCompute::Compute(int iOffs2Hole, int iLength, int& iFrontCut, int& iNewOffs2Hole)
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
        BOOL bRet = Compute((iTmp - iFrontCut), iLength, iFC, iNewOffs2Hole);
        iFrontCut += iFC;
        // Fehler, darf nicht vorkommen!!!!!
        return bRet;
    }

    
    iNewOffs2Hole = c_iSpace - iOffsAfterHole;

    return TRUE;
}


BOOL CHoleProfileCutOptimize::Go(CPtrArray* pProfiles, CPtrArray* pCutList)
{
    CHoleProfile* pProfile, *pMem;
    int iProfileCount, i, idx, cnt, iLengthCnt; 
    HoleProfileOpt* p;          // Array fuer Profile
    CPtrArray cutList, tempList;

    // Anzahl Profile ermitteln
    iProfileCount = 0;
    iLengthCnt = pProfiles->GetSize();
    for (i = 0; i < iLengthCnt; i++)
    {
        pProfile = (CHoleProfile*)pProfiles->GetAt(i);
        if (pProfile)
        {
            iProfileCount += pProfile->m_dwCount;
        }
        else return FALSE;
    }
    p = new HoleProfileOpt[iProfileCount];
    if (p == NULL)
        return FALSE;
    pMem = new CHoleProfile[iProfileCount];   // Speicher fuer temporäre Profil-Liste
    if (pMem == NULL)
        return FALSE;


    
#if 0  
    // Profile in Array eintragen in aufsteigender Reihenfolge
    idx = 0;
    for (i = 0; i < iLengthCnt; i++)
    {
        pProfile = (CHoleProfile*)pProfiles->GetAt(i);
        if (pProfile)
        {
            cnt = pProfile->m_dwCount;
            while (cnt > 0)
            {
                p[idx].dwLength   = pProfile->m_dwLength;
                p[idx++].byValid  = 1;
                cnt--;
            }
        }
        else return FALSE;
    }
#else
    // Profile in Array eintragen in zufälliger Reihenfolge
    idx = 0;
    for (i = 0; i < iLengthCnt; i++)
    {
        pProfile = (CHoleProfile*)pProfiles->GetAt(i);
        if (pProfile)
        {
            cnt = pProfile->m_dwCount;
            while (cnt > 0)
            {
                p[idx].dwLength   = pProfile->m_dwLength;
                p[idx++].byValid  = 1;
                cnt--;
            }
        }
        else return FALSE;
    }
#endif  // 0

    while (TRUE)
    {
        // neue Profil-Liste erzeugen

    

        // Cut-Liste erzeugen und Rest vergleichen

    }



}

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDoc

IMPLEMENT_DYNCREATE(CHoleProfileDoc, CDocument)

CHoleProfileDoc::CHoleProfileDoc()
{
}

BOOL CHoleProfileDoc::OnNewDocument()
{
    CHoleProfileDlg dlg(&m_Profiles);

	if (!CDocument::OnNewDocument())
		return FALSE;

    if (dlg.DoModal() != IDOK)
        return FALSE;

    if (GenerateCutList() == FALSE)
    {
        // Fehler
        AfxMessageBox("Fehler beim Berechnen der Schnittliste!!!!", MB_ICONSTOP|MB_OK);
    }


	return TRUE;
}

BOOL CHoleProfileDoc::EditProfiles(void)
{
    CHoleProfileDlg dlg(&m_Profiles);

    if (dlg.DoModal() != IDOK)
        return FALSE;

    if (GenerateCutList() == FALSE)
    {
        // Fehler
        AfxMessageBox("Fehler beim Berechnen der Schnittliste!!!!", MB_ICONSTOP|MB_OK);
    }

	return TRUE;    
}

CHoleProfileDoc::~CHoleProfileDoc()
{
    DestroyCutList();
    EmptyProfileList();
}


BEGIN_MESSAGE_MAP(CHoleProfileDoc, CDocument)
	//{{AFX_MSG_MAP(CHoleProfileDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDoc diagnostics

#ifdef _DEBUG
void CHoleProfileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHoleProfileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDoc serialization

void CHoleProfileDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDoc commands
void CHoleProfileDoc::DestroyCutList(void)
{
    // erstmal Schnittliste ruecksetzen
    for (int i = 0; i < m_CutList.GetSize(); i++)
    {
        CPtrArray* p = (CPtrArray*)m_CutList.GetAt(i);
        if (p)
        {
            for (int s = 0; s < p->GetSize(); s++)
            {
                CHoleProfileCut* pCut = (CHoleProfileCut*)p->GetAt(s);
                if (pCut)
                    delete pCut;
            }
            delete p;
        }
    }
    m_CutList.RemoveAll();
}

void CHoleProfileDoc::EmptyProfileList(void)
{
    // erstmal Schnittliste ruecksetzen
    for (int i = 0; i < m_Profiles.GetSize(); i++)
    {
        CHoleProfile* pPrf = (CHoleProfile*)m_Profiles.GetAt(i);
        if (pPrf)
            delete pPrf;
    }
    m_Profiles.RemoveAll();
}

BOOL CHoleProfileDoc::GenerateCutList(void)
{
    CHoleProfileCompute hp;
    int iOffs2Hole = HP_DEFAULT_HOLE_OFFS;
    int iProfileLength = HP_DEFAULT_LENGTH;   // zur Zeit immer von ganzem Profil ausgehen
    int iFrontCut, iNewOffs2Hole;

    // erstmal Schnittliste ruecksetzen
    DestroyCutList();

    for (int i = 0; i < m_Profiles.GetSize(); i++)
    {
        CPtrArray* p = new CPtrArray;
        if (p)
        {
            iOffs2Hole = HP_DEFAULT_HOLE_OFFS;
            iProfileLength = HP_DEFAULT_LENGTH;   // zur Zeit immer von ganzem Profil ausgehen
            CHoleProfile* pProfile = (CHoleProfile*)m_Profiles.GetAt(i);
            m_CutList.Add(p);
            for (int s = 0; s < pProfile->m_dwCount; s++)
            {
                while (TRUE)
                {
                    if (hp.Compute(iOffs2Hole, pProfile->m_dwLength, iFrontCut, iNewOffs2Hole) == TRUE)
                    {
                        if (iProfileLength < (pProfile->m_dwLength + iFrontCut))
                        {
                            // Profil nicht mehr lang genug, neues Profil nehmen
                            CHoleProfileCut* c = new CHoleProfileCut;
                            c->m_eAction = caNew;
                            c->m_dwRest = iProfileLength;
                            p->Add(c);
                            iProfileLength = HP_DEFAULT_LENGTH;
                            iOffs2Hole     = HP_DEFAULT_HOLE_OFFS;
                            continue;
                        }
                        CHoleProfileCut* c = new CHoleProfileCut;
                        c->m_eAction = caCut;
                        c->m_dwLength = iFrontCut;
                        p->Add(c);
                        iOffs2Hole = iNewOffs2Hole - CUT_SAW_THICKNESS;
                        if (iOffs2Hole < 0)
                        {
#if 0
                            // innerhalb eines Lochs, komplett wegschneiden
                            iProfileLength -= (HP_HOLE_LENGTH + iOffs2Hole);
                            iProfileLength += CUT_SAW_THICKNESS;   // wird unten abgezogen
                            iOffs2Hole = HP_HOLE_DIST;
#endif //0
                        }

                        iProfileLength -= (pProfile->m_dwLength + iFrontCut + CUT_SAW_THICKNESS);
                        if (iProfileLength <= 0)
                        {
                            // Laenge reicht nicht mehr aus, neues Profile nehmen
                            iProfileLength = HP_DEFAULT_LENGTH;
                            iOffs2Hole     = HP_DEFAULT_HOLE_OFFS;
                        }
                        break;
                    }
                    else
                        return FALSE;
                }
            }
        }
    }
    return TRUE;
}


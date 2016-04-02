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
        if ((iTmp - (c_iMin - iOffsAfterHole)) < c_iMin)
        {
            int iFC;
            BOOL bRet = Compute((iTmp - (c_iMin - iOffsAfterHole)), iLength, iFC, iNewOffs2Hole);
            iFrontCut += iFC;
            // Fehler, darf nicht vorkommen!!!!!
            return bRet;
        }

        iOffsAfterHole = c_iMin;
        iNewOffs2Hole = c_iSpace - iOffsAfterHole;

        return TRUE;
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


BOOL CHoleProfileCompute::GenerateCutList(CPtrArray* pProfileList, CPtrArray* pCutList, CUT_INFO* pInfo)
{
    int iOffs2Hole = HP_DEFAULT_HOLE_OFFS;
    int iProfileLength = HP_DEFAULT_LENGTH;   // zur Zeit immer von ganzem Profil ausgehen
    int iFrontCut, iNewOffs2Hole;

    pInfo->dwTotalRest = 0;
    pInfo->dwUsedProfiles = 1;   // mit einem Profil anfangen

    for (int i = 0; i < pProfileList->GetSize(); i++)
    {
//        iOffs2Hole = HP_DEFAULT_HOLE_OFFS;
//        iProfileLength = HP_DEFAULT_LENGTH;   // zur Zeit immer von ganzem Profil ausgehen
        CHoleProfile* pProfile = (CHoleProfile*)pProfileList->GetAt(i);
        for (int s = 0; s < (int)pProfile->m_dwCount; s++)
        {
            while (TRUE)
            {
                if (Compute(iOffs2Hole, pProfile->m_dwLength, iFrontCut, iNewOffs2Hole) == TRUE)
                {
                    if (iProfileLength < (int)(pProfile->m_dwLength + iFrontCut))
                    {
                        // Profil nicht mehr lang genug, neues Profil nehmen
                        if (pCutList != NULL)
                        {
                            CProfileCutElem* c = new CProfileCutElem;
                            c->m_eAction = caNew;
                            c->m_dwRest = iProfileLength;
                            pCutList->Add(c);
                        }
                        pInfo->dwTotalRest += iProfileLength;
                        pInfo->dwUsedProfiles++;
                        iProfileLength = HP_DEFAULT_LENGTH;
                        iOffs2Hole     = HP_DEFAULT_HOLE_OFFS;
                        continue;
                    }
                    if (pCutList != NULL)
                    {
                        CProfileCutElem* c = new CProfileCutElem;
                        c->m_eAction = caCut;
                        c->m_dwLength = iFrontCut;
                        pCutList->Add(c);
                    }
                    pInfo->dwTotalRest += iFrontCut;
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
                        pInfo->dwUsedProfiles++;
                        pInfo->dwTotalRest += (iProfileLength + (pProfile->m_dwLength + iFrontCut + CUT_SAW_THICKNESS));
                        iProfileLength = HP_DEFAULT_LENGTH;
                        iOffs2Hole     = HP_DEFAULT_HOLE_OFFS;
                    }
                    if (pCutList != NULL)
                    {
                        CProfileCutElem* c = new CProfileCutElem;
                        c->m_eAction = caLength;
                        c->m_dwLength = pProfile->m_dwLength;
                        pCutList->Add(c);
                    }
                    break;
                }
                else
                    return FALSE;
            }
        }
    }
    return TRUE;
}


int CHoleProfileCutOptimize::GetRandom(int iProfileCount)
{
    return (rand()%iProfileCount);
}

#define MAX_SHOTS  2000
BOOL CHoleProfileCutOptimize::Go(CPtrArray* pProfiles, CPtrArray* pCutList, CUT_INFO* pInfo)
{
    CHoleProfile* pProfile, *pMem;
    int iProfileCount, i, idx, cnt, iLengthCnt, iTry; 
    HoleProfileOpt* p;          // Array fuer Profile
    CPtrArray tempList;
    CUT_INFO info, newInfo;

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
    if (iProfileCount == 0)
        return TRUE;   // fertig!!!

    p = new HoleProfileOpt[iProfileCount];
    if (p == NULL)
        return FALSE;
    pMem = new CHoleProfile[iProfileCount];   // Speicher fuer temporäre Profil-Liste
    if (pMem == NULL)
        return FALSE;


    
#if 1  
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

    info.dwTotalRest = info.dwUsedProfiles = 0xffffffff;  // auf Maximal-Werte setzen
    info = *pInfo;  //??
    iTry = 0;
    while (iTry++ < MAX_SHOTS)
    {
        // neue Profil-Liste erzeugen
        idx = 0;
        while (TRUE)
        {
            i = GetRandom(iProfileCount);
            if (p[i].byValid == 1)
            {
                // noch nicht verwendet
                pProfile = &pMem[idx++];
                pProfile->m_dwCount = 1;
                pProfile->m_dwLength = p[i].dwLength;
                tempList.Add(pProfile);
                p[i].byValid = 0;
            }
            if (idx == iProfileCount)
                break;  // fertig
            if ((100*idx)/iProfileCount > 90)
            {
                // wenn nur noch 10 Prozent zu vergeben, den rest der Reihe nach 
                for (i = 0; i < iProfileCount; i++)
                {
                    if (p[i].byValid == 1)
                    {
                        pProfile = &pMem[idx++];
                        pProfile->m_dwCount = 1;
                        pProfile->m_dwLength = p[i].dwLength;
                        tempList.Add(pProfile);
                        p[i].byValid = 0;
                    }
                }
                break;
            }
        }
    

        // Cut-Liste erzeugen und Rest vergleichen
        if (m_cCut.GenerateCutList(&tempList, NULL, &newInfo) == FALSE)
            return FALSE;

        if ((newInfo.dwUsedProfiles < info.dwUsedProfiles) || 
            ((newInfo.dwUsedProfiles == info.dwUsedProfiles) && (newInfo.dwTotalRest < info.dwTotalRest)))
        {
            // neue Liste ist optimaler, merken
            // erstmal alte Liste loeschen
            for (i = 0; i < pCutList->GetSize(); i++)
            {
                CProfileCutElem* pCut = (CProfileCutElem*)pCutList->GetAt(i);
                if (pCut)
                    delete pCut;
            }
            pCutList->RemoveAll();
            if (m_cCut.GenerateCutList(&tempList, pCutList, &newInfo) == FALSE)
                return FALSE;

            info = newInfo;
        }
        for (i = 0; i < iProfileCount; i++)
        {
            p[i].byValid = 1;
        }

        // Profil-Liste leeren
        tempList.RemoveAll();
    }

    *pInfo = info; 

    if (p != NULL)
        delete p;
    if (pMem != NULL)
        delete pMem;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDoc

CHoleProfileCutOptimize CHoleProfileDoc::m_opt;
CRegistryItemDword CHoleProfileDoc::m_riHPOpt;
CRegistryItemDword CHoleProfileDoc::m_riHPOptTime;
BOOL   CHoleProfileDoc::m_bOptSetsInit = FALSE;


IMPLEMENT_DYNCREATE(CHoleProfileDoc, CDocument)

CHoleProfileDoc::CHoleProfileDoc()
{
    if (m_bOptSetsInit == FALSE)
    {
        BOOL bCreated;
        CString strSubKey, strValue;
        CTtmainApp *pApp = (CTtmainApp*) AfxGetApp();
        if (pApp)
        {
            strSubKey = _T("");
            strValue = _T("BH_PROF_OPT");
            m_riHPOpt.Connect(pApp->GetRegistryManager(), strSubKey,
                strValue, bCreated);
            if (bCreated == TRUE) m_riHPOpt = 0;  // default keine Optimierung
            strSubKey = _T("");
            strValue = _T("BH_PROF_OPT_TIME");
            m_riHPOptTime.Connect(pApp->GetRegistryManager(), strSubKey,
                strValue, bCreated);
            if (bCreated == TRUE) m_riHPOptTime = 1;
        }
        m_bOptSetsInit = TRUE;
    }
    m_SetsOpt.bDoOpt = m_riHPOpt.GetVal();
    m_SetsOpt.iOptTime = m_riHPOptTime.GetVal();
}


BOOL CHoleProfileDoc::OnNewDocument()
{
    CHoleProfileDlg dlg(&m_Profiles, &m_SetsOpt);


	if (!CDocument::OnNewDocument())
		return FALSE;

    if (dlg.DoModal() != IDOK)
        return FALSE;

    m_riHPOpt = m_SetsOpt.bDoOpt;
    m_riHPOptTime= m_SetsOpt.iOptTime;

    if (GenerateCutList(&m_Info) == FALSE)
    {
        // Fehler
        AfxMessageBox("Fehler beim Berechnen der Schnittliste!!!!", MB_ICONSTOP|MB_OK);
        return FALSE;
    }

    BeginWaitCursor();
    if (m_SetsOpt.bDoOpt == TRUE)
    {
        DWORD dwEnd = GetTickCount();
        DWORD dwAct = dwEnd;
        dwEnd += m_SetsOpt.iOptTime*1000;
        
        while (dwAct < dwEnd)
        {
            m_InfoOpt = m_Info;
            if (m_opt.Go(&m_Profiles, &m_CutListOpt, &m_InfoOpt) == FALSE)
            {
                // Fehler
                 EndWaitCursor();
                AfxMessageBox("Fehler beim Berechnen der optimierten Schnittliste!!!!", MB_ICONSTOP|MB_OK);
                return FALSE;
            }
            dwAct = GetTickCount();
        }
    }
    EndWaitCursor();

	return TRUE;
}


BOOL CHoleProfileDoc::EditProfiles(void)
{
    CHoleProfileDlg dlg(&m_Profiles, &m_SetsOpt);

    if (dlg.DoModal() != IDOK)
        return FALSE;

    m_riHPOpt = m_SetsOpt.bDoOpt;
    m_riHPOptTime= m_SetsOpt.iOptTime;

    if (GenerateCutList(&m_Info) == FALSE)
    {
        // Fehler
        AfxMessageBox("Fehler beim Berechnen der Schnittliste!!!!", MB_ICONSTOP|MB_OK);
        return FALSE;
    }
    // Optimierungs-Info loeschen, da sich an Schnittliste etwas geaendert haben koennte
    m_InfoOpt.dwUsedProfiles = 0xffffffff;
    m_InfoOpt.dwTotalRest = 0xffffffff;
    BeginWaitCursor();
    if (m_SetsOpt.bDoOpt == TRUE)
    {
        DWORD dwEnd = GetTickCount();
        DWORD dwAct = dwEnd;
        dwEnd += m_SetsOpt.iOptTime*1000;
        
        while (dwAct < dwEnd)
        {
            m_InfoOpt = m_Info;
            if (m_opt.Go(&m_Profiles, &m_CutListOpt, &m_InfoOpt) == FALSE)
            {
                // Fehler
                EndWaitCursor();
                AfxMessageBox("Fehler beim Berechnen der optimierten Schnittliste!!!!", MB_ICONSTOP|MB_OK);
                return FALSE;
            }
            dwAct = GetTickCount();
        }
    }
    EndWaitCursor();

	return TRUE;    
}

CHoleProfileDoc::~CHoleProfileDoc()
{
    DestroyCutList();
    EmptyProfileList();
    if (m_bOptSetsInit == TRUE)
    {
        m_riHPOpt.Update();
        m_riHPOptTime.Update();
    }
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
                CProfileCutElem* pCut = (CProfileCutElem*)p->GetAt(s);
                if (pCut)
                    delete pCut;
            }
            delete p;
        }
    }
    m_CutList.RemoveAll();

    for (int i = 0; i < m_CutListOpt.GetSize(); i++)
    {
        CProfileCutElem* pCut = (CProfileCutElem*)m_CutListOpt.GetAt(i);
        if (pCut)
            delete pCut;
    }
    m_CutListOpt.RemoveAll();
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


BOOL CHoleProfileDoc::GenerateCutList(CUT_INFO* pInfo)
{
    CHoleProfileCompute hp;
    int iOffs2Hole = HP_DEFAULT_HOLE_OFFS;
    int iProfileLength = HP_DEFAULT_LENGTH;   // zur Zeit immer von ganzem Profil ausgehen
    int iFrontCut, iNewOffs2Hole;

    // erstmal Schnittliste ruecksetzen
    DestroyCutList();
    if (pInfo != NULL)
    {
        pInfo->dwTotalRest = 0;
        pInfo->dwUsedProfiles = 0;
    }

    for (int i = 0; i < m_Profiles.GetSize(); i++)
    {
        CPtrArray* p = new CPtrArray;
        if (p)
        {
            iOffs2Hole = HP_DEFAULT_HOLE_OFFS;
            iProfileLength = HP_DEFAULT_LENGTH;   // zur Zeit immer von ganzem Profil ausgehen
            pInfo->dwUsedProfiles++;
            CHoleProfile* pProfile = (CHoleProfile*)m_Profiles.GetAt(i);
            m_CutList.Add(p);
            for (int s = 0; s < (int)pProfile->m_dwCount; s++)
            {
                while (TRUE)
                {
                    if (hp.Compute(iOffs2Hole, pProfile->m_dwLength, iFrontCut, iNewOffs2Hole) == TRUE)
                    {
                        if (iProfileLength < (int)(pProfile->m_dwLength + iFrontCut))
                        {
                            // Profil nicht mehr lang genug, neues Profil nehmen
                            CProfileCutElem* c = new CProfileCutElem;
                            c->m_eAction = caNew;
                            c->m_dwRest = iProfileLength;
                            p->Add(c);
                            pInfo->dwTotalRest += iProfileLength;
                            pInfo->dwUsedProfiles++;
                            iProfileLength = HP_DEFAULT_LENGTH;
                            iOffs2Hole     = HP_DEFAULT_HOLE_OFFS;
                            continue;
                        }
                        CProfileCutElem* c = new CProfileCutElem;
                        c->m_eAction = caCut;
                        c->m_dwLength = iFrontCut;
                        p->Add(c);
                        pInfo->dwTotalRest += iFrontCut;
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
                            pInfo->dwUsedProfiles++;
                            pInfo->dwTotalRest += (iProfileLength + (pProfile->m_dwLength + iFrontCut + CUT_SAW_THICKNESS));
                            iProfileLength = HP_DEFAULT_LENGTH;
                            iOffs2Hole     = HP_DEFAULT_HOLE_OFFS;
                        }
                        break;
                    }
                    else
                        return FALSE;
                }
            }
            pInfo->dwTotalRest += iProfileLength;
        }
    }
    return TRUE;
}


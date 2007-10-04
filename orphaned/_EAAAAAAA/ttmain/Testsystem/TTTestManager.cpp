// TTTestManager.cpp: implementation of the CTTTestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <errno.h>
#include <fstream.h>
#include "..\datatype.h"
#include "..\resource.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "..\torn.h"
#include "..\ftor.h"
#include "..\tuern.h"
#include "..\ttmainDoc.h"
#include "..\ttmainView.h"
#include "RegistryManager.h"
#include "TTTestManager.h"
#include "TTTestConfigDlg.h"


#define  LOG_LIMITER  "\n------------------------------------------------------------------\n"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTTestManager::CTTTestManager()
{
    m_pFrmWnd = NULL;
}

CTTTestManager::~CTTTestManager()
{
    m_riConfig.Update();
}


// Function name	: CTTTestManager::Initialize
// Description	    : initialisiert Test-Manager
// Return type		: BOOL 
// Argument         : CFrameWnd* pFrmWnd
BOOL CTTTestManager::Initialize(CFrameWnd* pFrmWnd, CRegistryManager* pRegMan)
{
    CString strSubKey, strValue;
    BOOL bCreated;

    m_pFrmWnd = pFrmWnd;
    m_pRegMan = pRegMan;

    if (m_pRegMan)
    {
        strSubKey = _T("Testsystem");
        strValue = _T("ConfigFlags");
        m_riConfig.Connect(pRegMan, strSubKey,
            strValue, bCreated);
        if (bCreated == TRUE) m_riConfig = 0;
    
        m_Config.ribfConfig.dwConfig = m_riConfig.GetVal();
    }

    m_Config.ribfCheck[0].dwTest = 0xffffffff;   // default immer alles testen!!
    m_Config.ribfCheck[1].dwTest = 0xffffffff;   // default immer alles testen!!

    m_Config.strBaseTestDir = "Testdateien\\";    
    m_Config.strTestFile = m_Config.strBaseTestDir + "Test1.txt";
    m_Config.strLogFile = m_Config.strBaseTestDir + "Log1.log";
    return TRUE;
}


// Function name	: CTTTestManager::Configure
// Description	    : startet Konfigurations-Dialog für Test-Manager
// Return type		: BOOL 
// Argument         : void
BOOL CTTTestManager::ConfigureDlg(void)
{
    CTTTestConfigDlg cfgDlg(&m_Config, NULL);

    if (cfgDlg.DoModal() == IDOK)
    {
        cfgDlg.GetConfigData(&m_Config);
        m_riConfig = m_Config.ribfConfig.dwConfig;
    }


    return TRUE;
}


// Function name	: CTTTestManager::DoTest
// Description	    : startet Test-Lauf
// Return type		: BOOL 
// Argument         : void
BOOL CTTTestManager::DoTest(void)
{
    char strRefFile[MAX_PATH];
    CTorDoor* pTor = NULL, *pRefTor = NULL;
    BOOL bResult = TRUE;
    CTtmainView* pView = NULL;
    POSITION pos;
    CDocument* pDoc = NULL;

    // Konfigurierung?
    if (m_Config.ribfConfig.bf.bShowCfgDlg == TRUE)
    {
        ConfigureDlg();
    }

    // Test-Datei öffnen
    if (OpenTestFile() == FALSE)
    {
        AfxMessageBox("Fehler beim Öffnen der Test-Datei", MB_OK|MB_ICONSTOP);
        return FALSE;
    }

    // Log-Datei öffnen
    if (OpenLogFile() == FALSE)
    {
        AfxMessageBox("Fehler beim Öffnen der Log-Datei", MB_OK|MB_ICONSTOP);
        return FALSE;
    }

    m_Statistic.dwCntCompares = m_Statistic.dwErrors = m_Statistic.dwWarnings = m_Statistic.dwRemarks = 0;
    while (TRUE)
    {
        // Eintrag holen zurück mit Tor-Objekt
        if (GetNextSet(strRefFile, &pTor) == FALSE)
        {
            LogError("Fehler beim Holen des nächsten Test-Paars: Ref-Datei=%s", strRefFile);
#if 0
            AfxMessageBox("Fehler beim Holen des nächsten Test-Paars", MB_OK|MB_ICONSTOP);
            bResult = FALSE;
            break;
#else
            continue;
#endif // 0
        }

        if (*strRefFile == 0 || pTor == NULL)
            // Ende des test-files erreicht
            break;

        m_Statistic.dwCntCompares++;

        // Profile berechnen ist in GetNextSet()
        
        // Referenz-Datei laden
        if ((pDoc = AfxGetApp()->OpenDocumentFile(strRefFile)) == NULL)
        {
            LogError("Fehler beim Laden der Referenz-Datei %s", strRefFile);
#if 0
            AfxMessageBox("Fehler beim Laden der Referenz-Datei", MB_OK|MB_ICONSTOP);
            bResult = FALSE;
            break;
#else
            continue;
#endif
        }

        // von aktivem View Referenz-Objekt holen
        pos = pDoc->GetFirstViewPosition();
        if (pos == NULL)
        {
            AfxMessageBox("Fehler beim Anzeigen des Referenz-Tors", MB_OK|MB_ICONSTOP);
            LogError("Fehler beim Anzeigen des Referenz-Tors in %s", strRefFile);
            bResult = FALSE;
            break;
        }
        pView = (CTtmainView*)pDoc->GetNextView(pos);

        pRefTor = (CTorDoor*)pView->m_pTore.GetAt(0);
        if (pRefTor == NULL)
        {
            AfxMessageBox("Fehler beim Holen des Referenz-Tors", MB_OK|MB_ICONSTOP);
            LogError("Fehler beim Holen des Referenz-Tors");
            bResult = FALSE;
            break;
        }

        // Vergleich der beiden Objekte und Ergebnis in Log-datei eintragen
        if (CompareSet(pRefTor, pTor, strRefFile) == FALSE)
        {
            if (m_Config.ribfConfig.bf.bMessageBoxOnError == TRUE)
                AfxMessageBox("Fehler beim Vergleich des Test-Paars", MB_OK|MB_ICONSTOP);
            LogError("Fehler beim Vergleich des Test-Paars");
            if (m_Config.ribfConfig.bf.bBreakOnError == TRUE)
            {
                bResult = FALSE;
                break;
            }
        }

        // Aufräumen
        if (pTor)
        {
            delete pTor;
            pTor = NULL;
        }
        if (pView)
        {
            CWnd* pWnd = pView->GetParent();
            if (pWnd)
                pWnd->SendMessage(WM_CLOSE, 0, 0);
            pView = NULL;
        }
    }

    // noch mal alles aufräumen
    if (pTor)
    {
        delete pTor;
        pTor = NULL;
    }
    if (pView)
    {
        CWnd* pWnd = pView->GetParent();
        if (pWnd)
            pWnd->SendMessage(WM_CLOSE, 0, 0);
        pView = NULL;
    }
    // Log-Datei schliessen
    CloseLogFile();

    // Test-Datei schliessen
    CloseTestFile();

    if (m_Config.ribfConfig.bf.bShowLogFile)
        ShowLogFile();

    return bResult;
}


// Function name	: CTTTestManager::OpenTestFile
// Description	    : öffnet Test-Datei
// Return type		: BOOL 
// Argument         : void
BOOL CTTTestManager::OpenTestFile(void)
{
    if (m_isTestFile.is_open())
        return FALSE;

    m_isTestFile.open(m_Config.strTestFile, ios::in|ios::nocreate, filebuf::sh_read);

    if (m_isTestFile.rdstate() & ios::failbit)
        return false;

    return TRUE;
}

BOOL CTTTestManager::CloseTestFile(void)
{
    m_isTestFile.close();
    return TRUE;
}

// Function name	: CTTTestManager::OpenLogFile
// Description	    : öffnet die Log-Datei und erzeugt Header
// Return type		: BOOL 
// Argument         : void
BOOL CTTTestManager::OpenLogFile(void)
{
    char strBuf[MAX_PATH];

    if (m_osLogFile.is_open())
        return FALSE;

    m_osLogFile.open(m_Config.strLogFile, ios::out, filebuf::sh_read|filebuf::sh_write);
    if (m_osLogFile.rdstate() & ios::failbit)
        return false;

    CTime t = CTime::GetCurrentTime();
    sprintf(strBuf, "Beginn des Testlaufs:   %02d.%02d.%02d, %02d:%02d", t.GetDay(), t.GetMonth(),
		t.GetYear(), t.GetHour(), t.GetMinute()); 

    // write header
    m_osLogFile << "Test-Log-File für Torzuschnitt-Programm\n";
    m_osLogFile << "Zuschnitt-Version:      1.72\n";
    m_osLogFile << "Name der Test-Datei:    "<< m_Config.strTestFile; m_osLogFile << "\n";
    m_osLogFile << strBuf;
    m_osLogFile << LOG_LIMITER;


    return TRUE;    
}


// Function name	: CTTTestManager::CloseLogFile
// Description	    : schliesst Log-datei
// Return type		: BOOL 
// Argument         : void
BOOL CTTTestManager::CloseLogFile(void)
{
    char strBuf[MAX_PATH];

    CTime t = CTime::GetCurrentTime();
    m_osLogFile << LOG_LIMITER;
    sprintf(strBuf, "Ende des Testlaufs:   %02d.%02d.%02d, %02d:%02d", t.GetDay(), t.GetMonth(),
		t.GetYear(), t.GetHour(), t.GetMinute());
    m_osLogFile << strBuf;

    DumpStatistic();

    m_osLogFile.close();

    return TRUE;
}

// Function name	: CTTTestManager::GetNextSet
// Description	    : liefert nächstes zu testendes Paar (Ref-datei und Tor-Objekt)
// Return type		: BOOL 
// Argument         : char** pstrRefFile
// Argument         : CTorDoor** ppTor
BOOL CTTTestManager::GetNextSet(char* strRefFile, CTorDoor** ppTor)
{
////////////////////////////////
// Format Test-Datei:
//
// [Referenz-Dateiname]
// A: Tor-Typ Füllung Anzahl OkF B H ObtH Bogen SehnenH ProfilD
// F: geteilt Oberteil Fenster Glasart Breite
// F: geteilt Oberteil Fenster Glasart Breite

    char strBuffer[1024];
    CString str;
    CTorDoor* pT;
	TDataScan dataScan;
    int iAnzFluegel;

    if (m_isTestFile.is_open() == false)
        return FALSE;

    *strRefFile = 0;
    *ppTor = NULL;

    while (m_isTestFile.eof() == false)
    {
        m_isTestFile.getline(strBuffer, 1024);
        str = strBuffer;
        str.TrimLeft();
        if ((str.IsEmpty() == FALSE) && (str.GetAt(0) == '['))
        {
            // Ref-Datei gefunden
            str.TrimLeft('[');
            str.TrimLeft();
            str.TrimRight(']');
            str.TrimRight();
            str = m_Config.strBaseTestDir + str;
            strcpy(strRefFile, str);

            // Lade Tor
            while (m_isTestFile.eof() == false)
            {
                // "A:" suchen
                int iFuellung, iAnz, iOkf, iBreite, iHoehe, iObtHoehe, iBogen,
                    iSehnenH, iProfilD, i400R, iProfilMass, iParams;
                char strTTyp[20];

                m_isTestFile.getline(strBuffer, 1024);
                str = strBuffer;
                str.TrimLeft();
                if (str.IsEmpty() == TRUE)
                    continue;
                if (str.GetAt(0) != 'A')
                    // falsches Format
                    return FALSE;
                if (str.GetAt(1) != ':')
                    // falsches Format
                    return FALSE;

                str.TrimLeft("A:");
                str.TrimLeft();

                // Parameter einlesen
                iParams = sscanf(str, "%s %d %d %d %d %d %d %d %d %d %d %d", strTTyp,&iFuellung,&iAnz,&iOkf,&iBreite,
                    &iHoehe,&iObtHoehe,&iBogen,&iSehnenH,&iProfilD,&i400R,&iProfilMass);
                if (iParams == 10)
                {
                    // Version 1, sinnvolle Defauls nehmen
                    i400R = FALSE;
                    iProfilMass = PD_70_40;
                }
                else
                if (iParams == 12)
                {
                    // Version 2
                    int i=0;
                }
                else 
                    // falsches Format
                    return FALSE;
//                else
                {
                    switch (dataScan.getTorArt(strTTyp))
                    {
			            case ATOR:
				            pT = new CTorN();
			            break;
			            case ATUER:
				            pT = new CTuerN();
			            break;
			            case FT3:
			            case FT4:
			            case FT5:
				            pT = new CFTor();
			            break;
                        case STUER:
                            pT = new CSchiebeTuer();
                            break;
                        case STOR:
                            pT = new CSchiebeTor();
                            break;
			            default:
				            return FALSE; // unbekannter Typ
                    }
                    ASSERT(pT);
		            dataScan.getTTDaten(pT, strTTyp);
		            strcpy(pT->sArtikel, strTTyp);
                    pT->Fuellung = iFuellung;
	                pT->StueckZahl = iAnz;
	                pT->Size.Hoehe = iHoehe;
	                pT->Size.Breite = iBreite;
	                pT->Size.ObtHoehe = iObtHoehe;
	                pT->OKF = iOkf;
                    pT->bBogen = iBogen;
    	            pT->Size.SehnenHoehe = iSehnenH;
    	            pT->Size.iProfilDicke = iProfilD;
                    pT->b400R = i400R;
                    pT->ProfilMass = (tProfilMass)iProfilMass;

                    // ok, fertig
                    break;
                }
            }
            while (m_isTestFile.eof() == false)
            {
                // "F:" suchen
                int iGeteilt, iOberteil, iFenster, iGlasart, iBreite, iSprosse, iParams;

                m_isTestFile.getline(strBuffer, 1024);
                str = strBuffer;
                str.TrimLeft();
                if (str.IsEmpty() == TRUE)
                    break;
                if (str.GetAt(0) != 'F')
                    // letzte Fluegel
                    break;
                if (str.GetAt(1) != ':')
                    // letzter Fluegel
                    break;
                str.TrimLeft("F:");
                str.TrimLeft();

                // Parameter einlesen
                iParams = sscanf(str, "%d %d %d %d %d %d", &iGeteilt,&iOberteil,&iFenster,
                    &iGlasart,&iBreite, &iSprosse);
                if (iParams == 5)
                {
                    // altes Format
                    iSprosse = (tSprosse)U;   // default auf U
                }
                else
                if (iParams == 6)
                {
                    // neues Format mit Sprosse
                    int i=0;
                }
                else
                    // falsches Format
                    return FALSE;
                CFlParam* pFl = new CFlParam;
                ASSERT(pFl);
                pFl->Geteilt = iGeteilt;
                pFl->OArt = iOberteil;
                pFl->FArt = iFenster;
                pFl->GArt = iGlasart;
                pFl->iBreite = iBreite;
                pFl->ObtHoehe = pT->Size.ObtHoehe;
                pFl->Sprosse = (tSprosse)iSprosse;
                pT->FlParam->Add(pFl);
                continue;
            }
            switch (pT->Art)
            {
                case STUER:
			    case ATUER:
                    iAnzFluegel = 1;
                    break;
                case STOR:
			    case ATOR:
                    iAnzFluegel = 2;
                    break;
			    break;
			    break;
			    case FT3:
                    iAnzFluegel = 3;
                    break;
			    case FT4:
                    iAnzFluegel = 4;
                    break;
			    case FT5:
                    iAnzFluegel = 5;
                    break;
			    default:
				    return FALSE; // unbekannter Typ
            }
			if (iAnzFluegel != pT->FlParam->GetSize())
            {
                // Fehler bei der Fluegel-Anzahl
                return FALSE;
            }


            // ************************** Profile + Fuellung berechnen
            pT->updateValues();
            // *******************************************************

            *ppTor = pT;
            break;
        }
    }

    return TRUE;
}


// Function name	: CTTTestManager::CompareSet
// Description	    : vergleicht die beiden Tore und trägt Probleme in Log-Datei ein
// Return type		: BOOL 
// Argument         : CTorDoor* pRefTor
// Argument         : CTorDoor* pTor
BOOL CTTTestManager::CompareSet(CTorDoor* pRefTor, CTorDoor* pTor, char* strRefFile)
{
    int iCheck = 0;
    BOOL bErr = FALSE;

    if (pRefTor->Typ == 4)
        iCheck = 0;
    else
        iCheck = 1;

    // Profile vergleichen
    if (m_Config.ribfCheck[iCheck].bf.bProfile == TRUE)
    {
        // Profile testen
        if (pRefTor->Profile->GetSize() != pTor->Profile->GetSize())
        {
            LogError("Anzahl Profile ist nicht identisch", pRefTor, pTor);
            bErr = TRUE;
        }
        if (pRefTor->Profile->GetSize() != 0)
        {
            for (int i = 0; i < pRefTor->Profile->GetSize(); i++)
            {
                CProfileElem* pRefElem = (CProfileElem*)pRefTor->Profile->GetAt(i);
                CProfileElem* pElem    = (CProfileElem*)pTor->Profile->GetAt(i);
                ASSERT(pRefElem);
                ASSERT(pElem);
                if (*pRefElem != *pElem)
                {
                    LogError("Profil %d ist nicht identisch", i);
                    bErr = TRUE;
                }
            }
        }

        // Glas-Profile vergleichen
        if (pRefTor->GlasProfile->GetSize() != pTor->GlasProfile->GetSize())
        {
            LogError("Fehler: Anzahl Glas-Profile ist nicht identisch", pRefTor, pTor);
            bErr = TRUE;
        }
        if (pRefTor->GlasProfile->GetSize() != 0)
        {
            for (int i = 0; i < pRefTor->GlasProfile->GetSize(); i++)
            {
                CGlasProfileElem* pRefElem = (CGlasProfileElem*)pRefTor->Profile->GetAt(i);
                CGlasProfileElem* pElem    = (CGlasProfileElem*)pTor->Profile->GetAt(i);
                ASSERT(pRefElem);
                ASSERT(pElem);
                if (*pRefElem != *pElem)
                {
                    LogError("Glas-Profil %d ist nicht identisch", i);
                    bErr = TRUE;
                }
            }
        }
    }
    else
    {
        LogRemark("Profile für Tor aus Datei %s werden nicht geprüft!", strRefFile);
    }

    if (m_Config.ribfCheck[iCheck].bf.bFuellung == TRUE)
    {
        // Holz-Elemente vergleichen
        if (pRefTor->HolzElemente->GetSize() != pTor->HolzElemente->GetSize())
        {
            LogError("Fehler: Anzahl Holz-Elemente ist nicht identisch", pRefTor, pTor);
            bErr = TRUE;
        }
        int iHAnz = __min(pRefTor->HolzElemente->GetSize(), pTor->HolzElemente->GetSize());
        if (iHAnz != 0)
        {
            for (int i = 0; i < iHAnz; i++)
            {
                CHolzElement* pRefElem = (CHolzElement*)pRefTor->HolzElemente->GetAt(i);
                CHolzElement* pElem    = (CHolzElement*)pTor->HolzElemente->GetAt(i);
                ASSERT(pRefElem);
                ASSERT(pElem);
                if (*pRefElem != *pElem)
                {
                    LogError("Holz-Element %d ist nicht identisch", i);
                    bErr = TRUE;
                }
            }
        }

        // Beto-Plan-Elemente vergleichen
        if (pRefTor->BetoPlanElemente->GetSize() != pTor->BetoPlanElemente->GetSize())
        {
            LogError("Fehler: Anzahl BetoPlan-Elemente ist nicht identisch", pRefTor, pTor);
            bErr = TRUE;
        }
        if (pRefTor->BetoPlanElemente->GetSize() != pTor->BetoPlanElemente->GetSize())
        {
            LogError("Anzahl BetoPlan-Elemente ist nicht identisch. Ref = %ld, Act = %ld", 
                pRefTor->BetoPlanElemente->GetSize(), pTor->BetoPlanElemente->GetSize());
        }
        int iBetoAnz = __min(pRefTor->BetoPlanElemente->GetSize(), pTor->BetoPlanElemente->GetSize());
        if (iBetoAnz != 0)
        {
            for (int i = 0; i < iBetoAnz; i++)
            {
                CBetoPlanElement* pRefElem = (CBetoPlanElement*)pRefTor->BetoPlanElemente->GetAt(i);
                CBetoPlanElement* pElem    = (CBetoPlanElement*)pTor->BetoPlanElemente->GetAt(i);
                ASSERT(pRefElem);
                ASSERT(pElem);
                if (*pRefElem != *pElem)
                {
                    LogError("BetoPlan-Element %d ist nicht identisch", i);
                    bErr = TRUE;
                }
            }
        }

        // Riegel-Elemente vergleichen
        if (pRefTor->RiegelElemente->GetSize() != pTor->RiegelElemente->GetSize())
        {
            LogError("Anzahl Riegel ist nicht identisch");
            bErr = TRUE;
        }
        if (pRefTor->RiegelElemente->GetSize() != 0)
        {
            for (int i = 0; i < pRefTor->RiegelElemente->GetSize(); i++)
            {
                CRiegelElement* pRefElem = (CRiegelElement*)pRefTor->HolzElemente->GetAt(i);
                CRiegelElement* pElem    = (CRiegelElement*)pTor->HolzElemente->GetAt(i);
                ASSERT(pRefElem);
                ASSERT(pElem);
                if (*pRefElem != *pElem)
                {
                    LogError("Riegel-Element %d ist nicht identisch", i);
                    bErr = TRUE;
                }
            }
        }
    }
    else
    {
        LogRemark("Füllung für Tor aus Datei %s wird nicht geprüft!", strRefFile);
    }

    if (bErr == TRUE)
    {
        DumpTore(pRefTor, pTor, strRefFile);
    }
    return (bErr == FALSE);
}



#define NL "\n"


// Function name	: CTTTestManager::LogError
// Description	    : trägt Fehler in Log-Datei ein
// Return type		: void 
// Argument         : const char *pFormat
// Argument         : ...
void CTTTestManager::LogError(const char *pFormat, ...)
{
    char    aBuffer[MAX_PATH];
    va_list argptr;

    ASSERT(pFormat != NULL);

    va_start (argptr, pFormat);
    vsprintf (aBuffer, pFormat, argptr);
    va_end   (argptr);

    m_osLogFile << "FEHLER: ";
    m_osLogFile << aBuffer;
    m_osLogFile << NL;

    m_Statistic.dwErrors++;

}


// Function name	: CTTTestManager::LogRemark
// Description	    : trägt Bemerkung in Log-Datei ein
// Return type		: void 
// Argument         : const char *pFormat
// Argument         : ...
void CTTTestManager::LogRemark(const char *pFormat, ...)
{
    char    aBuffer[MAX_PATH];
    va_list argptr;

    ASSERT(pFormat != NULL);

    va_start (argptr, pFormat);
    vsprintf (aBuffer, pFormat, argptr);
    va_end   (argptr);

    m_osLogFile << "REMARK: ";
    m_osLogFile << aBuffer;
    m_osLogFile << NL;

    m_Statistic.dwRemarks++;
}


// Function name	: CTTTestManager::DumpTore
// Description	    : dumpt Tor-Inhalte in Log-Datei
// Return type		: void 
// Argument         : CTorDoor* pRefTor
// Argument         : CTorDoor* pTor
void CTTTestManager::DumpTore(CTorDoor* pRefTor, CTorDoor* pTor, char* strRefFile)
{
#define DL  ","
    m_osLogFile << "DUMP - Referenz-Tor";
    if (strRefFile != NULL)
    {
        m_osLogFile << " (" << strRefFile << " ):\n";
    }
    DumpTor(pRefTor);
    m_osLogFile << "DUMP - Test-Tor:\n";
    DumpTor(pTor);
    m_osLogFile << NL<<NL;
}

void CTTTestManager::DumpTor(CTorDoor* pTor)
{
#define DL  ","

    int i;

    m_osLogFile <<"A: "<<pTor->Typ<<DL<<pTor->Fuellung<<DL<<pTor->StueckZahl<<DL;
    m_osLogFile <<pTor->OKF<<DL<<pTor->Size.Breite<<DL<<pTor->Size.Hoehe<<DL;
    m_osLogFile <<pTor->Size.ObtHoehe<<DL<<pTor->bBogen<<DL<<pTor->b400R<<DL<<pTor->ProfilMass<<NL;

    // Fluegel-Parameter
    for (i = 0; i < pTor->FlParam->GetSize(); i++)
    {
        CFlParam* pFl = (CFlParam*) pTor->FlParam->GetAt(i);
        ASSERT(pFl);
        DumpFluegel(pFl);
    }

    // Profile
    for (i = 0; i < pTor->Profile->GetSize(); i++)
    {
        CProfileElem* pE = (CProfileElem*) pTor->Profile->GetAt(i);
        ASSERT(pE);
        DumpProfil(pE, "Profil:  ");
    }

    // Glas-Profile
    for (i = 0; i < pTor->GlasProfile->GetSize(); i++)
    {
        CProfileElem* pE = (CProfileElem*) pTor->GlasProfile->GetAt(i);
        ASSERT(pE);
        DumpProfil(pE, "G-Profil:");
    }

    // Holz-Elemente
    for (i = 0; i < pTor->HolzElemente->GetSize(); i++)
    {
        CHolzElement* pE = (CHolzElement*) pTor->HolzElemente->GetAt(i);
        ASSERT(pE);
        DumpFuellung(pE, "Holz:    ");
    }

    // BetoPlan-Elemente
    for (i = 0; i < pTor->BetoPlanElemente->GetSize(); i++)
    {
        CHolzElement* pE = (CHolzElement*) pTor->BetoPlanElemente->GetAt(i);
        ASSERT(pE);
        DumpFuellung(pE, "Beto: ");
    }

    // Riegel-Elemente
    for (i = 0; i < pTor->RiegelElemente->GetSize(); i++)
    {
        CRiegelElement* pE = (CRiegelElement*) pTor->RiegelElemente->GetAt(i);
        ASSERT(pE);
        DumpRiegel(pE);
    }

    m_osLogFile.flush();
}

void CTTTestManager::DumpFluegel(CFlParam* pFl)
{
    m_osLogFile <<"Fluegel: "<<pFl->Geteilt<<DL<<pFl->OArt<<DL<<pFl->FArt<<DL;
    m_osLogFile << pFl->GArt<<DL<<pFl->iBreite<<DL<<pFl->Sprosse<<DL;
    m_osLogFile << pFl->prflOben<<DL<<pFl->prflUnten<<DL<<pFl->prflLinks<<DL<<pFl->prflRechts<<NL;
}

void CTTTestManager::DumpProfil(CProfileElem* pE, char* strName)
{
    m_osLogFile <<strName<<pE->Anzahl<<DL<<pE->Direction<<DL<<pE->Laenge<<DL<<pE->uTyp<<DL;
    m_osLogFile <<pE->iWinkel1<<DL<<pE->iWinkel2<<DL<<pE->Profil<<NL;
}

void CTTTestManager::DumpFuellung(CHolzElement* pE, char* strName)
{
    m_osLogFile <<strName<<pE->m_iAnzahl<<DL<<pE->m_iBreite<<DL<<pE->m_iLaenge<<NL;
}

void CTTTestManager::DumpRiegel(CRiegelElement* pE)
{
    m_osLogFile <<"Riegel:  "<<pE->m_iAnzahl<<DL<<pE->m_iLaenge<<NL;
}

void CTTTestManager::DumpStatistic(void)
{
    m_osLogFile <<NL<<"ERGEBNIS:             "<<m_Statistic.dwCntCompares<<" Tore, "<<m_Statistic.dwErrors<<" Fehler, ";
    m_osLogFile <<m_Statistic.dwWarnings<<" Warnungen, "<<m_Statistic.dwRemarks<<" Anmerkungen"<<NL;
}

void CTTTestManager::ShowLogFile(void)
{
    char strWDir[MAX_PATH];
    CString strFullLogFilePath;

    GetCurrentDirectory(MAX_PATH, strWDir);

    strFullLogFilePath = strWDir;
    strFullLogFilePath += "\\";
    strFullLogFilePath += m_Config.strLogFile;
    SHELLEXECUTEINFO ExecInfo;
    memset(&ExecInfo, 0, sizeof(SHELLEXECUTEINFO));
    ExecInfo.hwnd = AfxGetMainWnd()->GetSafeHwnd();
    ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
    ExecInfo.lpVerb = "open";
    ExecInfo.lpFile = m_Config.strLogFile;
    ExecInfo.lpDirectory = strWDir;
    ExecInfo.nShow = SW_SHOW;
    if (ShellExecuteEx(&ExecInfo) == TRUE)
    {
    }
    else
        AfxMessageBox("Fehler beim Öffnen der Log-Datei", MB_OK|MB_ICONWARNING);
}

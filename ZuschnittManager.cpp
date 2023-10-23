// ZuschnittManager.cpp: implementation of the CZuschnittManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Macros.h"
#include "ttdata.h"
#include "Tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"

////////////
// Defines

#define NEW_LINE    "\n"
#define COMMENT     '#'

////////////
// structs

typedef struct tagProperty
{
    eTTEigenschaften m_Property;
    char m_strName[100];
public:
} Property;

////////////
// statics

static Property g_Eigenschaften[] =
{ 
    { ART, "Art" }, { TYP, "Typ" }, { FLUEGEL_ANZ, "Flügelanzahl" },
    { RAHMEN, "Rahmen" }, { OBER_TEIL, "Oberteil" }, { GETEILT, "Geteilt" },
    { OKF, "OKF" }, { BOGEN, "Bogen" }
};  // Achtung: gleiche Reihenfolge wie enum!!!!!


////////////
// strings

static char* g_strZuschnittDateiKopf[] =
{ 
"#####################################################\n",
"# Datei für Zuschnitt-Regeln\n",
"#\n",
"# Übersicht über Schlüsselworte:\n",
"#       Schlüsselwort         mögliche Werte\n",
"#\n",
"#       Art                   Tür, Tor, *\n",
"#       Typ                   400, 600, 800, *\n",
"#       OKF                   ja, nein, *\n",
"#       Bogen                 ja, nein, *\n",
"#       Flügelanzahl          1, 2, 3, 4, 5, *\n",
"#       Rahmen                W, Z, R, *\n",
"#       Oberteil              kein, OzO, OiF, *\n",
"#       Geteilt               ja, nein, *\n",
"#\n",
"#\n",
"# Syntax-Erklärung\n",
"#\n",
"# Schlüsselword[Wert], Schlüsselword[Wert],...: Zuschnitt-Zahl\n",
"# Beispiel: Art[Tür],Typ[400],Rahmen[W]: 100\n",
"\0"
};

static char* g_strZuschnittSenk[] = 
{
"# Abschnitt für senkrechte Zuschnittzugaben in [mm]\n",
"[Senk]\n",
"\0"
};
static char* g_strZuschnittQuer[] = 
{
"# Abschnitt für waagrechte Zuschnittzugaben in [mm]\n",
"[Quer]\n",
"\0"
};
static char* g_strZuschnittOber[] = 
{
"# Abschnitt für Zuschnittzugaben bei Oberteilen in [mm]\n",
"[Ober]\n",
"\0"
};
static char* g_strZuschnittUnter[] = 
{
"# Abschnitt für Zuschnittzugaben bei Unterteilen in [mm]\n",
"[Unter]\n",
"\0"
};

static char* g_strZuschnittSchiebefenster[] = 
{
"# Abschnitt für Zugaben für Schiebefenster (senkrecht, waagrecht)\n",
"[SF]\n",
"\0"
};

static char* g_strZuschnittSchiebefensterTyp2[] = 
{
"# Abschnitt für Zugaben (negativ) für Schiebefenster Typ 2 ( senkrecht, waagrecht)\n",
"[SF2]\n",
"\0"
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZuschnittManager::CZuschnittManager()
{
    m_pBaumSenk = NULL;
    m_pBaumQuer = NULL;
    m_pBaumOber = NULL;
    m_pBaumUnter = NULL;
    m_pSF = m_pSF2 = NULL;
}

CZuschnittManager::~CZuschnittManager()
{
    ZugabeWerteRuecksetzen ();
}

BOOL CZuschnittManager::ZuschnittDateiAuswerten(char *strDateiName)
{
    char strBuffer[1024];
    CString str;

    ZugabeWerteRuecksetzen();

    if (strDateiName == NULL)
    {
        if (m_strZuschnittDatei.IsEmpty())
            return FALSE;
        strDateiName = m_strZuschnittDatei.GetBuffer(m_strZuschnittDatei.GetLength());
    }
    // Datei öffnen
    ifstream file(strDateiName);  // mode: Datei überschreiben, erzeugen falls nicht
                                  // existent
    m_strZuschnittDatei.ReleaseBuffer();

    if (file.fail())
    {
        ErrorMessage("Datei kann nicht geöffnet werden");
        return FALSE;
    }

    // Zeile einlesen bis zu einem Zuschnitt-Abschnitt
    while (file.eof() == false)
    {
        file.getline(strBuffer, 1024);
        str = strBuffer;
        str.TrimLeft();
        if ((str.IsEmpty() == FALSE) && (str.GetAt(0) == '['))
        {
            // Zuschnitt-Abschnitt gefunden
            if (AbschnittAuswerten(file, str) == false)
                return FALSE;
        }
    }

    return TRUE;
}

BOOL CZuschnittManager::ZuschnittDateiAbspeichern(char* strDateiName)
{
    if (strDateiName == NULL)
    {
        if (m_strZuschnittDatei.IsEmpty())
            return FALSE;
        strDateiName = m_strZuschnittDatei.GetBuffer(m_strZuschnittDatei.GetLength());
    }
    ofstream file(strDateiName);  // mode: Datei überschreiben, erzeugen falls nicht
                                  // existent
    m_strZuschnittDatei.ReleaseBuffer();
    if (file.fail())
    {
        ErrorMessage("Datei kann nicht geöffnet werden");
        return FALSE;
    }

    // Kopf schreiben
    WriteStringArray(file, g_strZuschnittDateiKopf);
    file<<NEW_LINE;

    // Abschnitt [Senk]
    WriteStringArray(file, g_strZuschnittSenk);
    if (m_pBaumSenk != NULL)
    {
        if (BaumInDateiSchreiben(file, m_pBaumSenk) == FALSE)
        {
            // vielleicht auch Datei löschen??
            return FALSE;
        }
    }
    file<<NEW_LINE;
    file<<NEW_LINE;

    // Abschnitt [Quer]
    WriteStringArray(file, g_strZuschnittQuer);
    if (m_pBaumQuer != NULL)
    {
        if (BaumInDateiSchreiben(file, m_pBaumQuer) == FALSE)
        {
            // vielleicht auch Datei löschen??
            return FALSE;
        }
    }
    file<<NEW_LINE;
    file<<NEW_LINE;

        // Abschnitt [Ober]
    WriteStringArray(file, g_strZuschnittOber);
    if (m_pBaumOber != NULL)
    {
        if (BaumInDateiSchreiben(file, m_pBaumOber) == FALSE)
        {
            // vielleicht auch Datei löschen??
            return FALSE;
        }
    }
    file<<NEW_LINE;
    file<<NEW_LINE;

    // Abschnitt [Unter]
    WriteStringArray(file, g_strZuschnittUnter);
    if (m_pBaumUnter != NULL)
    {
        if (BaumInDateiSchreiben(file, m_pBaumUnter) == FALSE)
        {
            // vielleicht auch Datei löschen??
            return FALSE;
        }
    }
    file<<NEW_LINE;
    file<<NEW_LINE;

    // Abschnitt [SF]
    WriteStringArray(file, g_strZuschnittSchiebefenster);
    if (m_pSF != NULL)
    {
        if (SFZugabenInDateiSchreiben(file, m_pSF) == FALSE)
        {
            // vielleicht auch Datei löschen??
            return FALSE;
        }
    }
    file<<NEW_LINE;
    file<<NEW_LINE;


    // Abschnitt [SF2]
    WriteStringArray(file, g_strZuschnittSchiebefensterTyp2);
    if (m_pSF2 != NULL)
    {
        if (SFZugabenInDateiSchreiben(file, m_pSF2) == FALSE)
        {
            // vielleicht auch Datei löschen??
            return FALSE;
        }
    }

    file<<NEW_LINE;
    file<<NEW_LINE;

    file.flush();
    return TRUE;
}

BOOL CZuschnittManager::AbschnittAuswerten(ifstream &file, CString &str)
{
    // '[' entfernen
    str.TrimLeft('[');

    // string bis ']' lesen
    CString strAbschnitt = str.SpanExcluding("]");
    if (strAbschnitt.IsEmpty())
    {
        ErrorMessage("Dateiinhalt ist nicht korrekt!");
        return FALSE;
    }

    // Abschnitt ermitteln
    if (strAbschnitt == "Senk")
    {
        return BaumAufbauen(file, &m_pBaumSenk);
    }
    else if (strAbschnitt == "Quer")
    {
        return BaumAufbauen(file, &m_pBaumQuer);
    }
    else if (strAbschnitt == "Ober")
    {
        return BaumAufbauen(file, &m_pBaumOber);
    }
    else if (strAbschnitt == "Unter")
    {
        return BaumAufbauen(file, &m_pBaumUnter);
    }
    else if (strAbschnitt == "SF")
    {
        return SFZugabenEinlesen(file, &m_pSF);
    }
    else if (strAbschnitt == "SF2")
    {
        return SFZugabenEinlesen(file, &m_pSF2);
    }
    else
    {
        ErrorMessage("Abschnitt ist unbekannt!");
        return FALSE;
    }

    return TRUE;
}

BOOL CZuschnittManager::BaumAufbauen(ifstream &file, CNode **pNode)
{
    char strBuffer[1024];
    CString str, strPart;
    CNode *pFirst = *pNode;
    CNode *pTemp = *pNode;
    CProperty property;
    int iValue, iPosition;

    while (file.eof() == false)
    {
        file.getline(strBuffer, sizeof(strBuffer));
        str = strBuffer;
        str.TrimLeft();
        if (str.IsEmpty())
        {
            // Leerzeile bedeutet Ende des Abschnitts
            return TRUE;
        }
        if (str[0] == COMMENT)
        {
            // Kommentar
            continue;
        }
        // erste Node allokieren?
        if (pFirst == NULL)
        {
            pFirst = new CNode;
            if (pFirst == NULL)
            {
                ErrorMessage("Speicherproblem!");
                return FALSE;
            }
            *pNode = pFirst;
        }

        pTemp = pFirst;
        // Zeile durchmachen
        while (str.IsEmpty() == FALSE)
        {        
            strPart = str.SpanExcluding("[");
            // str zeigt auf Property
            if (GetProperty(strPart, &property) == FALSE)
            {
                ErrorMessage("Fehler beim Auslesen der Datei:\nSchlüsselwort nicht bekannt!");
                return FALSE;
            }
            // str vor bis nach '[', zeigt auf Value
            str = str.Right(str.GetLength() - (strPart.GetLength()+1));
            strPart = str.SpanExcluding("]");
            if (property.ConvertStringToInt(strPart, iValue) == FALSE)
            {
                ErrorMessage("Syntax-Fehler!");
                return FALSE;
            }

            // Propery in Node setzen, braucht man eigentlich nur einmal???
            pTemp->SetProperty(&property);
            if (pTemp->AddChild(iValue, &pTemp, iPosition) == FALSE)
            {
                ErrorMessage("Speicherproblem!");
                return FALSE;
            }
            // pTemp zeigt jetzt auf nächste Node!!!!

            // str auf nächste Property setzen
            if (str.Find(',') != -1)
            {
                // weitere Property in der Zeile
                strPart = str.SpanExcluding(",");
                str = str.Right(str.GetLength() - (strPart.GetLength()+1));
                str.TrimLeft();
            }
            else if (str.Find(':') != -1)
            {
                // nur noch Wert in der Zeile
                strPart = str.SpanExcluding(":");
                str = str.Right(str.GetLength() - (strPart.GetLength()+1));
                str.TrimLeft();  
                str.TrimRight(); // in str Value!!!
                iValue = atoi(str);
                pTemp->SetNodeValue(iValue);
                str = "";   // neue Zeile triggern
            }
            else
            {
                // Fehler!
                ErrorMessage("Syntax-Fehler!");
                return FALSE;
            }
        }
    }
    return TRUE;
}


// Function name	: CZuschnittManager::BaumInDateiSchreiben
// Description	    : schreibt einen Baum formatiert in die Datei
// Return type		: BOOL 
// Argument         : ofstream& file
// Argument         : CNode *pNode
BOOL CZuschnittManager::BaumInDateiSchreiben(ofstream& file, CNode *pNode)
{
    CString strLeaf = "";
    return pNode->WriteLeaf(file, strLeaf);
}

BOOL CZuschnittManager::GetProperty(CString &str, CProperty *pProperty)
{
    for (int i=0; i<sizeof(g_Eigenschaften)/sizeof(Property); i++)
    {
        if (str == g_Eigenschaften[i].m_strName)
        {
            pProperty->m_Property = g_Eigenschaften[i].m_Property;
            pProperty->m_strName = g_Eigenschaften[i].m_strName;
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CZuschnittManager::BaumLoeschen(CNode *pNode)
{
    pNode->DestroyLeaf();
    delete pNode;

    return TRUE;
}


// Function name	: CZuschnittManager::ZugabeWerteHolen
// Description	    : gibt die im Baum abgespeicherten Zugabe-Werte für Tor
//                    und Flügel zurück
// Return type		: BOOL  TRUE==alles im Baum gefunden, FALSE==eine passende Regel
//                          nicht gefunden
// Argument         : CTorDoor *pTor
// Argument         : CFlParam* pFluegel
// Argument         : int* iSenk
// Argument         : int* iQuer
// Argument         : int* iOber
// Argument         : int* iUnter
// Argument         : CSchiebeFensterZugaben* pSf
// Argument         : CSchiebeFensterZugaben* pSf2
BOOL CZuschnittManager::ZugabeWerteHolen(CTorDoor *pTor, CFlParam* pFluegel,
                                            int* piSenk, int* piQuer, int* piOber, int* piUnter,
                                            CSchiebeFensterZugaben* pSf, CSchiebeFensterZugaben* pSf2)
{
    BOOL bReturn = TRUE;
    if (piSenk != NULL)
    {
        if (m_pBaumSenk != NULL)
        {
            if (ZugabeWertHolen(pTor, pFluegel, m_pBaumSenk, piSenk) == FALSE)
                bReturn = FALSE;
        }
        else
            bReturn = FALSE;
    }
    if (piQuer != NULL)
    {
        if (m_pBaumQuer != NULL)
        {
            if (ZugabeWertHolen(pTor, pFluegel, m_pBaumQuer, piQuer) == FALSE)
                bReturn = FALSE;
        }
        else
            bReturn = FALSE;
    }
    if (piOber != NULL)
    {
        if (m_pBaumOber != NULL)
        {
            if (ZugabeWertHolen(pTor, pFluegel, m_pBaumOber, piOber) == FALSE)
                bReturn = FALSE;
        }
        else
            bReturn = FALSE;
    }
    if (piUnter != NULL)
    {
        if (m_pBaumUnter != NULL)
        {
            if (ZugabeWertHolen(pTor, pFluegel, m_pBaumUnter, piUnter) == FALSE)
                bReturn = FALSE;
        }
        else
            bReturn = FALSE;
    }
    if ((pSf != NULL) && (m_pSF != NULL))
    {
        *pSf = *m_pSF;
    }
    if ((pSf2 != NULL) && (m_pSF2 != NULL))
    {
        *pSf2 = *m_pSF2;
    }
    return TRUE;
}


BOOL CZuschnittManager::ZugabeWertHolen(CTorDoor *pTor, CFlParam *pFluegel, CNode *pNode, 
                                        int* piValue)
{
    CNode* pNodeValue;
    if ((pNodeValue = pNode->GetEndNode(pTor, pFluegel)) != NULL)
    {
        // Regel gefunden
        *piValue = pNodeValue->GetNodeValue();
        return TRUE;
    }

    // keine Regel gefunden
//    *piValue = -1;  // erstmal nicht überschreien!!!!
    return FALSE;
}

void CZuschnittManager::ErrorMessage(char *strError)
{
    HWND hWnd = NULL;
    CWnd *pWnd = AfxGetMainWnd();
    if (pWnd)
    {
        hWnd = pWnd->GetSafeHwnd();
    }
    MessageBox(hWnd, strError, "Fehler!", MB_OK|MB_ICONERROR);
}

void CZuschnittManager::WriteStringArray(ofstream &file, char **strArray)
{
    int i = 0;
    while (strArray[i][0] != 0)
    {
        file<<strArray[i];
        i++;
    }
}


// Function name	: CZuschnittManager::ZugabeWerteRuecksetzen
// Description	    : löscht alle Bäume und somit alle Regeln
// Return type		: BOOL 
BOOL CZuschnittManager::ZugabeWerteRuecksetzen()
{
    if (m_pBaumSenk != NULL)
    {
        BaumLoeschen(m_pBaumSenk);
        m_pBaumSenk = NULL;
    }
    if (m_pBaumQuer != NULL)
    {
        BaumLoeschen(m_pBaumQuer);
        m_pBaumQuer = NULL;
    }
    if (m_pBaumOber != NULL)
    {
        BaumLoeschen(m_pBaumOber);
        m_pBaumOber = NULL;
    }
    if (m_pBaumUnter != NULL)
    {
        BaumLoeschen(m_pBaumUnter);
        m_pBaumUnter = NULL;
    }

    SAFE_DELETE(m_pSF);
    SAFE_DELETE(m_pSF2);

    return TRUE;
}

CNode** CZuschnittManager::BaumAnfordern(eTYPE_PROFIL profil)
{
    switch (profil)
    {
    case SENK:
        return &m_pBaumSenk;
    case QUER:
        return &m_pBaumQuer;
    case OBER:
        return &m_pBaumOber;
    case UNTER:
        return &m_pBaumUnter;
    default:
        return NULL;
    }
}

BOOL CZuschnittManager::SFZugabenInDateiSchreiben(ofstream &file, 
                                                  CSchiebeFensterZugaben *pSF)
{
    file<<pSF->m_iSenkrecht<<", "<<pSF->m_iQuer<<", "<<pSF->m_iSenkrecht_400
        <<", "<<pSF->m_iQuer_400;
    if (file.fail())
        return FALSE;
    else
        return TRUE;
}

BOOL CZuschnittManager::SFZugabenEinlesen(ifstream &file, CSchiebeFensterZugaben** ppSF)
{
    char strBuffer[1000];
    CString str, strPart;

    while (file.eof() == false)
    {
        file.getline(strBuffer, sizeof(strBuffer));
        str = strBuffer;
        str.TrimLeft();
        str.TrimRight();
        if (str.IsEmpty())
        {
            // zwar ein Abschnitt, aber keine Werte, kein Fehler
            return TRUE;
        }
        if (str[0] == COMMENT)
        {
            // Kommentar
            continue;
        }

        if (*ppSF == NULL)
        {
            *ppSF = new CSchiebeFensterZugaben;
            if (*ppSF == NULL)
            {
                ErrorMessage("Speicherproblem!");
                return FALSE;
            }
        }
        strPart = str.SpanExcluding(",");
        (*ppSF)->m_iSenkrecht = atoi(strPart);
        str = str.Right(str.GetLength() - (strPart.GetLength()+1));
        str.TrimLeft();
        strPart = str.SpanExcluding(",");
        (*ppSF)->m_iQuer = atoi(strPart);

        str = str.Right(str.GetLength() - (strPart.GetLength()+1));
        strPart = str.SpanExcluding(",");
        (*ppSF)->m_iSenkrecht_400 = atoi(strPart);
        str = str.Right(str.GetLength() - (strPart.GetLength()+1));
        str.TrimLeft();
        (*ppSF)->m_iQuer_400 = atoi(str);
        break;
    }
    if (file.fail())
        return FALSE;
    else
        return TRUE;
}


// Function name	: CZuschnittManager::ZugabeNodeHolen
// Description	    : gibt die Node für den gewünschten Zuschnitt zurück
// Return type		: CNode* 
// Argument         : eTYPE_PROFIL type
// Argument         : CTorDoor *pTor
// Argument         : CFlParam *pFluegel
CNode* CZuschnittManager::ZugabeNodeHolen(eTYPE_PROFIL type,
                                          CTorDoor *pTor, CFlParam *pFluegel)
{
    CNode* pBaum;
    switch (type)
    {
    case SENK:
        pBaum = m_pBaumSenk;
        break;
    case QUER:
        pBaum = m_pBaumQuer;
        break;
    case OBER:
        pBaum = m_pBaumOber;
        break;
    case UNTER:
        pBaum = m_pBaumUnter;
        break;
    default:
        return NULL;
    }
    if (pBaum != NULL)
        return pBaum->GetEndNode(pTor, pFluegel);
    else
        return NULL;
}

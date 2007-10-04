// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ttdata.h"
#include "Tordoor.h"
#include "Node.h"


////////////
// arrays

// Eigenschaften
//?? Achtung: Bogen rausgenommen, da nicht unterstützt!!!!
char* g_arrEigenschaftsStr[ZR_MAX_EIGENSCHAFTEN] = 
{
    "Art", "Typ", "Flügelanzahl", "Rahmen", "Oberteil", "Geteilt", "OKF"
    ,"Bogen"
};


// für alle möglichen Werte
char* g_arrArtStr[MAX_ART_STR+REST_STR] =
{
    "Tor", "Tür", "Schiebetür", "Schiebetor", "*"
};

char* g_arrTypStr[MAX_TYP_STR+REST_STR] =
{
    "400", "600", "800", "*"
};

char* g_arrRahmenStr[MAX_RAHMEN_STR+REST_STR] =
{
    "Z", "R", "W", "*"
};

char* g_arrOberteilStr[MAX_OBERTEIL_STR+REST_STR] =
{
    "kein", "OzO", "OiF", "*"
};

char* g_arrFluegelAnzStr[MAX_FLUEGELANZ_STR+REST_STR] =
{
    "2", "3", "4", "5", "*"
};

char* g_arrJaNeinStr[MAX_JANEIN_STR+REST_STR] =
{
    "ja", "nein", "*"
};

// Function name	: CProperty::GetTorValue
// Description	    : liefert in Abhängigkeit der Eigenschaft der Property den zugehörigen
//                    Wert des Tores
// Return type		: DWORD 
// Argument         : CTorDoor *pTor
// Argument         : CFlParam* pFluegel    aktueller Tor-Fluegel
int CProperty::GetTorValue(CTorDoor *pTor, CFlParam* pFluegel)
{
    switch (m_Property)
    {
    case ART:
        // FT3, FT4, FT5 auf Tor umsetzen
        if (pTor->Art == FT3 || pTor->Art == FT4 || pTor->Art == FT5)
            return ATOR;
        else
            return pTor->Art;
        break;
    case TYP:
        return pTor->Typ;
        break;
    case FLUEGEL_ANZ:
        return pTor->FlParam->GetSize();
        break;
    case RAHMEN:
        return pTor->RahmenArt;
        break;
    case OBER_TEIL:
        return pFluegel->OArt;
        break;
    case GETEILT:
        return pFluegel->Geteilt;
        break;
    case OKF:
        return pTor->OKF;
        break;
    case BOGEN:
        return pTor->bBogen;
        break;
    default:
        break;
    }
    return 0;
}


// Function name	: CProperty::ConvertStringToInt
// Description	    : gibt für einen übergebenen String den entsprechenden
//                    Integer zurück, wie von CTordoor gebraucht
// Return type		: BOOL 
// Argument         : ->CString& strValue  übergebener String
// Argument         : <-int& iValue  Ergebnis
BOOL CProperty::ConvertStringToInt(CString& strValue, int& iValue)
{
    if (m_Property == NOT_SET)
        return FALSE;

    switch (m_Property)
    {
    case ART:
        // Problem: in CTordoor wird zwischen Tor und FT3, FT4, FT5 unterschieden
        // ich verwende aber FlügelAnz zur Unterscheidung
        // Lösung: in GetTorValue() wird alles auf ATOR umgesetzt
        if (strValue == "Tür")
            iValue = ATUER;
        else if (strValue == "Tor")
            iValue = ATOR;
        else if (strValue == "Schiebetür")
            iValue = STUER;
        else if (strValue == "Schiebetor")
            iValue = STOR;
        else if (strValue == '*')
            iValue = ART_END;
        else
            return FALSE;
        break;
    case TYP:
        if (strValue == '*')
            iValue = -1;            // -1 == Restauswahl
        else
            iValue = atoi(strValue)/100;
        break;
    case FLUEGEL_ANZ:
        if (strValue == '*')
            iValue = -1;            // -1 == Restauswahl
        else
            iValue = atoi(strValue);
        break;
    case RAHMEN:
        if (strValue == '*')
            iValue = RAHMEN_END;
        else if (strValue == 'W')
            iValue = RW;
        else if (strValue == 'Z')
            iValue = RZ;
        else if (strValue == 'R')
            iValue = RR;
        else
            return FALSE;
        break;
    case OBER_TEIL:
        if (strValue == '*')
            iValue = OBERTEIL_END;
        else if (strValue == "kein")
            iValue = LEER;
        else if (strValue == "OzO")
            iValue = OzO;
        else if (strValue == "OiF")
            iValue = OiF;
        else
            return FALSE;
        break;
    case GETEILT:
    case BOGEN:
    case OKF:
        if (strValue == '*')
            iValue = -1;
        else if (strValue == "ja")
            iValue = TRUE;
        else if (strValue == "nein")
            iValue = FALSE;
        else
            return FALSE;
        break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}


// Function name	: CProperty::ConvertIntToString
// Description	    : gibt für übergebenen Int String zurück für Zuschnitts-Regeln
// Return type		: BOOL 
// Argument         : ->int iValue
// Argument         : <-CString& strValue
BOOL CProperty::ConvertIntToString(int iValue, CString& strValue)
{
    char strTemp[50];

    strValue = "";
    switch (m_Property)
    {
    case ART:
        switch (iValue)
        {
        case ATUER:
            strValue = "Tür";
            break;
        case ATOR:
        case FT3:
        case FT4:
        case FT5:
            strValue = "Tor";
            break;
        case STUER:
            strValue = "Schiebetür";
            break;
        case STOR:
            strValue = "Schiebetor";
            break;
        case ART_END:
            strValue = '*';
            break;
        default:
            return FALSE;
        }
        break;
    case TYP:
        switch (iValue)
        {
        case -1:
            strValue = '*';
            break;
        default:
            strValue = itoa(iValue*100, strTemp, 10);
            break;
        }
        break;
    case FLUEGEL_ANZ:
        switch (iValue)
        {
        case -1:
            strValue = '*';
            break;
        default:
            strValue = itoa(iValue, strTemp, 10);
            break;
        }
        break;
    case RAHMEN:
        switch (iValue)
        {
        case RAHMEN_END:
            strValue = '*';
            break;
        case RW:
            strValue = 'W';
            break;
        case RZ:
            strValue = 'Z';
            break;
        case RR:
            strValue = 'R';
            break;
        default:
            return FALSE;
        }
        break;
    case OBER_TEIL:
        switch (iValue)
        {
        case OBERTEIL_END:
            strValue = '*';
            break;
        case LEER:
            strValue = "kein";
            break;
        case OzO:
            strValue = "OzO";
            break;
        case OiF:
            strValue = "OiF";
            break;
        default:
            return FALSE;
        }
        break;
    case GETEILT:
    case OKF:
    case BOGEN:
        switch (iValue)
        {
        case -1:
            strValue = '*';
            break;
        case TRUE:
            strValue = "ja";
            break;
        case FALSE:
            strValue = "nein";
            break;
        default:
            return FALSE;
        }
        break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}

BOOL CProperty::IsDefaultValue(int iValue)
{
    switch (m_Property)
    {
    case ART:
        if (iValue == ART_END)
            return TRUE;
        break;
    case TYP:
        if (iValue == -1)
            return TRUE;
        break;
    case FLUEGEL_ANZ:
        if (iValue == -1)
            return TRUE;
        break;
    case RAHMEN:
        if (iValue == RAHMEN_END)
            return TRUE;
        break;
    case OBER_TEIL:
        if (iValue == OBERTEIL_END)
            return TRUE;
        break;
    case GETEILT:
    case OKF:
    case BOGEN:
        if (iValue == -1)
            return TRUE;
        break;
    default:
        return FALSE;
        break;
    }

    return FALSE;
}


// Function name	: CProperty::GetPropertyStrings
// Description	    : liefert ein Array mit den Strings der möglichen Eigenschaften
// Return type		: BOOL 
// Argument         : int& iCountStr    Anzahl Strings im Array
// Argument         : char*** pStr      Array von Strings
BOOL CProperty::GetPropertyStrings(int& iCountStr, char*** pStr)
{
    *pStr = g_arrEigenschaftsStr;
    iCountStr = ZR_MAX_EIGENSCHAFTEN-1;   //?? Achtung: Bogen raus, da nicht unterstützt

    return TRUE;
}

// Function name	: CProperty::GetPropertyValueStrings
// Description	    : liefert ein Array mit den Strings der möglichen Werte
// Return type		: BOOL 
// Argument         : int &iCountStr    Anzahl Strings im Array
// Argument         : char **pStr       Array von Strings
BOOL CProperty::GetPropertyValueStrings(int &iCountStr, char ***pStr)
{
    if (NOT_SET == m_Property)
        return FALSE;

    switch (m_Property)
    {
    case ART:
        *pStr = g_arrArtStr;
        iCountStr = MAX_ART_STR+REST_STR;//sizeof((char[])g_arrArtStr);
        break;
    case TYP:
        *pStr = g_arrTypStr;
        iCountStr = MAX_TYP_STR+REST_STR;
        break;
    case FLUEGEL_ANZ:
        *pStr = g_arrFluegelAnzStr;
        iCountStr = MAX_FLUEGELANZ_STR+REST_STR;
        break;
    case RAHMEN:
        *pStr = g_arrRahmenStr;
        iCountStr = MAX_RAHMEN_STR+REST_STR;
        break;
    case OBER_TEIL:
        *pStr = g_arrOberteilStr;
        iCountStr = MAX_OBERTEIL_STR+REST_STR;
        break;
    case GETEILT:
    case OKF:
    case BOGEN:
        *pStr = g_arrJaNeinStr;
        iCountStr = MAX_JANEIN_STR+REST_STR;
        break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}


// Function name	: CProperty::ConvertPropertyStrToEnum
// Description	    : wndelt einen Property-String in das entsprechende Enum
// Return type		: BOOL 
// Argument         : CString &strProp
// Argument         : eTTEigenschaft &eProp
BOOL CProperty::ConvertPropertyStrToEnum(CString &strProp, eTTEigenschaften &eProp)
{
    for (int i=0; i<ZR_MAX_EIGENSCHAFTEN; i++)
    {
        if (strProp == g_arrEigenschaftsStr[i])
        {
            eProp = (eTTEigenschaften)i;
            return TRUE;
        }
    }

    eProp = NOT_SET;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNode::CNode()
{
    m_pChildNodes = new CNAMember[INITIAL_COUNT_NODES];
    if (m_pChildNodes != NULL)
    {
        m_dwValidNodes = 0;
        m_dwCountNodes = INITIAL_COUNT_NODES;
    }
    else
    {
    }
    m_pAddValues = NULL;
    hTreeItem = NULL;
    iChildIndex = 0;
    m_pParent = NULL;
    m_iNodeValue = 0x7fffffff;      //?? auf ungültigen Wert setzen
}

CNode::~CNode()
{
    if (m_pChildNodes != NULL)
    {
        delete [] m_pChildNodes;
        m_pChildNodes = NULL;
    }
    if (m_pAddValues != NULL)
    {
        delete m_pAddValues;
        m_pAddValues = NULL;
    }
}


// Function name	: CNode::SetProperty
// Description	    : setzt die Eigenschaft, für die die Node steht
// Return type		: void 
// Argument         : DWORD dwValue     Eigenschaft vom Typ eTTEigenschaften
// Argument         : char *strName     Name der Eigenschaft
void CNode::SetProperty(eTTEigenschaften Property, char *strName)
{
    m_Property.m_Property = Property;
    m_Property.m_strName = strName;
}



// Function name	: CNode::AddChild
// Description	    : fügt eine Child Node in das Array ein oder falls schon
//                    vorhanden, gibt Zeiger auf Child-Node zurück
// Return type		: BOOL 
// Argument         : int iValue        Wert, für das die Child-Node steht
// Argument         : CNode **ppNode    Zeiger auf Child-Node, wird hier allokiert, falls NULL wird Platz reserviert
// Argument         : int &iPosition    Position, die der Child erhält
BOOL CNode::AddChild(int iValue, CNode **ppNode, int &iPosition)
{
    iPosition = -1;
    if (m_dwValidNodes == m_dwCountNodes)
    {
        // kein Platz mehr frei im Array, eventuell Array vergrößern
        return FALSE;
    }
    // Suchen ob Wert schon vorkommt
    for (DWORD i=0; i<m_dwValidNodes; i++)
    {
        if (m_pChildNodes[i].GetValue() == iValue)
        {
            CNode* pNode = m_pChildNodes[i].GetNode();
            if (ppNode == NULL && pNode == NULL)
            {
                // Fehler, zweimal reservieren nicht erlaubt
                return FALSE;
            }
            else
            if (ppNode == NULL && pNode != NULL)
            {
                // es gibt schon einen Wert, aber Reservieren ist erlaubt
                continue;  // weitersuchen
            }
            else
            if (ppNode != NULL && pNode == NULL)
            {
                // Platz ist reserviert, wird jetzt besetzt
                break;
            }
            else // ppNode != NULL && pNode != NULL
            {
                // Child schon eingetragen, zurückliefern
                iPosition = (int)i;
                *ppNode = pNode;
                return TRUE;
            }
        }
    }

    BOOL bDefaultExists = ChildWithDefaultExists();
    if (ppNode == NULL)
    {
        // NULL bedeutet, Platz reservieren für diesen Wert
        if (bDefaultExists == TRUE)
        {
            // Default nach hinten schieben
            iPosition = m_dwValidNodes-1;
            m_pChildNodes[m_dwValidNodes].Set(
                m_pChildNodes[m_dwValidNodes-1].GetValue(), 
                m_pChildNodes[m_dwValidNodes-1].GetNode());
            m_pChildNodes[m_dwValidNodes-1].Set(iValue, NULL);
            m_dwValidNodes++;
        }
        else
        {
            iPosition = m_dwValidNodes;
            m_pChildNodes[m_dwValidNodes++].Set(iValue, NULL);
        }
    }
    else
    {
        // neuen Child anlegen
        *ppNode = new CNode;
        if (*ppNode == NULL)
            return FALSE;

        ((CNode*)*ppNode)->SetParent(this);

        if (bDefaultExists == TRUE)
        {
            // Default nach hinten schieben
            iPosition = m_dwValidNodes-1;
            m_pChildNodes[m_dwValidNodes].Set(
                m_pChildNodes[m_dwValidNodes-1].GetValue(), 
                m_pChildNodes[m_dwValidNodes-1].GetNode());
            m_pChildNodes[m_dwValidNodes-1].Set(iValue, *ppNode);
            m_dwValidNodes++;
        }
        else
        {
            iPosition = m_dwValidNodes;
            m_pChildNodes[m_dwValidNodes++].Set(iValue, *ppNode);
        }
    }

    return TRUE;
}



// Function name	: CNode::RemoveChild
// Description	    : entfernt das Child und alle darunter und gibt den Speicher frei
// Return type		: BOOL 
// Argument         : CNode* pChild
BOOL CNode::RemoveChild(CNode* pChild)
{
    for (DWORD d=0; d<GetChildCount(); d++)
    {
        if (m_pChildNodes[d].GetNode() == pChild)
        {
            pChild->DestroyLeaf();
            delete pChild;
            if (d < (GetChildCount()-1))
            {
                memcpy(&m_pChildNodes[d], &m_pChildNodes[d+1], 
                        (GetChildCount()-(d+1))*sizeof(CNAMember));
            }
            break;
        }
    }
    if (d == GetChildCount())
    {
        return FALSE;       // Fehler: nicht gefunden
    }
    else
    {
        m_dwValidNodes--;
        return TRUE;
    }
}

// Function name	: CNode::GetEndNode
// Description	    : durchläuft die Baumstruktur bis die EndNode für das Tor gefunden ist
// Return type		: CNode*            Ende-Node, NUL==keine passende gefunden
// Argument         : CTorDoor *pTor    Tor
CNode* CNode::GetEndNode(CTorDoor *pTor, CFlParam* pFluegel)
{
    if (m_dwValidNodes == 0)
    {
        // diese Node ist die Ende-Node
        if (GetProperty().m_Property != NOT_SET)
        {
            // keine End-Node mit gültigem Zuschnittwert
            return NULL;
        }
        return this;
    }
    else
    {
        // ist keine Ende-Node, nächste Node suchen
        int iTorValue;
        iTorValue = m_Property.GetTorValue(pTor, pFluegel);
        for (DWORD i=0; i<m_dwValidNodes; i++)
        {
            if (m_pChildNodes[i].GetValue() == iTorValue)
            {
                // passende Node gefunden
                // falls NULL zurückgeliefert wird, kann es sein, daß der Zweig
                // ungültig ist, z.B. keine Ende-Node enthält
                // deshalb wird noch versucht eine Default-Node zu finden
                CNode* pChild;
                if ((pChild=m_pChildNodes[i].GetNode()->GetEndNode(pTor, pFluegel))
                    != NULL)
                    return pChild;
                else
                    break;  // weiter mit Default-Node
            }
        }

        // keine Node gefunden, was tun????

        // 1.Vorschlag: immer die letzte Node als default nehmen
        //return m_pChildNodes[m_dwValidNodes].GetNode()->GetEndNode(pTor, pFluegel);

        // 2.Vorschlag: Fehler mit NULL zurückliefern
        // return NULL;

        // 3.Vorschlag: noch einmal alle Nodes nach einer Default-Node durchsuchen
        for (i=0; i<m_dwValidNodes; i++)
        {
            if (m_Property.IsDefaultValue(m_pChildNodes[i].GetValue()) == TRUE)
            {
                // passende Node gefunden
                return m_pChildNodes[i].GetNode()->GetEndNode(pTor, pFluegel);
            }
        }
    }

    return NULL;
}


// Function name	: CNode::DestroyLeaf
// Description	    : gibt die Node und alle Child-Nodes frei
// Return type		: BOOL 
// Argument         : void
BOOL CNode::DestroyLeaf(void)
{
    for (DWORD i=0; i<m_dwValidNodes; i++)
    {
        if (m_pChildNodes[i].GetNode() != NULL)
        {
            m_pChildNodes[i].GetNode()->DestroyLeaf();
            delete m_pChildNodes[i].GetNode();
            m_pChildNodes[i].Set(0,NULL);
        }
    }

    return TRUE;
}


BOOL CNode::WriteLeaf(ofstream &file, CString strLeaf)
{
    char strTemp[50];
    CString strSave, strValue;

    if (IsEndNode() == TRUE)
    {
        // dies ist eine Ende-Node, String rausschreiben
        strLeaf += ": ";
        strLeaf += itoa(GetNodeValue(), strTemp, 10);
        strLeaf += '\n';
        file<<strLeaf;
        return TRUE;
    }
    else
    {
        // sich selber formatieren
        if (strLeaf.IsEmpty() == FALSE)
            strLeaf += ", ";
        strLeaf += m_Property.m_strName;
        strLeaf += '[';
        strSave = strLeaf;
        // weiter mit den Child-Nodes
        for (DWORD i=0; i<m_dwValidNodes; i++)
        {
            if (m_Property.ConvertIntToString(m_pChildNodes[i].GetValue(), strValue) == TRUE)
            {
                strLeaf += strValue;
                strLeaf += ']';
                m_pChildNodes[i].GetNode()->WriteLeaf(file, strLeaf);
                strLeaf = strSave;
            }
            else
                return FALSE;
        }
    }

    return TRUE;
}


BOOL CNode::SetValueForChild(int iChild, int iValue)
{
    if ((DWORD)iChild > m_dwValidNodes)
    {
        return FALSE;
    }

    //?? überprüfen, ob gültiger Wert

    m_pChildNodes[iChild].Set(iValue);
    return TRUE;
}


// Function name	: CNode::DeleteChildAt
// Description	    : löscht die Child-Node und alle darunter
// Return type		: BOOL 
// Argument         : int iNode
BOOL CNode::DeleteChildAt(int iNode)
{
    if ((DWORD)iNode >= GetChildCount())
        return FALSE;
    CNode* pNode = GetChildAt(iNode);
    if (pNode == NULL)
        return FALSE;

    pNode->DestroyLeaf();
    delete pNode;

    // Werte zusammenfassen
    if ((DWORD)iNode < (m_dwValidNodes-1))
    {
        // wenn nicht das letzte Array-Element
        for (DWORD i=(DWORD)iNode; i<(m_dwValidNodes-1); i++)
        {
            m_pChildNodes[i].Set(m_pChildNodes[i+1].GetValue(), 
                m_pChildNodes[i+1].GetNode());
        }
    }
    m_pChildNodes[m_dwValidNodes].Set(0, NULL);
    m_dwValidNodes--;
    return TRUE;
}



// Function name	: CNode::GetChildByValue
// Description	    : liefert für übergebenen Wert den zugehörigen Child zurück
// Return type		: CNode*   Child, NULL falls nicht gefunden
// Argument         : int iValue  Wert für Child
CNode* CNode::GetChildByValue(int iValue)
{
    for (DWORD i = 0; i < GetChildCount(); i++)
    {
        if (iValue == m_pChildNodes[i].GetValue())
        {
            return m_pChildNodes[i].GetNode();
        }
    }

    return NULL;
}


// Function name	: CNode::GetFreeValue
// Description	    : liefert, falls möglich einen Wert zurück, für den noch kein hild
//                    existiert
// Return type		: BOOL 
// Argument         : int &iValue
// Argument         : char** pStr
BOOL CNode::GetFreeValue(int &iValue, CString &strValue)
{
    int iCountStr;
    char** pStr; 
    CString strTemp;

    if (m_Property.GetPropertyValueStrings(iCountStr, &pStr)
        == FALSE)
        return FALSE;

    for (int i=0; i<iCountStr; i++)
    {
        for (DWORD d=0; d<GetChildCount(); d++)
        {
            m_Property.ConvertIntToString(m_pChildNodes[d].GetValue(), strTemp);
            if (strTemp == pStr[i])
            {
                // kommt schon vor, nächster Wert
                break;
            }
        }
        if ((d == GetChildCount()) && (i < iCountStr))
        {
            // freien Wert gefunden
            if (d == 0)
            {
                // noch gar kein Child
                strValue = pStr[0];
            }
            else
            {
                strValue = pStr[i];
            }
            m_Property.ConvertStringToInt(strValue, iValue);
            return TRUE;
        }
    }

    return FALSE;
}


// Function name	: CNode::ChildWithDefaultExists
// Description	    : gibt an, ob schon ein Child für den Default exisistiert
// Return type		: BOOL TRUE == Child mit Default
// Argument         : void
BOOL CNode::ChildWithDefaultExists(void)
{
    //?? default sollte eigentlich immer der letzte Child sein!!!
    for (DWORD d=0; d<GetChildCount(); d++)
    {
        if (m_Property.IsDefaultValue(m_pChildNodes[d].GetValue())
            == TRUE)
            return TRUE;
    }

    return FALSE;
}


// Function name	: CProperty::QueryAllowedActions
// Description	    : gibt an, welche Aktionen für die Node möglich sind
// Return type		: void 
// Argument         : CNode *pNode      fragliche Node oder nur für eigene Node
// Argument         : BOOL &bNew        einen Child hinzufügen
// Argument         : BOOL &bChange     den Wert der Node ändern
// Argument         : BOOL &bDelete     die noe und alle Childs löschen
void CNode::QueryAllowedActions(CNode *pNode, BOOL &bNew, BOOL &bChange, 
                                    BOOL &bDelete)
{
//    CNode* pParent;
    int iCntValues;
    char** ppStr;
    if (pNode == NULL)
        pNode = (CNode*)this;
//    pParent = pNode->GetParent();

    if (pNode->IsEndNode())
    {
        // Node enthält einen Zuschnittswert
        // bNew: nicht erlaubt:
        bNew = FALSE;
        // bChange: Ändern erlaubt:
        bChange = TRUE;
        // bDelete: löschen erlaubt
        bDelete = TRUE;
    }
    else
    {
        // Node ist ein Eigenschaftswert
        // bNew: nur erlaubt, wenn noch ein Wert für die Eigenschaft frei ist
        pNode->GetProperty().GetPropertyValueStrings(iCntValues, &ppStr);
        if (pNode->GetChildCount() < (DWORD)iCntValues)
            bNew = TRUE;
        else
            bNew = FALSE;
        // bChange: Ändern erlaubt, wenn  keine Childs
        if (pNode->GetChildCount() == 0)
            bChange = TRUE;
        else
            bChange = FALSE;
        // bDelete: Löschen immer erlaubt
        bDelete = TRUE;
    }
}


// Function name	: CNode::ValueForChildAllowed
// Description	    : gibt an, ob ein neuer Child diesen Wert annehmen darf
// Return type		: BOOL   FALSE == nein, TRUE == ja
// Argument         : char *strValue
BOOL CNode::ValueForChildAllowed(char *strValue)
{
    int iValue;
    CString str = strValue;
    if (GetProperty().ConvertStringToInt(str, iValue)
        == FALSE)
        return FALSE;

    for (DWORD d=0; d<GetChildCount(); d++)
    {
        if (iValue == m_pChildNodes[d].GetValue())
            return FALSE;
    }
    return TRUE;
}



// Function name	: CNode::IsPropertyPossible
// Description	    : gibt Auskunft, ob eine Node die Property annehmen kann
// Return type		: BOOL TRUE==ja, FALSE==nein
// Argument         : char *strProperty
BOOL CNode::IsPropertyPossible(char *strProperty)
{
#if 0
    if (GetProperty().m_Property != VOLATILE)
        return FALSE;       // Fehler: Node hat schon eine Property
#endif

    // alle Eltern-Nodes nach dieser Property durch suchen
    CNode* pParent = (CNode*)this;
    while ((pParent = pParent->GetParent()) != NULL)
    {
        if (pParent->GetProperty().m_strName == strProperty)
            return FALSE;       // Property kommt schon vor
    }

    return TRUE;
}

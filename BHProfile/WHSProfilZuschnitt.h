#pragma once

#include <list>
#include <vector>
#include <map>
//#include <ProfileCutElem.h>

#include "cutstock.h"
#include "..\HoleProfileDoc.h"      // fuer CUT_INFO
//#include "..\cspsol-0.3\include\cspsol.h"

using namespace std;



class CWHSProfilZuschnitt
{
    InputMap  m_InputMapFile;       // hier sind alle Profile drin, wie in der Datei
    InputMap  m_InputMap;           // hier sind alle Profile drin, die automatisch geschnitten werden
    InputMap  m_InputMapRest;       // restliche Anzahl Profile mit Stückzahl 1, die von Hand geschnitten werden
    InputMap  m_InputMapIgnore;     // Map mit Profilen, die nur geschnitten werden aber nicht verarbeitet
    vector <ProfileCutList*> m_CutList;
    int m_iProfilLänge;             // Länge eines Profils in [mm]
    int m_iSchnittVerlust;          // Verlust beim Schnitt in [mm]
    bool m_bRoundUp;                // ungerade Anzahl Profile aufrunden (ausser bei Anzahl 1)

    bool CheckCutList(InputMap& inputMap, vector <ProfileCutList*>& cutList, CUT_INFO& cInfo);
    bool ReadProfileListIntern(InputMap& inputMap, char* strFile, char* szError);
    bool ReadProfileListExcel(InputMap& inputMap, char* strFile, char* szError);
public:
    CWHSProfilZuschnitt(int iProfilLänge, int iSchnittVerlust, bool bRoundUp);
    ~CWHSProfilZuschnitt(void);

    bool UpdateSettings(int iProfilLänge, int iSchnittVerlust, bool bRoundUp);
    bool CutProfile(InputMap& inputMap, MIIter& iter, int iRestLength, ProfileCutList* pCutList);
    bool ReadProfileList(char* szFile, char* szError);
    bool GenerateCutList(void);
    void PrintInputList(void);
    void PrintCutList(void);
    int  GetProfileCount(void);
    void GetCutListForPrint(vector<int>& printList);
    void GetCutListForSaw(vector<CUT_LIST_ELEM>& sawList);
    void GetProfileList(InputMap& list);
    bool AddProfil(int iLength, int iCount);
    bool DeleteProfil(int iLength);
    bool UpdateProfil(int iLength, int iCount);
    int  GetCountForProfil(int iLength);
    bool Compare(char* szFile, bool bWithMapRest, bool bWithMapIgnore, char* szError);
};

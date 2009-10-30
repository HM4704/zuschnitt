
#include "..\stdafx.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "..\Node.h"
#include "..\ZuschnittManager.h"
#include "..\ttmain.h"
#include "..\BasicExcel.hpp"
#include "WHSProfilZuschnitt.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace YExcel;


CWHSProfilZuschnitt::CWHSProfilZuschnitt(int iProfilLänge, int iSchnittVerlust, bool bRoundUp)
{
    m_iProfilLänge = iProfilLänge;
    m_iSchnittVerlust = iSchnittVerlust;
    m_bRoundUp     = bRoundUp;
}

CWHSProfilZuschnitt::~CWHSProfilZuschnitt(void)
{
    CLIter iter;

    for ( iter = m_CutList.begin( ); iter != m_CutList.end( ); iter++ )
    {
        ProfileCutList* pCutList = *iter;

        if (pCutList)
        {
            delete pCutList;
        }
    }
}


bool CWHSProfilZuschnitt::ReadProfileList(char* strFile, char* szError)
{

    return ReadProfileListIntern(m_InputMapFile, strFile, szError);
}

bool CWHSProfilZuschnitt::ReadProfileListExcel(InputMap& inputMap, char* strFile, char* szError)
{
    BasicExcel excel;
    BasicExcelWorksheet* pSheet;

    inputMap.clear();

    try
    {
        if (excel.Load(strFile) == false)
        {
            strncpy(szError, "Datei kann nicht geöffnet werden. Ist die Datei noch in Excel geöffnet?", MAX_PATH);
            return false;
        }
    }
    catch(...)
    { 
        strncpy(szError, "Problem beim Lesen der Datei. Das Format ist unbekannt.", MAX_PATH);
        return false;
    }


    pSheet = excel.GetWorksheet((size_t)0);
    if (pSheet == NULL)
    {
        strncpy(szError, "Datei kann nicht verarbeitet werden. Die Datei enthält kein Blatt", MAX_PATH);
        return false;
    }

    size_t maxRows = pSheet->GetTotalRows();
    size_t maxCols = pSheet->GetTotalCols();

    for (size_t c=0; c<maxCols; ++c)
    {
        for (size_t r=0; r<maxRows; ++r)
        {
            printf("%10d", r+1);
            BasicExcelCell* cell = pSheet->Cell(r,c);
            switch (cell->Type())
            {
            case BasicExcelCell::UNDEFINED:
                printf(" ");
                break;

            case BasicExcelCell::INT:
                printf("%10d", cell->GetInteger());
                break;

            case BasicExcelCell::DOUBLE:
                printf("%10.6lf", cell->GetDouble());
                break;

            case BasicExcelCell::STRING:
                {
                    if (strstr(cell->GetString(), "Anzahl") != NULL)
                    {
                        // ok Spalte mit Anzahl gefunden
                        size_t col_count = c;
                        size_t col_len   = 0;
                        // jetzt nach Spalte fuer Laengen suchen
                        for (size_t c1 = (col_count + 1); c1 < maxCols; c1++)
                        {
                            BasicExcelCell* cell = pSheet->Cell(r, c1);
                            if (cell->Type() == BasicExcelCell::STRING)
                            {
                                if (strstr(cell->GetString(), "Profile") != NULL)
                                {
                                    // ok, gefunden
                                    col_len = c1;
                                    break;
                                }   
                            }
                        }
                        if (col_len != 0)
                        {
                            // alle Paare abspeichern
                            for (size_t rval = (r + 1); rval < maxRows; rval++)
                            {
                                BasicExcelCell* cell_count = pSheet->Cell(rval, col_count);
                                BasicExcelCell* cell_len   = pSheet->Cell(rval, col_len);
                                if ((cell_count->Type() == BasicExcelCell::INT) || (cell_count->Type() == BasicExcelCell::DOUBLE))
                                {
                                    // ok gueltiger Wert fuer Anzahl
                                    if ((cell_len->Type() == BasicExcelCell::INT) || (cell_len->Type() == BasicExcelCell::DOUBLE))
                                    {
                                        // ok gueltiger Wert fuer Laenge
                                        int iLen, iCount;
                                        pair< MIIter, bool > pr;

                                        if (cell_count->Type() == BasicExcelCell::INT)
                                            iCount = cell_count->GetInteger();
                                        else
                                            iCount = (int)cell_count->GetDouble();
                                        if (cell_len->Type() == BasicExcelCell::INT)
                                            iLen = cell_len->GetInteger();
                                        else
                                            iLen = (int)cell_len->GetDouble();
                                        pr = inputMap.insert ( Int_Pair ( iLen, iCount ) );
                                        if( pr.second == true ) 
                                        {
                                            // The element was inserted successfully
                                        }
                                        else 
                                        {
                                            // Key number already exists, add it
                                            ( pr.first )->second += iCount;
                                        }
                                    } // if ((cell_len->Type() == BasicExcelCell::INT) || (cell_len->Type() == BasicExcelCell::DOUBLE))
                                } // if ((cell_count->Type() == BasicExcelCell::INT) || (cell_count->Type() == BasicExcelCell::DOUBLE))
                            } // for..
                        } // if (col_len != 0)
                    }
                }
                break;

            case BasicExcelCell::WSTRING:
                wprintf(L"%10s", cell->GetWString());
                break;
            }  // switch
        } // for
    }

    if (inputMap.size() == 0)
    {
        strncpy(szError, "Es wurden keine Profile in der Datei gefunden", MAX_PATH);
        return false;
    }

    return true;
}

bool CWHSProfilZuschnitt::ReadProfileListIntern(InputMap& inputMap, char* strFile, char* szError)
{
    char strBuffer[1024];
    MIIter iter;
    bool  bNextPair;
    char  cToken = 0;
    char  szFormat[100];


    if (strstr(strFile, ".xls") != NULL)
    {
        return ReadProfileListExcel(inputMap, strFile, szError);
    }

    inputMap.clear();

    ifstream file(strFile);
    if (file.fail())
    {
        return false;
    }    


    // Zeile einlesen bis Ende
    while (file.eof() == false)
    {
        int iLen, iCount;
        char* pC;
        pair< MIIter, bool > pr;

        file.getline(strBuffer, 1024);
        
        if (cToken == 0)
        {
            // beim ersten Mal Trennzeichen ermitteln
            if (strchr(strBuffer, ';') != NULL)
            {
                strcpy(szFormat, "%ld;%ld");
                cToken = ';';
            }
            else
            {
                strcpy(szFormat, "%ld,%ld");
                cToken = ',';
            }
        }

        pC = strBuffer;
        while ((*pC == '"') || (*pC == cToken))
            pC++;

        if (*pC == 0)
            continue;

        if ((*pC >= '0') && (*pC <= '9'))
        {
            // ist eine Zeile mit Zahlen
            bNextPair = true;
            while (bNextPair)
            {
                bNextPair = false;
                if (sscanf_s(pC, szFormat, &iCount, &iLen) == 2)
                {
                    if (iCount > 0)
                    {
#if 0
                        if ((iLen < m_iSchnittVerlust) || (iLen > (m_iProfilLänge + m_iSchnittVerlust)))
                        {
                            sprintf(szError, "Fehler in Datei gefunden: Profillänge %d ist nicht möglich!", iLen);
                            return false;
                        }
#endif // 0
                        pr = inputMap.insert ( Int_Pair ( iLen, iCount ) );
                        if( pr.second == true ) 
                        {
                            // The element was inserted successfully
                        }
                        else 
                        {
                            // Key number already exists, add it
                            ( pr.first )->second += iCount;
                        }
                    }
                }
                else
                    break;

                if ((pC = strchr(pC, cToken)) != NULL)
                {
                    pC++;
                    while (TRUE)
                    {
                        if ((pC = strchr(pC, cToken)) != NULL)
                        {
                            while (*pC == cToken)
                                pC++;
                            if ((*pC >= '0') && (*pC <= '9'))
                            {
                                bNextPair = true;
                                break;
                            }
                        }
                        else
                            break;
                    }  // while (TRUE)
                }
            }
        }
    }

    return true;
}

bool CWHSProfilZuschnitt::UpdateSettings(int iProfilLänge, int iSchnittVerlust, bool bRoundUp)
{
    m_iProfilLänge = iProfilLänge;
    m_iSchnittVerlust = iSchnittVerlust;
    m_bRoundUp     = bRoundUp;

    if (m_InputMap.size())
        return GenerateCutList();
    else
        return true;
}

// schneidet aus einem Profil die Teil-Profile
//
bool CWHSProfilZuschnitt::CutProfile(InputMap& inputMap, MIIter& iter, int iRestLength, ProfileCutList* pCutList)
{
//    MIIter nextIter;

    if (iter == inputMap.end())
    {
        // keine Länge mehr, fertig
        return true;
    }
    if (iter->second > 0)
    {
        if ((iRestLength - (iter->first + m_iSchnittVerlust)) > 0)
        {
            // Länge passt rein, verwenden
            iter->second--;
            iRestLength = iRestLength - (iter->first + m_iSchnittVerlust);
            pCutList->push_back(iter->first);
            if (iter->second == 0)
            {       
                // Eintrag entfernen
                MIIter titer = iter;
                iter++;
                inputMap.erase(titer);
            }
        }
        else
        {
            // Laenge passt nicht rein, mit nächster Länge weitermachen
            iter++;
        }
    }
    else
    {
        // Eintrag entfernen
        MIIter titer = iter;
        iter++;
        inputMap.erase(titer);
    }

    return CutProfile(inputMap, iter, iRestLength, pCutList);
}

bool CWHSProfilZuschnitt::GenerateCutList(void)
{
    // Algorithmus:
    // - nimm nächstes längstes Profil aus der Inputliste
    // - zähle zusammen und ziehe von einem ganzen Profil ab
    // wenn 0 fertig
    // wenn > 0 weiter mit nächstem Profil
    // wenn < 0, dann einen Schritt zurück
    // wenn nicht mehr weiter zurück geht, dann mit Rest fertig
    MIIter iter, iter1;
    pair< MIIter, bool > pr;

    
    m_InputMap.clear();
    
    for (iter = m_InputMapFile.begin(); iter != m_InputMapFile.end(); iter++)
    {
        pr = m_InputMap.insert(Int_Pair (iter->first, iter->second));
        if( pr.second == true ) 
        {
            // The element was inserted successfully
        }
        else 
        {
            // Key number already exists, error
            ASSERT(false);
        }
    }

    m_InputMapIgnore.clear();
    m_InputMapRest.clear();

    // Behandlung fuer ungerade Anzahl
    iter = m_InputMap.begin( );
    while (iter != m_InputMap.end( ))
    {
        if (iter->first > (m_iProfilLänge-m_iSchnittVerlust))
        {
            // Profil zu lange, von Hand schneiden
            m_InputMapRest.insert(Int_Pair (iter->first, iter->second));
            iter = m_InputMap.erase(iter);
        }
        else
        if (iter->second == 1)
        {
            // nur 1 Element, in Restliste verschieben
            m_InputMapRest.insert(Int_Pair (iter->first, 1));
            // ACHTUNG: iter zeigt auf nächstes 
            iter = m_InputMap.erase(iter);
        }
        else
        if (iter->second%2)
        {
            // ungerade Anzahl, es werden immer 2 Profile gleichzeitig geschrieben
            if (m_bRoundUp == false)
            {
                // nicht aufrunden, ein Stück in Restliste legen
                m_InputMapRest.insert(Int_Pair (iter->first, 1));
                iter->second--;
            }
            else
            {
                // aufrunden, Profil in Ignore Map merken, weil es nicht verarbeitet werden soll (kein Etikett drucken)
                m_InputMapIgnore.insert(Int_Pair (iter->first, 1));
                iter->second++;
            }
            iter++;
        }
        else
        {
            iter++;
        }

    }

    InputMap inputMap(m_InputMap);

    // Anzahl halbieren, weil immer 2 Profile gleichzeitig geschnitten werden
    for ( iter = inputMap.begin( ); iter != inputMap.end( ); iter++ )
    {
        if ((iter->second%2) == 0)
        {
            iter->second /= 2;
        }
        else
        {
            // ungerade Anzahl darf hier nicht mehr vorkommen, Abbruch
            return false;
        }
    }

    if (!m_CutList.empty())
    {
        CLIter clIter;
        for ( clIter = m_CutList.begin( ); clIter != m_CutList.end( ); clIter++ )
        {
            ProfileCutList* pCutList = *clIter;
            delete pCutList;
        }
        m_CutList.clear();
    }
    while (!inputMap.empty())
    {
        iter = inputMap.begin();
        ProfileCutList* pCutList = new ProfileCutList;

        // nimm längstes Profil
        if (CutProfile(inputMap, iter, m_iProfilLänge, pCutList) == true)
        {
            m_CutList.push_back(pCutList);
        }
        else
        {
            break;
        }
    }

    CUT_INFO cInfo;
    if (CheckCutList(m_InputMap, m_CutList, cInfo) == false)
    {
        TRACE("******CheckCutList ERROR: Fehler bei eigner Cutlist Routine");
        //??
        return false;
    }

#if 0
    InputMap inputMap2(m_InputMap);
    vector <ProfileCutList*> cutList;
    // Anzahl halbieren, weil immer 2 Profile gleichzeitig geschnitten werden
    for ( iter = inputMap2.begin( ); iter != inputMap2.end( ); iter++ )
    {
        if ((iter->second%2) == 0)
        {
            iter->second /= 2;
        }
        else
        {
            // ungerade Anzahl darf hier nicht mehr vorkommen, Abbruch
            return false;
        }
    }

    CSPSolve(inputMap2, m_iProfilLänge, cutList);
    CUT_INFO cInfoOpt;
    if (CheckCutList(m_InputMap, cutList, cInfoOpt) == false)
    {
        TRACE("******CheckCutList ERROR: Fehler bei optimierter Cutlist Routine");
        //??
    }


    if ((cInfoOpt.dwUsedProfiles < cInfo.dwUsedProfiles) || 
        ((cInfoOpt.dwUsedProfiles == cInfo.dwUsedProfiles) && (cInfoOpt.dwTotalRest < cInfo.dwTotalRest)))
    {
        // optimale Loesung besser, Cutlists vertauschen
        m_CutList.swap(cutList);
    }
    // andere Cutlists loeschen
    CLIter piter;
    for ( piter = cutList.begin( ); piter != cutList.end( ); piter++ )
    {
        ProfileCutList* pCutList = *piter;

        if (pCutList)
            delete pCutList;
    }
#endif // 0

//#define USE_CUTSTOCK_ALG
    // use optimal solution
#ifdef USE_CUTSTOCK_ALG
    InputMap inputMap2(m_InputMap);
    CUTSTOCK_INPUT input;
    CUTSTOCK_OUTPUT output;
    vector <ProfileCutList*> cutList;
    input.iSrcCost = 100;
    input.iSrcLength = m_iProfilLänge;
    output.pCutList = &cutList;

    // Anzahl halbieren, weil immer 2 Profile gleichzeitig geschnitten werden
    for ( iter = inputMap2.begin( ); iter != inputMap2.end( ); iter++ )
    {
        if ((iter->second%2) == 0)
        {
            iter->second /= 2;
        }
        else
        {
            // ungerade Anzahl darf hier nicht mehr vorkommen, Abbruch
            return false;
        }
    }
    input.pInpuMap = &inputMap2;
    cutstock_main(&input, &output);
    CUT_INFO cInfoOpt;
    if (CheckCutList(m_InputMap, cutList, cInfoOpt) == false)
    {
        TRACE("******CheckCutList ERROR: Fehler bei optimierter Cutlist Routine");
        //??
    }


    if ((cInfoOpt.dwUsedProfiles < cInfo.dwUsedProfiles) || 
        ((cInfoOpt.dwUsedProfiles == cInfo.dwUsedProfiles) && (cInfoOpt.dwTotalRest < cInfo.dwTotalRest)))
    {
        // optimale Loesung besser, Cutlists vertauschen
        m_CutList.swap(cutList);
    }
    // andere Cutlists loeschen
    CLIter piter;
    for ( piter = cutList.begin( ); piter != cutList.end( ); piter++ )
    {
        ProfileCutList* pCutList = *piter;

        if (pCutList)
            delete pCutList;
    }
#endif // USE_CUTSTOCK_ALG

    return true;
}

bool CWHSProfilZuschnitt::CheckCutList(InputMap& inputMap, vector <ProfileCutList*>& cutList, CUT_INFO& cInfo)
{
    CLIter iter;
    InputMap newInput;
    InputMap::iterator mIter;
    pair< MIIter, bool > pr;
    DWORD dwLength;

    cInfo.dwUsedProfiles = 0;
    cInfo.dwTotalRest    = 0;

    // erzeuge InputMap
    for ( iter = cutList.begin( ); iter != cutList.end( ); iter++ )
    {
        ProfileCutList* pCutList = *iter;

        cInfo.dwUsedProfiles++;

        if (pCutList)
        {
            list<int>::iterator liter;
            dwLength = 0;
            for ( liter = pCutList->begin( ); liter != pCutList->end( ); liter++ )
            {
                dwLength += *liter;
                pr = newInput.insert ( Int_Pair ( *liter, 2 /* es werden immer zwei geschnitten */) );
                if( pr.second == true ) 
                {
                    // The element was inserted successfully
                }
                else 
                {
                    // Key number already exists, add it
                    ( pr.first )->second += 2;  //?? es werden immer zwei geschnitten
                }
            }
            if (dwLength > m_iProfilLänge)
            {
                TRACE("******CheckCutList ERROR: Profil Abfolge länger als Ausgangsprofil!!!");
                return false;
            }
            cInfo.dwTotalRest += (m_iProfilLänge - dwLength);
        }    
    }

    // vergleiche neue und originale InputMap
    InputMap::iterator newIter, orgIter;
    if (newInput.size() != inputMap.size())
    {
        TRACE("******CheckCutList ERROR: original und neue InputMap sind nicht gleich gross");
        return false;
    }

    for (newIter = newInput.begin(), orgIter = inputMap.begin(); 
         newIter != newInput.end(), orgIter != inputMap.end(); newIter++, orgIter++)
    {
        if ((newIter->first != orgIter->first) || (newIter->second != orgIter->second))
        {
            TRACE("******CheckCutList ERROR: original(%ld, %ld) und neuer Eintrag(%ld, %ld) sind unterschiedlich", orgIter->first,
                orgIter->second, newIter->first, newIter->second);
            return false;
        }
    }

    return true;
}

int  CWHSProfilZuschnitt::GetProfileCount(void)
{
    MIIter iter;
    int iCount = 0;
    for ( iter = m_InputMap.begin( ); iter != m_InputMap.end( ); iter++ )
    {
        iCount += iter->second;
    }

    // Anzahl Elemente in Rest Map addieren
    for ( iter = m_InputMapRest.begin( ); iter != m_InputMapRest.end( ); iter++ )
    {
        iCount += iter->second;
    }
    // Elemente in Ignore Map abziehen
    iCount -= m_InputMapIgnore.size();

    return iCount;
}

void CWHSProfilZuschnitt::PrintInputList(void)
{
    MIIter iter;

    cout<<"Input List:"<<endl;

    for ( iter = m_InputMap.begin( ); iter != m_InputMap.end( ); iter++ )
        cout<<" "<<iter->first<<", "<< iter->second<<endl;

}

void CWHSProfilZuschnitt::PrintCutList(void)
{
    CLIter iter;

    cout<<endl<<"Cut List:"<<endl;

    for ( iter = m_CutList.begin( ); iter != m_CutList.end( ); iter++ )
    {
        ProfileCutList* pCutList = *iter;

        if (pCutList)
        {
            list<int>::iterator liter;
            for ( liter = pCutList->begin( ); liter != pCutList->end( ); liter++ )
                cout<<" "<<*liter;
            cout<<endl;
        }    
    }
}

// liefert die Profile in der Reihenfolge zurück wie sie geschnitten werden
// inklusive der Rest Profile und exklusiv der Ignore Profile
void CWHSProfilZuschnitt::GetCutListForPrint(vector<int>& printList)
{
    CLIter iter;
    MIIter miter;
    int i;

    InputMap  inputMapIgnore(m_InputMapIgnore);

    printList.clear();
    for ( iter = m_CutList.begin( ); iter != m_CutList.end( ); iter++ )
    {
        ProfileCutList* pCutList = *iter;

        if (pCutList)
        {
            list<int>::iterator liter;
            for ( liter = pCutList->begin( ); liter != pCutList->end( ); liter++ )
            {
                // jedes Profil wird doppelt geschnitten
                if ((miter = inputMapIgnore.find(*liter)) != inputMapIgnore.end())
                {
                    // dieses Profil ist in der Ignore Liste, nur einmal einfügen
                    // und aus Ignore Liste rausnehmen
                    inputMapIgnore.erase(miter);
                }
                else
                {
                    printList.push_back(*liter);
                }
                printList.push_back(*liter);
            }
        }    
    }

    // jetzt noch die Profile aus der Rest Liste einfügen
    for ( miter = m_InputMapRest.begin( ); miter != m_InputMapRest.end( ); miter++ )
    {
        for (i = 0; i < miter->second; i++)
            printList.push_back(miter->first);
    }
}

// liefert die Profile in der Reihenfolge wie sie zur Säge gesendet werden sollen
// aufeinander folgende gleichlange Profile werden aufaddiert
// Anzahl geteilt durch 2, exklusive der Rest Profile und exklusiv der Ignore Profile
void CWHSProfilZuschnitt::GetCutListForSaw(vector<CUT_LIST_ELEM>& sawList)
{
    CLIter iter;
    CUT_LIST_ELEM cle;

    cle.iLength = cle.iCount = 0;

    sawList.clear();

    for ( iter = m_CutList.begin( ); iter != m_CutList.end( ); iter++ )
    {
        ProfileCutList* pCutList = *iter;

        if (pCutList)
        {
            list<int>::iterator liter;
            for ( liter = pCutList->begin( ); liter != pCutList->end( ); liter++ )
            {
                if (cle.iLength != *liter)
                {
                    // neue Länge
                    if (cle.iCount != 0)
                        sawList.push_back(cle);
                    cle.iLength = *liter;
                    cle.iCount = 1;
                }
                else
                {
                    // gleiches Profil, Anzahl erhöhen
                    cle.iCount++;
                }
            }
        }
    }
    if (cle.iCount != 0)
        sawList.push_back(cle);
}

void CWHSProfilZuschnitt::GetProfileList(InputMap& list)
{
    MIIter iter;
    pair< MIIter, bool > pr;

    // erst mal alle 
    list = m_InputMap;

    // Elemente in rest map hinzu
    for ( iter = m_InputMapRest.begin( ); iter != m_InputMapRest.end( ); iter++ )
    {
//        assert(iter->second == 1);
        pr = list.insert ( Int_Pair ( iter->first, iter->second ) );
        if( pr.second == true ) 
        {
            // ok, gabs noch nicht
        }
        else 
        {
            // Key number already exists, add it
            ( pr.first )->second += iter->second;
        }

    }

    // Elemente in Ignore Map abziehen
    for ( iter = m_InputMapIgnore.begin( ); iter != m_InputMapIgnore.end( ); iter++ )
    {
        MIIter titer;
        assert(iter->second == 1);
        if ((titer = list.find(iter->first)) != list.end())
        {
            assert(titer->second > 1);
            titer->second -= iter->second;
        }
        else
        {
            // darf nicht vorkommen
            assert(false);
        }
    }
}

bool CWHSProfilZuschnitt::AddProfil(int iLength, int iCount)
{
    pair< MIIter, bool > pr;

    pr = m_InputMapFile.insert ( Int_Pair ( iLength, iCount ) );
    if( pr.second == true ) 
    {
        // The element was inserted successfully
    }
    else 
    {
        // Key number already exists, not allowed
        return false;
    }

    return GenerateCutList();
}

bool CWHSProfilZuschnitt::DeleteProfil(int iLength)
{
    if (m_InputMapFile.erase(iLength) != 1)
    {
        return false;
    }

    return GenerateCutList();
}

bool CWHSProfilZuschnitt::UpdateProfil(int iLength, int iCount)
{
    MIIter mIter;
    if ((mIter = m_InputMapFile.find(iLength)) != m_InputMapFile.end())
    {
        mIter->second = iCount;
        return GenerateCutList();
    }
    else
        return false;
}

int  CWHSProfilZuschnitt::GetCountForProfil(int iLength)
{
    MIIter mIter;
    if ((mIter = m_InputMapFile.find(iLength)) != m_InputMapFile.end())
        return mIter->second;
    else
        return 0;
}

bool CWHSProfilZuschnitt::Compare(char* szFile, bool bWithMapRest, bool bWithMapIgnore, char* szError)
{
    InputMap inputMap;
    bool bOk = true;
    MIIter iter, iter1;

    if (ReadProfileListIntern(inputMap, szFile, szError) == false)
        return false;

    for (iter = m_InputMap.begin(), iter1 = inputMap.begin(); iter != m_InputMap.end(), iter1 != inputMap.end(); iter++, iter1++)
    {
        if ((iter->first != iter1->first) || (iter->second != (2*iter1->second)))
        {
            sprintf(szError, "Fehler: Original(%ld, %ld) und Neu(%ld, %ld) stimmen nicht überein!!!", 
                iter->first, iter->second, iter1->first, iter1->second);
//            return false;
            bOk = false;
        }
    }

    return bOk;
}

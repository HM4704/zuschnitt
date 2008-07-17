// ZuschnittManager.h: interface for the CZuschnittManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZUSCHNITTMANAGER_H__0471C6A5_A44D_11D5_8E7C_8FDE04C7301C__INCLUDED_)
#define AFX_ZUSCHNITTMANAGER_H__0471C6A5_A44D_11D5_8E7C_8FDE04C7301C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream>

using namespace std;

//class ZRTreeCtrl;
typedef enum { SENK=0, QUER, OBER, UNTER } eTYPE_PROFIL;


class CSchiebeFensterZugaben
{
public:
    CSchiebeFensterZugaben() { m_iSenkrecht = m_iQuer = 
                               m_iSenkrecht_400 = m_iQuer_400= 0; };
    int     m_iSenkrecht;
    int     m_iQuer;
    int     m_iSenkrecht_400;
    int     m_iQuer_400;
};


class CZuschnittManager  
{

friend class CZRTreeCtrl;

    CNode *m_pBaumSenk;
    CNode *m_pBaumQuer;
    CNode *m_pBaumOber;
    CNode *m_pBaumUnter;

    CString m_strZuschnittDatei;

public:
    CSchiebeFensterZugaben *m_pSF;                 // Zugaben für Schiebefenster
    CSchiebeFensterZugaben *m_pSF2;                 // Zugaben für Schiebefenster Typ2

    void ZuschnittDateiSetzen(CString& strDateiName) 
        { m_strZuschnittDatei = strDateiName; };
	CNode** BaumAnfordern(eTYPE_PROFIL profil);
	BOOL ZugabeWerteRuecksetzen(void);
	void ErrorMessage(char* strError);
	BOOL ZuschnittDateiAuswerten(char* strDateiName);
    BOOL ZuschnittDateiAbspeichern(char* strDateiName);
    BOOL ZugabeWerteHolen(CTorDoor *pTor, CFlParam* pFluegel, 
        int* piSenk, int* piQuer, int* piOber, int* piUnter, 
        CSchiebeFensterZugaben* pSf, CSchiebeFensterZugaben* pSf2);
    CNode* ZugabeNodeHolen(eTYPE_PROFIL type,
                           CTorDoor *pTor, CFlParam *pFluegel);
	CZuschnittManager();
	virtual ~CZuschnittManager();

protected:
	BOOL SFZugabenEinlesen(ifstream& file, CSchiebeFensterZugaben** ppSF);
	BOOL SFZugabenInDateiSchreiben(ofstream& file, CSchiebeFensterZugaben* pSF);
	BOOL ZugabeWertHolen(CTorDoor* pTor, CFlParam* pFluegel, CNode* pNode, int* piValue);
	BOOL BaumInDateiSchreiben(ofstream& file, CNode* pNode);
	void WriteStringArray(ofstream& file, char **strArray);
	BOOL BaumLoeschen(CNode* pNode);
	BOOL GetProperty(CString& str, CProperty* pProperty);
	BOOL BaumAufbauen(ifstream& file, CNode** pNode);
	BOOL AbschnittAuswerten(ifstream& file, CString& str);
};

#endif // !defined(AFX_ZUSCHNITTMANAGER_H__0471C6A5_A44D_11D5_8E7C_8FDE04C7301C__INCLUDED_)

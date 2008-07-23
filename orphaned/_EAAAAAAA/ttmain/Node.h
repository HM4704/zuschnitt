// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__0471C6A1_A44D_11D5_8E7C_8FDE04C7301C__INCLUDED_)
#define AFX_NODE_H__0471C6A1_A44D_11D5_8E7C_8FDE04C7301C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream.h>

///////////
// Defines

#define  INITIAL_COUNT_NODES        10    // max. 10 Verzweigungen am Anfang
#define  ZR_MAX_EIGENSCHAFTEN       8    // Anzahl der Eigenschaften

// Anzahl der Werte
#define MAX_ART_STR                 2    // 
#define MAX_TYP_STR                 3
#define MAX_RAHMEN_STR              3
#define MAX_OBERTEIL_STR            3
#define MAX_FLUEGELANZ_STR          4
#define MAX_JANEIN_STR              2
#define REST_STR                    1
#define ZR_MAX_WERTE  (MAX_ART_STR+MAX_TYP_STR+MAX_RAHMEN_STR+MAX_OBERTEIL_STR+MAX_FLUEGELANZ_STR+MAX_JANEIN_STR+REST_STR)



///////////
// Enums

typedef enum 
{
    ART = 0, TYP, FLUEGEL_ANZ, RAHMEN, OBER_TEIL, GETEILT, OKF, BOGEN, NOT_SET,
    VOLATILE /* fuer TreeCtrl */
} eTTEigenschaften;

///////////
// Klassen

class CNode;

class CProperty
{
public:
	BOOL ConvertPropertyStrToEnum(CString& strProp, eTTEigenschaften& eProp);
	BOOL GetPropertyValueStrings(int& iCountStr, char*** pStr);
	BOOL GetPropertyStrings(int& iCountStr, char*** pStr);
    eTTEigenschaften m_Property;
    CString m_strName;
    CProperty() { m_Property = NOT_SET; m_strName = ""; };
    void SetProperty(CProperty *pProp) 
    { 
        m_Property = pProp->m_Property;
        m_strName = pProp->m_strName; 
    };
    virtual BOOL ConvertStringToInt(CString& strValue, int& iValue);
    virtual BOOL ConvertIntToString(int iValue, CString& strValue);
	virtual int  GetTorValue(CTorDoor* pTor, CFlParam* pFluegel);
    virtual BOOL IsDefaultValue(int iValue);
    CString GetName(void) { return m_strName; };
    eTTEigenschaften GetPropertyValue(void) { return m_Property; };
    virtual ~CProperty() {};
};

// Node Array Member
class CNAMember
{
    int     m_iValue;            // Wert für Verzweigungspfad
    CNode*  m_pNode;             // Zeiger auf nächste Node
public:
    CNAMember() { m_iValue = 0; m_pNode = NULL; };
    virtual ~CNAMember() {};
    int     GetValue(void) { return m_iValue; };
    CNode*  GetNode(void) { return m_pNode; };
    void    Set(int iValue, CNode* pNode)
    {   m_iValue = iValue; m_pNode = pNode; };
    void    Set(int iValue)
    {   m_iValue = iValue; };
};

// Klasse für Zuschnitt-Werte
class CProfileAddValues
{
public:
    int     m_iQuerTeil;
    int     m_iSenkTeil;
    int     m_iOberTeil;
    int     m_iUnterTeil;

    CProfileAddValues() { m_iQuerTeil=m_iSenkTeil=m_iOberTeil=m_iUnterTeil=0; };
    virtual ~CProfileAddValues() {};
};

class CNode  
{
    CProperty   m_Property;         // Eigenschaft, die Node beschreibt,z.B.:
                                    // Art, Typ, OKF, usw.

    CNAMember   *m_pChildNodes;     // Zeiger auf Child-Nodes
    DWORD       m_dwValidNodes;     // Anzahl gültige Nodes im Array
    DWORD       m_dwCountNodes;     // Anzahl von Nodes im Array

    CProfileAddValues *m_pAddValues;    // Zeiger auf Werte, falls dies letzte 
                                        // Node ist
    int         m_iNodeValue;         // Zuschnitts-Wert der Ende-Node
    CNode       *m_pParent;           // Zeiger auf Vorgänger
public:
	BOOL IsPropertyPossible(char* strProperty);
    HANDLE      hTreeItem;            // temporärer Speicher für TreeItem
    int         iChildIndex;          // temporärer Speicher für Child-Index

	BOOL    ValueForChildAllowed(char* strValue);
	BOOL    DeleteChildAt(int iNode);
	CNode*  GetEndNode(CTorDoor *pTor, CFlParam* pFluegel);
	BOOL    AddChild(int iValue, CNode **ppNode, int &iPosition);
    BOOL    RemoveChild(CNode* pChild);
	void    SetProperty(eTTEigenschaften Property, char* strName);
    CProperty& GetProperty(void) { return m_Property; };
    DWORD   GetChildCount(void) { return m_dwValidNodes; };
    CNode*  GetChildAt(int iNode) { return  m_pChildNodes[iNode].GetNode(); };
    int     GetValueForChild(int iNode) { return m_pChildNodes[iNode].GetValue(); };
    BOOL    SetValueForChild(int iChild, int iValue);
    CNode*  GetChildByValue(int iValue);
    void    SetParent(CNode* pParent)  { m_pParent = pParent; };
    CNode*  GetParent(void)    { return m_pParent; };
    BOOL    GetFreeValue(int &iValue, CString &strValue);
    void    SetProperty(CProperty *pProp) { m_Property.SetProperty(pProp); };
    void    SetNodeValue(int iValue) { m_iNodeValue = iValue; };
    int     GetNodeValue(void)       { return m_iNodeValue; };
    BOOL    DestroyLeaf(void);
	BOOL    WriteLeaf(ofstream& file, CString strLeaf);
    BOOL    IsEndNode(void)  { return (m_Property.m_Property == NOT_SET); };
    BOOL    IsRootNode(void) { return (m_pParent == NULL); };
    BOOL    ChildWithDefaultExists(void);
	void    QueryAllowedActions(CNode* pNode, BOOL &bNew, BOOL &bChange, BOOL &bDelete);
	CNode();
	virtual ~CNode();

protected:
};

#endif // !defined(AFX_NODE_H__0471C6A1_A44D_11D5_8E7C_8FDE04C7301C__INCLUDED_)

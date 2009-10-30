#pragma once
#include <string>

using namespace std;

//////////////////////////////////////////
// Basis-Klasse zum Abspeichern eines Parameters
class CPS312Param
{
    string m_strParamName;
    bool   m_bReadOnly;
public:
    CPS312Param(void);
    CPS312Param(char* szName, bool bReadOnly = false);
    ~CPS312Param(void);

    const char* GetName(void);
    
    virtual bool  SaveValue(char* szVal, int iLen);
    virtual bool  GetValueString(char* szVal, int iMaxLen, int* piLen) = 0;
    virtual bool  IsValuePossible(char* szVal) = 0;
};

class CPS312ParamInt : public CPS312Param
{
    int m_iValue;
    int m_iMinVal;          // untere Grenze fuer Wert
    int m_iMaxVal;          // obere Grenze für Wert
public:
    CPS312ParamInt(char* szName, int iValue, int iMinVal, int iMaxVal, bool bReadOnly = false);
    ~CPS312ParamInt(void) {};

    virtual bool  SaveValue(char* szVal, int iLen);
    virtual bool  GetValueString(char* szVal, int iMaxLen, int* piLen);
    virtual bool  IsValuePossible(char* szVal);
    int GetValueInt(void) { return m_iValue; };
};

class CPS312ParamFloat : public CPS312Param
{
    float m_fValue;
    float m_fMinVal;        // untere Grenze fuer Wert
    float m_fMaxVal;        // obere Grenze fuer Wert
public:
    CPS312ParamFloat(char* szName, float fValue, float fMinVal, float fMaxVal, bool bReadOnly = false);
    ~CPS312ParamFloat(void) {};

    virtual bool  SaveValue(char* szVal, int iLen);
    virtual bool  GetValueString(char* szVal, int iMaxLen, int* piLen);
    virtual bool  IsValuePossible(char* szVal);
};

class CPS312ParamString : public CPS312Param
{
    string m_strValue;
public:
    CPS312ParamString(char* szName, char* szValue, bool bReadOnly = false);
    ~CPS312ParamString(void) {};

    virtual bool  SaveValue(char* szVal, int iLen);
    virtual bool  GetValueString(char* szVal, int iMaxLen, int* piLen);
    virtual bool  IsValuePossible(char* szVal);
};

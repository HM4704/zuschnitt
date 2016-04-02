#include "StdAfx.h"
#include "PS312Param.h"

CPS312Param::CPS312Param(void)
{
    m_bReadOnly = false;
}

CPS312Param::CPS312Param(char* szName, bool bReadOnly)
{
    m_strParamName.assign(szName);
    m_bReadOnly = bReadOnly;
}

CPS312Param::~CPS312Param(void)
{
}

const char* CPS312Param::GetName(void)
{
    return m_strParamName.c_str();
}

bool CPS312Param::SaveValue(char* szVal, int iLen)
{
    if (m_bReadOnly == true)
        return false;
    return true;
}

CPS312ParamInt::CPS312ParamInt(char* szName, int iValue, int iMinVal, int iMaxVal, bool bReadOnly)
   : CPS312Param(szName, bReadOnly)
{
    m_iValue = iValue;
    m_iMinVal = iMinVal;
    m_iMaxVal = iMaxVal;
}

bool CPS312ParamInt::SaveValue(char* szVal, int iLen)
{
    char szInt[30];
    int  iVal;

    if (CPS312Param::SaveValue(szVal, iLen) == false)
        return false;

    memcpy(szInt, szVal, iLen);
    szInt[iLen] = 0;

    iVal = atoi(szInt);
    if ((iVal < m_iMinVal) || (iVal > m_iMaxVal))
        return false;

    m_iValue = iVal;
    return true;
}

bool CPS312ParamInt::GetValueString(char* szVal, int iMaxLen, int* piLen)
{
    char szBuf[30];

    _itoa_s(m_iValue, szBuf, sizeof(szBuf), 10);
    *piLen = (int)strlen(szBuf);
    if (*piLen > iMaxLen)
        return false;

//    *piLen -= 1;
    if (*piLen < 0)
        *piLen = 0;
    memcpy(szVal, szBuf, *piLen);

    return true;
}

bool CPS312ParamInt::IsValuePossible(char* szVal)
{
    return false;
}

CPS312ParamFloat::CPS312ParamFloat(char* szName, float fValue, float fMinVal, float fMaxVal, bool bReadOnly)
   : CPS312Param(szName, bReadOnly)
{
    m_fValue = fValue;
    m_fMinVal = fMinVal;
    m_fMaxVal = fMaxVal;
}

bool CPS312ParamFloat::SaveValue(char* szVal, int iLen)
{
    char szFloat[30];
    _CRT_FLOAT fltval;

    if (CPS312Param::SaveValue(szVal, iLen) == false)
        return false;

    memcpy(szFloat, szVal, iLen);
    szFloat[iLen] = 0;

    if (_atoflt(&fltval, szFloat) != 0)
        return false;

    if ((fltval.f < m_fMinVal) || (fltval.f > m_fMaxVal))
        return false;

    m_fValue = fltval.f;
    return true;
}

bool CPS312ParamFloat::GetValueString(char* szVal, int iMaxLen, int* piLen)
{
    char szBuf[30];

    if (_gcvt_s(szBuf, sizeof(szBuf), m_fValue, 10) != 0)
        return false;

    *piLen = (int)strlen(szBuf);
    if (*piLen > iMaxLen)
        return false;

//    *piLen -= 1;
    if (*piLen < 0)
        *piLen = 0;
    memcpy(szVal, szBuf, *piLen);

    return true;
}

bool CPS312ParamFloat::IsValuePossible(char* szVal)
{
    return false;
}

CPS312ParamString::CPS312ParamString(char* szName, char* szValue, bool bReadOnly)
   : CPS312Param(szName, bReadOnly)
{
    m_strValue.assign(szValue);
}

bool CPS312ParamString::SaveValue(char* szVal, int iLen)
{
    if (CPS312Param::SaveValue(szVal, iLen) == false)
        return false;

    return false;
}

bool CPS312ParamString::GetValueString(char* szVal, int iMaxLen, int* piLen)
{
    return false;
}

bool CPS312ParamString::IsValuePossible(char* szVal)
{
    return false;
}


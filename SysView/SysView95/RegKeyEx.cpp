// RegKeyEx.cpp: implementation of the CRegKeyEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegKeyEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegKeyEx::CRegKeyEx() : CRegKey()
{
}


LONG CRegKeyEx::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
{
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
//	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	return lRes;
}

LONG CRegKeyEx::GetRegistryValue(HKEY hKey, char *strRegPath, char *strValue, DWORD *pdwValue)
{
    CRegKeyEx RegKey;
    LONG    lRet;


    if ((lRet= RegKey.Open(hKey, strRegPath)) != ERROR_SUCCESS)
    {
//        MessageBox("OpenKey() failed!");
        return lRet;
    }

    if ((lRet = RegKey.QueryValue(*pdwValue, strValue)) != ERROR_SUCCESS)
    {
//        MessageBox("QueryValue() failed!");
        return lRet;
    }

    if ((lRet = RegKey.Close()) != ERROR_SUCCESS)
    {
//        MessageBox("OpenKey() failed!");
        return lRet;
    }

    return 0;
}


// Function name	: CRegKeyEx::GetRegistryValue
// Description	    : öffnet Registry, holt Namen und schließt die 
//                    Registry wieder
// Return type		: LONG : 0 = kein Fehler
// Argument         : HKEY hKey    globaler Key (HKEY_LOCAL_MACHINE)
// Argument         : char *strRegPath  Name des Pfads
// Argument         : char *strValueName    Name des Keys
// Argument         : char *strValue        Ergebnis der Abfrage
//                  : DWORD *pdwCount       Länge des Strings
LONG CRegKeyEx::GetRegistryValue(HKEY hKey, char *strRegPath, char *strValueName, char *strValue
                                 , DWORD *pdwCount)
{
    CRegKey RegKey;
    LONG    lRet;


    if ((lRet= RegKey.Open(hKey, strRegPath)) != ERROR_SUCCESS)
    {
//        MessageBox("OpenKey() failed!");
        return lRet;
    }

    if ((lRet = RegKey.QueryValue(strValue, strValueName, pdwCount)) != ERROR_SUCCESS)
    {
//        MessageBox("QueryValue() failed!");
        return lRet;
    }

    if ((lRet = RegKey.Close()) != ERROR_SUCCESS)
    {
//        MessageBox("OpenKey() failed!");
        return lRet;
    }

    return 0;
}

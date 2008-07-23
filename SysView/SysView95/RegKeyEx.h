// RegKeyEx.h: interface for the CRegKeyEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGKEYEX_H__B6447F53_C568_11D4_8E7B_AF5D82745037__INCLUDED_)
#define AFX_REGKEYEX_H__B6447F53_C568_11D4_8E7B_AF5D82745037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NO_REGMAN_DLL
#define GLOBAL
#else
#ifdef MY_EXPORT
#define GLOBAL   __declspec( dllexport )
#else
#define GLOBAL   __declspec( dllimport )
#endif
#endif


class GLOBAL CRegKeyEx : public CRegKey
{
public:
	CRegKeyEx();

// Attributes
public:

// Operations
public:
	LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName);
    LONG GetRegistryValue(HKEY hKey, char *strRegPath, char *strValue, DWORD *pdwValue);
    LONG GetRegistryValue(HKEY hKey, char *strRegPath, char *strValueName, char *strValue
                                 , DWORD *pdwCount);
};

#endif // !defined(AFX_REGKEYEX_H__B6447F53_C568_11D4_8E7B_AF5D82745037__INCLUDED_)

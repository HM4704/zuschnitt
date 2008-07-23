// ActiveXUtils.h: interface for the CActiveXUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVEXUTILS_H__AAC39A65_DB1C_11D4_8E7B_EFD2835E9237__INCLUDED_)
#define AFX_ACTIVEXUTILS_H__AAC39A65_DB1C_11D4_8E7B_EFD2835E9237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CActiveXUtils  
{
public:
	BOOL CallDllEntry(char* pszDllName, char* szDllEntry);
	BOOL UnRegisterControl(char* pszName);
	BOOL RegisterControl(char* pszName);
	CActiveXUtils();
	virtual ~CActiveXUtils();

};

#endif // !defined(AFX_ACTIVEXUTILS_H__AAC39A65_DB1C_11D4_8E7B_EFD2835E9237__INCLUDED_)

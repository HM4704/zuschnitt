#if !defined(AFX_REGISTRYMANAGER_H__90337901_1707_11D5_8E7C_CC9F50371E31__INCLUDED_)
#define AFX_REGISTRYMANAGER_H__90337901_1707_11D5_8E7C_CC9F50371E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegistryManager.h : header file
//

#pragma warning( push )
#pragma warning(disable: 4275)  // deriving exported class from non-exported
#pragma warning(disable: 4251)  // using non-exported as public in exported

#include "RegKeyEx.h"
#include "RegistryItem.h"

/////////////////////////////////////////////////////////////////////////////
// Defines
#define REG_KEY_LOCALMACHINE           HKEY_LOCAL_MACHINE
#define REG_KEY_CURRENTUSER            HKEY_CURRENT_USER
#define REG_KEY_SOFTWARE               "Software"
#define REG_KEY_RUN                    "Microsoft\\Windows\\CurrentVersion\\Run"


/////////////////////////////////////////////////////////////////////////////
// Macros
#define     RM_SUCCESS(lRet)    (lRet == ERROR_SUCCESS)
                                


//class CRegistryManager;
/////////////////////////////////////////////////////////////////////////////
// class CRegistryItem


/////////////////////////////////////////////////////////////////////////////
// class CRegistryManager
#ifdef NO_REGMAN_DLL
#define GLOBAL
#else
#ifdef MY_EXPORT
#define GLOBAL   __declspec( dllexport )
#else
#define GLOBAL   __declspec( dllimport )
#endif
#endif

class GLOBAL CRegistryManager : public CObject
{

// Attributes
public:

private:
#if 0
    CString     m_strApplication;       // Name der Application
#else
    char        m_strApplication[100];       // Name der Application
#endif
    CRegKeyEx   m_keyApplication;       // Key auf Application
    DWORD       m_dwMaxKeys;            // Anzahl maximal offener Keys
    PCRegistryItem   *m_ppRegItems;

// Operations
public:
	BOOL DeleteRunItem(CString strItemName);
	BOOL RegisterRunItem(CString strItemName, CString strCmdLine);
	HKEY GetHKey(void);
	CRegistryManager();           
	virtual ~CRegistryManager();

	BOOL Close(void);
	BOOL Open(CString& strApplication, DWORD dwMaxKeys, BOOL bCreate);
    BOOL InsertItem(PCRegistryItem ppItem);
    BOOL RemoveItem(PCRegistryItem ppItem);
    HKEY GetKey(void) { return m_keyApplication.m_hKey; }

// Implementation
protected:

	inline CString MakePath(CString strKey);

};

#pragma warning( pop ) 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTRYMANAGER_H__90337901_1707_11D5_8E7C_CC9F50371E31__INCLUDED_)

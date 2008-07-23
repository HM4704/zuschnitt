// RegistryManager.cpp : implementation file
//

#include "stdafx.h"
//#include "sysview95.h"
#include "RegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define SAFE_DELETE(PDATA)      if (PDATA != NULL)      \
                                {                       \
                                    delete PDATA;       \
                                    PDATA = NULL;       \
                                }  

#define SAFE_DELETE_ARR(PDATA)  if (PDATA != NULL)      \
                                {                       \
                                    delete [] PDATA;       \
                                    PDATA = NULL;       \
                                }  

/////////////////////////////////////////////////////////////////////////////
// CRegistryManager


CRegistryManager::CRegistryManager()
{
    m_ppRegItems = NULL;
}

CRegistryManager::~CRegistryManager()
{
    SAFE_DELETE_ARR(m_ppRegItems)
}


// Function name	: CRegistryManager::Open
// Description	    : verbindet sich mit Registry-Eintrag
// Return type		: BOOL 
// Argument         : CString strApplication    Name der Application == Registry-Eintrag
// Argument         : DWORD dwMaxKeys           maximale Anzahl von Keys
// Argument         : BOOL bCreate              TRUE == Eintrag erzeugen, falls nicht vorhanden
BOOL CRegistryManager::Open(CString& strApplication, DWORD dwMaxKeys, BOOL bCreate)
{
#if 0
    m_strApplication = strApplication;
#else
    strcpy(m_strApplication, strApplication.GetBuffer(strApplication.GetLength()));
    strApplication.ReleaseBuffer();
#endif
    m_dwMaxKeys = dwMaxKeys;

    m_ppRegItems = new PCRegistryItem[m_dwMaxKeys];
    if (m_ppRegItems == NULL)
        return FALSE;

    for (DWORD i = 0; i < m_dwMaxKeys; i++)
        m_ppRegItems[i] = NULL;

    CString strTemp;
    strTemp.Empty();
    strTemp = MakePath(strTemp);


    if (m_keyApplication.Open(REG_KEY_LOCALMACHINE, strTemp) != ERROR_SUCCESS)
    {
        if (bCreate == FALSE)
            return FALSE;

        // erzeuge Eintrag
        if (m_keyApplication.Create(REG_KEY_LOCALMACHINE, strTemp) != ERROR_SUCCESS)
            return FALSE;

    }

    return TRUE;
}



// Function name	: CRegistryManager::Close
// Description	    : 
// Return type		: BOOL 
BOOL CRegistryManager::Close()
{
    BOOL bRet = FALSE;

    if (m_keyApplication.m_hKey != NULL)
    {
        bRet = RM_SUCCESS(m_keyApplication.Close());
    }

    return bRet;
}



// Function name	: CRegistryManager::InsertItem
// Description	    : 
// Return type		: BOOL 
// Argument         : PCRegistryItem ppItem
BOOL CRegistryManager::InsertItem(PCRegistryItem ppItem)
{
    for (DWORD i = 0; i < m_dwMaxKeys; i++)
    {
        if (m_ppRegItems[i] == NULL)
        {
            m_ppRegItems[i] = ppItem;
            return TRUE;
        }
    }

    return FALSE;
}


// Function name	: CRegistryManager::RemoveItem
// Description	    : 
// Return type		: BOOL 
// Argument         : PCRegistryItem ppItem
BOOL CRegistryManager::RemoveItem(PCRegistryItem ppItem)
{
    for (DWORD i = 0; i < m_dwMaxKeys; i++)
    {
        if (m_ppRegItems[i] == ppItem)
        {
            m_ppRegItems[i] = NULL;
            return TRUE;
        }
    }

    return FALSE;
}

// Function name	: CRegistryManager::MakePath
// Description	    : 
// Return type		: inline CString 
// Argument         : CString strKey
inline CString CRegistryManager::MakePath(CString strKey)
{
    CString str = REG_KEY_SOFTWARE;
    str += "\\";
    str += m_strApplication;
    if (!strKey.IsEmpty())
    {
        str += "\\";
        str += strKey;
    }

    return str;
}

HKEY CRegistryManager::GetHKey()
{
    return m_keyApplication.m_hKey;
}

BOOL CRegistryManager::RegisterRunItem(CString strItemName, CString strCmdLine)
{
    CRegKeyEx keyRun;
    CString strKeyName = REG_KEY_SOFTWARE;
    strKeyName += "\\";
    strKeyName += REG_KEY_RUN;

    if (keyRun.Open(REG_KEY_CURRENTUSER, strKeyName) != ERROR_SUCCESS)
        return FALSE;

    if (keyRun.SetValue(strCmdLine, strItemName) != ERROR_SUCCESS)
        return FALSE;

    if (keyRun.Close() != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

BOOL CRegistryManager::DeleteRunItem(CString strItemName)
{
    CRegKeyEx keyRun;
    CString strKeyName = REG_KEY_SOFTWARE;
    strKeyName += "\\";
    strKeyName += REG_KEY_RUN;

    if (keyRun.Open(REG_KEY_CURRENTUSER, strKeyName) != ERROR_SUCCESS)
        return FALSE;

    if (keyRun.DeleteValue(strItemName) != ERROR_SUCCESS)
        return FALSE;

    if (keyRun.Close() != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

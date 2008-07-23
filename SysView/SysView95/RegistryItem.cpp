#include <stdafx.h>


#include "RegistryItem.h"
#include "RegistryManager.h"



CRegistryItem::CRegistryItem() 
{
    m_pManager = NULL;
}

// Function name	: CRegistryItemInt::Connect
// Description	    : 
// Return type		: BOOL 
// Argument         : CRegistryManager *pManager
// Argument         : CString strRegName
BOOL CRegistryItem::Connect(CRegistryManager *pManager, CString strSubKeyName, 
                               CString strValueName, BOOL &bCreated)
{
    m_pManager = pManager;
    if (m_pManager == NULL)
        return FALSE;

    m_strSubKeyName = strSubKeyName;
    m_strValueName = strValueName;

    bCreated = FALSE;
    if (m_strSubKeyName.IsEmpty())
    {
        Attach(pManager->GetHKey());
        if (Update(FALSE) == FALSE)
        {
            bCreated = TRUE;
        }
    }
    else
    {
        if (OpenKey() == FALSE)
        {
            if (CreateKey() == FALSE)
                return FALSE;
            else
            {
                bCreated = TRUE;
            }
        }
        else
        {
            if (Update(FALSE) == FALSE)
                bCreated = TRUE;
        }
    }

    return m_pManager->InsertItem(this);
}



BOOL CRegistryItem::Disconnect(void)
{
    if (m_pManager)
        return m_pManager->RemoveItem(this);
    else
        return FALSE;
}

BOOL CRegistryItem::CloseKey(void)
{
    if (m_pManager)
        return (Close() == ERROR_SUCCESS);
    else
        return FALSE;
}



CRegistryItemDword::~CRegistryItemDword()
{
//    Update(TRUE);  zu langsam????
    Disconnect();
}

BOOL CRegistryItemDword::Update(BOOL bToRegistry) 
{
    if (bToRegistry == TRUE)
        return SaveItem();
    else
        return LoadItem();
}

BOOL CRegistryItemDword::CreateKey(void)
{
    if (m_pManager)
        return (Create(m_pManager->GetKey(), m_strSubKeyName) ==
            ERROR_SUCCESS);
    else
        return FALSE;
}

BOOL CRegistryItemDword::OpenKey(void)
{
    if (m_pManager)
        return (Open(m_pManager->GetKey(), m_strSubKeyName) == ERROR_SUCCESS);
    else
        return FALSE;
}

BOOL CRegistryItemDword::LoadItem(void)
{
    if (m_pManager)
        return (QueryValue(m_dwItem, m_strValueName) == ERROR_SUCCESS);
    else
        return FALSE;
}

BOOL CRegistryItemDword::SaveItem(void)
{
    if (m_pManager)
        return (SetValue(m_dwItem, m_strValueName) == ERROR_SUCCESS);
    else
        return FALSE;
}




CRegistryItemString::~CRegistryItemString()
{
//    Update(TRUE);  zu langsam????
    Disconnect();
}

BOOL CRegistryItemString::Update(BOOL bToRegistry) 
{
    if (bToRegistry == TRUE)
        return SaveItem();
    else
        return LoadItem();
}

BOOL CRegistryItemString::CreateKey(void)
{
    if (m_pManager)
        return (Create(m_pManager->GetKey(), m_strSubKeyName) ==
            ERROR_SUCCESS);
    else
        return FALSE;
}

BOOL CRegistryItemString::OpenKey(void)
{
    if (m_pManager)
        return (Open(m_pManager->GetKey(), m_strSubKeyName) == ERROR_SUCCESS);
    else
        return FALSE;
}

BOOL CRegistryItemString::LoadItem(void)
{
    BOOL bRet;
    DWORD dwLen = MAXLEN_STRING;
    if (m_pManager)
    {
        bRet = (CRegKey::QueryValue(m_strItem.GetBuffer(MAXLEN_STRING), m_strValueName, &dwLen) == ERROR_SUCCESS);
        m_strItem.ReleaseBuffer();
        return bRet;
    }
    else
        return FALSE;
}

BOOL CRegistryItemString::SaveItem(void)
{
    if (m_pManager)
        return (SetValue(m_strItem, m_strValueName) == ERROR_SUCCESS);
    else
        return FALSE;
}



/*
int operator=(int& i, CRegistryItemInt& item) 
{ 
    i = item.GetVal(); 
    return i; 
}
*/
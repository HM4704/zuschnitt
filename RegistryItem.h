#ifndef REGISTRY_ITEM_H
#define REGISTRY_ITEM_H


#include "RegKeyEx.h"
//#include "RegistryManager.h"

#ifdef MY_EXPORT
#define GLOBAL   __declspec( dllexport )
#else
#define GLOBAL   __declspec( dllimport )
#endif

class CRegistryManager;

class GLOBAL CRegistryItem : public CRegKeyEx
{

protected:
#if 0
    CString             m_strSubKeyName;
    CString             m_strValueName;
#else
    char                m_strSubKeyName[255];
    char                m_strValueName[100];
#endif
    CRegistryManager*   m_pManager;

protected:
    virtual BOOL    CreateKey(void) { return TRUE; }
    virtual BOOL    OpenKey(void)   { return TRUE; }
    virtual BOOL    LoadItem(void)   { return TRUE; }
    virtual BOOL    SaveItem(void)   { return TRUE; }
    virtual BOOL    CloseKey(void);

public:
    CRegistryItem();
    virtual ~CRegistryItem() { /*m_Manager->Update(this, TRUE);*/ }

    virtual BOOL Connect(CRegistryManager *pManager, 
        CString& strSubKeyName, CString& strValueName, BOOL &bCreated);
    virtual BOOL Disconnect(void);

    virtual BOOL  Update(BOOL bToRegistry = TRUE) { return bToRegistry; }
};

typedef CRegistryItem* PCRegistryItem;

class GLOBAL CRegistryItemDword : public CRegistryItem
{
private:
    DWORD     m_dwItem;

protected:
    virtual BOOL    CreateKey(void);
    virtual BOOL    OpenKey(void);
    virtual BOOL    LoadItem(void);
    virtual BOOL    SaveItem(void);

public:
    virtual ~CRegistryItemDword();
    int  GetVal(void)  { return m_dwItem; }
    virtual BOOL Update(BOOL bToRegistry = TRUE);

    DWORD& operator=(DWORD i) { m_dwItem = i; return m_dwItem; }

    DWORD& operator=(CRegistryItemDword& i) { m_dwItem = i.m_dwItem;
#if 0
                                         m_strSubKeyName = i.m_strSubKeyName;
                                         m_strValueName = i.m_strValueName;
#else
                                         strcpy(m_strSubKeyName, i.m_strSubKeyName);
                                         strcpy(m_strValueName, i.m_strValueName);
#endif
                                         m_pManager = i.m_pManager;
                                         return m_dwItem;
    };

//    friend int operator=(int& i, CRegistryItemInt& item);
};


#define MAXLEN_STRING       1000

class GLOBAL CRegistryItemString : public CRegistryItem
{
private:
#if 1
    CString    m_strItem;
#else
    char       m_strItem[100];
#endif

protected:
    virtual BOOL    CreateKey(void);
    virtual BOOL    OpenKey(void);
    virtual BOOL    LoadItem(void);
    virtual BOOL    SaveItem(void);

public:
    virtual ~CRegistryItemString();
    CString  GetVal(void)  { return m_strItem; }
    virtual BOOL Update(BOOL bToRegistry = TRUE);

    CString& operator=(CString &str) { m_strItem = str; return m_strItem; }
    CString& operator=(char* str) { m_strItem = str; return m_strItem; }
    CString& operator=(CRegistryItemString& str) { m_strItem = str.m_strItem;
#if 0
                                         m_strSubKeyName = str.m_strSubKeyName;
                                         m_strValueName = str.m_strValueName;
#else
                                         strcpy(m_strSubKeyName, str.m_strSubKeyName);
                                         strcpy(m_strValueName, str.m_strValueName);
#endif
                                         m_pManager = str.m_pManager;
                                         return m_strItem;
    };

//    friend int operator=(int& i, CRegistryItemInt& item);
};

#endif
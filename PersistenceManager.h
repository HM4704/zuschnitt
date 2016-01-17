#pragma once

class CPersistenceManager
{
    CStringList m_slistKunde;
    CStringList m_slistKommission;

public:
    CPersistenceManager(void);
    virtual ~CPersistenceManager(void);

    CStringList& GetListKunde(void);
    CStringList& GetListKommission(void);

    void AddKundeToList(CString kunde);
    void AddKommssionToList(CString kommission);
};

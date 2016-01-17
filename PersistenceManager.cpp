#include "stdafx.h"
#include "PersistenceManager.h"

CPersistenceManager::CPersistenceManager(void)
{
}

CPersistenceManager::~CPersistenceManager(void)
{
}

CStringList& CPersistenceManager::GetListKunde(void)
{
    return m_slistKunde;
}

CStringList& CPersistenceManager::GetListKommission(void)
{
    return m_slistKommission;
}

void CPersistenceManager::AddKundeToList(CString kunde)
{
    m_slistKunde.AddHead(kunde);
}

void CPersistenceManager::AddKommssionToList(CString kommission)
{
    m_slistKommission.AddHead(kommission);
}

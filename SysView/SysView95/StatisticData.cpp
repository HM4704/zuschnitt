// StatisticData.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticData

CStatisticData::CStatisticData()
{
}

CStatisticData::~CStatisticData()
{
}

/////////////////////////////////////////////////////////////////////////////
// CStatisticData functions

// Start durch Zugriff auf das Directory
// Zuerst auf Srv zugreifen, dann auf StartStat
void  CStatisticData::StartStatistics(void)
{
    DWORD dwValue;

    m_RegKey.GetRegistryValue(REG_KEY_PERFSTATS, REG_PERFSTATS_STARTSRV,
        REG_PERFSTATS_KERNEL, &dwValue);
    m_RegKey.GetRegistryValue(REG_KEY_PERFSTATS, REG_PERFSTATS_START,
        REG_PERFSTATS_CPUUSAGE, &dwValue);
}

// Stop durch Zugriff auf das Directory
// Zuerst auf Srv zugreifen, dann auf StopStat
void  CStatisticData::StopStatistics(void)
{
    DisableDataCollection(_T("KERNEL"), _T("CPUUsage"));   
}

BOOL  CStatisticData::GetCPUUsage(DWORD* pdwValue)
{
    if (m_RegKey.GetRegistryValue(REG_KEY_PERFSTATS, REG_PERFSTATS_DATA,
        REG_PERFSTATS_CPUUSAGE, pdwValue) == ERROR_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL  CStatisticData::GetPgCommit(DWORD* pdwValue)
{
    if (m_RegKey.GetRegistryValue(REG_KEY_PERFSTATS, REG_PERFSTATS_DATA,
        REG_PERFSTATS_CPGCOMMIT, pdwValue) == ERROR_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CStatisticData::GetMemoryUsage(DWORD *pdwValue)
{
    MEMORYSTATUS memStat;
    GlobalMemoryStatus(&memStat);

    *pdwValue = memStat.dwMemoryLoad;

    return TRUE;
}

BOOL CStatisticData::DisableDataCollection(char *szObjName, char *szCounterName)   
{
    HKEY hOpen;    
    LPBYTE pByte;    
    DWORD cbData;    
    DWORD dwType;    
    DWORD rc;
    char *szCounterData;    
    BOOL bSuccess = TRUE;

    if ( (rc = RegOpenKeyEx(HKEY_DYN_DATA,"PerfStats\\StopStat", 0,
                           KEY_READ, &hOpen)) == ERROR_SUCCESS)    
    {
      // concatenate the object and couter key names
      szCounterData = (char*)LocalAlloc(LPTR,
            lstrlen(szObjName) + lstrlen(szCounterName) + 2);
      wsprintf(szCounterData, "%s\\%s", szObjName, szCounterName);

      // query to get data size
      if ( (rc = RegQueryValueEx(hOpen,szCounterData,NULL,&dwType,
            NULL, &cbData )) == ERROR_SUCCESS )      
      {
         pByte = (BYTE*)LocalAlloc(LPTR, cbData);
         // query the performance start key to initialize performance data
         // query the start key to initialize performance data
         rc = RegQueryValueEx(hOpen,szCounterData,NULL,&dwType, pByte,
                              &cbData );
         // at this point we don't do anything with the data
         // free up resources         
         LocalFree(pByte);      
      }      
      else
         bSuccess = FALSE;      
      RegCloseKey(hOpen);
      LocalFree(szCounterData);    
    }    
    else      
        bSuccess = FALSE;
    return bSuccess;   
}

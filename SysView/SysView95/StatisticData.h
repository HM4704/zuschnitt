#if !defined(AFX_STATISTICDATA_H__FE3945E1_CAD8_11D4_8E7B_D979D2C3D239__INCLUDED_)
#define AFX_STATISTICDATA_H__FE3945E1_CAD8_11D4_8E7B_D979D2C3D239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticData.h : header file
//

#include "RegKeyEx.h"

// Registry-Defines
#define REG_KEY_PERFSTATS           HKEY_DYN_DATA
#define REG_PERFSTATS_KERNEL        "KERNEL"
#define REG_PERFSTATS_DATA          "PerfStats\\StatData"
#define REG_PERFSTATS_STARTSRV      "PerfStats\\StartSrv"
#define REG_PERFSTATS_START         "PerfStats\\StartStat"
#define REG_PERFSTATS_STOPSRV       "PerfStats\\StopSrv"
#define REG_PERFSTATS_STOP          "PerfStats\\StopStat"
#define REG_PERFSTATS_CPUUSAGE      "KERNEL\\CPUUsage"
#define REG_PERFSTATS_CPGCOMMIT     "VMM\\cpgCommit"


/////////////////////////////////////////////////////////////////////////////
// CStatisticData window

class CStatisticData : public CObject
{
// Construction
public:
	CStatisticData();

// Attributes
public:

// Operations
public:
    void  StartStatistics(void);
    void  StopStatistics(void);
    BOOL  GetCPUUsage(DWORD* pdwValue);
    BOOL  GetPgCommit(DWORD* pdwValue);
    DisableDataCollection(char *szObjName, char *szCounterName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticData)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetMemoryUsage(DWORD* pdwValue);
	virtual ~CStatisticData();

	// Generated message map functions
protected:
    CRegKeyEx   m_RegKey;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICDATA_H__FE3945E1_CAD8_11D4_8E7B_D979D2C3D239__INCLUDED_)

#if !defined(AFX_PERFORMANCEPG_H__947751A1_DB19_11D4_8E7B_943C12868137__INCLUDED_)
#define AFX_PERFORMANCEPG_H__947751A1_DB19_11D4_8E7B_943C12868137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PerformancePg.h : header file
//

#include "StatisticData.h"

#pragma warning( push )
#pragma warning(disable: 4100)
#include "chart.h"
#pragma warning( pop )

#include "SysBasePage.h"
#include "TrayIcon.h"

///////////////////////////////////////////
// defines
#define     TIMERCPUUSAGE     0x03

#define     UPDATE_HIGH       250       // Updategeschwindigkeit in ms
#define     UPDATE_MID        1000       // Updategeschwindigkeit in ms
#define     UPDATE_LOW        3000       // Updategeschwindigkeit in ms
#define     UPDATE_MANUAL        0          


#define     WM_PERF_NOTIFYICON		(WM_USER+100)

#define     NUM_ICONS          10

/////////////////////////////////////////////////////////////////////////////
// CPerformancePg dialog

class CPerformancePg : public CSysBasePage
{
	DECLARE_DYNCREATE(CPerformancePg)

// Construction
public:
	CPerformancePg();
	~CPerformancePg();

// Dialog Data
	//{{AFX_DATA(CPerformancePg)
	enum { IDD = IDD_PERFORMANCE_PG };
	CEdit	m_ctrlMemUsage;
	CButton	m_ctrlStMemUsage;
	CButton	m_ctrlStCpuUsage;
	CStatic	m_ctrlFrMemUsage;
	CStatic	m_ctrlFrCpuUsage;
	CEdit	m_ctrlCpuUsage;
	UINT	m_dwCpuUsage;
	UINT	m_dwMemUsage;
	//}}AFX_DATA

private:
	inline HICON GetPerformanceIcon(DWORD dwValue);
	void UnloadIcons(void);
	void LoadIcons(void);
	BOOL SetUpdateRate(DWORD dwUpdateTime);
	BOOL PlaceChartCtrl(CChart& chart, UINT IdFrame);

    CChart          m_ChartCpuUsage;
    CChart          m_ChartMemUsage;
    CStatisticData  m_StatData;
    DWORD           m_dwTimerCPUUsage;
    CRegistryItemDword  m_ridUpdateTime;         // Update-Zeit
    BOOL            m_bTimerStarted;
    CRect           m_rectClient;
    BOOLEAN         m_bStop;                // zum Debuggen: Anhalten des Updates
    CTrayIcon       m_trayCpuUsage;
    HINSTANCE       m_hInstance;
    HICON           m_hArTrayIcons[NUM_ICONS];

    // for status bar view
    CStatusBar     *m_pStatusBar;
    int             m_iIndexCpuUsage;
    int             m_iIndexMemUsage;
    char            m_strTextCpuUsage[MAX_PATH];
    char            m_strTextMemUsage[MAX_PATH];
    int             m_iStrLenCpuUsage;
    int             m_iStrLenMemUsage;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPerformancePg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateStatusBar(void);
	BOOL RegisterStatusBarInfo(int iStrIdCpuUsage, int iStrIdMmUsage);
    virtual BOOL PlaceItems(CRect &rect);

	// Generated message map functions
	//{{AFX_MSG(CPerformancePg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateHigh();
	afx_msg void OnUpdateUpdateHigh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLow();
	afx_msg void OnUpdateUpdateLow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMid();
	afx_msg void OnUpdateUpdateMid(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateManual();
	afx_msg void OnUpdateUpdateManual(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERFORMANCEPG_H__947751A1_DB19_11D4_8E7B_943C12868137__INCLUDED_)

#if !defined(AFX_CHARTCTL_H__7186233C_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED_)
#define AFX_CHARTCTL_H__7186233C_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ChartCtl.h : Declaration of the CChartCtrl ActiveX Control class.

#include "..\SysView95\RegistryManager.h"


#define  FIELD_SIZE    2000
#define  COLUMN_WIDTH  20

typedef struct tagChartMinMax
{  
    int iMin;
    int iMax;
} ChartMinMax;

/////////////////////////////////////////////////////////////////////////////
// CChartCtrl : See ChartCtl.cpp for implementation.

class CChartCtrl : public COleControl
{
	DECLARE_DYNCREATE(CChartCtrl)

// Constructor
public:
	CChartCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnBackColorChanged();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Redraw(CDC *pMemDC, CDC *pBgDC);
	void Restart(CDC* pMemDC, CDC* pBgDC, BOOLEAN bDraw);
	BOOL ControlColorChanged(void);
	void CreateBackground(CDC* pBgDC);
	~CChartCtrl();

	DECLARE_OLECREATE_EX(CChartCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CChartCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CChartCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CChartCtrl)		// Type name and misc status

    void Start(CDC* pMemDC, CDC* pBgDC);
    CreateChart(COLORREF rgbLine, COLORREF rgbGrid);
    void Reset(void);
    int  MapRange(DWORD val);
    BOOL DrawChart(CDC* pMemDC, CDC* pBgDC);
    BOOL DrawFromTo(CDC* pMemDC, CDC* pBgDC, 
        DWORD dwFrom, DWORD dwTo);
	BOOL SetupRegistryValues(void);

// Message maps
	//{{AFX_MSG(CChartCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnOptions();
	afx_msg void OnUpdateOptions(CCmdUI* pCmdUI);
	afx_msg void OnReset();
	afx_msg void OnUpdateReset(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CChartCtrl)
	OLE_COLOR m_gridColor;
	afx_msg void OnGridColorChanged();
	OLE_COLOR m_lineColor;
	afx_msg void OnLineColorChanged();
	long m_refreshTime;
	afx_msg void OnRefreshTimeChanged();
	OLE_COLOR m_bgColor;
	afx_msg void OnBgColorChanged();
	afx_msg void MinMax(long ulMin, long ulMax);
	afx_msg void Clear();
	afx_msg BOOL Update(long dwValue);
	afx_msg void SetControlInfo(LPCTSTR strControlName, LPUNKNOWN pManager);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CChartCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

private:
    DWORD   m_dwX;              // aktuelle Position in X-Richtung
    DWORD   m_dwValid;          // gültiger Bereich
    CRect   m_rectChart;        // Position und Größe des Client-Rahmens
    BOOL    m_bRun;             // Flag ob Aufzeichnung läuft
    CBrush  m_brBackground;     // Farbe des Hintergrunds
    CPen    m_penLine;          // Farbe der Linie
    ChartMinMax m_MinMax;       // abzudeckender Bereich
    DWORD   m_dwDelta;          // Delta zwischen Min und Max
    DWORD  *m_pmtField;         // abgespeicherte Werte

    CBitmap *m_pBitmap;         // Bitmap zum Reinzeichnen
    int   m_dwPosDraw;        // Position bis wohin gezeichnet

    static UINT m_statWndID;           // Window-ID
    CRITICAL_SECTION    ChartCritSect;

    CBitmap *m_pOldBitmap;
    CPen    *m_pOldPen;

    CBitmap *m_pBgBitmap;       // Bitmap für Hintergrund
    CPen    m_penBgLine;        // Stift für Hintergrund-Raster
    CBrush  *m_pOldBgBrush;
    CBitmap *m_pOldBgBmp;
    CPen    *m_pOldBgPen;

    CSize   m_sizeControl;
    DWORD   m_dwXSpace;
    DWORD   m_dwYSpace;    
    DWORD   m_dwPosGrid;        // Position im Grid des Backgrounds


    CMenu   m_Menu;             // Menumember

    CRegistryManager *m_pManager;
    CString m_strName;
    CRegistryItemDword  m_riBgColor;
    CRegistryItemDword  m_riGridColor;
    CRegistryItemDword  m_riLineColor;

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CChartCtrl)
	dispidLineColor = 2L,
	dispidGridColor = 1L,
	dispidRefreshTime = 3L,
	dispidBgColor = 4L,
	dispidMinMax = 5L,
	dispidClear = 6L,
	dispidUpdate = 7L,
	dispidSetControlInfo = 8L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTCTL_H__7186233C_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED)

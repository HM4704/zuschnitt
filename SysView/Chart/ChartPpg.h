#if !defined(AFX_CHARTPPG_H__7186233E_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED_)
#define AFX_CHARTPPG_H__7186233E_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ChartPpg.h : Declaration of the CChartPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CChartPropPage : See ChartPpg.cpp.cpp for implementation.

class CChartPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CChartPropPage)
	DECLARE_OLECREATE_EX(CChartPropPage)

// Constructor
public:
	CChartPropPage();

// Dialog Data
	//{{AFX_DATA(CChartPropPage)
	enum { IDD = IDD_PROPPAGE_CHART };
	long	m_refreshTime;
	UINT	m_gridColor;
	UINT	m_lineColor;
	UINT	m_bgColor;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CChartPropPage)
	afx_msg void OnChangegridcolor();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangelinecolor();
	afx_msg void OnChangebgcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DoColorDialog(UINT &dwColor, int nId);
	void FillFrame(CDC* pDC, UINT dwColor, int nId);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTPPG_H__7186233E_CEAC_11D4_8E7B_C3BDF0D38D3C__INCLUDED)

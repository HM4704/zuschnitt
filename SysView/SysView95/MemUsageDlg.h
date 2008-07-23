#if !defined(AFX_MEMUSAGEDLG_H__FF1D4E61_4789_11D5_8E7C_F16EECC60B50__INCLUDED_)
#define AFX_MEMUSAGEDLG_H__FF1D4E61_4789_11D5_8E7C_F16EECC60B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemUsageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMemUsageDlg dialog

class CMemUsageDlg : public CDialog
{
// Construction
public:
	BOOL SetMemUsage(MEM_USAGE* pMemUsage);
	CMemUsageDlg(HANDLE hDevice, char* strProcName, 
        DWORD dwPoolId, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMemUsageDlg)
	enum { IDD = IDD_MEM_USAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemUsageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
    HANDLE  m_hWintop;
    char    m_strProcName[MAX_PATH];
    DWORD   m_dwPoolId;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMemUsageDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMUSAGEDLG_H__FF1D4E61_4789_11D5_8E7C_F16EECC60B50__INCLUDED_)

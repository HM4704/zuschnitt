#if !defined(AFX_SYSBASEPAGE_H__98112744_F230_11D4_8E7B_90DB50037237__INCLUDED_)
#define AFX_SYSBASEPAGE_H__98112744_F230_11D4_8E7B_90DB50037237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysBasePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysBasePage dialog

class CSysBasePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysBasePage)

// Construction
public:
	CSysBasePage();
	CSysBasePage(UINT nIdMenu, UINT nIdDlg);
	~CSysBasePage();

// Dialog Data
	//{{AFX_DATA(CSysBasePage)
//	enum { IDD = _UNKNOWN_RESOURCE_ID_ };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

protected:
    BOOL            m_bDlgInitDone;

private:
    CMenu m_Menu;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSysBasePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL PlaceItems(CRect &rect);
	// Generated message map functions
	//{{AFX_MSG(CSysBasePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSBASEPAGE_H__98112744_F230_11D4_8E7B_90DB50037237__INCLUDED_)

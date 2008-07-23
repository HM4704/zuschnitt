#if !defined(AFX_ZRBASEPAGE_H__67ED68E6_B9BC_11D5_8E7C_BB7956C7DF54__INCLUDED_)
#define AFX_ZRBASEPAGE_H__67ED68E6_B9BC_11D5_8E7C_BB7956C7DF54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRBasePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Structs

// 3 Zustände: normal, hidden, disabled
typedef struct tagSetButtonsStruct
{
    DWORD   dwStateButtonApply;
    DWORD   dwStateButtonSave;
    DWORD   dwStateButtonCancel;
    DWORD   dwStateButtonHelp;
    DWORD   dwStateButtonNew;
} SetButtonsStruct;

/////////////////////////////////////////////////////////////////////////////
// CZRBasePage dialog

class CZRBasePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CZRBasePage)

// Construction
public:
    CZRBasePage() {};
	CZRBasePage(char* strTitle, UINT nId);
	~CZRBasePage();

// Dialog Data
	//{{AFX_DATA(CZRBasePage)
//	enum { IDD = _UNKNOWN_RESOURCE_ID_ };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

    CString     m_strTitle;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CZRBasePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    virtual void GetButtonStates(SetButtonsStruct* pButtons) {};
    CString& GetTitle(void) { return m_strTitle; };

// Implementation
protected:
	LONG SignalPageChanged(void);
	// Generated message map functions
	//{{AFX_MSG(CZRBasePage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void SendButtonStates(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRBASEPAGE_H__67ED68E6_B9BC_11D5_8E7C_BB7956C7DF54__INCLUDED_)

#if !defined(AFX_ZRPROFILEPAGE_H__DDC883A9_B5A3_11D5_8E7C_96319B2B0854__INCLUDED_)
#define AFX_ZRPROFILEPAGE_H__DDC883A9_B5A3_11D5_8E7C_96319B2B0854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRProfilePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZRProfilePage dialog

class CZRProfilePage : public CZRBasePage
{
	DECLARE_DYNCREATE(CZRProfilePage)

// Construction
public:
	void GetButtonStates(SetButtonsStruct* pButtons);
	BOOL SetRootNode(CNode** ppRootNode);
	CZRProfilePage();
    CZRProfilePage(char* strTitle, CNode** ppRootNode);
	~CZRProfilePage();

// Dialog Data
	//{{AFX_DATA(CZRProfilePage)
	enum { IDD = IDD_ZR_PROFILE_DLG };
	CZRTreeCtrl	m_treeZR;
	//}}AFX_DATA

    CNode       **m_ppRootNode;
    CNode       *m_pNodeToShow;              // anzuzeigende Node

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CZRProfilePage)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnQueryCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL ShowNode(CNode* pNode);
	// Generated message map functions
	//{{AFX_MSG(CZRProfilePage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
    afx_msg LRESULT OnTreeCtrlChanged(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnNewTree(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShowNode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRPROFILEPAGE_H__DDC883A9_B5A3_11D5_8E7C_96319B2B0854__INCLUDED_)

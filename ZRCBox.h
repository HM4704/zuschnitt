#if !defined(AFX_ZRCBOX_H__FD4C6525_ADF6_11D5_8E7C_82A42447CB2C__INCLUDED_)
#define AFX_ZRCBOX_H__FD4C6525_ADF6_11D5_8E7C_82A42447CB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRCBox.h : header file
//


#define     WM_ZR_BOX_DESTROY           (WM_USER+9)     // Combo Box schliessen

/////////////////////////////////////////////////////////////////////////////
// CZRCBox window

class CZRCBox : public CComboBox
{
// Construction
public:
	CZRCBox();

// Attributes
public:

private:
    DWORD       m_dwMsgId;          // Msg-Id für End of select
    CNode       *m_pNode;           // Node des Items
    HTREEITEM   m_hItem;            // zugehöriges Item
    BOOL        m_bIsDestroyed;     // wurde DestroyWindow() schon aufgerufen?


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZRCBox)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CRect& rect, CWnd* pParent, DWORD dwMsgId, CNode* pNode, int iMode=0);
	virtual ~CZRCBox();

	// Generated message map functions
protected:
	void AddTextSized(CDC* pDc, char* str, CSize &size);
	//{{AFX_MSG(CZRCBox)
	afx_msg void OnSelendOk();
	afx_msg void OnKillFocus();
    afx_msg LRESULT OnDestroyBox(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRCBOX_H__FD4C6525_ADF6_11D5_8E7C_82A42447CB2C__INCLUDED_)

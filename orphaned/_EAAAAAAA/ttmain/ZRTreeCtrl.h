#if !defined(AFX_ZRTREECTRL_H__E4887385_A9FF_11D5_8E7C_FE0166658E54__INCLUDED_)
#define AFX_ZRTREECTRL_H__E4887385_A9FF_11D5_8E7C_FE0166658E54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZRTreeCtrl.h : header file
//

#define   WM_COMBOBOX_SELECTION     WM_USER+100     // Botschaft mit Combobox-Auswahl
//#define   WM_ZR_TREECTRL_NEW        WM_USER+101     // Menuitem Neu in TreeCtrl
#define   WM_COMBOBOX_SELECTION_PROPERTY    WM_USER+101 // Combobox fuer Eigenschaftswerte
#define   WM_PARENT_CTRL_CHANGED    WM_USER+102     // es hat eine Änderung gegeben

/////////////////////////////////////////////////////////////////////////////
// CZRTreeCtrl window

class CZRTreeEdit;

class CZRTreeCtrl : public CTreeCtrl
{

friend class CZuschnittManager;

// Construction
public:
	CZRTreeCtrl();

// Attributes
private:
	void SetChange(void);
	BOOL IsPropertyValue(HTREEITEM hItem);
    CZuschnittManager   *m_pMan;
    CImageList          *m_pImageList;
    CNode               **m_ppRootNode;
    BOOL                m_bChanged;
    CZRTreeEdit*        m_ptreeEdit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZRTreeCtrl)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ExpandItems(HTREEITEM hItem, UINT nCode);
	BOOL FindAndSelectItem(CNode* pNode, BOOL bCollapseAll = TRUE);
	void ReturnWasPressed(void);
	void NewTree(void);
	BOOL SetRootNode(CNode** ppRootNode);
	BOOL RemoveItemAndChilds(HTREEITEM hItem, BOOL bRemoveSibling);
	void SetZRManager(CZuschnittManager* pMan);
    BOOL HasChanged(void) { return m_bChanged; };
    void ResetChanged(void) { m_bChanged = FALSE; };
	virtual ~CZRTreeCtrl();

	// Generated message map functions
protected:
	void QueryPossibleActions(HTREEITEM hItem, BOOL &bNew, BOOL &bChange, BOOL &bDelete);
	BOOL DeleteLeaf(HTREEITEM hItem);
	BOOL ReadTree(CNode *pRoot);
	void BuildTree(CNode* pParentNode);
    void InsertNode(HTREEITEM hParentItem, CNode* pNode);
    HTREEITEM FindItem(HTREEITEM hItem, CNode* pNode);

	//{{AFX_MSG(CZRTreeCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateZrPropValueChange(CCmdUI* pCmdUI);
	afx_msg void OnZrPropValueChange();
    afx_msg LRESULT OnComboBoxSelection(WPARAM, LPARAM);
    afx_msg LRESULT OnComboBoxSelectionProperty(WPARAM, LPARAM);
	afx_msg void OnZrPropValueNew();
	afx_msg void OnUpdateZrPropValueNew(CCmdUI* pCmdUI);
	afx_msg void OnZrPropValueDelete();
	afx_msg void OnUpdateZrPropValueDelete(CCmdUI* pCmdUI);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZRTREECTRL_H__E4887385_A9FF_11D5_8E7C_FE0166658E54__INCLUDED_)

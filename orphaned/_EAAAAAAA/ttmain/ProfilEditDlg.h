#if !defined(AFX_PROFILEDITDLG_H__0CB7D4FF_3006_41FB_A6AC_4E90EEDD4725__INCLUDED_)
#define AFX_PROFILEDITDLG_H__0CB7D4FF_3006_41FB_A6AC_4E90EEDD4725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfilEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageListEx dialog
class CImageListEx : public CImageList
{
#define IMG_LIST_SIZE  20
    struct ImageList
    {
        BOOL bUsed;
        char szImgName[50];
        int  iImgIdx;
    };

    ImageList m_imgList[IMG_LIST_SIZE];
public:
    void Init(int cx, int cy, UINT nFlags, int nInitial, int nGrow);
    void InsertImage(char* szName, CBitmap* pbmImage, COLORREF crMask);
    int  FindImageWithName(char* szName);
};

/////////////////////////////////////////////////////////////////////////////
// CProfilEditDlg dialog

class CProfilEditDlg : public CDialog
{
    CTorDoor* m_pTor;
    CPtrArray m_aProfile;
    CImageListEx* m_pImgList;
// Construction
public:
	CProfilEditDlg(CWnd* pParent = NULL);   // standard constructor
	CProfilEditDlg(CWnd* pParent, CTorDoor* pTor);
	~CProfilEditDlg(void);   // standard destructor

// Dialog Data
	//{{AFX_DATA(CProfilEditDlg)
	enum { IDD = IDD_PROFIL_EDIT_DLG };
	CListCtrl	m_lvProfilList;
	CButton	m_btProfilEdit;
	CButton	m_btProfilDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfilEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
    void FillList(void);
    void CopyProfiles(CPtrArray* pArrSrc, CPtrArray* pArrDest);
    void DeleteProfiles(CPtrArray* pArrSrc);
    void UpdateButtons(void);
    void InitProfilList(void);
    void InsertProfil(CProfileElem* pProfil);

protected:

	// Generated message map functions
	//{{AFX_MSG(CProfilEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnProfilDel();
	virtual void OnOK();
	afx_msg void OnProfilNew();
	afx_msg void OnProfilEdit();
	afx_msg void OnItemActivateProfilList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkProfilList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickProfilList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEDITDLG_H__0CB7D4FF_3006_41FB_A6AC_4E90EEDD4725__INCLUDED_)

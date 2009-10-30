#pragma once


#include "BHProfile\\WHSProfilZuschnitt.h"
#include "afxcmn.h"
#include "xmlutil\\XmlNodeWrapper.h"
#include "afxwin.h"

// CWhsProfilDlg-Dialogfeld

class CWhsProfilDlg : public CPrintPreviewDialog
{
	DECLARE_DYNAMIC(CWhsProfilDlg)

    CWHSProfilZuschnitt* m_pWHSProfile;
    CRect                m_rcEticet;            // Groesse einer Ettikette
    vector<int>          m_PrintList;           // Liste mit den zu druckenden Profilen
    int                  m_iProfilesPerPage;    // Profile pro Seite fuer Druck
    int                  m_iPageCount;          // Anzahl zu druckender Seiten
    CFont                m_font;                // Font
    CFont                m_fontPageNo;          // Font fuer Seitennummer
    CPen                 m_MarkPen;
    int                  m_iPageLen;
    int                  m_iPageHeight;

public:
	CWhsProfilDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CWhsProfilDlg();

// Dialogfelddaten
	enum { IDD = IDD_WHS_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    void    InitProfilList(void);
    void    FillList(void);
    void    InsertProfil(int iCount, int iLength);
    bool    BuildCutList(CXmlDocumentWrapper& cutList);
    
    void    getFont(CFont* pFont, int height, int width, int orient, int weight);

    void    EnableButtons();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedWhsProfilOpen();
public:
    float m_fEticetLen;
public:
    float m_fEticetHeight;
public:
    int m_iEticetLeft;
public:
    int m_iEticetTop;
    int m_iProfileCount;       // Anzahl Profile die zu drucken sind
public:
    CString m_strListName;
public:
    BOOL m_bRoundUpProfileCount;            // ungerade Anzahl Profile aufrunden
public:
    CListCtrl m_lcWhsProfilList;
public:
    virtual BOOL OnInitDialog();
public:
    int m_iBladeWidth;
public:
    int m_iProfileLen;
public:
    afx_msg void OnEnChangeBladeWidth();
public:
    afx_msg void OnEnChangeProfileLen();
public:
    afx_msg void OnBnClickedRoundUpP();
public:
    afx_msg void OnClickedSaveCutlist();
public:
    afx_msg void OnClickedStartSawDlg();
public:
    BOOL m_bPrintProfMark;
public:
    afx_msg void OnClickedWhsEditCutlistSett();
public:
    CButton m_btPrintPreview;
public:
    CButton m_btPrint;
public:
    CButton m_btSaveCutlist;
public:
    CButton m_btStartSawDlg;
public:
    afx_msg void OnClickedWhsVerify();
public:
    CButton m_btVerify;
public:
    afx_msg void OnBnClickedWhsProfilNew();
public:
    afx_msg void OnBnClickedWhsProfilEdit();
public:
    afx_msg void OnBnClickedWhsProfilDel();
public:
    CButton m_btProfilNew;
public:
    CButton m_btProfilEdit;
public:
    CButton m_btProfilDel;
public:
    afx_msg void OnNMClickWhsProfilList(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnNMDblclkWhsProfilList(NMHDR *pNMHDR, LRESULT *pResult);
};

#pragma once

#include "Trcom.h"
#include "PS312Control.h"
#include "afxwin.h"
#include "afxcmn.h"

// CPS312TestDlg-Dialogfeld

class CPS312TestDlg : public CDialog
{
	DECLARE_DYNAMIC(CPS312TestDlg)

    CPS312Control m_PS312;
    CTrCom        m_Com;
    CString       m_strCom;

    
    CPtrArray* m_pContainer;        // fuer Profilliste

public:
	CPS312TestDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CPS312TestDlg();

// Dialogfelddaten
	enum { IDD = IDD_PS312_TEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

    void ShowError(int iError);
    void ShowError(char* szError);

    void InitProfilList(void);
    void FillList(void);
    void InsertProfil(PS312_DATA_SET* pProfil);
    void UpdateButtons(void);

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
public:
    CString m_strPS312Status;
public:
    afx_msg void OnClickedTestPs312StatusAkt();
public:
    CButton m_btStatusAkt;
public:
    afx_msg void OnClickedTestPs312CmdSend();
public:
    int m_iPs312Cmd;
public:
    int m_iPS312Level;
public:
    int m_iPS312ParamNr;
public:
    int m_iPS312ParamVal;
public:
    afx_msg void OnClickedTestPs312ParamRead();
public:
    afx_msg void OnClickedTestPs312ParamSet();
public:
    CEdit m_edPS312KommTrace;
public:
    afx_msg void OnClickedTestPs312ComSett();
public:
    CListCtrl m_lcProgList;
public:
    afx_msg void OnClickedNewProfil();
public:
    afx_msg void OnClickedChangeProfil();
public:
    afx_msg void OnClickedDelProfil();
public:
    CButton m_btProfilEdit;
public:
    CButton m_btProfilDel;
public:
    afx_msg void OnLvnItemActivateProgList(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnDblclkProgList(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnClickProgList(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnClickedProgRead();
public:
    afx_msg void OnClickedProgSend();
public:
    afx_msg void OnClickedProgStart();
public:
    int m_iProgNr;
};

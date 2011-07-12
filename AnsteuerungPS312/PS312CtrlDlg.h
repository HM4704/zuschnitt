#pragma once

#include "Trcom.h"
#include "PS312Control.h"
#include "LedButton.h"
#include "..\xmlutil\XmlNodeWrapper.h"
#include "..\bhprofile\WHSProfilZuschnitt.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>

using namespace std;


// CPS312CtrlDlg-Dialogfeld

#define PS312_CTRL_DLG_MAX_STATUS_LEDS  11


typedef struct _CTRL_DATA_SET
{
    byte           byStatus;
    int            iDataSetNr;
    PS312_DATA_SET ds;
} CTRL_DATA_SET;

class CPS312CtrlDlg : public CDialog
{

    DECLARE_DYNAMIC(CPS312CtrlDlg)

    CPS312Control m_PS312;
    CTrCom        m_Com;
    CString       m_strCom;

    CXmlDocumentWrapper* m_pCutListDoc;
    int           m_iStatusPS312;
    int           m_iCountProgs;             // Anzahl Programme im Gerät
    int           m_iActDataSetNr;          // nächster zu sendende Datensatz Teil
    int           m_iMaxDataSetNr;
    bool          m_bErrorDetected;         // ein Fehler verhindert das Programmieren
    bool          m_bDelCutList;            // falls Liste selbst allokiert wurde
    int           m_iLastProgUsed;          // zuletzt verwendete Programmnummer
    HANDLE        m_hTraceFile;             // Handle der Trace Datei

    vector<CTRL_DATA_SET> m_CutList;        // fuer Profilliste

public:
	CPS312CtrlDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	CPS312CtrlDlg(CXmlDocumentWrapper* pCutList, CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CPS312CtrlDlg();

// Dialogfelddaten
	enum { IDD = IDD_PS312_CONTROL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

    BOOL LoadList(CXmlDocumentWrapper* pCutListDoc);
    void InitProfilList(void);
    void InsertProfil(vector<CTRL_DATA_SET>::iterator iter);
    void FillList(void);
    void RecalculateDataSets(void);
    void CreateInputList(InputMap& inputMap, vector<CTRL_DATA_SET>& cList);

    void GetDeviceParameters(void);
    void ManagePS312State(void);
    void SetLedsStatus(void);
    void EnableButtons(void);


    void ShowError(char* szError);
    void ShowError(int iError);
    void InitTrace(void);
    void Trace(char* szFormat,...);

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
public:
    CEdit m_edPS312KommTrace;

    CLedButton m_statusLeds[PS312_CTRL_DLG_MAX_STATUS_LEDS];

public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
    afx_msg void OnClickedPs312CtrlProgLoad();
public:
    CListCtrl m_lcProgList;
public:
    CString m_strSawWidth;
public:
    CString m_strProfileLength;
public:
    int m_iDataSetCount;
public:
    int m_iDSetPerProg;
public:
    CComboBox m_cbProgNr;
public:
    afx_msg void OnClickedPs312CtrlProgSend();
public:
    CButton m_btProgSend;
public:
    afx_msg void OnClickedPs312CtrlRetryConnect();
public:
    CButton m_btRetryConnect;
public:
    afx_msg void OnClickedStartTestDlg();
public:
    CButton m_btStartTest;
public:
    afx_msg void OnDestroy();
};

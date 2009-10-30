#pragma once


// CComSetDlg-Dialogfeld

class CComSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CComSetDlg();

// Dialogfelddaten
	enum { IDD = IDD_COM_SETTINGS };

    void GetComSettings(CString& strSettings) { strSettings = m_strComSet; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
    CString m_strComSet;
};

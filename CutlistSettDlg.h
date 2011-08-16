#pragma once


// CCutlistSettDlg-Dialogfeld

class CCutlistSettDlg : public CDialog
{
	DECLARE_DYNAMIC(CCutlistSettDlg)

public:
	CCutlistSettDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CCutlistSettDlg();

// Dialogfelddaten
	enum { IDD = IDD_WHS_EDIT_CUTLIST_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
    int m_iProfileLen;
public:
    int m_iBladeWidth;
public:
    BOOL m_bRoundUp;
};

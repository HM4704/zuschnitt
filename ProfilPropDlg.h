#pragma once


// CProfilPropDlg-Dialogfeld
class CProfilProp
{
public:
    CProfilProp(int iLength, int iCount) { m_iLength = iLength; m_iCount = iCount; };
    int m_iCount;
    int m_iLength;
};

class CProfilPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CProfilPropDlg)

public:
	CProfilPropDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	CProfilPropDlg(CProfilProp& profilProp, CWnd* pParent = NULL);
	virtual ~CProfilPropDlg();

// Dialogfelddaten
	enum { IDD = IDD_BH_PROFIL_EDIT_DLG };
	int		m_iCount;
	int		m_iLength;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
public:
    afx_msg void OnBnClickedOk();
};

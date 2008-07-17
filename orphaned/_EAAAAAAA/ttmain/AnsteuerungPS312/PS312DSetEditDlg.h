#pragma once


class CPS312DSetEditData
{
public:
    CPS312DSetEditData() { m_bMassAbsolut = false; m_iCount = 1; m_iLength = -1000; };
public:
    int   m_iCount;
    int   m_iLength;
    bool  m_bMassAbsolut;
};

// CPS312DSetEditDlg-Dialogfeld

class CPS312DSetEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CPS312DSetEditDlg)

public:
	CPS312DSetEditDlg(CPS312DSetEditData* pData = NULL, CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CPS312DSetEditDlg();

// Dialogfelddaten
	enum { IDD = IDD_PS312_DATASET_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

    CPS312DSetEditData* m_pData;

	DECLARE_MESSAGE_MAP()
private:
    bool m_bMassAbsolut;
private:
    int m_iCount;
private:
    int m_iLength;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
};

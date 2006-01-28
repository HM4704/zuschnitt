#if !defined(AFX_HOLEPROFILEDOC_H__C0605CFD_E3F3_488E_A7A4_58B13C28BEB5__INCLUDED_)
#define AFX_HOLEPROFILEDOC_H__C0605CFD_E3F3_488E_A7A4_58B13C28BEB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoleProfileDoc.h : header file
//


////// 
class CHoleProfile
{
public:
    CHoleProfile() { m_dwCount = 1; m_dwLength = 1000; };

    DWORD   m_dwCount;          // Anzahl des Profils
    DWORD   m_dwLength;         // Laenge des Profils
};


enum eCutAction { caNew /* Neues Profil, verwurf Rest */, caCut /* vorne wegschneiden */, caLength /* Profillaenge */ };

class CHoleProfileCut
{
public:
    eCutAction m_eAction;
    union 
    {
        DWORD  m_dwLength;     // fuer vorne Wegschneiden
        DWORD  m_dwRest;       // fuer Rest Verwurf
    };
};

#define HP_DEFAULT_LENGTH       6003        // Laenge eines neuen Profils
#define HP_DEFAULT_HOLE_OFFS    100         // Abstand vom Anfang zum ersten Loch bei einem neuen Profil
#define HP_HOLE_LENGTH          60          // Laenge eines Lochs
#define HP_HOLE_DIST            133         // Abstand zwischen zwei Loechern

#define CUT_SAW_THICKNESS       6           // Dicke eines Saegeblatts

struct CUT_INFO
{
    DWORD dwUsedProfiles;                   // verwendete Profile
    DWORD dwTotalRest;                      // gesamter Rest
};

class CHoleProfileCompute
{
public:
    BOOL Compute(int iOffs2Hole, int iLength, int& iFrontCut, int& iNewOffs2Hole);
    BOOL GenerateCutList(CPtrArray* pProfileList, CPtrArray* pCutList, CUT_INFO* pInfo);
};


class CHoleProfileCutOptimize
{
    struct HoleProfileOpt
    {
       DWORD         dwLength;
       unsigned char byValid;
    };

    CHoleProfileCompute m_cCut;
public:
    CHoleProfileCutOptimize() { srand( (unsigned)time( NULL ) ); };
    virtual ~CHoleProfileCutOptimize() {};
    BOOL Go(CPtrArray* pProfiles, CPtrArray* pCutList, CUT_INFO* pInfo);
    inline int  GetRandom(int iProfileCount);
private:

};

/////////////////////////////////////////////////////////////////////////////
// CHoleProfileDoc document
struct HP_OPT_SETTINGS
{
    BOOL bDoOpt;
    int  iOptTime;              // Rechenzeit für Optimierung
};


class CHoleProfileDoc : public CDocument
{
    static CHoleProfileCutOptimize m_opt;
    HP_OPT_SETTINGS         m_SetsOpt;
    CUT_INFO                m_Info;
    CUT_INFO                m_InfoOpt;
    static CRegistryItemDword m_riHPOpt;
    static CRegistryItemDword m_riHPOptTime;
    static BOOL             m_bOptSetsInit;

protected:
	CHoleProfileDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHoleProfileDoc)

// Attributes
public:
    CPtrArray m_Profiles;
    CPtrArray m_CutList;            // Zuschnittliste
    CPtrArray m_CutListOpt;         // optimierte Zuschnittliste

// Operations
public:

    BOOL GenerateCutList(CUT_INFO* pInfo);
    void DestroyCutList(void);
    void EmptyProfileList(void);
    BOOL EditProfiles(void);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoleProfileDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHoleProfileDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHoleProfileDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLEPROFILEDOC_H__C0605CFD_E3F3_488E_A7A4_58B13C28BEB5__INCLUDED_)

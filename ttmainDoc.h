// ttmainDoc.h : interface of the CTtmainDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTMAINDOC_H__141C612D_1243_11D4_8E7B_F82D876B953C__INCLUDED_)
#define AFX_TTMAINDOC_H__141C612D_1243_11D4_8E7B_F82D876B953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TT_DOC_TYPE		0xfe6a
#define TT_DOC_VERSION	1

class CTtmainDoc : public CDocument
{
protected: // create from serialization only
	CTtmainDoc();
	DECLARE_DYNCREATE(CTtmainDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTtmainDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTtmainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTtmainDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTMAINDOC_H__141C612D_1243_11D4_8E7B_F82D876B953C__INCLUDED_)

// HtmlHelper.h: interface for the CHtmlHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLHELPER_H__1CBD9767_F521_11D4_8E7C_B61CE55A2136__INCLUDED_)
#define AFX_HTMLHELPER_H__1CBD9767_F521_11D4_8E7C_B61CE55A2136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum PLACING { PLACE_NOMATTER, PLACE_POS, PLACE_ALIGN };

class CHtmlHelper  
{
private:
    char    cFileDirectory[MAX_PATH];

public:
	char* GetFileDirectory(void);
	void SetFileDirectory(const char* strNewDir);
	CHtmlHelper();
    CHtmlHelper(const char* strFileDirectory);

	BOOL OpenDoc(const char* strHmtlFile, PLACING place, RECT* pRect = NULL);
	virtual ~CHtmlHelper();

protected:
	BOOL PositionWindow(RECT* pRect, const char* strWndNamePart, HANDLE hWndProcess);
};

#endif // !defined(AFX_HTMLHELPER_H__1CBD9767_F521_11D4_8E7C_B61CE55A2136__INCLUDED_)

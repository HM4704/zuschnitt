// ActiveXUtils.cpp: implementation of the CActiveXUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActiveXUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const TCHAR _szAppName[] = _T("RegSvr32");
const char _szDllRegSvr[] = "DllRegisterServer";
const char _szDllUnregSvr[] = "DllUnregisterServer";
HINSTANCE _hInstance;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActiveXUtils::CActiveXUtils()
{

}

CActiveXUtils::~CActiveXUtils()
{

}

BOOL CActiveXUtils::RegisterControl(char *pszDllName)
{
    return CallDllEntry(pszDllName, (char *)_szDllRegSvr);
}

BOOL CActiveXUtils::UnRegisterControl(char *pszDllName)
{
    return CallDllEntry(pszDllName, (char *)_szDllUnregSvr);
}

BOOL CActiveXUtils::CallDllEntry(char *pszDllName, char *szDllEntry)
{
    BOOL bReturn = TRUE;

	HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);

	LPCSTR pszDllEntryPoint = szDllEntry;

	// Initialize OLE.
	if (FAILED(OleInitialize(NULL))) {
		return FALSE;
	}

	SetErrorMode(SEM_FAILCRITICALERRORS);       // Make sure LoadLib fails.
	// Load the library.
	HINSTANCE hLib = LoadLibraryEx(pszDllName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (hLib < (HINSTANCE)HINSTANCE_ERROR) {
		bReturn = FALSE;
		goto CleanupOle;
	}

	// Find the entry point.
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, pszDllEntryPoint);

	if (lpDllEntryPoint == NULL) {
		TCHAR szExt[_MAX_EXT];
		_tsplitpath(pszDllName, NULL, NULL, NULL, szExt);

#if 0
        if ((_stricmp(szExt, ".dll") != 0) && (_stricmp(szExt, ".ocx") != 0))
			DisplayMessage(IDS_NOTDLLOROCX, pszDllName, pszDllEntryPoint);
		else
			DisplayMessage(IDS_NOENTRYPOINT, pszDllName, pszDllEntryPoint);
#endif

		bReturn = FALSE;
		goto CleanupLibrary;
	}

	// Call the entry point.
	if (FAILED((*lpDllEntryPoint)())) {
		bReturn = FALSE;
	}

CleanupLibrary:
    FreeLibrary(hLib);

CleanupOle:
	OleUninitialize();

    return bReturn;
}

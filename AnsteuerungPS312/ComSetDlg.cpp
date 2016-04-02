// ComSetDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "..\ttdata.h"
#include "..\tordoor.h"
#include "..\Node.h"
#include "..\ZuschnittManager.h"
#include "..\ttmain.h"
#include "ComSetDlg.h"


// CComSetDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CComSetDlg, CDialog)

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetDlg::IDD, pParent)
    , m_strComSet(_T("baud=9600 parity=N data=8 stop=1"))
{
// Einstellungen:
// COMx[:][baud=b][parity=p][data=d][stop=s][to={on|off}][xon={on|off}]
//[odsr={on|off}][octs={on|off}][dtr={on|off|hs}][rts={on|off|hs|tg}][idsr={on|off}]
}

CComSetDlg::~CComSetDlg()
{
}

void CComSetDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_COM_SET_STRING, m_strComSet);
}


BEGIN_MESSAGE_MAP(CComSetDlg, CDialog)
END_MESSAGE_MAP()


// CComSetDlg-Meldungshandler

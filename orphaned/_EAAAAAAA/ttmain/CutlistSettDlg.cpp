// CutlistSettDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ZRSheet.h"
#include "RegistryManager.h"
#include "MainFrm.h"
#include "ttmain.h"
#include "CutlistSettDlg.h"


// CCutlistSettDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CCutlistSettDlg, CDialog)

CCutlistSettDlg::CCutlistSettDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCutlistSettDlg::IDD, pParent)
    , m_iProfileLen(0)
    , m_iBladeWidth(0)
    , m_bRoundUp(FALSE)
{

}

CCutlistSettDlg::~CCutlistSettDlg()
{
}

void CCutlistSettDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_WHS_PROFILE_LEN_EDIT, m_iProfileLen);
    DDV_MinMaxInt(pDX, m_iProfileLen, 0, 20000);
    DDX_Text(pDX, IDC_WHS_BLADE_WIDTH_EDIT, m_iBladeWidth);
    DDV_MinMaxInt(pDX, m_iBladeWidth, 0, 100);
    DDX_Check(pDX, IDC_WHS_ROUND_UP_P_EDIT, m_bRoundUp);
}


BEGIN_MESSAGE_MAP(CCutlistSettDlg, CDialog)
END_MESSAGE_MAP()


// CCutlistSettDlg-Meldungshandler

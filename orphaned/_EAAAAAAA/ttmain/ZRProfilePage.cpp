// ZRProfilePage.cpp : implementation file
//

#include "stdafx.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRTreeCtrl.h"
#include "ZRSheet.h"
#include "ZRBasePage.h"
#include "ZRProfilePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZRProfilePage property page

IMPLEMENT_DYNCREATE(CZRProfilePage, CZRBasePage)

CZRProfilePage::CZRProfilePage() : CZRBasePage("", CZRProfilePage::IDD)
{
	//{{AFX_DATA_INIT(CZRProfilePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_ppRootNode = NULL;
    m_pNodeToShow = NULL;
}

CZRProfilePage::CZRProfilePage(char* strTitle, CNode** ppRootNode) : 
                CZRBasePage(strTitle, CZRProfilePage::IDD)
{
    m_strTitle = strTitle;
    m_ppRootNode = ppRootNode;
    m_pNodeToShow = NULL;
}

CZRProfilePage::~CZRProfilePage()
{
}

void CZRProfilePage::DoDataExchange(CDataExchange* pDX)
{
	CZRBasePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZRProfilePage)
	DDX_Control(pDX, IDC_ZR_PROFILE_TREE, m_treeZR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZRProfilePage, CZRBasePage)
	//{{AFX_MSG_MAP(CZRProfilePage)
	ON_WM_SIZE()
    ON_MESSAGE(WM_PARENT_CTRL_CHANGED, OnTreeCtrlChanged)
    ON_MESSAGE(WM_ZR_SHEET_NEW_TREE, OnNewTree)
    ON_MESSAGE(WM_ZR_SHEET_SHOW_NODE, OnShowNode)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRProfilePage message handlers

void CZRProfilePage::OnSize(UINT nType, int cx, int cy) 
{
	CZRBasePage::OnSize(nType, cx, cy);

    if (nType == SIZE_RESTORED)
    {
        if (m_treeZR.GetSafeHwnd() != NULL)
        {
            CRect rect;
            GetClientRect(&rect);
            rect.right = cx;
            rect.bottom = cy;
            m_treeZR.MoveWindow(&rect);
        }
    }
}

BOOL CZRProfilePage::OnInitDialog() 
{
	CZRBasePage::OnInitDialog();
	
    if (m_ppRootNode != NULL)
        m_treeZR.SetRootNode(m_ppRootNode);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CZRProfilePage::SetRootNode(CNode **ppRootNode)
{
    m_ppRootNode = ppRootNode;
    return m_treeZR.SetRootNode(m_ppRootNode);
}

BOOL CZRProfilePage::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if (m_treeZR.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) == TRUE)
        return TRUE;
	
	return CZRBasePage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CZRProfilePage::PreTranslateMessage(MSG* pMsg) 
{	
	return CZRBasePage::PreTranslateMessage(pMsg);
}

BOOL CZRProfilePage::OnSetActive() 
{
	return CZRBasePage::OnSetActive();
}


LRESULT CZRProfilePage::OnTreeCtrlChanged(WPARAM wParam, LPARAM lParam)
{
    return SignalPageChanged();
}


void CZRProfilePage::GetButtonStates(SetButtonsStruct *pButtons)
{
    // Button-Zustände ermitteln
    // Apply nur, wenn sich im TreeCtrl etwas geändert hat
    pButtons->dwStateButtonApply = WS_VISIBLE;
    // immer sichbar
    pButtons->dwStateButtonCancel = WS_VISIBLE;
    // immer sichbar
    pButtons->dwStateButtonHelp = WS_VISIBLE;
    // nur sichtbar, wenn TreeCtrl leer
    if (*m_ppRootNode == NULL)
        pButtons->dwStateButtonNew = WS_VISIBLE;
    else
        pButtons->dwStateButtonNew = WS_DISABLED;

    // Apply nur, wenn sich im TreeCtrl etwas geändert hat
    if (m_treeZR.HasChanged())
        pButtons->dwStateButtonSave = WS_VISIBLE;
    else
        pButtons->dwStateButtonSave = WS_DISABLED;
}

LRESULT CZRProfilePage::OnNewTree(WPARAM wParam, LPARAM lParam)
{
    m_treeZR.NewTree();
    return 0L;
}

BOOL CZRProfilePage::OnKillActive() 
{
    CWnd* pParent = GetParent();
    if (pParent)
        pParent->SetFocus();
	return CZRBasePage::OnKillActive();
}

void CZRProfilePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    m_treeZR.RedrawWindow();
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CZRProfilePage::OnOK() 
{
	// TODO: Add extra validation here
	
	CZRBasePage::OnOK();
}


BOOL CZRProfilePage::OnQueryCancel() 
{
    if (m_treeZR.HasChanged())
        return FALSE;
	return CZRBasePage::OnQueryCancel();
}

LRESULT CZRProfilePage::OnShowNode(WPARAM wParam, LPARAM lParam)
{
    m_pNodeToShow = (CNode*)lParam;
    return ShowNode(m_pNodeToShow);
}

BOOL CZRProfilePage::ShowNode(CNode *pNode)
{
    return m_treeZR.FindAndSelectItem(pNode);
}

// ZRCBox.cpp : implementation file
//

#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ZRCBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static BOOL IsRunningOnWinXP(void)
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   //
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      case VER_PLATFORM_WIN32_NT:

      // Test for the product.

         if ( osvi.dwMajorVersion <= 4 )
            //printf("Microsoft Windows NT ");
            return FALSE;

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            //printf ("Microsoft Windows 2000 ");
            return FALSE;

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
            //printf ("Microsoft Windows XP ");
            return TRUE;

      case VER_PLATFORM_WIN32_WINDOWS:
          // Windows 95,98,ME
            return FALSE;
         break;

   }
   return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CZRCBox

CZRCBox::CZRCBox()
{
    m_pNode = NULL;
    m_bIsDestroyed = FALSE;
}

CZRCBox::~CZRCBox()
{
}


BEGIN_MESSAGE_MAP(CZRCBox, CComboBox)
	//{{AFX_MSG_MAP(CZRCBox)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendOk)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillFocus)
    ON_MESSAGE(WM_ZR_BOX_DESTROY, OnDestroyBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRCBox message handlers


// Function name	: CZRCBox::Create
// Description	    : erzeugt eine Combo-Box
// Return type		: BOOL 
// Argument         : CRect &rect       Fenster-Position
// Argument         : CWnd *pParent     Parent-Fenster
// Argument         : DWORD dwMsgId     Msg-Id für Senden bei erfolgter Auswahl
// Argument         : CNode *pNode      betreffende Node
// Argument         : int iMode         Mode: 0==Wert, 1==Eigenschaft
BOOL CZRCBox::Create(CRect &rect, CWnd *pParent, DWORD dwMsgId, CNode *pNode, 
                     int iMode)
{
    BOOL bRet;
    m_pNode = pNode;
    if (m_pNode == NULL)
        return FALSE;

    m_dwMsgId = dwMsgId;
    
    if ((bRet = CComboBox::Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_SORT, 
        rect, pParent, 0)) == FALSE)
        return bRet;

    CPaintDC dc(this);
    CFont* pFont = GetFont();
    CSize size = 0;

    // Box mit den möglichen Werten füllen
    int iCountStr = 0;
    char** pStr = NULL;
    if (iMode == 0)
    {
        // Eigenschaftswert
        if (m_pNode->GetProperty().GetPropertyValueStrings(iCountStr, &pStr) == FALSE)
            return FALSE;
        for (int i=0; i<iCountStr; i++)
        {
            if (m_pNode->ValueForChildAllowed(pStr[i]))
            {
                AddTextSized(&dc, pStr[i], size);
            }
        }
        // den eigenen Wert auch noch hinzufügen
        CString strValue;
        int iValue = m_pNode->GetValueForChild(m_pNode->iChildIndex);
        if (m_pNode->GetProperty().ConvertIntToString(iValue, strValue) == TRUE)
        {
            AddTextSized(&dc, strValue.GetBuffer(strValue.GetLength()), size);
            strValue.ReleaseBuffer();
        }


        // bisherigen Wert auswählen
        m_pNode->GetProperty().ConvertIntToString(
            m_pNode->GetValueForChild(pNode->iChildIndex), strValue);
        SetCurSel(FindString(-1, strValue));
    }
    else
    {
        // Eigenschaft
        if (m_pNode->GetProperty().GetPropertyStrings(iCountStr, &pStr) == FALSE)
            return FALSE;
        for (int i=0; i<iCountStr; i++)
        {
            if (m_pNode->IsPropertyPossible(pStr[i]) == TRUE)
                AddTextSized(&dc, pStr[i], size);
        }
        
        if (m_pNode->GetProperty().m_Property != VOLATILE 
            && m_pNode->GetProperty().m_Property != NOT_SET)
        {
            SetCurSel(FindString(-1, m_pNode->GetProperty().m_strName));
        }
        else
        {
            // 1. Wert auswählen
            SetCurSel(0);
        }
    }

    // Fenstergröße anpassen
    NONCLIENTMETRICS metrics;
    metrics.cbSize = sizeof(metrics);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &metrics, 0);
    size.cx = __max(size.cx, 20);
    MoveWindow(rect.left, rect.top, size.cx+metrics.iCaptionWidth, rect.Height());

    SetFocus();
    return TRUE;
}

void CZRCBox::PostNcDestroy() 
{
    delete this;
    
	CComboBox::PostNcDestroy();
}

void CZRCBox::OnSelendOk() 
{
    char strTemp[MAX_PATH];

    if (GetLBText(GetCurSel(), strTemp) != CB_ERR)
    {
        GetParent()->SendMessage(m_dwMsgId, (WPARAM)m_pNode, (LPARAM)strTemp);
    }

    m_bIsDestroyed = TRUE;
    if (IsRunningOnWinXP() == TRUE)
        PostMessage(WM_ZR_BOX_DESTROY);
    else
        DestroyWindow();
}

void CZRCBox::OnKillFocus() 
{
    if (m_bIsDestroyed == FALSE)
    {
        if (IsRunningOnWinXP() == TRUE)
            PostMessage(WM_ZR_BOX_DESTROY);
        else
            DestroyWindow();
    }
}


LRESULT CZRCBox::OnDestroyBox(WPARAM wParam, LPARAM lParam)
{
    DestroyWindow();
    return 0L;
}

void CZRCBox::AddTextSized(CDC *pDc, char* str, CSize &size)
{
    CSize size1 = 0;

    AddString(str);
    size1 = pDc->GetTextExtent(str);
    if (size1.cx > size.cx)
        size = size1;
}

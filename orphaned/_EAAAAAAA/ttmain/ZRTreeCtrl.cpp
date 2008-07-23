// ZRTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Macros.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "resource.h"       // 
#include "ttmain.h"
#include "ZRCBox.h"
#include "ZRTreeNewDlg.h"
#include "ZRTreeCtrl.h"
#include "ZRTreeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////
// defines
#define   IDX_BMP_Z_WERT            (ZR_MAX_EIGENSCHAFTEN+ZR_MAX_WERTE)

////////////
// structs
typedef struct
{
    eTTEigenschaften Eigenschaft;
    //UINT uiBmp;                     // Index in der Imageliste
    char strName[30];
} zrEigenschaft;

typedef struct
{
    UINT uiBmp;                     // Index in der Imageliste
    char strName[30];
} zrWert;


////////////////
// arrays

// Achtung: Reihenfolge muß der Reihenfolge der Bitmaps entsprechen!!!!!!
zrEigenschaft g_arrEigenschaften[ZR_MAX_EIGENSCHAFTEN] =
{
    { TYP, "Typ" }, { ART, "Art" }, { RAHMEN, "Rahmen" }, 
    { FLUEGEL_ANZ, "Flügelanzahl" }, { OBER_TEIL, "Oberteil" }, 
    { GETEILT, "Geteilt" }, { OKF, "OKF" }, { BOGEN, "Bogen" }
};

zrWert g_arrWerte[ZR_MAX_WERTE] =
{
    { 0, "Tor" }, { 1, "Tür" }, { 2, "*" }, { 3, "400" }, { 4, "600" }, { 5, "800" },
    { 6, "Z" }, { 7, "R" }, { 8, "W" }, { 9, "kein" }, { 10, "OzO" }, { 11, "OiF" },
    { 12, "2" }, { 13, "3" }, { 14, "4" }, { 15, "5" }, { 16, "ja" }, { 17, "nein" }
};
/////////////////////////////////////////////////////////////////////////////
// CZRTreeCtrl

CZRTreeCtrl::CZRTreeCtrl()
{
    m_pMan = NULL;
    m_pImageList = NULL;
    m_ppRootNode = NULL;
    m_bChanged = FALSE;
}

CZRTreeCtrl::~CZRTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CZRTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CZRTreeCtrl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRButtonClick)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_ZR_PROP_VALUE_CHANGE, OnUpdateZrPropValueChange)
	ON_COMMAND(ID_ZR_PROP_VALUE_CHANGE, OnZrPropValueChange)
    ON_MESSAGE(WM_COMBOBOX_SELECTION, OnComboBoxSelection)
    ON_MESSAGE(WM_COMBOBOX_SELECTION_PROPERTY, OnComboBoxSelectionProperty)
	ON_COMMAND(ID_ZR_PROP_VALUE_NEW, OnZrPropValueNew)
	ON_UPDATE_COMMAND_UI(ID_ZR_PROP_VALUE_NEW, OnUpdateZrPropValueNew)
	ON_COMMAND(ID_ZR_PROP_VALUE_DELETE, OnZrPropValueDelete)
	ON_UPDATE_COMMAND_UI(ID_ZR_PROP_VALUE_DELETE, OnUpdateZrPropValueDelete)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZRTreeCtrl message handlers

void CZRTreeCtrl::SetZRManager(CZuschnittManager *pMan)
{
	CBitmap             bitmap;
	UINT                nID;
    CImageList          *pImageList = NULL;

    m_pMan = pMan;
    if (m_pMan != NULL)
    {
        // load Bitmaps
        if (pImageList == NULL)
        {
	        pImageList = new CImageList();
	        pImageList->Create(32, 16, ILC_MASK, 20, 6);
        }
	    for (nID = IDB_BMTREEFIRST; nID <= IDB_BMTREELAST; nID++)  // load bitmaps for dog, bird and fish
	    {
		    bitmap.LoadBitmap(nID);
		    pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
		    bitmap.DeleteObject();
	    }
	    SetImageList(pImageList, TVSIL_NORMAL);

        BuildTree(m_pMan->m_pBaumSenk);
        BuildTree(m_pMan->m_pBaumQuer);
        BuildTree(m_pMan->m_pBaumSenk);
        BuildTree(m_pMan->m_pBaumSenk);
    }

    m_ptreeEdit = NULL;
}



// Function name	: CZRTreeCtrl::SetRootNode
// Description	    : Übergabe der anzuzeigenden Node
// Return type		: BOOL 
// Argument         : CNode *pRootNode
BOOL CZRTreeCtrl::SetRootNode(CNode **ppRootNode)
{
	CBitmap             bitmap;
	UINT                nID;
    CImageList          *pImageList = NULL;

    m_ppRootNode = ppRootNode;
    if (m_ppRootNode != NULL)
    {
        // load Bitmaps
        if (pImageList == NULL)
        {
	        pImageList = new CImageList();
	        pImageList->Create(32, 16, ILC_MASK, 20, 6);
        }
	    for (nID = IDB_BMTREEFIRST; nID <= IDB_BMTREELAST; nID++)  // load bitmaps for dog, bird and fish
	    {
		    bitmap.LoadBitmap(nID);
		    pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
		    bitmap.DeleteObject();
	    }
	    SetImageList(pImageList, TVSIL_NORMAL);

        BuildTree(*m_ppRootNode);
    }

    m_ptreeEdit = NULL;

    return TRUE;
}

// Function name	: CZRTreeCtrl::BuildTree
// Description	    : erzeugt aus dem Anfangsnode einen Baum
// Return type		: void 
// Argument         : CNode* pParentNode
void CZRTreeCtrl::BuildTree(CNode* pParentNode)
{
    if (pParentNode != NULL)
    {
        InsertNode(NULL, pParentNode);
    }
}

void CZRTreeCtrl::InsertNode(HTREEITEM hParentItem, CNode* pNode)
{
	TV_INSERTSTRUCT     curTreeItem;
    HTREEITEM           hTreeItem, hItemValue;
    CString             strPropName;
    char                strTemp[50];

    if (pNode == NULL)
        return;

    // Property-Typ eingeben
	curTreeItem.hParent = hParentItem;
	curTreeItem.hInsertAfter = TVI_SORT;
    if (pNode->GetProperty().m_Property == VOLATILE)
    {
        // ist ein flüchtiges Element, nichts machen
        return;
    }
    if (pNode->IsEndNode() == FALSE)
    {
        strPropName = pNode->GetProperty().GetName();
	    curTreeItem.item.pszText = strPropName.GetBuffer(strPropName.GetLength());
        for (int i=0; i<ZR_MAX_EIGENSCHAFTEN; i++)
        {
            if (g_arrEigenschaften[i].Eigenschaft == pNode->GetProperty().GetPropertyValue())
            {
                // i entspricht Index in Imagelist
                break;
            }
        }
	    curTreeItem.item.iImage = i;
    }
    else
    {
	    curTreeItem.item.pszText = itoa(pNode->GetNodeValue(), strTemp, 10);
	    curTreeItem.item.iImage = IDX_BMP_Z_WERT;
    }
	curTreeItem.item.iSelectedImage = curTreeItem.item.iImage + 0;
	curTreeItem.item.lParam = (LPARAM)pNode;
	curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	hTreeItem = InsertItem(&curTreeItem);
//    pNode->hTreeItem = (HANDLE)hTreeItem;

    // alle nachfolgenden Nodes eintragen
    if (pNode->IsEndNode() == FALSE)
    {
        CProperty prop = pNode->GetProperty();

        for (int i=pNode->GetChildCount()-1; i>=0; i--)
        {
            // Property-Werte und Nachfolger
	        curTreeItem.hParent = hTreeItem;
	        curTreeItem.hInsertAfter = TVI_FIRST;
            prop.ConvertIntToString(pNode->GetValueForChild(i), strPropName);
	        curTreeItem.item.pszText = strPropName.GetBuffer(strPropName.GetLength());
            for (int x=0; x<ZR_MAX_WERTE; x++)
            {
                if (g_arrWerte[x].strName == strPropName)
                {
                    // i entspricht Index in Imagelist
                    break;
                }
            }
	        curTreeItem.item.iImage = x+ZR_MAX_EIGENSCHAFTEN;
	        curTreeItem.item.iSelectedImage = curTreeItem.item.iImage + 0;
	        curTreeItem.item.lParam = (LPARAM)pNode;
        	curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	        hItemValue = InsertItem(&curTreeItem);

            InsertNode(hItemValue, pNode->GetChildAt(i));
        }
    }
}

void CZRTreeCtrl::OnDestroy() 
{	
    m_pImageList = GetImageList(TVSIL_NORMAL);
    if (m_pImageList != NULL)
    {
        m_pImageList->DeleteImageList();
        delete m_pImageList;	
    }

	CTreeCtrl::OnDestroy();
}

BOOL CZRTreeCtrl::ReadTree(CNode *pRoot)
{
    return FALSE;
}

void CZRTreeCtrl::OnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CZRTreeCtrl::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    BOOL bNew, bChange, bDelete;
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    QueryPossibleActions(pTVDispInfo->item.hItem, bNew, bChange, bDelete);

    if (bChange == TRUE)
    {
        CNode* pNode = (CNode*)GetItemData(pTVDispInfo->item.hItem);
        if ((pNode != NULL) && (pNode->IsEndNode() == FALSE) 
            /*IsPropertyValue(pTVDispInfo->item.hItem) == TRUE*/)
        {
            // enable editing if value
            OnZrPropValueChange();
            *pResult = TRUE;   // kein edit
            return;
        }
        else
            *pResult = FALSE;   // edit erlauben
    }
    else
        *pResult = TRUE;    // edit verweigern

    if (*pResult == FALSE)
    {
        // edit wurde enabled
        CEdit* pEdit = GetEditControl();
        m_ptreeEdit = new CZRTreeEdit;
    
        if (m_ptreeEdit != NULL && pEdit != NULL)
            m_ptreeEdit->SubclassWindow(pEdit->GetSafeHwnd());
    }

}

void CZRTreeCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
    CNode* pNode = (CNode*)GetItemData(pTVDispInfo->item.hItem);
    
    SAFE_DELETE(m_ptreeEdit);

    if (pNode == NULL)
    {
        // error, don't accept
    	*pResult = FALSE;
        return;
    }
    if (pNode->IsEndNode() == TRUE)
    {
        // Ende-Node, Zuschnitt-Wert speichern
        if (pTVDispInfo->item.pszText == NULL)
        {
            // Fehler, nicht uebernehmen
    	    *pResult = FALSE;
            return;
        }
        int iValue = atoi(pTVDispInfo->item.pszText);
        if (iValue <= 0)
        {
            // Fehler, nicht uebernehmen
    	    *pResult = FALSE;
            return;
        }
        if (pNode->GetNodeValue() != iValue)
        {
            pNode->SetNodeValue(iValue);
            SetChange();
        }
    }
    if (IsPropertyValue(pTVDispInfo->item.hItem) == TRUE)
    {
        // ist Eigenschaftswert-Item, uebernehmen
        //??? zuerst abprüfen
        
    }
    // Edit uebernehmen
	*pResult = TRUE;
}


// Function name	: CZRTreeCtrl::IsPropertyValue
// Description	    : gibt an ob das Item einen Wert oder eine Eigenschaft darstellt
// Return type		: BOOL TRUE==Wert FALSE==Eigenschaft
// Argument         : HTREEITEM hItem
BOOL CZRTreeCtrl::IsPropertyValue(HTREEITEM hItem)
{
    HTREEITEM hParent = GetParentItem(hItem);
    CNode* pNode = (CNode*)GetItemData(hItem);

    if (pNode == NULL)
        return FALSE;
    if (hParent == NULL)
    {
        // kein Parent-Item -> muß Eigenschafts-Item sein, kein edit
	    return FALSE;
    }    
    CNode* pParent = (CNode*)GetItemData(hParent);
    if (pNode == pParent)
    {
        // pNode muß Property-Value sein, enable edit
        return TRUE;
    }

    return FALSE;
}

void CZRTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
    CMenu menu;
    BOOL bNew, bChange, bDelete;
    HTREEITEM hItem = GetSelectedItem();

    menu.LoadMenu(IDR_ZR_PROP_VALUE);
    CMenu *pMenu = menu.GetSubMenu(0);
    if (pMenu)
    {         
        QueryPossibleActions(hItem, bNew, bChange, bDelete);
        if (bNew == FALSE)
            pMenu->EnableMenuItem(ID_ZR_PROP_VALUE_NEW, MF_BYCOMMAND|MF_GRAYED);
        if (bChange == FALSE)
            pMenu->EnableMenuItem(ID_ZR_PROP_VALUE_CHANGE, MF_BYCOMMAND|MF_GRAYED);
        if (bDelete == FALSE)
            pMenu->EnableMenuItem(ID_ZR_PROP_VALUE_DELETE, MF_BYCOMMAND|MF_GRAYED);

        ClientToScreen(&point);
        pMenu->TrackPopupMenu(/*TPM_LEFTALIGN|TPM_RIGHTBUTTON*/0, 
            point.x, point.y, GetParent(), NULL);
    }

    //?? wenn drin, dann Probleme???
//	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CZRTreeCtrl::OnZrPropValueNew() 
{
    CNode* pChild;
    int iCountStr, iValue, iPosition;
    CString strValue;
    char** pStr;
    TVINSERTSTRUCT item;
    HTREEITEM hNew, hChild, hItem;
    hItem = GetSelectedItem();
    if (hItem == NULL)
    {
        NewTree();
        return;
    }
    CNode* pNode = (CNode*)GetItemData(hItem);
    if (pNode == NULL)
        return;

    if (pNode->IsEndNode() == TRUE)
        return;

    if (IsPropertyValue(hItem) == FALSE)
    {
        // ist eine Eigenschaft -> neuer Wert
        if (pNode->GetFreeValue(iValue, strValue) == FALSE)
            return;

        if (pNode->AddChild(iValue, &pChild, iPosition) == FALSE)
        {
            return;
        }
        pChild->GetProperty().m_Property = VOLATILE;

        // Einfügeplatz festlegen
        if (iPosition != 0)
        {
            hChild = GetChildItem(hItem);
            if (hChild == NULL)
            {
                pNode->DeleteChildAt(iPosition);
                return;
            }
            iPosition--;
            while (iPosition-- > 0)
            {
                hChild = GetNextSiblingItem(hChild);
            }
            item.hInsertAfter = hChild;        
        }
        else
        {
            item.hInsertAfter = TVI_FIRST;
        }
	    item.hParent = hItem;
	    item.item.pszText = strValue.GetBuffer(strValue.GetLength());            //?? später vielleicht anderen Wert wählen
        strValue.ReleaseBuffer();
        for (int x=0; x<ZR_MAX_WERTE; x++)
        {
            if (strcmp(g_arrWerte[x].strName, strValue) == 0)
            {
                // i entspricht Index in Imagelist
                break;
            }
        }
	    item.item.iImage = x+ZR_MAX_EIGENSCHAFTEN;
	    item.item.iSelectedImage = item.item.iImage + 0;
	    item.item.lParam = (LPARAM)pNode;
        item.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	    hNew = InsertItem(&item);
        SelectItem(hNew);
        
        OnZrPropValueChange();
    }
    else
    {
        // ist ein Wert -> neue Eigenschaft ODER End-Wert
        CZRTreeNewDlg dlg(this);
        if (dlg.DoModal() == IDOK)
        {
            if (dlg.m_rbNewProperty == 0)
            {
                // neue Eigenschaft
                CNode *pChild;
                HTREEITEM hAct = hItem;
                int i = 0;
                // Position des Items und damit der Child-Node finden
                while ((hAct = GetPrevSiblingItem(hAct)) != NULL)
                    i++;
                pChild = pNode->GetChildAt(i);
                if (pChild == NULL)
                    return;         // Fehler
                if (pChild->GetProperty().m_Property != VOLATILE)
                    return;         // Fehler, wird beim Anlegen eines Eigenschaftswertes angelegt

                CString strValue;
	            item.hParent = hItem;
	            item.hInsertAfter = TVI_LAST;
                pNode->GetProperty().GetPropertyStrings(iCountStr, &pStr);
                // Property auswählen
                for (int x=0; x<iCountStr; x++)
                {
                    if (pChild->IsPropertyPossible(pStr[x]) == TRUE)
                        break;
                }
                if (x == iCountStr)
                {
                    pNode->DeleteChildAt(i);
                    return;     // Fehler: keine Property mehr möglich
                }
                strValue = pStr[x];
	            item.item.pszText = strValue.GetBuffer(strValue.GetLength());            //?? später vielleicht anderen Wert wählen
                strValue.ReleaseBuffer();
                for (x=0; x<ZR_MAX_WERTE; x++)
                {
                    if (strcmp(g_arrEigenschaften[x].strName, strValue) == 0)
                    {
                        // i entspricht Index in Imagelist
                        break;
                    }
                }
	            item.item.iImage = x;
	            item.item.iSelectedImage = item.item.iImage + 0;
	            item.item.lParam = (LPARAM)pChild;
                item.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	            hNew = InsertItem(&item);
                SelectItem(hNew);

                // neues Child mit Wert vorbelegen
                eTTEigenschaften eE;
                pChild->GetProperty().ConvertPropertyStrToEnum(strValue, eE);
                pChild->SetProperty(eE, strValue.GetBuffer(strValue.GetLength()));
                strValue.ReleaseBuffer();
                OnZrPropValueChange();
            }
            else
            {
                // neuer Zuschnittswert
                // Vorraussetzung: es gibt schon einen Child mit Zustand VOLATILE
                CNode* pChild;
                char strTemp[100];
                int iValue;

                CString strText = GetItemText(hItem);       // man hat 2 Möglichkeiten,
                // um den Child zu finden: über Position im Baum oder über Item-Text
                pNode->GetProperty().ConvertStringToInt(strText, iValue);
                pChild = pNode->GetChildByValue(iValue);
                if (pChild == NULL)
                    return;     // Fehler

                pChild->GetProperty().m_Property = NOT_SET;
                pChild->SetNodeValue(0);

	            item.hParent = hItem;
	            item.hInsertAfter = TVI_LAST;
	            item.item.pszText = itoa(pChild->GetNodeValue(), strTemp, 10);
	            item.item.iImage = IDX_BMP_Z_WERT;
	            item.item.iSelectedImage = item.item.iImage + 0;
	            item.item.lParam = (LPARAM)pChild;
	            item.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	            hNew = InsertItem(&item);
                SelectItem(hNew);
                EditLabel(hNew);
            }
        }
    }
    SetChange();
}

void CZRTreeCtrl::OnUpdateZrPropValueChange(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CZRTreeCtrl::OnZrPropValueChange() 
{
    //?? Listbox mit möglichen Werten erzeugen 
    CRect rect;

    HTREEITEM hItem = GetSelectedItem();
    if (hItem == NULL)
        return;
    CNode* pNode = (CNode*)GetItemData(hItem);
    if (pNode == NULL)
        return;

    if (pNode->IsEndNode() == TRUE)
    {
        // Zuschnittwert ändern
        EditLabel(hItem);
    }
    else
        if (IsPropertyValue(hItem) == TRUE)
        {
            // Eigenschaftswert ändern
            if (GetItemRect(hItem, &rect, TRUE) == TRUE)
            {
                CZRCBox* pBox = new CZRCBox;        // pBox löscht sich selber wieder
                if (pBox == NULL)
                    return;

                //rect.left = rect.right;
                rect.right = rect.left+50;
                rect.bottom += 100;

                pNode->hTreeItem = hItem;       // Item merken
                // Item-Text benützen, um richtigen Array-Index zu finden
                CString str = GetItemText(hItem);
                int iValue;
                if (pNode->GetProperty().ConvertStringToInt(str, iValue) == FALSE)
                {
                    delete pBox;
                    return;
                }
                for (int i=0; i<(int)pNode->GetChildCount(); i++)
                {
                    if (iValue == pNode->GetValueForChild(i))
                        break;
                }
                if (i==(int)pNode->GetChildCount())
                {
                    delete pBox;
                    return;
                }
                pNode->iChildIndex = i;
                if (pBox->Create(rect, this, WM_COMBOBOX_SELECTION, pNode, 0) == TRUE)
                {
                    pBox->SetItemHeight(-1, GetItemHeight()-5);
                    pBox->SetFont(GetFont());
                }
                else
                    delete pBox;
            }
        }
        else
        {
            // Eigenschaft ändern -> nur bei neu
            if (GetItemRect(hItem, &rect, TRUE) == TRUE)
            {
                CZRCBox* pBox = new CZRCBox;        // pBox löscht sich selber wieder
                if (pBox == NULL)
                    return;

                //rect.left = rect.right;
                rect.right = rect.left+50;
                rect.bottom += 100;

                pNode->hTreeItem = hItem;       // Item merken
                if (pBox->Create(rect, this, WM_COMBOBOX_SELECTION_PROPERTY, pNode, 1) == TRUE)
                {
                    pBox->SetItemHeight(-1, GetItemHeight()-5);
                    pBox->SetFont(GetFont());
                }
                else
                    delete pBox;
            }
        }
}



// Function name	: OnComboBoxSelection
// Description	    : Handler für Message von Combobox, daß eine gültige Auswahl getroffen wurde
// Return type		: LRESULT 
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam     Zeiger auf String
LRESULT CZRTreeCtrl::OnComboBoxSelection(WPARAM wParam, LPARAM lParam)
{
    int iValue;
    CString strTemp = (char*)lParam;
    CNode* pNode = (CNode*)wParam;
    //?? Auswahl überprüfen
    if (strTemp.IsEmpty() || pNode == NULL)
        return 0L;

    // herausfinden, ob nur Änderung oder neuer Child
    int iItemCount = 0;
    int iNodeCount = pNode->GetChildCount();
    HTREEITEM hParent = GetParentItem((HTREEITEM)pNode->hTreeItem);
    if (hParent == NULL)
    {
        //?? kein Parent
    }
    HTREEITEM hChild = GetChildItem(hParent);
    if (hChild != NULL)
        iItemCount++;
    else
        return 0L;
    while ((hChild = GetNextSiblingItem(hChild)) != NULL)
    {
        iItemCount++;
    }
#if 0      //?? wird wahrscheinlich nicht mehr gebraucht
    if (iItemCount == (iNodeCount+1))
    {
        int iPosition;
        // neuer Child
        CNode* pChild;
        if (pNode->GetProperty().ConvertStringToInt(strTemp, iValue) == FALSE)
            return 0L;   // Fehler
        if (pNode->AddChild(iValue, &pChild, iPosition) == FALSE)
            return 0L;   // Fehler

        //?? Absturz: kommt man hier noch rein???
        DWORD* pDw = NULL;
        *pDw = 0;

        pChild->SetProperty(VOLATILE, "");
        // Tree-Item updaten
        // Images finden
        for (int x=0; x<ZR_MAX_WERTE; x++)
        {
            if (g_arrWerte[x].strName == strTemp)
            {
                // x entspricht Index in Imagelist
                break;
            }
        }
        x += ZR_MAX_EIGENSCHAFTEN;
        // update
        CString strOld = GetItemText((HTREEITEM)pNode->hTreeItem);
        if (strOld != strTemp)
        {
            SetItem((HTREEITEM)pNode->hTreeItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT, strTemp, 
                x, x, 0, 0, NULL);
        }
    }
    else
#endif
//    if (iItemCount == iNodeCount)
//    {
        // pNode updaten
        if (pNode->GetProperty().ConvertStringToInt(strTemp, iValue) == FALSE)
            return 0L;   // Fehler
        if (pNode->GetValueForChild(pNode->iChildIndex) != iValue)
        {
            pNode->SetValueForChild(pNode->iChildIndex, iValue);

            // Tree-Item updaten
            // Images finden
            for (int x=0; x<ZR_MAX_WERTE; x++)
            {
                if (g_arrWerte[x].strName == strTemp)
                {
                    // x entspricht Index in Imagelist
                    break;
                }
            }
            x += ZR_MAX_EIGENSCHAFTEN;
            // update
            SetItem((HTREEITEM)pNode->hTreeItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT, strTemp, 
                x, x, 0, 0, NULL);
            SetChange();
        }
//    }
    return 0L;
}

LRESULT CZRTreeCtrl::OnComboBoxSelectionProperty(WPARAM wParam, LPARAM lParam)
{
    HTREEITEM hItem;
    TVITEM item;

    CString strTemp = (char*)lParam;
    CNode* pNode = (CNode*)wParam;
    //?? Auswahl überprüfen
    if (strTemp.IsEmpty() || pNode == NULL)
        return 0L;
    
    hItem = (HTREEITEM)pNode->hTreeItem;

    if (hItem == NULL)
        return 0L;

    for (int x=0; x<ZR_MAX_WERTE; x++)
    {
        if (strTemp == g_arrEigenschaften[x].strName)
        {
            // x entspricht Index in Imagelist
            break;
        }
    }

    // TreeCtrl updaten
    item.hItem = hItem;
	item.iImage = x;
	item.iSelectedImage = item.iImage + 0;
	item.lParam = (LPARAM)pNode;
    item.pszText = (char*)lParam;
    item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	SetItem(&item);

    // Eigenschaft in pNode setzen
    pNode->SetProperty(g_arrEigenschaften[x].Eigenschaft, g_arrEigenschaften[x].strName);

    SetChange();

    return 0L;
}


void CZRTreeCtrl::OnUpdateZrPropValueNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CZRTreeCtrl::OnZrPropValueDelete() 
{
#if 1
    CWnd* pWnd = this;
#else
    CWnd* pWnd= (CWnd*)GetParent();
    pWnd = pWnd->GetParent();
#endif
    if (pWnd != NULL)
    {
        if (pWnd->MessageBox("Wollen Sie diesen Zweig wirklich löschen?", 
            "Achtung", MB_OKCANCEL|MB_ICONWARNING)
            == IDOK)
        {
            // ok, Node und Unter-Nodes löschen
            HTREEITEM hItem = GetSelectedItem();

            // Items löschen
            if (DeleteLeaf(hItem) == FALSE)
            {
                //?? Fehlerbehandlung, vielleicht alles aus Datei neu einlesen
            }
            SetChange();
        }
    }
}

void CZRTreeCtrl::OnUpdateZrPropValueDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

BOOL CZRTreeCtrl::DeleteLeaf(HTREEITEM hItem)
{
    CNode *pNode = (CNode*)GetItemData(hItem);
    if (pNode == NULL)
        return FALSE;

    if (pNode->IsEndNode() == TRUE)
    {
        // Problem: wenn Zuschnittwert weg ist, dannist eigentlich auch 
        // der Eigenschaftswert weg -> auch Parent-Item löschen
        CNode* pParent = pNode->GetParent();
        if (pParent != NULL)
        {
            pParent->RemoveChild(pNode);
            return RemoveItemAndChilds(GetParentItem(hItem), FALSE);
        }
        else
            return FALSE;
    }
    if (IsPropertyValue(hItem) == FALSE)
    {
        // pNode auf VOLATILE setzen und alle Kinder löschen
        pNode->GetProperty().m_Property = VOLATILE;

        if (RemoveItemAndChilds(hItem, FALSE) == FALSE)
            return FALSE;
        if (pNode->DestroyLeaf() == FALSE)
            return FALSE;
        if (pNode->IsRootNode() == TRUE)
        {
            // Ist Root-Node
            delete pNode;
            *m_ppRootNode = NULL;
        }
    }
    else
    {
        // Eigenschaftswert, pNode zeigt auf richtiges Array
        // Array-Eintrag anhand des Wertes suchen
        CString strValue = GetItemText(hItem);
        int iValue;
        if (pNode->GetProperty().ConvertStringToInt(strValue, iValue) == FALSE)
            return FALSE;
        for (DWORD i=0; i<pNode->GetChildCount(); i++)
        {
            if (iValue == pNode->GetValueForChild(i))
                break;
        }
        if (i==pNode->GetChildCount())
            return FALSE;    // Fehler, nicht gefunden

        if (pNode->DeleteChildAt(i) == FALSE)
            return FALSE;

        if (RemoveItemAndChilds(hItem, FALSE) == FALSE)
            return FALSE;
    }
    return TRUE;
}

BOOL CZRTreeCtrl::RemoveItemAndChilds(HTREEITEM hItem, BOOL bRemoveSibling)
{
    HTREEITEM hChild, hSibling;

    if ((hChild = GetChildItem(hItem)) != NULL)
    {
        // kein Ende-Item
        if (RemoveItemAndChilds(hChild, TRUE) == FALSE)
            return FALSE;
    }
    if (bRemoveSibling == TRUE)
    {
        if ((hSibling = GetNextSiblingItem(hItem)) != NULL)
        {
            // ein Item in der gleichen Zeile löschen
            if (RemoveItemAndChilds(hSibling, TRUE) == FALSE)
                return FALSE;
        }
    }
    // sich selber löschen
    return DeleteItem(hItem);
}

void CZRTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// Klick mit linker Maustaste
	*pResult = 0;
}

void CZRTreeCtrl::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


// Function name	: CZRTreeCtrl::QueryPossibleActions
// Description	    : ermittelt, welche Aktionen für hItem und pNode möglich sind
// Return type		: void 
// Argument         : HTREEITEM hItem   
// Argument         : CNode *pNode
// Argument         : BOOL &bNew
// Argument         : BOOL &bChange
// Argument         : BOOL &bDelete
void CZRTreeCtrl::QueryPossibleActions(HTREEITEM hItem, BOOL &bNew, BOOL &bChange, BOOL &bDelete)
{
    if (hItem == NULL)
    {
        bNew = TRUE;        // New == neuer Baum
        bChange = bDelete = FALSE;
        return;
    }
    CNode *pNode = (CNode*)GetItemData(hItem);

    if (pNode == NULL)
    {
        // muß Anfangsitem sein: disablen von Ändern und Löschen
        bNew = TRUE;
        bChange = bDelete = FALSE;
        return;
    }

    pNode->QueryAllowedActions(pNode, bNew, bChange, bDelete);
    if (IsPropertyValue(hItem))
    {
        // Eigenschaftswert: alles zulassen
        // Position im Baum ermitteln
        HTREEITEM hAct = hItem;
        int i = 0;
        while ((hAct = GetPrevSiblingItem(hAct)) != NULL)
        {
            i++;
        }
        CNode* pChild = pNode->GetChildAt(i);
        if (pChild == NULL || pChild->GetProperty().m_Property != VOLATILE)
        {
            // Child hat schon festgelegten Wert->kein Neu
            bNew = FALSE;
        }
        else
            bNew = TRUE;
        // bChange erlauben
        bChange = TRUE;
    }
}

void CZRTreeCtrl::PostNcDestroy() 
{	
	CTreeCtrl::PostNcDestroy();
}


// Function name	: CZRTreeCtrl::NewTree
// Description	    : erzeugt eine neue Anfangsnode
// Return type		: void 
void CZRTreeCtrl::NewTree()
{
    CString strValue;
    TVINSERTSTRUCT item;
    HTREEITEM hNew;
    int iCountStr;
    char** pStr;

    if (m_pMan != NULL)
        return;     // Fehler: ich weiß nicht, welcher Baum neu sein soll

    if (*m_ppRootNode != NULL)
        return;     // Fehler: nur beim leerem Baum möglich

    *m_ppRootNode = new CNode;
    if (*m_ppRootNode == NULL)
        return;

    // neue Eigenschaft
	item.hParent = NULL;
	item.hInsertAfter = TVI_FIRST;
    ((CNode*)*m_ppRootNode)->GetProperty().GetPropertyStrings(iCountStr, &pStr);
    // Property auswählen: einfach die erste
    strValue = pStr[0];
	item.item.pszText = strValue.GetBuffer(strValue.GetLength());
    strValue.ReleaseBuffer();
    for (int x=0; x<ZR_MAX_WERTE; x++)
    {
        if (strcmp(g_arrEigenschaften[x].strName, strValue) == 0)
        {
            // x entspricht Index in Imagelist
            break;
        }
    }
	item.item.iImage = x;
	item.item.iSelectedImage = item.item.iImage + 0;
	item.item.lParam = (LPARAM)*m_ppRootNode;
    item.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	hNew = InsertItem(&item);
    SelectItem(hNew);

    // neue Node mit Wert vorbelegen
    eTTEigenschaften eE;
    ((CNode*)*m_ppRootNode)->GetProperty().ConvertPropertyStrToEnum(strValue, eE);
    ((CNode*)*m_ppRootNode)->SetProperty(eE, strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    OnZrPropValueChange();
    SetChange();
}

void CZRTreeCtrl::SetChange()
{
    m_bChanged = TRUE;
    CWnd* pParent = GetParent();
    if (pParent)
    {
        pParent->SendMessage(WM_PARENT_CTRL_CHANGED, 0, 0);
    }
}


void CZRTreeCtrl::ReturnWasPressed()
{
    CEdit* pEdit = GetEditControl();
    if (pEdit != NULL)
    {
        // nur wenn begin edit
        pEdit->SendMessage(WM_CHAR, (WPARAM)'\n', 0);
    }
}

BOOL CZRTreeCtrl::FindAndSelectItem(CNode *pNode, BOOL bCollapseAll)
{
    TVITEM item;


    HTREEITEM hItem = GetRootItem();
    if (bCollapseAll == TRUE)
        ExpandItems(hItem, TVE_COLLAPSE);
    item.hItem = hItem;
    item.lParam = (LPARAM)pNode;
    item.mask = TVIF_PARAM|TVIF_HANDLE;
    if (GetItem(&item) == TRUE)
    {
        if (item.lParam == (LPARAM)pNode)
        {
            // Item gefunden
            SelectItem(item.hItem);
            EnsureVisible(item.hItem);
            return TRUE;
        }
    }

    // weiter suchen
    if ((hItem = FindItem(GetChildItem(hItem), pNode)) != NULL)
    {
        // gefunden
        SelectItem(hItem);
        EnsureVisible(hItem);
        return TRUE;
    }
    // Select root, um Irrtum auszuschließen
    hItem = GetRootItem();
    if (hItem != NULL)
    {
        SelectItem(hItem);
        EnsureVisible(hItem);
    }
    return FALSE;
}

HTREEITEM CZRTreeCtrl::FindItem(HTREEITEM hItem, CNode* pNode)
{
    TVITEM item;

    // erst sich selbst abfragen
    item.hItem = hItem;
    item.lParam = (LPARAM)pNode;
    item.mask = TVIF_PARAM|TVIF_HANDLE;
    if (GetItem(&item) == TRUE)
    {
        if (item.lParam == (LPARAM)pNode)
        {
            // Item gefunden
            return item.hItem;
        }
    }        

    // nicht gefunden, Child abfragen
    HTREEITEM hChild;
    hChild = GetChildItem(hItem);
    if (hChild != NULL)
    {
        if ((hChild = FindItem(hChild, pNode)) != NULL)
        {
            // gefunden 
            return hChild;
        }
    }

    // nicht gefunden, Nachbarn abfragen
    while (hItem != NULL)
    {
        hItem = GetNextSiblingItem(hItem);
        if (hItem != NULL)
        {
            if ((hChild = FindItem(hItem, pNode)) != NULL)
            {
                // gefunden
                return hChild;
            }
        }
    }

    // nichts gefunden
    return NULL;
}

BOOL CZRTreeCtrl::ExpandItems(HTREEITEM hItem, UINT nCode)
{
    HTREEITEM hChild, hTmp;
    hChild = GetChildItem(hItem);
    if (hChild != NULL)
    {
        ExpandItems(hChild, nCode);
    }

    // jetz Nachbarn
    hTmp = hItem;
    while (hTmp != NULL)
    {
        hTmp = GetNextSiblingItem(hTmp);
        if (hTmp != NULL)
        {
            ExpandItems(hTmp, nCode);
        }
    }

    // selber
    return Expand(hItem, nCode);
}

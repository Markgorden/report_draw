// CntrItem.cpp : implementation of the CDrawCntrItem class
//

#include "stdafx.h"
#include "Draw.h"

#include "DrawDoc.h"
#include "NetworkView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawCntrItem implementation

IMPLEMENT_SERIAL(CDrawCntrItem, COleClientItem, 0)

CDrawCntrItem::CDrawCntrItem(CDrawDoc* pContainer ,CDrawOleObj* pDrawObj)
: COleClientItem(pContainer)
{
	m_pDrawObj = pDrawObj;
}

CDrawCntrItem::~CDrawCntrItem()
{
	if (m_pDrawObj != NULL)
		m_pDrawObj->m_pClientItem = NULL;
}

void CDrawCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	switch(nCode)
	{
	case OLE_CHANGED_STATE:
	case OLE_CHANGED_ASPECT:
		m_pDrawObj->Invalidate();
		break;
	case OLE_CHANGED:
		UpdateExtent(); // extent may have changed
		m_pDrawObj->Invalidate();
		break;
	}
}

BOOL CDrawCntrItem::OnChangeItemPosition(const CRect& rectPos)
{

	ASSERT_VALID(this);

	CNetworkView* pView = GetActiveView();
	ASSERT_VALID(pView);
	CRect rect = rectPos;
	pView->ClientToDoc(rect);

	if (rect != m_pDrawObj->m_position)
	{
		// invalidate old rectangle
		m_pDrawObj->Invalidate();

		// update to new rectangle
		m_pDrawObj->m_position = rect;
		GetExtent(&m_pDrawObj->m_extent);

		// and invalidate new rectangle
		m_pDrawObj->Invalidate();

		// mark document as dirty
		GetDocument()->SetModifiedFlag();
	}
	return COleClientItem::OnChangeItemPosition(rectPos);
}

void CDrawCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// update to extent of item if m_position is not initialized
	if (m_pDrawObj->m_position.IsRectEmpty())
		UpdateExtent();

	// copy m_position, which is in document coordinates
	CNetworkView* pView = GetActiveView();
	ASSERT_VALID(pView);
	rPosition = m_pDrawObj->m_position;
	pView->DocToClient(rPosition);

}

void CDrawCntrItem::OnActivate()
{
    // Allow only one inplace activate item per frame
    CNetworkView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CDrawCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CDrawCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class first to read in COleClientItem data.
	// Since this sets up the m_pDocument pointer returned from
	//  CDrawCntrItem::GetDocument, it is a good idea to call
	//  the base class Serialize first.
	COleClientItem::Serialize(ar);

	// now store/retrieve data specific to CDrawCntrItem
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawCntrItem diagnostics

#ifdef _DEBUG
void CDrawCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CDrawCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

BOOL CDrawCntrItem::UpdateExtent()
{
	CSize size;
	if (!GetExtent(&size) || size == m_pDrawObj->m_extent)
		return FALSE;       // blank

	// if new object (i.e. m_extent is empty) setup position
	if (m_pDrawObj->m_extent == CSize(0, 0))
	{
		m_pDrawObj->m_position.right =
			m_pDrawObj->m_position.left + MulDiv(size.cx, 10, 254);
		m_pDrawObj->m_position.bottom =
			m_pDrawObj->m_position.top - MulDiv(size.cy, 10, 254);
	}
	// else data changed so scale up rect as well
	else if (!IsInPlaceActive() && size != m_pDrawObj->m_extent)
	{
		m_pDrawObj->m_position.right = m_pDrawObj->m_position.left +
			MulDiv(m_pDrawObj->m_position.Width(), size.cx, m_pDrawObj->m_extent.cx);
		m_pDrawObj->m_position.bottom = m_pDrawObj->m_position.top +
			MulDiv(m_pDrawObj->m_position.Height(), size.cy, m_pDrawObj->m_extent.cy);
	}

	m_pDrawObj->m_extent = size;
	m_pDrawObj->Invalidate();   // redraw to the new size/position
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

// LineList1.cpp : implementation file
//

#include "stdafx.h"
#include "Draw.h"
#include "LineList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineList
#define COLOR_ITEM_HEIGHT   15

CLineList::CLineList()
{
}

CLineList::~CLineList()
{
}

void CLineList::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
	lpMIS->itemHeight = COLOR_ITEM_HEIGHT;
}

void CLineList::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COLORREF cr = (COLORREF)lpDIS->itemData; // RGB in item data

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		// Paint the color item in the color requested
		CBrush br(cr);
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		COLORREF crHilite = RGB(255-GetRValue(cr),
						255-GetGValue(cr), 255-GetBValue(cr));
		CBrush br(crHilite);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		CBrush br(cr);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}
}

void CLineList::AddColorItem(COLORREF color)
{
	// add a listbox item
	AddString((LPCTSTR) color);
		// Listbox does not have the LBS_HASSTRINGS style, so the
		//  normal listbox string is used to store an RGB color
}

int CLineList::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	COLORREF cr1 = (COLORREF)lpCIS->itemData1;
	COLORREF cr2 = (COLORREF)lpCIS->itemData2;
	if (cr1 == cr2)
		return 0;       // exact match

	// first do an intensity sort, lower intensities go first
	int intensity1 = GetRValue(cr1) + GetGValue(cr1) + GetBValue(cr1);
	int intensity2 = GetRValue(cr2) + GetGValue(cr2) + GetBValue(cr2);
	if (intensity1 < intensity2)
		return -1;      // lower intensity goes first
	else if (intensity1 > intensity2)
		return 1;       // higher intensity goes second

	// if same intensity, sort by color (blues first, reds last)
	if (GetBValue(cr1) > GetBValue(cr2))
		return -1;
	else if (GetGValue(cr1) > GetGValue(cr2))
		return -1;
	else if (GetRValue(cr1) > GetRValue(cr2))
		return -1;
	else
		return 1;
}

BEGIN_MESSAGE_MAP(CLineList, CComboBox)
	//{{AFX_MSG_MAP(CLineList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineList message handlers

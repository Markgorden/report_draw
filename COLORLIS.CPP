// colorlis.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "Draw.h"
#include "colorlis.h"
#include "line.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineWidthMenu


CLineWidthMenu::CLineWidthMenu()
{
	VERIFY(CreatePopupMenu());
	ASSERT(GetMenuItemCount()==0);
	for (int i=0; i<=8;i++)
		VERIFY(AppendMenu(MF_OWNERDRAW, ID_LINEWIDTH0+i, (LPCTSTR)(ID_LINEWIDTH0+i)));
}

void CLineWidthMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpDIS->itemID;
	ASSERT(id == lpDIS->itemData);
	ASSERT(id >= ID_LINEWIDTH0);
	ASSERT(id <= ID_LINEWIDTH8);
	CDC dc;
	dc.Attach(lpDIS->hDC);

	CRect rc(lpDIS->rcItem);
	ASSERT(rc.Width() < 500);
	if (lpDIS->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&rc);

	COLORREF cr = (lpDIS->itemState & ODS_SELECTED) ?
		::GetSysColor(COLOR_HIGHLIGHT) :
		dc.GetBkColor();

	CBrush brushFill(cr);
	cr = dc.GetTextColor();

	if (lpDIS->itemState & ODS_SELECTED)
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

	int nBkMode = dc.SetBkMode(TRANSPARENT);
	dc.FillRect(&rc, &brushFill);

	rc.left += 50;
	CString str;
	str.LoadString(id);
	dc.TextOut(rc.left,rc.top,str,str.GetLength());
	rc.left -= 45;
	rc.top += 2;
	rc.bottom -= 2;
	rc.right = rc.left + 40;
	COLORREF cl = ::GetSysColor(COLOR_WINDOWTEXT);
	CBrush brush(cl);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CRect lrc = rc; //rect as a line
	int mid=(lrc.bottom+lrc.top)/2;
	if (((id-ID_LINEWIDTH0+1)/2)*2==id-ID_LINEWIDTH0+1){//even
		lrc.top = mid-(id-ID_LINEWIDTH0+1)/2;
		lrc.bottom = mid+(id-ID_LINEWIDTH0+1)/2;
	}
	else{ //odd
		lrc.top = mid-(id-ID_LINEWIDTH0+1)/2-1;
		lrc.bottom = mid+(id-ID_LINEWIDTH0+1)/2;
	}
	lrc.bottom--;
	dc.Rectangle(lrc);

	dc.SelectObject(pOldBrush);
	dc.SetTextColor(cr);
	dc.SetBkMode(nBkMode);

	dc.Detach();
}

void CLineWidthMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpMIS->itemID;
	ASSERT(id == lpMIS->itemData);
	ASSERT(id >= ID_LINEWIDTH0);
	ASSERT(id <= ID_LINEWIDTH8);
	CDisplayIC dc;
	CString str;
	str.LoadString(id);
	CSize sizeText = dc.GetTextExtent(str,str.GetLength());
	ASSERT(sizeText.cx < 500);
	lpMIS->itemWidth = sizeText.cx + 50;
	lpMIS->itemHeight = sizeText.cy;
}


/////////////////////////////////////////////////////////////////////////////
// CLineStyleMenu


CLineStyleMenu::CLineStyleMenu()
{
	VERIFY(CreatePopupMenu());
	ASSERT(GetMenuItemCount()==0);
	for (int i=0; i<=4;i++)
		VERIFY(AppendMenu(MF_OWNERDRAW, ID_LINESTYLE0+i, (LPCTSTR)(ID_LINESTYLE0+i)));
}


void CLineStyleMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpDIS->itemID;
	ASSERT(id == lpDIS->itemData);
	ASSERT(id >= ID_LINESTYLE0);
	ASSERT(id <= ID_LINESTYLE8);

    CDC*    pDC     = CDC::FromHandle(lpDIS->hDC);
    CRect   rect    = lpDIS->rcItem;
    UINT    state   = lpDIS->itemState;

	ASSERT(rect.Width() < 500);
	if (state & ODS_FOCUS)
		pDC->DrawFocusRect(&rect);

	COLORREF cr = (lpDIS->itemState & ODS_SELECTED) ?
		::GetSysColor(COLOR_HIGHLIGHT) :
		pDC->GetBkColor();

	CBrush brushFill(cr);

	int nBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(&rect, &brushFill);


//center the Line
    rect.DeflateRect(2, 0);
	Graphics graphics(pDC->m_hDC);
	Pen pen(Color(255,0,0,0), 3);

	pen.SetDashStyle((DashStyle)(id-ID_LINESTYLE0));
	pen.SetDashCap(DashCapFlat);
	graphics.DrawLine(&pen, rect.TopLeft().x, (rect.TopLeft().y+rect.BottomRight().y)/2,rect.BottomRight().x,(rect.TopLeft().y+rect.BottomRight().y)/2);
}

void CLineStyleMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpMIS->itemID;
	ASSERT(id == lpMIS->itemData);
	ASSERT(id >= ID_LINESTYLE0);
	ASSERT(id <= ID_LINESTYLE8);
	CDisplayIC dc;
	CString str;
	str.LoadString(id);
	CSize sizeText = dc.GetTextExtent(str,str.GetLength());
	ASSERT(sizeText.cx < 500);
	lpMIS->itemWidth = sizeText.cx + 50;
	lpMIS->itemHeight = sizeText.cy;
}

/////////////////////////////////////////////////////////////////////////////
// CLeftArrowMenu


CLeftArrowMenu::CLeftArrowMenu()
{
	VERIFY(CreatePopupMenu());
	ASSERT(GetMenuItemCount()==0);
	for (int i=0; i<=5;i++)
		VERIFY(AppendMenu(MF_OWNERDRAW, ID_LEFTARROW0+i, (LPCTSTR)(ID_LEFTARROW0+i)));
}

void CLeftArrowMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpDIS->itemID;
	ASSERT(id == lpDIS->itemData);
	ASSERT(id >= ID_LEFTARROW0);
	ASSERT(id <= ID_LEFTARROW5);
	CDC dc;
	dc.Attach(lpDIS->hDC);

	CRect rc(lpDIS->rcItem);
	ASSERT(rc.Width() < 500);
	if (lpDIS->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&rc);

	COLORREF cr = (lpDIS->itemState & ODS_SELECTED) ?
		::GetSysColor(COLOR_HIGHLIGHT) :
		dc.GetBkColor();

	CBrush brushFill(cr);
	cr = dc.GetTextColor();

	if (lpDIS->itemState & ODS_SELECTED)
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

	int nBkMode = dc.SetBkMode(TRANSPARENT);
	dc.FillRect(&rc, &brushFill);

	//rc.left += 50;
	CString str;
	str.LoadString(id);
	//dc.TextOut(rc.left,rc.top,str,str.GetLength());
	//rc.left -= 45;
	COLORREF cl = ::GetSysColor(COLOR_WINDOWTEXT);
	CBrush brush(cl);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CPen pen;
	if (!pen.CreatePen(PS_SOLID, 1, RGB(0,0,0)))
		return;
	CPen *pOldPen = dc.SelectObject(&pen);

	rc.left += 3;
//	rc.right = rc.left + 63;
	POINT p1,p2;
	p1.x = rc.left;
	p1.y = p2.y = (rc.bottom+rc.top)/2;
	p2.x = p1.x+10;
	//gDrawArrow(id-ID_LEFTARROW0,p1, p2,5,&dc, 10,0,10,brush);
	gDrawArrowHead(id-ID_LEFTARROW0,p1, p2,5, &dc,brush);
	dc.SelectObject(pOldPen);
	//dc.Rectangle(lrc);
	dc.MoveTo(rc.left+63,p2.y);
	dc.LineTo(rc.left,p2.y);


	dc.SelectObject(pOldBrush);
	dc.SetTextColor(cr);
	dc.SetBkMode(nBkMode);

	dc.Detach();
}

void CLeftArrowMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpMIS->itemID;
	ASSERT(id == lpMIS->itemData);
	ASSERT(id >= ID_LEFTARROW0);
	ASSERT(id <= ID_LEFTARROW5);
	CDisplayIC dc;
	CString str;
	str.LoadString(id);
	CSize sizeText = dc.GetTextExtent(str,str.GetLength());
	ASSERT(sizeText.cx < 500);
	lpMIS->itemWidth = sizeText.cx + 50;
	lpMIS->itemHeight = sizeText.cy;
}

/////////////////////////////////////////////////////////////////////////////
// CRightArrowMenu


CRightArrowMenu::CRightArrowMenu()
{
	VERIFY(CreatePopupMenu());
	ASSERT(GetMenuItemCount()==0);
	for (int i=0; i<=5;i++)
		VERIFY(AppendMenu(MF_OWNERDRAW, ID_RIGHTARROW0+i, (LPCTSTR)(ID_RIGHTARROW0+i)));
}


void CRightArrowMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpDIS->itemID;
	ASSERT(id == lpDIS->itemData);
	ASSERT(id >= ID_RIGHTARROW0);
	ASSERT(id <= ID_RIGHTARROW5);
	CDC dc;
	dc.Attach(lpDIS->hDC);

	CRect rc(lpDIS->rcItem);
	ASSERT(rc.Width() < 500);
	if (lpDIS->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&rc);

	COLORREF cr = (lpDIS->itemState & ODS_SELECTED) ?
		::GetSysColor(COLOR_HIGHLIGHT) :
		dc.GetBkColor();

	CBrush brushFill(cr);
	cr = dc.GetTextColor();

	if (lpDIS->itemState & ODS_SELECTED)
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

	int nBkMode = dc.SetBkMode(TRANSPARENT);
	dc.FillRect(&rc, &brushFill);

	//rc.left += 50;
	CString str;
	str.LoadString(id);
	//dc.TextOut(rc.left,rc.top,str,str.GetLength());
	//rc.left -= 45;
	COLORREF cl = ::GetSysColor(COLOR_WINDOWTEXT);
	CBrush brush(cl);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CPen pen;
	if (!pen.CreatePen(PS_SOLID, 1, RGB(0,0,0)))
		return;
	CPen *pOldPen = dc.SelectObject(&pen);

	rc.left += 3;
//	rc.right = rc.left + 63;
	POINT p1,p2;
	p1.x = rc.left+63;
	p1.y = p2.y = (rc.bottom+rc.top)/2;
	p2.x = p1.x-10;
	//gDrawArrow(id-ID_RIGHTARROW0,p1, p2,5,&dc, 10,0,10,brush);
	gDrawArrowHead(id-ID_RIGHTARROW0,p1, p2,5, &dc,brush);
	dc.SelectObject(pOldPen);
	//dc.Rectangle(lrc);
	dc.MoveTo(p1.x,p2.y);
	dc.LineTo(rc.left,p2.y);


	dc.SelectObject(pOldBrush);
	dc.SetTextColor(cr);
	dc.SetBkMode(nBkMode);

	dc.Detach();
}

void CRightArrowMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_MENU);
	UINT id = (UINT)(WORD)lpMIS->itemID;
	ASSERT(id == lpMIS->itemData);
	ASSERT(id >= ID_RIGHTARROW0);
	ASSERT(id <= ID_RIGHTARROW5);
	CDisplayIC dc;
	CString str;
	str.LoadString(id);
	CSize sizeText = dc.GetTextExtent(str,str.GetLength());
	ASSERT(sizeText.cx < 500);
	lpMIS->itemWidth = sizeText.cx + 50;
	lpMIS->itemHeight = sizeText.cy;
}

/*Global Function that draw arrow in pDC
  precondition, Brush and Pen are already setup

				  *		   *	
				 **  	   **
			(p1)***========***(p2)
				 **		   **
				  *		   *
  */
void gDrawArrow(int lArrowType, int rArrowType, int lArrowSize, int rArrowSize,
						  POINT  p1, POINT p2, CDC *pDC, CDashLine& dashline, CBrush& brush)
{
	//draw the line first;
	int arrowlength=2*lArrowSize;  //arrowlength is the start arrow length
	int arrowlength1=2*rArrowSize; //arrowlength1 is the end arrow length
	int dx=0,dy=0,dx1=0,dy1=0;
	POINT p3, p4;

	long r2=(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
	double l=sqrt((double)r2);
    if (l>0)
	{
		dy=(int)((arrowlength*(p2.y-p1.y))/l);
		dx=(int)((arrowlength*(p2.x-p1.x))/l);
		dy1=(int)((arrowlength1*(p2.y-p1.y))/l);
		dx1=(int)((arrowlength1*(p2.x-p1.x))/l);
	}
	else
		return; //don't draw it, if head and tail are on the same point

	p3=p1;
	p4=p2;
	if (rArrowType){ 
		p4.y -= dy1;
		p4.x -= dx1;
	}

	if (lArrowType){
		p3.y +=dy;
		p3.x +=dx;
	}

		
	dashline.MoveTo(p3);
	dashline.LineTo(p4);
	//draw left arrow
	gDrawArrowHead( lArrowType,p1,p3,lArrowSize,pDC, brush);
	//draw right arrow
	gDrawArrowHead( rArrowType,p2,p4,rArrowSize,pDC, brush);

}

/*Global Function that draw arrow head in pDC
  precondition, Bruch and Pen are already setup */

void gDrawArrowHead(int arrowType, 
						  POINT  p1, POINT p2, int arrowSize ,CDC *pDC, CBrush& brush)
{
	int	dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	int arrowlength = (int) pow((dx*dx+dy*dy),0.5);//|p1 - p2|;
	POINT points[4];
	CRgn rgn;
	switch (arrowType){
	case 0:
		break;
	case 1:
        //actually a triagle
		points[0]=p1;
		points[1]=points[2]=p2;
		points[1].x+=arrowSize*dy/arrowlength;
		points[1].y-=arrowSize*dx/arrowlength;
		points[2].x-=arrowSize*dy/arrowlength;
		points[2].y+=arrowSize*dx/arrowlength;
		rgn.CreatePolygonRgn(points, 3, ALTERNATE);
		pDC->FillRgn(&rgn,&brush);
		pDC->Polygon(points,3);
		break;
	case 2:
		points[0]=p1;
		points[1]=points[2]=points[3]=p2;
		points[1].x+=arrowSize*dy/arrowlength;
		points[1].y-=arrowSize*dx/arrowlength;
		points[2].x-=arrowSize*dy/arrowlength;
		points[2].y+=arrowSize*dx/arrowlength;
        pDC->MoveTo(points[1]);
		pDC->LineTo(points[0]);
		pDC->LineTo(points[2]);
        pDC->MoveTo(points[0]);
		pDC->LineTo(points[3]);
		break;
	case 3:
		points[0]=p1;
		points[1]=points[2]=points[3]=p2;
		points[1].x-=(p1.x-p2.x);
		points[1].y-=(p1.y-p2.y);
		points[3]=points[1];
		points[1].x+=arrowSize*dy/arrowlength;
		points[1].y-=arrowSize*dx/arrowlength;
		points[3].x-=arrowSize*dy/arrowlength;
		points[3].y+=arrowSize*dx/arrowlength;
		rgn.CreatePolygonRgn(points, 4, ALTERNATE);
        pDC->FillRgn(&rgn,&brush);
		pDC->Polygon(points,4);
		break;
	case 4:
		points[0]=p1;
		points[2]=p2;
		points[1].x=points[3].x=(p1.x+p2.x)/2;
		points[1].y=points[3].y=(p1.y+p2.y)/2;
		//points[2].x-=(p1.x-p2.x);
		//points[2].y-=(p1.y-p2.y);
		points[1].x+=(long)(.5*arrowSize*dy/arrowlength);
		points[1].y-=(long)(.5*arrowSize*dx/arrowlength);
		points[3].x-=(long)(.5*arrowSize*dy/arrowlength);
		points[3].y+=(long)(.5*arrowSize*dx/arrowlength);
		rgn.CreatePolygonRgn(points, 4, ALTERNATE);
        pDC->FillRgn(&rgn,&brush);
		pDC->Polygon(points,4);
		break;
	case 5:
		POINT p;
		p.x=(p1.x+p2.x)/2;
		p.y=(p1.y+p2.y)/2;
        long r = 0;//(long)sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))/2+1;
		CRect circleRect;
		circleRect.left=p.x-r;
		circleRect.right=p.x+r;
		circleRect.bottom=p.y+r;
		circleRect.top=p.y-r;
		pDC->Ellipse(&circleRect);
		break;
	}
}
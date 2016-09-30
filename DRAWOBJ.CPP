// drawobj.cpp - implementation for drawing objects
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

#include "networkview.h"
#include "drawobj.h"
#include "drawtool.h"

#include "DrawProperty.h"
#include "LineProperty.h"
#include "TextProperty.h"
#include "colorlis.h"
#include "textformatpage.h"
#include "maction.h"
#include "Bezier.h"
#include "spline.h"
#include <math.h>
#include <memory.h>
#include <stack>
#include "Drawdoc.h"

typedef std::stack<int> CPointStack;

#include "line.h"
#include "cntritem.h"

#define PADDING 2

extern int nDragHandle;

void Normallize(Rect &rect){
//Ensure positive width and height
	if (rect.Width<0) {
		rect.X +=rect.Width;
		rect.Width = -rect.Width;
	}
	if (rect.Height<0) {
		rect.Y+=rect.Height;
		rect.Height = -rect.Height;
	}
}

void Normallize(RectF &rect){
//Ensure positive width and height
	if (rect.Width<0) {
		rect.X +=rect.Width;
		rect.Width = -rect.Width;
	}
	if (rect.Height<0) {
		rect.Y+=rect.Height;
		rect.Height = -rect.Height;
	}
}

IMPLEMENT_SERIAL(CDrawObj, CObject, 0)

CDrawObj::CDrawObj()
{
	m_nPage = 1;
	m_dashStyle = DashStyleSolid;
	m_dashCap = DashCapRound ;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	m_brushType = BrushTypeLinearGradient;
	m_penType = PenTypeSolidColor ;
	m_Transparency = 0; //for alpha blending;
	m_brushColor = Color(255,0xc0,255,255);
	m_gradientColor = Color(255,255,255,100);
	m_penColor= Color(255,0,0,0);
	m_penWidth =2;
	m_linearGradientMode = LinearGradientModeForwardDiagonal ; //for linear gradient only;
	m_hatchStyle = HatchStyleHorizontal;
	m_startArrow = OpenAnchor;
	m_endArrow =   DimondAnchor;
	m_startArrowSize = 2;
	m_endArrowSize = 4;
	m_nStretch = 0;
	m_bToCenter = 0;
	m_penbrushType = BrushTypeSolidColor;
}

CDrawObj::~CDrawObj()
{
}

CDrawObj::CDrawObj(const CRect& position)
{
	m_position = position;
	m_pDocument = NULL;


	m_dashStyle = DashStyleSolid;
	m_dashCap = DashCapRound ;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	m_brushType = BrushTypeLinearGradient;
	m_penType = PenTypeSolidColor ;
	m_Transparency = 0; //for alpha blending;
	m_brushColor = Color(255,0xC0,255,255);
	m_gradientColor = Color(255,255,255,100);
	m_penColor= Color(255,0,0,0);
	m_penWidth =1;
	m_linearGradientMode = LinearGradientModeForwardDiagonal ; //for linear gradient only;
	m_hatchStyle = HatchStyleHorizontal;
	m_startArrow = OpenAnchor;
	m_endArrow =   DimondAnchor;
	m_startArrowSize = 2;
	m_endArrowSize = 4;
	m_nStretch = 0;
	m_bToCenter = 0;
	m_penbrushType = BrushTypeSolidColor;
	m_state = 0x7;
}

void CDrawObj::Initialize()
{
   //do nothing except for CDrawReaction and CDrawNode
}

void CDrawObj::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_position;
		ar << m_state;
		ar<< (BYTE)m_dashStyle;
		ar<< (BYTE)m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
		ar<< (BYTE)m_brushType;
		ar<< (BYTE)m_penType;
		ar<< m_Transparency; //for alpha blending;
		ar<< m_brushColor; //when hatch pattern used, this is foreColor
		ar<< m_gradientColor;
		ar<< m_penColor;
		ar<< m_penEndColor;
		ar<< (BYTE)m_penWidth;
		ar<< (BYTE)m_linearGradientMode; //for linear gradient only;
		ar<< (BYTE)m_hatchStyle;
		ar<< (BYTE)m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
		ar<< (BYTE)m_endArrowSize;
		ar<< (BYTE)m_startArrow,
		ar<< (BYTE)m_endArrow;
		ar<< m_fillImage;
		ar<< (BYTE)m_wrapMode;
		ar<< m_bToCenter; //0, - center path gradient.
		ar<< m_nStretch;
		ar<< (BYTE)m_penbrushType;
		ar<< (BYTE)m_penlinearGradientMode;
		ar<< (BYTE)m_Transparency;
		ar<<m_nPage;
	}
	else
	{
		m_pDocument = (CDrawDoc*)ar.m_pDocument;

		ar >> m_position;
		ar >> m_state;
		BYTE temp;
		ar>> temp; 		m_dashStyle = (DashStyle) temp;
		ar>> temp;		m_dashCap = (DashCap)temp;
		ar>> temp;		m_brushType = (BrushType) temp;
		ar>> temp;		m_penType = (PenType) temp;
		ar>> m_Transparency;
		ar>> m_brushColor; //when hatch pattern used, this is foreColor
		ar>> m_gradientColor;
		ar>> m_penColor;
		ar>> m_penEndColor;
		ar>> m_penWidth;
		ar>> temp;		m_linearGradientMode = (LinearGradientMode)temp;
		ar>> temp;		m_hatchStyle = (HatchStyle)temp;
		ar>> m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
		ar>> m_endArrowSize;
		ar>> temp;		m_startArrow = (ArrowTypeG)temp;
		ar>> temp;		m_endArrow = (ArrowTypeG)temp;
		ar>> m_fillImage;
		ar>> temp;		m_wrapMode = (WrapMode) temp;
		ar>> m_bToCenter; //0, - center path gradient.
		ar>> m_nStretch;
		ar>> temp;		m_penbrushType = (BrushType)temp;
		ar>> temp;		m_penlinearGradientMode = (LinearGradientMode)temp;
		ar>> temp;		m_Transparency = (LinearGradientMode)temp;
		ar>>m_nPage;
	}
}

void CDrawObj::Remove()
{
	delete this;
}

void CDrawObj::Draw(CDC*)
{
}

void CDrawObj::DrawTracker(CDC* pDC, TrackerState state)
{
	ASSERT_VALID(this);

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			int nHandleCount = GetHandleCount();
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CPoint handle = GetHandle(nHandle);
				pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
			}
		}
		break;
	}
}

/* Set the arrow anchors based m_startArrow, m_endArrow;
   because the size of anchor for predefined anchor type in GDI+, such as LineCapArrowAnchor. 
   so it is better to use LinCapCustom. and keep all the anchors as static memeber of CDrawObj;

    Type =0. startArrow and endArrow used;
	Type =1. startArrow used;
	Type =2. endArrow used;

    also decide the size of arrow by, m_startArrowSize;
*/
void CDrawObj::SetArrowAnchors(Pen *pPen, int Type = 0)
{
	if (Type >= 3){
			pPen->SetStartCap(LineCapNoAnchor);
			pPen->SetEndCap(LineCapNoAnchor);
		return;
	}
	LineCap startLineCap= LineCapCustom;
	LineCap endLineCap= LineCapCustom;
	//startLineCap 

	if (Type == 1) 
		endLineCap = LineCapNoAnchor;
	else if (Type ==2)
		startLineCap = LineCapNoAnchor;


	GraphicsPath path1,
				 path2;

	int inset1 = 0,
		inset2 = 0;

	Matrix matrix;
	matrix.Rotate(90);

//set start cap
	switch (m_startArrow) {
	case SquareAnchor:
		path1.AddRectangle(Rect(-1, -1, 2, 2));
		break;
	case RoundAnchor:
		path1.AddEllipse(Rect(-1, -1, 2, 2));
		break;
	case ArrowAnchor:
		{
		Point pts1[] = { Point(-1, -1),
					  Point(1, 0),
					  Point(-1, 1)};

		path1.AddPolygon(pts1,3);
		break;
		}
	case DimondAnchor:
		{
		Point pts2[] = { Point(-2, 0),
					  Point(0, -1),
					  Point(2, 0),
					  Point(0, 1)};		

		path1.AddPolygon(pts2,4);
		break;
		}
	case StealthAnchor:
		{
		Point  pts3[] = { Point(-1, 0),
					  Point(-2, 2),
					  Point(2, 0),
					  Point(-2, -2)};		

		path1.AddPolygon(pts3,4);
		inset1 = 1;
		break;
		}
	case OpenAnchor:
		{
		Point  pts4[] = { Point(-2, -2),
						Point(2, 0),
						Point(-2, 2),
						Point(2,0)};	

		path1.AddPolygon(pts4,4);
		inset2=-1;
		break;
		}
	default:
		break;
	}

	if (m_startArrow != NoAnchor && m_startArrow != CustomAnchor && startLineCap==LineCapCustom ){
		path1.Transform(&matrix);
		if (m_startArrow == OpenAnchor){
			CustomLineCap custCap1(NULL,&path1);
			custCap1.SetWidthScale((float)m_startArrowSize/m_penWidth);
			custCap1.SetBaseInset(inset1);
			pPen->SetCustomStartCap(&custCap1);
		}
		else{
			CustomLineCap custCap1(&path1,NULL);
			custCap1.SetWidthScale((float)m_startArrowSize/m_penWidth);
			custCap1.SetBaseInset(inset1);
			pPen->SetCustomStartCap(&custCap1);
		}
	}
	else
		pPen->SetStartCap(LineCapNoAnchor);

//end cap
	switch (m_endArrow) {
	case SquareAnchor:
		path2.AddRectangle(Rect(-1, -1, 2, 2));
		break;
	case RoundAnchor:
		path2.AddEllipse(Rect(-1, -1, 2, 2));
		break;
	case ArrowAnchor:
		{
		Point pts1[] = { Point(-1, -1),
					  Point(1, 0),
					  Point(-1, 1)};

		path2.AddPolygon(pts1,3);
		break;
		}
	case DimondAnchor:
		{
		Point pts2[] = { Point(-2, 0),
					  Point(0, -1),
					  Point(2, 0),
					  Point(0, 1)};		

		path2.AddPolygon(pts2,4);
		break;
		}
	case StealthAnchor:
		{
		Point  pts3[] = { Point(-1, 0),
					  Point(-2, 2),
					  Point(2, 0),
					  Point(-2, -2)};		

		path2.AddPolygon(pts3,4);
		inset2 = 1;
		break;
		}
	case OpenAnchor:
		{
		Point  pts4[] = { Point(-2, -2),
						Point(2, 0),
						Point(-2, 2),	
						Point(2,0)};
		inset2=-1;

		path2.AddPolygon(pts4,4);
		break;
		}
	default:
		break;
	}

	if (m_endArrow != NoAnchor && m_endArrow != CustomAnchor && endLineCap==LineCapCustom ){
		path2.Transform(&matrix);
		if (m_endArrow == OpenAnchor){
			CustomLineCap custCap2(NULL, &path2);
			custCap2.SetWidthScale((float)m_endArrowSize/m_penWidth);
			custCap2.SetBaseInset(inset2);
			pPen->SetCustomEndCap(&custCap2);
		}
		else{
			CustomLineCap custCap2(&path2, NULL);
			custCap2.SetWidthScale((float)m_endArrowSize/m_penWidth);
			custCap2.SetBaseInset(inset2);
			pPen->SetCustomEndCap(&custCap2);
		}
	}
	else
		pPen->SetEndCap(LineCapNoAnchor);
}

// position is in logical
void CDrawObj::MoveTo(const CRect& position, CNetworkView* pView)
{
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	if (pView == NULL)
	{
		Invalidate();
		m_position = position;
		Invalidate();
	}
	else
	{
		pView->InvalObj(this);
		m_position = position;
		pView->InvalObj(this);
	}
}

// Note: if bSelected, hit-codes start at one for the top-left
// and increment clockwise, 0 means no hit.
// If !bSelected, 0 = no hit, 1 = hit (anywhere)

// point is in logical coordinates
int CDrawObj::HitTest(CPoint point, CNetworkView* pView, BOOL bSelected)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);
	if (m_nPage!=m_pDocument->m_nCurrentPage)
		return 0;

	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc = GetHandleRect(nHandle,pView);
			if (point.x >= rc.left && point.x < rc.right &&
				point.y <= rc.top && point.y > rc.bottom)
				return nHandle;
		}
	}
	else
	{
		if (point.x >= m_position.left && point.x < m_position.right &&
			point.y <= m_position.top && point.y > m_position.bottom)
			return 1;
	}
	return 0;
}

// rect must be in logical coordinates
BOOL CDrawObj::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

BOOL CDrawObj::InRect(const CRect& rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
    CRect unionRect;

	//when fixed has not width or height, UnionRect will ignore it.
	if (fixed.bottom==fixed.top)
		fixed.bottom++;
	if (fixed.right==fixed.left)
		fixed.right++;

	unionRect.UnionRect(fixed,rectT);
	return (unionRect==rectT);
}

int CDrawObj::GetHandleCount()
{
	ASSERT_VALID(this);
	return 8;
}

// returns logical coords of center of handle
CPoint CDrawObj::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	int x, y, xCenter, yCenter;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = m_position.left + m_position.Width() / 2;
	yCenter = m_position.top + m_position.Height() / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = m_position.left;
		y = m_position.top;
		break;

	case 2:
		x = xCenter;
		y = m_position.top;
		break;

	case 3:
		x = m_position.right;
		y = m_position.top;
		break;

	case 4:
		x = m_position.right;
		y = yCenter;
		break;

	case 5:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 6:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 7:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 8:
		x = m_position.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

// return rectange of handle in logical coords
CRect CDrawObj::GetHandleRect(int nHandleID, CNetworkView* pView)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	CRect rect;
	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
	pView->DocToClient(point);
	// return CRect of handle in device coords
	int r=pView->GetZoomRatio();
	r= (r>1)? r: 1;
	rect.SetRect(point.x-3*r, point.y-3*r, point.x+3*r, point.y+3*r);
	pView->ClientToDoc(rect);

	return rect;
}

HCURSOR CDrawObj::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

// point must be in logical
void CDrawObj::MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView)
{
	ASSERT_VALID(this);

	CRect position = m_position;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;
	}

	MoveTo(position, pView);
}
//shift the postion a little, used by copy and paste only!!
void CDrawObj::Shift(int x, int y)
{
	m_position.OffsetRect(x,y);
}

void CDrawObj::Invalidate()
{
	ASSERT_VALID(this);
	m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
}

CDrawObj* CDrawObj::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawObj* pClone = new CDrawObj(m_position);

	pClone->m_brushColor =m_brushColor;
	pClone->m_brushType =m_brushType;
	pClone->m_bToCenter = m_bToCenter;
	pClone->m_dashCap =m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	pClone->m_dashStyle = m_dashStyle;
	pClone->m_endArrow =m_endArrow;
	pClone->m_endArrowSize =m_endArrowSize;
	pClone->m_fillImage = m_fillImage;
	pClone->m_gradientColor =m_gradientColor;
	pClone->m_hatchStyle =m_hatchStyle;
	pClone->m_linearGradientMode =m_linearGradientMode; //for linear gradient only;
	pClone->m_nPage =m_nPage;
	pClone->m_nStretch =m_nStretch;
	pClone->m_penbrushType = m_penbrushType;
	pClone->m_penColor =m_penColor;
	pClone->m_penEndColor =m_penEndColor;
	pClone->m_penlinearGradientMode = m_penlinearGradientMode;
	pClone->m_penType =m_penType ;
	pClone->m_penWidth =m_penWidth;
	pClone->m_startArrow =m_startArrow;
	pClone->m_startArrowSize =m_startArrowSize;
	pClone->m_state = m_state;
	pClone->m_Transparency =m_Transparency; //for alpha blending;
	pClone->m_wrapMode = m_wrapMode;
	
	ASSERT_VALID(pClone);

	if (pDoc != NULL)
		pDoc->Add(pClone);

	return pClone;
}
void CDrawObj::RetrieveData(CDrawObj *pObj)  //help function for CEditActio{
{
	m_brushColor = pObj->m_brushColor;
	m_brushType =pObj->m_brushType;
	m_bToCenter = pObj->m_bToCenter;
	m_dashCap =pObj->m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	m_dashStyle = pObj->m_dashStyle;
	m_endArrow =pObj->m_endArrow;
	m_endArrowSize =pObj->m_endArrowSize;
	m_fillImage = pObj->m_fillImage;
	m_gradientColor =pObj->m_gradientColor;
	m_hatchStyle =pObj->m_hatchStyle;
	m_linearGradientMode =pObj->m_linearGradientMode; //for linear gradient only;
	m_nPage =pObj->m_nPage;
	m_nStretch =pObj->m_nStretch;
	m_penbrushType = pObj->m_penbrushType;
	m_penColor =pObj->m_penColor;
	m_penEndColor =pObj->m_penEndColor;
	m_penlinearGradientMode = pObj->m_penlinearGradientMode;
	m_penType =pObj->m_penType ;
	m_penWidth =pObj->m_penWidth;
	m_startArrow =pObj->m_startArrow;
	m_startArrowSize =pObj->m_startArrowSize;
	m_state = pObj->m_state;
	m_Transparency =pObj->m_Transparency; //for alpha blending;
	m_wrapMode = pObj->m_wrapMode;
}

void CDrawObj::OnEditProperties(CNetworkView* pView)
{

	ASSERT_VALID(this);
    CDrawProperty sheet ( _T("Properties") );
	CLineProperty linePage;
	linePage.SetValues(this);
	//if it is a arrow;
	CTextProperty textPage;
	CDrawText* pText;
	if(this->IsKindOf( RUNTIME_CLASS( CDrawText ) ) ){
		//case this to CDrawRect
		pText= (CDrawText* )(this) ;
			textPage.m_text=pText->m_text;
	}    

	sheet.AddPage( &linePage );
	sheet.AddPage( &textPage );

	if (sheet.DoModal() != IDOK)
		return;

	linePage.GetValues(this);
	if(this->IsKindOf( RUNTIME_CLASS( CDrawText ) ) ){
		//case this to CDrawRect
		pText= (CDrawText*)(this) ;
			pText->m_text=textPage.m_text;
	}    

	Invalidate();

}

void CDrawObj::OnOpen(CNetworkView* pView)
{
	OnEditProperties(pView);
}


ArrowTypeG CDrawObj::GetArrowType(UINT id)
{
	if (id == ID_ACTION_LEFT_ARROW)
		return m_startArrow;
	else if (id == ID_ACTION_RIGHT_ARROW)
		return m_endArrow;
	else {
		ASSERT(0);
		return NoAnchor;
	}
}

void CDrawObj::SetArrowType(UINT id, ArrowTypeG type){
	ASSERT_VALID(this);

	if (id == ID_ACTION_LEFT_ARROW)
		m_startArrow = type;
	else if (id == ID_ACTION_RIGHT_ARROW)
		m_endArrow = type;
	else {
		ASSERT(0);
	}
	Invalidate();
}

void CDrawObj::SetLineStyle(int style){
	ASSERT_VALID(this);

	m_dashStyle = (DashStyle)style;
	Invalidate();
	//m_pDocument->SetModifiedFlag();
}
void CDrawObj::SetLineWidth(int width){
	ASSERT_VALID(this);

	m_penWidth = width;
	Invalidate();
//	m_pDocument->SetModifiedFlag();
}
void CDrawObj::SetLineColor(COLORREF color)
{
	ASSERT_VALID(this);

	m_penColor.SetFromCOLORREF(color);
	Invalidate();
	//m_pDocument->SetModifiedFlag();
}

void CDrawObj::SetFillColor(COLORREF color)
{
	ASSERT_VALID(this);

	m_brushColor.SetFromCOLORREF(color);
	Invalidate();
//	m_pDocument->SetModifiedFlag();
}

void CDrawObj::OnModified(int flag, int value){
	switch (flag){
	case 0: //line width
		m_penWidth = value;
		Invalidate();
		m_pDocument->SetModifiedFlag();
		return;
	case 1: //line style
		m_dashStyle = (DashStyle)value;
		Invalidate();
		m_pDocument->SetModifiedFlag();
		return;
	}
}

BOOL CDrawObj::PointOnLine(CPoint point, POINT p1, POINT p2)
{
	CRect rect;
	rect.SetRect(point.x-1, point.y-1, point.x+1, point.y+1);
	return Intersects(rect,p1,p2);
}


BOOL CDrawObj::Intersects(CRect rect, POINT p1, POINT p2)
{
	CRgn rgn;
	rect.NormalizeRect();
	int w = (m_penWidth + 10) / 2;
	double l=sqrt((double)(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
	double dx=w*(p1.y-p2.y)/l;
	double dy=w*(p1.x-p2.x)/l;
	int x=(int)dx;
	int y=(int)dy;
	
	POINT points[4];
	points[0].x = p1.x;
	points[0].y = p1.y;
	points[1].x = p1.x;
	points[1].y = p1.y;
	points[2].x = p2.x;
	points[2].y = p2.y;
	points[3].x = p2.x;
	points[3].y = p2.y;

	points[0].x -= x;
	points[1].x += x;
	points[2].x += x;
	points[3].x -= x;

	points[0].y += y;
	points[1].y -= y;
	points[2].y -= y;
	points[3].y += y;

	rgn.CreatePolygonRgn(points, 4, WINDING);
	return rgn.RectInRegion(rect);
}

#ifdef _DEBUG
void CDrawObj::AssertValid()
{
	ASSERT(m_position.left <= m_position.right);
	ASSERT(m_position.bottom <= m_position.top);
}
#endif

////////////////////////////////////////////////////////////////////////////
// CDrawRect

IMPLEMENT_SERIAL(CDrawRect, CDrawObj, 0)

CDrawRect::CDrawRect()
{
}

CDrawRect::CDrawRect(const CRect& position)
	: CDrawObj(position)
{
	ASSERT_VALID(this);

	m_nShape = rectangle;
	m_roundness.x = 16;
	m_roundness.y = 16;
}

void CDrawRect::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CDrawObj::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << (BYTE) m_nShape;
		ar << m_roundness;
	}
	else
	{
		BYTE bTemp;
		ar >> bTemp; m_nShape = (Shape)bTemp;
		ar >> m_roundness;
	}
}

void CDrawRect::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	Graphics graphics (pDC->m_hDC);

	graphics.SetSmoothingMode(theApp.m_smoothingMode);

	CRect rect = m_position;
	Rect theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 
		//A bug of GDI+, the width and height for Rect must be positive when fill or draw Rectangle
		//but it is ok to fill or draw ellipse. :(
	//if (m_nShape==rectangle || m_nShape==roundRectangle)
		Normallize(theRect); 
	CRect myRect;
    CRgn rgn; 

	BYTE bDrawLine, bFill;
	bDrawLine = m_state & 0x1;
	bFill = m_state & 0x2;

	Brush *pBrush =NULL;
    //creat Bursh; not CBrush
	Color tc = SetA (m_brushColor, (BYTE) (255 * (100 - m_Transparency)/100));
	SolidBrush solidBrush(tc); //solid brush
    //hatch brush
	HatchBrush hBrush(m_hatchStyle, m_brushColor,m_gradientColor);

	GraphicsPath path;
	if (m_nShape==rectangle || m_nShape==roundRectangle){
		path.AddRectangle(theRect);
	}
	else if(m_nShape==ellipse){
		path.AddEllipse(theRect);
	}

	PathGradientBrush pthGrBrush(&path);
	
	if (m_bToCenter) {
		pthGrBrush.SetCenterColor(m_gradientColor);

		// Set the color along the entire boundary of the path to aqua.
		Color colors[] = {m_brushColor};
		int count = 1;
		pthGrBrush.SetSurroundColors(colors, &count);
	}
	else{
		pthGrBrush.SetCenterColor(m_brushColor);

		// Set the color along the entire boundary of the path to aqua.
		Color colors[] = {m_gradientColor};
		int count = 1;
		pthGrBrush.SetSurroundColors(colors, &count);
	}
	pthGrBrush.SetGammaCorrection(theApp.m_bGamma);

	//because the coordinate i use is the mirror to the defult, has to transform.
	Color c1, c2;
	LinearGradientMode lmode;
	if (m_linearGradientMode == LinearGradientModeHorizontal)
	{
		c1 = m_brushColor;
		c2 = m_gradientColor;
		lmode = LinearGradientModeHorizontal;
	}
	else if (m_linearGradientMode == LinearGradientModeVertical ){
		c1 = m_gradientColor ;
		c2 = m_brushColor;
		lmode = LinearGradientModeVertical;
	}
	else if (m_linearGradientMode == LinearGradientModeForwardDiagonal ){
		c1 = m_gradientColor ;
		c2 = m_brushColor;
		lmode = LinearGradientModeBackwardDiagonal  ;
	}
	else {
		c1 = m_gradientColor ;
		c2 = m_brushColor;
		lmode = LinearGradientModeForwardDiagonal ;
	}

	//line gradient brush
	LinearGradientBrush linGrBrush( theRect, c1,  c2, lmode);

	linGrBrush.SetGammaCorrection(theApp.m_bGamma);

    int len = m_fillImage.GetLength();
	unsigned short* string = new unsigned short[m_fillImage.GetLength()+1];
	int i = 0;
	for ( i = 0; i<len;i++)
		string[i] = (unsigned short) m_fillImage[i];
	string[i] = 0;

	Image* image;
	image = Image::FromFile((WCHAR*)string);
 
	delete [] string;
	
	ImageType type = image->GetType();
    UINT imageWidth = image->GetWidth();
	UINT imageHeight= image->GetHeight();


	//TRACE2("Width = %d and Height =%d.\n",imageWidth, imageHeight);
 
	image->RotateFlip(RotateNoneFlipY);

	if(type == ImageTypeBitmap){
		TRACE("The type of image1 is ImageTypeBitmap.\n");
	}

   
	if(type == ImageTypeMetafile) {
		Matrix matrix(1,0,0,-1,0,0);
		graphics.SetTransform(&matrix);
		theRect.Y *=-1;
		theRect.Height*=-1;
		Normallize (theRect);
		//TRACE("The type of image2 is ImageTypeMetafile.\n");
	}


	TextureBrush tBrush(image);

	if (m_nStretch)//strech
		tBrush.SetTransform(&Matrix(1.0*theRect.Width/image->GetWidth(), 0.0f, 1.0f,
				1.0*theRect.Height/image->GetHeight(), 0.0f, 1.0f));
	else{ 
		tBrush.SetWrapMode(m_wrapMode);
	}

	if (m_brushType == BrushTypeSolidColor){
		pBrush = &solidBrush;
	}
	else if( m_brushType ==  BrushTypeHatchFill){
		pBrush = &hBrush;
	}
	else if (m_brushType ==  BrushTypeTextureFill){
		pBrush = &tBrush;
	}
	else if (m_brushType == BrushTypePathGradient){
		pBrush = &pthGrBrush;
	}
	else if (m_brushType == BrushTypeLinearGradient){
		pBrush = &linGrBrush;
	}

	//Create Pen Object; not CPen
	Pen pen(m_penColor, m_penWidth);
	LinearGradientBrush penLinBrush(theRect, m_penColor, m_penEndColor, m_penlinearGradientMode);
	if (m_penbrushType == BrushTypeLinearGradient)
		pen.SetBrush(&penLinBrush);

	pen.SetDashStyle(m_dashStyle);
	pen.SetDashCap(m_dashCap);


	Status st;
	switch (m_nShape)
	{
	case rectangle:{
		if ((m_brushType ==  BrushTypeTextureFill)&& m_nStretch && bFill){
			//directly draw the picture in the reactange;
			graphics.DrawImage(image,theRect);
		}
		else if((m_brushType ==  BrushTypeTextureFill) && (m_wrapMode == WrapModeClamp ) &&bFill){
			//move the image to the right position. then clip the image
			RectF myrect;
			myrect.X = theRect.X + 0.5*theRect.Width - 0.5*image->GetWidth();
			myrect.Y = theRect.Y + 0.5*theRect.Height - 0.5*image->GetHeight();
			myrect.Width = image->GetWidth();
			myrect.Height = image->GetHeight();
			GraphicsPath path;
			path.AddRectangle (theRect);
			DrawImage(&graphics, image, &myrect, &path);
		}
		else if (bFill)
			st = graphics.FillRectangle(pBrush,theRect);
		if (bDrawLine)
			st = graphics.DrawRectangle(&pen,theRect);
		break;
		}	
	case roundRectangle:{ //GDI+ doesn't have FileRoundRectangle function, has to use FillRegion and DrawPath
		
		int X1 = theRect.X;
		int X2 = theRect.X+theRect.Width;
		int Y1 = theRect.Y;
		int Y2 = theRect.Y+theRect.Height;
		int W =  m_roundness.x/2;
		int H =  m_roundness.y/2;

		if (2*W >= theRect.Width)
			W = theRect.Width/2;

		if (2*H >= theRect.Height)
			H = theRect.Height/2;

		GraphicsPath roundRectPath;
		roundRectPath.AddArc(X1,Y1,2*W,2*H,180,90);
		roundRectPath.AddArc(X2-2*W,Y1,2*W,2*H,270,90);
		roundRectPath.AddArc(X2-2*W,Y2-2*H,2*W,2*H,0,90);
		roundRectPath.AddArc(X1,Y2-2*H,2*W,2*H,90,90);
		roundRectPath.CloseFigure();
		if (bDrawLine)
			graphics.DrawPath(&pen,&roundRectPath);
		if ((m_brushType ==  BrushTypeTextureFill)&& m_nStretch && bFill)
			CDrawObj::DrawImage(&graphics,image,&theRect,&roundRectPath);
		else if((m_brushType ==  BrushTypeTextureFill) && (m_wrapMode == WrapModeClamp )&& bFill){
			//move the image to the right position. then clip the image
			RectF myrect;
			myrect.X = theRect.X + 0.5*(theRect.Width - image->GetWidth());
			myrect.Y = theRect.Y + 0.5*(theRect.Height - image->GetHeight());
			myrect.Width = image->GetWidth();
			myrect.Height = image->GetHeight();
			DrawImage(&graphics, image, &myrect, &roundRectPath);
		}
		else if( bFill)
			graphics.FillPath(pBrush,&roundRectPath);
		break;
		}
	case ellipse:{
		if ((m_brushType ==  BrushTypeTextureFill)&& m_nStretch && bFill){
			GraphicsPath path;
			path.AddEllipse(theRect);
			CDrawObj::DrawImage(&graphics,image,&theRect,&path);
		}
		else if((m_brushType ==  BrushTypeTextureFill) && (m_wrapMode == WrapModeClamp )&& bFill){
			//move the image to the right position. then clip the image
			RectF myrect;
			myrect.X = theRect.X + 0.5*(theRect.Width - image->GetWidth());
			myrect.Y = theRect.Y + 0.5*(theRect.Height - image->GetHeight());
			myrect.Width = image->GetWidth();
			myrect.Height = image->GetHeight();
			GraphicsPath path;
			path.AddEllipse(theRect);
			DrawImage(&graphics, image, &myrect, &path);
		}
		else if(bFill)
			graphics.FillEllipse(pBrush,theRect);
		if(bDrawLine)
			graphics.DrawEllipse (&pen,theRect);
		break;
		}
	case line:
		if(bDrawLine)
			graphics.DrawLine(&pen,rect.BottomRight().x,rect.BottomRight().y, rect.TopLeft().x, rect.TopLeft().y);
		break;
	case arrow:
		SetArrowAnchors(&pen,0);
		if(bDrawLine)
			graphics.DrawLine(&pen,rect.BottomRight().x,rect.BottomRight().y, rect.TopLeft().x, rect.TopLeft().y);
		break;
	}
	graphics.ReleaseHDC(pDC->m_hDC);
}


int CDrawRect::GetHandleCount()
{
	ASSERT_VALID(this);

	return (m_nShape == line ||m_nShape==arrow)? 2 :
		CDrawObj::GetHandleCount() + (m_nShape == roundRectangle);
}

// returns center of handle in logical coordinates
CPoint CDrawRect::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	if ((m_nShape == line || m_nShape==arrow) && nHandle == 2)
		nHandle = 5;
	else if (m_nShape == roundRectangle && nHandle == 9)
	{
		CRect rect = m_position;
		rect.NormalizeRect();
		CPoint point = rect.BottomRight();
		point.x -= m_roundness.x / 2;
		point.y -= m_roundness.y / 2;
		return point;
	}

	return CDrawObj::GetHandle(nHandle);
}

HCURSOR CDrawRect::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	if ((m_nShape == line||m_nShape==arrow) && nHandle == 2)
		nHandle = 5;
	else if (m_nShape == roundRectangle && nHandle == 9)
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	return CDrawObj::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawRect::MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView)
{
	ASSERT_VALID(this);
	if ((m_nShape == line || m_nShape==arrow) && nHandle == 2)
		nHandle = 5;
	else if (m_nShape == roundRectangle && nHandle == 9)
	{
		CRect rect = m_position;
		rect.NormalizeRect();
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < rect.left + rect.Width() / 2)
			point.x = rect.left + rect.Width() / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < rect.top + rect.Height() / 2)
			point.y = rect.top + rect.Height() / 2;
		m_roundness.x = 2 * (rect.right - point.x);
		m_roundness.y = 2 * (rect.bottom - point.y);
//		m_pDocument->SetModifiedFlag();
		if (pView == NULL)
			Invalidate();
		else
			pView->InvalObj(this);
		return;
	}

	CDrawObj::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
BOOL CDrawRect::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);
    CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();

	//sometimes vertical line 
	if ((rectT & fixed).IsRectEmpty())
		if(m_nShape!=line && m_nShape!=arrow)
			return FALSE;

	CRgn rgn;
	switch (m_nShape)
	{
	case rectangle:
		return TRUE;

	case roundRectangle:
		rgn.CreateRoundRectRgn(fixed.left, fixed.top, fixed.right, fixed.bottom,
			m_roundness.x, m_roundness.y);
		break;

	case ellipse:
		rgn.CreateEllipticRgnIndirect(fixed);
		break;

	case line:
	case arrow:
		{
			int w = (m_penWidth + 10) / 2;
			//if (m_nShape==arrow) w+=m_rArrowSize;
			//int y = (m_logpen.lopnWidth.y + 5) / 2;   document says lopenWidth.y is not used;
			double l=sqrt((double)(fixed.top-fixed.bottom)*(fixed.top-fixed.bottom)+(fixed.left-fixed.right)*(fixed.left-fixed.right));
			double dx=w*(fixed.top-fixed.bottom)/l;
			double dy=w*(fixed.left-fixed.right)/l;
			int x=-(int)dx;
			int y=-(int)dy;
			
			int change=1;
			if (fixed.bottom!=m_position.bottom) change*=-1;
			if (fixed.left!=m_position.left) change*=-1;
			
			POINT points[4];
			points[0].x = m_position.left;
			points[0].y = m_position.top;
			points[1].x = m_position.left;
			points[1].y = m_position.top;
			points[2].x = m_position.right;
			points[2].y = m_position.bottom;
			points[3].x = m_position.right;
			points[3].y = m_position.bottom;

			if ((fixed.left-fixed.right)<=0)
			{
				points[0].x -= x;
				points[1].x += x;
				points[2].x += x;
				points[3].x -= x;
			}
			else
			{
				points[0].x += x;
				points[1].x -= x;
				points[2].x -= x;
				points[3].x += x;
			}

			if ((fixed.top - fixed.bottom)*change>=0)
			{
				points[0].y -= y;
				points[1].y += y;
				points[2].y += y;
				points[3].y -= y;
			}
			else
			{
				points[0].y += y;
				points[1].y -= y;
				points[2].y -= y;
				points[3].y += y;
			}
			rgn.CreatePolygonRgn(points, 4, WINDING);
		}
		break;
	default:
		ASSERT(0);
		return FALSE;
	}
	return rgn.RectInRegion(rectT);
}

CDrawObj* CDrawRect::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawRect* pClone		= new CDrawRect(m_position);
	pClone->m_brushColor =m_brushColor;
	pClone->m_brushType =m_brushType;
	pClone->m_bToCenter = m_bToCenter;
	pClone->m_dashCap =m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	pClone->m_dashStyle = m_dashStyle;
	pClone->m_endArrow =m_endArrow;
	pClone->m_endArrowSize =m_endArrowSize;
	pClone->m_fillImage = m_fillImage;
	pClone->m_gradientColor =m_gradientColor;
	pClone->m_hatchStyle =m_hatchStyle;
	pClone->m_linearGradientMode =m_linearGradientMode; //for linear gradient only;
	pClone->m_nPage =m_nPage;
	pClone->m_nStretch =m_nStretch;
	pClone->m_penbrushType = m_penbrushType;
	pClone->m_penColor =m_penColor;
	pClone->m_penEndColor =m_penEndColor;
	pClone->m_penlinearGradientMode = m_penlinearGradientMode;
	pClone->m_penType =m_penType ;
	pClone->m_penWidth =m_penWidth;
	pClone->m_startArrow =m_startArrow;
	pClone->m_startArrowSize =m_startArrowSize;
	pClone->m_state = m_state;
	pClone->m_Transparency =m_Transparency; //for alpha blending;
	pClone->m_wrapMode = m_wrapMode;
	pClone->m_nShape		= m_nShape;
	pClone->m_roundness		= m_roundness;  //for roundrect
	ASSERT_VALID(pClone);
	if (pDoc != NULL)
		pDoc->Add(pClone);

	ASSERT_VALID(pClone);
	return pClone;
}

void CDrawRect::RetrieveData(CDrawObj *pObj) //help function for CEditAction
{
	CDrawObj::RetrieveData(pObj);
	ASSERT(pObj->IsKindOf(RUNTIME_CLASS(CDrawRect)));
	CDrawRect *pRect = (CDrawRect*) pObj;

	m_nShape	=	pRect->m_nShape;
	m_roundness	=	pRect->m_roundness ;  //for roundrect
}



void CDrawRect::OnEditProperties(CNetworkView* pView)
{
	ASSERT_VALID(this);
	//Back up the data;
	pView->lastAction=new CEditAction(pView, ID_ACTION_EDIT, this);

    CDrawProperty sheet ( _T("Properties") );
	CLineProperty linePage;
	linePage.SetValues(this);
	sheet.AddPage( &linePage );

	if (sheet.DoModal() != IDOK){
		delete pView->lastAction; //No edit action is produced at all.
		pView->lastAction=NULL;
		return;
	}

	linePage.GetValues(this);
	Invalidate();
	pView->lastAction->Do(); //well, it is already done,  Do() just inform UndoManager :)
	//m_pDocument->SetModifiedFlag();

}

////////////////////////////////////////////////////////////////////////////
// CDrawPoly

IMPLEMENT_SERIAL(CDrawPoly, CDrawObj, 0)

CDrawPoly::CDrawPoly()
{
	m_points = NULL;
	m_nPoint = 0;
	m_nAllocPoints = 0;
	m_type = 0;
}

CDrawPoly::CDrawPoly(const CRect& position)
	: CDrawObj(position)
{
	m_points = NULL;
	m_nPoint = 0;
	m_nAllocPoints = 0;
}

CDrawPoly::~CDrawPoly()
{
	if (m_points != NULL)
		delete[] m_points;
}

void CDrawPoly::RetrieveData(CDrawObj *pObj) //help function for CEditAction
{
	CDrawObj::RetrieveData(pObj);
	ASSERT(pObj->IsKindOf(RUNTIME_CLASS(CDrawPoly)));
	CDrawPoly *pPoly = (CDrawPoly*) pObj;

	m_type	=	pPoly->m_type;
}

void CDrawPoly::Serialize( CArchive& ar )
{
	int i;
	CDrawObj::Serialize( ar );
	if( ar.IsStoring() )
	{
		ar << (WORD) m_nPoint;
		ar << (WORD) m_nAllocPoints;
		ar << m_type;
		for (i = 0;i< m_nPoint; i++)
			ar << m_points[i];
	}
	else
	{
		WORD wTemp;
		ar >> wTemp; m_nPoint = wTemp;
		ar >> wTemp; m_nAllocPoints = wTemp;
		ar >> m_type; //version 2.0
		m_points = new CPoint[m_nAllocPoints];
		for (i = 0;i < m_nPoint; i++)
			ar >> m_points[i];
	}
}

void CDrawPoly::Draw(CDC* pDC)
{
	ASSERT_VALID(this);
	Graphics graphics (pDC->m_hDC);
	graphics.SetSmoothingMode(theApp.m_smoothingMode);

	CRect rect = m_position;
	Rect theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 
	Normallize(theRect); 

	Brush *pBrush =NULL;
    //creat Bursh; not CBrush
	SolidBrush solidBrush(m_brushColor); //solid brush
    //hatch brush
	HatchBrush hBrush(m_hatchStyle, m_brushColor,m_gradientColor);

	GraphicsPath path;

	PointF *points = new PointF[m_nPoint+1];

	int i;
	for (i=0;i<m_nPoint;i++){
		points[i].X = m_points[i].x;
		points[i].Y = m_points[i].y;
	}

	points[i].X = m_points[0].x;
	points[i].Y = m_points[0].y;

	if (m_type<=1){
		path.AddPolygon(points,m_nPoint);
	}
	else if (m_type>1){
		path.AddCurve(points,m_nPoint+1);
	}

	PathGradientBrush pthGrBrush(&path);
	if (m_bToCenter) {
		pthGrBrush.SetCenterColor(m_gradientColor);

		// Set the color along the entire boundary of the path to aqua.
		Color colors[] = {m_brushColor};
		int count = 1;
		pthGrBrush.SetSurroundColors(colors, &count);
	}
	else{
		pthGrBrush.SetCenterColor(m_brushColor);

		// Set the color along the entire boundary of the path to aqua.
		Color colors[] = {m_gradientColor};
		int count = 1;
		pthGrBrush.SetSurroundColors(colors, &count);
	}
	pthGrBrush.SetGammaCorrection(theApp.m_bGamma);

	//line gradient brush
	//because the coordinate i use is the mirror to the defult, has to transform.
	Color c1, c2;
	LinearGradientMode lmode;
	if (m_linearGradientMode == LinearGradientModeHorizontal)
	{
		c1 = m_brushColor;
		c2 = m_gradientColor;
		lmode = LinearGradientModeHorizontal;
	}
	else if (m_linearGradientMode == LinearGradientModeVertical ){
		c1 = m_gradientColor ;
		c2 = m_brushColor;
		lmode = LinearGradientModeVertical;
	}
	else if (m_linearGradientMode == LinearGradientModeForwardDiagonal ){
		c1 = m_gradientColor ;
		c2 = m_brushColor;
		lmode = LinearGradientModeBackwardDiagonal  ;
	}
	else {
		c1 = m_gradientColor ;
		c2 = m_brushColor;
		lmode = LinearGradientModeForwardDiagonal ;
	}
	LinearGradientBrush linGrBrush( theRect, c1,  c2, lmode);
	linGrBrush.SetGammaCorrection(theApp.m_bGamma);

	//Text brush
    int len = m_fillImage.GetLength();
	unsigned short* string = new unsigned short[m_fillImage.GetLength()+1];
	for (i = 0; i<len;i++)
		string[i] = (unsigned short) m_fillImage[i];
	string[i] = 0;

	Image* image;
	image = Image::FromFile((WCHAR*)string);
	
	delete string;
	
	image->RotateFlip(RotateNoneFlipY);
	TextureBrush tBrush(image);

	if (m_nStretch)//strech
		tBrush.SetTransform(&Matrix(1.0*rect.Width()/image->GetWidth(), 0.0f, 0.0f,
				1.0*rect.Height()/image->GetHeight(), 0.0f, 0.0f));
	else{ 
		tBrush.SetWrapMode(m_wrapMode);
	}

	if (m_brushType == BrushTypeSolidColor){
		pBrush = &solidBrush;
	}
	else if( m_brushType ==  BrushTypeHatchFill){
		pBrush = &hBrush;
	}
	else if (m_brushType ==  BrushTypeTextureFill){
		pBrush = &tBrush;
	}
	else if (m_brushType == BrushTypePathGradient){
		pBrush = &pthGrBrush;
	}
	else if (m_brushType == BrushTypeLinearGradient){
		pBrush = &linGrBrush;
	}

	//Create Pen Object; not CPen
	Pen pen(m_penColor, m_penWidth);
	pen.SetDashStyle(m_dashStyle);
	pen.SetDashCap(m_dashCap);


	switch (m_type)
	{
	case POLYGON:{ // polygon
		if ((m_brushType ==  BrushTypeTextureFill)&& m_nStretch){
			//directly draw the picture in the reactange;
			CDrawObj::DrawImage(&graphics,image,&theRect,&path);
		}
		else if((m_brushType ==  BrushTypeTextureFill) && (m_wrapMode == WrapModeClamp )){
			//move the image to the right position. then clip the image
			RectF myrect;
			myrect.X = theRect.X + 0.5*theRect.Width - 0.5*image->GetWidth();
			myrect.Y = theRect.Y + 0.5*theRect.Height - 0.5*image->GetHeight();
			myrect.Width = image->GetWidth();
			myrect.Height = image->GetHeight();
			DrawImage(&graphics, image, &myrect, &path);
		}
		else
			graphics.FillPath(pBrush,&path);
		graphics.DrawPath(&pen,&path);
		break;
		}	
	case POLYLINE:{ //polyline
		graphics.DrawLines(&pen,points,m_nPoint);
		break;
		}
	case POLYCURVE:{ //a curve
		graphics.DrawCurve(&pen,points,m_nPoint);
		break;
		}
	case POLYCLOSEDCURVE:
		if ((m_brushType ==  BrushTypeTextureFill)&& m_nStretch){
			//directly draw the picture in the reactange;
			CDrawObj::DrawImage(&graphics,image,&theRect,&path);
		}
		else if((m_brushType ==  BrushTypeTextureFill) && (m_wrapMode == WrapModeClamp )){
			//move the image to the right position. then clip the image
			RectF myrect;
			myrect.X = theRect.X + 0.5*theRect.Width - 0.5*image->GetWidth();
			myrect.Y = theRect.Y + 0.5*theRect.Height - 0.5*image->GetHeight();
			myrect.Width = image->GetWidth();
			myrect.Height = image->GetHeight();
			GraphicsPath path;
			path.AddRectangle (theRect);
			DrawImage(&graphics, image, &myrect, &path);
		}
		else
			graphics.FillPath(pBrush,&path);
		graphics.DrawPath(&pen,&path);
		break;
	}
	delete [] points;
}

// position must be in logical coordinates
void CDrawPoly::MoveTo(const CRect& position, CNetworkView* pView)
{
	ASSERT_VALID(this);
	if (position == m_position)
		return;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);

	for (int i = 0; i < m_nPoint; i += 1)
	{
		m_points[i].x += position.left - m_position.left;
		m_points[i].y += position.top - m_position.top;
	}

	m_position = position;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);
//	m_pDocument->SetModifiedFlag();
}

void CDrawPoly::Shift(int x, int y)
{
	ASSERT_VALID(this);
	m_position.OffsetRect(x,y);

	for (int i = 0; i < m_nPoint; i += 1)
	{
		m_points[i].x += x;
		m_points[i].y += y;
	}
}

int CDrawPoly::GetHandleCount()
{
	return m_nPoint;
}

CPoint CDrawPoly::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	ASSERT(nHandle >= 1 && nHandle <= m_nPoint);
	return m_points[nHandle - 1];
}

HCURSOR CDrawPoly::GetHandleCursor(int )
{
	return AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}

// point is in logical coordinates
void CDrawPoly::MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView)
{
	ASSERT_VALID(this);
	ASSERT(nHandle >= 1 && nHandle <= m_nPoint);
	if (m_points[nHandle - 1] == point)
		return;

	m_points[nHandle - 1] = point;
	RecalcBounds(pView);

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);
//	m_pDocument->SetModifiedFlag();
}

// rect must be in logical coordinates
BOOL CDrawPoly::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	//not in m_position return false;
    CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();

	//sometimes vertical line 
	if ((rectT & fixed).IsRectEmpty())
			return FALSE;



	if (m_type ==POLYGON){
		CRgn rgn;
		rgn.CreatePolygonRgn(m_points, m_nPoint, ALTERNATE);
		return rgn.RectInRegion(rect);
	}
	else if (m_type ==POLYLINE){
		//check each line;
		for (int i=0;i<m_nPoint-1;i++)
			if (CDrawObj::PointOnLine(rect.TopLeft(),m_points[i],m_points[i+1]))
				return TRUE;

		return FALSE;
	}
	else {
		CSpline spline(m_points, m_nPoint);
		//generate a curve
		spline.Generate();
		//get the points number
		m_CurvePoints.RemoveAll();
		m_CurvePoints.SetSize(spline.GetCurveCount());
		int PointCount = 0;
		spline.GetCurve(m_CurvePoints.GetData(), PointCount);
		for (int i = 0; i<PointCount-1; i++){
			if (CDrawObj::PointOnLine(rect.TopLeft(),m_CurvePoints[i],m_CurvePoints[i+1]))
				return TRUE;
		}
		
		if (m_type == POLYCURVE)
			return FALSE;
		CRgn rgn;
		rgn.CreatePolygonRgn(m_points, m_nPoint, ALTERNATE);
		return rgn.RectInRegion(rect);
	}
	return false;

}

CDrawObj* CDrawPoly::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawPoly* pClone = new CDrawPoly(m_position);
	pClone->m_points = new CPoint[m_nAllocPoints];
	memcpy(pClone->m_points, m_points, sizeof(CPoint) * m_nPoint);
	pClone->m_nAllocPoints = m_nAllocPoints;
	pClone->m_nPoint = m_nPoint;
	pClone->m_type =m_type;
	pClone->m_brushColor =m_brushColor;
	pClone->m_brushType =m_brushType;
	pClone->m_bToCenter = m_bToCenter;
	pClone->m_dashCap =m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	pClone->m_dashStyle = m_dashStyle;
	pClone->m_endArrow =m_endArrow;
	pClone->m_endArrowSize =m_endArrowSize;
	pClone->m_fillImage = m_fillImage;
	pClone->m_gradientColor =m_gradientColor;
	pClone->m_hatchStyle =m_hatchStyle;
	pClone->m_linearGradientMode =m_linearGradientMode; //for linear gradient only;
	pClone->m_nPage =m_nPage;
	pClone->m_nStretch =m_nStretch;
	pClone->m_penbrushType = m_penbrushType;
	pClone->m_penColor =m_penColor;
	pClone->m_penEndColor =m_penEndColor;
	pClone->m_penlinearGradientMode = m_penlinearGradientMode;
	pClone->m_penType =m_penType ;
	pClone->m_penWidth =m_penWidth;
	pClone->m_startArrow =m_startArrow;
	pClone->m_startArrowSize =m_startArrowSize;
	pClone->m_state = m_state;
	pClone->m_Transparency =m_Transparency; //for alpha blending;
	pClone->m_wrapMode = m_wrapMode;
	ASSERT_VALID(pClone);

	if (pDoc != NULL)
		pDoc->Add(pClone);

	ASSERT_VALID(pClone);
	return pClone;
}

// point is in logical coordinates
void CDrawPoly::AddPoint(const CPoint& point, CNetworkView* pView)
{
	ASSERT_VALID(this);
	if (m_nPoint == m_nAllocPoints)
	{
		CPoint* newPoints = new CPoint[m_nAllocPoints + 10];
		if (m_points != NULL)
		{
			memcpy(newPoints, m_points, sizeof(CPoint) * m_nAllocPoints);
			delete[] m_points;
		}
		m_points = newPoints;
		m_nAllocPoints += 10;
	}

	if (m_nPoint == 0 || m_points[m_nPoint - 1] != point)
	{
		m_points[m_nPoint++] = point;
		if (!RecalcBounds(pView))
		{
			if (pView == NULL)
				Invalidate();
			else
				pView->InvalObj(this);
		}
//		m_pDocument->SetModifiedFlag();
	}
}

BOOL CDrawPoly::RecalcBounds(CNetworkView* pView)
{
	ASSERT_VALID(this);

	if (m_nPoint == 0)
		return FALSE;

	CRect bounds(m_points[0], CSize(0, 0));
	
	if (m_type<=1 || m_nPoint<=2){
		for (int i = 1; i < m_nPoint; ++i)
		{
			if (m_points[i].x < bounds.left)
				bounds.left = m_points[i].x;
			if (m_points[i].x > bounds.right)
				bounds.right = m_points[i].x;
			if (m_points[i].y < bounds.top)
				bounds.top = m_points[i].y;
			if (m_points[i].y > bounds.bottom)
				bounds.bottom = m_points[i].y;
		}
	}
	else{
		CSpline spline(m_points, m_nPoint);
		//generate a curve
		spline.Generate();
		//get the points number
		m_CurvePoints.RemoveAll();
		m_CurvePoints.SetSize(spline.GetCurveCount());
		int PointCount = 0;
		spline.GetCurve(m_CurvePoints.GetData(), PointCount);
		for (int i = 0; i<PointCount; i++){
			if (m_CurvePoints[i].x < bounds.left)
				bounds.left = m_CurvePoints[i].x;
			if (m_CurvePoints[i].x > bounds.right)
				bounds.right = m_CurvePoints[i].x;
			if (m_CurvePoints[i].y < bounds.top)
				bounds.top = m_CurvePoints[i].y;
			if (m_CurvePoints[i].y > bounds.bottom)
				bounds.bottom = m_CurvePoints[i].y;
		}
	}

	bounds.InflateRect(20,20);

	if (bounds == m_position)
		return FALSE;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);

	m_position = bounds;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);

	return TRUE;
}

void CDrawPoly::OnEditProperties(CNetworkView* pView)
{
	ASSERT_VALID(this);

	pView->lastAction = new CEditAction(pView, ID_ACTION_EDIT, this);
    CDrawProperty sheet ( _T("Properties") );
	CLineProperty linePage;

	linePage.SetValues((CDrawObj*)this);

	sheet.AddPage( &linePage );

	if (sheet.DoModal() != IDOK){
		delete pView->lastAction;
		pView->lastAction = 0;
		return;
	}

	linePage.GetValues((CDrawObj*)this);
	Invalidate();
	pView->lastAction->Do();

}
////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDrawOleObj, CDrawObj, 0)

BOOL CDrawOleObj::c_bShowItems = TRUE;

CDrawOleObj::CDrawOleObj() : m_extent(0,0)
{
	m_pClientItem = NULL;
}

CDrawOleObj::CDrawOleObj(const CRect& position)
	: CDrawObj(position), m_extent(0, 0)
{
	m_pClientItem = NULL;
}

CDrawOleObj::~CDrawOleObj()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Release();
		m_pClientItem = NULL;
	}
}

void CDrawOleObj::Remove()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Delete();
		m_pClientItem = NULL;
	}
	CDrawObj::Remove();
}

void CDrawOleObj::Serialize( CArchive& ar )
{
	ASSERT_VALID(this);

	CDrawObj::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_extent;
		ar << m_pClientItem;
	}
	else
	{
		ar >> m_extent;
		ar >> m_pClientItem;
		m_pClientItem->m_pDrawObj = this;
	}
}

CDrawObj* CDrawOleObj::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	AfxGetApp()->BeginWaitCursor();

	CDrawOleObj* pClone = NULL;
	CDrawCntrItem* pItem = NULL;
	TRY
	{
		// perform a "deep copy" -- need to copy CDrawOleObj and the CDrawCntrItem
		//  that it points to.
		CDrawOleObj* pClone = new CDrawOleObj(m_position);
		CDrawCntrItem* pItem = new CDrawCntrItem(m_pDocument, pClone);
		if (!pItem->CreateCloneFrom(m_pClientItem))
			AfxThrowMemoryException();

		pClone->m_pClientItem = pItem;
		ASSERT_VALID(pClone);

		if (pDoc != NULL)
			pDoc->Add(pClone);
	}
	CATCH_ALL(e)
	{
		pItem->Delete();
		pClone->m_pClientItem = NULL;
		pClone->Remove();
		AfxGetApp()->EndWaitCursor();

		THROW_LAST();
	}
	END_CATCH_ALL

	AfxGetApp()->EndWaitCursor();
	return pClone;
}

void CDrawOleObj::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	CDrawCntrItem* pItem = m_pClientItem;
	if (pItem != NULL)
	{
		// draw the OLE item itself
		pItem->Draw(pDC, m_position);

		// don't draw tracker in print preview or on printer
		if (!pDC->IsPrinting())
		{
			// use a CRectTracker to draw the standard effects
			CRectTracker tracker;
			tracker.m_rect = m_position;
			pDC->LPtoDP(tracker.m_rect);

			if (c_bShowItems)
			{
				// put correct border depending on item type
				if (pItem->GetType() == OT_LINK)
					tracker.m_nStyle |= CRectTracker::dottedLine;
				else
					tracker.m_nStyle |= CRectTracker::solidLine;
			}

			// put hatching over the item if it is currently open
			if (pItem->GetItemState() == COleClientItem::openState ||
				pItem->GetItemState() == COleClientItem::activeUIState)
			{
				tracker.m_nStyle |= CRectTracker::hatchInside;
			}
			tracker.Draw(pDC);
		}
	}
}

void CDrawOleObj::OnOpen(CNetworkView* pView)
{
	AfxGetApp()->BeginWaitCursor();
	m_pClientItem->DoVerb(
		GetKeyState(VK_CONTROL) < 0 ? OLEIVERB_OPEN : OLEIVERB_PRIMARY,
		pView);
	AfxGetApp()->EndWaitCursor();
}

void CDrawOleObj::OnEditProperties(CNetworkView* pView)
{
	// using COlePropertiesDialog directly means no scaling
	COlePropertiesDialog dlg(m_pClientItem, 100, 100, NULL);

	dlg.DoModal();
}

// position is in logical
void CDrawOleObj::MoveTo(const CRect& position, CNetworkView* pView)
{
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	// call base class to update position
	CDrawObj::MoveTo(position, pView);

	// update position of in-place editing session on position change
	if (m_pClientItem->IsInPlaceActive())
		m_pClientItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CDrawText
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDrawText, CDrawObj, 0)

CDrawText::CDrawText() : CDrawRect()
{
	m_fontColor			= Color(255,0,0,0);
	m_fontAlignment	= StringAlignmentCenter;
	m_fontStyle			= FontStyleRegular ;
	m_fontSize			= 12;
	m_fontName			= "Arial";
	m_format			= StringFormatFlagsDirectionRightToLeft;
	m_trimmingSyle		= StringTrimmingNone;
}

CDrawText::CDrawText(const CRect& position)
	: CDrawRect(position)
{
	ASSERT_VALID(this);
	m_text="Text";
	m_nBorderType=FALSE;
	m_fontColor			= Color(255,0,0,0);
	m_fontAlignment	= StringAlignmentCenter;
	m_fontStyle			= FontStyleRegular ;
	m_fontSize			= 12;
	m_fontName			= "Arial";
	m_format			= StringFormatFlagsDirectionRightToLeft;
	m_trimmingSyle		= StringTrimmingNone;
}
void CDrawText::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	CDrawRect::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nBorderType;
		ar << m_text;
	
		ar << m_fontColor;
		ar << m_fontName;
		ar << m_fontSize;
		ar << (BYTE)m_fontStyle;
		ar << (BYTE)m_trimmingSyle;
		ar << (BYTE)m_format;
		ar << (BYTE)m_fontAlignment;
	
	}
	else
	{
		ar >> m_nBorderType;
		ar >> m_text;
	
		ar >> m_fontColor;
		ar >> m_fontName;
		ar >> m_fontSize;
		BYTE temp;
		ar >> temp; m_fontStyle  = (FontStyle)temp;
		ar >> temp; m_trimmingSyle  = (StringTrimming)temp;
		ar >> temp; m_format  = (StringFormatFlags)temp;
		ar >> temp; m_fontAlignment  = (StringAlignment)temp;
	}
}

void CDrawText::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

    if (m_nBorderType>=0 && m_nBorderType<3 ){ //if have border
		CDrawRect::Draw(pDC);
	}
    
	//drawthe text here;

	Graphics graphics (pDC->m_hDC);

	graphics.SetSmoothingMode(theApp.m_smoothingMode);

	CRect rect = m_position;
	RectF theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

	int len = m_fontName.GetLength();
	unsigned short* fn = new unsigned short[m_fontName.GetLength()+1];
	int i = 0;
	for ( i = 0; i<len;i++)
		fn[i] = (unsigned short) m_fontName[i];
	fn[i] = 0;

	FontFamily  fontFamily((WCHAR*)fn );

	delete [] fn;

	Gdiplus::Font font(&fontFamily, m_fontSize, m_fontStyle, UnitPoint);
	
	SolidBrush  solidBrush(m_fontColor);

	StringFormat stringFormat;
	stringFormat.SetAlignment(m_fontAlignment);
	stringFormat.SetLineAlignment(m_fontAlignment);
	stringFormat.SetFormatFlags(m_format);
	stringFormat.SetTrimming(m_trimmingSyle);

	len = m_text.GetLength();
	unsigned short* ptext = new unsigned short[m_text.GetLength()];
	for (i = 0; i<len;i++)
		ptext[i] = (unsigned short) m_text[i];	


	//must reverse Text, because in my system, y is small as it gose low
	Matrix matrix(1,0,0,-1,0,0);
	graphics.SetTransform(&matrix);

	theRect.Y *=-1;
	theRect.Height*=-1;
	Normallize (theRect);

	graphics.DrawString((WCHAR*)ptext,len, &font, theRect,&stringFormat, &solidBrush);

	delete [] ptext;

}


//like resize(), but found the reat bound the text tightly.
void CDrawText::FitContent(CNetworkView *pView)
{
    if(!pView){ //get the active view
	
		POSITION pos =m_pDocument->GetFirstViewPosition();
		while (pos != NULL){
			CView* pV = m_pDocument->GetNextView(pos);
			if (pV->IsKindOf(RUNTIME_CLASS(CNetworkView))){
				pView=(CNetworkView*)pV;
				break;
			}
		}   
	}
	ASSERT(pView);

	CClientDC CDC(pView);
	int x= TextLen(&CDC);//length of your string;
    int y= MaxHeight(&CDC);//hight of the string;
	if (x==0){ //empty text string
		x = 4;
		y = 9;
	}

	if (m_position.left<m_position.right) 
		m_position.right=m_position.left+x+2*m_penWidth+4;
    else
		m_position.left=m_position.right+x+2*m_penWidth+4;

	if (m_position.top<m_position.bottom) 
		m_position.bottom=m_position.top+y+2*m_penWidth+2;
    else
		m_position.top=m_position.bottom+y+2*m_penWidth+2;

	m_position.NormalizeRect();
}

int CDrawText::GetHandleCount()
{
	ASSERT_VALID(this);

	return CDrawObj::GetHandleCount() + (m_nShape == roundRectangle);
}

// returns center of handle in logical coordinates
CPoint CDrawText::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
    if (m_nShape == roundRectangle && nHandle == 9)
		{
		CRect rect = m_position;
		rect.NormalizeRect();
		CPoint point = rect.BottomRight();
		point.x -= m_roundness.x / 2;
		point.y -= m_roundness.y / 2;
		return point;
		}
	return CDrawObj::GetHandle(nHandle);
}

HCURSOR CDrawText::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);
	if (m_nShape == roundRectangle && nHandle == 9)
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	return CDrawObj::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawText::MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView)
{
	ASSERT_VALID(this);
	if (m_nShape == roundRectangle && nHandle == 9)
	{
		CRect rect = m_position;
		rect.NormalizeRect();
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < rect.left + rect.Width() / 2)
			point.x = rect.left + rect.Width() / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < rect.top + rect.Height() / 2)
			point.y = rect.top + rect.Height() / 2;
		m_roundness.x = 2 * (rect.right - point.x);
		m_roundness.y = 2 * (rect.bottom - point.y);
//		m_pDocument->SetModifiedFlag();
		if (pView == NULL)
			Invalidate();
		else{
			pView->InvalObj(this);
			Resize(pView);
		}
		return;
	}

    //nHandle dicide with point is moved
	CDrawObj::MoveHandleTo(nHandle, point, pView);    //make sure the movement is good
	Resize(pView);
    //must resize
}

// rect must be in logical coordinates
BOOL CDrawText::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);
    CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();

	//sometimes vertical line 
	if ((rectT & fixed).IsRectEmpty())
			return FALSE;

	CRgn rgn;
	switch (m_nShape)
	{
	case rectangle:
		return TRUE;

	case roundRectangle:
		rgn.CreateRoundRectRgn(fixed.left, fixed.top, fixed.right, fixed.bottom,
			m_roundness.x, m_roundness.y);
		break;

	case ellipse:
		rgn.CreateEllipticRgnIndirect(fixed);
		break;

	case line:
	case arrow:
		return TRUE;
		break;
	}
	//return rgn.RectInRegion(fixed); error!
	return rgn.RectInRegion(rectT);
}

CDrawObj* CDrawText::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawText* pClone = new CDrawText(m_position);
	pClone->m_brushColor =m_brushColor;
	pClone->m_brushType =m_brushType;
	pClone->m_bToCenter = m_bToCenter;
	pClone->m_dashCap =m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	pClone->m_dashStyle = m_dashStyle;
	pClone->m_endArrow =m_endArrow;
	pClone->m_endArrowSize =m_endArrowSize;
	pClone->m_fillImage = m_fillImage;
	pClone->m_gradientColor =m_gradientColor;
	pClone->m_hatchStyle =m_hatchStyle;
	pClone->m_linearGradientMode =m_linearGradientMode; //for linear gradient only;
	pClone->m_nPage =m_nPage;
	pClone->m_nStretch =m_nStretch;
	pClone->m_penbrushType = m_penbrushType;
	pClone->m_penColor =m_penColor;
	pClone->m_penEndColor =m_penEndColor;
	pClone->m_penlinearGradientMode = m_penlinearGradientMode;
	pClone->m_penType =m_penType ;
	pClone->m_penWidth =m_penWidth;
	pClone->m_startArrow =m_startArrow;
	pClone->m_startArrowSize =m_startArrowSize;
	pClone->m_state = m_state;
	pClone->m_Transparency =m_Transparency; //for alpha blending;
	pClone->m_wrapMode = m_wrapMode;
	pClone->m_nShape = m_nShape;
	pClone->m_roundness = m_roundness;
    pClone->m_text=m_text;
	pClone->m_nBorderType=m_nBorderType;

	
	pClone->m_allignment = m_allignment;
	pClone->m_color =m_color;
	pClone->m_fontAlignment=m_fontAlignment;
	pClone->m_fontColor=m_fontColor;
	pClone->m_fontName =m_fontName;
	pClone->m_fontSize =m_fontSize;
	pClone->m_fontStyle =m_fontStyle;
	pClone->m_format =m_format;
	pClone->m_nBorderType=m_nBorderType;
    pClone->m_text=m_text;
	pClone->m_trimmingSyle = m_trimmingSyle;
	
	ASSERT_VALID(pClone);
	if (pDoc != NULL)
		pDoc->Add(pClone);

	ASSERT_VALID(pClone);
	return pClone;
}

void CDrawText::RetrieveData(CDrawObj *pObj) //help function for CEditAction
{
	CDrawRect::RetrieveData(pObj);
	ASSERT(pObj->IsKindOf(RUNTIME_CLASS(CDrawText)));
	CDrawText* pText = (CDrawText *)pObj;

	m_allignment	= pText->m_allignment;
	m_color			= pText->m_color;
	m_fontAlignment	= pText->m_fontAlignment;
	m_fontColor		= pText->m_fontColor;
	m_fontName		= pText->m_fontName;
	m_fontSize		= pText->m_fontSize;
	m_fontStyle		= pText->m_fontStyle;
	m_format		= pText->m_format;
	m_nBorderType	= pText->m_nBorderType;
    m_text			= pText->m_text;
	m_trimmingSyle	= pText->m_trimmingSyle;
}

void CDrawText::OnEditProperties(CNetworkView* pView)
{
	ASSERT_VALID(this);

    CDrawProperty sheet ( _T("Properties") );
	CTextFormatPage tPage;
	tPage.SetValues(this);
	CTextProperty textPage;
	textPage.m_text=m_text;

	sheet.AddPage( &textPage );
	sheet.AddPage( &tPage );

	if (sheet.DoModal() != IDOK)
		return;

	m_text=textPage.m_text;
	tPage.GetValues(this);

    Resize(pView);
	Invalidate();
//	m_pDocument->SetModifiedFlag();

}

void CDrawText::Resize(int x, int y)
{
	if(abs(m_position.left-m_position.right)<x+2*m_penWidth){
		if (m_position.left<m_position.right) 
			m_position.right=m_position.left+x+2*m_penWidth+8;
        else
			m_position.left=m_position.right+x+m_penWidth+8;
	}
	if(abs(m_position.top-m_position.bottom)<y+m_penWidth){
		if (m_position.top<m_position.bottom) 
			m_position.bottom=m_position.top+y+2*m_penWidth+2;
        else
			m_position.top=m_position.bottom+y+2*m_penWidth+2;
	}
	return;
}

int CDrawText::MaxHeight(CDC *pDC)
{
	if (m_text =="") return 1;

	Graphics graphics (pDC->m_hDC);

	CRect rect = m_position;
	RectF theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

	int len = m_fontName.GetLength();
	unsigned short* fn = new unsigned short[m_fontName.GetLength()+1];
	int i = 0;
	for ( i = 0; i<len;i++)
		fn[i] = (unsigned short) m_fontName[i];
	fn[i] = 0;

	FontFamily  fontFamily((WCHAR*)fn );

	delete [] fn;

	Gdiplus::Font font(&fontFamily, m_fontSize, m_fontStyle, UnitPoint);
	
	SolidBrush  solidBrush(m_fontColor);

	StringFormat stringFormat;
	stringFormat.SetAlignment(m_fontAlignment);
	stringFormat.SetLineAlignment(m_fontAlignment);
	stringFormat.SetFormatFlags(m_format);
	stringFormat.SetTrimming(m_trimmingSyle);

	len = m_text.GetLength();
	unsigned short* ptext = new unsigned short[m_text.GetLength()];
	for (i = 0; i<len;i++)
		ptext[i] = (unsigned short) m_text[i];	


	RectF boundRect;
	graphics.MeasureString((WCHAR*)ptext,len, &font, theRect,&stringFormat, &boundRect);

	delete [] ptext;
	return boundRect.Height;
}

void CDrawText::SetTextColor(COLORREF color)
{
	ASSERT_VALID(this);
	m_fontColor.SetFromCOLORREF(color);
	Invalidate();
}

int CDrawText::TextLen(CDC *pDC)
{
	Graphics graphics (pDC->m_hDC);

	CRect rect = m_position;
	RectF theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

	int len = m_fontName.GetLength();
	unsigned short* fn = new unsigned short[m_fontName.GetLength()+1];
	int i = 0;
	for ( i = 0; i<len;i++)
		fn[i] = (unsigned short) m_fontName[i];
	fn[i] = 0;

	FontFamily  fontFamily((WCHAR*)fn );

	delete [] fn;

	Gdiplus::Font font(&fontFamily, m_fontSize, m_fontStyle, UnitPoint);
	
	SolidBrush  solidBrush(m_fontColor);

	StringFormat stringFormat;
	stringFormat.SetAlignment(m_fontAlignment);
	stringFormat.SetLineAlignment(m_fontAlignment);
	stringFormat.SetFormatFlags(m_format);
	stringFormat.SetTrimming(m_trimmingSyle);

	len = m_text.GetLength();
	unsigned short* ptext = new unsigned short[m_text.GetLength()];
	for (i = 0; i<len;i++)
		ptext[i] = (unsigned short) m_text[i];	


	RectF boundRect;
	graphics.MeasureString((WCHAR*)ptext,len, &font, theRect,&stringFormat, &boundRect);

	delete [] ptext;
	return boundRect.Width;
}

void CDrawText::Resize(CNetworkView* pView)
{
    if(!pView){ //get the active view
	
		POSITION pos =m_pDocument->GetFirstViewPosition();
		while (pos != NULL){
			CView* pV = m_pDocument->GetNextView(pos);
			if (pV->IsKindOf(RUNTIME_CLASS(CNetworkView))){
				pView=(CNetworkView*)pV;
				break;
			}
		}   
	}
	ASSERT(pView);

	CClientDC CDC(pView);
	CFont font;
	if (!font.CreateFontIndirect(&m_logfont))
		return;
    CFont *pOldFont=(CFont *)CDC.SelectObject(&font);
	int lx= TextLen(&CDC);//length of your string;
    int ly= MaxHeight(&CDC);//hight of the string;
	if (lx==0){ //empty text string
		lx = 4;
		ly = 9;
	}

    Resize(lx+2*m_penWidth+2*PADDING,
		ly+m_penWidth*2+2*PADDING); //resize the m_position, by the new lenght and height of string
	m_position.NormalizeRect();

    CDC.SelectObject(pOldFont);

}

#ifdef _DEBUG
//object dumping code;
void CDrawObj::Dump( CDumpContext& dc ) const{
	CObject::Dump(dc);
	dc<<"\tleft="<<m_position.TopLeft().x<<" top="<<m_position.TopLeft().y;
	dc<<" right="<<m_position.BottomRight().x<<" button="<<m_position.BottomRight().y<<"\n";
	dc<<"\tconnect to document at "<<(void *)m_pDocument<<"\n";
}

void CDrawOleObj::Dump( CDumpContext& dc ) const{

}


void CDrawPoly::Dump( CDumpContext& dc ) const{

}

void CDrawRect::Dump( CDumpContext& dc ) const{
	CDrawObj::Dump(dc);
	dc<<"\tCDrawRect type="<<m_nShape<<"\n";
}



void CDrawText::Dump( CDumpContext& dc ) const{
	CDrawRect::Dump(dc);
	dc<<"\tCDrawText text="<<m_text<<"\n";
}


#endif


/* It is found when the width of line is very big >20, 
   the arrow start point need to be modified
   */

void ConvertPoint(POINT& x, POINT& y, int r){
    //make sure x is the left point, 
	if (x.x>y.x){
		ConvertPoint(y,x,r);
		
		//switch back
		POINT c;
		c=x; 	x=y; 	y=c;
		return;
	}

	POINT r1,r2;

	if (x.y<y.y){
		r1.x=x.x+r;
		r1.y=x.y+r;
		r2.x=y.x-r;
		r2.y=y.y-r;
	}
	else{
		r1.x=x.x+r;
		r1.y=x.y-r;
		r2.x=y.x-r;
		r2.y=y.y+r;
	}

	x=r1;
	y=r2;
	return;
}

/* It is found when the width of line is very big >20, 
   the arrow start point need to be modified
   */

void ConvertRect(CRect& r, int w){
	POINT x=r.TopLeft();
	POINT y=r.BottomRight();
	ConvertPoint(x,y,w);
	CRect newRect(x,y);
	r=newRect;
	return;
}


CCharFormat	CDrawObj::GetCharFormat(){
	CCharFormat cf;
	memcpy(cf.szFaceName, "Arial", 32);
	cf.yHeight=240;
	cf.dwMask |= CFM_FACE;
	cf.dwMask |= CFM_SIZE;
	return cf;
}

/*Modify the char format for CText Object*/
void CDrawText::SetCharFormat(CCharFormat cf,CNetworkView* pView)
{
	CDrawObj::SetCharFormat(cf, pView);
	// cf.szFaceName is fontname  cf.dwMask & CFM_FACE ==TRUE
	//cf.yHeight is hight (in twips), when cf.dwMask & CFM_SIZE ==TRUE
	//need to modify m_logfont here.
	LOGFONT		logfont;
	logfont =	m_logfont;
	//only change fontname and size
	if (cf.dwMask & CFM_FACE) {//change fontname, which is at most 32 byte
		memcpy(logfont.lfFaceName, cf.szFaceName,32);
		m_fontName = CString(cf.szFaceName,32);
	}
	if (cf.dwMask & CFM_SIZE){ //change size  : logfont.lfHeight= - (int)(cf.yHeight/15+0.5);
		logfont.lfHeight = cf.yHeight/20.0;
		m_fontSize = cf.yHeight/20.0;
	}
	m_logfont = logfont;
	//update the object
    Resize(pView);
	Invalidate();
}

CCharFormat	CDrawText::GetCharFormat(){
	CCharFormat cf;
	memcpy(cf.szFaceName, m_logfont.lfFaceName, 32);
	if (m_logfont.lfHeight <0) 
		cf.yHeight=long((-m_logfont.lfHeight)+0.5)*15; //lagacy code from version 1.0 
	else
		cf.yHeight = m_logfont.lfHeight*20.0;

	cf.dwMask |= CFM_FACE;
	cf.dwMask |= CFM_SIZE;
	return cf;
}



void CDrawRect::OnModified(int flag, int value)
{
	CDrawObj::OnModified(flag,value);
	switch (flag){
	case 2: //left arrow type
		m_startArrow = (ArrowTypeG)value;
		Invalidate();
		m_pDocument->SetModifiedFlag();
		return;
	case 3: //right arrow type
		m_endArrow =(ArrowTypeG)value;
		Invalidate();
		m_pDocument->SetModifiedFlag();
		return;
	}
}




POINT CDrawObj::AdjustIntect(POINT p1, POINT p2, int size)
{
	POINT p3; //p3 will on the line from p2 to p1, and the distance from p3 to p1 is size;
	float dx,dy;

	size *=2;
	float length = (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y);
	length = sqrt(length);

	float ratio = size/length;

	dx = ratio*(p1.x - p2.x);
	p3.x = p1.x - dx;

	dy = ratio*(p1.y - p2.y);
	p3.y = p1.y - dy;

	return p3;
}


CArchive& operator<< (CArchive &ar, Color& c){

	ar<<c.GetA();
	ar<<c.GetR();
	ar<<c.GetG();
	ar<<c.GetB();

	return ar;
}

CArchive& operator>> (CArchive &ar, Color& c){
	BYTE a, r, g,b;

	ar>>a;
	ar>>r;
	ar>>g;
	ar>>b;
	c.SetValue(Color::MakeARGB(a, r, g, b));
	return ar;
}


Status  CDrawObj::DrawImage(Graphics* g, Image* pImage, Rect *pRect, GraphicsPath* pPath){
	Region OldRegion;
	g->GetClip(&OldRegion);
	Region region(pPath);
	g->SetClip(&region);
	g->DrawImage(pImage,*pRect);
	g->SetClip(&OldRegion);
	return g->GetLastStatus();
}

Status  CDrawObj::DrawImage(Graphics* g, Image* pImage, RectF *pRect, GraphicsPath* pPath){
	Region OldRegion;
	g->GetClip(&OldRegion);
	Region region(pPath);
	g->SetClip(&region);
	g->DrawImage(pImage,*pRect);
	g->SetClip(&OldRegion);
	return g->GetLastStatus();
}

//a help function to make Color object based on COLORREF and alpha value;
Color CDrawObj::SetA(Color &c,	BYTE a)
{
	DWORD a1 = a;
	ARGB argb = c.GetValue();
	a1 = a1*0x1000000;
	argb = argb & 0x00ffffff;
	argb = argb | a1;
	Color c1(argb);
	return c1;
}



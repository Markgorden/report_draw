/********************************************************************/
/* NetworkView.h: Interface for CNetworkView			            */
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 2001, all rights reserved.				*/
/********************************************************************/


#ifndef __DRAWTOOL_H__
#define __DRAWTOOL_H__

#include "drawobj.h"

class CNetworkView;

enum DrawShape
{
	selection,
	line,
	rect,
	roundRect,
	ellipse,
	poly,
	arrow,
	text,
	scribble,
	curve,
	closedcurve,
	zoom
};

class CDrawTool
{
// Constructors
public:
	CDrawTool(DrawShape nDrawShape);

// Overridables
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CNetworkView* pView);
	virtual void MoveSelection(CNetworkView* pView, UINT direction);
	virtual void OnCancel();

// Attributes
	DrawShape m_drawShape;

	static CDrawTool* FindTool(DrawShape drawShape);
	static CPtrList c_tools;
	static CPoint c_down;
	static UINT c_nDownFlags;
	static CPoint c_last;
	static DrawShape c_drawShape;
};

class CSelectTool : public CDrawTool
{
// Constructors
public:
	virtual void MoveSelection(CNetworkView* pView, UINT direction);
	CSelectTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CNetworkView* pView);
	bool m_bSecondClick;
};

class CZoomTool : public CDrawTool
{
// Constructors
public:
	CZoomTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
};


class CRectTool : public CDrawTool
{
// Constructors
public:
	CRectTool(DrawShape drawShape);

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
};

class CTextTool : public CRectTool
{
// Constructors
public:
	CTextTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
};

class CPolyTool : public CDrawTool
{
// Constructors
public:
	CPolyTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	CDrawPoly* m_pDrawObj;
};

class CScribbleTool : public CDrawTool
{
// Constructors
public:
	CScribbleTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	CDrawPoly* m_pDrawObj;
};
class CCurveTool : public CDrawTool
{
// Constructors
public:
	CCurveTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	CDrawPoly* m_pDrawObj;
};
class CClosedCurveTool : public CDrawTool
{
// Constructors
public:
	CClosedCurveTool();

// Implementation
	virtual void OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	CDrawPoly* m_pDrawObj;
};
////////////////////////////////////////////////////////////////////////////

#endif // __DRAWTOOL_H__

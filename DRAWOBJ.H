/********************************************************************/
/* drawobj.h -  Interface for CDrawObj and derivatives              */
/*             CVariables, CParameters, CConstraints, CChart, CSeria*/
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 2001, all rights reserved.				*/
/********************************************************************/

#ifndef __DRAWOBJ_H__
#define __DRAWOBJ_H__

#include "stdafx.h"
#include "line.h"
class CNetworkView;
class CDrawDoc;


/////////////////////////////////////////////////////////////////////
// CDrawObj - base class for all 'drawable objects'
/////////////////////////////////////////////////////////////////////

enum ArrowType { noArrow, pureArrow, openArrow,stealthArrow, diamondArrow, ovalArrow};
enum ArrowTypeG {
	NoAnchor		= 0x10,   //LineCap::LineCapNoAnchor
	SquareAnchor	= 0x11,   //LineCap::LineCapSquareAnchor
	RoundAnchor     = 0x12,   //LineCap::LineCapRoundAnchor
	DimondAnchor	= 0x13,   //LineCap::LineCapDimondAnchor
	ArrowAnchor		= 0x14,
	OpenAnchor		= 0xF1,
	StealthAnchor   = 0xF2,
	CustomAnchor    = 0xFF    //LineCap::LineCapNoAnchor
};
	
CArchive & operator<< (CArchive &ar, Color& c);
CArchive& operator>> (CArchive &ar, Color& c);

class CDrawObj : public CObject
{
protected:
	DECLARE_SERIAL(CDrawObj);
	CDrawObj();

// Constructors
public:
	CDrawObj(const CRect& position);

// Attributes
	CRect				m_position;
	CDrawDoc*			m_pDocument;
	BOOL				m_bShown;
	int					m_nPage;  
// Operations
	virtual ~CDrawObj();
	virtual int			GetHandleCount();
	virtual	void		OnAdded(){return;}
	virtual void        OnRemoved(){return;}
	virtual CPoint		GetHandle(int nHandle);
	CRect				GetHandleRect(int nHandleID, CNetworkView* pView);
	virtual HCURSOR		GetHandleCursor(int nHandle);
	virtual void		OnModified(int flag, int value);
	virtual void		Draw(CDC*);
	enum				TrackerState { normal, selected, active };
	virtual void		DrawTracker(CDC* pDC, TrackerState state);
	virtual void		MoveTo(const CRect& positon, CNetworkView* pView = NULL);
	virtual int			HitTest(CPoint point, CNetworkView* pView, BOOL bSelected);
	virtual BOOL		Intersects(const CRect& rect);
	virtual BOOL		InRect(const CRect& rect);
	virtual void		MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView = NULL);
	virtual void		OnOpen(CNetworkView* pView);
	virtual void		OnEditProperties(CNetworkView* pView);
	virtual CDrawObj*	Clone(CDrawDoc* pDoc = NULL);
	virtual void		Remove();
	void				Invalidate();
	virtual void		Initialize();
	virtual void		Serialize(CArchive& ar);
	virtual void		Shift(int x, int y);

// help function for Undo/Redo
public:
	static Color		SetA(Color &c, BYTE a);
	POINT				AdjustIntect (POINT p1, POINT p2, int size);
	static Status 		DrawImage(Graphics*, Image*, Rect *, GraphicsPath*);
	static Status 		DrawImage(Graphics*, Image*, RectF *, GraphicsPath*);
	void				SetArrowAnchors(Pen*, int);
	BOOL				Intersects(CRect rect, POINT, POINT);
	BOOL				PointOnLine(CPoint , POINT, POINT);
	BOOL				PointOnPoint(CPoint , POINT&);
	void				SetLineColor(COLORREF color);
	void				SetFillColor(COLORREF color);
	void				SetLineStyle(int style);
	void				SetLineWidth(int width);
	virtual void		SetTextColor(COLORREF color){return;}
	virtual void		SetArrowType(UINT, ArrowTypeG type);
	virtual void		SetCharFormat(CCharFormat cf, CNetworkView*){return;}
	virtual void		RetrieveData(CDrawObj *pObj);  //help function for CEditAction
	COLORREF			GetLineColor(){ return m_penColor.ToCOLORREF();}
	COLORREF			GetFillColor(){ return m_brushColor.ToCOLORREF();}

	virtual CCharFormat	GetCharFormat(); //return nothing
	int					GetLineStyle(){return m_dashStyle;}
	int					GetLineWidth(){ return m_penWidth;}
	virtual COLORREF	GetTextColor(){ return m_penColor.ToCOLORREF();}
	virtual ArrowTypeG  GetArrowType(UINT);
	
	
	
#ifdef _DEBUG
	void AssertValid();
	virtual void		Dump( CDumpContext& dc ) const;
#endif

	// implementation data
protected:

	BYTE					m_state;  

	DashStyle				m_dashStyle;
	DashCap					m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	BrushType               m_brushType,
							m_penbrushType;  // BrushTypeLinearGradient or BrushTypeSolidColor Only
	PenType					m_penType;
	BYTE					m_Transparency; //for alpha blending;
	Color  				    m_brushColor, //when hatch pattern used, this is foreColor
	                        m_gradientColor,
							m_penColor,
							m_penEndColor;
	BYTE					m_penWidth;
	LinearGradientMode      m_linearGradientMode, //for linear gradient only;
							m_penlinearGradientMode;
	HatchStyle              m_hatchStyle;
	BYTE                    m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
	                        m_endArrowSize;
	ArrowTypeG				m_startArrow,
							m_endArrow;
	CString					m_fillImage;
	WrapMode                m_wrapMode;
	BYTE					m_bToCenter; //0, - center path gradient.
	BYTE					m_nStretch;

	friend class CNetworkView;
	friend class CReactionTool;
	friend class CRegulationTool;
	friend class CSelectTool;
	friend class CEditMultiAction;
	friend class CLineProperty;
	friend class CTextFormatPage;
};

// special 'list' class for this application (requires afxtempl.h)
typedef CTypedPtrList<CObList, CDrawObj*> CDrawObjList;

////////////////////////////////////////////////////////////////////////
// specialized draw objects
////////////////////////////////////////////////////////////////////////

class CDrawRect : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawRect);
	CDrawRect();

public:
	CDrawRect(const CRect& position);

// Implementation
public:
	virtual void		Serialize(CArchive& ar);
	virtual void		Draw(CDC*);
	virtual int			GetHandleCount();
	virtual CPoint		GetHandle(int nHandle);
	virtual HCURSOR		GetHandleCursor(int nHandle);
	virtual void		MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView = NULL);
	virtual BOOL		Intersects(const CRect& rect);
	virtual CDrawObj*	Clone(CDrawDoc* pDoc);
	virtual void		OnEditProperties(CNetworkView* pView);
	virtual void		RetrieveData(CDrawObj *pObj);  //help function for CEditAction
	virtual void		OnModified(int flag, int value);
	POINT				GetIntectPoint(POINT p1, POINT p2);
#ifdef _DEBUG
	virtual void		Dump( CDumpContext& dc ) const;
#endif
public:
	enum	Shape	  { rectangle, roundRectangle, ellipse, line,arrow };
	Shape	m_nShape;

protected:
	CPoint		m_roundness;					 /* for roundRect corners */
private:

	friend class CRectTool;
	friend class CNetworkView;
	friend class CLineProperty;
	friend class CDrawObj;
	friend class CSelectTool;
	friend class CEditMultiAction;
	friend class CEditMultiAction;
};
/////////////////////////////////////////////////////////////////////////////
// CDrawText 
/////////////////////////////////////////////////////////////////////////////
class CDrawText : public CDrawRect
{
protected:
	DECLARE_SERIAL(CDrawText);
	CDrawText();

public:
	CDrawText(const CRect& position);

// Implementation
public:
	void				SetCharFormat(CCharFormat cf,CNetworkView*);
	virtual CCharFormat	GetCharFormat();
	virtual void		Resize(CNetworkView* pView=NULL);
	virtual void		Serialize(CArchive& ar);
	virtual void		Draw(CDC*);
	virtual int			GetHandleCount();
	virtual CPoint		GetHandle(int nHandle);
	virtual HCURSOR		GetHandleCursor(int nHandle);
	virtual void		MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView = NULL);
	virtual BOOL		Intersects(const CRect& rect);
	virtual CDrawObj*	Clone(CDrawDoc* pDoc);
	virtual void		OnEditProperties(CNetworkView* pView);
	void				FitContent(CNetworkView *pView);
	virtual void		SetTextColor(COLORREF color);
	virtual void		RetrieveData(CDrawObj *pObj);  //help function for CEditAction
public:
	CString				m_text;
    int					m_allignment;
protected:
	int					MaxHeight(CDC* pDC);
	int					TextLen(CDC* pDC);
	void				Resize(int x, int y);

    int					m_nBorderType;		/*0 rect, 1 oval, 2 roundrect, 3, none	*/
	LOGFONT				m_logfont;
    COLORREF			m_color;							/*	color of text	*/
															
	Color				m_fontColor;
	CString				m_fontName;
	BYTE				m_fontSize;
	FontStyle           m_fontStyle;
	StringTrimming		m_trimmingSyle;
	StringFormatFlags   m_format;
	StringAlignment     m_fontAlignment;
	friend class CTextTool;
	friend class CReactantTool;
	friend class CTextFormatPage;
#ifdef _DEBUG
	virtual void Dump( CDumpContext& dc ) const;
#endif
};


/////////////////////////////////////////////////////////////////////////////
// CDrawPoly 
/////////////////////////////////////////////////////////////////////////////
class CDrawPoly;

#define POLYGON				0
#define POLYLINE			1
#define POLYCURVE			2
#define POLYCLOSEDCURVE		3

class CDrawPoly : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawPoly);
	CDrawPoly();

public:
	CDrawPoly(const CRect& position);

// Operations
	BOOL RecalcBounds(CNetworkView* pView = NULL);
	void AddPoint(const CPoint& point, CNetworkView* pView = NULL);

// Implementation
public:
	virtual ~CDrawPoly();
	virtual void Serialize(CArchive& ar);
	virtual void	Draw(CDC*);
	virtual void MoveTo(const CRect& position, CNetworkView* pView = NULL);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CNetworkView* pView = NULL);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void RetrieveData(CDrawObj *pObj);  //help function for CEditAction
	virtual void OnEditProperties(CNetworkView* pView);
	virtual void Shift(int x, int y);
protected:
	int			m_nPoint,
				m_nAllocPoints;
	CPoint*		m_points;
	CDrawPoly*	m_pDrawObj;
	BYTE		m_type;
	CArray<POINT, POINT> m_CurvePoints;

	friend class CPolyTool;
	friend class CScribbleTool;
	friend class CCurveTool;
	friend class CClosedCurveTool;
#ifdef _DEBUG
	virtual void Dump( CDumpContext& dc ) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CDrawOle 
/////////////////////////////////////////////////////////////////////////////
class CDrawCntrItem;    // COleClientItem derived class
class CDrawOleObj : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawOleObj);
	CDrawOleObj();

public:
	CDrawOleObj(const CRect& position);

// Implementation
public:
	virtual void Serialize(CArchive& ar);
	virtual void		Draw(CDC*);
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void OnOpen(CNetworkView* pView);
	virtual void MoveTo(const CRect& positon, CNetworkView* pView = NULL);
	virtual void OnEditProperties(CNetworkView* pView);
	virtual void Remove();
	virtual ~CDrawOleObj();
	static BOOL c_bShowItems;

	CDrawCntrItem*	m_pClientItem;
	// current extent is tracked separate from scaled position
	CSize				m_extent;
#ifdef _DEBUG
	virtual void Dump( CDumpContext& dc ) const;
#endif
};


void Normallize(Rect &);
void Normallize(RectF &);

#endif // __DRAWOBJ_H__

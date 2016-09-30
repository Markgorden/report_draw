// uizoom.h : header file
//				  Implements Zooming functions in a CScrollView window
//
// Written by Brad Pirtle, CS:72450,1156, Internet:pirtle@qlogic.com
// Copyright 1994, QuickLogic Corp., all rights reserved.
// Version 1.0
#ifndef ZOOMVIEW_H
#define ZOOMVIEW_H

/////////////////////////////////////////////////////////////////////////////
// CZoomView view
class CZoomView : public CScrollView
{
	DECLARE_DYNCREATE(CZoomView)
protected:
	CZoomView();			// protected constructor used by dynamic creation

// Operations
public:
	// Overridden CScrollView member functions
	void SetZoomSizes(SIZE sizeTotal, const SIZE& sizePage = sizeDefault, const SIZE& sizeLine = sizeDefault);
	void CenterOnLogicalPoint(CPoint ptCenter);
	CPoint GetLogicalCenterPoint(void);

	// Zooming functions
	typedef enum {MODE_ZOOMOFF, MODE_ZOOMIN, MODE_ZOOMOUT} ZoomMode_;
	void      SetZoomMode(ZoomMode_ zoomMode);
	ZoomMode_ GetZoomMode() {return m_zoomMode;};
	int       DoZoomIn    (CRect &rect);
	int       DoZoomIn    (CPoint *point = NULL, float delta = 1.25);
	int       DoZoomOut   (CPoint *point = NULL, float delta = 1.25);
	int       DoZoomFull  ();

	// Override this to get notified of zoom scale change
	virtual void NotifyZoom(void) {};

	// Zooming utility functions
	void      ViewDPtoLP (LPPOINT lpPoints, int nCount = 1);
	void      ViewLPtoDP (LPPOINT lpPoints, int nCount = 1);
	void      ClientToDevice(CPoint &point);
	void      NormalizeRect(CRect &rect);
	void 		 DrawBox(CDC &dc, CRect &rect, BOOL xor = TRUE);
	void 		 DrawLine(CDC &dc, const int &x1, const int &y1, 
							 const int &x2, const int &y2, BOOL xor = TRUE);
	void DoLButtonDown(UINT nFlags, CPoint point);
	void DoLButtonUp(UINT nFlags, CPoint point);
	void DoMouseMove(UINT nFlags, CPoint point);
	void DoRButtonDown(UINT nFlags, CPoint point);

	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

// Implementation
protected:
	virtual ~CZoomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif //_DEBUG
	virtual void OnDraw(CDC* pDC) {CZoomView::OnDraw(pDC);}; // Pure Virtual
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

private:
	// Internally called functions
	void 		 PersistRatio(const CSize &orig, CSize &dest, CPoint &remainder);
	void      CalcBars(void);

	// Private member variables
	ZoomMode_ m_zoomMode;
	BOOL      m_bCaptured;
	CRect     m_ptDragRect;
	CSize     m_origTotalDev;           // Origional total size in device units
	CSize     m_origPageDev;            // Origional per page scroll size in device units
	CSize     m_origLineDev;            // Origional per line scroll size in device units
	CSize	  m_origVPSize;				// Origional view portsize;
	CSize     m_viewportSize;
	HCURSOR   m_hZoomCursor;
protected:
	float     m_zoomScale;

public:
	int DoZoomOriginal();
	int GetZoomRatio();
	// Generated message map functions
	//{{AFX_MSG(CZoomView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
#endif

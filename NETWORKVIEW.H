/********************************************************************/
/* NetworkView.h: Interface for CNetworkView			            */
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 2001, all rights reserved.				*/
/********************************************************************/

#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#endif
#include "zoomview.h"
#include "drawobj.h"
class CDrawObj;
class CDrawDoc;
class CMAction;
/////////////////////////////////////////////////////////////////////////////
// CNetworkView view, it is derived from CZoomView to have zooming function

class CNetworkView : public CZoomView
{
protected:
	CNetworkView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CNetworkView)

// Attributes
public:
	CDrawDoc*		GetDocument(){ return (CDrawDoc*)m_pDocument; }
	void			SetPageSize(CSize size);
	CRect			GetInitialPosition();

// Operations
public:
	void			DocToClient(CRect& rect);
	void			DocToClient(CPoint& point);
	void			ClientToDoc(CPoint& point);
	void			ClientToDoc(CRect& rect);
	void			Select(CDrawObj* pObj, BOOL bAdd = FALSE);
	void			SelectWithinRect(CRect rect, BOOL bAdd = FALSE);
	void			Deselect(CDrawObj* pObj);
	void			CloneSelection();
	void			UpdateActiveItem();
	void			InvalObj(CDrawObj* pObj);
	void			Remove(CDrawObj* pObj);
	void			PasteNative(COleDataObject& dataObject);
	void			PasteEmbedded(COleDataObject& dataObject, CPoint point );
	void			SetTextColor (COLORREF color);
	void			SetLineColor (COLORREF color);
	void			SetFillColor (COLORREF color);
// Implementation
protected:
	// added for drop-target functionality
	COleDropTarget	m_dropTarget;   /* for drop target functionality		   */
	CPoint			m_dragPoint;    /* current position						   */
	CSize			m_dragSize;     /* size of dragged object				   */
	CSize			m_dragOffset;   /* offset between pt and drag object corner*/
	DROPEFFECT		m_prevDropEffect;
	BOOL			m_bDragDataAcceptable;
	void *			actionTarget;
	bool			redraw;
	BOOL			locked;
					// last character typed in this edit view
	BOOL			GetObjectInfo(COleDataObject* pDataObject,
						CSize* pSize, CSize* pOffset);
	// end of drop-target additions
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNetworkView();
#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif
public:
	virtual void	OnPrint(CDC* pDC, CPrintInfo* pInfo);  // overriden to record time/date
	virtual void	OnActivateView(BOOL bActivate, CView* pActiveView, CView* pDeactiveView);
	virtual void	OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void	OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL	OnScrollBy(CSize sizeScroll, BOOL bDoScroll);
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);
	void			DrawGrid(CDC* pDC);

	// added for drop-target functionality
	virtual BOOL		OnDrop(COleDataObject* pDataObject,
						DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT	OnDragEnter(COleDataObject* pDataObject,
						DWORD grfKeyState, CPoint point);
	virtual DROPEFFECT	OnDragOver(COleDataObject* pDataObject,
						DWORD grfKeyState, CPoint point);
	virtual void		OnDragLeave();
	static CLIPFORMAT	m_cfObjectDescriptor;
	// end of drop-target additions

	static CLIPFORMAT	m_cfDraw; // custom clipboard format

	CDrawObjList		m_selection;

	COLORREF			m_gridColor,
						m_lineColor,
						m_fillColor,
						m_textColor,
						m_clrDefault; //default char color
	
	BOOL				m_bActive; // is the view active?
	BOOL				m_bGrid;
	int					m_bShowFlux;
	CCharFormat			m_defaultCharFormat;
	CMAction			*lastAction;
	int					n_Copies; //how many pasting has been made after one copy
protected:

	// Printing support
	virtual BOOL		OnPreparePrinting(CPrintInfo* pInfo);
	virtual void		OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void		OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// OLE Container support
public:
	void				MoveSelection(UINT direction);
	void				SetCharFormat( CCharFormat cf );
	CCharFormat&		GetCharFormatSelection();
	virtual BOOL		IsSelected(const CObject* pDocItem) const;
//redo/undo supprot
public:
	void				GetDefaultFont(CCharFormat& cf, UINT nFontNameID);
	int					SearchStr ( CString&, BOOL );
	bool				CanAddNewAction();
	void				RemoveLastAction();
	void*				GetActionTarget() const		{ return actionTarget; }
	void				SetActionTarget(void *at)	{ actionTarget = at; }
// Generated message map functions
protected:
	//{{AFX_MSG(CNetworkView)
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEdit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDrawSelect();
	afx_msg void OnDrawRoundRect();
	afx_msg void OnDrawRect();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRoundRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSingleSelect(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateAnySelect(CCmdUI* pCmdUI);
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnObjectFillColor();
	afx_msg void OnObjectLineColor();
	afx_msg void OnObjectMoveBack();
	afx_msg void OnObjectMoveForward();
	afx_msg void OnObjectMoveToBack();
	afx_msg void OnObjectMoveToFront();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnViewShowObjects();
	afx_msg void OnUpdateViewShowObjects(CCmdUI* pCmdUI);
	afx_msg void OnEditProperties();
	afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCharColor();
	afx_msg void OnFillColor();
	afx_msg void OnLineColor();
	afx_msg void OnDrawArrow();
	afx_msg void OnUpdateDrawArrow(CCmdUI* pCmdUI);
	afx_msg void OnDrawText();
	afx_msg void OnUpdateDrawText(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomFull();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnViewZoom100();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnAddPage();
	afx_msg void OnRemovePage();
	afx_msg void OnUpdateRemovePage(CCmdUI* pCmdUI);
	afx_msg void OnDrawScribble();
	afx_msg void OnDrawCurve();
	afx_msg void OnUpdateDrawScribble(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCurve(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFontname();
	afx_msg void OnFontsize();
	afx_msg void OnDrawClosedcurve();
	afx_msg void OnUpdateDrawClosedcurve(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnBarSetFocus(UINT, NMHDR*, LRESULT*);
	afx_msg void OnBarKillFocus(UINT, NMHDR*, LRESULT*);
	afx_msg void OnBarReturn(UINT, NMHDR*, LRESULT* );
	afx_msg void OnLineWidth(UINT nID);
	afx_msg void OnLineStyle(UINT nID);
	afx_msg void OnLeftArrow(UINT nID);
	afx_msg void OnRightArrow(UINT nID);
	//afx_msg void OnFormatFont();
	DECLARE_MESSAGE_MAP()
	friend class CMainFrame;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined NETWORKVIEW_H

// NetworkView.cpp : implementation file
//
#include "stdafx.h"
#include "Draw.h"
#include "NetworkView.h"
#include "drawobj.h"
#include "cntritem.h"
#include "Drawdoc.h"
#include "drawtool.h"
#include "mainfrm.h"
#include "DrawProperty.h"
#include "UNDO MANAGER\UMResources.h"
#include "UNDO MANAGER\UndoManager.h"
#include "MAction.h"
#include <direct.h>
#include "Lineproperty.h"

#define  BUFSIZE 256

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CPoint lastPoint;

// private clipboard format (list of Draw objects)
CLIPFORMAT CNetworkView::m_cfDraw = (CLIPFORMAT)
	::RegisterClipboardFormat(_T("MetaLab"));

CLIPFORMAT CNetworkView::m_cfObjectDescriptor = NULL;


IMPLEMENT_DYNCREATE(CNetworkView, CZoomView)

/////////////////////////////////////////////////////////////////////////////
// CNetworkView

BEGIN_MESSAGE_MAP(CNetworkView, CZoomView)
	//{{AFX_MSG_MAP(CNetworkView)
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT, OnCancelEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW_SELECT, OnDrawSelect)
	ON_COMMAND(ID_DRAW_ROUNDRECT, OnDrawRoundRect)
	ON_COMMAND(ID_DRAW_RECT, OnDrawRect)
	ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, OnUpdateDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, OnUpdateDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, OnUpdateDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDRECT, OnUpdateDrawRoundRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SELECT, OnUpdateDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateSingleSelect)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateAnySelect)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_OBJECT_FILLCOLOR, OnObjectFillColor)
	ON_COMMAND(ID_OBJECT_LINECOLOR, OnObjectLineColor)
	ON_COMMAND(ID_OBJECT_MOVEBACK, OnObjectMoveBack)
	ON_COMMAND(ID_OBJECT_MOVEFORWARD, OnObjectMoveForward)
	ON_COMMAND(ID_OBJECT_MOVETOBACK, OnObjectMoveToBack)
	ON_COMMAND(ID_OBJECT_MOVETOFRONT, OnObjectMoveToFront)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_VIEW_SHOWOBJECTS, OnViewShowObjects)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWOBJECTS, OnUpdateViewShowObjects)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditProperties)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHAR_COLOR, OnCharColor)
	ON_COMMAND(ID_FILL_COLOR, OnFillColor)
	ON_COMMAND(ID_LINE_COLOR, OnLineColor)
	ON_COMMAND(ID_DRAW_ARROW, OnDrawArrow)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARROW, OnUpdateDrawArrow)
	ON_COMMAND(ID_DRAW_TEXT, OnDrawText)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TEXT, OnUpdateDrawText)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_FULL, OnZoomFull)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_VIEW_ZOOM_100, OnViewZoom100)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_ADD_PAGE, OnAddPage)
	ON_COMMAND(ID_REMOVE_PAGE, OnRemovePage)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_PAGE, OnUpdateRemovePage)
	ON_COMMAND(ID_DRAW_SCRIBBLE, OnDrawScribble)
	ON_COMMAND(ID_DRAW_CURVE, OnDrawCurve)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SCRIBBLE, OnUpdateDrawScribble)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CURVE, OnUpdateDrawCurve)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(IDC_FONTNAME, OnFontname)
	ON_COMMAND(IDC_FONTSIZE, OnFontsize)
	ON_COMMAND(ID_DRAW_CLOSEDCURVE, OnDrawClosedcurve)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CLOSEDCURVE, OnUpdateDrawClosedcurve)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOBACK, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOFRONT, OnUpdateSingleSelect)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)

//	ON_NOTIFY_RANGE(FN_GETFORMAT, AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnGetCharFormat)
//	ON_NOTIFY_RANGE(FN_SETFORMAT, AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnSetCharFormat)


	ON_NOTIFY_RANGE(NM_SETFOCUS, AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnBarSetFocus)
	ON_NOTIFY_RANGE(NM_KILLFOCUS, AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnBarKillFocus)
	ON_NOTIFY_RANGE(NM_RETURN, AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnBarReturn)

	ON_COMMAND_RANGE(ID_LINEWIDTH0, ID_LINEWIDTH8, OnLineWidth)
	ON_COMMAND_RANGE(ID_LINESTYLE0, ID_LINESTYLE8, OnLineStyle)
	ON_COMMAND_RANGE(ID_LEFTARROW0, ID_LEFTARROW5, OnLeftArrow)
	ON_COMMAND_RANGE(ID_RIGHTARROW0, ID_RIGHTARROW5, OnRightArrow)

	
	ON_CBN_SELENDOK(IDC_FONTNAME, OnFontname)
	ON_CBN_SELENDOK(IDC_FONTSIZE, OnFontsize)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkView construction/destruction

CNetworkView::CNetworkView()
{
	m_bGrid = FALSE;
	m_gridColor = RGB(0, 0, 128);
	m_lineColor = RGB(0, 0, 0);
	m_fillColor = RGB(64,64,64);
	m_textColor = RGB(0,0,0);
	m_bActive = FALSE;
	m_bShowFlux=0;
	if( m_cfObjectDescriptor == NULL )
		m_cfObjectDescriptor = (CLIPFORMAT)::RegisterClipboardFormat(_T("Object Descriptor") );
	m_prevDropEffect = DROPEFFECT_NONE;
	lastAction=0;
	redraw=true;
	locked=FALSE;
	actionTarget=0;
	n_Copies =0;
}

CNetworkView::~CNetworkView()
{
}
/////////////////////////////////////////////////////////////////////////////
// CNetworkView drawing
BOOL CNetworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.style & WS_CHILD);
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return TRUE;
}

void CNetworkView::OnActivateView(BOOL bActivate, CView* pActiveView,
	CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActiveView, pDeactiveView);
	// force menu rebuild by creating "NULL" action
	// (eventually, the notification callback calls
	// the appropriate menu building routine)
	//if (bActivate && IsWindowVisible())
		//gUndoManager->SetLastAction(GetActionTarget(), 0);
	// invalidate selections when active status changes
	if (m_bActive != bActivate)
	{
		if (bActivate)  // if becoming active update as if active
			m_bActive = bActivate;
		if (!m_selection.IsEmpty())
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_bActive = bActivate;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CNetworkView drawing
// make the updated area a little bigger than needed, consider the drager stuff

void CNetworkView::InvalObj(CDrawObj* pObj)
{
	CRect rect = pObj->m_position;
	DocToClient(rect);
	if (m_bActive  && IsSelected(pObj))
	{
		rect.left -= (long)(5*m_zoomScale);
		rect.top -= (long)(5*m_zoomScale);
		rect.right +=(long)(5*m_zoomScale);
		rect.bottom += (long)(5*m_zoomScale);
	}
	rect.InflateRect(int(m_zoomScale), int (m_zoomScale)); // handles CDrawOleObj objects
	rect.InflateRect(pObj->m_penWidth,pObj->m_penWidth);
	rect.InflateRect((pObj->m_startArrowSize + pObj->m_endArrowSize)*pObj->m_penWidth + 5, 
			(pObj->m_startArrowSize + pObj->m_endArrowSize)*pObj->m_penWidth + 5);

	InvalidateRect(rect, FALSE);
}

void CNetworkView::OnUpdate(CView* , LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		InvalObj((CDrawObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			CDrawObjList* pList = pHint != NULL ?
				(CDrawObjList*)pHint : &m_selection;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
				InvalObj(pList->GetNext(pos));
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		if (pHint != &m_selection)
		{
			CDrawObjList* pList = (CDrawObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObj* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);   // remove it from this view's selection
			}
		}
		break;

	case HINT_UPDATE_OLE_ITEMS:
		{
			CDrawDoc* pDoc = GetDocument();
			POSITION pos = pDoc->GetObjects()->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
				if (pObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
					InvalObj(pObj);
			}
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}

void CNetworkView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if (!pDC->IsPrinting() || pInfo->m_bPreview)
		CZoomView::OnPrepareDC(pDC, pInfo);
	else{
		CZoomView::OnPrepareDC(pDC, pInfo);
		//I really don't know why there is trouble, when printing GDI+ output to the priter, using MM_ANISOTROPIC mode
		// i test that MM_TEXT mode is OK. But my project us MM_ANISOTROPIC. I have resize viewpoint for the CDC before
		// painting.
		CSize size= pDC->GetViewportExt();
		int x = pDC->GetDeviceCaps(LOGPIXELSX) / 100;
		int y = pDC->GetDeviceCaps(LOGPIXELSY) / 100;

		pDC->SetViewportExt(CSize(size.cx/x, size.cy/y));
		CPoint p = pDC->GetWindowOrg();
		
		pDC->SetViewportOrg(-p.x*x, p.y*y);
		pDC->OffsetViewportOrg(p.x,-p.y);

	}

}

BOOL CNetworkView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	
	if (bDoScroll)	{
		if (!CScrollView::OnScrollBy(sizeScroll, bDoScroll))
			return FALSE;
	// THIS CODE CAUSE PROBLEM IN WIN98
	// update the position of any in-place active item
		Invalidate();
		UpdateActiveItem();
		UpdateWindow();
	}
	return TRUE;
}

void CNetworkView::OnDraw(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);
	CRect rect = client;
	DocToClient(rect);

	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}
	else {//printing
	}
	// paint background

	if(!pDC->IsPrinting()){
		CBrush brush;

		if (!brush.CreateSolidBrush(RGB(128,128,128)))
			return;

		brush.UnrealizeObject();
		pDrawDC->FillRect(client, &brush);

	}

	CBrush brush1;
	if (!brush1.CreateSolidBrush(pDoc->GetPaperColor()))
		return;

	CRect rect1;
	rect1.left = -pDoc->GetSize().cx / 2;
	rect1.top = -pDoc->GetSize().cy / 2;
	rect1.right = rect1.left + pDoc->GetSize().cx;
	rect1.bottom = rect1.top + pDoc->GetSize().cy;

	brush1.UnrealizeObject();

	pDrawDC->FillRect(rect1, &brush1);

	//draw the boarder and shadow
	if(!pDC->IsPrinting()){
		CBrush brush2;
		if (!brush2.CreateSolidBrush(RGB(0,0,0)))
			return;
		pDrawDC->Draw3dRect(rect1,RGB(128,128,128),RGB(0,0,0));	
		//print page : 1 at the cornner;
		CString s;
		s.Format("PAGE : %d",GetDocument()->m_nCurrentPage);
		LOGFONT m_logfont;
		strcpy(m_logfont.lfFaceName, "Arial");
		m_logfont.lfWeight=FW_REGULAR;
		m_logfont.lfWidth=0;
		m_logfont.lfHeight=-16;
		m_logfont.lfItalic=FALSE;
		m_logfont.lfOrientation=0;
		m_logfont.lfWeight=400;
		m_logfont.lfEscapement=0;
		m_logfont.lfUnderline=FALSE;
		m_logfont.lfStrikeOut=FALSE;
		m_logfont.lfOutPrecision=OUT_DEVICE_PRECIS;
		m_logfont.lfCharSet=ANSI_CHARSET;
		m_logfont.lfPitchAndFamily=18;
		CFont font;
		if (!font.CreateFontIndirect(&m_logfont))
			return;
		CFont *pOldFont=(CFont *)pDC->SelectObject(&font);
		pDrawDC->SetTextColor(RGB(255,255,200));
		pDrawDC->SetBkColor(RGB(128,128,128));
		pDrawDC->TextOut(rect1.left,rect1.bottom,s);
		pDrawDC->SelectObject(pOldFont);
	}

	
	if (!pDC->IsPrinting() && m_bGrid)
		DrawGrid(pDrawDC);

	pDoc->Draw(pDrawDC, this); //print or drawing
 
	if (pDrawDC != pDC)//drawing in the view
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left-2, rect.top-2, rect.Width()+4, rect.Height()+4,
			&dc, -2, -2, SRCCOPY); //Modifed on March 20,2001, to kill the bug of updating
		dc.SelectObject(pOldBitmap);
	}
}

void CNetworkView::Remove(CDrawObj* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL)
		m_selection.RemoveAt(pos);
}

void CNetworkView::PasteNative(COleDataObject& dataObject)
{
	// get file refering to clipboard data
	CFile* pFile = dataObject.GetFileData(m_cfDraw);
	if (pFile == NULL)
		return;

	// connect the file to the archive
	CArchive ar(pFile, CArchive::load);
	TRY
	{
		ar.m_pDocument = GetDocument(); // set back-pointer in archive

		// read the selection
		m_selection.Serialize(ar);
	}
	CATCH_ALL(e)
	{
		ar.Close();
		delete pFile;
		THROW_LAST();
	}
	END_CATCH_ALL

	ar.Close();
	delete pFile;
}

void CNetworkView::PasteEmbedded(COleDataObject& dataObject, CPoint point )
{
	BeginWaitCursor();

	// paste embedded
	CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
	ASSERT_VALID(pObj);
	CDrawCntrItem* pItem = new CDrawCntrItem(GetDocument(), pObj);
	ASSERT_VALID(pItem);
	pObj->m_pClientItem = pItem;
	lastAction = new CAddAction(this, ID_ACTION_ADD_COMMON,pObj);

	TRY
	{
		if (!pItem->CreateFromData(&dataObject) &&
			!pItem->CreateStaticFromData(&dataObject))
		{
			AfxThrowMemoryException();      // any exception will do
		}

		// add the object to the document
		lastAction->Do();
		//GetDocument()->Add(pObj);
		//m_selection.AddTail(pObj);
		ClientToDoc( point );
		//pObj->MoveTo( CRect( point, pObj->m_extent ), this );

		// try to get initial presentation data
		pItem->UpdateLink();
		pItem->UpdateExtent();
	}
	CATCH_ALL(e)
	{
		// clean up item
		pItem->Delete();
		pObj->m_pClientItem = NULL;
		GetDocument()->Remove(pObj);
		delete lastAction;
		//pObj->Remove(); will be deleted in the distructor of CAddAction


		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH_ALL

	EndWaitCursor();
}

void CNetworkView::DrawGrid(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();

	COLORREF oldBkColor = pDC->SetBkColor(pDoc->GetPaperColor());

	CRect rect;
	rect.left = -pDoc->GetSize().cx / 2;
	rect.top = -pDoc->GetSize().cy / 2;
	rect.right = rect.left + pDoc->GetSize().cx;
	rect.bottom = rect.top + pDoc->GetSize().cy;

	// Center lines
	CPen penDash;
	penDash.CreatePen(PS_DASH, 1, m_gridColor);
	CPen* pOldPen = pDC->SelectObject(&penDash);

	pDC->MoveTo(0, rect.top);
	pDC->LineTo(0, rect.bottom);
	pDC->MoveTo(rect.left, 0);
	pDC->LineTo(rect.right, 0);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_DOT, 1, m_gridColor);
	pDC->SelectObject(&penDot);

	for (int x = rect.left / 50 * 50; x < rect.right; x += 50)
	{
		if (x != 0)
		{
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);
		}
	}

	for (int y = rect.top / 50 * 50; y < rect.bottom; y += 50)
	{
		if (y != 0)
		{
			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);
		}
	}

	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 1, m_gridColor);
	pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldPen);
	pDC->SetBkColor(oldBkColor);


	
}

void CNetworkView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
	CSize size = GetDocument()->GetSize();
	SetZoomSizes(size);
}

void CNetworkView::SetPageSize(CSize size)
{
	SetZoomSizes(size);
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_WINDOW, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkView printing

BOOL CNetworkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(GetDocument()->m_nTotalPage);
	return DoPreparePrinting(pInfo);
}

void CNetworkView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnBeginPrinting(pDC,pInfo);
	// check page size -- user could have gone into print setup
	// from print dialog and changed paper or orientation
	GetDocument()->ComputePageSize();
}

void CNetworkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CNetworkView::IsSelected(const CObject* pDocItem) const
{
	CDrawObj* pDrawObj = (CDrawObj*)pDocItem;
	if (pDocItem->IsKindOf(RUNTIME_CLASS(CDrawCntrItem)))
		pDrawObj = ((CDrawCntrItem*)pDocItem)->m_pDrawObj;
	return m_selection.Find(pDrawObj) != NULL;
}

void CNetworkView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CDrawCntrItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	// First create the C++ object
	CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
	ASSERT_VALID(pObj);
	CDrawCntrItem* pItem = new CDrawCntrItem(GetDocument(), pObj);
	ASSERT_VALID(pItem);
	pObj->m_pClientItem = pItem;

	lastAction = new CAddAction(this, ID_ACTION_ADD_COMMON,pObj);
	// Now create the OLE object/item
	TRY
	{
		if (!dlg.CreateItem(pObj->m_pClientItem))
			AfxThrowMemoryException();

		lastAction->Do();
		// add the object to the document
		//GetDocument()->Add(pObj);

		// try to get initial presentation data
		pItem->UpdateLink();
		pItem->UpdateExtent();

		// if insert new object -- initially show the object
		if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);
	}
	CATCH_ALL(e)
	{
		// clean up item
		pItem->Delete();
		pObj->m_pClientItem = NULL;
		//remove last Action
		RemoveLastAction();
		lastAction = NULL;

		GetDocument()->Remove(pObj);
		//pObj->Remove();

		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH_ALL

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.
void CNetworkView::OnCancelEdit()
{
	// deactivate any in-place active item on this view!
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		// if we found one, deactivate it
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);

	// escape also brings us back into select mode
	ReleaseCapture();

	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnCancel();

	CDrawTool::c_drawShape = selection;
}

void CNetworkView::OnSetFocus(CWnd* pOldWnd)
{
	CDrawDoc *pDoc = GetDocument();
	COleClientItem* pActiveItem = pDoc->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();
			return;
		}
	}
	CScrollView::OnSetFocus(pOldWnd);
}

CRect CNetworkView::GetInitialPosition()
{
	CRect rect(10, 10, 10, 10);
	ClientToDoc(rect);
	return rect;
}

void CNetworkView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CNetworkView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom <= rect.top);
}

void CNetworkView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CNetworkView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CNetworkView::Select(CDrawObj* pObj, BOOL bAdd)
{
	if (!bAdd)
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	if (pObj == NULL || IsSelected(pObj))
		return;

    if (GetDocument()->m_nCurrentPage!=pObj->m_nPage) 
		return;

	m_selection.AddTail(pObj);
	InvalObj(pObj);
}

// rect is in device coordinates
void CNetworkView::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
		Select(NULL);

	ClientToDoc(rect);

	CDrawObjList* pObList = GetDocument()->GetObjects();
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj != NULL)
	{
		CDrawObj* pObj = pObList->GetNext(posObj);
		//if (pObj->Intersects(rect))
		if (pObj->InRect(rect))
			Select(pObj, TRUE);
	}
}

void CNetworkView::Deselect(CDrawObj* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL)
	{
		InvalObj(pObj);
		m_selection.RemoveAt(pos);
	}
}

void CNetworkView::CloneSelection()
{
	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->Clone(pObj->m_pDocument);
			// copies object and adds it to the document
	}
}

void CNetworkView::UpdateActiveItem()
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// this will update the item rectangles by calling
		//  OnGetPosRect & OnGetClipRect.
		pActiveItem->SetItemRects();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkView message handlers

void CNetworkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	if(lastAction)
		if (!lastAction->IsFinished())
			if (lastAction->GetActionType() == ID_ACTION_MOVE_ONE)
				lastAction->Finish();

	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonDown(this, nFlags, point);
}

void CNetworkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonUp(this, nFlags, point);
}

void CNetworkView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnMouseMove(this, nFlags, point);
}

void CNetworkView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonDblClk(this, nFlags, point);
}

void CNetworkView::OnDestroy()
{
	CScrollView::OnDestroy();

	// deactivate the inplace active item on this view
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}
}

void CNetworkView::OnDrawSelect()
{
	CDrawTool::c_drawShape = selection;
}

void CNetworkView::OnDrawRoundRect()
{
	CDrawTool::c_drawShape = roundRect;
}

void CNetworkView::OnDrawRect()
{
	CDrawTool::c_drawShape = rect;
}

void CNetworkView::OnDrawLine()
{
	CDrawTool::c_drawShape = line;
}

void CNetworkView::OnDrawEllipse()
{
	CDrawTool::c_drawShape = ellipse;
}

void CNetworkView::OnDrawPolygon()
{
	CDrawTool::c_drawShape = poly;
}

void CNetworkView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == ellipse);
}

void CNetworkView::OnUpdateDrawLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == line);
}

void CNetworkView::OnUpdateDrawRect(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == rect);
}

void CNetworkView::OnUpdateDrawRoundRect(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == roundRect);
}

void CNetworkView::OnUpdateDrawSelect(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == selection);
}

void CNetworkView::OnUpdateSingleSelect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_selection.GetCount() == 1);
}

void CNetworkView::OnEditSelectAll()
{
	CDrawObjList* pObList = GetDocument()->GetObjects();
	POSITION pos = pObList->GetHeadPosition();
	while (pos != NULL)
		Select(pObList->GetNext(pos), TRUE);
}

void CNetworkView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->GetObjects()->GetCount() != 0);
}

void CNetworkView::OnEditClear()
{
    //delete the seleciton
	// update all the views before the selection goes away
	GetDocument()->UpdateAllViews(NULL, HINT_DELETE_SELECTION, &m_selection);
	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	CDrawObj* pObj=m_selection.GetHead();
	// now remove the selection from the document
	lastAction = new CDeleteAction(this, ID_ACTION_DELETE, m_selection);
    lastAction->Do();
}

void CNetworkView::OnUpdateAnySelect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_selection.IsEmpty());
}

void CNetworkView::OnUpdateDrawPolygon(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == poly);
}

void CNetworkView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	UpdateActiveItem();
}

void CNetworkView::OnViewGrid()
{
	m_bGrid = !m_bGrid;
	Invalidate(FALSE);
}

void CNetworkView::OnUpdateViewGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bGrid);

}

BOOL CNetworkView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CNetworkView::OnObjectFillColor()
{
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	COLORREF color = dlg.GetColor();

	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetFillColor(color);
	}
}

void CNetworkView::OnObjectLineColor()
{
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	COLORREF color = dlg.GetColor();

	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetLineColor(color);
	}
}

void CNetworkView::OnObjectMoveBack()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
    
	//if this object is already the first object, don't move.
	CDrawObjList* pObjects = pDoc->GetObjects();

	if (pObj!=pObjects->GetHead()){
		lastAction = new CMoveOneStepAction(this, ID_ACTION_MOVE_POSITION, false, pObj);
		lastAction->Do();
	}
	else{
		AfxMessageBox("Cannot move backward, because it is already the last one!");
	}
}

void CNetworkView::OnObjectMoveForward()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();

	//if this object is already the first object, don't move.
	CDrawObjList* pObjects = pDoc->GetObjects();
	if (pObj!=pObjects->GetTail()){
		lastAction = new CMoveOneStepAction(this, ID_ACTION_MOVE_POSITION, true, pObj);
		lastAction->Do();
	}	
	else{
		AfxMessageBox("Cannot move forward, because it is already the first one!");
	}
}
	
void CNetworkView::OnObjectMoveToBack()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
	//if this object is already the first object, don't move.
	CDrawObjList* pObjects = pDoc->GetObjects();
	CDrawObj* pAfter= pDoc->FindObjectAfter(pObj);

	if (pObj!=pObjects->GetHead()){
		lastAction = new CMoveStepsAction(this, ID_ACTION_MOVE_POSITION, false, pAfter, pObj);
		lastAction->Do();
	}
	else{
		AfxMessageBox("Cannot move backward, because it is already the last one!");
	}
}

void CNetworkView::OnObjectMoveToFront()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
	CDrawObjList* pObjects = pDoc->GetObjects();
	CDrawObj* pAfter= pDoc->FindObjectAfter(pObj);
	if (pObj!=pObjects->GetTail()){
		lastAction = new CMoveStepsAction(this, ID_ACTION_MOVE_POSITION, true, pAfter, pObj);
		lastAction->Do();
	}	
	else{
		AfxMessageBox("Cannot move forward, because it is already the first one!");
	}
}

void CNetworkView::OnEditCopy()
{
	ASSERT_VALID(this);
	ASSERT(m_cfDraw != NULL);

	// Create a shared file and associate a CArchive with it
	CSharedFile file;
	CArchive ar(&file, CArchive::store);

	// Serialize selected objects to the archive
	//delete CFluxText from the selection first;

	m_selection.Serialize(ar);

	ar.Close();
    n_Copies = 0; //new copible data avaialbe;
	COleDataSource* pDataSource = NULL;
	TRY
	{
		pDataSource = new COleDataSource;
		// put on local format instead of or in addation to
		pDataSource->CacheGlobalData(m_cfDraw, file.Detach());

		// if only one item and it is a COleClientItem then also
		// paste in that format
		CDrawObj* pDrawObj = m_selection.GetHead();
		if (m_selection.GetCount() == 1 &&
			pDrawObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
		{
			CDrawOleObj* pDrawOle = (CDrawOleObj*)pDrawObj;
			pDrawOle->m_pClientItem->GetClipboardData(pDataSource, FALSE);
		}
		pDataSource->SetClipboard();
	}
	CATCH_ALL(e)
	{
		delete pDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL
}

void CNetworkView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	//at least one selected object should be non FluxText.
    int num = m_selection.GetCount();
	pCmdUI->Enable(num);
}

void CNetworkView::OnEditCut()
{
	OnEditCopy();
	OnEditClear();
}

void CNetworkView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_selection.IsEmpty());
}

void CNetworkView::OnEditPaste()
{
	COleDataObject dataObject;

	lastAction = new CPasteAction(this,ID_ACTION_PASTE);
	lastAction->Do();
/*
	dataObject.AttachClipboard();
	// invalidate current selection since it will be deselected
	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	m_selection.RemoveAll();


	if (dataObject.IsDataAvailable(m_cfDraw))
	{
		PasteNative(dataObject);

		// now add all items in m_selection to document
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
			GetDocument()->Add(m_selection.GetNext(pos));
	}
	else{
		
		PasteEmbedded(dataObject, GetInitialPosition().TopLeft() );
	}

	//GetDocument()->SetModifiedFlag();

	// invalidate new pasted stuff
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_SELECTION, &m_selection);*/
}

void CNetworkView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// determine if private or standard OLE formats are on the clipboard
	COleDataObject dataObject;
	BOOL bEnable = dataObject.AttachClipboard() &&
		(dataObject.IsDataAvailable(m_cfDraw) ||
		 COleClientItem::CanCreateFromData(&dataObject));

	// enable command based on availability
	pCmdUI->Enable(bEnable);
}

/*void CNetworkView::OnFilePrint()
{
	CScrollView::OnFilePrint();
	GetDocument()->ComputePageSize();
}
*/
void CNetworkView::OnViewShowObjects()
{
	CDrawOleObj::c_bShowItems = !CDrawOleObj::c_bShowItems;
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_OLE_ITEMS, NULL);
}

void CNetworkView::OnUpdateViewShowObjects(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CDrawOleObj::c_bShowItems);
}

void CNetworkView::OnEditProperties()
{
	if (m_selection.GetCount() == 1 && CDrawTool::c_drawShape == selection)
	{
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		ASSERT(pTool != NULL);
		pTool->OnEditProperties(this);
	}
}

void CNetworkView::OnUpdateEditProperties(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_selection.GetCount() == 1 &&
				   CDrawTool::c_drawShape == selection);
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkView diagnostics

#ifdef _DEBUG
void CNetworkView::AssertValid() const
{
	//CScrollView::AssertValid();
}

void CNetworkView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// new
// support for drag/drop

int CNetworkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

    //default font;
	GetDefaultFont(m_defaultCharFormat, IDS_DEFAULTFONT);



	if (GetActionTarget() == 0)
		SetActionTarget(GetDocument()); 

	// register drop target
	if( m_dropTarget.Register( this ) )
		return 0;
	else
		return -1;
}

BOOL CNetworkView::GetObjectInfo(COleDataObject* pDataObject,
	CSize* pSize, CSize* pOffset)
{
	ASSERT(pSize != NULL);

	// get object descriptor data
	HGLOBAL hObjDesc = pDataObject->GetGlobalData(m_cfObjectDescriptor);
	if (hObjDesc == NULL)
	{
		if (pOffset != NULL)
			*pOffset = CSize(0, 0); // fill in defaults instead
		*pSize = CSize(0, 0);
		return FALSE;
	}
	ASSERT(hObjDesc != NULL);

	// otherwise, got CF_OBJECTDESCRIPTOR ok.  Lock it down and extract size.
	LPOBJECTDESCRIPTOR pObjDesc = (LPOBJECTDESCRIPTOR)GlobalLock(hObjDesc);
	ASSERT(pObjDesc != NULL);
	pSize->cx = (int)pObjDesc->sizel.cx;
	pSize->cy = (int)pObjDesc->sizel.cy;
	if (pOffset != NULL)
	{
		pOffset->cx = (int)pObjDesc->pointl.x;
		pOffset->cy = (int)pObjDesc->pointl.y;
	}
	GlobalUnlock(hObjDesc);
	GlobalFree(hObjDesc);

	// successfully retrieved pSize & pOffset info
	return TRUE;
}

DROPEFFECT CNetworkView::OnDragEnter(COleDataObject* pDataObject,
	DWORD grfKeyState, CPoint point)
{
	ASSERT(m_prevDropEffect == DROPEFFECT_NONE);
	m_bDragDataAcceptable = FALSE;
	if (!COleClientItem::CanCreateFromData(pDataObject))
		return DROPEFFECT_NONE;

	m_bDragDataAcceptable = TRUE;
	GetObjectInfo(pDataObject, &m_dragSize, &m_dragOffset);
	CClientDC dc(NULL);
	dc.HIMETRICtoDP(&m_dragSize);
	dc.HIMETRICtoDP(&m_dragOffset);

	return OnDragOver(pDataObject, grfKeyState, point);
}

DROPEFFECT CNetworkView::OnDragOver(COleDataObject*,
	DWORD grfKeyState, CPoint point)
{
	if(m_bDragDataAcceptable == FALSE)
		return DROPEFFECT_NONE;

	point -= m_dragOffset;  // adjust target rect by original cursor offset

	// check for point outside logical area -- i.e. in hatched region
	// GetTotalSize() returns the size passed to SetScrollSizes
	CRect rectScroll(CPoint(0, 0), GetTotalSize());

	CRect rectItem(point,m_dragSize);
	rectItem.OffsetRect(GetDeviceScrollPosition());

	DROPEFFECT de = DROPEFFECT_NONE;
	CRect rectTemp;
	if (rectTemp.IntersectRect(rectScroll, rectItem))
	{
		// check for force link
		if ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
			de = DROPEFFECT_NONE; // Draw isn't a linking container
		// check for force copy
		else if ((grfKeyState & MK_CONTROL) == MK_CONTROL)
			de = DROPEFFECT_COPY;
		// check for force move
		else if ((grfKeyState & MK_ALT) == MK_ALT)
			de = DROPEFFECT_MOVE;
		// default -- recommended action is move
		else
			de = DROPEFFECT_MOVE;
	}

	if (point == m_dragPoint)
		return de;

	// otherwise, cursor has moved -- need to update the drag feedback
	CClientDC dc(this);
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		// erase previous focus rect
		dc.DrawFocusRect(CRect(m_dragPoint, m_dragSize));
	}
	m_prevDropEffect = de;
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		m_dragPoint = point;
		dc.DrawFocusRect(CRect(point, m_dragSize));
	}
	return de;
}

BOOL CNetworkView::OnDrop(COleDataObject* pDataObject,
	DROPEFFECT /*dropEffect*/, CPoint point)
{
	ASSERT_VALID(this);

	// clean up focus rect
	OnDragLeave();

	// offset point as appropriate for dragging
	GetObjectInfo(pDataObject, &m_dragSize, &m_dragOffset);
	CClientDC dc(NULL);
	dc.HIMETRICtoDP(&m_dragSize);
	dc.HIMETRICtoDP(&m_dragOffset);
	point -= m_dragOffset;

	// invalidate current selection since it will be deselected
	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	m_selection.RemoveAll();
	if (m_bDragDataAcceptable)
		PasteEmbedded(*pDataObject, point);

	// update the document and views
	GetDocument()->SetModifiedFlag();
	GetDocument()->UpdateAllViews(NULL, 0, NULL);      // including this view

	return TRUE;
}

void CNetworkView::OnDragLeave()
{
	CClientDC dc(this);
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		dc.DrawFocusRect(CRect(m_dragPoint,m_dragSize)); // erase previous focus rect
		m_prevDropEffect = DROPEFFECT_NONE;
	}
}


void CNetworkView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// make sure window is active
	GetParentFrame()->ActivateFrame();

	CPoint local = point;
	ScreenToClient(&local);
	ClientToDoc(local);

	CDrawObj* pObj;
	pObj = GetDocument()->ObjectAt(local);
	if(pObj != NULL)
	{
		if(!IsSelected(pObj))
			Select( pObj, FALSE );          // reselect item if appropriate
		UpdateWindow();

		CMenu menu;
		if (menu.LoadMenu(ID_POPUP_MENU))
		{
			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup != NULL);

			pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
								   point.x, point.y,
								   AfxGetMainWnd()); // route commands through main window
		}
	}
	//pObj==NULL deselect
	CDrawTool::c_drawShape = selection;

}

void CNetworkView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Draw(pDC, this, pInfo->m_nCurPage); //print this page only
}

void CNetworkView::OnCharColor() 
{

	POSITION pos = m_selection.GetHeadPosition();
	COLORREF color = CBCGPColorMenuButton::GetColorByCmdID (ID_CHAR_COLOR);
	lastAction = new CColorAction(this, ID_ACTION_COLOR_TEXT, m_selection);
	lastAction->Do();

	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetTextColor(color);
	}
}

void CNetworkView::OnFillColor() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_selection.GetHeadPosition();
	COLORREF color = CBCGPColorMenuButton::GetColorByCmdID (ID_FILL_COLOR);
	lastAction = new CColorAction(this, ID_ACTION_COLOR_FILL, m_selection);
	lastAction->Do();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetFillColor(color);
	}

}

void CNetworkView::OnLineColor() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_selection.GetHeadPosition();
	COLORREF color = CBCGPColorMenuButton::GetColorByCmdID (ID_LINE_COLOR);
	lastAction = new CColorAction(this, ID_ACTION_COLOR_LINE, m_selection);
	lastAction->Do();

	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetLineColor(color);
	}
}

void CNetworkView::SetTextColor (COLORREF color)
{
	m_textColor = color;
}

void CNetworkView::SetLineColor (COLORREF color)
{
	m_lineColor = color;
}

void CNetworkView::SetFillColor (COLORREF color)
{
	m_fillColor = color;
}


void CNetworkView::OnDrawArrow() 
{
	CDrawTool::c_drawShape = arrow;
}

void CNetworkView::OnUpdateDrawArrow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == arrow);
}

void CNetworkView::OnDrawText() 
{
	CDrawTool::c_drawShape = text;

}

void CNetworkView::OnUpdateDrawText(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == text);
}


void CNetworkView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//34 page down; 33 page Up
	CPoint p=GetScrollPosition(); //logical unit
	CSize size=GetTotalSize(); 
	int dy=size.cy/3;
	CRect myRect;
	GetClientRect(myRect);
	int h= myRect.bottom-myRect.top;
	CPoint p2;


	if(nChar==33){ //page u
		p2.x = p.x;
		p2.y = p.y -dy;
		ScrollToPosition(p2);
		p2=GetScrollPosition();
		if (p.y==p2.y){//didnot move, so that is the top of the page
			int page=GetDocument()->m_nCurrentPage;
			GetDocument()->OnPagePrevious();
			if (page > 1){
				p.y = (size.cy)*m_zoomScale-h;
				ScrollToPosition(p);
			}
		}
	}
    else if(nChar==34){ //page down
		p2.x = p.x;
		p2.y = p.y + dy;
		if (h/m_zoomScale > size.cy){//didnot move, so that is the end of the page
			int page=GetDocument()->m_nCurrentPage;
			GetDocument()->OnNextPage();
			if (page < GetDocument()->m_nTotalPage){
				p2.y=0;
				ScrollToPosition(p2);
			}
			return;
		}
		ScrollToPosition(p2);
		p2=GetScrollPosition();
		if (p.y==p2.y || (p2.y+dy)/m_zoomScale > size.cy){//didnot move, so that is the end of the page
			int page=GetDocument()->m_nCurrentPage;
			GetDocument()->OnNextPage();
			if (page < GetDocument()->m_nTotalPage){
				p2.y=0;
				ScrollToPosition(p2);
			}
		}
	}
	else if (nChar>=37 && nChar <=40){ 
		MoveSelection(nChar);
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CNetworkView::OnZoomIn() 
{
//change zoommode only
	CDrawTool::c_drawShape = zoom;
	CWnd* pWin=((CWinApp *) AfxGetApp())->m_pMainWnd;
	if(GetZoomMode()==MODE_ZOOMIN){
		SetZoomMode(MODE_ZOOMOFF);
		pWin->SendMessage(WM_SETMESSAGESTRING,0,(LPARAM) (LPCSTR) "");
	}
	else{
		SetZoomMode(MODE_ZOOMIN);
		pWin->SendMessage(WM_SETMESSAGESTRING,0,(LPARAM) (LPCSTR) "Click to Zoom in");
	}
}

void CNetworkView::OnZoomOut() 
{
	CDrawTool::c_drawShape = zoom;
	// TODO: Add your command handler code here
	CWnd* pWin=((CWinApp *) AfxGetApp())->m_pMainWnd;
	if(GetZoomMode()==MODE_ZOOMOUT){
		SetZoomMode(MODE_ZOOMOFF);
		pWin->SendMessage(WM_SETMESSAGESTRING,0,(LPARAM) (LPCSTR) "");
	}
	else{
		SetZoomMode(MODE_ZOOMOUT);
		pWin->SendMessage(WM_SETMESSAGESTRING,0,(LPARAM) (LPCSTR) "Click to Zoom out");
	}
}

void CNetworkView::OnZoomFull() 
{
	// TODO: Add your command handler code here
	DoZoomFull();
}

void CNetworkView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(GetZoomMode() == MODE_ZOOMOUT &&CDrawTool::c_drawShape == zoom);
}

void CNetworkView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(GetZoomMode() == MODE_ZOOMIN && CDrawTool::c_drawShape == zoom);
	
}

/*
void CNetworkView::OnGetCharFormat(UINT i,NMHDR* pNMHDR, LRESULT* pResult){
	ASSERT(pNMHDR != NULL);
	ASSERT(pResult != NULL);
	((CHARHDR*)pNMHDR)->cf = m_defaultCharFormat; //GetCharFormatSelection();
	*pResult=1;
}

*/
/*
void CNetworkView::OnSetCharFormat(UINT i,NMHDR* pNMHDR, LRESULT* pResult){
	ASSERT(pNMHDR != NULL);
	ASSERT(pResult != NULL);

	CHARHDR* p=(CHARHDR*)pNMHDR;

	SetCharFormat(p->cf);
}
*/
void CNetworkView::OnBarSetFocus(UINT i, NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult=1;
}

void CNetworkView::OnBarKillFocus(UINT i, NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult=1;
}

void CNetworkView::OnBarReturn(UINT i, NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult=1;
}

void CNetworkView::OnEditRedo() 
{
	gUndoManager->Redo(GetActionTarget());
	
}

void CNetworkView::OnEditUndo() 
{
	gUndoManager->Undo(GetActionTarget());
}

CCharFormat& CNetworkView::GetCharFormatSelection()
{
	POSITION pos=m_selection.GetHeadPosition();
	while (pos){
		CDrawObj *pObj = m_selection.GetNext(pos);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawText))){
				m_defaultCharFormat=pObj->GetCharFormat();
				return m_defaultCharFormat;
		}
	}
// none selected, return default font.
	return m_defaultCharFormat;
}

void CNetworkView::SetCharFormat(CCharFormat cf)
{
//change the font for the select part
	POSITION pos=m_selection.GetHeadPosition();
	int n=0;
	while (pos){
		CDrawObj *pObj = m_selection.GetNext(pos);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawText)))
			n++;
	}
	
	if(!n) {
		return; //don't fire any action if nothing is gona to be changed.
	}

	lastAction = new CFontAction(this, ID_ACTION_FONT, m_selection);
	lastAction->Do();
	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetCharFormat(cf,this);
	}
}

void CNetworkView::MoveSelection(UINT direction)
{
    if(m_selection.GetCount() == 0) return;

	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);

	pTool->MoveSelection(this, direction);

}
void CNetworkView::OnLineWidth(UINT nID)
{
	nID-= ID_LINEWIDTH0; 
	POSITION pos = m_selection.GetHeadPosition();
	lastAction = new CLineSizeAction(this, ID_ACTION_LINE_SIZE, m_selection);
	lastAction->Do();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetLineWidth(nID);
		//pObj->OnModified(0,nID);
	}
}
void CNetworkView::OnLineStyle(UINT nID)
{
	nID-= ID_LINESTYLE0; 
	POSITION pos = m_selection.GetHeadPosition();
	lastAction = new CLineStyleAction(this, ID_ACTION_LINE_STYLE, m_selection);
	lastAction->Do();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetLineStyle((DashStyle)nID);
		//pObj->OnModified(1,nID);
	}
}
void CNetworkView::OnLeftArrow(UINT nID)
{
	nID-= ID_LEFTARROW0; 
	POSITION pos = m_selection.GetHeadPosition();
	lastAction = new CArrowAction(this, ID_ACTION_LEFT_ARROW, m_selection);
	lastAction->Do();


	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetArrowType(ID_ACTION_LEFT_ARROW, CLineProperty::GetArrowType(nID));
		//pObj->OnModified(2,nID);
	}
}
void CNetworkView::OnRightArrow(UINT nID)
{
	nID-= ID_RIGHTARROW0; 
	POSITION pos = m_selection.GetHeadPosition();
	lastAction = new CArrowAction(this, ID_ACTION_RIGHT_ARROW, m_selection);
	lastAction->Do();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->SetArrowType(ID_ACTION_RIGHT_ARROW, CLineProperty::GetArrowType(nID));
		//pObj->OnModified(3,nID);
	}
}

void CNetworkView::OnViewZoom100() 
{
	CZoomView::DoZoomOriginal()	;
}


void CNetworkView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if (gUndoManager->CanRedo(GetActionTarget()))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

}

void CNetworkView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if (gUndoManager->CanUndo(GetActionTarget()))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

void CNetworkView::RemoveLastAction()
{
	gUndoManager->RemoveLastAction(GetDocument());
}

bool CNetworkView::CanAddNewAction()
{
	if (lastAction==NULL) return true;
	if (lastAction->IsFinished()) return true;
	return false;
}


void CNetworkView::OnAddPage() 
{
	// add a page after current page
	lastAction = new CAddPageAction(this, ID_ACTION_PAGE_ADD);
	lastAction->Do();
}


void CNetworkView::OnRemovePage() 
{
	lastAction = new CDeletePageAction(this, ID_ACTION_PAGE_DELETE);
	lastAction->Do();
}

void CNetworkView::OnUpdateRemovePage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(	GetDocument()->m_nTotalPage>1);
}



void CNetworkView::OnDrawScribble() 
{
	CDrawTool::c_drawShape = scribble;
	}

void CNetworkView::OnDrawCurve() 
{
	CDrawTool::c_drawShape = curve;
}

void CNetworkView::OnUpdateDrawScribble(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == scribble);
	
}

void CNetworkView::OnUpdateDrawCurve(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == curve);
	
}


BOOL CNetworkView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	DWORD dwVersion = GetVersion();
 
	// Get major and minor version numbers of Windows
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	// Get build numbers for Windows NT or Win32s
	if (dwVersion < 0x80000000)                // Windows NT
		return CZoomView::OnMouseWheel(nFlags, zDelta, pt);	

	return FALSE;	
	/*	else if (dwWindowsMajorVersion < 4)        // Win32s
		dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
	else         // Windows 95 -- No build numbers provided
		dwBuild =  0;
 		//return FALSE;
*/	
}

void CNetworkView::GetDefaultFont(CCharFormat &cf, UINT nFontNameID)
{
	USES_CONVERSION;
	CString strDefFont;
	VERIFY(strDefFont.LoadString(nFontNameID));
	ASSERT(cf.cbSize == sizeof(CCharFormat));
	cf.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|CFM_SIZE|
		CFM_COLOR|CFM_OFFSET|CFM_PROTECTED;
	cf.dwEffects = CFE_AUTOCOLOR;
	cf.yHeight = 200; //10pt
	cf.yOffset = 0;
	cf.crTextColor = RGB(0, 0, 0);
	cf.bCharSet = 0;
	cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	ASSERT(strDefFont.GetLength() < LF_FACESIZE);
	lstrcpynA(cf.szFaceName, T2A((LPTSTR) (LPCTSTR) strDefFont), LF_FACESIZE);
	cf.dwMask |= CFM_FACE;
	cf.dwMask |= CFM_CHARSET; //tao, Mar 23, 2002
}

void CNetworkView::OnFontname() 
{
	USES_CONVERSION;

	CBCGPToolbarFontCombo* pSrcCombo = 
		(CBCGPToolbarFontCombo*) CBCGPToolbarComboBoxButton::GetByCmd (IDC_FONTNAME, TRUE);
	if (pSrcCombo == NULL)
	{
	//	CRichEditView::OnFormatFont ();
		return;
	}

	CCharFormat cf;
	cf.szFaceName[0] = NULL;
	cf.dwMask = CFM_FACE | CFM_CHARSET;

	CString str = pSrcCombo->GetText ();
	if (pSrcCombo->SetFont (str))
	{
		const CBCGPFontDesc* pDesc = pSrcCombo->GetFontDesc ();
		ASSERT_VALID (pDesc);
		ASSERT(pDesc->m_strName.GetLength() < LF_FACESIZE);

		lstrcpynA(cf.szFaceName,
			T2A((LPTSTR) (LPCTSTR) pDesc->m_strName), LF_FACESIZE);
		cf.bCharSet = pDesc->m_nCharSet;
		cf.bPitchAndFamily = pDesc->m_nPitchAndFamily;

		CBCGPToolbarFontSizeCombo* pSizeCombo =
			DYNAMIC_DOWNCAST (CBCGPToolbarFontSizeCombo, CBCGPToolbarFontSizeCombo::GetByCmd (IDC_FONTSIZE));
		if (pSizeCombo != NULL)
		{
			int nSize = pSizeCombo->GetTwipSize();
			if (nSize == -2) // error
			{
				AfxMessageBox(IDS_INVALID_NUMBER, MB_OK|MB_ICONINFORMATION);
				nSize = pSizeCombo->GetTwipsLast ();
			}
			else if ((nSize >= 0 && nSize < 20) || nSize > 32760)
			{
				AfxMessageBox(IDS_INVALID_FONTSIZE, MB_OK|MB_ICONINFORMATION);
				nSize = pSizeCombo->GetTwipsLast ();
			}

			pSizeCombo->RebuildFontSizes (pDesc->m_strName);
			pSizeCombo->SetTwipSize (nSize);
		}
	}

	SetCharFormat (cf);
	SetFocus ();
}

void CNetworkView::OnFontsize() 
{
	CBCGPToolbarFontSizeCombo* pSrcCombo = 
		(CBCGPToolbarFontSizeCombo*) CBCGPToolbarComboBoxButton::GetByCmd (IDC_FONTSIZE, TRUE);
	if (pSrcCombo == NULL)
	{
		return;
	}

	int nSize = pSrcCombo->GetTwipSize();
	if (nSize == -2)
	{
		AfxMessageBox(IDS_INVALID_NUMBER, MB_OK|MB_ICONINFORMATION);
	}
	else if ((nSize >= 0 && nSize < 20) || nSize > 32760)
	{
		AfxMessageBox(IDS_INVALID_FONTSIZE, MB_OK|MB_ICONINFORMATION);
	}
	else if (nSize > 0)
	{
		CCharFormat cf;
		cf.dwMask = CFM_SIZE;
		cf.yHeight = nSize;

		SetCharFormat (cf);
		SetFocus ();
	}
}

void CNetworkView::OnDrawClosedcurve() 
{
	CDrawTool::c_drawShape = closedcurve;
}

void CNetworkView::OnUpdateDrawClosedcurve(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(CDrawTool::c_drawShape == closedcurve);
}

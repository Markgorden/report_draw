#include "stdafx.h"
#include "Draw.h"
#include "DrawDoc.h"
#include "networkview.h"
#include "drawobj.h"
#include "drawtool.h"
//for dialog
#include "DrawProperty.h"
#include "LineProperty.h"
#include "TextProperty.h"
#include "TextFormatPage.h"
#include "maction.h"


//#include "Dlgs\RegulationProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawTool implementation

CPtrList CDrawTool::c_tools; //this a global variable

static CSelectTool		selectTool; //global
static CRectTool		lineTool(line);
static CRectTool		rectTool(rect);
static CRectTool		roundRectTool(roundRect);
static CRectTool		ellipseTool(ellipse);
static CRectTool		arrowTool(arrow);
static CPolyTool		polyTool;
static CTextTool		textTool;
static CZoomTool		zoomTool;
static CCurveTool		curveTool;
static CScribbleTool	scribbleTool;
static CClosedCurveTool closedcurveTool;
CPoint CDrawTool::c_down;
UINT CDrawTool::c_nDownFlags;
CPoint CDrawTool::c_last;
DrawShape CDrawTool::c_drawShape = selection;

int nDragHandle;

CDrawTool::CDrawTool(DrawShape drawShape)
{
	m_drawShape = drawShape;
	c_tools.AddTail(this);
}

CDrawTool* CDrawTool::FindTool(DrawShape drawShape)
{
	POSITION pos = c_tools.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawTool* pTool = (CDrawTool*)c_tools.GetNext(pos);
		if (pTool->m_drawShape == drawShape)
			return pTool;
	}

	return NULL;
}

void CDrawTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	// deactivate any in-place active item on this view!
	COleClientItem* pActiveItem = pView->GetDocument()->GetInPlaceActiveItem(pView);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
		ASSERT(pView->GetDocument()->GetInPlaceActiveItem(pView) == NULL);
	}

	pView->SetCapture();
	c_nDownFlags = nFlags;
	c_down = point;
	c_last = point;
}

void CDrawTool::OnLButtonDblClk(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
}

void CDrawTool::MoveSelection(CNetworkView* pView, UINT direction){

}


void CDrawTool::OnLButtonUp(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& point)
{
	ReleaseCapture();

	if (point == c_down)
		c_drawShape = selection;
}

void CDrawTool::OnMouseMove(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& point)
{
	c_last = point;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CDrawTool::OnEditProperties(CNetworkView* /*pView*/)
{
}

void CDrawTool::OnCancel()
{
	c_drawShape = selection;
}

////////////////////////////////////////////////////////////////////////////
// CResizeTool

enum SelectMode
{
	none,
	netSelect,
	move,
	size
};

SelectMode selectMode = none;

CPoint lastPoint;

CSelectTool::CSelectTool()
	: CDrawTool(selection)
{
	m_bSecondClick=false;
}

//Before mouse button is down. some object may already have been selected in the view
void CSelectTool::MoveSelection(CNetworkView* pView, UINT direction)
{
	//when arrows are touched in the view and selection tool is current activated.
	//move selections a little bit.
	//precondition : THERE ARE SOME OBJECTS SELECTED!!!
	POSITION pos = pView->m_selection.GetHeadPosition();

	while (pos != NULL)
	{
		CDrawObj* pObj = pView->m_selection.GetNext(pos);
		CRect position = pObj->m_position;
		if (selectMode == move) //Only move one. I think this is enough
		{

			if (direction>=37 && direction<=40)
				if (pView->lastAction==NULL||pView->lastAction->IsFinished()){
					pView->lastAction = new CMoveSingleAction(pView,ID_ACTION_MOVE_ONE, pObj);
					pView->lastAction->Do(); //Not finish
				}

			switch (direction){
	
			case 37://left arrow
				position += CSize(-1,0);
				break;
			case 38: // up
				if (pView->lastAction==NULL||pView->lastAction->IsFinished())
					pView->lastAction = new CMoveSingleAction(pView,ID_ACTION_MOVE_ONE, pObj);
				position += CSize(0,1);
				break;
			case 39: //right
				if (pView->lastAction==NULL||pView->lastAction->IsFinished())
					pView->lastAction = new CMoveSingleAction(pView,ID_ACTION_MOVE_ONE, pObj);
				position += CSize(1,0);
				break;
			case 40: //down
				if (pView->lastAction==NULL||pView->lastAction->IsFinished())
					pView->lastAction = new CMoveSingleAction(pView,ID_ACTION_MOVE_ONE, pObj);
				position += CSize(0,-1);
				break;
			}
			pObj->MoveTo(position, pView);
		}
	}


}
void CSelectTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CPoint local = point;
	pView->ClientToDoc(local);

	CDrawObj* pObj;
	selectMode = none;

	// Check for resizing (only allowed on single selections)
	if (pView->m_selection.GetCount() == 1)
	{
		pObj = pView->m_selection.GetHead();
		nDragHandle = pObj->HitTest(local, pView, TRUE);
		if (nDragHandle != 0)
			selectMode = size;
		// if not on the handle, nDragHandle=0
	}

	// See if the click was on an object, then select and start move.
	// But if the click was on a reaction, only select but not move it at the first time.
	// second mouse down can drag the handle. m_bSecondClick is used to count the number is mouse down.
	 
	if (selectMode == none)
	{
		pObj = pView->GetDocument()->ObjectAt(local);

		if (pObj != NULL)
		{
			selectMode = move;

			if (!pView->IsSelected(pObj))
				pView->Select(pObj, (nFlags & MK_SHIFT) != 0);

			// Ctrl+Click clones the selection...
			//if ((nFlags & MK_CONTROL) != 0)
			//	pView->CloneSelection();
		}
	}

	// Click on background, start a net-selection
	if (selectMode == none)
	{
		if ((nFlags & MK_SHIFT) == 0)
			pView->Select(NULL);

		selectMode = netSelect;

		CClientDC dc(pView);
		CRect rect(point.x, point.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
	}

	lastPoint = local;
	CDrawTool::OnLButtonDown(pView, nFlags, point);
}

void CSelectTool::OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if ((nFlags & MK_SHIFT) != 0)
	{
		// Shift+DblClk deselects object...
		CPoint local = point;
		pView->ClientToDoc(local);
		CDrawObj* pObj = pView->GetDocument()->ObjectAt(local);
		if (pObj != NULL)
			pView->Deselect(pObj);
	}
	else
	{
		// "Normal" DblClk opens properties, or OLE server...
		if (pView->m_selection.GetCount() == 1)
			pView->m_selection.GetHead()->OnOpen(pView);
		else if (pView->m_selection.GetCount() >1)
			OnEditProperties(pView);
	}

	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CSelectTool::OnEditProperties(CNetworkView* pView)
{
	if (pView->m_selection.GetCount() == 1)
		pView->m_selection.GetHead()->OnEditProperties(pView);

	//multiple choose
	else{
		//open properties
		CDrawProperty sheet ( _T("Properties") );
		CLineProperty linePage;
		linePage.m_penSize = 1;
		linePage.m_isArrow=true;
		pView->lastAction = new CEditMultiAction(pView,ID_ACTION_EDIT_MULTI, pView->m_selection);
//set the intial value of line page using the first selected object

		POSITION pos = pView->m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObj* pObj = pView->m_selection.GetNext(pos);
			linePage.SetValues(pObj);
			break;
		}

		sheet.AddPage( &linePage );

		if (sheet.DoModal() != IDOK){
			delete pView->lastAction;
			pView->lastAction=0;
			return;
		}

		pos = pView->m_selection.GetTailPosition();
		while (pos != NULL)
		{
			CDrawObj* pObj = pView->m_selection.GetPrev(pos);
			linePage.GetValues(pObj);

			pObj->Invalidate();
		}
		pView->lastAction->Do();
	}
}

void CSelectTool::OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CPoint local = point;
	pView->ClientToDoc(local);

	if (pView->lastAction)
		if (pView->lastAction->GetActionType() ==ID_ACTION_MOVE)
			if (!pView->lastAction->IsFinished())
				pView->lastAction->Do();

	if(selectMode ==size && pView->m_selection.GetCount() == 1 && nDragHandle){ 
		//nDraghandle might aleady be changed by MoveHandleTo() function
		CDrawObj *pObj = pView->GetDocument()->ObjectAt(local);
		if(pObj){ //actually, only if the obj is a CDrawReaction or CDrawRegulation, ObjectAt(local) returns the pointer.
			if (pView->lastAction)
				if (pView->lastAction->GetActionType() ==ID_ACTION_SIZE) //it might also be CAddAction
					pView->lastAction->Do();
		}
	}

	if (pView->GetCapture() == pView)
	{
		if (selectMode == netSelect)
		{
			CClientDC dc(pView);
			CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);

			pView->SelectWithinRect(rect, TRUE);
		}
		else if (selectMode != none) //selectMode == size or move
		{
			pView->GetDocument()->UpdateAllViews(pView);
			m_bSecondClick=true;
		}
	}

	CDrawTool::OnLButtonUp(pView, nFlags, point);
}


/*Notice, CReactionTool, CRectTool, CPolyTool, CRegulationTool will call SeletetTool::OnMouseMove()
  Don't new any action this is CReactionTool, because the arrow produced temperarly by CReactionTool
  is not a state of document, which should not be an action at all.
  */
void CSelectTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (pView->GetCapture() != pView)
	{
		if (c_drawShape == selection && pView->m_selection.GetCount() == 1)
		{
			CDrawObj* pObj = pView->m_selection.GetHead();
			CPoint local = point;
			pView->ClientToDoc(local);
			int nHandle = pObj->HitTest(local, pView, TRUE);
			if (nHandle != 0)
			{
				SetCursor(pObj->GetHandleCursor(nHandle));
				return; // bypass CDrawTool
			}
		}
		if (c_drawShape == selection)
			CDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	if (selectMode == netSelect)
	{
		CClientDC dc(pView);
		CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(c_down.x, c_down.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);

		CDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	CPoint local = point;
	pView->ClientToDoc(local);
	CPoint delta = (CPoint)(local - lastPoint);

	if (selectMode == move){
		bool makeMoveAction = true;
		if (makeMoveAction ){
			if (pView->CanAddNewAction())
				pView->lastAction = new CMoveAction(pView,ID_ACTION_MOVE, pView->m_selection);	
		}
	}
				

	POSITION pos = pView->m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pView->m_selection.GetNext(pos);
		CRect position = pObj->m_position;

		if (selectMode == move)
		{
				position += delta;
				pObj->MoveTo(position, pView);
		}
		else if (nDragHandle != 0 ){ //Sizing
			if (pView->CanAddNewAction()){
				pView->lastAction = new CSizeAction(pView,ID_ACTION_SIZE,nDragHandle, lastPoint, pObj);
			}
			pObj->MoveHandleTo(nDragHandle, local, pView);
		}
	}

	lastPoint = local;

	if (selectMode == size && c_drawShape == selection)
	{
		c_last = point;
		SetCursor(pView->m_selection.GetHead()->GetHandleCursor(nDragHandle));
		return; // bypass CDrawTool
	}

	c_last = point;

	if (c_drawShape == selection)
		CDrawTool::OnMouseMove(pView, nFlags, point);
}

////////////////////////////////////////////////////////////////////////////
// CRectTool (does rectangles, round-rectangles, line, arrow and  ellipses)

CRectTool::CRectTool(DrawShape drawShape)
	: CDrawTool(drawShape)
{
}

void CRectTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
//when mouse is down, an Object is newed, but if the up position is the same as down position
//this object will be deleted in OnLButtonUp.
	
	
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	CDrawRect* pObj = new CDrawRect(CRect(local, CSize(0, 0)));
	switch (m_drawShape)
	{
	default:
		ASSERT(FALSE); // unsuported shape!

	case rect:
		pObj->m_nShape = CDrawRect::rectangle;
		break;

	case roundRect:
		pObj->m_nShape = CDrawRect::roundRectangle;
		break;

	case ellipse:
		pObj->m_nShape = CDrawRect::ellipse;
		break;

	case line:
		pObj->m_nShape = CDrawRect::line;
		break;
	case arrow:
		pObj->m_nShape = CDrawRect::arrow;
		break;
	}

	if (pView->lastAction)
		if(!pView->lastAction->IsFinished())
			pView->lastAction->Finish();

	pView->lastAction = new CAddAction(pView,ID_ACTION_ADD_COMMON,pObj);
	pView->lastAction->Do();
	
	//pView->GetDocument()->Add(pObj);

	selectMode = size;
	nDragHandle = 1;
	lastPoint = local;
}

void CRectTool::OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CRectTool::OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (point == c_down)
	{
		// Don't create empty objects...
		CDrawObj *pObj = pView->m_selection.GetTail();
		pView->GetDocument()->Remove(pObj);
		CRect rect=pObj->m_position;
		pView->DocToClient(rect);
		pObj->Remove();
		//remove the last CAddAction;

		pView->RemoveLastAction();
		pView->lastAction=NULL;

        //update the areas the missing object occupy
        rect.InflateRect(6,6);
		pView->InvalidateRect(&rect,TRUE);
		//selectTool.OnLButtonDown(pView, nFlags, point); // try a select!
	}
	if (pView->lastAction)
		pView->lastAction->Finish(); //finsih the drawing

	selectTool.OnLButtonUp(pView, nFlags, point);
}

void CRectTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	selectTool.OnMouseMove(pView, nFlags, point);
}


////////////////////////////////////////////////////////////////////////////
// CPolyTool

CPolyTool::CPolyTool()
	: CDrawTool(poly)
{
	m_pDrawObj = NULL;
}

void CPolyTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	if (m_pDrawObj == NULL)
	{
		pView->SetCapture();

		m_pDrawObj = new CDrawPoly(CRect(local, CSize(0, 0)));
		m_pDrawObj->m_type = 0;
		pView->lastAction = new CAddAction(pView,ID_ACTION_ADD_COMMON,m_pDrawObj);
		pView->lastAction->Do();
		//pView->GetDocument()->Add(m_pDrawObj);
		//pView->Select(m_pDrawObj);
		m_pDrawObj->AddPoint(local, pView);
	}
	else if (local == m_pDrawObj->m_points[0])
	{
		// Stop when the first point is repeated...
		ReleaseCapture();
		m_pDrawObj->m_nPoint -= 1;
		if (m_pDrawObj->m_nPoint < 2)
		{
			ASSERT(0);
			m_pDrawObj->Remove();
		}
		else
		{
			pView->InvalObj(m_pDrawObj);
		}
		m_pDrawObj = NULL;
		c_drawShape = selection;
		return;
	}

	local.x += 1; // adjacent points can't be the same!
	m_pDrawObj->AddPoint(local, pView);

	selectMode = size;
	nDragHandle = m_pDrawObj->GetHandleCount();
	lastPoint = local;
}

void CPolyTool::OnLButtonUp(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
	// Don't release capture yet!
}

void CPolyTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (m_pDrawObj != NULL && (nFlags & MK_LBUTTON) != 0)
	{
		CPoint local = point;
		pView->ClientToDoc(local);
		m_pDrawObj->AddPoint(local);
		nDragHandle = m_pDrawObj->GetHandleCount();
		lastPoint = local;
		c_last = point;
		SetCursor(AfxGetApp()->LoadCursor(IDC_PENCIL));
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		selectTool.OnMouseMove(pView, nFlags, point);
	}
}

void CPolyTool::OnLButtonDblClk(CNetworkView* pView, UINT , const CPoint& )
{
	ReleaseCapture();

	int nPoints = m_pDrawObj->m_nPoint;
	if (nPoints > 2 &&
		(m_pDrawObj->m_points[nPoints - 1] == m_pDrawObj->m_points[nPoints - 2] ||
		m_pDrawObj->m_points[nPoints - 1].x - 1 == m_pDrawObj->m_points[nPoints - 2].x &&
		m_pDrawObj->m_points[nPoints - 1].y == m_pDrawObj->m_points[nPoints - 2].y))

	{
		// Nuke the last point if it's the same as the next to last...
		m_pDrawObj->m_nPoint -= 1;
		pView->InvalObj(m_pDrawObj);
	}

	m_pDrawObj = NULL;
	c_drawShape = selection;
	pView->lastAction->Finish();

}

void CPolyTool::OnCancel()
{
	CDrawTool::OnCancel();

	m_pDrawObj = NULL;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CTextTool 

CTextTool::CTextTool(): CRectTool(text)
{
}

void CTextTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{

//when mouse is down, an Object is newed, 
	
	CDrawTool::OnLButtonDown(pView, nFlags, point);
//get the point that cursor is down
	CPoint local = point;
	pView->ClientToDoc(local);
	CDrawText* pObj = new CDrawText(CRect(local, CSize(500, 500)));

// pop the Texttool dialog for the imput of text

	CDrawProperty sheet ( _T("Properties") );
	CTextFormatPage tPage;

	tPage.SetValues(pObj);
	
	CTextProperty textPage;
	sheet.AddPage( &textPage );
	sheet.AddPage( &tPage );
	
	if (sheet.DoModal() != IDOK){
	    delete pObj; //delete the CDrawText object
		return;
	}
//get format of line, background,
	tPage.GetValues(pObj);
//get the text
	pObj->m_text=textPage.m_text;
	pObj->FitContent(pView);

	pView->lastAction = new CAddAction(pView,ID_ACTION_ADD_COMMON,pObj);
	//pView->GetDocument()->Add(pObj);//add the CDrawText to domcument
	//pView->Select(pObj); //select it
	
	pView->lastAction->Do();
	pView->lastAction->Finish(); //unlike other CDrawObj, adding a text is onetime action

	pObj->Invalidate();  //redraw it
//	pObj->m_pDocument->SetModifiedFlag();
//change to seletTool
	c_drawShape = selection;

}

void CTextTool::OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CTextTool::OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (point == c_down)
	{
		//create empty text box for input
		selectTool.OnLButtonDown(pView, nFlags, point); // try a select!
	}

	selectTool.OnLButtonUp(pView, nFlags, point);
}

void CTextTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
//    selectTool.OnMouseMove(pView, nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CZoomTool 

CZoomTool::CZoomTool(): CDrawTool(zoom)
{
}

void CZoomTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	ASSERT(pView);
}

void CZoomTool::OnLButtonDblClk(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
}

void CZoomTool::OnLButtonUp(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	ASSERT(pView);
	pView->DoLButtonUp(nFlags,point);

}

void CZoomTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	ASSERT(pView);
	if (pView->GetZoomMode()==CZoomView::MODE_ZOOMOUT)
		SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOMCURSOR1));
	else if(pView->GetZoomMode()==CZoomView::MODE_ZOOMIN)
		SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOMCURSOR));
///    selectTool.OnMouseMove(pView, nFlags, point);
}


//
////////////////////////////////////////////////////////////////////////////
// CScribbleTool

CScribbleTool::CScribbleTool()
	: CDrawTool(scribble)
{
	m_pDrawObj = NULL;
}

void CScribbleTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	if (m_pDrawObj == NULL)
	{
		pView->SetCapture();

		m_pDrawObj = new CDrawPoly(CRect(local, CSize(0, 0)));
		m_pDrawObj->m_type = 1;

		pView->lastAction = new CAddAction(pView,ID_ACTION_ADD_COMMON,m_pDrawObj);
		pView->lastAction->Do();
		//pView->GetDocument()->Add(m_pDrawObj);
		//pView->Select(m_pDrawObj);
		m_pDrawObj->AddPoint(local, pView);
	}
	else if (local == m_pDrawObj->m_points[0])
	{
		// Stop when the first point is repeated...
		ReleaseCapture();
		m_pDrawObj->m_nPoint -= 1;
		if (m_pDrawObj->m_nPoint < 2)
		{
			ASSERT(0);
			m_pDrawObj->Remove();
		}
		else
		{
			pView->InvalObj(m_pDrawObj);
		}
		m_pDrawObj = NULL;
		c_drawShape = selection;
		return;
	}

	local.x += 1; // adjacent points can't be the same!
	m_pDrawObj->AddPoint(local, pView);

	selectMode = size;
	nDragHandle = m_pDrawObj->GetHandleCount();
	lastPoint = local;
}

void CScribbleTool::OnLButtonUp(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
	// Don't release capture yet!
}

void CScribbleTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (m_pDrawObj != NULL && (nFlags & MK_LBUTTON) != 0)
	{
		CPoint local = point;
		pView->ClientToDoc(local);
		m_pDrawObj->AddPoint(local);
		nDragHandle = m_pDrawObj->GetHandleCount();
		lastPoint = local;
		c_last = point;
		SetCursor(AfxGetApp()->LoadCursor(IDC_PENCIL));
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		selectTool.OnMouseMove(pView, nFlags, point);
	}
}

void CScribbleTool::OnLButtonDblClk(CNetworkView* pView, UINT , const CPoint& )
{
	ReleaseCapture();

	int nPoints = m_pDrawObj->m_nPoint;
	if (nPoints > 2 &&
		(m_pDrawObj->m_points[nPoints - 1] == m_pDrawObj->m_points[nPoints - 2] ||
		m_pDrawObj->m_points[nPoints - 1].x - 1 == m_pDrawObj->m_points[nPoints - 2].x &&
		m_pDrawObj->m_points[nPoints - 1].y == m_pDrawObj->m_points[nPoints - 2].y))

	{
		// Nuke the last point if it's the same as the next to last...
		m_pDrawObj->m_nPoint -= 1;
		pView->InvalObj(m_pDrawObj);
	}

	m_pDrawObj = NULL;
	c_drawShape = selection;
	pView->lastAction->Finish();

}

void CScribbleTool::OnCancel()
{
	CDrawTool::OnCancel();

	m_pDrawObj = NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// CCurveTool

CCurveTool::CCurveTool()
	: CDrawTool(curve)
{
	m_pDrawObj = NULL;
}

void CCurveTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	if (m_pDrawObj == NULL)
	{
		pView->SetCapture();

		m_pDrawObj = new CDrawPoly(CRect(local, CSize(0, 0)));
		m_pDrawObj->m_type = 2;

		pView->lastAction = new CAddAction(pView,ID_ACTION_ADD_COMMON,m_pDrawObj);
		pView->lastAction->Do();
		//pView->GetDocument()->Add(m_pDrawObj);
		//pView->Select(m_pDrawObj);
		m_pDrawObj->AddPoint(local, pView);
	}
	else if (local == m_pDrawObj->m_points[0])
	{
		// Stop when the first point is repeated...
		ReleaseCapture();
		m_pDrawObj->m_nPoint -= 1;
		if (m_pDrawObj->m_nPoint < 2)
		{
			ASSERT(0);
			m_pDrawObj->Remove();
		}
		else
		{
			pView->InvalObj(m_pDrawObj);
		}
		m_pDrawObj = NULL;
		c_drawShape = selection;
		return;
	}

	local.x += 1; // adjacent points can't be the same!
	m_pDrawObj->AddPoint(local, pView);

	selectMode = size;
	nDragHandle = m_pDrawObj->GetHandleCount();
	lastPoint = local;
}

void CCurveTool::OnLButtonUp(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
	// Don't release capture yet!
}

void CCurveTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (m_pDrawObj != NULL && (nFlags & MK_LBUTTON) != 0)
	{
		CPoint local = point;
		pView->ClientToDoc(local);
		m_pDrawObj->AddPoint(local);
		nDragHandle = m_pDrawObj->GetHandleCount();
		lastPoint = local;
		c_last = point;
		SetCursor(AfxGetApp()->LoadCursor(IDC_PENCIL));
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		selectTool.OnMouseMove(pView, nFlags, point);
	}
}

void CCurveTool::OnLButtonDblClk(CNetworkView* pView, UINT , const CPoint& )
{
	ReleaseCapture();

	int nPoints = m_pDrawObj->m_nPoint;
	if (nPoints > 2 &&
		(m_pDrawObj->m_points[nPoints - 1] == m_pDrawObj->m_points[nPoints - 2] ||
		m_pDrawObj->m_points[nPoints - 1].x - 1 == m_pDrawObj->m_points[nPoints - 2].x &&
		m_pDrawObj->m_points[nPoints - 1].y == m_pDrawObj->m_points[nPoints - 2].y))

	{
		// Nuke the last point if it's the same as the next to last...
		m_pDrawObj->m_nPoint -= 1;
		pView->InvalObj(m_pDrawObj);
	}

	m_pDrawObj = NULL;
	c_drawShape = selection;
	pView->lastAction->Finish();

}

void CCurveTool::OnCancel()
{
	CDrawTool::OnCancel();

	m_pDrawObj = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// CClosedCurveTool

CClosedCurveTool::CClosedCurveTool()
	: CDrawTool(closedcurve)
{
	m_pDrawObj = NULL;
}

void CClosedCurveTool::OnLButtonDown(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	if (m_pDrawObj == NULL)
	{
		pView->SetCapture();

		m_pDrawObj = new CDrawPoly(CRect(local, CSize(0, 0)));
		m_pDrawObj->m_type = 3;

		pView->lastAction = new CAddAction(pView,ID_ACTION_ADD_COMMON,m_pDrawObj);
		pView->lastAction->Do();
		//pView->GetDocument()->Add(m_pDrawObj);
		//pView->Select(m_pDrawObj);
		m_pDrawObj->AddPoint(local, pView);
	}
	else if (local == m_pDrawObj->m_points[0])
	{
		// Stop when the first point is repeated...
		ReleaseCapture();
		m_pDrawObj->m_nPoint -= 1;
		if (m_pDrawObj->m_nPoint < 2)
		{
			ASSERT(0);
			m_pDrawObj->Remove();
		}
		else
		{
			pView->InvalObj(m_pDrawObj);
		}
		m_pDrawObj = NULL;
		c_drawShape = selection;
		return;
	}

	local.x += 1; // adjacent points can't be the same!
	m_pDrawObj->AddPoint(local, pView);

	selectMode = size;
	nDragHandle = m_pDrawObj->GetHandleCount();
	lastPoint = local;
}

void CClosedCurveTool::OnLButtonUp(CNetworkView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
	// Don't release capture yet!
}

void CClosedCurveTool::OnMouseMove(CNetworkView* pView, UINT nFlags, const CPoint& point)
{
	if (m_pDrawObj != NULL && (nFlags & MK_LBUTTON) != 0)
	{
		CPoint local = point;
		pView->ClientToDoc(local);
		m_pDrawObj->AddPoint(local);
		nDragHandle = m_pDrawObj->GetHandleCount();
		lastPoint = local;
		c_last = point;
		SetCursor(AfxGetApp()->LoadCursor(IDC_PENCIL));
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		selectTool.OnMouseMove(pView, nFlags, point);
	}
}

void CClosedCurveTool::OnLButtonDblClk(CNetworkView* pView, UINT , const CPoint& )
{
	ReleaseCapture();

	int nPoints = m_pDrawObj->m_nPoint;
	if (nPoints > 2 &&
		(m_pDrawObj->m_points[nPoints - 1] == m_pDrawObj->m_points[nPoints - 2] ||
		m_pDrawObj->m_points[nPoints - 1].x - 1 == m_pDrawObj->m_points[nPoints - 2].x &&
		m_pDrawObj->m_points[nPoints - 1].y == m_pDrawObj->m_points[nPoints - 2].y))

	{
		// Nuke the last point if it's the same as the next to last...
		m_pDrawObj->m_nPoint -= 1;
		pView->InvalObj(m_pDrawObj);
	}

	m_pDrawObj = NULL;
	c_drawShape = selection;
	pView->lastAction->Finish();

}

void CClosedCurveTool::OnCancel()
{
	CDrawTool::OnCancel();

	m_pDrawObj = NULL;
}

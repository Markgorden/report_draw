// MAction.cpp: implementation of the CBEAction class.
//
// Copyright (c) 2001 Tao Zhu, all rights reserved.
//
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MAction.h"
#include "NetworkView.h"
#include "DrawDoc.h"
#include "resource.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMAction::CMAction(CNetworkView *aView, UINT type)
	: CAction(aView->GetActionTarget(), type), operationTarget(aView)
{
	m_page		  = ((CDrawDoc*) actionTarget)->m_nCurrentPage;
}

CMAction::~CMAction()
{
}

void CMAction::Do()
{
	if (operationTarget->lastAction) {
		if(operationTarget->lastAction->done ==false)
			operationTarget->lastAction->Do();
		operationTarget->lastAction->Finish();
	}
	CAction::Do();
	//If an action has inform the operation target to add itsleft to stack, the earlier One should be finshed.
}

void CMAction::Undo()
{
	CAction::Undo();
	// regain the input focus
	operationTarget->SetFocus();
	//if current page is not the page that action happens, go back to that page.
	if (operationTarget->GetDocument()->m_nCurrentPage!=m_page)
		operationTarget->GetDocument()->GoToPage(m_page);
}

void CMAction::Redo()
{
	CAction::Redo();
	// regain the input focus
	operationTarget->SetFocus();
	//if current page is not the page that action happens, go back to that page.
	if (operationTarget->GetDocument()->m_nCurrentPage!=m_page)
		operationTarget->GetDocument()->GoToPage(m_page);
}


//////////////////////////////////////////////////////////////////////
CAddAction::CAddAction(CNetworkView *aView, UINT actionType, CDrawObj *data)
	: CMAction(aView, actionType)
{
	m_data = data;
	m_bAdded = false;
}

CAddAction::~CAddAction()
{
	if (!m_bAdded)
		delete m_data;  //this drawobj associated with addAction is deleted when the action is deleted.
					  //only if the object is not added in the document;
}

void CAddAction::Do()
{
	CAction::Do(); //Notify Manager
	// Call helper (subclasses can extend)
	PerformOperation();
	m_bAdded = true;

	//Finish();  //Adding is not a one-time Action
}

// Actually perform the operation, or tell the view to do it. Subclasses
// should override to provide custom edit-like operations such as insert
// date/time.
 
void CAddAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->Add(m_data);
	operationTarget->Select(m_data); //awalys selected new objet
}


//Selection of object is not restored in the Undo. Because i don't think it is a state of CDocument,
//though it is a state of CView. The real reason behind this is, it is not important
//for users to retrieve what have been seleted after doing something.
void CAddAction::Undo()
{
	CMAction::Undo();
	//update the view before delete, it seems strange, but in this way, the draggers can be cleared.
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_data);
	//delete the new added obj;
	((CDrawDoc*)actionTarget)->Remove(m_data);
	m_bAdded = false;

}

void CAddAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
	//update the view
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_data);
	// this is a one-shot action
	m_bAdded = true;
}


//////////////////////////////////////////////////////////////////////
CSizeAction::CSizeAction(CNetworkView *aView, UINT actionType, int n, CPoint point, CDrawObj *pObj)
	: CMAction(aView, actionType)
{
	m_rect		=	pObj->m_position;
	m_pObj		=	pObj;
	nHandle		=	n;
	oldPoint	=	point;
}

CSizeAction::~CSizeAction()
{
}


// Actually perform the operation, or tell the view to do it. Subclasses
// should override to provide custom edit-like operations such as insert
// date/time.


//Selection of object is not restored in the Undo. Because i don't think it is a state of CDocument,
//though it is a state of CView. The real reason behind this is, it is not important
//for users to retrieve what have been seleted after doing something.
void CSizeAction::Undo()
{
	CMAction::Undo();
	//update the view before delete, it seems strange, but in this way, the draggers can be cleared.
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_pObj);

	CPoint temp=m_pObj->GetHandle(nHandle);
	m_pObj->MoveHandleTo(nHandle, oldPoint, operationTarget);
	oldPoint = temp;

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_pObj);
}

void CSizeAction::Redo()
{
	CMAction::Redo();
	//update the view before delete, it seems strange, but in this way, the draggers can be cleared.
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_pObj);
	CPoint temp=m_pObj->GetHandle(nHandle);
	m_pObj->MoveHandleTo(nHandle, oldPoint, operationTarget);
	oldPoint = temp;
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_pObj);
}

void CSizeAction::Do()
{
	CAction::Do(); //Notify Manager only
	Finish();
}
//////////////////////////////////////////////////////////////////////
CMoveAction::CMoveAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_rects = new CRect[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		m_rects[i++]=pObj->m_position;
	}
    //some reactions' state are also changed due to the moving of the reatant it connects to.
	//these don't include the reactions already in the m_list;
}

CMoveAction::~CMoveAction()
{
	delete[] m_rects;
}

void CMoveAction::PerformOperation(bool undo)
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	//delete the new added obj;

	int i=0;
	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL)
	{ 
		CDrawObj *pObj = m_list.GetNext(pos);
		CRect temp = pObj->m_position;
		pObj->MoveTo(m_rects[i], operationTarget);
		m_rects[i] =temp;
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);

}

void CMoveAction::Undo()
{
	CMAction::Undo();
	PerformOperation(1);
}

void CMoveAction::Redo()
{
	CMAction::Redo();
	PerformOperation(0);
}

void CMoveAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}

/////////////////////////////////////////////////////////
//CEditAction need allocate memory  for two CDrawObj object (size depend the CDrawObj class)
CEditAction::CEditAction(CNetworkView *aView, UINT actionType, CDrawObj *data)
	: CMAction(aView, actionType)
{
	m_data   = data;
	m_oldData= m_data->Clone(NULL); //only part of data is copied to m_oldData;
	m_newData= 0;
}

CEditAction::~CEditAction()
{
	delete m_oldData;
	if (m_newData) //Action may be deleted before it is done, if user hit cancel in the dialog.
		delete m_newData;
}

void CEditAction::Do()
{
	CAction::Do(); //Notify Manager
	m_newData= m_data->Clone(NULL); //only part of data is copied to m_oldData;
	Finish();
}

void CEditAction::Undo()
{
	CMAction::Undo();
	m_data->RetrieveData(m_oldData);
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_data);
}

void CEditAction::Redo()
{
	CMAction::Redo();
	m_data->RetrieveData(m_newData);
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, m_data);
}


//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CMoveOneStepAction::CMoveOneStepAction(CNetworkView *aView, UINT actionType, bool forward, CDrawObj* data)
	: CMAction(aView, actionType)
{
	m_data		= data;
	m_bForward	= forward;

}

CMoveOneStepAction::~CMoveOneStepAction()
{
}

void CMoveOneStepAction::Undo()
{
	CMAction::Undo();
	if (m_bForward)
		MoveBackward();
	else
		MoveForward();
}

void CMoveOneStepAction::Redo()
{
	CMAction::Redo();
	if (m_bForward)
		MoveForward();
	else
		MoveBackward();
}

void CMoveOneStepAction::Do()//Notify Manager only
{
	CAction::Do(); 
	if (m_bForward)
		MoveForward();
	else
		MoveBackward();

	CAction::Finish();
}


void CMoveOneStepAction::MoveForward()
{
	CDrawDoc *pDoc = ((CDrawDoc*)actionTarget);
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(m_data);
	ASSERT(pos != NULL);
	if (pos != pObjects->GetTailPosition())
	{
		POSITION posNext = pos;
		pObjects->GetNext(posNext);
		pObjects->RemoveAt(pos);
		pObjects->InsertAfter(posNext, m_data);
		operationTarget->InvalObj(m_data);
	}

}

void CMoveOneStepAction::MoveBackward()
{
	CDrawDoc *pDoc = ((CDrawDoc*)actionTarget);
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(m_data);
	ASSERT(pos != NULL);
	if (pos != pObjects->GetHeadPosition())
	{
		POSITION posPrev = pos;
		pObjects->GetPrev(posPrev);
		pObjects->RemoveAt(pos);
		pObjects->InsertBefore(posPrev, m_data);
		operationTarget->InvalObj(m_data);
	}
}

//////////////////////////////////////////////////////////////////////
CMoveStepsAction::CMoveStepsAction(CNetworkView *aView, UINT actionType, bool forward, CDrawObj *pAfter, CDrawObj* data)
	: CMAction(aView, actionType)
{
	m_data		= data;
	m_bForward	= forward;
	m_pAfter	= pAfter;

}

CMoveStepsAction::~CMoveStepsAction()
{
}

void CMoveStepsAction::Undo()
{
	CMAction::Undo();
	//Move back to old position;
	CDrawDoc *pDoc = ((CDrawDoc*)actionTarget);
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(m_data);
	pObjects->RemoveAt(pos);
	if (m_pAfter){
		POSITION pos1 = pObjects->Find(m_pAfter);
		pObjects->InsertBefore(pos1, m_data);
	}
	else
		pObjects->AddTail(m_data);
	operationTarget->InvalObj(m_data);

}

void CMoveStepsAction::Redo()
{
	CMAction::Redo();
	if (m_bForward)
		MoveToFront();
	else
		MoveToBack();
}

void CMoveStepsAction::Do()//Notify Manager only
{
	CAction::Do(); 
	if (m_bForward)
		MoveToFront();
	else
		MoveToBack();
	CAction::Finish();
}


void CMoveStepsAction::MoveToFront()
{
	CDrawDoc *pDoc = ((CDrawDoc*)actionTarget);
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(m_data);
	ASSERT(pos != NULL);
	pObjects->RemoveAt(pos);
	pObjects->AddTail(m_data);
	operationTarget->InvalObj(m_data);
}

void CMoveStepsAction::MoveToBack()
{
	CDrawDoc *pDoc = ((CDrawDoc*)actionTarget);
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(m_data);
	ASSERT(pos != NULL);
	pObjects->RemoveAt(pos);
	pObjects->AddHead(m_data);
	operationTarget->InvalObj(m_data);
}

//////////////////////////////////////////////////////////////////////
CArrowAction::CArrowAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_data = new ArrowTypeG[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		m_data[i++]=pObj->GetArrowType(actionType); //ID_ACTION_LEFT_ARROW vs. ID_ACTION_RIGHT_ARROW
	}
}

CArrowAction::~CArrowAction()
{
	delete[] m_data;
}

void CArrowAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	//delete the new added obj;

	POSITION pos=m_list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{ 
		CDrawObj *pObj = m_list.GetNext(pos);
		ArrowTypeG temp = pObj->GetArrowType(actionType);
		pObj->SetArrowType(actionType, m_data[i]);
		m_data[i] =temp;
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
}

void CArrowAction::Undo()
{
	CMAction::Undo();
	PerformOperation();
}

void CArrowAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}

void CArrowAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}

//////////////////////////////////////////////////////////////////////
CColorAction::CColorAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_data = new COLORREF[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		switch (actionType){
		case ID_ACTION_COLOR_LINE:
			m_data[i++]=pObj->GetLineColor(); 
			break;
		case ID_ACTION_COLOR_FILL:
			m_data[i++]=pObj->GetFillColor(); 
			break;
		case ID_ACTION_COLOR_TEXT:
			m_data[i++]=pObj->GetTextColor(); 
			break;
		default:
			ASSERT(0);
		}
	}
}

CColorAction::~CColorAction()
{
	delete[] m_data;
}

void CColorAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	//delete the new added obj;

	POSITION pos=m_list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{ 
		CDrawObj *pObj = m_list.GetNext(pos);
		COLORREF temp;
		switch (actionType){
		case ID_ACTION_COLOR_LINE:
			temp = pObj->GetLineColor();
			pObj->SetLineColor(m_data[i]);
			m_data[i] =temp;
			break;
		case ID_ACTION_COLOR_FILL: 
			temp = pObj->GetFillColor();
			pObj->SetFillColor(m_data[i]);
			m_data[i] =temp;
			break;
		case ID_ACTION_COLOR_TEXT:
			temp = pObj->GetTextColor();
			pObj->SetTextColor(m_data[i]);
			m_data[i] =temp;
			break;
		default:
			ASSERT(0);
		}
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
}

void CColorAction::Undo()
{
	CMAction::Undo();
	PerformOperation();
}

void CColorAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}

void CColorAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}

//////////////////////////////////////////////////////////////////////
CLineStyleAction::CLineStyleAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_data = new int[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		m_data[i++]=pObj->GetLineStyle(); //ID_ACTION_LEFT_ARROW vs. ID_ACTION_RIGHT_ARROW
	}
}

CLineStyleAction::~CLineStyleAction()
{
	delete[] m_data;
}

void CLineStyleAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	//delete the new added obj;

	POSITION pos=m_list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{ 
		CDrawObj *pObj = m_list.GetNext(pos);
		int temp = pObj->GetLineStyle();
		pObj->SetLineStyle(m_data[i]);
		m_data[i] =temp;
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
}

void CLineStyleAction::Undo()
{
	CMAction::Undo();
	PerformOperation();
}

void CLineStyleAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}

void CLineStyleAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}

//////////////////////////////////////////////////////////////////////
CLineSizeAction::CLineSizeAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_data = new int[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		m_data[i++]=pObj->GetLineWidth(); //ID_ACTION_LEFT_ARROW vs. ID_ACTION_RIGHT_ARROW
	}
}

CLineSizeAction::~CLineSizeAction()
{
	delete[] m_data;
}

void CLineSizeAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	//delete the new added obj;

	POSITION pos=m_list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{ 
		CDrawObj *pObj = m_list.GetNext(pos);
		int temp = pObj->GetLineWidth();
		pObj->SetLineWidth(m_data[i]);
		m_data[i] =temp;
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
}

void CLineSizeAction::Undo()
{
	CMAction::Undo();
	PerformOperation();
}

void CLineSizeAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}

void CLineSizeAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}

//////////////////////////////////////////////////////////////////////
CFontAction::CFontAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_data = new CCharFormat[m_N];
	m_position = new CRect[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		m_position[i]=pObj->m_position;
		m_data[i++]=pObj->GetCharFormat(); //ID_ACTION_LEFT_ARROW vs. ID_ACTION_RIGHT_ARROW
	}
}
//precodition, m_N>=1
CFontAction::~CFontAction()
{
	delete[] m_data;
	delete[] m_position;
}

void CFontAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	//delete the new added obj;

	POSITION pos=m_list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{ 
		CDrawObj *pObj = m_list.GetNext(pos);
		CCharFormat temp = pObj->GetCharFormat();
		CRect	temp1=pObj->m_position;
		pObj->SetCharFormat(m_data[i], operationTarget);
		pObj->MoveTo(m_position[i],operationTarget);
		//if pObj is Rectant, it actually change the size of the reaction;
		m_data[i] =temp;
		m_position[i]=temp1;
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
}

void CFontAction::Undo()
{
	CMAction::Undo();
	PerformOperation();
}

void CFontAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}

void CFontAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}

//////////////////////////////////////////////////////////////////////
//CDeleteAction need allocate memory  for two CDrawObj object (size depend the CDrawObj class)
CDeleteAction::CDeleteAction(CNetworkView *aView, UINT actionType,  const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_Pos = NULL;
	m_bDeleted=false;
	m_N = list.GetCount();
	CDrawObjList* Objects =((CDrawDoc*)actionTarget)->GetObjects() ;
	CDrawObjList temp;
	//first loop adding the deleted CDrawObj into our m_list;
	POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		temp.AddTail(pObj);  //collect the drawobj pointer in list to a temp list
	}

	//second loop reorder the reactions list according to Objects list.
	m_N		= temp.GetCount();
	int* index	= new int[m_N];
	m_Pos		= new int[m_N];
	int j=0;
	pos = temp.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj *pObj = temp.GetNext(pos);
		m_Pos[j] = GetObjectPosition(*Objects,pObj);
		index[j]= j;
		j++;
	}

	//reorder posInDoc[j] using bubble;
	for(j=0;j<m_N;j++){
		for(int k=j+1;k<m_N;k++){
			if (m_Pos[j]>m_Pos[k]){
				//bubble up
				int x;
				x		=m_Pos[k];
				m_Pos[k]=m_Pos[j];
				m_Pos[j]=	x;
				x		=index[k];
				index[k]=index[j];
				index[j]=	x;
			}
		}
	}
	//now add the temp list to m_list according the order in the index[j];
	for (j=0;j<m_N;j++){
		pos = temp.FindIndex(index[j]);
		m_list.AddTail(temp.GetAt(pos));
	}

	delete [] index;
}

CDeleteAction::~CDeleteAction()
{
	if (m_bDeleted){ //Delete all objects in the m_list
		//delete objects
		POSITION pos = m_list.GetHeadPosition();
		while(pos){
			CDrawObj *pObj = m_list.GetNext(pos);
			delete pObj; //Notice the destructor of CDrawNode will delete un-referenced RNode.
		}
	}
	if (m_Pos)
		delete []m_Pos;
}

void CDeleteAction::PerformOperation()
{
	int n=0;
	//delete all the regulation fist;
	POSITION pos = m_list.GetTailPosition();
	while (pos != NULL)
	{
		CDrawObj *pObj = m_list.GetPrev(pos);
		((CDrawDoc*)actionTarget)->Remove(pObj); //it will also remvoe CRNode
	}
	operationTarget->OnUpdate(NULL, HINT_DELETE_SELECTION, &m_list);
	m_bDeleted=true;
}

void CDeleteAction::Do()
{
	CAction::Do(); //Notify Manager
	PerformOperation();
	Finish();
}

void CDeleteAction::UndoOperation(){
	//add back all the objects according its position in the CDrawDoc::m_objects.
	CDrawObjList* Objects =((CDrawDoc*)actionTarget)->GetObjects();
	POSITION pos = m_list.GetHeadPosition();
	int lengthA  = Objects->GetCount(); //original length = lengthA + m_N;
	int i=0;
	while (pos){
		CDrawObj* pObj = m_list.GetNext(pos);
		//now insert the pObj to the m_Pos[i] position of Objects
		POSITION pos1=Objects->FindIndex(m_Pos[i++]-1);
		if (pos1==NULL){
			Objects->AddTail(pObj); //directly access DrawObjlist in Document is not good, but ...
			pObj->OnAdded();
		}
		else{
			Objects->InsertBefore(pos1,pObj);
			pObj->OnAdded();
		}
	}

	//reset the oldReactionList[]
	m_bDeleted=false;
	operationTarget->OnUpdate(NULL, HINT_UPDATE_SELECTION, &m_list);
}

void CDeleteAction::Undo()
{
	CMAction::Undo();
	UndoOperation();
}

void CDeleteAction::RedoOperation()
{
	int n=0;
	//delete other objects;
	POSITION pos = m_list.GetTailPosition();
	while (pos != NULL)
	{
		CDrawObj *pObj = m_list.GetPrev(pos);
		((CDrawDoc*)actionTarget)->Remove(pObj);
	}
	operationTarget->OnUpdate(NULL, HINT_DELETE_SELECTION, &m_list);
	m_bDeleted=true;
}

void CDeleteAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}



//////////////////////////////////////////////////////////////////////
CEditMultiAction::CEditMultiAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list)
	: CMAction(aView, actionType)
{
	m_N = list.GetCount();
	m_data = new CDrawData[m_N];
	POSITION pos = list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawObj *pObj = list.GetNext(pos);
		m_list.AddTail(pObj);  //collect the drawobj pointer in list to m_list
		GetData(pObj,m_data[i++]); 
	}
}

CEditMultiAction::~CEditMultiAction()
{
	if (m_data)
		delete [] m_data;
}

void CEditMultiAction::PerformOperation()
{
	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
	POSITION pos=m_list.GetHeadPosition();
	int i=0;
	while (pos != NULL)
	{
		CDrawData temp; 
		CDrawObj *pObj = m_list.GetNext(pos);
		GetData(pObj,temp); 
		SetData(pObj,m_data[i]);
		m_data[i]=temp;
		i++;
	}

	((CDrawDoc*)actionTarget)->UpdateAllViews(NULL,  HINT_UPDATE_SELECTION, &m_list);
}

void CEditMultiAction::Undo()
{
	CMAction::Undo();
	PerformOperation();
}

void CEditMultiAction::Redo()
{
	CMAction::Redo();
	PerformOperation();
}

void CEditMultiAction::Do()//Notify Manager only
{
	CAction::Do(); 
	CAction::Finish();
}


void CEditMultiAction::SetData(CDrawObj *pObj, CDrawData &data)
{

	pObj->m_dashStyle			= data.m_dashStyle;
	pObj->m_dashCap				= data.m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	pObj->m_brushType			= data.m_brushType,
	pObj->m_penbrushType			= data.m_penbrushType;  // BrushTypeLinearGradient or BrushTypeSolidColor Only
	pObj->m_penType				= data.m_penType;
	pObj->m_Transparency			= data.m_Transparency; //for alpha blending;
	pObj->m_brushColor 			= data.m_brushColor, //when hatch pattern used, this is foreColor
	pObj->m_gradientColor        = data.m_gradientColor,
	pObj->m_penColor				= data.m_penColor,
	pObj->m_penEndColor			= data.m_penEndColor;
	pObj->m_penWidth				= data.m_penWidth;
	pObj->m_linearGradientMode   = data.m_linearGradientMode, //for linear gradient only;
	pObj->m_penlinearGradientMode= data.m_penlinearGradientMode;
	pObj->m_hatchStyle           = data.m_hatchStyle;
	pObj->m_startArrowSize       = data.m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
	pObj->m_endArrowSize         = data.m_endArrowSize;
	pObj->m_startArrow			= data.m_startArrow,
	pObj->m_endArrow				= data.m_endArrow;
	pObj->m_fillImage			= data.m_fillImage;
	pObj->m_wrapMode             = data.m_wrapMode;
	pObj->m_bToCenter			= data.m_bToCenter; //0, - center path gradient.
	pObj->m_nStretch				= data.m_nStretch;
	pObj->Invalidate();
}

void CEditMultiAction::GetData(CDrawObj *pObj, CDrawData &data)
{
	data.m_dashStyle			= pObj->m_dashStyle;
	data.m_dashCap				= pObj->m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	data.m_brushType			= pObj->m_brushType,
	data.m_penbrushType			= pObj->m_penbrushType;  // BrushTypeLinearGradient or BrushTypeSolidColor Only
	data.m_penType				= pObj->m_penType;
	data.m_Transparency			= pObj->m_Transparency; //for alpha blending;
	data.m_brushColor 			= pObj->m_brushColor, //when hatch pattern used, this is foreColor
	data.m_gradientColor        = pObj->m_gradientColor,
	data.m_penColor				= pObj->m_penColor,
	data.m_penEndColor			= pObj->m_penEndColor;
	data.m_penWidth				= pObj->m_penWidth;
	data.m_linearGradientMode   = pObj->m_linearGradientMode, //for linear gradient only;
	data.m_penlinearGradientMode= pObj->m_penlinearGradientMode;
	data.m_hatchStyle           = pObj->m_hatchStyle;
	data.m_startArrowSize       = pObj->m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
	data.m_endArrowSize         = pObj->m_endArrowSize;
	data.m_startArrow			= pObj->m_startArrow,
	data.m_endArrow				= pObj->m_endArrow;
	data.m_fillImage			= pObj->m_fillImage;
	data.m_wrapMode             = pObj->m_wrapMode;
	data.m_bToCenter			= pObj->m_bToCenter; //0, - center path gradient.
	data.m_nStretch				= pObj->m_nStretch;
}

//////////////////////////////////////////////////////////////////////
//CPasteAction need allocate memory  for two CDrawObj object (size depend the CDrawObj class)
CPasteAction::CPasteAction(CNetworkView *aView, UINT actionType)
	: CMAction(aView, actionType)
{
	m_dataObject.AttachClipboard(); //when will the data be released?
	m_bPasted = false;
}

CPasteAction::~CPasteAction()
{ 
	if(!m_bPasted){ //delete all the objects in m_list;
		POSITION pos = m_list.GetHeadPosition();
		while (pos != NULL){
			CDrawObj *pObj = m_list.GetNext(pos);
			pObj->Remove();
		}
	}
	m_list.RemoveAll();
}

void CPasteAction::Do()
{
	CAction::Do(); //Notify Manager
	operationTarget->OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	operationTarget->m_selection.RemoveAll();
	if (m_dataObject.IsDataAvailable(operationTarget->m_cfDraw))
	{
		operationTarget->PasteNative(m_dataObject);
		// I don want the objects are added to the same position, i want to it move a little (10*10).
		POSITION pos = operationTarget->m_selection.GetHeadPosition();
		while (pos) {
			CDrawObj *pObj = operationTarget->m_selection.GetNext(pos);
			pObj->Shift(operationTarget->n_Copies*10+10,-10*operationTarget->n_Copies-10);
		}
		// now add all items in m_selection to document
		pos = operationTarget->m_selection.GetHeadPosition();
		int n=0;
		while (pos != NULL){
			CDrawObj *pObj = operationTarget->m_selection.GetNext(pos);
			m_list.AddTail(pObj);
			operationTarget->GetDocument()->Add(pObj);

		}
		//now initialize all the new added object, build the links between reactant and reaction;
		pos = m_list.GetHeadPosition();
		while (pos != NULL){
			CDrawObj *pObj = m_list.GetNext(pos);
			pObj->Initialize();
		}
	}
	else{
		operationTarget->PasteEmbedded(m_dataObject, operationTarget->GetInitialPosition().TopLeft() );
	}
    // invalidate new pasted stuff
	operationTarget->GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_SELECTION, &operationTarget->m_selection);
	m_bPasted = true;
	operationTarget->n_Copies++;
	Finish();
}

void CPasteAction::Undo()
{
	CMAction::Undo();
	operationTarget->OnUpdate(NULL, HINT_DELETE_SELECTION, &m_list);
	POSITION pos = m_list.GetHeadPosition();
 
	while (pos != NULL){
		CDrawObj *pObj = m_list.GetNext(pos);
		POSITION pos1 = operationTarget->GetDocument()->GetObjects()->Find(pObj);
		ASSERT(pos1);
		if (pos1)
			operationTarget->GetDocument()->GetObjects()->RemoveAt(pos1);
	}
	operationTarget->n_Copies--;
	m_bPasted = false;
}

void CPasteAction::Redo()
{
	CMAction::Redo();
	operationTarget->OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	//adding back the objects, that is produced in the Do() function;
	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL){
		CDrawObj *pObj = m_list.GetNext(pos);
		operationTarget->GetDocument()->GetObjects()->AddTail(pObj);
	}
	operationTarget->GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_SELECTION, &m_list);
	operationTarget->n_Copies++;
	m_bPasted = true;
}


//////////////////////////////////////////////////////////////////////
CAddPageAction::CAddPageAction(CNetworkView *aView, UINT actionType)
	: CMAction(aView, actionType)
{
}

CAddPageAction::~CAddPageAction()
{
}

void CAddPageAction::Do()
{
	CAction::Do(); //Notify Manager only, the action is taken

	CDrawDoc* pDoc= (CDrawDoc* )actionTarget;
	pDoc->m_nTotalPage++;
	m_AddedPage = pDoc->m_nCurrentPage+1; //we insert a new page after the current page.

	//update all drawobj; if they are in the pages after inserted page, pObj->m_nPage++;
	POSITION pos = pDoc->GetObjects()->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
		if (pObj->m_nPage> pDoc->m_nCurrentPage)
			pObj->m_nPage++;
	}
	pDoc->m_nCurrentPage++;
	pDoc->ShowPageNum();
	pDoc->UpdateAllViews(NULL);
}

void CAddPageAction::Undo()
{
	CMAction::Undo();
	CDrawDoc* pDoc= (CDrawDoc* )actionTarget;
	//now pDoc->m_CurrentPage is back to before insert; (CMAction::Undo;
	pDoc->m_nTotalPage--;
	//update all drawobj; if they are in the pages after inserted page, pObj->m_nPage++;
	POSITION pos = pDoc->GetObjects()->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
		if (pObj->m_nPage>m_AddedPage)
			pObj->m_nPage--;
	}
	pDoc->ShowPageNum();
	pDoc->UpdateAllViews(NULL);
}

void CAddPageAction::Redo()
{
	CMAction::Redo();
	CDrawDoc* pDoc= (CDrawDoc* )actionTarget;
	pDoc->m_nTotalPage++;
	m_AddedPage = pDoc->m_nCurrentPage+1; //we insert a new page after the current page.

	//update all drawobj; if they are in the pages after inserted page, pObj->m_nPage++;
	POSITION pos = pDoc->GetObjects()->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
		if (pObj->m_nPage>pDoc->m_nCurrentPage)
			pObj->m_nPage++;
	}
	pDoc->m_nCurrentPage++;
	pDoc->ShowPageNum();
	pDoc->UpdateAllViews(NULL);
}


//////////////////////////////////////////////////////////////////////
CDeletePageAction::CDeletePageAction(CNetworkView *aView, UINT actionType)
	: CMAction(aView, actionType)
{
	deleteAction = NULL;
}

CDeletePageAction::~CDeletePageAction()
{
	if (deleteAction)
		delete deleteAction;
}

void CDeletePageAction::Do()
{
	CAction::Do(); //Notify Manager
	CDrawDoc* pDoc= (CDrawDoc* )actionTarget;
	n_DeletedPage = pDoc->m_nCurrentPage; //always delete current page.

	CDrawObjList list;
	POSITION pos = pDoc->GetObjects()->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
		if (pObj->m_nPage==n_DeletedPage){
			list.AddTail(pObj);
		}
		else if(pObj->m_nPage > n_DeletedPage)
			pObj->m_nPage--;
	}

	if (list.GetCount()){
		deleteAction = new CDeleteAction(operationTarget,ID_ACTION_DELETE,list);
		deleteAction->PerformOperation(); //don't add this action to stack
		deleteAction->Finish();
	}
	//now remove the page
	pDoc->m_nTotalPage--;
	pDoc->m_nCurrentPage--;
	if (pDoc->m_nCurrentPage==0)
		pDoc->GoToPage(1);
	pDoc->ShowPageNum();
	pDoc->UpdateAllViews(NULL);
}

void CDeletePageAction::Undo()
{
	CAction::Undo();
	operationTarget->SetFocus();

	//unlike all other action, DeletePageAction cannot return the position it happens before the 
	//action is undone.
	
	//first add back the page;
	CDrawDoc* pDoc= (CDrawDoc* )actionTarget;
	pDoc->m_nTotalPage++;
	POSITION pos = pDoc->GetObjects()->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
		if(pObj->m_nPage >= n_DeletedPage)
			pObj->m_nPage++;
	}
	if (deleteAction)
		deleteAction->UndoOperation();

	pDoc->m_nCurrentPage=n_DeletedPage;
	pDoc->ShowPageNum();
	pDoc->UpdateAllViews(NULL);

}

void CDeletePageAction::Redo()
{
	CAction::Undo();
	operationTarget->SetFocus();

	//unlike all other action, DeletePageAction cannot return the position it happens before the 
	//action is undone.
	
	//first add back the page;
	CDrawDoc* pDoc= (CDrawDoc* )actionTarget;
	POSITION pos = pDoc->GetObjects()->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
		if(pObj->m_nPage > n_DeletedPage)
			pObj->m_nPage--;
	}

	if (deleteAction)
		deleteAction->RedoOperation();
	//now remove the page
	pDoc->m_nTotalPage--;
	pDoc->m_nCurrentPage--;
	pDoc->ShowPageNum();
	pDoc->UpdateAllViews(NULL);
}


//////////////////////////////////////////////////////////////////////
CMoveSingleAction::CMoveSingleAction(CNetworkView *aView, UINT actionType, CDrawObj *data)
	: CMAction(aView, actionType)
{
	m_data = data;
	m_rect = m_data->m_position;
}

CMoveSingleAction::~CMoveSingleAction()
{
}


void CMoveSingleAction::Undo()
{
	CMAction::Undo();
	CRect temp=m_data->m_position;
	m_data->MoveTo(m_rect);
	m_rect = temp;
}

void CMoveSingleAction::Redo()
{
	CMAction::Redo();
	CRect temp=m_data->m_position;
	m_data->MoveTo(m_rect);
	m_rect = temp;
}

void CMoveSingleAction::Do()//Notify Manager only
{
	CAction::Do(); //Not finish at this point, since it is drove my keyboard input
}


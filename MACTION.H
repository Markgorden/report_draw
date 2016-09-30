/*************************************************************************/
/* Interface to all the Actions in the editing of network.               */
/*                                                                       */
/* Copyright (c) 2001 Tao Zhu, all rights reserved.						 */
/*																		 */
/*************************************************************************/

#if !defined(MLACTION_H)
#define MLACTION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "undo manager\Action.h"
#include "DrawObj.h"

class CNetworkView;
class CDrawObj;
//class CDrawObjList;

/* CMAction is the base of all action class in Draw project */
class CMAction : public CAction  
{
public:
	CMAction(CNetworkView *aView, UINT actionType);
	virtual ~CMAction();

	virtual void		Undo();
	virtual void		Redo();
	virtual void		Do();

	CNetworkView		*operationTarget;
	int					m_page; //The page that action happens
	int  m_bBuilt;									/*if the system is built*/
};

/*CAddAction is for adding new DrawObj into the system.
	Rectange, line, ecllipse, roundRect, arrow, Text, DrawNode, DrawReaction */

class CAddAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CAddAction(CNetworkView *aView, UINT actionType, CDrawObj *data);
	virtual ~CAddAction();
	virtual	void		Do();
	CDrawObj			*m_data;
	bool				m_bAdded;


protected:
	virtual void		PerformOperation();
};



/*CSizeAction is for resize a single drawObj (not include CDrawReaction) */

class CSizeAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CSizeAction(CNetworkView *aView, UINT actionType,int n, CPoint point, CDrawObj *pObj);
	virtual ~CSizeAction();
	CRect				m_rect;
	CDrawObj			*m_pObj;
	int					nHandle;
	CPoint				oldPoint;
};


/*CMoveAction is for move a group of drawObjs (including CDrawReaction) */
//Need more work!! Since CDrawReaction may change the number of handles.
class CSizeReactionData;

class CMoveAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CMoveAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list);
	virtual ~CMoveAction();
	CDrawObjList		m_list;
	CRect				*m_rects;
	int					m_N;

	int					m_nReaction;
protected:
	virtual void		PerformOperation(bool);
};

class CMoveSingleAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CMoveSingleAction(CNetworkView *aView, UINT actionType, CDrawObj* data);
	virtual ~CMoveSingleAction();
	CDrawObj*			m_data;
	CRect				m_rect;

};






class CDeleteAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CDeleteAction(CNetworkView *aView, UINT actionType,const CDrawObjList &data);
	virtual ~CDeleteAction();
	virtual	void		Do();
	CDrawObjList		m_list;
	int					*m_Pos;
	bool				m_bDeleted;
	int					m_N;
	virtual void		PerformOperation();
	void				UndoOperation();
	void				RedoOperation();
};

/*Paste is annoying action. The first idea is to restore the data that is being pasted. but, wait!. It need too much
memory space. I think the better option is to remmerber what objects (the pointers only) have been copied and pasted.
When action is being done or redone. we need copy these object to clipboard then paste it. they will always be added 
to the tail of CDrawDoc::m_objects. so there is no need to remember the positionl.
For undo, we just simple remove m_nObjects drawObjs from CDrawDoc::m_objects.

CFluxText should not be pasted!!!

*/
class CPasteAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CPasteAction(CNetworkView *aView, UINT actionType);
	virtual ~CPasteAction();
	virtual	void		Do();
	CDrawObjList		m_list;
	int					m_nObjects;
	bool				m_bPasted;
	COleDataObject		m_dataObject;

};


//data class that use for OnEditProperty() event;
//for most drawobj, i use the original class to store information, because they are not big.
//but fot reaction, sum of member variable is not changed through OnEditProperty()
//so i define a smaller class CReactionData to save usage of memory.

class CEditAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CEditAction(CNetworkView *aView, UINT actionType, CDrawObj *pObj);
	virtual ~CEditAction();
	CDrawObj			*m_data;
	CDrawObj			*m_oldData; 
	CDrawObj			*m_newData;
};



//A move action that moves only one selected object.
class CMoveOneStepAction:public CMAction
{
public:
	void				MoveBackward();
	void				MoveForward();
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CMoveOneStepAction(CNetworkView *aView, UINT actionType, bool forward, CDrawObj* data);
	virtual ~CMoveOneStepAction();
	CDrawObj*			m_data;
	bool				m_bForward;
};

//Move the head or Tail
class CMoveStepsAction:public CMAction
{
public:
	void				MoveToBack();
	void				MoveToFront();
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CMoveStepsAction(CNetworkView *aView, UINT actionType, bool forward, CDrawObj *pAfter, CDrawObj* data);
	virtual ~CMoveStepsAction();
	CDrawObj*			m_data;
	bool				m_bForward;
	CDrawObj*			m_pAfter;
};


/*The are seven actions that change the property of a list of objects.(line size, line style, left arrow style, 
	right arrow style,  text color, line color, and fill color */
class CColorAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CColorAction(CNetworkView *aView, UINT actionType, const CDrawObjList &data);
	virtual ~CColorAction();
	virtual	void		Do();
	CDrawObjList		m_list;
	COLORREF			*m_data;
	int					m_N;

protected:
	virtual void		PerformOperation();
};


class CArrowAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CArrowAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list);
	virtual ~CArrowAction();
	virtual	void		Do();
	CDrawObjList		m_list;
	ArrowTypeG 			*m_data;
	int					m_N;

protected:
	virtual void		PerformOperation();
};

class CLineSizeAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CLineSizeAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list);
	virtual ~CLineSizeAction();
	virtual	void		Do();
	CDrawObjList		m_list;
	int					*m_data;
	int					m_N;

protected:
	virtual void		PerformOperation();
};

class CLineStyleAction:public CMAction
{
public:
	void					Redo();
	void					Undo();
	CLineStyleAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list);
	virtual ~CLineStyleAction();
	virtual	void			Do();
	CDrawObjList			m_list;
	int						*m_data;
	int						m_N;

protected:
	virtual void			PerformOperation();
};

class CFontAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CFontAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list);
	virtual ~CFontAction();
	virtual	void		Do();
	CDrawObjList		m_list;
	CCharFormat			*m_data;
	CRect				*m_position;
	int					m_N;

protected:
	virtual void		PerformOperation();
};


//To save memory, CDrawData is used to save the informtion that is changed in Edit action
// which is fired when multiple selection.
class CDrawData{
public:
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
	int						m_penWidth;
	LinearGradientMode      m_linearGradientMode, //for linear gradient only;
							m_penlinearGradientMode;
	HatchStyle              m_hatchStyle;
	int                     m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
	                        m_endArrowSize;
	ArrowTypeG				m_startArrow,
							m_endArrow;
	CString					m_fillImage;
	WrapMode                m_wrapMode;
	BYTE					m_bToCenter; //0, - center path gradient.
	BYTE					m_nStretch;
};

class CEditMultiAction:public CMAction
{
public:
	void GetData(CDrawObj*, CDrawData&);
	void SetData(CDrawObj*, CDrawData&);
	void				Redo();
	void				Undo();
	virtual	void		Do();
	CEditMultiAction(CNetworkView *aView, UINT actionType, const CDrawObjList &list);
	virtual ~CEditMultiAction();
	CDrawData			*m_data;
	CDrawObjList		m_list;
	int					m_N;
protected:
	virtual void		PerformOperation();
};


class CAddPageAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CAddPageAction(CNetworkView *aView, UINT actionType);
	virtual ~CAddPageAction();
	virtual	void		Do();
	int					m_AddedPage;
};

//Delete page will delete all objects in the page, so a deleteAction is fired.
class CDeletePageAction:public CMAction
{
public:
	void				Redo();
	void				Undo();
	CDeletePageAction(CNetworkView *aView, UINT actionType);
	virtual ~CDeletePageAction();
	virtual	void		Do();
	int					n_DeletedPage;
	CDeleteAction		*deleteAction;
};

#endif
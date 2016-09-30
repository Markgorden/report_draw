// UndoManager.cpp: implementation of the CUndoManager class.
//
// Copyright (c) 1998 Matt Clay, all rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UndoManager.h"
#include "Action.h"

#include <assert.h>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUndoManager *gUndoManager = 0;

#ifdef _MFC_VER
UMNotifyFn gUMDefaultNotifier = UMCmdTargetNotifier;
#else
UMNotifyFn gUMDefaultNotifier = 0;
#endif

#ifdef _WIN32
	const UINT UM_Do	= ::RegisterWindowMessage("UndoManager:Do");
	const UINT UM_Undo	= ::RegisterWindowMessage("UndoManager:Undo");
	const UINT UM_Redo	= ::RegisterWindowMessage("UndoManager:Redo");

#ifdef _MFC_VER
	void UMCmdTargetNotifier(void *actionTarget, UMNotificationData &data)
	{
		CCmdTarget *cmdTarget = (CCmdTarget *)actionTarget;

		if (cmdTarget->IsKindOf( RUNTIME_CLASS(CDocument) ))
		{
			((CDocument *)cmdTarget)->SetModifiedFlag(data.isDirty);
		}
	}
#endif
#else
	const UINT UM_Do	= 0;
	const UINT UM_Undo	= 1;
	const UINT UM_Redo	= 2;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActionManager::CActionManager(void *aTarget, NotifyFn fn, long c)
	: actionTarget(aTarget), notifyFn(fn), cleanMarker(0), capacity(c)
{

}

CActionManager::~CActionManager()
{
	DeleteActions(undoStack, undoStack.size());
	DeleteActions(redoStack, redoStack.size());
}

void CActionManager::SetStackCapacity(long nActions)
{
	assert(nActions > 0);

	// the new size may be smaller than the previous stack
	// size, so we may have to delete some initial actions
	if (undoStack.size() > nActions)
		DeleteActions(undoStack, undoStack.size() - nActions);

	if (redoStack.size() > nActions)
		DeleteActions(redoStack, redoStack.size() - nActions);

	capacity = nActions;

	// and the new size may also be greater than our previous
	// clean state; if so, we need to mark the document so that
	// it cannot be cleaned
	if (cleanMarker >= capacity)
		cleanMarker = -1;
}

void CActionManager::SetLastAction(CAction *anAction)
{
	// ensure last action knows its done
	if (undoStack.empty() == false)
	{
		CAction *previousAction = undoStack.back();
		if (previousAction->IsFinished() == false)
			previousAction->Finish();
	}

	// this may just be a notification that the current
	// action is done; if so, the action must finish
	// (above) but nothing else should change.
	if (anAction != 0)
	{
		// a new undoable action cancels any previously redoable ones 
		DeleteActions(redoStack, redoStack.size());

		// remove bottom action if stack is full
		if (undoStack.size() == capacity)
		{
			CAction *firstAction = undoStack.front();

			// we are removing an action which modified the
			// command target (e.g., a document); this effects
			// our ability to undo back into an unmodified state
			cleanMarker = -1;

			undoStack.pop_front();
			delete firstAction;
		}

		// we use the standard reverse vector approach for our stacks,
		// even though we can efficiently push front with a deque; this
		// way however, indexes in the deque remain constant; that makes
		// building menus and tracking the clean marker much easier
		undoStack.push_back(anAction);
		NotifyTarget(UM_Do, 1);
	}
	else
		NotifyTarget(UM_Do, 1);

}

const UMString &CActionManager::GetActionName(StackKind kind, long pos) const
{
	const ActionStack &theStack = (kind == CUndoManager::UNDO) ? undoStack : redoStack;

	assert(pos < theStack.size());
	return theStack[theStack.size() - (pos + 1)]->GetName();
}

long CActionManager::GetStackSize(CUndoManager::StackKind kind) const
{
	const ActionStack &theStack = (kind == CUndoManager::UNDO) ? undoStack : redoStack;
	return theStack.size();
}

bool CActionManager::CanUndo(long nActions) const
{
	return nActions <= undoStack.size();
}

void CActionManager::Undo(long nActions) 
{
	assert( CanUndo(nActions) );

	for(long i = 0; i < nActions; ++i)
	{
		CAction *lastAction = undoStack.back();

		undoStack.pop_back();

		// to undo it, the action must finish
		// whatever its currently doing 
		if (lastAction->IsFinished() == false)
			lastAction->Finish();

		lastAction->Undo();

		// redo size is limited by the undo stack and its
		// limitations are enforced by SetLastAction()
		redoStack.push_back(lastAction);
	}

	// may have undone very first action since last
	// save, or undone something that was saved
	NotifyTarget(UM_Undo, nActions);
}

bool CActionManager::CanRedo(long nActions) const
{
	return nActions <= redoStack.size();
}

void CActionManager::Redo(long nActions) 
{
	assert( CanRedo(nActions) );

	for(long i = 0; i < nActions; ++i)
	{
		CAction *lastAction = redoStack.back();

		redoStack.pop_back();
		lastAction->Redo();

		// undo size is limited by the redo stack, which is
		// limited in turn by the original undo stack, which
		// is limited by SetLastAction() (whew!)
		undoStack.push_back(lastAction);
	}

	NotifyTarget(UM_Redo, nActions);
}

void CActionManager::NotifyTarget(UINT operation, long nActions)
{
	if (notifyFn)
	{
		UMNotificationData data(gUndoManager, operation, nActions, cleanMarker != undoStack.size());
		notifyFn(actionTarget, data);
	}
}

void CActionManager::TargetCleaned()
{
	if (undoStack.size() > 0)
	{
		// finish the last action; once we've
		// saved a document, actions like typing
		// must create a new action
		CAction *lastAction = undoStack.back();

		if (lastAction->IsFinished() == false)
			lastAction->Finish();
	}

	// set the marker to the size of the stack
	// (we must be at that point for the target
	// to be clean)
	cleanMarker = undoStack.size();
}

void CActionManager::DeleteActions(ActionStack &aStack, long nActions)
{
	// delete from the bottom up; because
	// we're using a reverse storage strategy,
	// we walk from front to back
	for(long i = 0; i < nActions; ++i)
	{
		CAction *anAction = aStack.front();
		aStack.pop_front();
		delete anAction;
	}
}

CUndoManager::~CUndoManager()
{
	for(AMTable::iterator amPtr = amTable.begin(); amPtr != amTable.end(); ++amPtr)
		delete *amPtr;
}

void CUndoManager::RegisterTarget(void *actionTarget, long capacity, NotifyFn notifyFn)
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	if ( tablePtr == amTable.end() )
	{
		CActionManager *amPtr = new CActionManager(actionTarget, notifyFn, capacity);
		amTable.push_back(amPtr);
	}
	else
	{
		(*tablePtr)->SetNotifyFn(notifyFn);
		(*tablePtr)->SetStackCapacity(capacity);
	}
}

void CUndoManager::RemoveTarget(void *actionTarget)
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	CActionManager *amPtr = *tablePtr;
	amTable.erase(tablePtr);
	delete amPtr;
}

void CUndoManager::SetLastAction(void *actionTarget, CAction *itsLastAction)
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->SetLastAction(itsLastAction);
}

const UMString &CUndoManager::GetActionName(const void *actionTarget, CUndoManager::StackKind stackKind, long pos) const
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	return (*tablePtr)->GetActionName(stackKind, pos);
}

long CUndoManager::GetStackSize(const void *actionTarget, CUndoManager::StackKind stackKind) const
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	return (*tablePtr)->GetStackSize(stackKind);
}

long CUndoManager::GetStackCapacity(const void *actionTarget) const
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	return (*tablePtr)->GetStackCapacity();
}

void CUndoManager::SetStackCapacity(const void *actionTarget, long capacity)
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->SetStackCapacity(capacity);
}

UMNotifyFn CUndoManager::GetNotifyFn(const void *actionTarget) const
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	return (*tablePtr)->GetNotifyFn();
}

void CUndoManager::SetNotifyFn(const void *actionTarget, NotifyFn fn)
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->SetNotifyFn(fn);
}


bool CUndoManager::CanUndo(const void *actionTarget, long nActions) const
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	return (*tablePtr)->CanUndo(nActions);
}

void CUndoManager::Undo(void *actionTarget, long nActions) 
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->Undo(nActions);
}

bool CUndoManager::CanRedo(const void *actionTarget, long nActions) const
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	return (*tablePtr)->CanRedo(nActions);
}

void CUndoManager::Redo(void *actionTarget, long nActions) 
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->Redo(nActions);
}

bool CUndoManager::CanDo(const void *actionTarget, CUndoManager::StackKind kind, long nActions) const
{
	assert(kind == UNDO || kind == REDO);
	
	return (kind == UNDO) ? CanUndo(actionTarget, nActions) : CanRedo(actionTarget, nActions);
}

void CUndoManager::TargetCleaned(const void *actionTarget) 
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->TargetCleaned();
}

struct IsTargetPredicate
{
	IsTargetPredicate(const void *aTarget) : actionTarget(aTarget) { }

	bool operator()(const CActionManager *amPtr) const
	{
		return actionTarget == amPtr->GetTarget();
	}

	const void *actionTarget;
};

CUndoManager::AMTable::iterator CUndoManager::FindActionManager(const void *actionTarget) const
{
	CUndoManager *that = const_cast<CUndoManager *>(this);
	IsTargetPredicate isTarget(actionTarget);
	AMTable::iterator tablePtr = std::find_if(that->amTable.begin(), that->amTable.end(), isTarget);

	return tablePtr;
}

void CUndoManager::RemoveLastAction(const void *actionTarget) const //remove last action, because last action is failed
//so it is not neccessry to keep it in the undo stack;
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->RemoveLastAction();
}

void CActionManager::RemoveLastAction()
{
	assert( CanUndo(1) );
	undoStack.pop_back();
	// may have undone very first action since last
	// save, or undone something that was saved
	//NotifyTarget(UM_Undo, nActions);
}

void CUndoManager::ClearTarget(const void *actionTarget) const //clear all actions
{
	AMTable::iterator tablePtr = FindActionManager(actionTarget);
	assert(tablePtr != amTable.end());

	(*tablePtr)->ClearTarget();
}

void CActionManager::ClearTarget() //clear all actioin
{
	DeleteActions(undoStack, undoStack.size());
	DeleteActions(redoStack, redoStack.size());
}

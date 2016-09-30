// UndoManager.h: interface for the CUndoManager class.
//
// Copyright (c) 1998 Matt Clay, all rights reserved.
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOMANAGER_H)
#define UNDOMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <deque>
#include <vector>
#include "UMString.h"

class CAction;
class CActionManager;
class CUndoManager;

struct UMNotificationData
{
	UMNotificationData(CUndoManager *um, UINT op, long na, bool d)
		: undoManager(um), operation(op), nActions(na), isDirty(d)
	{}

	CUndoManager *undoManager;
	UINT operation;
	long nActions;
	bool isDirty;
};

typedef void (*UMNotifyFn)(void *actionTarget, UMNotificationData &data);

#ifdef _MFC_VER
	// Standard callbacks (adapters) for notifying an action target
	// the undo manager has made a change that may affect it.

	// Default: if action target is a document, marks it dirty/clean
	void UMCmdTargetNotifier(void *actionTarget, UMNotificationData &data);
#endif

// Used as a default parameter; non-const so you can modify it easily
// while still taking advantage of the default parameter; initially,
// assigned to UMCmdTargetNotifier (if MFC) or 0 (otherwise)
extern UMNotifyFn gUMDefaultNotifier;

#if !defined(UM_DEFAULT_STACK_CAPACITY)
#define UM_DEFAULT_STACK_CAPACITY 25
#endif

class CUndoManager {
public:
	void					RemoveLastAction(const void *actionTarget) const;
	void					ClearTarget		(const void *actionTarget) const;
	enum					StackKind		{ UNDO, REDO };
	typedef					UMNotifyFn		NotifyFn;

							CUndoManager() { }
	virtual					~CUndoManager();

							// all targets must register; note that this routine
							// can be called again to change the settings of a
							// previously registered action target
	virtual void			RegisterTarget(
											void *actionTarget,
											long capacity = UM_DEFAULT_STACK_CAPACITY,
											NotifyFn fn = gUMDefaultNotifier
										  );

							// removes an action target; the actions and stacks
							// associated with the target are deleted
	virtual void			RemoveTarget	(void *actionTarget);

							// add an action to the action target's undo stack; if the
							// argument is null, the previous action is told to finish
	virtual void			SetLastAction	(void *actionTarget, CAction *itsLastAction = 0);

							// undo the last n actions applied to an action target
	virtual void			Undo			(void *actionTarget, long nActions = 1);

							// can we undo the last n actions?
	virtual bool			CanUndo			(const void *actionTarget, long nActions = 1) const;

							// redo the last n undone actions
	virtual void			Redo			(void *actionTarget, long nActions = 1);

							// can we redo the last n undone action?
	virtual bool			CanRedo			(const void *actionTarget, long nActions = 1) const;

							// can we undo or redo the last n actions?
			bool			CanDo			(const void *aTarget, StackKind kind, long nActions = 1) const;

							// call when the action target has been cleaned (saved)
			void			TargetCleaned	(const void *actionTarget);

							// get the name of the action at position p in a stack
	const	UMString &		GetActionName	(const void *actionTarget, StackKind kind, long p) const;

							// get the current size of a stack
			long			GetStackSize	(const void *actionTarget, StackKind kind) const;

							// get/set the maximum size of a stack (both stacks have same max)
			long			GetStackCapacity(const void *actionTarget) const;
			void			SetStackCapacity(const void *actionTarget, long capacity);

							// sets/gets the notification callback
			UMNotifyFn		GetNotifyFn	(const void *actionTarget) const;
			void			SetNotifyFn	(const void *actionTarget, NotifyFn fn);

							// we use a vector here because the items in the table
							// are just pointers, so they don't waste as much space
							// given the STL's policy of rare deallocation, and we
							// often limited to just a few action managers (documents).
	typedef					std::vector<CActionManager *>	AMTable;

protected:
	AMTable::iterator		FindActionManager(const void *actionTarget) const;

	AMTable					amTable;
};

class CActionManager {
public:
	void					RemoveLastAction();
	void					ClearTarget		();
	typedef					CUndoManager::StackKind		StackKind;
	typedef					CUndoManager::NotifyFn	NotifyFn;

							CActionManager		(void *actionTarget, NotifyFn fn, long capacity);
	virtual					~CActionManager		();

			long			GetStackCapacity	()	const { return capacity; }
			void			SetStackCapacity	(long capacity);

			NotifyFn		GetNotifyFn			()	const { return notifyFn; }
			void			SetNotifyFn			(NotifyFn fn) { notifyFn = fn; }

	virtual void			SetLastAction		(CAction *itsLastAction);
	const	UMString &		GetActionName		(StackKind kind, long pos) const;
			long			GetStackSize		(StackKind kind)	const;

	virtual void			Undo				(long nActions = 1);
	virtual bool			CanUndo				(long nActions = 1) const;

	virtual void			Redo				(long nActions = 1);
	virtual bool			CanRedo				(long nActions = 1) const;

	void *					GetTarget			() const { return actionTarget; }
	void					TargetCleaned		();

							// we use a deque so that we can pop off both ends
							// (e.g., off the bottom during overflow) and so we
							// can access all actions, say when building an undo
							// menu, rather than just the top; also note that we
							// push onto the back (like you would with a vector)
	typedef					std::deque<CAction *>				ActionStack;

protected:
	virtual void			DeleteActions		(ActionStack &aStack, long nActions);
	virtual void			NotifyTarget		(UINT operation, long nActions);

	void *					actionTarget;
	ActionStack				undoStack, redoStack;
	NotifyFn				notifyFn;
	long					cleanMarker;
	long					capacity;
};

// an action has been done; undo stack has action pushed
// on top, redo stack has been emptied of all previously
// undone actions
extern const UINT UM_Do;

// an action has been undone; undo stack has been popped,
// redo stack has been pushed with old undo action on top
extern const UINT UM_Undo;

// an action has been redone; redo stack has been popped,
// undo stack has been pushed with old redo action on top
extern const UINT UM_Redo;

// allows customization, but the programmer must explicitly
// create and destroy, probably in an application object
extern CUndoManager *gUndoManager;

#endif

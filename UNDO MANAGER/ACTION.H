// Action.h: interface for the CAction class.
//
// Copyright (c) 1998 Matt Clay, all rights reserved.
//
// Actions are undoable operations. This class abstracts that notion
// by defining the following interface:
//
//		void Do();
//			-- carry out the operation; generally called only once;
//			-- usually, the action must save the existing state before
//			-- performing its operation
//		void Undo();
//			-- called to undo what was done in Do(); usually must
//			-- save the new state, as created by Do(), before reverting
//			-- to the previous state before Do()
//		void Redo();
//			-- called to redo an undone action; generally this is
//			-- different from Do() because all states have already
//			-- been saved explicitly by Do() or Undo(); occasionally,
//			-- a state may be more difficult to store than to compute,
//			-- so then this routine may just call Do()
//
// Each derived class should call its base class's implementation of
// these routines.
//
// Most routines are virtual to allow overrides -- even GetActionName()
// is redefined in one derived action (CREStyleAction). Performance
// is not really a factor as doing the operation the action encapsulates
// will be far more expensive in the general case.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ACTION_H)
#define ACTION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#pragma warning(disable:4786)

#include "UMResources.h"
#include "UMString.h"

class CAction  
{
public:
	CAction(void *actionTarget, UINT actionType = 0, bool autoNotify = true);
	virtual ~CAction();

					// Notifies the Undo Manager of this action; this
					// routine will be called automatically if autoNotify
					// is true; otherwise, it must be called explicitly.
	void			NotifyUndoManager();

	void			SetActionType(const UINT type) { actionType = type; }
	UINT			GetActionType() {return actionType;}


					// Get or set a the name of the action.
	const UMString&	GetName() const;

					// do the operation
	virtual void	Do();

					// undo the operation
	virtual void	Undo();

					// redo the operation
	virtual void	Redo();

					// is the action finished yet?
	bool			IsFinished() const { return finished; }

					// finish the action; useful saving important
					// but last-minute bits of data and ending long
					// duration actions
	virtual void	Finish();

					// is the action done or undone?
	virtual bool	IsUndone() const;

					// was the action successful in performing the
					// operation? generally used for OLE operations
	bool			IsSuccess() const { return success; }

					// should the action notify the undo manager
					// automatically? (you must call notify the
					// undo manager yourself if not.)
	void			SetAutoNotify(bool an) { autoNotify = an; }

protected:
					// a pointer to what the action affects; this is
					// usually a container of some sort -- subclasses
					// add something more specific (e.g., a contained
					// subview); it's generic, cast as necessary
	void *			actionTarget;

					// indicates whether the undo manager should
					// be notified automatically of when an action
					// has been done.
	bool			autoNotify;

					// indicates whether the action is complete;
					// some actions, like typing, have a duration
					// longer than a single call to Do()
	bool			finished;

					// indicates whether the action has been undone or not;
					// after do & redo, it's false; after undo; it's true;
	bool			undone;

					// for operations which have a reasonable probability
					// of failure; generally used in highly dynamic situations
	bool			success;

					// used for both identifying the particular action to
					// perform (e.g., an edit action can be a cut or paste)
					// as well as a resource string describing the action.
	UINT			actionType;

					// name of the action; usually dependent on the action
					// type and obtained from a resource; could be font name
					// or something else though.
	UMString		name;

	bool			done; 
};

#endif

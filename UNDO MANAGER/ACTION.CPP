// Action.cpp: implementation of the CAction class.
//
// Copyright (c) 1998 Matt Clay, all rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Action.h"
#include "UndoManager.h"

#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAction::CAction(void *aTarget, UINT type, bool notify)
	: actionTarget(aTarget), actionType(type), autoNotify(notify),
	  finished(false), undone(false), success(true),done(false)
{
#ifdef _MFC_VER
	if (actionType != 0)
	{
		CString resString;
		resString.LoadString(actionType);
		name = resString;
	}
#endif
}

CAction::~CAction()
{

}

const UMString &CAction::GetName() const
{
	return name;
}

void CAction::NotifyUndoManager()
{
	assert(gUndoManager);
	gUndoManager->SetLastAction(actionTarget, this);
}

void CAction::Do()
{
	if (autoNotify)
		NotifyUndoManager();
	done = true;
}

void CAction::Undo()
{
	undone = true;
}

void CAction::Redo()
{
	undone = false;
}

void CAction::Finish()
{
	finished = true;
}

bool CAction::IsUndone() const
{
	return undone;
}
/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiCommand.h"

nuiCommand::nuiCommand(bool IsUndoable)
{
  SetObjectClass(_T("nuiCommand"));
  SetUndoable(IsUndoable);
}

nuiCommand::~nuiCommand()
{
}

bool nuiCommand::Do()
{
  if (mDone)
    return false;

  mDone = OnDo();
  return mDone;
}

bool nuiCommand::Undo()
{
  if (mDone || !mCanUndo)
    return false;

  mDone = !OnUndo();
  return !mDone;
}

bool nuiCommand::CanUndo()
{
  return mCanUndo;
}

bool nuiCommand::IsDone()
{
  return mDone;
}


bool nuiCommand::OnDo()
{
  return true;
}

bool nuiCommand::OnUndo()
{
  return false;
}


void nuiCommand::SetUndoable(bool set)
{
  mCanUndo = set;
}


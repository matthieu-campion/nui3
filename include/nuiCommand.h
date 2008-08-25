/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiCommand_h__
#define __nuiCommand_h__

//#include "nui.h"
#include "nuiObject.h"

class NUI_API nuiCommand : public nuiObject
{
public:
  nuiCommand(bool CanUndo = false);
  virtual ~nuiCommand();

  bool Do(); ///< Execute the command.
  bool Undo(); ///< Undo the effect of the command
  bool CanUndo(); ///< Return true if the command can be undone.
  bool IsDone(); ///< Return true if the command has been done.


protected:
  virtual bool OnDo(); ///< This method implements the real "do command" code. Override it to add your code.
  virtual bool OnUndo(); ///< This method implements the real "undo command" code. Override it to add your code.

  void SetUndoable(bool set); ///< You can call this method to change the "Undoable" ability of this command.

private:
  bool mCanUndo;
  bool mDone;
};

#endif

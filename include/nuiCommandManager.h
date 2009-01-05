/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"
#include "nuiSingleton.h"
#include "nuiCommand.h"

class nuiCommandManager : public nuiSingleton<nuiCommandManager>
{
    friend class nuiSingleton<nuiCommandManager>;
    
public:

  
  static uint32 GetCommandDescCount();
  static const std::map<nglString, nuiCommandDesc*>& GetCommandDescs();
  
  static void  AddCommandDesc(const nglString& rName, 	nuiCommandDesc* pCommandDesc); 
  static bool  RemoveCommandDesc(const nglString& rName);               
  static nuiCommandDesc* GetCommandDesc(const nglString& rName);
  
  static bool Flatten(); ///< empty the Command Manager Command stacks (for instance, each time a "can't undo" command is executed)
  static bool FlattenDoStack();
  static bool FlattenUndoStack();

  //  static bool Do(const nglString& rCommandName, const Memento& rArguments);
  static bool Do(nuiCommand* pCommand);
  static bool Undo();
  static bool CanUndo();
  static bool Redo();
  static bool CanRedo();
  static bool Repeat();
  static bool CanRepeat();
  
  static nuiCommand* GetLastDoneCmd();
  static nuiCommand* GetLastUndoneCmd();
  static bool IsDoneStackEmpty();
  static bool IsUndoneStackEmpty();
  
  static nuiSimpleEventSource<nuiChanged> CommandChanged;

  
  static nglString  Dump(uint level = 0); // level=[1..n] means only the n last items in the stacks. level=0 means all of them
  static void       DumpFormat(nglString& str, nglChar white = _T(' ')); // format a string to get a specific length
  static void       Trash();
  
private:

  nuiCommandManager();
  ~nuiCommandManager();


  std::list<nuiCommand*> mDoneStack;
  std::list<nuiCommand*> mUndoneStack;
  
  nuiCommand* mpLastDoneSaved;
  nuiCommand* mpLastUndoneSaved;
  
  std::map<nglString, nuiCommandDesc*> mCommands;
  
};



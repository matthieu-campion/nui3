/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"


//
// note to create and use a nuiCommand:
//
// get a look to the "Commands" tutorial in [nui source directory] / tutorials
//


class nuiCommand
{
public:
  
  nuiCommand(const nglString& rName, const nglString& rDescription, bool CanUndo, bool UseCommandManager, bool CanRepeat);
  virtual ~nuiCommand();
  
  bool Do(); ///< Execute the command
  bool Undo();   ///< Undo the command
  
  bool CanUndo() const; ///< Return true if the command supports Undo.
  bool UseCommandManager() const; ///< Return true if the command uses the command manager or is self managed
  bool CanRepeat() const; ///< Return true if the command supports Repeat
  bool IsDone() const;
  
  const nglString& GetName() const;
  const nglString& GetDescription() const;
  void  SetDescription(const nglString& rDescription);
  
  virtual bool SetArgs(const std::vector<nglString>& args)=0; // allows to automatically set the arguments
  // a typed SetArgs method should be given in each nuiCommand inherited class
  virtual nuiCommand* Clone(); // clone the command with the already specified arguments
  
protected:
  void SetCanUndo(bool Set);
  void SetUseCommandManager(bool Set);
  void SetCanRepeat(bool Set);
  
  virtual bool ExecuteDo() = 0; ///< Override this method to implement the Do.
  virtual bool ExecuteUndo();   ///< Override this method to implement the Undo.
  
private:
  nglString mName;
  nglString mDescription;
  bool mIsDone;
  bool mCanUndo;
  // #HACK: in case of commands using dialog boxes, the return value of ExecuteDo is not relevant (user might cancel), so that all those commands are set to not use command manager no matter if they affect the model or not
  bool mUseCommandManager; // Some commands are not affecting the model, so that they don't need to use the command manager... The cmd stack flattening and other operations is performed by the command itself
  bool mCanRepeat;
};



class nuiCommandDesc
{
public:
  nuiCommandDesc(const nglString& rName);
  virtual ~nuiCommandDesc();
  
  void AddArg(const nglString& rName, const nglString& rType);
  const nglString&  GetName() const;
  
  uint32            GetArgCount() const;
  const nglString&  GetArgType(uint32 index) const;
  const nglString&  GetArgName(uint32 index) const;
  nglString         Dump() const;
  
  virtual nuiCommand*   CreateCommand() const=0;
  
  virtual nglString  Comment() const =0;
  
private:
  
  nglString                   mName;
  std::vector<nglString>      mArgsName;
  std::vector<nglString>  mArgsType;
};




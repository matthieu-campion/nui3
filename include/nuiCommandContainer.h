/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once

#include "nui.h"
#include "nuiCommand.h"

typedef std::vector<nuiCommand*> CommandList;

class nuiCommandContainer : public nuiCommand
{
public:
  nuiCommandContainer(const nglString& rName, const nglString& rDescription);
  virtual ~nuiCommandContainer();
  
  virtual bool SetArgs(const std::vector<nglString>& args);
  
  void AddCommand(nuiCommand* pCommand);
  nuiCommand* GetCommand(size_t index) const;
  
protected:
  virtual bool ExecuteDo();
  virtual bool ExecuteUndo();
    
private:
  CommandList mCommands;
  
};

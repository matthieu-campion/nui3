/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

 
#pragma once
 
 
#include "nuiCommand.h"
 
 
class cmdSecondSample: public nuiCommand
{
public:
  cmdSecondSample();
  
  virtual ~cmdSecondSample();
                  
  virtual bool ExecuteDo();    
  virtual bool SetArgs(const std::vector<nglString>& args);

// following method is commented, since we configure this command in order not to be undone, and not to be repeated (=>it can not be cloned)
//  virtual bool ExecuteUndo();
//    virtual nuiCommand* Clone(); // clone the command with the already specified arguments
    
    
private:

  class Desc : nuiCommandDesc
  {
  public:
      Desc();
      virtual nuiCommand* CreateCommand() const;
      virtual nglString Comment() const;
  };
  
  static Desc ThisDesc;
  
};
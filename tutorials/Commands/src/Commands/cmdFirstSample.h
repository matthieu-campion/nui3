/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

 
 
#pragma once
 
 
#include "nuiCommand.h"
 
 
class cmdFirstSample: public nuiCommand
{
public:
  cmdFirstSample();
  
  virtual ~cmdFirstSample();
                  
  virtual bool ExecuteDo();    
  virtual bool ExecuteUndo();
  virtual bool SetArgs(const std::vector<nglString>& args);

  // Clone is used to "repeat" a command
  virtual nuiCommand* Clone(); // clone the command with the already specified arguments

  // you are free to add your own setters for coding convenience,  especially if you don't use the virtual SetArgs method
  // inherited from the nuiCommand class, which is most usefull if you wish to run your application using an external script.
  bool SetArgs(int32 number1, int32 number2);
  
    
private:

  class Desc : nuiCommandDesc
  {
  public:
      Desc();
      virtual nuiCommand* CreateCommand() const;
      virtual nglString Comment() const;
  };
  
  static Desc ThisDesc;
  
  // command arguments
  int32 mArg1;
  int32 mArg2;
};
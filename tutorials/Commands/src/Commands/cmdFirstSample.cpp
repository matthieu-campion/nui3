/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

 
#include "./Commands/cmdFirstSample.h"
#include "MainWindow.h"
#include "Application.h"
 
cmdFirstSample::cmdFirstSample() : nuiCommand(_T("FirstCommandRef"), _T("first Sample command"), true/*can undo*/, true/* use command manager*/, true/*can repeat*/)
{
  mArg1 = mArg2 = 0;
}
  
cmdFirstSample::~cmdFirstSample()
{
    
}
    


//************************************************************
//
// ExecuteDo
//
//                    
bool cmdFirstSample::ExecuteDo()
{
  uint32 iWantToDoSomething = mArg1 + mArg2;
  
  GetMainWindow()->AddMessage(_T("cmdFirstSample ExecuteDo : %d + %d = %d\n"), mArg1, mArg2, iWantToDoSomething);
  return true;
}
    



//************************************************************
//
// ExecuteUndo
//
//    
bool cmdFirstSample::ExecuteUndo()
{
  // here you can do everything you want to "undo" the action programmed in ::ExecuteDo()
  GetMainWindow()->AddMessage(_T("cmdFirstSample ExecuteUndo"));
  return true;
}
    


//************************************************************
//
// SetArgs
//
//
bool cmdFirstSample::SetArgs(const std::vector<nglString>& args)
{
  mArg1 = args[1].GetInt();
  mArg2 = args[2].GetInt();
  return true;
}



//************************************************************
//
// SetArgs
//
//
bool cmdFirstSample::SetArgs(int32 number1, int32 number2)
{
  mArg1 = number1;
  mArg2 = number2;
  return true;
}
    



//************************************************************
//
// Clone
//
//
nuiCommand* cmdFirstSample::Clone()
{
  cmdFirstSample* newCmd = new cmdFirstSample ();
  newCmd->SetArgs(mArg1, mArg2);
  return (nuiCommand*)newCmd;
}








//***********************************************************************************************************
//
// CommandDesc
//
//***********************************************************************************************************


//************************************************************
//
// Constr
//
//
cmdFirstSample::Desc::Desc() : nuiCommandDesc(_T("FirstCommandRef"))
{
  AddArg(_T("number1"), _T("int"));
  AddArg(_T("number2"), _T("int"));
}
        


//************************************************************
//
// CreateCommand
//
//
nuiCommand* cmdFirstSample::Desc::CreateCommand() const
{
  cmdFirstSample* newCmd = new cmdFirstSample ();
  return newCmd;
}
        

//************************************************************
//
// Comment
//
//
nglString cmdFirstSample::Desc::Comment() const
{
  return _T("this a sample command. It simply adds two integer numbers.");
}
        
        

        
        
cmdFirstSample::Desc cmdFirstSample::ThisDesc;
 


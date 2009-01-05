/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */



#include "./Commands/cmdSecondSample.h"
 
 

cmdSecondSample::cmdSecondSample() : nuiCommand(_T("SecondCommandRef"), _T("second Sample command"), false/*can undo*/, true/* use command manager*/, false/*can repeat*/)
{
}
  
    
cmdSecondSample::~cmdSecondSample()
{
    
}
    


//************************************************************
//
// ExecuteDo
//
//                    
bool cmdSecondSample::ExecuteDo()
{
  NGL_OUT(_T("SampleCommand ExecuteDo\n"));
  return true;
}
    


    


//************************************************************
//
// SetArgs
//
//
bool cmdSecondSample::SetArgs(const std::vector<nglString>& args)
{
  return true;
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
cmdSecondSample::Desc::Desc() : nuiCommandDesc(_T("SecondCommandRef"))
{
}
        


//************************************************************
//
// CreateCommand
//
//
nuiCommand* cmdSecondSample::Desc::CreateCommand() const
{
  cmdSecondSample* newCmd = new cmdSecondSample ();
  return newCmd;
}
        

//************************************************************
//
// Comment
//
//
nglString cmdSecondSample::Desc::Comment() const
{
  return _T("this a sample command. It does nothing special:)");
}
        
        

        
        
cmdSecondSample::Desc cmdSecondSample::ThisDesc;
 


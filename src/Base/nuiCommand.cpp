/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"



nuiCommand::nuiCommand(const nglString& rName, const nglString& rDescription, bool CanUndo, bool UseCommandManager, bool CanRepeat)
: mName(rName), mDescription(rDescription), mCanUndo(CanUndo), mUseCommandManager(UseCommandManager), mCanRepeat(CanRepeat), mIsDone(false)
{
  
}


nuiCommand::~nuiCommand()
{
  
}


bool nuiCommand::Do()
{
  mIsDone = ExecuteDo();
  return mIsDone;
}


bool nuiCommand::Undo()
{
  NGL_ASSERT(IsDone());
  mIsDone = ExecuteUndo();
  return mIsDone;
}


bool nuiCommand::ExecuteUndo()
{
  return false;
}

nuiCommand* nuiCommand::Clone()
{
  return NULL;
}


bool nuiCommand::CanUndo() const
{
  return mCanUndo;
}

bool nuiCommand::UseCommandManager() const
{
  return mUseCommandManager;
}

bool nuiCommand::CanRepeat() const
{
  return mCanRepeat;
}

bool nuiCommand::IsDone() const
{
  return mIsDone;
}

const nglString& nuiCommand::GetDescription() const
{
  return mDescription;
}

void nuiCommand::SetDescription(const nglString& rDescription)
{
  mDescription = rDescription;
}

void nuiCommand::SetCanUndo(bool Set)
{
  mCanUndo = Set;
}

void nuiCommand::SetUseCommandManager(bool Set)
{
  mUseCommandManager = Set;
}

void nuiCommand::SetCanRepeat(bool Set)
{
  mCanRepeat = Set;
}



const nglString& nuiCommand::GetName() const
{
  return mName;
}






//***************************************************************************************************************************
//
//
// nuiCommandDesc
//
//
//***************************************************************************************************************************



nuiCommandDesc::nuiCommandDesc(const nglString& rName)
: mName(rName)
{
  // register the commanddesc in the command manager
  nuiCommandManager::AddCommandDesc(rName, this);
}


nuiCommandDesc::~nuiCommandDesc()
{

}


//************************************************************
//
// AddArg
//
//  
void nuiCommandDesc::AddArg(const nglString& rName, const nglString& rType)
{
  mArgsName.push_back(rName);
  mArgsType.push_back(rType);
}



//************************************************************
//
// GetName
//
//  
const nglString& nuiCommandDesc::GetName() const
{
  return mName;
}



//************************************************************
//
// GetArgCount
//
//  
uint32 nuiCommandDesc::GetArgCount() const
{
  return (uint32)mArgsName.size();
}



//************************************************************
//
// GetArgType
//
//
const nglString& nuiCommandDesc::GetArgType(uint32 index) const
{
  return mArgsType[index];
}


//************************************************************
//
// GetArgName
//
//
const nglString& nuiCommandDesc::GetArgName(uint32 index) const
{
  return mArgsName[index];
}




//************************************************************
//
// Dump
//
//
nglString nuiCommandDesc::Dump() const
{
  nglString res = mName;
  
  
  for (uint32 i=0; i < (uint32)mArgsName.size(); i++)
  {
    res.Append(_T(" (")).Append(mArgsType[i]).Append(_T(")")).Append(mArgsName[i]);
  }
  
  return res;
}




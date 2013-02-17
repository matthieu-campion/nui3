/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

using namespace std;

nuiCommandContainer::nuiCommandContainer(const nglString& rName, const nglString& rDescription)
: nuiCommand(rName, rDescription, true, true, true) 
{
}

nuiCommandContainer::~nuiCommandContainer()
{
  CommandList::iterator end = mCommands.end();
  for (CommandList::iterator it = mCommands.begin(); it != end; ++it)
  {
    delete (*it);
  }
}

bool nuiCommandContainer::SetArgs(const std::vector<nglString>& args)
{
  return false;
}

void nuiCommandContainer::AddCommand(nuiCommand* pCommand)
{
  mCommands.push_back(pCommand);
  SetCanUndo(CanUndo() && pCommand->CanUndo());
  SetUseCommandManager(UseCommandManager() && pCommand->UseCommandManager());
  SetCanRepeat(CanRepeat() && pCommand->CanRepeat());
}

nuiCommand* nuiCommandContainer::GetCommand(size_t index) const
{
  if (index<mCommands.size())
  {
    return mCommands[index];
  }
  else
  {
    return NULL;
  }
}

bool nuiCommandContainer::ExecuteDo()
{
  CommandList::iterator end = mCommands.end();
  bool res = true;
  for (CommandList::iterator it = mCommands.begin(); res && (it != end); ++it)
  {
    nuiCommand* pCommand = *it;
    res = pCommand->Do();
  }
  return res;
}

bool nuiCommandContainer::ExecuteUndo()
{
  CommandList::reverse_iterator end(mCommands.begin());
  bool res = true;
  for (CommandList::reverse_iterator it(mCommands.end()); res && (it != end); ++it)
  {
    nuiCommand* pCommand = *it;
    res = pCommand->Undo();
  }
  return res;
}

/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h" 
#include "nuiCommandManager.h"
#include "nuiCommand.h"


nuiCommandManager::nuiCommandManager()
{
  mpLastDoneSaved = NULL;
  mpLastUndoneSaved = NULL;
  
}

nuiCommandManager::~nuiCommandManager()
{

    // clean command stacks
    
    std::list<nuiCommand*>::iterator it;
    
    for (it = mDoneStack.begin(); it != mDoneStack.end (); ++it)
    {
        nuiCommand* ptr = *it;
        delete ptr;
    }

    for (it = mUndoneStack.begin(); it != mUndoneStack.end (); ++it)
    {
        nuiCommand* ptr = *it;
        delete ptr;
    }

    
}
 
 

//************************************************************
//
// Trash : delete singleton
//
//
void nuiCommandManager::Trash()
{
	nuiCommandManager*	inst = Instance();
  delete inst;
}
 
 
 
  
   
//************************************************************
//
// GetCommandDescCount
//
//     
uint32 nuiCommandManager::GetCommandDescCount()
{
    nuiCommandManager*	inst = Instance();
    return (uint32)inst->mCommands.size ();
}



//************************************************************
//
// GetCommandDescs
//
//
const std::map<nglString, nuiCommandDesc*>& nuiCommandManager::GetCommandDescs()
{   
    nuiCommandManager*	inst = Instance();
    return inst->mCommands;
}



//************************************************************
//
// AddCommandDesc
//
//
void nuiCommandManager::AddCommandDesc(const nglString& rName, nuiCommandDesc* pCommandDesc)
{
    nuiCommandManager*	inst = Instance();
    inst->mCommands [rName] = pCommandDesc;
}


//************************************************************
//
// RemoveCommandDesc
//
//
bool nuiCommandManager::RemoveCommandDesc(const nglString& rName)
{
    nuiCommandManager*	inst = Instance();
    
    std::map<nglString, nuiCommandDesc*>::iterator itr = inst->mCommands.find(rName);
    if (itr == inst->mCommands.end()) return false;
    
    inst->mCommands.erase (itr);
    return true;
   
}




//************************************************************
//
// GetCommandDesc
//
//
nuiCommandDesc* nuiCommandManager::GetCommandDesc(const nglString& rName)
{
    nuiCommandManager*	inst = Instance();
    
    // find and check
    std::map<nglString, nuiCommandDesc*>::const_iterator itr = inst->mCommands.find(rName);
    if (itr == inst->mCommands.end())
        return NULL;
    return itr->second;    
}
 
  
  
  
  
  
//************************************************************
//
// Flatten
//
//   
bool nuiCommandManager::Flatten()
{
  nuiCommandManager* inst = Instance();
  
  std::list<nuiCommand*>::iterator it;
  
  for (it = inst->mDoneStack.begin(); it != inst->mDoneStack.end(); ++it)
  {
    nuiCommand* pCmd = *it;
    delete pCmd;
  }

  for (it = inst->mUndoneStack.begin(); it != inst->mUndoneStack.end(); ++it)
  {
    nuiCommand* pCmd = *it;
    delete pCmd;
  }
  
  inst->mDoneStack.clear ();
  inst->mUndoneStack.clear ();
  
  // send event
  CommandChanged();
  
  return true;
}

//************************************************************
//
// FlattenDoStack
//
//   
bool nuiCommandManager::FlattenDoStack()
{
  nuiCommandManager* inst = Instance();
  
  std::list<nuiCommand*>::iterator it;
  
  for (it = inst->mDoneStack.begin(); it != inst->mDoneStack.end(); ++it)
  {
    nuiCommand* pCmd = *it;
    delete pCmd;
  }
  
  inst->mDoneStack.clear ();
  // send event
  CommandChanged();
  
  return true;
}

//************************************************************
//
// FlattenUndoStack
//
//   
bool nuiCommandManager::FlattenUndoStack()
{
  nuiCommandManager* inst = Instance();
  
  std::list<nuiCommand*>::iterator it;
  
  for (it = inst->mUndoneStack.begin(); it != inst->mUndoneStack.end(); ++it)
  {
    nuiCommand* pCmd = *it;
    delete pCmd;
  }
  
  inst->mUndoneStack.clear();
  // send event
  CommandChanged();
  
  return true;
}



//************************************************************
//
// Do
//
//   

//bool nuiCommandManager::Do(const nglString& rName, const Memento& rArguments)
//{
//  nuiCommandManager*	inst = Instance();
//
//  // check
//  std::map<nglString, nuiCommandDesc*>::const_iterator itr = inst->mCommands.find(rName);
//  if (itr == inst->mCommands.end())
//  return false;
//
//  // build command
//  nuiCommand* newCommand = itr->second->CreateCommand();
//
//      
//  // store and execute
//  return Do(newCommand);
//}





//************************************************************
//
// Do
//
//
bool nuiCommandManager::Do(nuiCommand* pCommand)
{
  nuiCommandManager*	inst = Instance();
  
  // Is the command undo-able ?
  // If not, pop up a warning notifying user that do/undo stacks will be cleared (if the command succeeds)
  if (pCommand->UseCommandManager() && !pCommand->CanUndo())
  {
    // #TODO
  }  
  
  // execute
  bool res = pCommand->Do();
  
  if (!res)
  {
    delete pCommand;
    return false;
  }
  
  if (pCommand->UseCommandManager())
  {
    // if the command can be undone, store it in done stack and clear undo stack 
    if (pCommand->CanUndo())
    {
      inst->mDoneStack.push_back(pCommand);
      FlattenUndoStack();
    }
    // otherwise, flatten all the Command stacks 
    else
    {
      inst->Flatten();
      delete pCommand;
    }
    // refresh gui (undo / redo / repeat / save menu items + unsavedchanges notification)
    CommandChanged();
  }
  else // The command resets what's needed by itself, juste delete the command
  {
    delete pCommand;
  }
  
  return true;
}



//************************************************************
//
// CanUndo
//
//
bool nuiCommandManager::CanUndo()
{
  nuiCommandManager* inst = Instance();
  if (inst->mDoneStack.empty()) return false;
  
  nuiCommand* last = inst->mDoneStack.back();
  return last->CanUndo();
}




//************************************************************
//
// Undo
//
//
bool nuiCommandManager::Undo()
{
  nuiCommandManager*	inst = Instance();
  
  //check
  if (0 == inst->mDoneStack.size())
  {
      NGL_OUT(_T("command manager can't undo : empty commands stack!\n"));
      return false;
  }
  
  // get the last command
  nuiCommand* last = inst->mDoneStack.back();
  
  if (!last->CanUndo())
  {
      NGL_OUT(_T("command manager : %s can't undo!\n"), last->GetName().GetChars());
      return false;
  }
  
  
  // execute undo
  bool res= last->Undo();

  inst->mDoneStack.pop_back();
  
  if (res)
  {
    // move the last command from the Done stack to the Undone stack
    inst->mUndoneStack.push_back(last);
  }
  else
  {
    delete last;
  }
  
  // refresh gui
  CommandChanged();
    
    return res;
}




//************************************************************
//
// CanRedo
//
//
bool nuiCommandManager::CanRedo()
{
  nuiCommandManager* inst = Instance();
  if (inst->mUndoneStack.empty()) return false;
  return true;
}




//************************************************************
//
// Redo
//
//
bool nuiCommandManager::Redo()
{
  nuiCommandManager*	inst = Instance();

  //check
  if (0 == inst->mUndoneStack.size())
  {
      NGL_OUT(_T("command manager can't redo : empty undone commands stack!\n"));
      return false;
  }    

  // get the last undone command
  nuiCommand* last = inst->mUndoneStack.back();



  // execute redo
  bool res = last->Do();

  inst->mUndoneStack.pop_back();
  
  if (res)
  {
    // move the last command from the Undone stack to the Done stack
    inst->mDoneStack.push_back (last);
  }
  else
  {
    delete last;
  }
  
  // refresh gui
  CommandChanged();

  return res;

}
  
  

//************************************************************
//
// CanRepeat
//
//
bool nuiCommandManager::CanRepeat()
{
  nuiCommandManager* inst = Instance();
  if (inst->mDoneStack.empty()) return false;
  nuiCommand* last = inst->mDoneStack.back();
  return last->CanRepeat();
}
  
  

//************************************************************
//
// Repeat
//
//
bool nuiCommandManager::Repeat()
{
  nuiCommandManager*	inst = Instance();
  
  //check
  if (0 == inst->mDoneStack.size())
  {
      NGL_OUT(_T("command manager can't repeat : empty commands stack!\n"));
      return false;
  }
  
  // get the last command
  nuiCommand* last = inst->mDoneStack.back();
  
  if (!last->CanRepeat())
  {
      NGL_OUT(_T("command manager : %s can't repeat!\n"), last->GetName().GetChars());
      return false;
  }
  
  nuiCommand* repeatCmd = last->Clone();
  
  
  // execute
  bool res = repeatCmd->Do();
  
  // flat the undo stack
  inst->FlattenUndoStack();

  if (res)
  {
    // store the clone
    inst->mDoneStack.push_back (repeatCmd);
    // refresh gui
    CommandChanged();
  }
  else
  {
    delete repeatCmd;
  }
  
	return res;
}  
  
nuiCommand* nuiCommandManager::GetLastDoneCmd()
{
  nuiCommandManager* pManager = Instance();
  if (pManager->mDoneStack.size() > 0)
  {
    return pManager->mDoneStack.back();
  }
  else
  {
    return NULL;
  }
}

nuiCommand* nuiCommandManager::GetLastUndoneCmd()
{
  nuiCommandManager* pManager = Instance();
  if (pManager->mUndoneStack.size() > 0)
  {
    return pManager->mUndoneStack.back();
  }
  else
  {
    return NULL;
  }
}
  
bool nuiCommandManager::IsDoneStackEmpty()
{
  nuiCommandManager* pManager = Instance();
  return (pManager->mDoneStack.empty());
}
bool nuiCommandManager::IsUndoneStackEmpty()
{
  nuiCommandManager* pManager = Instance();
  return (pManager->mUndoneStack.empty());
}
  
#define CMDMNG_DUMPFORMAT_STRLEN 30

//************************************************************
//
// DumpFormat 
//
// format a string to get a specific length
//
//
void nuiCommandManager::DumpFormat(nglString& str, nglChar white)
{
uint length = str.GetLength();

    if (length < CMDMNG_DUMPFORMAT_STRLEN)
        for (uint i= length; i < CMDMNG_DUMPFORMAT_STRLEN; i++)
            str.Append(white);
            
    else if (length < CMDMNG_DUMPFORMAT_STRLEN)
    {
        str.DeleteRight(5);
        str.Append(_T("[...]"));
    }
    
    str.Prepend(white);
    str.Append(white);
    
}
  
  
  
//************************************************************
//
// Dump  : prints the list of the Done Commands and Undo Commands
//
// level=[1..n] means only the n last items in the stacks. level=0 means all of them
//
//
nglString nuiCommandManager::Dump(uint level)
{
nglString res;
nglString dStr, uStr;
nglString row;


    nuiCommandManager*	inst = Instance();

    inst->DumpFormat(row, _T('-'));
    row.Append(row);
    
    dStr.Format(_T("Done Commands stack [%d]"),(uint32)inst->mDoneStack.size());
    uStr.Format(_T("Undone Commands stack [%d]"),(uint32)inst->mUndoneStack.size());
    
    inst->DumpFormat(dStr);
    inst->DumpFormat(uStr);
    
    res.Format(_T("-%s-\n|%s|%s|\n"), row.GetChars(), dStr.GetChars(), uStr.GetChars());
    res.Append(_T("-")).Append(row).Append(_T("-\n"));
    
    
    std::list<nuiCommand*>::iterator dItr = inst->mDoneStack.end();
    std::list<nuiCommand*>::iterator uItr = inst->mUndoneStack.end();
    
    int32 dLevel=inst->mDoneStack.size();
    int32 uLevel=inst->mUndoneStack.size();
    int32 i=(level == 0)? dLevel+uLevel : level;
    do
    {
    nglString tmp;
     
        i--; dLevel--; uLevel--;
    
            
        if (dLevel>=0)
        {
            --dItr;
            dStr = (*dItr)->GetName();
        }
        else dStr = nglString::Null;

        if (uLevel>=0)
        {
            --uItr;
            uStr = (*uItr)->GetName();
        }
        else uStr = nglString::Null;
        
        inst->DumpFormat(dStr);
        inst->DumpFormat(uStr);
        
        tmp.Format(_T("|%s|%s|\n"), dStr.GetChars(), uStr.GetChars());
        res.Append(tmp);

    }
    while ( ((dLevel>=0) || (uLevel>=0))   && (i > 0));
    
    res.Append(_T("-")).Append(row).Append(_T("-\n"));

    return res;
    

}



nuiSimpleEventSource<nuiChanged> nuiCommandManager::CommandChanged;


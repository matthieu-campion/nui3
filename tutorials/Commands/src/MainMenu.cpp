/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "MainMenu.h"
#include "MainWindow.h"
#include "Application.h"

#include "nuiCommandManager.h"
#include "nuiCommand.h"


enum mxMenuCommand
{
  eMenu1Cmd1 = 0,
  eMenu1Cmd2,
  
  eMenu2Undo,
  eMenu2Redo,
  eMenu2Repeat
};



MainMenu::MainMenu()
: nuiMainMenu(NULL),mEventSink(this)
{
  Init();
  
  
  // root item for the menu bar. it's not displayed
  nuiMainMenuItem* pRoot = new nuiMainMenuItem(_T("MainMenuRoot")); 
  
  // create the main menu object, you'll have to register it from the main window
  SetRoot(pRoot);
  
  
  nuiMainMenuItem* pMenu1 = new nuiMainMenuItem(_T("Menu1"));
  pRoot->AddChild(pMenu1);
  nuiMainMenuItem* pMenu2 = new nuiMainMenuItem(_T("Menu2"));
  pRoot->AddChild(pMenu2);
  
  
  //***********************************************************
  // menu 1
  //
  nuiMainMenuItem* pItem = new nuiMainMenuItem(_T("First Command"));
  pMenu1->AddChild(pItem);
  mEventSink.Connect(pItem->Activated, &MainMenu::OnMenuCommand, (void*)eMenu1Cmd1);

  pItem = new nuiMainMenuItem(_T(""), eItemSeparator);
  pMenu1->AddChild(pItem);
  
  pItem = new nuiMainMenuItem(_T("Second Command"));
  pMenu1->AddChild(pItem);
  mEventSink.Connect(pItem->Activated, &MainMenu::OnMenuCommand, (void*)eMenu1Cmd2);
  
  
  //***********************************************************
  // menu 2
  //

  mpUndo = new nuiMainMenuItem(_T("Undo"));
  pMenu2->AddChild(mpUndo);
  mEventSink.Connect(mpUndo->Activated, &MainMenu::OnMenuCommand, (void*)eMenu2Undo);
  
  mpRedo = new nuiMainMenuItem(_T("Redo"));
  pMenu2->AddChild(mpRedo);
  mEventSink.Connect(mpRedo->Activated, &MainMenu::OnMenuCommand, (void*)eMenu2Redo);
  
  mpRepeat = new nuiMainMenuItem(_T("Repeat"));
  pMenu2->AddChild(mpRepeat);
  mEventSink.Connect(mpRepeat->Activated, &MainMenu::OnMenuCommand, (void*)eMenu2Repeat);
  
  mpUndo->SetEnabled(nuiCommandManager::CanUndo());
  mpRedo->SetEnabled(nuiCommandManager::CanRedo());
  mpRepeat->SetEnabled(nuiCommandManager::CanRepeat());  
    
  
  //*******************************************************************
  // undo, redo and repeat must update after each Command
  mEventSink.Connect(nuiCommandManager::CommandChanged, &MainMenu::OnCommandChanged);
  
}


MainMenu::~MainMenu()
{
}



void MainMenu::Init()
{
  mCommands[eMenu1Cmd1] = _T("FirstCommandRef");
  mCommands[eMenu1Cmd2] = _T("SecondCommandRef");

  mCommands[eMenu2Undo] = _T("Undo");
  mCommands[eMenu2Redo] = _T("Redo");
  mCommands[eMenu2Repeat] = _T("Repeat");  
}



bool MainMenu::OnMenuCommand(const nuiEvent& rEvent)
{
  uint32 menuCmd = (uint32)rEvent.mpUser;
  
  // undo, redo and repeat are special cases
  switch (menuCmd)
  {
    case eMenu2Undo:
      return nuiCommandManager::Undo();
  
    case eMenu2Redo:
      return nuiCommandManager::Redo();
  
    case eMenu2Repeat:
      return nuiCommandManager::Repeat();
      
    // in our sample application, we automatically launch any command from the menu,
    // withouth any arguments.
    // Obviously, you're free to make your own system depending on your application purpose.
    //
    default:
    {
      std::map<uint32,nglString>::iterator it = mCommands.find(menuCmd);
      NGL_ASSERT(it != mCommands.end());
      const nglString cmd = it->second;
      
      nuiCommandDesc* cmdDesc = nuiCommandManager::GetCommandDesc(cmd);
      NGL_ASSERT(cmdDesc != NULL);
      nuiCommand* pCommand = cmdDesc->CreateCommand();
      nuiCommandManager::Do(pCommand);      
    }
  }
  
  return true;
}



bool MainMenu::OnCommandChanged(const nuiEvent& rEvent)
{
  // update undo / redo menu
  bool canundo = nuiCommandManager::CanUndo();
  bool canredo =  nuiCommandManager::CanRedo();
  bool canrepeat = nuiCommandManager::CanRepeat();
  
  nuiCommand* pLastDoneCmd = nuiCommandManager::GetLastDoneCmd();
  nuiCommand* pLastUndoneCmd = nuiCommandManager::GetLastUndoneCmd();
  
  // update Undo MenuItem text
  mpUndo->SetEnabled(canundo);
  nglString undotext = _T("Undo");
  if (canundo && pLastDoneCmd)
  {
    undotext.Add(_T(" '")).Add(pLastDoneCmd->GetDescription()).Add(_T("'"));
  }
  SetItemText(mpUndo, undotext);
  
  
  // update Redo MenuItem text
  mpRedo->SetEnabled(canredo);
  nglString redotext = _T("Redo");
  if (canredo && pLastUndoneCmd)
  {
    redotext.Add(_T(" '")).Add(pLastUndoneCmd->GetDescription()).Add(_T("'"));
  }
  SetItemText(mpRedo, redotext);
  
  
  // update Repeat MenuItem text
  mpRepeat->SetEnabled(canrepeat);
  nglString repeattext = _T("Repeat");
  if (canrepeat && pLastDoneCmd)
  {
    repeattext.Add(_T(" '")).Add(pLastDoneCmd->GetDescription()).Add(_T("'"));
  }
  SetItemText(mpRepeat, repeattext);
  
  
  // update monitor info
  GetMainWindow()->UpdateCommandManagerInfo();
  
  return true;
}





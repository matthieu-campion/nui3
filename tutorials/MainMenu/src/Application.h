/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#pragma once

#include "nui.h"
#include "nuiApplication.h"
#include "nuiMainMenu.h"

class MainWindow;

class Application : public nuiApplication
{
public:
  Application();
  ~Application();

  void OnInit();
  void OnExit (int Code);
  
  nuiMainMenu* GetMainMenu();
  nuiMainMenuItem* GetTestMenu();

protected:
  
  void InitMainMenu();
  
  // menu commands' receivers
  bool OnMenuFileOpen(const nuiEvent& rEvent);
  bool OnMenuFileClose(const nuiEvent& rEvent);
  bool OnMenuFileDI(const nuiEvent& rEvent);
  bool OnMenuEditCopy(const nuiEvent& rEvent);
  bool OnMenuEditPaste(const nuiEvent& rEvent);  
  bool OnMenuEditOptionChecked(const nuiEvent& rEvent);  
  bool OnMenuEditPrefOption1Checked(const nuiEvent& rEvent);  
  
  MainWindow* mpMainWindow;
  nuiMainMenu* mpMainMenu;
  nuiMainMenuItem* mpMenuTest;
  
  nuiEventSink<Application> mEventSink;
};


// a global call to retrieve the application object
Application* GetApp();
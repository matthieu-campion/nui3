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
  void OnMenuFileOpen(const nuiEvent& rEvent);
  void OnMenuFileClose(const nuiEvent& rEvent);
  void OnMenuFileDI(const nuiEvent& rEvent);
  void OnMenuEditCopy(const nuiEvent& rEvent);
  void OnMenuEditPaste(const nuiEvent& rEvent);  
  void OnMenuEditOptionChecked(const nuiEvent& rEvent);  
  void OnMenuEditPrefOption1Checked(const nuiEvent& rEvent);  
  
  MainWindow* mpMainWindow;
  nuiMainMenu* mpMainMenu;
  nuiMainMenuItem* mpMenuTest;
  
  nuiEventSink<Application> mEventSink;
};


// a global call to retrieve the application object
Application* GetApp();
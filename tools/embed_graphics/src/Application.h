/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiApplication.h"
#include "PreferencesBase.h"

class MainWindow;

//#define APP_WIDTH 400
//#define APP_HEIGHT 200

#define APP_FULL_WIDTH 1024
#define APP_FULL_HEIGHT 768

class Application : public nuiApplication
{
public:
  Application();
  ~Application();

  void OnInit();
  void OnExit (int Code);

  MainWindow* GetMainWindow();
  PreferencesBase& GetPreferences();
private:
  
  MainWindow* mpMainWindow;
  PreferencesBase mPrefs;
  
};


// a global call to retrieve the application object
Application* GetApp();
MainWindow* GetMainWindow();

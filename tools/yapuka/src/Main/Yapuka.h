/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "Main/MainWindow.h"
#include "Main/Preferences.h"



class nuiApp : public nuiApplication
{
public:
  nuiApp();
  ~nuiApp();

  void OnInit();
  void OnExit (int Code);
	MainWindow* GetMainWindow();
  
  
  Preferences& GetPreferences()
  {
    return mPreferences;
  }
  
  bool IsFullVersion()
  {
    return mFullVersion;
  }
  
protected:
   MainWindow *win;
  
private:
  Preferences mPreferences;
  bool mFullVersion;
};

nuiApp* GetApp();
MainWindow* GetMainWindow();
Preferences& GetPreferences();

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "ngl.h"
#include "nuiApplication.h"
#include "nuiWindow.h"
#include "DecorationManager.h"
#include "Preferences.h"

#define PROPERTY_STYLE _T("Style")

class MainWindow;

class nuiApp : public nuiApplication
{
public:
  nuiApp();
  ~nuiApp();

  void OnInit();
  void OnExit (int Code);
	MainWindow* GetMainWindow();
  DecorationManager* GetDecorationManager();
  
  
  Preferences& GetPreferences()
  {
    return mPreferences;
  }
  
protected:
   MainWindow *win;
   DecorationManager* mpDecorationManager;
  
private:
  Preferences mPreferences;
};


MainWindow* GetMainWindow();
DecorationManager* GetDecorationManager();
Preferences& GetPreferences();

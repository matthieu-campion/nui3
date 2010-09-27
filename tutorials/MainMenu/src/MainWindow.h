/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiMainMenu.h"

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();
  
  void MyCommand(const nglString& rEvent);
  
protected:

  void OnAddItem(const nuiEvent& rEvent);
  void OnAddSubMenu(const nuiEvent& rEvent);
  void OnMenuCommand(const nuiEvent& rEvent);

  nglString mText;
  nuiLabel* mpText;
  nuiMainMenuItem* mpCurrentTestMenu;
  nuiMainMenuItem* mpLastItem;
  uint32 mUniqueID;
  
  nuiEventSink<MainWindow> mEventSink;
};


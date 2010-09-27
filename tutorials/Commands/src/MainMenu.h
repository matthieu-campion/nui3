/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once


#include "nui.h"
#include "nuiMainMenu.h"

class MainMenu : public nuiMainMenu
{
public:
  MainMenu();
  virtual ~MainMenu();

  nuiMainMenuItem* mpUndo;
  nuiMainMenuItem* mpRedo;
  nuiMainMenuItem* mpRepeat;
  
private:

  void Init();
  
  void OnMenuCommand(const nuiEvent& rEvent);
  void OnCommandChanged(const nuiEvent& rEvent);
  
  std::map<uint32, nglString> mCommands;
  nuiEventSink<MainMenu> mEventSink;
};


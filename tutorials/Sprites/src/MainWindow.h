/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMainWindow.h"

class nuiSpriteDef;
class nuiSprite;
class nuiSpriteView;

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();
  
  void OnCreation();
  void OnClose();
  
  bool MouseMoved(const nglMouseInfo& rInfo);
protected:
  
private:
  
  bool LoadCSS(const nglPath& rPath);
  
  nuiEventSink<MainWindow> mEventSink;
  
  nuiSpriteDef* mpSpriteDef;
  nuiSpriteView* mpSpriteView;
  std::vector<nuiSprite*> mSprites;
};


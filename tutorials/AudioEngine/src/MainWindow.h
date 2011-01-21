/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMainWindow.h"

class nuiVoice;
class nuiSound;


class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();
  
  void OnCreation();
  void OnClose();
  
protected:
  
private:
  void OnButtonActivated(const nuiEvent& rEvent);
  void OnSoundHotKey(const nuiEvent& rEvent);
  void OnSynthButtonActivated(const nuiEvent& rEvent);
  void OnSynthSoundHotKey(const nuiEvent& rEvent);
  
  nuiVoice* mpLoopVoice;
  std::vector<nuiSound*> mSounds;
  std::vector<nuiButton*> mSoundButtons;
  std::vector<nuiSynthSound*> mSynthSounds;
  std::vector<nuiButton*> mSynthSoundButtons;
  
  bool LoadCSS(const nglPath& rPath);
  
  nuiEventSink<MainWindow> mEventSink;
};


/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMainWindow.h"

class nuiComboBox;

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();
  
  void OnCreation();
  void OnClose();
  
protected:
  
  float GetProgress();
  void SetProgress(float progress);
  
private:
  
  float mProgress;
  
  bool LoadCSS(const nglPath& rPath);
  
  void OnCountSliderChanged(const nuiEvent& rEvent);
  void OnGoButtonClick(const nuiEvent& rEvent);
  void OnTimerTick(const nuiEvent& rEvent);
  void OnAnimStop(const nuiEvent& rEvent);
  
  nuiSlider* mpCountSlider;
  nuiLabel* mpCountValLabel;
  
  nuiComboBox* mpLoopCombo;
  
  nuiButton* mpGoButton;
  
  nuiButton* mpButton;
  nuiEventSink<MainWindow> mEventSink;
};


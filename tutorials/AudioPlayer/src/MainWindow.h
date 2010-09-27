/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiDialogSelectFile.h"

class TimeBar;

class MainWindow : public nuiMainWindow, public nuiNotificationObserver
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();

protected:
  
  void OnButtonClick(const nuiEvent& rEvent);
  
private:
  
  bool LoadCSS(const nglPath& rPath);
  void OnBrowse(const nuiEvent& rEvent);
  void OnDialogDone(const nuiEvent& event);
  
  virtual void OnNotification(const nuiNotification& rNotif);
  
  TimeBar* mpTimeBar;
  nuiLabel* mpPlayButtonLabel;
  nuiButton* mpPlayButton;
  nglPath mBrowsedFile;
  nuiDialogSelectFile* mpDialog;
  nuiEventSink<MainWindow> mEventSink;
};


/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiDialogSelectFile.h"

class MainWindow : public nuiMainWindow, public nuiNotificationObserver
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();
  
  void OnCreation();
  void OnClose();
  
protected:
  
  bool OnBrowse(const nuiEvent& rEvent);
  bool OnDialogDone(const nuiEvent& event);
  
  virtual void OnNotification(const nuiNotification& rNotif);
  
private:
  
  bool LoadCSS(const nglPath& rPath);
  
  nuiLabel* mpMyLabel;
  nglPath mBrowsedFile;
  nuiDialogSelectFile* mpDialog;
  nuiEventSink<MainWindow> mEventSink;
};


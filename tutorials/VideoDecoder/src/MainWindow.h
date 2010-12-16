/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiDialogSelectFile.h"

class nuiVideoDecoder;


class MainWindow : public nuiMainWindow, public nuiNotificationObserver{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();

protected:
  
  void OnBrowse(const nuiEvent& rEvent);
  void OnDialogDone(const nuiEvent& event);
  virtual void OnNotification(const nuiNotification& rNotif);
 
  void OnBackBtnClicked(const nuiEvent& rEvent);
  void OnFastBackBtnClicked(const nuiEvent& rEvent);
  void OnForwardBtnClicked(const nuiEvent& rEvent);
  void OnFastForwardBtnClicked(const nuiEvent& rEvent);
  
private:
  
  bool LoadCSS(const nglPath& rPath);
  
  bool LoadVideo();
  
  void UpdateVideoName();
  void UpdateVideoInfos();
  void UpdateVideoImage();
  
  nuiLabel* mMyLabel;
  nuiEventSink<MainWindow> mEventSink;
  
  nuiVBox* mpMainVBox;
  
  nuiLabel* mpVideoNameLabel;
  nuiLabel* mpDurationLabel;
  nuiLabel* mpWidthLabel;
  nuiLabel* mpHeightLabel;
  nuiLabel* mpBitsPerPixelLabel;
  
  nuiDialogSelectFile* mpDialog;
  
  nglPath mBrowsedFile;
  nuiVideoDecoder* mpVideoDecoder;
  nglImage* mpNglImage;
  nuiImage* mpImage;
  nuiTexture* mpTexture;
  
  nuiTimer mTimer;
  void OnPlayBtnClicked(const nuiEvent& rEvent);
  void OnTimerTick(const nuiEvent& rEvent);
};


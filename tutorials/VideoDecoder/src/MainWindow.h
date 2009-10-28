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
  
  bool OnBrowse(const nuiEvent& rEvent);
  bool OnDialogDone(const nuiEvent& event);
  virtual void OnNotification(const nuiNotification& rNotif);
 
  bool OnBackBtnClicked(const nuiEvent& rEvent);
  bool OnFastBackBtnClicked(const nuiEvent& rEvent);
  bool OnForwardBtnClicked(const nuiEvent& rEvent);
  bool OnFastForwardBtnClicked(const nuiEvent& rEvent);
  
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
};


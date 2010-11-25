/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMainWindow.h"
#include "nuiNavigationController.h"
#include "nuiViewController.h"


class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();
  
  void OnCreation();
  void OnClose();
  
protected:
  
  
private:
  
  nuiNavigationController* mpNav;
  bool LoadCSS(const nglPath& rPath);  
  nuiEventSink<MainWindow> mEventSink;
};


class MyViewController : public nuiViewController
{
public:
  MyViewController(nuiNavigationController* pNav = NULL);
  virtual ~MyViewController();
  
protected:
  
  void OnPushClick(const nuiEvent& rEvent);
  void OnPopClick(const nuiEvent& rEvent);
  
  virtual void ViewWillAppear();
  virtual void ViewWillDisappear();
  virtual void ViewDidAppear();
  virtual void ViewDidDisappear();

  
private:
  bool mRunning;
  static uint32 mCount;
  static nuiNavigationController* mpNav;
  nuiEventSink<MyViewController> mEventSink;  
};


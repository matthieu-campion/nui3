/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMainWindow.h"
#include "nuiNavigationController.h"
#include "nuiViewController.h"

class nuiImageAnimation;
class nuiSprite;



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



// the inherited view controller to handle the application running
class MyViewController : public nuiViewController
{
public:
  MyViewController();
  virtual ~MyViewController();
  
protected:

  // available virtual callbacks from nuiViewController
  //
  virtual void SwipeBegan(nuiPosition swipeDirection);
  virtual void SwipeEnd(nuiPosition swipeDirection);
  //  virtual void ViewWillAppear();
  //  virtual void ViewWillDisappear();
  //  virtual void ViewDidAppear();
  //  virtual void ViewDidDisappear();

private:

  void OnResetArrows(const nuiEvent& rEvent);
  
  nuiTimer mTimer;
  std::vector<nuiSprite*> mpArrows;
  nuiEventSink<MyViewController> mEventSink;
};


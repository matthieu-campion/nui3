/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"
#include "nuiNavigationController.h"
#include "nuiNavigationBar.h"


class nuiNavigationBar;

class nuiViewController : public nuiSimpleContainer
{
  friend class nuiNavigationController;
  
public:
  nuiViewController();
  virtual ~nuiViewController();
  
  void SetTitle(const nglString& rTitle);
  
  nuiNavigationBar* GetNavigationBar();
  
protected:
  
  virtual void ViewDidLoad();
  virtual void ViewWillAppear();
  virtual void ViewWillDisappear();
  virtual void ViewDidAppear();
  virtual void ViewDidDisappear();
  
  
  virtual void ConnectTopLevel();

private:
  
  bool IsOverlayed() const;
  void SetOverlayed(bool set);
  
  bool mAnimated;
  nuiNavigationBar* mpBar;

  bool mLoaded;
  bool mClicked;
  double mSwipeTime;
  double mSwipeInitiatedTime;
  nuiSize mSwipeStartX;
  nuiSize mSwipeStartY;
  bool mSwipeInitiated;
  bool mSwipeActivated;
  nuiPosition mSwipeDirection;
  bool mOverlayed;
  
};
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
  
  virtual void ViewWillAppear();
  virtual void ViewWillDisappear();
  virtual void ViewDidAppear();
  virtual void ViewDidDisappear();
  
  virtual void SwipeBegan(nuiPosition swipeDirection);
  virtual void SwipeEnd(nuiPosition swipeDirection);
  nuiSimpleEventSource<0> EventSwipeBegan;
  nuiSimpleEventSource<0> EventSwipeEnd;
  nuiPosition GetSwipeDirection() const;
  

  
  virtual void ConnectTopLevel();
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

private:
  
  nuiPosition GetGestureDirection(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const;
  
  
  bool mAnimated;
  nuiNavigationBar* mpBar;

  bool mClicked;
  double mSwipeTime;
  double mSwipeInitiatedTime;
  nuiSize mSwipeStartX;
  nuiSize mSwipeStartY;
  bool mSwipeInitiated;
  bool mSwipeActivated;
  nuiPosition mSwipeDirection;
  
};
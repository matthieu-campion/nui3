/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"
#include "nuiNavigationController.h"


class nuiViewController : public nuiSimpleContainer
{
  friend class nuiNavigationController;
  
public:
  nuiViewController();
  virtual ~nuiViewController();
  
protected:
  
  virtual void ViewWillAppear();
  virtual void ViewWillDisappear();
  virtual void ViewDidAppear();
  virtual void ViewDidDisappear();
  virtual void ConnectTopLevel();
  
private:
  
  bool mAnimated;
  
};
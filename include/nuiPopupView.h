/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"

class nuiPopupView : public nuiSimpleContainer
{
public:
  nuiPopupView(nuiWidget* pWidget, nuiSize X, nuiSize Y);
  virtual ~nuiPopupView();
  
  void SetPos(nuiSize X, nuiSize Y);
  
  bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);

  bool KeyUp(const nglKeyEvent& rEvent);
  bool KeyDown(const nglKeyEvent& rEvent);
  
  bool SetRect(const nuiRect& rRect);
  
  bool Draw(nuiDrawContext* pContext);
  
  void ConnectTopLevel();
protected:
  nuiSize mX, mY;
  nuiRect mChildrenRect;
};


/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


#pragma once

class nuiRectView : public nuiWidget
{
public:
  nuiRectView(const nuiRect& rRect = nuiRect());
  virtual ~nuiRectView();
  
  bool Draw(nuiDrawContext* pContext);

  nuiRect CalcIdealSize();
  
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved    (nuiSize X, nuiSize Y);
  
  void SetHover(bool Hover);
  
  nuiSignal1<const nuiRect&> RectChanged;

  const nuiRect& GetCurrentRect() const;
  void SetCurrentRect(const nuiRect& rRect);
  
  void SetDisplayRectOnlyOnHover(bool set);
  bool GetDisplayRectOnlyOnHover() const;
  
private:
  nuiRect mCurrentRect;
  bool mClicked;
  nuiSize mMouseX;
  nuiSize mMouseY;
  nuiPosition mMousePosition;
  bool mDisplayRectOnlyOnHover;
};

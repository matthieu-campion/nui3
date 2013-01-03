/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiContainer.h"
#include "nuiEvent.h"
#include "nuiMouseEvent.h"

enum nuiBackgroundType
{
  eOutterBackground = 0,
  eInnerBackground
};

/// This widget implements a pane area. It draws a decorated background and can intercept mouse events
class NUI_API nuiBackgroundPane : public nuiSimpleContainer
{
public:
  nuiBackgroundPane(nuiBackgroundType type = eOutterBackground);
  virtual ~nuiBackgroundPane();
  
  nuiBackgroundType GetType() const;

  // Out going Events
  nuiMouseClicked   ClickedMouse;
  nuiMouseUnclicked UnclickedMouse;
  nuiMouseMoved     MovedMouse;

  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

  virtual void SetInterceptMouse(bool intercept=false); ///< If intercept==true then all the mouse event will be intercepted by the UserArea. By default no event is intercepted.
  virtual bool GetInterceptMouse(); ///< Return the mouse event interception state.

protected:
  bool mInterceptMouse;
  
private:
  nuiBackgroundType mType;
};



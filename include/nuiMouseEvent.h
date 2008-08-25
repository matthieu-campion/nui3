/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiMouseEvent_h__
#define __nuiMouseEvent_h__

#include "nuiEvent.h"
#include "nuiEventRegistry.h"
#include "nglWindow.h"
#include "nuiFlags.h"
#include "nuiRect.h"

//////////// EVENTS:
/// This is a helper class to easily dispatch mouse clicked events
template<nuiEventId eventId>  class NUI_API nuiMouseEvent: public nuiEvent
{
public:
  nuiMouseEvent(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button = 0)
    : nuiEvent(eventId)
  {
    mX = X;
    mY = Y;
    mButton = Button;
  }

  nuiSize mX;
  nuiSize mY;
  nglMouseInfo::Flags mButton;
};

typedef nuiMouseEvent<nuiClicked> nuiMouseClickedEvent;
typedef nuiMouseEvent<nuiUnclicked> nuiMouseUnClickedEvent;
typedef nuiMouseEvent<nuiMoved> nuiMouseMovedEvent;


//////// EVENT SOURCES:
/// This is a helper class to easily dispatch mouse clicked events
template<nuiEventId eventId> class NUI_API nuiMouseEventSource : public nuiEventSource
{
public:
  nuiMouseEventSource()
  {
  }

  virtual bool operator() (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button = 0)
  {
    return SendEvent(nuiMouseEvent<eventId>(X, Y, Button));
  }
};

typedef nuiMouseEventSource<nuiClicked> nuiMouseClicked;
typedef nuiMouseEventSource<nuiUnclicked> nuiMouseUnclicked;
typedef nuiMouseEventSource<nuiMoved> nuiMouseMoved;

#endif // __nuiMouseEvent_h__

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
  nuiMouseEvent(const nglMouseInfo& rInfo)
    : nuiEvent(eventId), mInfo(rInfo), mX((nuiSize)rInfo.X), mY((nuiSize)rInfo.Y), mButton(rInfo.Buttons)
  {
  }

  nuiSize mX;
  nuiSize mY;
  nglMouseInfo::Flags mButton;
  nglMouseInfo mInfo;
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

  virtual bool operator() (const nglMouseInfo& rInfo)
  {
    nuiMouseEvent<eventId> e(rInfo);
    e.AddArgument(e.mX);
    e.AddArgument(e.mY);
    e.AddArgument((uint32)e.mButton);
    e.AddArgument(rInfo.TouchId);
    return SendEvent(e);
  }

  virtual bool operator() (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button = 0)
  {
    nglMouseInfo info;
    info.X = ToZero(X);
    info.Y = ToZero(Y);
    info.Buttons = Button;
    
    return (*this)(info);
  }
};

typedef nuiMouseEventSource<nuiClicked> nuiMouseClicked;
typedef nuiMouseEventSource<nuiUnclicked> nuiMouseUnclicked;
typedef nuiMouseEventSource<nuiMoved> nuiMouseMoved;

#endif // __nuiMouseEvent_h__

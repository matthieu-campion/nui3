/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiUserArea_h__
#define __nuiUserArea_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiEvent.h"
#include "nuiMouseEvent.h"

class NUI_API nuiDrawEvent : public nuiEvent
{
public:
  nuiDrawEvent(nuiDrawContext* pContext)
  : nuiEvent(nuiUserDraw)
  {
    mpContext = pContext;
  }
  
  nuiDrawEvent(const nuiEvent& rEvent)
  : nuiEvent(rEvent)
  {
    NGL_ASSERT(dynamic_cast<const nuiDrawEvent*> (&rEvent));
    mpContext = (*(const nuiDrawEvent*)(&rEvent)).mpContext;
  }
  
  nuiDrawContext* mpContext;
};

//////// EVENT SOURCES:
/// This is a helper class to easily dispatch draw events
class NUI_API nuiDrawEventSource : public nuiEventSource
{
public:
  nuiDrawEventSource()
  {
  }

  virtual bool operator() (nuiDrawContext* pContext)
  {
    return SendEvent(nuiDrawEvent(pContext));
  }
};



/// This widget implements a user area. It draws basicaly nothing by itself and fires a UserDraw event instead. It up to the programmer to catch this event and fill the area with whatever opengl code he wants. 
class NUI_API nuiUserArea : public nuiWidget
{
public:
  nuiUserArea();
  virtual ~nuiUserArea();

  virtual bool Draw(nuiDrawContext* pContext);

  // Out going Events
  nuiDrawEventSource UserDraw;

  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

  virtual void SetInterceptMouse(bool intercept=false); ///< If intercept==true then all the mouse event will be intercepted by the UserArea. By default no event is intercepted.
  virtual bool GetInterceptMouse(); ///< Return the mouse event intercption state.

  void EnableGLInit(bool Value); ///< When set to true some OpenGL rendering context inits  will take place before the UserDraw event is dispatched.
  bool IsGLInitEnabled(); ///< See EnableGLInit.

protected:
  bool mInterceptMouse;
  bool mPrepareGLContext;
};

#endif // __nuiUserArea_h__

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiAnimView_h__
#define __nuiAnimView_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiTimer.h"
#include "nuiAnimation.h"

/// nuiAnimView is a widget container that keep all its children at their original position and size.
class NUI_API nuiAnimView : public nuiSimpleContainer, public nuiAnimation, virtual public nuiRefCount
{
public:
  nuiAnimView ();
  virtual ~nuiAnimView();

  virtual bool Draw(nuiDrawContext* pContext);

  // Overloaded from nuiAnimation:
  void OnFrame();


protected:
};

#endif // __nuiAnimation_h__

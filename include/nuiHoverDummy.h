/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiHoverDummy_h__
#define __nuiHoverDummy_h__

#include "nuiContainer.h"

class NUI_API nuiHoverDummy : public nuiSimpleContainer
{
public:
  nuiHoverDummy(nuiWidgetPtr pHoverOn = NULL, nuiWidgetPtr pHoverOff = NULL);
  virtual ~nuiHoverDummy();

  void SetDummy(bool Hover, nuiWidgetPtr pWidget); ///< Set the dummy widget to display when the widget is in a particular Hover. 
  nuiWidgetPtr GetDummy(bool Hover);  ///< Get the dummy widget to display when the widget is in a particular Hover. 

  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  void SetHover(bool Hover);

protected:
  nuiWidgetPtr mpHover[2];
};

#endif // __nuiHoverDummy_h__

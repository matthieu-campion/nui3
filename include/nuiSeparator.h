/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSeparator_h__
#define __nuiSeparator_h__

#include "nui.h"

class NUI_API nuiSeparator : public nuiWidget
{
public:
  nuiSeparator(nuiOrientation orientation);
  virtual ~nuiSeparator();

  virtual nuiRect CalcIdealSize();

  virtual bool Draw(nuiDrawContext* pContext);

private:
  nuiOrientation mOrientation;
};

#endif // __nuiSeparator_h__

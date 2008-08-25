/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiShapeView_h__
#define __nuiShapeView_h__

#include "nuiWidget.h"
#include "nuiShape.h"

// nuishapeView.h

class nuiShape;

class NUI_API nuiShapeView : public nuiWidget
{
public:
  nuiShapeView(nuiShape* pShape);
  virtual ~nuiShapeView();

  nuiRect CalcIdealSize();
  bool Draw(nuiDrawContext* pContext);

  void SetShapeMode(nuiShapeMode ShapeMode);
  nuiShapeMode GetShapeMode() const;
  void EnableAntialiasing(bool Set);
  bool ISAntialiasingEnabled() const;
  void SetLineWidth(nuiSize Width);
  nuiSize GetLineWidth() const;

protected:
  nuiShape* mpShape;
  nuiShapeMode mShapeMode;
  bool mAntialiasing;
  nuiSize mLineWidth;
};

#endif // __nuiShapeView_h__


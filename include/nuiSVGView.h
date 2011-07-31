/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSVGView_h__
#define __nuiSVGView_h__

//#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiSVGShape.h"
#include "nuiMetaPainter.h"

class NUI_API nuiSVGView : public nuiWidget
{
public:
  nuiSVGView(nuiSVGShape* pShape = NULL); ///< Load a svg view from an existing svg shape.
  nuiSVGView(const nglPath& rShapePath); ///< Construct a svg view from a SVG xml description file.
  virtual ~nuiSVGView();

  bool Load (nglIStream& rSource);

  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();

  void EnableAntialias(bool Antialias);
  bool IsAntialiasEnabled();

private:
  nuiSVGShape* mpShape;
  bool mAntialias;
  nuiMetaPainter mCache;

  void Init();
};

#endif // __nuiSVGView_h__

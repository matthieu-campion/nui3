/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiOffscreenView_h__
#define __nuiOffscreenView_h__

//#include "nui.h"
#include "nuiContainer.h"

class nuiGLPainter;

class NUI_API nuiOffscreenView: public nuiSimpleContainer
{
public:
  nuiOffscreenView(nuiContainerPtr pParent);
  virtual ~nuiOffscreenView();

  nuiXMLNode* Serialize(nuiXMLNode* pParentNode) const;

  bool Draw(nuiDrawContext* pContext);

  bool SetRect(const nuiRect& rRect);
private:
  nuiGLPainter* mpContext;
  nuiTexture* mpTexture;
  bool mRedrawChildren; ///< Set to true when the layout of this container is changed
};

#endif //#ifndef __nuiOffscreenView_h__

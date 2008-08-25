/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiShaderView_h__
#define __nuiShaderView_h__

//#include "nui.h"
#include "nuiContainer.h"

class nuiShader;

class NUI_API nuiShaderView : public nuiSimpleContainer
{
public:
  nuiShaderView(nuiShader* pShader);
  bool Load(const nuiXMLNode* pNode);
  virtual ~nuiShaderView();

  bool Draw(nuiDrawContext* pContext);

private:
  nuiShader* mpShader;
};


#endif

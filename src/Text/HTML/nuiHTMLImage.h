/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTMLItem.h"

class nuiHTMLImage : public nuiHTMLItem
{
public:
  nuiHTMLImage(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor);
  virtual ~nuiHTMLImage();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
private:
  nuiTexture* mpTexture;
  
  float mWidth;
  float mHeight;
};



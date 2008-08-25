/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nuiTexture.h"
#include "nuiDrawContext.h"

#ifndef __nuiTextureHelpers_h__
#define __nuiTextureHelpers_h__

// Define a set of texture drawing helpers:
void nuiRadialBlur(nuiDrawContext* pContext, nuiTexture* pTexture, const nuiRect& rDestinationBase, float ScaleCoef, const nuiVector2& Direction, uint Iterations, float Strength = 0.8f, float PreMultiplyAlpha = 1.0f);

#endif // __nuiTextureHelpers_h__

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#ifdef HAVE_FREETYPE

#include "nglFontCache.h"
#include "nglFont.h"


nglFontCache::nglFontCache()
{
  mpFont = NULL;
  mScaleX = 1.f;
  mScaleY = 1.f;
}

nglFontCache::~nglFontCache()
{
}

void nglFontCache::SetScale (float ScaleX, float ScaleY)
{
  mScaleX = ScaleX;
  mScaleY = ScaleY;
}

void nglFontCache::PushContext()
{
}

void nglFontCache::PopContext()
{
}

void nglFontCache::SetContext()
{
}

#endif // HAVE_FREETYPE

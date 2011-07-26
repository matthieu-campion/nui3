/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "DefaultDecoration/AtlasTexture.h" 

void nuiInitDefaultDecorationAtlas()
{
  nglIMemory* pIMem = new nglIMemory(gpAtlasTexture, gAtlasTextureSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  pTex->SetSource("DefaultDecorationAtlas");

#include "DefaultDecoration/Atlas.h"

  delete pIMem;
}

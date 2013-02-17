/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

void nglClipBoard::Clear()
{
}

void nglClipBoard::SetData(const nglMimeSource &src)
{
  Clear();
}

bool nglClipBoard::IsAvailable(const nglMimeSource::nglMimeDataType &mimeType)
{
  return false;
}

void nglClipBoard::GetData(nglMimeSource &dst)
{
}


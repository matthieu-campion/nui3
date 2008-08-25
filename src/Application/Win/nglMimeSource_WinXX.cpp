/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglLog.h"

#include "nglMimeSource.h"

nglMimeSource::nglMimeDataType nglMimeSource::Register(const nglString &mimeType)
{
  UINT internalMimeType = RegisterClipboardFormat(mimeType.GetChars());
  if (internalMimeType == 0)
  {
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't register mime type"));
  }
  return internalMimeType;
}

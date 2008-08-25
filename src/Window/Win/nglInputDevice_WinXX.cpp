/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglInputDevice.h"
#include "nglInputDevice_DirectX.h"


nglInputDevice* nglInputDevice::Enum (uint Index)
{
#ifdef _WIN32_
#ifdef NGL_USE_DIRECTINPUT
  return nglInputDeviceDirectX::Enum(Index);
#else
    return NULL;
#endif
#else
  return NULL;
#endif
}

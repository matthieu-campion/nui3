/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglVideoMode.h"
#include "nglKernel.h"


nglVideoMode::nglVideoMode()
{
  // Get current video mode
  mMode.dmSize = sizeof(DEVMODE);
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mMode);
  
  mScaleFactor = 1.0f;
  mInvScaleFactor = 1.0f / mScaleFactor;
}

nglVideoMode::nglVideoMode (const nglVideoMode& rMode)
{
  mMode = rMode.mMode;
}

nglVideoMode::~nglVideoMode()
{
}

bool nglVideoMode::Enum(nglVideoMode& rMode, uint Index)
{
  DEVMODE mode;
  mode.dmSize = sizeof(DEVMODE);

  if (EnumDisplaySettings(NULL, Index, &mode))
  {
    rMode.mMode  = mode;
    return true;
  }

  return false;
}


bool nglVideoMode::SetMode() const
{
  DEVMODE mode = mMode;
  bool done;

  mode.dmFields = DM_BITSPERPEL |  DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
  done = ChangeDisplaySettings(&mode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
  NGL_DEBUG( NGL_LOG(_T("vidmode"), NGL_LOG_INFO, _T("switching to %s: %s"), Dump().GetChars(), done ? _T("ok"):_T("failed")); )
  return done;
}

uint nglVideoMode::GetWidth() const
{
  return mMode.dmPelsWidth;
}

uint nglVideoMode::GetHeight() const
{
  return mMode.dmPelsHeight;
}

uint nglVideoMode::GetDepth() const
{
  return mMode.dmBitsPerPel;
}

uint nglVideoMode::GetFrequency() const
{
  return mMode.dmDisplayFrequency;
}

static float gScaleFactor = 1.0f;
static float gInvScaleFactor = 1.0f;

float nuiGetScaleFactor()
{
  return gScaleFactor;
}

float nuiGetInvScaleFactor()
{
  return gInvScaleFactor;
}




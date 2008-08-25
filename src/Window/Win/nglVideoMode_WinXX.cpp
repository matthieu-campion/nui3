/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nglVideoMode.h"
#include "nglKernel.h"


nglVideoMode::nglVideoMode()
{
  // Get current video mode
  mMode.dmSize = sizeof(DEVMODE);
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mMode);
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

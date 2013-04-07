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

#include <Cocoa/Cocoa.h>

nglVideoMode::nglVideoMode()
{
  NSRect rect = [[NSScreen mainScreen] frame];

  mWidth = (uint)rect.size.width;
  mHeight = (uint)rect.size.height;

  mBPP = 32;
  
  Init();
}

nglVideoMode::nglVideoMode (const nglVideoMode& rMode)
{
  mWidth = rMode.mWidth;
  mHeight = rMode.mHeight;
  mRate = rMode.mRate;
  mBPP = rMode.mBPP;

}

nglVideoMode::~nglVideoMode()
{
}

void nglVideoMode::Init()
{
}

bool nglVideoMode::Enum(nglVideoMode& Mode, uint Index)
{
  return true;
}

uint nglVideoMode::GetWidth() const
{
  return mWidth;
}

uint nglVideoMode::GetHeight() const
{
  return mHeight;
}

uint nglVideoMode::GetDepth() const
{
  return mBPP;
}

uint nglVideoMode::GetFrequency() const
{
  return mRate; // Unknown
}


bool nglVideoMode::SetMode (nglVideoMode* pVideoMode, bool Lock)
{
  bool done = false;

  boolean_t exactMatch;
  CFDictionaryRef mode = CGDisplayBestModeForParametersAndRefreshRate((CGDirectDisplayID)pVideoMode->mDisplay, pVideoMode->mBPP, pVideoMode->mWidth, pVideoMode->mHeight, pVideoMode->mRate, &exactMatch);
  CGDisplaySwitchToMode( (CGDirectDisplayID)pVideoMode->mDisplay, mode);
  NGL_DEBUG( NGL_LOG(_T("vidmode"), NGL_LOG_INFO, _T("switching to %s: %s"), pVideoMode->Dump().GetChars(), done ? _T("ok"):_T("failed")); )
  return done;
}


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
#include NGL_CONFIG_H
#include "nglVideoMode.h"
#include "nglKernel.h"


nglVideoMode::nglVideoMode()
{
}

nglVideoMode::nglVideoMode (const nglVideoMode& rMode)
{
}

nglVideoMode::~nglVideoMode()
{
}


bool nglVideoMode::Enum (nglVideoMode& Mode, uint Index)
{
  return false;
}

uint nglVideoMode::GetWidth() const
{
  return 0;
}

uint nglVideoMode::GetHeight() const
{
  return 0;
}

uint nglVideoMode::GetDepth() const
{
  return 0;
}

uint nglVideoMode::GetFrequency() const
{
  return 0; // Unknown
}

float nuiGetScaleFactor() { return 1.0;}
float nuiGetInvScaleFactor() { return nuiGetScaleFactor()/1.0;}


/*
 NUI - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 SÈbastien MÈtrot
 
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

///#FIXME

nglStringConv::nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default)
{
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mState = eStringConv_OK;
}

nglStringConv::nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default, bool NoInit)
{
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mState = eStringConv_OK;
}

nglStringConv::~nglStringConv()
{
}

int32 nglStringConv::Process(const char*& pSource, int32& rToRead, char*& pTarget, int32& rToWrite)
{  
  int done = 0;

  while (rToRead > 0 && rToWrite > 0)
  {
    *pTarget++ = *pSource++;
    rToRead--;
    rToWrite--;
    done++;
  }

  return done;
}


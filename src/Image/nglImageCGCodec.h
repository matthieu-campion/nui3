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

#ifndef __nglImageCGCodec_h__
#define __nglImageCGCodec_h__

#include "nglImageCodec.h"

//! Functions prototypes
size_t	CGGetBytesCallback(void* pInfo, void* pBuffer, size_t Count);
void		CGReleaseInfoCallback(void* pInfo);
void		CGRewindCallback(void* pInfo);
off_t		CGSkipForwardBytesCallback(void* pInfo, off_t Count);

class nglImageCGCodecInfo : public nglImageCodecInfo
{
public:
  nglImageCGCodecInfo()
  {
    mCanSave = false; ///< FIXME
    mCanLoad = true;

    mName = _T("CG");

    mExtensions.push_back(nglString(_T(".png")));
    mExtensions.push_back(nglString(_T(".jpeg")));
    mExtensions.push_back(nglString(_T(".jpg")));
    mExtensions.push_back(nglString(_T(".jpe")));
    mExtensions.push_back(nglString(_T(".jif")));
    
    mInfo = _T("CG image codec, using Core Graphics Framework from Apple");
  }

  virtual ~nglImageCGCodecInfo()
  {
  }

  virtual nglImageCodec* CreateInstance();
};

#endif // __nglImageCGCodec_h__

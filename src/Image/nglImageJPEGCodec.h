/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nglImageJPEGCodec_h__
#define __nglImageJPEGCodec_h__

//#include "nui.h"
#include "nglImageCodec.h"

class nglImageJPEGCodecInfo : public nglImageCodecInfo
{
public:
  nglImageJPEGCodecInfo()
  {
    mCanSave = true;
    mCanLoad = true;
    mName = _T("JPEG");
    mExtensions.push_back(nglString(_T(".jpeg")));
    mExtensions.push_back(nglString(_T(".jpg")));
    mExtensions.push_back(nglString(_T(".jpe")));
    mExtensions.push_back(nglString(_T(".jif")));
    mInfo = _T("JPEG image codec, using libjpeg from www.ijg.org");
  }

  virtual ~nglImageJPEGCodecInfo()
  {
  }

  virtual nglImageCodec* CreateInstance();
};

#endif // __nglImageJPEGCodec_h__

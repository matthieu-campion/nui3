/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nglImagePNGCodec_h__
#define __nglImagePNGCodec_h__

//#include "nui.h"
#include "nglImageCodec.h"

class nglImagePNGCodecInfo : public nglImageCodecInfo
{
public:
  nglImagePNGCodecInfo()
  {
    mCanSave = true;
    mCanLoad = true;
    mName = _T("PNG");
    mExtensions.push_back(nglString(_T(".png")));
    mInfo = _T("PNG image codec, using libpng");
  };

  virtual ~nglImagePNGCodecInfo()
  {
  };

  virtual nglImageCodec* CreateInstance();
};

#endif // __nglImagePNGCodec_h__

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nglImageGIFCodec_h__
#define __nglImageGIFCodec_h__

//#include "nui.h"
#include "nglImageCodec.h"


class nglImageGIFCodec : public nglImageCodec
{
public:
  nglImageGIFCodec(void);
  virtual ~nglImageGIFCodec(void);

  virtual bool Init(nglImage* pImage);

  virtual bool Probe(nglIStream* pIStream);
  virtual bool Feed(nglIStream* pIStream);
  virtual bool Save(nglOStream* pOStream);
  virtual float GetCompletion();

protected:
};

class nglImageGIFCodecInfo : public nglImageCodecInfo
{
public:
  nglImageGIFCodecInfo(): nglImageCodecInfo()
  {
    mCanSave = false;
    mCanLoad = true;
    mName = _T("Simple GIF loader");
    mExtensions.push_back(_T(".gif"));
    mInfo = _T("This codec is based on libungif");
  }

  virtual nglImageCodec* CreateInstance()
  {
    return new nglImageGIFCodec();
  }
};

#endif // __nglImageGIFCodec_h__

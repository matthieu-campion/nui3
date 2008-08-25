/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nglImagePPMCodec_h__
#define __nglImagePPMCodec_h__

//#include "nui.h"
#include "nglImageCodec.h"

struct ppmHeader
{
  nglString MagicWord;
  nglString Comments;
  uint16    Width;
  uint16    Height;
  uint16    MaxColor;
};


class nglImagePPMCodec :
  public nglImageCodec
{
public:
  nglImagePPMCodec(void);
  virtual ~nglImagePPMCodec(void);

  virtual bool Init(nglImage* pImage);

  virtual bool Probe(nglIStream* pIStream);
  virtual bool Feed(nglIStream* pIStream);
  virtual bool Save(nglOStream* pOStream);
  virtual float GetCompletion();

protected:
  bool ReadHeader(nglIStream* pIStream);
  enum ppmState
  {
    ppmError = 0,
    ppmReadHeader,
    ppmReadData
  };



  uint mLine;
  uint mLineSize;
  ppmState mState;
  ppmHeader mHeader;
  char* mpImageID;
  char* mpBuffer;
};

class nglImagePPMCodecInfo : public nglImageCodecInfo
{
public:
  nglImagePPMCodecInfo():
      nglImageCodecInfo()
  {
    mCanSave = true;
    mCanLoad = true;
    mName = _T("PPM");
    mExtensions.push_back(_T(".ppm"));
    mInfo = _T("Simple PPM codec");
  }

  virtual nglImageCodec* CreateInstance()
  {
    return new nglImagePPMCodec();
  }
};

#endif // __nglImagePPMCodec_h__

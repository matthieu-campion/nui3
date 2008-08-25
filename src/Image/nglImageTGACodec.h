/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nglImageTGACodec_h__
#define __nglImageTGACodec_h__

//#include "nui.h"
#include "nglImageCodec.h"


struct TargaHeader
{
  uint8 IDFieldSize;
  uint8 ColorMapType;
  uint8 ImageTypeCode;
  // Color map specs:
  uint16 ColorMapOrigin;
  uint16 ColorMapLength;
  uint8  ColorMapEntrySize;
  // Image Specification:
  uint16 PosX;
  uint16 PosY;
  uint16 Width;
  uint16 Height;
  uint8 ImagePixelSize;
  uint8 ImageDescriptorByte;
};


class nglImageTGACodec : public nglImageCodec
{
public:
  nglImageTGACodec(void);
  virtual ~nglImageTGACodec(void);

  virtual bool Init(nglImage* pImage);

  virtual bool Probe(nglIStream* pIStream);
  virtual bool Feed(nglIStream* pIStream);
  virtual bool Save(nglOStream* pOStream);
  virtual float GetCompletion();

protected:
  bool ReadHeader(nglIStream* pIStream,bool Peek);

  enum tgaState
  {
    tgaError = 0,
    tgaHeader,
    tgaID,
    tgaData
  };

  uint mLine;
  uint mLineSize;
  TargaHeader mHeader;
  tgaState mState;
  char* mpImageID;
};

class nglImageTGACodecInfo : public nglImageCodecInfo
{
public:
  nglImageTGACodecInfo(): nglImageCodecInfo()
  {
    mCanSave = false;
    mCanLoad = true;
    mName = _T("Simple TGA loader");
    mExtensions.push_back(_T(".tga"));
    mInfo = _T("NGL image codec sample code");
  }

  virtual nglImageCodec* CreateInstance()
  {
    return new nglImageTGACodec();
  }
};

#endif // __nglImageTGACodec_h__

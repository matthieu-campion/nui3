/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglIStream.h"



class nuiNativeResource : public nglIStream
{
public:
  nuiNativeResource(const nglPath& rPath);
  ~nuiNativeResource();

  bool IsValid() const;
  static bool GetResourcesList(std::vector<nglPath>& rResources);

///< Wrappers to our internal stream (nglIMemory on Window(s), nglIFile otherwise)
  nglStreamState GetState() const;
  nglFileOffset GetPos() const;
  nglFileOffset SetPos (nglFileOffset Where, nglStreamWhence Whence = eStreamFromStart);
  nglFileSize Available (uint WordSize = 1);
  int64 ReadUInt8  (uint8*  pData, int64 Count = 1);
  int64 ReadUInt16 (uint16* pData, int64 Count = 1);
  int64 ReadUInt32 (uint32* pData, int64 Count = 1);
  int64 ReadUInt64 (uint64* pData, int64 Count = 1);
  int64 ReadInt8   (int8*  pData, int64 Count = 1);
  int64 ReadInt16  (int16* pData, int64 Count = 1);
  int64 ReadInt32  (int32* pData, int64 Count = 1);
  int64 ReadInt64  (int64* pData, int64 Count = 1);
  int64 ReadFloat  (float*  pData, int64 Count = 1);
  int64 ReadDouble (double* pData, int64 Count = 1);
  int64 ReadLine   (nglString& rLine, nglTextFormat* pFormat = NULL);

  int64 Read (void* pData, int64 WordCount, uint WordSize);
  int64 Peek (void* pData, int64 WordCount, uint WordSize);



  virtual void SetEndian(nglEndian nglEndian);
  virtual bool SetTextEncoding (nglTextEncoding Encoding);
  virtual void SetTextFormat (nglTextFormat Format);


private:
  nglIStream* mpIStream;
  bool mValid;
};

#if (defined _LINUX_) || (defined _CARBON_) || (defined _UIKIT_) || (defined _COCOA_) || (defined _ANDROID_) || (defined _MINUI3_)
nglPath nuiGetNativeResourcePath();
#endif


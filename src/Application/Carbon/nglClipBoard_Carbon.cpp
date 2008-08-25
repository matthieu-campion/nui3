/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglLog.h"
#include "nglClipBoard.h"
#include <Carbon/Carbon.h>

void nglClipBoard::Clear()
{
  OSStatus err;
  err = ::ClearCurrentScrap();
  NGL_ASSERT(!err);
}

void nglClipBoard::SetData(const nglMimeSource &src)
{
  Clear();
  ScrapRef scrapRef;
  OSStatus err = GetCurrentScrap(&scrapRef);
  NGL_ASSERT(!err);

  err = ::PutScrapFlavor( scrapRef, (ScrapFlavorType) src.GetMimeType(), 0, (Size)src.GetDataSize(), (void*)src.GetData());
  NGL_ASSERT(!err);
}

bool nglClipBoard::IsAvailable(const nglMimeSource::nglMimeDataType &mimeType)
{
  OSStatus err;
  ScrapRef scrapRef;
  err = ::GetCurrentScrap(&scrapRef);
  NGL_ASSERT(!err);
  
  UInt32 infoCount;
  err = ::GetScrapFlavorCount(scrapRef, &infoCount);
  NGL_ASSERT(!err);
  
  if (infoCount > 0)
  {
    ScrapFlavorFlags flavorFlags;
    err = ::GetScrapFlavorFlags(scrapRef, (ScrapFlavorType)mimeType, &flavorFlags);
    if (err == noErr)
      return true;
  }
  return false;
}

void nglClipBoard::GetData(nglMimeSource &dst)
{
  NGL_ASSERT(IsAvailable(dst.GetMimeType()));
  OSStatus err;
  ScrapRef scrapRef;
  err = ::GetCurrentScrap(&scrapRef);
  NGL_ASSERT(err == noErr);
  
  Size byteCount;
  err = ::GetScrapFlavorSize(scrapRef, dst.GetMimeType(), &byteCount);
  NGL_ASSERT(err == noErr);
  
  uint8* pData = new uint8[byteCount+1];
  err = ::GetScrapFlavorData(scrapRef, dst.GetMimeType(), &byteCount, pData);
  NGL_ASSERT(err == noErr);
  dst.SetData(pData, byteCount);
  delete pData;
}


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

#include "nglImage.h"
#include "nglImageCGCodec.h"

#include "nglIStream.h"
#include "nglOStream.h"

#include <CoreGraphics/CoreGraphics.h>

class nglImageCGCodec : public nglImageCodec
{
public:
  nglImageCGCodec();
  virtual ~nglImageCGCodec();

  virtual bool Init(nglImage* pImage);
  virtual bool Probe(nglIStream* pIStream);
  virtual bool Feed(nglIStream* pIStream); 
  virtual bool Save(nglOStream* pOStream); 
  virtual float GetCompletion(); 

private:
  CGImageRef ReadInfo(nglIStream* pIStream);
  bool ProbePNG(nglIStream* pIStream);
  bool ProbeJPEG(nglIStream* pIStream);

  nglIStream* mpIStream;

private:
  CGImageRef mpCGImage;
//  CGContextRef mpCGContext;
  CGDataProviderRef mpCGProvider;

///< CGDataProviderGetBytesCallback
  friend size_t CGGetBytesCallback(void* pInfo, void* pBuffer, size_t Count);
///< CGDataProviderReleaseInfoCallback
  friend void CGReleaseInfoCallback(void* pInfo);
///< CGDataProviderRewindCallback
  friend void CGRewindCallback(void* pInfo);
///< CGDataProviderSkipForwardCallback
  friend off_t CGSkipForwardBytesCallback(void* pInfo, off_t Count);

  CGDataProviderSequentialCallbacks mCGCallbacks;
};

nglImageCodec* nglImageCGCodecInfo::CreateInstance()
{
  return new nglImageCGCodec();
}

nglImageCGCodec::nglImageCGCodec()
{
//  mpCGContext = NULL;
  mpCGImage = NULL;
  mpIStream = NULL;

  mCGCallbacks.version = 0;
  mCGCallbacks.getBytes    = &CGGetBytesCallback;
  mCGCallbacks.skipForward = &CGSkipForwardBytesCallback;
  mCGCallbacks.rewind      = &CGRewindCallback;
  mCGCallbacks.releaseInfo = &CGReleaseInfoCallback;

///< init our data provider (wrapper for the stream)
  mpCGProvider = CGDataProviderCreateSequential(this, &mCGCallbacks);
  NGL_ASSERT(mpCGProvider);
}

nglImageCGCodec::~nglImageCGCodec()
{
//  if (mpCGContext)
//    CGContextRelease(mpCGContext);
  if (mpCGImage)
    CGImageRelease(mpCGImage);
  if (mpCGProvider)
    CGDataProviderRelease(mpCGProvider);
}

bool nglImageCGCodec::Probe(nglIStream* pIStream)
{
  if (ProbePNG(pIStream)) {
    return true;
  }
  else if (ProbeJPEG(pIStream)) {
    return true;
  }
  return false;
}

bool nglImageCGCodec::ProbePNG(nglIStream* pIStream)
{
  if (pIStream->Available(4)) /// from the PNG codec
  {
    char buffer[4];
    if (!pIStream->Peek(buffer,1,4))
      return false;
    if (buffer[1]=='P' && 
        buffer[2]=='N' && 
        buffer[3]=='G')
    {
      return true;
    }
  }
  return false;
}

bool nglImageCGCodec::ProbeJPEG(nglIStream* pIStream)
{
  if (pIStream->Available(10)) /// from the JPEG codec
  {
    char buffer[10];
    if (!pIStream->Peek(buffer,1,10))
      return false;
    if (buffer[6]=='J' && 
        buffer[7]=='F' && 
        buffer[8]=='I' && 
        buffer[9]=='F')
    {
      return true;
    }
  }
  return false;
}

CGImageRef nglImageCGCodec::ReadInfo(nglIStream* pIStream)
{
  mpIStream = pIStream;
  CGImageRef pCGImage = NULL;

  const bool shouldInterpolate = true;
  if (ProbePNG(pIStream))
  {
    pCGImage = CGImageCreateWithPNGDataProvider(mpCGProvider, NULL, shouldInterpolate, kCGRenderingIntentDefault);
  }
  else if (ProbeJPEG(pIStream))
  {
    pCGImage = CGImageCreateWithJPEGDataProvider(mpCGProvider, NULL, false, kCGRenderingIntentDefault);
  }
  if (pCGImage)
  {
    nglImageInfo info;
    info.mWidth = CGImageGetWidth(pCGImage);
    info.mHeight = CGImageGetHeight(pCGImage);
    if (info.mWidth > 0 && info.mHeight > 0)
    {
      info.mBufferFormat = eImageFormatRaw;
      info.mPixelFormat = eImagePixelRGBA;
      info.mBytesPerPixel = 4;
      info.mBitDepth = 8 * info.mBytesPerPixel;
      info.mBytesPerLine = info.mWidth * info.mBytesPerPixel;
      info.mpBuffer = NULL;

      SendInfo(info);

      return pCGImage;
    }
    CGImageRelease(pCGImage);
  }
  return NULL;
}

bool nglImageCGCodec::Init(nglImage* pImage)
{
  mpImage = pImage;
  return true;
}

bool nglImageCGCodec::Feed(nglIStream* pIStream)
{
  if (!mpCGImage)
  {
    mpCGImage = ReadInfo(pIStream); ///< shall allocate the buffer
  }
  if (!mpCGImage)
    return false;

  mpIStream = pIStream;
// Use the generic RGB color space.
  CGColorSpaceRef pCGColors = CGColorSpaceCreateDeviceRGB();
  if (pCGColors == NULL)
  {
    NGL_OUT(_T("nglImageCGCodec::Feed Error allocating color space\n"));
    return false;
  }

  nglImageInfo info;
  mpImage->GetInfo(info);
  NGL_ASSERT(info.mpBuffer);

// Create the bitmap context. 
// The image will be converted to the format specified here by CGBitmapContextCreate.
  CGContextRef pCGContext =
    CGBitmapContextCreate(info.mpBuffer, info.mWidth, info.mHeight,
                          info.mBitDepth/4, info.mBytesPerLine,
                          pCGColors, kCGImageAlphaPremultipliedLast);//kCGImageAlphaPremultipliedFirst);

  CGColorSpaceRelease(pCGColors);
  CGRect rect = { {0,0}, {info.mWidth, info.mHeight} };
  CGContextDrawImage(pCGContext, rect, mpCGImage);
  CGContextRelease(pCGContext);

  return (pIStream->GetState()==eStreamWait) || (pIStream->GetState()==eStreamReady);;
}

bool nglImageCGCodec::Save(nglOStream* pOStream)
{
  return false;
}

float nglImageCGCodec::GetCompletion()
{
  return 0;
}

size_t CGGetBytesCallback(void* pInfo, void* pBuffer, size_t Count)
{
  nglImageCGCodec* pCodec = (nglImageCGCodec*)pInfo;
  NGL_ASSERT(pCodec->mpIStream);
  Count = MIN(pCodec->mpIStream->Available(), Count);

  return pCodec->mpIStream->Read(pBuffer, Count, 1);
}

void CGReleaseInfoCallback(void* pInfo)
{
  nglImageCGCodec* pCodec = (nglImageCGCodec*)pInfo;
  NGL_ASSERT(pCodec->mpIStream);
}

void CGRewindCallback(void* pInfo)
{
  nglImageCGCodec* pCodec = (nglImageCGCodec*)pInfo;
  NGL_ASSERT(pCodec->mpIStream);

///< should we keep the initial offset to rewind to here?
  pCodec->mpIStream->SetPos(0, eStreamFromStart);
}

off_t CGSkipForwardBytesCallback(void* pInfo, off_t Count)
{
  nglImageCGCodec* pCodec = (nglImageCGCodec*)pInfo;
  NGL_ASSERT(pCodec->mpIStream);

  Count = MIN(pCodec->mpIStream->Available(), Count);
  pCodec->mpIStream->SetPos(Count, eStreamForward);

  return Count;
}

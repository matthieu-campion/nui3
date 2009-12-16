/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

#include "nuiVideoDecoder.h"

class nuiVideoDecoderPrivate
{
public:
  
  QTMovie* mpMovie;
};

nuiVideoDecoder::nuiVideoDecoder(const nglPath& path)
: mPath(path),
  mpImage(NULL)
{
  mpPrivate = new nuiVideoDecoderPrivate();
  mpPrivate->mpMovie = NULL;
  Init();
}

nuiVideoDecoder::~nuiVideoDecoder()
{
  if (mpPrivate->mpMovie)
    [mpPrivate->mpMovie release];
  
  delete mpPrivate;
  
  if (mpImage)
    delete mpImage;
}

bool nuiVideoDecoder::Init()
{
  NSApplicationLoad();
  
  nglString pathStr = mPath.GetPathName();
  char* pStr = pathStr.Export();
  NSString* pNSStr = [NSString stringWithCString:pStr encoding:NSUTF8StringEncoding];
  NSURL* pUrl = [NSURL fileURLWithPath:pNSStr];
  mpPrivate->mpMovie = [QTMovie movieWithFile:pNSStr error:nil];
  
  if (!mpPrivate->mpMovie)
    return false;
  
  [mpPrivate->mpMovie retain];
  return true;
}

bool nuiVideoDecoder::IsValid() const
{
  return (mpPrivate->mpMovie != NULL);
}

double nuiVideoDecoder::GetDuration() const
{
  if (!IsValid())
    return -1.0;
  
  QTTime qtDuration = [mpPrivate->mpMovie duration];
  
  double seconds = (double)(qtDuration.timeValue) / (double)(qtDuration.timeScale);
  return seconds;
}

double nuiVideoDecoder::GetPosition() const
{
  if (!IsValid())
    return -1.0;
  
  QTTime CurrTime = [mpPrivate->mpMovie currentTime];
  
  double seconds = (double)(CurrTime.timeValue) / (double)(CurrTime.timeScale);
  return seconds;
}

void nuiVideoDecoder::SetPosition(double TimePosition)
{
  if (!IsValid())
    return;
  
  QTTime time;
  time.timeScale = 1000;
  time.flags = 0;
  time.timeValue = TimePosition * time.timeScale;
  [mpPrivate->mpMovie setCurrentTime:time];
}

double nuiVideoDecoder::GetTimeTillNextFrame() const
{
  if (!IsValid())
    return -1.0;
  
  return 0;
}

bool nuiVideoDecoder::GoToNextFrame()
{
  if (!IsValid())
    return false;

  [mpPrivate->mpMovie stepForward];
  return true;
}

bool nuiVideoDecoder::GoToPrevFrame()
{
  if (!IsValid())
    return false;
  
  [mpPrivate->mpMovie stepBackward];
  
  return true;
}

nglImage* nuiVideoDecoder::GetCurrentImage()
{
  if (!IsValid())
    return NULL;
  
  NSImage* pNSImage = [mpPrivate->mpMovie currentFrameImage];
  NSData* pTiffData = [pNSImage TIFFRepresentation];
  NSBitmapImageRep* pBitmapRep = [NSBitmapImageRep imageRepWithData:pTiffData];
  NGL_ASSERT(pBitmapRep);
  
  BOOL planar = [pBitmapRep isPlanar];
  NSInteger nbPlanes = [pBitmapRep numberOfPlanes];
  NSBitmapFormat format = [pBitmapRep bitmapFormat];
  NSInteger bitsPerPixel = [pBitmapRep bitsPerPixel];
  NSInteger bytesPerPlane = [pBitmapRep bytesPerPlane];
  NSInteger samplesPerPixel = [pBitmapRep samplesPerPixel];
  NSInteger bytesPerRow = [pBitmapRep bytesPerRow];
  
  unsigned char* pBitmapArray[5];
  [pBitmapRep getBitmapDataPlanes:pBitmapArray];
  
  uint32 width = bytesPerRow / (bitsPerPixel / 8);
  uint32 height = bytesPerPlane / (bitsPerPixel / 8) / width;
  
  if (!mpImage)
  {
    nglImageInfo imgInfo(false);
    imgInfo.mWidth = width;
    imgInfo.mHeight = height;
    imgInfo.mBitDepth = bitsPerPixel;
    if (bitsPerPixel == 32)
      imgInfo.mPixelFormat = eImagePixelRGBA;
    else
      imgInfo.mPixelFormat = eImagePixelRGB;
    
    imgInfo.mBufferFormat = eImageFormatRaw;
    
    imgInfo.mBytesPerPixel = (imgInfo.mBitDepth + 1) / 8;
    imgInfo.mBytesPerLine = imgInfo.mWidth * imgInfo.mBytesPerPixel;
    imgInfo.mpBuffer = (char*)(pBitmapArray[0]);
    
    mpImage = new nglImage(imgInfo);
    
    if (!mpImage->IsValid())
    {
      delete mpImage;
      return NULL;
    }
    
    return mpImage;
  }
  
  NGL_ASSERT(width == mpImage->GetWidth());
  NGL_ASSERT(height == mpImage->GetHeight());
  NGL_ASSERT(bitsPerPixel == mpImage->GetBitDepth());
  
  char* pBuffer = mpImage->GetBuffer();
  uint32 nbBytes = width * height * (bitsPerPixel / 8);
  memcpy(pBuffer, pBitmapArray[0], nbBytes);   
  
  return mpImage;
}
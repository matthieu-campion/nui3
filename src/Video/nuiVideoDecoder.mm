/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#import "nui.h"
#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

class nuiVideoDecoderPrivate
{
public:
  
  QTMovie* mpMovie;
#ifndef __NUI64__
  QTVisualContextRef	mVisualContext;
#endif
  CVOpenGLTextureRef mCurrentTexture;
};

nuiVideoDecoder::nuiVideoDecoder(const nglPath& path)
: mPath(path),
  mpImage(NULL),
  mpTexture(NULL)
{
  NSApplicationLoad();
  
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
  CVReturn error = kCVReturnSuccess;

  nglString pathStr = mPath.GetPathName();
  char* pStr = pathStr.Export();
  NSString* pNSStr = [NSString stringWithCString:pStr encoding:NSUTF8StringEncoding];
  NSURL* pUrl = [NSURL fileURLWithPath:pNSStr];
  mpPrivate->mpMovie = [QTMovie movieWithFile:pNSStr error:NULL];
  
  if (!mpPrivate->mpMovie)
    return false;
  
  [mpPrivate->mpMovie retain];
  
  CGLContextObj ctx = CGLGetCurrentContext();
  CGLPixelFormatObj pixelFormat = CGLGetPixelFormat(ctx);
  //printf("cglcontextQT: 0x%x / 0x%x\n", ctx, pixelFormat);
  CGLRetainPixelFormat(pixelFormat);
  // creates a new OpenGL texture context for a specified OpenGL context and pixel format
#ifndef __NUI64__
	OSStatus err = QTOpenGLTextureContextCreate(kCFAllocatorDefault, ctx, pixelFormat, nil,	&(mpPrivate->mVisualContext));
  NGL_ASSERT(!err);
  
  err = SetMovieVisualContext([mpPrivate->mpMovie quickTimeMovie], mpPrivate->mVisualContext);
  NGL_ASSERT(!err);
  
  [mpPrivate->mpMovie gotoBeginning];
  MoviesTask([mpPrivate->mpMovie quickTimeMovie], 0); 
#endif
  
  QTTime qtDuration = [mpPrivate->mpMovie duration];  
  mDuration = (double)(qtDuration.timeValue) / (double)(qtDuration.timeScale);
  
  NSImage* img = [mpPrivate->mpMovie currentFrameImage];
  mWidth = [img size].width;
  mHeight = [img size].height;
  
  mRate = [mpPrivate->mpMovie rate];
  
  return true;
}



bool nuiVideoDecoder::IsValid() const
{
  return (mpPrivate->mpMovie != NULL);
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

bool nuiVideoDecoder::GoToNextFrame()
{
  if (!IsValid())
    return false;

  [mpPrivate->mpMovie stepForward];
  return true;
}

nglImage* nuiVideoDecoder::UpdateImage()
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


nuiTexture* nuiVideoDecoder::UpdateTexture()
{
  if (!IsValid())
    return NULL;

#ifndef __NUI64__
  if (!QTVisualContextIsNewImageAvailable(mpPrivate->mVisualContext, NULL))
    return NULL;
  
  if (mpPrivate->mCurrentTexture)
    CVOpenGLTextureRelease(mpPrivate->mCurrentTexture);
  QTVisualContextTask(mpPrivate->mVisualContext);
  OSStatus status = QTVisualContextCopyImageForTime(mpPrivate->mVisualContext, NULL, NULL, &(mpPrivate->mCurrentTexture));
  NGL_ASSERT(!status);
#endif
  
  GLuint texID = CVOpenGLTextureGetName(mpPrivate->mCurrentTexture);
  GLenum target = CVOpenGLTextureGetTarget(mpPrivate->mCurrentTexture);
  
  if (!mpTexture)
    mpTexture = nuiTexture::BindTexture(texID, target);
  else
    mpTexture->SetTextureIdAndTarget(texID, target);

  return mpTexture;
}


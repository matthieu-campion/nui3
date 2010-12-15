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
  QTVisualContextRef	mVisualContext;
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
  
  
  CGLContextObj ctx = CGLGetCurrentContext();
  CGLPixelFormatObj pixelFormat = CGLGetPixelFormat(ctx);
  // creates a new OpenGL texture context for a specified OpenGL context and pixel format
	OSStatus err = QTOpenGLTextureContextCreate(NULL, ctx, pixelFormat, nil,	&(mpPrivate->mVisualContext));
  
  
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
  nglString pathStr = mPath.GetPathName();
  char* pStr = pathStr.Export();
  NSString* pNSStr = [NSString stringWithCString:pStr encoding:NSUTF8StringEncoding];
  NSURL* pUrl = [NSURL fileURLWithPath:pNSStr];
  mpPrivate->mpMovie = [QTMovie movieWithFile:pNSStr error:nil];
  
  if (!mpPrivate->mpMovie)
    return false;
  
  [mpPrivate->mpMovie retain];
  
  
  OSStatus err = SetMovieVisualContext([mpPrivate->mpMovie quickTimeMovie], mpPrivate->mVisualContext);
  
  [mpPrivate->mpMovie gotoBeginning];
  MoviesTask([mpPrivate->mpMovie quickTimeMovie], 0); 
  [mpPrivate->mpMovie play];
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


nuiTexture* nuiVideoDecoder::GetCurrentTexture()
{
  if (!IsValid())
    return NULL;
  
//  CGLContextObj ctx = CGLGetCurrentContext();
//  CGLPixelFormatObj pixelFormat = CGLGetPixelFormat(ctx);
//  QTTime time = [mpPrivate->mpMovie currentTime];
//
//  NSMutableDictionary* dict = [NSMutableDictionary dictionary];
//  
//  NSString* type = QTMovieFrameImageTypeCVOpenGLTextureRef;
//  [dict setObject:type forKey:QTMovieFrameImageType];
//  
//  NSValue* ctxVal = [NSValue valueWithPointer:ctx];
//  [dict setObject:ctxVal forKey:QTMovieFrameImageOpenGLContext];
//  
//  NSValue* formatVal = [NSValue valueWithPointer: pixelFormat];
//  [dict setObject:formatVal forKey:QTMovieFrameImagePixelFormat];
//  
//  CVOpenGLTextureRef texture = (CVOpenGLTextureRef)[mpPrivate->mpMovie frameImageAtTime:time withAttributes:dict error:nil];
//  CVOpenGLTextureRetain(texture);
  
  if (!QTVisualContextIsNewImageAvailable(mpPrivate->mVisualContext, NULL))
    return NULL;
  
  if (mpPrivate->mCurrentTexture)
    CVOpenGLTextureRelease(mpPrivate->mCurrentTexture);
  OSStatus status = QTVisualContextCopyImageForTime(mpPrivate->mVisualContext, NULL, NULL, &(mpPrivate->mCurrentTexture));
  
  GLuint texID = CVOpenGLTextureGetName(mpPrivate->mCurrentTexture);
  GLenum target = CVOpenGLTextureGetTarget(mpPrivate->mCurrentTexture);
//  
//  GLfloat lowerLeft[2];
//  GLfloat lowerRight[2]; 
//  GLfloat upperRight[2];
//  GLfloat upperLeft[2];
//  CVOpenGLTextureGetCleanTexCoords(texture, lowerLeft, lowerRight, upperRight, upperLeft);
  
  
  
//  int c = 0;
//  uint8 * tab = new uint8[16];
//  tab[c++] = 0;
//  tab[c++] = 0;
//  tab[c++] = 0;
//  //tab[c++] = 0;
//
//  tab[c++] = 255;
//  tab[c++] = 255;
//  tab[c++] = 255;
//  //tab[c++] = 255;
//  
//  tab[c++] = 255;
//  tab[c++] = 255;
//  tab[c++] = 255;
//  //tab[c++] = 255;
//
//  tab[c++] = 0;
//  tab[c++] = 0;
//  tab[c++] = 0;
//  //tab[c++] = 0;
//  
//  GLuint texture = 0;
//  GLenum target = GL_TEXTURE_2D;
//  glGenTextures(1, &texture);
//  glBindTexture(target, texture);
//  glTexImage2D(target, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, tab);
//  
//  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
//  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
//  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//  
  
  if (!mpTexture)
    mpTexture = nuiTexture::BindTexture(texID, target);
  else
    mpTexture->SetTextureIdAndTarget(texID, target);
  
  return mpTexture;
}
/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include <ApplicationServices/ApplicationServices.h>


nglVideoMode::nglVideoMode()
{
#ifdef __NGL_MACHO__
  mDisplay = CGMainDisplayID();
#endif  
  Init();

#ifdef __NGL_MACHO__
  CFDictionaryRef displayMode ;
  CFNumberRef number ;

  displayMode = (CFDictionaryRef) CGDisplayCurrentMode((CGDirectDisplayID)mDisplay) ;

  /* Get the height and width of this display mode. Print them out. */
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayWidth ) ;
  CFNumberGetValue( number, kCFNumberLongType, &mWidth ) ;
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayHeight ) ;
  CFNumberGetValue( number, kCFNumberLongType, &mHeight ) ;
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayRefreshRate ) ;
  CFNumberGetValue( number, kCFNumberLongType, &mRate ) ;
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayBitsPerPixel ) ;
  CFNumberGetValue( number, kCFNumberLongType, &mBPP ) ;
#else
  mWidth = 800;
  mHeight = 600;
  mBPP = 32;
  mRate = 60;
#endif
  
  mScaleFactor = 1.0f;
  mInvScaleFactor = 1.0f / mScaleFactor;

}

nglVideoMode::nglVideoMode (const nglVideoMode& rMode)
{
  mDisplay = rMode.mDisplay;
  mWidth = rMode.mWidth;
  mHeight = rMode.mHeight;
  mRate = rMode.mRate;
  mBPP = rMode.mBPP;

}

nglVideoMode::~nglVideoMode()
{
#ifdef __NGL_MACHO__
  CGDisplayRelease( (CGDirectDisplayID)mDisplay ) ;  
#endif
}

void nglVideoMode::Init()
{
}

bool nglVideoMode::Enum(nglVideoMode& Mode, uint Index)
{
#ifdef __NGL_MACHO__
  CFArrayRef displayModeArray ;
  CFDictionaryRef displayMode ;
  CFNumberRef number ;

  displayModeArray = CGDisplayAvailableModes( (CGDirectDisplayID)Mode.mDisplay ) ;
  if ( CFArrayGetCount(displayModeArray) <= Index)
  {
    return false;
	}

  displayMode = (CFDictionaryRef) CFArrayGetValueAtIndex(displayModeArray, Index) ;

  /* Get the height and width of this display mode. Print them out. */
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayWidth ) ;
  CFNumberGetValue( number, kCFNumberLongType, &Mode.mWidth ) ;
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayHeight ) ;
  CFNumberGetValue( number, kCFNumberLongType, &Mode.mHeight ) ;
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayRefreshRate ) ;
  CFNumberGetValue( number, kCFNumberLongType, &Mode.mRate ) ;
  number = (CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayBitsPerPixel ) ;
  CFNumberGetValue( number, kCFNumberLongType, &Mode.mBPP ) ;

  return true;
#else
  return false;
#endif
}

uint nglVideoMode::GetWidth() const
{
  return mWidth;
}

uint nglVideoMode::GetHeight() const
{
  return mHeight;
}

uint nglVideoMode::GetDepth() const
{
  return mBPP;
}

uint nglVideoMode::GetFrequency() const
{
  return mRate; // Unknown
}


bool nglVideoMode::SetMode (nglVideoMode* pVideoMode, bool Lock)
{
#ifdef __NGL_MACHO__
  bool done = false;

  boolean_t exactMatch;
  CFDictionaryRef mode = CGDisplayBestModeForParametersAndRefreshRate((CGDirectDisplayID)pVideoMode->mDisplay, pVideoMode->mBPP, pVideoMode->mWidth, pVideoMode->mHeight, pVideoMode->mRate, &exactMatch);
  CGDisplaySwitchToMode( (CGDirectDisplayID)pVideoMode->mDisplay, mode);
//  NGL_DEBUG( NGL_LOG("vidmode", NGL_LOG_INFO, _T("switching to %s: %s"), Dump().GetChars(), done ? _T("ok"):_T("failed")); )
  return done;
#else
  return false;
#endif
}

static float gScaleFactor = 1.0f;
static float gInvScaleFactor = 1.0f;

float nuiGetScaleFactor()
{
  return gScaleFactor;
}

float nuiGetInvScaleFactor()
{
  return gInvScaleFactor;
}



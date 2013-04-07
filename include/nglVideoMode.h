/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglVideoMode.h
\brief Video mode description and enumeration

This class is not available if the _NOGFX_ symbol is defined.
*/

#ifndef __nglVideoMode_h__
#define __nglVideoMode_h__

//#include "nui.h"
#include "nglString.h"

#ifndef _NOGFX_

#ifdef _X11_
#include <X11/Xlib.h>
#endif // _X11_


//! Video mode management
/*!
This class let you find suitable resolutions for fullscreen rendering windows
(see nglWindowInfo for more details).
*/
class NGL_API nglVideoMode
{
public:
  /** @name Life cycle */
  //@{
  nglVideoMode();
  /*!<
    Get the current video mode. If you want to request other video modes,
    you must use the static 'requesters' methods Enum() or Find().

    The current video mode is useful to obtain the current desktop dimensions,
    and/or to switch to fullscreen mode without changing the desktop resolution.
  */
  nglVideoMode(const nglVideoMode& rMode); ///< Copy constructor
  ~nglVideoMode();
  //@}

  /** @name Requesters */
  //@{
  static bool Enum(nglVideoMode& rMode, uint Index);
  /*!<
    Enumerate available video modes.
    \param rMode video mode information is written here
    \param Index mode index
    \return if true, \a rMode contains valid information

    Video modes are numbered consecutively, starting from 0. To list available
    video mode, you shoul call Get() with a zero-based growing index until
    the method returns false. Example :

\code
nglVideoMode mode;
int i = 0;

while (nglVideoMode::Get(i, mode))
{
  i++;
  NGL_OUT(_T("Mode #%d: %dx%dx%d - vertical refreh: %d Hz"), i,
    mode.GetWidth(), pmode.GetHeight(), pmode.GetDepth(), pmode.GetVFrequency());
}
\endcode
  */
  static bool Find (nglVideoMode& rMode, uint Width, uint Height, uint Depth = 0, uint Frequency = 0);
  /*!<
    Find a video mode.
    \param rMode Video mode information is written here
    \param Width Desired width, must be non zero
    \param Height Desired height, must be non zero
    \param Depth Desired bitdepth. If set to zero, use current desktop bitdepth
    \param Frequency Desired vertical refresh frequency. If set to zero, try to match at least current desktop frequency
    \return if true, \a rMode contains valid information
  */
  //@}

  /** @name Properties */
  //@{
  uint GetWidth() const;       ///< Horizontal resolution in pixels
  uint GetHeight() const;      ///< Vertical resolution in pixels
  uint GetDepth() const;       ///< Color bits per pixel
  uint GetFrequency() const;   ///< Vertical monitor refresh rate in Hz
  nglString Dump() const;      ///< Dumps a textual description
  //@}

#ifdef _WIN32_
public:
  bool SetMode() const;

private:
  DEVMODE mMode;
#endif // _WIN32_

#ifdef _X11_
public:
  bool SetMode (bool Lock) const;

private:
  Display*                     mpDisplay;
  int                          mScreen;
  class XF86VidModeExt*        mpVidModeExt;

  void Init();
  static void Init(Display*& rpDisplay, int& rScreen);
  static void Exit();
#endif // _X11_

#if (defined _CARBON_ || defined _COCOA_)
private:
  static uint                  mXDepth;

  static void Init();
  static bool SetMode (nglVideoMode* pMode, bool Lock);

  CGDirectDisplayID mDisplay;
  long mHeight, mWidth, mRate, mBPP;

#endif // _CARBON_

#ifdef _UIKIT_
private:
  static uint                  mXDepth;

  static void Init();
  static bool SetMode (nglVideoMode* pMode, bool Lock);

  long mHeight, mWidth, mRate, mBPP;

#endif//_UIKIT_

private:
friend class nglApplication;
friend class nglWindow;
};


float nuiGetScaleFactor(); ///< return the current scale factor
float nuiGetInvScaleFactor(); ///< return the inverse of the current scale factor


#endif // !_NOGFX_
#endif // __nglVideoMode_h__

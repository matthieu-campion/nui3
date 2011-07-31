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
#include "nglVideoMode.h"
#include "nglKernel.h"
#ifdef HAVE_XF86VIDMODE
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#endif


#ifdef HAVE_XF86VIDMODE
class XF86VidModeExt
{
public:
  static bool                  mUse;
  static XF86VidModeModeInfo** mpModeList;
  static uint                  mModeCnt;

  XF86VidModeModeInfo*         mpMode;

  XF86VidModeExt() : mpMode(NULL) {}
};

bool                  XF86VidModeExt::mUse       = false;
XF86VidModeModeInfo** XF86VidModeExt::mpModeList = NULL;
uint                  XF86VidModeExt::mModeCnt   = 0;
#endif


nglVideoMode::nglVideoMode()
{
  Init();

#ifdef HAVE_XF86VIDMODE
  if (XF86VidModeExt::mUse)
  {
    XF86VidModeModeLine modeline;
    int dotclock;
    uint i = 0;

    XF86VidModeGetModeLine (mpDisplay, mScreen, &dotclock, &modeline);

    while (i < XF86VidModeExt::mModeCnt && !mpVidModeExt->mpMode)
    {
      XF86VidModeModeInfo* mode = XF86VidModeExt::mpModeList[i];

      if (mode->hdisplay == modeline.hdisplay &&
          mode->vdisplay == modeline.vdisplay)
        mpVidModeExt->mpMode = mode;

      i++;
    }
  }
#endif // HAVE_XF86VIDMODE
  
  mScaleFactor = 1.0f;
  mInvScaleFactor = 1.0f / mScaleFactor;

}

nglVideoMode::nglVideoMode (const nglVideoMode& rMode)
{
  mpDisplay = rMode.mpDisplay;
  mScreen   = rMode.mScreen;
#ifdef HAVE_XF86VIDMODE
  mpVidModeExt = new XF86VidModeExt(*rMode.mpVidModeExt);
#endif
}

nglVideoMode::~nglVideoMode()
{
#ifdef HAVE_XF86VIDMODE
  if (mpVidModeExt)
    delete mpVidModeExt;
#endif
}

void nglVideoMode::Init()
{
#ifdef HAVE_XF86VIDMODE
  mpVidModeExt = new XF86VidModeExt();
#endif
  Init(mpDisplay, mScreen);
}

void nglVideoMode::Init (Display*& rpDisplay, int& rScreen)
{
  rpDisplay = (Display*)App->GetDisplay();
  if (!rpDisplay)
    return;
  rScreen   = DefaultScreen (rpDisplay);

#ifdef HAVE_XF86VIDMODE
  if (!XF86VidModeExt::mpModeList)
  {
    int major, minor;

    // Get XF86VMode extension info
    XF86VidModeExt::mUse = XF86VidModeQueryExtension (rpDisplay, &major, &minor);
    if (XF86VidModeExt::mUse)
    {
      int cnt;

      XF86VidModeQueryVersion (rpDisplay, &major, &minor);
      NGL_DEBUG( NGL_LOG("vidmode", NGL_LOG_INFO, _T("detected XF86VidMode %d.%d"), major, minor); )

      XF86VidModeGetAllModeLines (rpDisplay, rScreen, &cnt, &XF86VidModeExt::mpModeList);
      XF86VidModeExt::mModeCnt = cnt;

      if (XF86VidModeExt::mpModeList)
        App->AddExit(Exit); // We'll need to free the mode list
    }
  }
#endif // HAVE_XF86VIDMODE
}

void nglVideoMode::Exit()
{
#ifdef HAVE_XF86VIDMODE
  if (XF86VidModeExt::mpModeList)
  {
    uint i;

    for (i = 0; i < XF86VidModeExt::mModeCnt; i++)
    {
      XF86VidModeModeInfo* mode = XF86VidModeExt::mpModeList[i];

      if (mode->privsize > 0)
        XFree(mode->c_private); // Are we sure of that ? Funky man page.
    }

    XFree(XF86VidModeExt::mpModeList);
  }
#endif // HAVE_XF86VIDMODE
}


bool nglVideoMode::Enum (nglVideoMode& Mode, uint Index)
{
  Display* display;
  int screen;

  Init(display, screen);
  Mode.mpDisplay = display;
  Mode.mScreen   = screen;

#ifdef HAVE_XF86VIDMODE
  Mode.mpVidModeExt = new XF86VidModeExt();

  if (XF86VidModeExt::mUse)
  {
    if (Index >= XF86VidModeExt::mModeCnt)
      return false;

    Mode.mpVidModeExt->mpMode = XF86VidModeExt::mpModeList[Index];
  }
  else
#endif // HAVE_XF86VIDMODE
  {
    // One video mode only, the current resolution
    if (Index > 0)
      return false;
  }
  return true;
}

uint nglVideoMode::GetWidth() const
{
#ifdef HAVE_XF86VIDMODE
  if (XF86VidModeExt::mUse)
    return mpVidModeExt->mpMode->hdisplay;
  else
#endif
  return DisplayWidth (mpDisplay, mScreen);
}

uint nglVideoMode::GetHeight() const
{
#ifdef HAVE_XF86VIDMODE
  if (XF86VidModeExt::mUse)
    return mpVidModeExt->mpMode->vdisplay;
  else
#endif
  return DisplayHeight (mpDisplay, mScreen);
}

uint nglVideoMode::GetDepth() const
{
  return DefaultDepth (mpDisplay, mScreen);
}

uint nglVideoMode::GetFrequency() const
{
#ifdef HAVE_XF86VIDMODE
  if (XF86VidModeExt::mUse)
  {
    XF86VidModeModeInfo* mode = mpVidModeExt->mpMode;

    return mode->dotclock * 1000 / (mode->htotal * mode->vtotal);
  }
  else
#endif
  return 0; // Unknown
}


bool nglVideoMode::SetMode (bool Lock) const
{
  bool done = true;

#ifdef HAVE_XF86VIDMODE
  if (XF86VidModeExt::mUse)
  {
    XF86VidModeModeInfo* mode = mpVidModeExt->mpMode;

    if (mode &&
        XF86VidModeSwitchToMode (mpDisplay, mScreen, mode))
    {
      XSync (mpDisplay, False);
      XF86VidModeSetViewPort (mpDisplay, mScreen, 0, 0);
      XF86VidModeLockModeSwitch (mpDisplay, mScreen, (Lock) ? True : False);
    }
    else
      done = false;
  }
#endif
  NGL_DEBUG( NGL_LOG(_T("vidmode"), NGL_LOG_INFO, _T("switching to %s: %s"), Dump().GetChars(), done ? _T("ok"):_T("failed")); )
  return done;
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




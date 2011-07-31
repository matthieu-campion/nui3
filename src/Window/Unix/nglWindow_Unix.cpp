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
#include "nglApplication.h"
#include "nglVideoMode.h"
#include "nglWindow.h"
#include "nuiMainMenu.h"
#include "ngl_unix.h"

#include <string.h>
#include <wchar.h>
#include <X11/cursorfont.h>


#define NGL_WINDOW_EBASE      (NGL_CONTEXT_ELAST+1)

/*
#define NGL_WINDOW_ENONE      0 (in nglWindow.h)
*/
#define NGL_WINDOW_ESYSWIN    (NGL_WINDOW_EBASE+1)
#define NGL_WINDOW_EVIDMODE   (NGL_WINDOW_EBASE+2)
#define NGL_WINDOW_ERANGE     (NGL_WINDOW_EBASE+3)

const nglChar* gpWindowErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("System window creation failed"),
/*  2 */ _T("Full screen video mode unavailable"),
/*  3 */ _T("Properties out of range"),
         NULL
};


/*
 * PC-102/105 mapping (XFree86)
 */

#define NGL_KEYMAP_SIZE 0x80
#define NGL_KEYMAP_MASK 0x7F

uint gpKeymapXFree86[NGL_KEYMAP_SIZE] = {
/*  0 */ 0, 0, 0, 0, 0, 0, 0, 0,
/*  8 */ 0, NK_ESC, NK_1, NK_2, NK_3, NK_4, NK_5, NK_6,
/* 10 */ NK_7, NK_8, NK_9, NK_0, NK_MINUS, NK_EQUAL, NK_BACKSPACE, NK_TAB,
/* 18 */ NK_Q, NK_W, NK_E, NK_R, NK_T, NK_Y, NK_U, NK_I,
/* 20 */ NK_O, NK_P, NK_LBRACKET, NK_RBRACKET, NK_ENTER, NK_LCTRL, NK_A, NK_S,
/* 28 */ NK_D, NK_F, NK_G, NK_H, NK_J, NK_K, NK_L, NK_SEMICOLON,
/* 30 */ NK_APOSTROPHE, NK_GRAVE, NK_LSHIFT, NK_NUMBERSIGN, NK_Z, NK_X, NK_C, NK_V,
/* 38 */ NK_B, NK_N, NK_M, NK_COMMA, NK_PERIOD, NK_SLASH, NK_RSHIFT, NK_PAD_ASTERISK,
/* 40 */ NK_LALT, NK_SPACE, NK_CAPSLOCK, NK_F1, NK_F2, NK_F3, NK_F4, NK_F5,
/* 48 */ NK_F6, NK_F7, NK_F8, NK_F9, NK_F10, NK_PAD_LOCK, NK_SCRLOCK, NK_PAD_7,
/* 50 */ NK_PAD_8, NK_PAD_9, NK_PAD_MINUS, NK_PAD_4, NK_PAD_5, NK_PAD_6, NK_PAD_PLUS, NK_PAD_1,
/* 58 */ NK_PAD_2, NK_PAD_3, NK_PAD_0, NK_PAD_PERIOD, 0, 0, NK_BACKSLASH, NK_F11,
/* 60 */ NK_F12, NK_HOME, NK_UP, NK_PAGEUP, NK_LEFT, 0, NK_RIGHT, NK_END,
/* 68 */ NK_DOWN, NK_PAGEDOWN, NK_INSERT, NK_DELETE, NK_PAD_ENTER, NK_RCTRL, NK_PAUSE, NK_SYSREQ,
/* 70 */ NK_PAD_SLASH, NK_RALT, 0, NK_LMETA, NK_RMETA, NK_MENU, 0, 0,
/* 78 */ 0, 0, 0, 0, 0, 0, 0, 0
};


/*
 * X events settings
 */

#define NGL_CLICK_DELAY 250

//#define NGL_X_EVENTS 0x1ffffff
#define NGL_X_KEY_EVENTS   (KeyPressMask | KeyReleaseMask)
#define NGL_X_MOUSE_EVENTS (ButtonPressMask | ButtonReleaseMask | PointerMotionMask)
#define NGL_X_EVENTS \
  ( ExposureMask | FocusChangeMask | \
    StructureNotifyMask | \
    EnterWindowMask | LeaveWindowMask | \
    NGL_X_KEY_EVENTS | \
    NGL_X_MOUSE_EVENTS )


/*
 * Constructors
 */

nglWindow::nglWindow (uint Width, uint Height, bool IsFullScreen)
{
  Register();
  nglContextInfo context; // Get default context
  nglWindowInfo info(Width, Height, IsFullScreen);

  InternalInit (context, info, NULL);
}

nglWindow::nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  Register();
  InternalInit (rContext, rInfo, pShared);
}


void nglWindow::InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  if (GetError())
    return;

  // X ressources
  mColormap    = 0;
  mWindow      = 0;
  mXCursor     = None;
  mEmptyCursor = None;
  mWMProtocol  = None;
  mWMDelete    = None;
  mHintsAtom   = None;
  mScreen      = DefaultScreen (mpDisplay);
  mGC          = DefaultGC (mpDisplay, mScreen);

  // Internals
  mInitCalled  = false;
  mConfigureCalled = false;
  mIsHidden    = true;
  mNeedGrab    = false;
  mWaitExpose  = false;
  mXPos        = 0;
  mYPos        = 0;
  mWidth       = rInfo.Width;
  mHeight      = rInfo.Height;
  mLastMouseX  = 0;
  mLastMouseY  = 0;
  mWasMapped   = false;
  mLastMappedX = 0;
  mLastMappedY = 0;
  mLastClick   = 0;
  mLastButton  = 0;
  mClickCnt    = 0;
  mFullScreen  = (rInfo.Flags & FullScreen) ? true : false;

  // Portable members
  mOSInfo    = rInfo.OSInfo;
  mFlags     = rInfo.Flags;
  mEventMask = rInfo.Events;
  mCursor    = rInfo.Cursor;
  mMouseMode = rInfo.MouseMode;
  mKeyRepeat = rInfo.KeyRepeat;
  memset (mpKeyState, 0, sizeof(bool)*NGL_KEY_MAX);

  if ((mWidth == 0) || (mHeight == 0))
  {
    SetError (_T("window"), NGL_WINDOW_ERANGE);
    return;
  }


  /*
   * Window creation
   */

  XSetWindowAttributes xattr;
  unsigned long xattr_mask;
  int  x, y;
  uint w = mWidth;
  uint h = mHeight;
  uint screen_w = DisplayWidth (mpDisplay, mScreen);
  uint screen_h = DisplayHeight (mpDisplay, mScreen);
  Window root = RootWindow (mpDisplay, mScreen);

  /*
   * GLX rendering context creation
   */
  if (!Build(mScreen, rContext, pShared))
    return; // An error is already raised by nglContext's code



  // Check fullscreen video mode
  if (mFullScreen)
  {
    uint bitdepth;

    x = 0;
    y = 0;
    bitdepth = rContext.FrameBitsR;
    bitdepth+= rContext.FrameBitsG;
    bitdepth+= rContext.FrameBitsB;
    if (!nglVideoMode::Find(mVideoMode, w, h, bitdepth))
    {
/*
      SetError ("window", NGL_WINDOW_EVIDMODE);
      return;
*/
      NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_WARNING, _T("can't switch to %dx%dx%d fullscreen mode"), w, h, bitdepth); )
      mFullScreen = false;
    }
  }

  if (!mFullScreen)
  {
    // Calculate position
    switch (rInfo.Pos)
    {
      case nglWindowInfo::ePosCenter:
        x = (screen_w - mWidth) / 2;
        y = (screen_h - mHeight) / 2;
        break;

      case nglWindowInfo::ePosMouse:
        Window child;
        int local_x, local_y;
        uint mask;
        XQueryPointer (mpDisplay, root,
          &root, &child,
          &x, &y,
          &local_x, &local_y,
          &mask);

        // Center on mouse pointer
        x -= mWidth / 2;
        y -= mHeight / 2;
        // Stay in screen boundaries
        if (x < 0) x = 0; else
        if ((x + mWidth) >= screen_w) x = screen_w - mWidth - 1;
        if (y < 0) y = 0; else
        if ((y + mHeight) >= screen_h) y = screen_h - mHeight - 1;
        break;

      case nglWindowInfo::ePosUser:
      case nglWindowInfo::ePosAuto:
      default:
        x = rInfo.XPos;
        y = rInfo.YPos;
    }
  }

  mColormap = XCreateColormap (mpDisplay, root, mpXVisual, AllocNone);

  // Set window attributes
  xattr_mask = CWColormap|CWEventMask|CWOverrideRedirect;
  xattr.colormap = mColormap;
  xattr.event_mask = NGL_X_EVENTS; // FIXME: use mEventMask
  xattr.override_redirect = (mFullScreen) ? True : False;
//  NGL_DEBUG( xattr.background_pixel = BlackPixel (mpDisplay, mScreen); )

  // Create window
  Window win;

  win = XCreateWindow (mpDisplay, root, x, y, w, h, 0, mDepth, InputOutput, mpXVisual, xattr_mask, &xattr);
  if (!win)
  {
    SetError (_T("window"), NGL_WINDOW_ESYSWIN);
    return;
  }

  // Set window size/pos WM hints
  if (!mFullScreen)
  {
    XSizeHints hints;

    hints.flags = 0;
    if (rInfo.Pos != nglWindowInfo::ePosAuto)
    {
      hints.flags |= USPosition;
      hints.x = x;
      hints.y = y;
    }
    if (mFlags & NoResize)
    {
      hints.flags |= PMinSize|PMaxSize;
      hints.min_width  = w;
      hints.min_height = h;
      hints.max_width  = w;
      hints.max_height = h;
    }
    XSetWMNormalHints (mpDisplay, win, &hints);

    if (mFlags & NoBorder)
      SetDeco(0);
  }

  // Bind GLX context to window
  if (!nglContext::MakeCurrent(win))
    return;
  NGL_DEBUG( nglContext::Dump(NGL_LOG_INFO); )

  // Catch ICCCM WM_DELETE_WINDOW message (for OnClose callback)
  mWMProtocol = XInternAtom (mpDisplay, "WM_PROTOCOLS", False);
  mWMDelete = XInternAtom (mpDisplay, "WM_DELETE_WINDOW", False);
  XSetWMProtocols (mpDisplay, win, &(mWMDelete), 1);

  // From this point, we can't fail
  mWindow = win;
  SetTitle (rInfo.Title);

  // nglApplication will dispatch X events for us, via OnXEvent()
  App->AddWindow (this);
#ifdef _DEBUG_
  nglWindowInfo info = rInfo;

  info.XPos = x;
  info.YPos = y;
  info.Width = w;
  info.Height = h;
  if (!mFullScreen) info.Flags &= ~FullScreen;
  info.Dump(NGL_LOG_INFO);
#endif // _DEBUG_

  /* Post an event to trigger the OnInit() callback asynchronoyusly
   * Important: cannot be called here because we are mostly invoked
   * from a constructor and we can't access the overloaded [user's] OnInit()
   */
  XExposeEvent xexpose;

  xexpose.type = Expose;
  xexpose.display = mpDisplay;
  xexpose.window = mWindow;
  xexpose.count = 0;
  XSendEvent (mpDisplay, mWindow, False, NoEventMask, (XEvent*)&xexpose);
  XFlush (mpDisplay);
}

nglWindow::~nglWindow()
{
  if (!mWindow)
    return;

  App->DelWindow (this);

  if (mFullScreen)
    mSavedVideoMode.SetMode(false); // Restore saved video mode

  /* Release X ressources
   */

  if (mWindow)
  {
    CallOnDestruction();
    XDestroyWindow (mpDisplay, mWindow);
    mWindow = 0;
  }
  if (mColormap)
    XFreeColormap (mpDisplay, mColormap);
  if (mEmptyCursor)
    XFreeCursor (mpDisplay, mEmptyCursor);

  XFlush (mpDisplay);
  mpDisplay = NULL;

  Unregister();
}


/*
 * Geometry
 */

uint nglWindow::GetWidth() const
{
  return mWidth;
}

uint nglWindow::GetHeight() const
{
  return mHeight;
}

void nglWindow::GetSize (uint& rWidth, uint& rHeight) const
{
  rWidth = mWidth;
  rHeight = mHeight;
}

bool nglWindow::SetSize (uint Width, uint Height)
{
  if (!mWindow) return false;

  // Clamping
  if (Width < 1)  Width = 1;
  if (Height < 1) Height = 1;

  XResizeWindow (mpDisplay, mWindow, Width, Height);
  XFlush (mpDisplay);
  return true;
}

void nglWindow::GetPosition (int& rXPos, int& rYPos) const
{
  if (!mWindow) return;

  rXPos = mXPos;
  rYPos = mYPos;
}

bool nglWindow::SetPosition (int XPos, int YPos)
{
  if (!mWindow) return false;

  XMoveWindow (mpDisplay, mWindow, XPos, YPos);
  XFlush (mpDisplay);
  return true;
}

bool nglWindow::GetResolution (float& rHorizontal, float& rVertical) const
{
  if (!mpDisplay)
    return false;

  rHorizontal = (float)DisplayWidth(mpDisplay, mScreen) * 25.4f / (float)DisplayWidthMM(mpDisplay, mScreen);
  rVertical   = (float)DisplayHeight(mpDisplay, mScreen) * 25.4f / (float)DisplayHeightMM(mpDisplay, mScreen);
  return true;
}


/*
 * Appearance
 */

nglWindow::StateInfo nglWindow::GetState() const
{
  XWindowAttributes attr;

  if (!mWindow)
    return eHidden;

  XGetWindowAttributes (mpDisplay, mWindow, &attr);
  return (attr.map_state == IsUnmapped) ? eHidden : eVisible;
}

void nglWindow::SetState (StateChange State)
{
  if (!mWindow) return;
  switch (State)
  {
    case eHide:
      if (!mIsHidden)
      {
        if (mWasMapped)
        {
          Window root;
          unsigned int x;

          // Remember position for next re-mapping
          XGetGeometry (mpDisplay, mWindow, &root, &mLastMappedX, &mLastMappedY, &x, &x, &x, &x);
        }
        XWithdrawWindow (mpDisplay, mWindow, mScreen);
        mIsHidden = true;
        break;
      }

    case eShow:
      if (mWasMapped)
      {
        XSizeHints hints;

        // Restore position when last hidden
        hints.flags = USPosition;
        hints.x = mLastMappedX;
        hints.y = mLastMappedY;
        XSetWMNormalHints (mpDisplay, mWindow, &hints);
      }
      if (mFullScreen)
      {
        if (!mVideoMode.SetMode(true))
          mFullScreen = false;
      }
      XMapWindow (mpDisplay, mWindow);
      mWasMapped = true;
      mIsHidden = false;
      break;

    case eMinimize:
      if (mIsHidden)
      {
        XWMHints hints;

        hints.flags = StateHint;
        hints.initial_state = IconicState;
        XSetWMHints (mpDisplay, mWindow, &hints);
        XMapWindow (mpDisplay, mWindow);
      }
      else
        XIconifyWindow (mpDisplay, mWindow, mScreen);
      mWasMapped = true;
      mIsHidden = false;
      break;
    case eMaximize:
      break;
  }
  XFlush (mpDisplay);
}


nglString nglWindow::GetTitle() const
{
  nglString result;
  char* title;

  if (mWindow && mpDisplay && mWindow && XFetchName (mpDisplay, mWindow, &title))
    result = title;

  return result;
}

void nglWindow::SetTitle (const nglString& rTitle)
{
  if (!mWindow) return;
  char* title = rTitle.Export();

  XStoreName (mpDisplay, mWindow, title ? title : "");
  XFlush (mpDisplay);

  if (title)
    free (title);
}


nuiMouseCursor nglWindow::GetCursor() const
{
  return mCursor;
}

bool nglWindow::SetCursor (nuiMouseCursor Cursor)
{
  if (!mWindow) return false;

  if (Cursor == mCursor)
    return true;

  ::Cursor xcursor = GetXCursor(Cursor);

  if (mXCursor != None)
    XFreeCursor(mpDisplay, mXCursor);

  XDefineCursor(mpDisplay, mWindow, xcursor);
  mXCursor = xcursor;
  mCursor = Cursor;

  return true;
}


/*
 * Events control
 */

nglWindow::EventMask nglWindow::GetEventMask() const
{
  return mEventMask;
}

void nglWindow::SetEventMask(EventMask Events)
{
  mEventMask = Events;
  // FIXME (update mWindow event field, spares bandwidth on indirect contexts)
}


/*
 * Extension
 */

const nglWindow::OSInfo* nglWindow::GetOSInfo() const
{
  return &mOSInfo;
}


/*
 * Mouse
 */

nglMouseInfo::Mode nglWindow::GetMouseMode() const
{
  return mMouseMode;
}

void nglWindow::SetMouseMode(nglMouseInfo::Mode Mode)
{
  mMouseMode = Mode; // FIXME
}


void nglWindow::GetMouse (nglMouseInfo &rInfo, bool Local) const
{
  if (!mWindow) return;

  Window root, child;
  int root_x, root_y, x, y;
  uint mask;

  XQueryPointer (mpDisplay, mWindow,
    &root, &child,
    &root_x, &root_y,
    &x, &y,
    &mask);

  switch(mMouseMode)
  {
    case nglMouseInfo::eRelative:
      rInfo.X = 0;
      rInfo.Y = 0;
      break;

    case nglMouseInfo::eAbsolute:
      if (Local)
      {
        rInfo.X = x;
        rInfo.Y = y;
      }
      else
      {
        rInfo.X = root_x;
        rInfo.Y = root_y;
      }
      break;
  }
  rInfo.Buttons = GetXButtons(mask);
}


/*
 * Keyboard
 */

bool nglWindow::GetKeyRepeat() const
{
  return mKeyRepeat;
}

void nglWindow::SetKeyRepeat(bool AllowRepeat)
{
  mKeyRepeat = AllowRepeat;

  if (mKeyRepeat)
    XAutoRepeatOn (mpDisplay);
  else
    XAutoRepeatOff (mpDisplay);
}


bool nglWindow::IsKeyDown(nglKeyCode Key) const
{
  if (Key == NK_META)
    return IsKeyDown(NK_LMETA) || IsKeyDown(NK_RMETA);
  else if (Key == NK_CTRL)
    return IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL);
  else if (Key == NK_SHIFT)
    return IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT);
  else if (Key == NK_ALT)
    return IsKeyDown(NK_LALT) || IsKeyDown(NK_RALT);

  if ((Key > 0) && (Key <= NGL_KEY_MAX))
    return mpKeyState[Key-1];
  else
    return false;
}


/*
 * Drag and drop
 */
void nglWindow::OnDragEnter() {}
void nglWindow::OnDragLeave() {}
nglDropEffect nglWindow::OnCanDrop(nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button) { return eDropEffectNone; }
void nglWindow::OnDropped (nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button) {}
bool nglWindow::Drag(nglDragAndDrop* pDragObject) { return false; }

void nglWindow::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
}
void nglWindow::OnDragStop(bool canceled)
{
}




/*
 * Rendering
 */

void nglWindow::BeginSession()
{
  if (!mWindow) return;

  glXMakeCurrent (mpDisplay, mWindow, mGlxCtx);
}

bool nglWindow::MakeCurrent() const
{
  return nglContext::MakeCurrent(mWindow);
}


void nglWindow::EndSession()
{
  if (!mWindow) return;

  glFinish();
  glXSwapBuffers (mpDisplay, mWindow);
  XFlush (mpDisplay);
}

void nglWindow::Invalidate()
{
  if (!mWindow) return;
  XExposeEvent expose;

  // Only generate an expose event if the previous one has been received.
  // (mWaitExpose is reset to false in the Paint method).
  if (mWaitExpose) return;
  mWaitExpose = true;
  // XSendEvent fills out serial and send_event(=true) for us.
  // We don't need to set x, y, width and height since we don't use it.
  expose.type = Expose;
  expose.display = mpDisplay;
  expose.window = mWindow;
  expose.count = 0;
  XSendEvent (mpDisplay, mWindow, False, NoEventMask, (XEvent*)&expose);
  XFlush (mpDisplay);
}


/*
 * Internals
 */

Window nglWindow::GetHandle()
{
  return mWindow;
}

void nglWindow::OnXEvent (const XEvent& rXEvent)
{
  if (rXEvent.xany.window != mWindow) return;

  switch (rXEvent.xany.type)
  {
/*
    case CreateNotify:
      CallOnCreation();
      break;
*/
    case MotionNotify:
      DoMouseMove (rXEvent.xmotion);
      break;

    case ButtonPress:
    case ButtonRelease:
      DoMouseClick (rXEvent.xbutton);
      break;

    case KeyPress:
    case KeyRelease:
      DoKeyPress (rXEvent.xkey);
      break;

    case ConfigureNotify:
      mXPos = rXEvent.xconfigure.x;
      mYPos = rXEvent.xconfigure.y;
      if (!mConfigureCalled ||
          (mWidth != (uint)rXEvent.xconfigure.width) ||
          (mHeight != (uint)rXEvent.xconfigure.height))
      {
        mConfigureCalled = true;
        mWidth = rXEvent.xconfigure.width;
        mHeight = rXEvent.xconfigure.height;
        CallOnResize (mWidth, mHeight);
      }
      break;

    case Expose:
      if (!mInitCalled)
      {
        CallOnCreation();
        mInitCalled = true;
      }
      else
      if (rXEvent.xexpose.count == 0)
      {
        XEvent next_ev;

        // Compress expose events
        while (XEventsQueued(mpDisplay, QueuedAfterReading) > 0)
        {
          XPeekEvent(mpDisplay, &next_ev);
          if (next_ev.type != Expose) break;
          XNextEvent(mpDisplay, &next_ev);
        }

        mWaitExpose = false;
        CallOnPaint();
      }
      break;

    case FocusIn:
      if (!mKeyRepeat)
        XAutoRepeatOff (mpDisplay);
      if (rXEvent.xfocus.mode == NotifyNormal)
      {
        CallOnActivation();
      }
      break;

    case FocusOut:
      if (!mKeyRepeat)
        XAutoRepeatOn (mpDisplay);
      if (rXEvent.xfocus.mode == NotifyNormal)
      {
        CallOnDesactivation();
      }
      break;

    case MapNotify:
      if (mFullScreen)
      {
//        XSetInputFocus (mpDisplay, mWindow, RevertToParent, CurrentTime);
        Grab (true);
      }
      CallOnState (eVisible);
      break;

    case UnmapNotify:
      CallOnState (eHidden);
      if (mFullScreen) Grab (false);
      break;

    case ClientMessage:
      if ((rXEvent.xclient.message_type == mWMProtocol) &&
          ((Atom)rXEvent.xclient.data.l[0] == mWMDelete))
        CallOnClose();
      break;

    case EnterNotify:
    case LeaveNotify:
    case ReparentNotify:
      break; // Unhandled events

#ifdef _DEBUG_
    default:
      NGL_LOG(_T("window"), NGL_LOG_INFO, _T("Unkown X event (type %d)\n"), rXEvent.xany.type);
#endif
  }
}

void nglWindow::DoMouseMove (const XMotionEvent& rEvent)
{
  nglMouseInfo info;

  info.X = rEvent.x;
  info.Y = rEvent.y;
  info.Buttons = GetXButtons(rEvent.state);

  if (mMouseMode == nglMouseInfo::eRelative)
  {
    uint x0, y0;

    // Get window center position in (x0,y0)
    GetSize (x0, y0);
    x0 /= 2;
    y0 /= 2;

    // Get relative motions
    info.X -= mLastMouseX;
    info.Y -= mLastMouseY;

    // Filter dummy events
    if ((info.X == 0) && (info.Y == 0)) return;

    // Update last coordinates
    mLastMouseX = rEvent.x;
    mLastMouseY = rEvent.y;

    // Check if this event was generated by our call to XWarpPointer
    if ((rEvent.x == (int)x0) && (rEvent.y == (int)y0)) return;

    // If the window lost its focus (tricky window managers ...), get grab back
    if (mNeedGrab)
      Grab (true); // This function does the warp-centering job
    else
    {
      XWarpPointer (mpDisplay, None, mWindow, 0, 0, 0, 0, x0, y0);
      XFlush (mpDisplay);
    }
  }
  CallOnMouseMove (info);
}

void nglWindow::DoMouseClick (const XButtonEvent& rEvent)
{
  nglMouseInfo info;

  if (rEvent.button == Button1) info.Buttons = nglMouseInfo::ButtonLeft; else
  if (rEvent.button == Button2) info.Buttons = nglMouseInfo::ButtonMiddle; else
  if (rEvent.button == Button3) info.Buttons = nglMouseInfo::ButtonRight; else
  if (rEvent.button == Button4) info.Buttons = nglMouseInfo::ButtonWheelUp; else
  if (rEvent.button == Button5) info.Buttons = nglMouseInfo::ButtonWheelDown; else
    info.Buttons = nglMouseInfo::ButtonNone;

  if (rEvent.type == ButtonPress)
  {
    if ((rEvent.button == mLastButton) && ((rEvent.time - mLastClick) <= NGL_CLICK_DELAY))
    {
      if (mClickCnt == 0)
      {
        info.Buttons |= nglMouseInfo::ButtonDoubleClick;
        mClickCnt++;
      }
      else
        mClickCnt++;
    }
    else
      mClickCnt = 0;
    mLastClick = rEvent.time;
    mLastButton = rEvent.button;
  }

  switch (mMouseMode)
  {
    case nglMouseInfo::eRelative:
      info.X = 0;
      info.Y = 0;
      break;

    case nglMouseInfo::eAbsolute:
      info.X = rEvent.x;
      info.Y = rEvent.y;
      break;
  }

  if (rEvent.type == ButtonPress)
    CallOnMouseClick (info);
  else
    CallOnMouseUnclick (info);
}

bool nglWindow::DoKeyPress (const XKeyEvent& rEvent)
{
  KeySym keysym;
  char buffer[16];
  int cnt;
  uint keycode;
  nglChar car = 0;

  keycode = gpKeymapXFree86[rEvent.keycode & NGL_KEYMAP_MASK];
  cnt = XLookupString ((XKeyEvent*)&rEvent, buffer, 16, &keysym, &mCompose);
  if (cnt > 0)
  {
    mbstate_t state = { 0 };
    wchar_t wc;

    if (mbrtowc(&wc, buffer, cnt, &state) > 0)
      car = wc;

    if (car == '\r')
      car = '\n';
  }

  nglKeyEvent event(keycode, car, car);
  if (rEvent.type == KeyPress)
    return CallOnKeyDown(event);
  else
    return CallOnKeyUp(event);
}

void nglWindow::SetDeco (uint Deco)  // 0 or MWM_DECOR_ALL
{
  MotifWmHints new_hints;
  MotifWmHints *hints;
  Atom type;
  int format;
  unsigned long nitems;
  unsigned long bytes_after;

  new_hints.flags = MWM_HINTS_DECORATIONS;
  new_hints.decorations = Deco;

  if (!mHintsAtom)
    mHintsAtom = XInternAtom (mpDisplay, "_MOTIF_WM_HINTS", False);

  XGetWindowProperty (mpDisplay, mWindow,
    mHintsAtom, 0, sizeof (MotifWmHints)/sizeof (long),
    False, AnyPropertyType, &type, &format, &nitems,
    &bytes_after, (unsigned char **)&hints);

  if (type == None)
    hints = &new_hints;
  else
  {
    hints->flags |= MWM_HINTS_DECORATIONS;
    hints->decorations = Deco;
  }

  XChangeProperty (mpDisplay, mWindow,
    mHintsAtom, mHintsAtom, 32, PropModeReplace,
    (unsigned char *)hints, sizeof (MotifWmHints)/sizeof (long));

  if (hints != &new_hints) XFree (hints);
}

void nglWindow::Grab(bool DoGrab)
{
  if (DoGrab)
  {
    uint w, h;
    int err;

    GetSize (w, h);
    XWarpPointer (mpDisplay, None, mWindow, 0, 0, w, h, w/2, h/2);
    mLastMouseX = w/2;
    mLastMouseY = h/2;
    XGrabKeyboard (mpDisplay, mWindow, True,
      GrabModeAsync, GrabModeAsync,
      CurrentTime);
    err = XGrabPointer (mpDisplay, mWindow, True,
      NGL_X_MOUSE_EVENTS,
      GrabModeAsync, GrabModeAsync,
      mWindow, None,
      CurrentTime);
    mNeedGrab = (err);
  }
  else
  {
    XUngrabKeyboard (mpDisplay, CurrentTime);
    XUngrabPointer (mpDisplay, CurrentTime);
  }
  XFlush (mpDisplay);
}

nglMouseInfo::Flags nglWindow::GetXButtons(uint Mask) const
{
  nglMouseInfo::Flags flags = nglMouseInfo::ButtonNone;

  if (Mask & Button1Mask) flags |= nglMouseInfo::ButtonLeft;
  if (Mask & Button2Mask) flags |= nglMouseInfo::ButtonMiddle;
  if (Mask & Button3Mask) flags |= nglMouseInfo::ButtonRight;
  if (Mask & Button4Mask) flags |= nglMouseInfo::ButtonWheelUp;
  if (Mask & Button5Mask) flags |= nglMouseInfo::ButtonWheelDown;

  return flags;
}

Cursor nglWindow::GetXCursor(nuiMouseCursor _Cursor)
{
  unsigned int id = 0;

  switch (_Cursor)
  {
    case eCursorNone :
      if (mEmptyCursor == None)
      {
        // Create an empty cursor (used by SetFlags(NoPointer))
        Pixmap pix;
        XColor color; // don't care
        char data = 0;

        pix = XCreateBitmapFromData (mpDisplay, mWindow, &data, 1, 1);
        mEmptyCursor = XCreatePixmapCursor (mpDisplay, pix, pix, &color, &color, 0, 0);
        XFreePixmap (mpDisplay, pix);
      }
      return mEmptyCursor;

    case eCursorArrow    : id = XC_left_ptr; break;
    case eCursorCross    : id = XC_crosshair; break;
    case eCursorHand     : id = XC_hand2; break;
    case eCursorHelp     : id = XC_question_arrow; break;
    case eCursorWait     : id = XC_watch; break;
    case eCursorCaret    : id = XC_xterm; break;
    case eCursorDnD      : id = XC_fleur; break;
    case eCursorForbid   : id = XC_X_cursor; break;
    case eCursorMove     : id = XC_fleur; break;
    case eCursorResize   : id = XC_sizing; break;
    case eCursorResizeNS : id = XC_sb_v_double_arrow; break;
    case eCursorResizeWE : id = XC_sb_h_double_arrow; break;
    case eCursorResizeN  : id = XC_top_side; break;
    case eCursorResizeS  : id = XC_bottom_side; break;
    case eCursorResizeW  : id = XC_left_side; break;
    case eCursorResizeE  : id = XC_right_side; break;
    case eCursorResizeNW : id = XC_top_left_corner; break;
    case eCursorResizeNE : id = XC_top_right_corner; break;
    case eCursorResizeSW : id = XC_bottom_left_corner; break;
    case eCursorResizeSE : id = XC_bottom_right_corner; break;
    case eCursorDoNotSet : break;
    case eCursorIBeam    : break;
    case eCursorClosedHand : break;
  }

  return XCreateFontCursor(mpDisplay, id);
}

void nglWindow::EnterModalState()
{
    NGL_LOG("deb", 0, "Entering modal state");
	App->EnterModalState();
}

void nglWindow::ExitModalState()
{
    NGL_LOG("deb", 0, "Exiting modal state");
	App->ExitModalState();
}

void nglWindow::StartTextInput(int32 X, int32 Y, int32 W, int32 H)
{
  //#FIXME
}

void nglWindow::EndTextInput()
{
  //#FIXME
}

bool nglWindow::IsEnteringText() const
{
  //#FIXME
  return false;
}


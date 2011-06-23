/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"
#include "nglKernel.h"
#include "nglWindow.h"
#include "nuiMainMenu.h"
#include "nuiStopWatch.h"



const nglMouseInfo::Flags nglMouseInfo::ButtonNone        = 0;
const nglMouseInfo::Flags nglMouseInfo::ButtonLeft        = 1 << 0;
const nglMouseInfo::Flags nglMouseInfo::ButtonRight       = 1 << 1;
const nglMouseInfo::Flags nglMouseInfo::ButtonMiddle      = 1 << 2;
const nglMouseInfo::Flags nglMouseInfo::ButtonWheelUp     = 1 << 3;
const nglMouseInfo::Flags nglMouseInfo::ButtonWheelDown   = 1 << 4;
const nglMouseInfo::Flags nglMouseInfo::ButtonWheelLeft   = 1 << 5;
const nglMouseInfo::Flags nglMouseInfo::ButtonWheelRight  = 1 << 6;
const nglMouseInfo::Flags nglMouseInfo::ButtonDoubleClick = 1 << 30;

const nglWindow::Flags nglWindow::NoFlag      = 0;
const nglWindow::Flags nglWindow::NoResize    = 1 << 0;
const nglWindow::Flags nglWindow::NoBorder    = 1 << 1;
const nglWindow::Flags nglWindow::FullScreen  = 1 << 2;

const nglWindow::Flags nglWindow::NoEvents    = 0;
const nglWindow::Flags nglWindow::MouseEvents = 1 << 0;
const nglWindow::Flags nglWindow::KeyEvents   = 1 << 1;
const nglWindow::Flags nglWindow::AllEvents   = nglWindow::MouseEvents | nglWindow::KeyEvents;


/*
 * nglWindowInfo
 */

nglWindowInfo::nglWindowInfo(uint UserWidth, uint UserHeight, bool IsFullScreen)
{
  Pos       = ePosAuto;
  XPos      = 0;
  YPos      = 0;
  Width     = UserWidth;
  Height    = UserHeight;
  MinWidth  = -1;
  MinHeight = -1;
  MaxWidth  = -1;
  MaxHeight = -1;

  Rotate    = 0;
  Flags     = IsFullScreen ? nglWindow::FullScreen : nglWindow::NoFlag;
  Events    = nglWindow::AllEvents;
  Cursor    = eCursorArrow;
  MouseMode = nglMouseInfo::eAbsolute;
  KeyRepeat = true;
  Title     = App->GetName();
}

void nglWindowInfo::Dump (uint Level) const
{
  NGL_LOG(_T("window"), Level, _T("Window info :"));

  NGL_LOG(_T("window"), Level, _T("  title     : '%s'"), Title.GetChars());

  const nglChar* pos = _T("");
  switch (Pos)
  {
    case ePosUser  : pos = _T("user defined"); break;
    case ePosCenter: pos = _T("centered on screen"); break;
    case ePosMouse : pos = _T("centered on mouse pointer"); break;
    case ePosAuto  : pos = _T("auto"); break;
  }
  NGL_LOG(_T("window"), Level, _T("  pos       : %d,%d (creation hint: %s)"), XPos, YPos, pos);
  NGL_LOG(_T("window"), Level, _T("  size      : %dx%d"), Width, Height);

  nglString buffer(_T(""));
  if (Flags & nglWindow::NoResize)   buffer += _T(" NoResize");
  if (Flags & nglWindow::NoBorder)   buffer += _T(" NoBorder");
  if (Flags & nglWindow::FullScreen) buffer += _T(" FullScreen");
  NGL_LOG(_T("window"), Level, _T("  flags     :%s"), buffer.IsEmpty() ? _T(" none") : buffer.GetChars());

  buffer.Wipe();
  if (Events & nglWindow::MouseEvents) buffer += _T(" mouse");
  if (Events & nglWindow::KeyEvents) buffer += _T(" keyboard");
  NGL_LOG(_T("window"), Level, _T("  events    :%s"), buffer.IsEmpty() ? _T(" none") : buffer.GetChars());

  NGL_LOG(_T("window"), Level, _T("  mouse mode: %s"), MouseMode == nglMouseInfo::eAbsolute ? _T("absolute") : _T("relative"));
  NGL_LOG(_T("window"), Level, _T("  key repeat: %s"), YESNO(KeyRepeat));
}


/*
 * Command line parsing
 */

void nglWindow::ParseArgs (nglContextInfo& rContext, nglWindowInfo& rInfo) /* static method */
{
  bool has_size = false;
  int i = 0;

  while (i < App->GetArgCount())
  {
    nglString opt = App->GetArg(i);

    if (opt == _T("--geometry") && (i+1) < App->GetArgCount())
    {
      std::vector<nglString> tokens;

      App->GetArg(i+1).Tokenize(tokens, _T('x'));
      if (tokens.size() >= 2)
      {
        rInfo.Width  = tokens[0].GetInt();
        rInfo.Height = tokens[1].GetInt();
        has_size = true;
        i++;
      }
    }
    else
    if (opt == _T("--fullscreen"))
    {
      rInfo.Flags |= nglWindow::FullScreen;
    }
    else
    if (opt == _T("--help") || opt == _T("-h"))
    {
      NGL_OUT(_T("Window options :\n") \
                 _T("\n") \
                 _T("  --geometry <width>x<height>  window size or fullscreen resolution\n") \
                 _T("  --fullscreen                 go fullscreen (fallback as window mode)\n\n"));
    }

    i++;
  }

  if (!has_size && (rInfo.Flags & nglWindow::FullScreen))
  {
    nglVideoMode current_mode;

    rInfo.Width = current_mode.GetWidth();
    rInfo.Height = current_mode.GetHeight();
  }
}


 /* Default user callbacks (do [almost] nothing)
 */

void nglWindow::OnCreation()
{
}

void nglWindow::OnDestruction()
{
}

void nglWindow::OnActivation()
{
}

void nglWindow::OnDesactivation()
{
}

void nglWindow::OnClose()
{
}

void nglWindow::OnPaint()
{
}

void nglWindow::OnState (StateInfo State)
{
}

void nglWindow::OnResize (uint Width, uint Height)
{
}

void nglWindow::OnTextCompositionStarted()
{
  
}

void nglWindow::OnTextCompositionConfirmed()
{
  
}

void nglWindow::OnTextCompositionCanceled()
{
  
}

void nglWindow::OnTextCompositionUpdated(const nglString& rString, int32 CursorPosition)
{
  
}

nglString nglWindow::OnGetTextComposition() const
{
  return nglString::Null;
}

void nglWindow::OnTextCompositionIndexToPoint(int32 CursorPosition, float& x, float& y) const
{
  x = y = 0;
}

bool nglWindow::OnTextInput(const nglString& rUnicodeTextInput)
{
  return false;
}

void nglWindow::OnTextInputCancelled()
{
}

bool nglWindow::OnKeyDown(const nglKeyEvent& rEvent)
{
  return false;
}

bool nglWindow::OnKeyUp (const nglKeyEvent& rEvent)
{
  return false;
}


bool nglWindow::OnMouseClick (nglMouseInfo& rInfo)
{
	return false;
}

bool nglWindow::OnMouseUnclick (nglMouseInfo& rInfo)
{
	return false;
}

bool nglWindow::OnMouseMove (nglMouseInfo& rInfo)
{
	return false;
}

bool nglWindow::OnRotation(uint Angle)
{
  return true;
}

void nglWindow::EnableAutoRotation(bool set)
{
  mAutoRotate = set;
}

bool nglWindow::GetAutoRotation() const
{
  return mAutoRotate;
}


/*
 * Internal callbacks
 */

void nglWindow::CallOnCreation()
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Creation\n")); )
  // Dump the context:
  Dump(0);
  // Set all keys as not pressed
  memset (mpKeyState, 0, sizeof(bool)*NGL_KEY_MAX);
  mFPSCount = 0;
  mFPS = 0;

  OnCreation();
}

void nglWindow::CallOnDestruction()
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Destruction\n")); )
  OnDestruction();
}

void nglWindow::CallOnActivation()
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Activation\n")); )
  OnActivation();
}

void nglWindow::CallOnDesactivation()
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Desactivation\n")); )
  // Keyboard keys are released when loosing focus
  memset (mpKeyState, 0, sizeof(bool)*NGL_KEY_MAX);
  OnDesactivation();
}

void nglWindow::CallOnClose()
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Close\n")); )
  OnClose();
}

void nglWindow::CallOnPaint()
{
  //nuiStopWatch watch(_T("nglWindow::CallOnPaint"));
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Paint\n")); )
  OnPaint();

  nglTime now;
  mFPSCount++;
  if (now - mFPSDelay > 1.0)
  {
    double v = (now - mFPSDelay);
    double c = mFPSCount;
    mFPS = c / v;
    NGL_LOG(_T("fps"), NGL_LOG_DEBUG, _T("FPS: %f (%f seconds - %d frames)\n"), mFPS, v, ToNearest(c));

    mFPSCount = 0;
    mFPSDelay = now;
  }
  mLastPaint = now;
}

void nglWindow::CallOnState (StateInfo State)
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("State: %s"), (State == eVisible) ? _T("visible") : _T("hidden")); )
  OnState (State);
}

void nglWindow::CallOnResize (uint Width, uint Height)
{
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Resize: %dx%d"), Width, Height); )
  OnResize (Width, Height);
#ifdef _COCOA_
  mWidth = Width;
  mHeight = Height;
#endif
}

bool nglWindow::CallOnKeyDown (const nglKeyEvent& rEvent)
{
#ifdef _DEBUG_
  //NGL_LOG(_T("window"), NGL_LOG_DEBUG, isprint(Char) ? _T("KeyDn: code=%d char='%c'") : _T("KeyDn: code=%d char=%d"), Key, Char);
#endif // _DEBUG_
  if (rEvent.mKey > 0)
    mpKeyState[rEvent.mKey-1] = true;
  return OnKeyDown (rEvent);
}

bool nglWindow::CallOnKeyUp (const nglKeyEvent& rEvent)
{
#ifdef _DEBUG_
  //NGL_LOG(_T("window"), NGL_LOG_DEBUG, isprint(Char) ? _T("KeyUp: code=%d char='%c'") : _T("KeyDn: code=%d char=%d"), Key, Char);
#endif // _DEBUG_
  if (rEvent.mKey > 0)
    mpKeyState[rEvent.mKey-1] = false;
  return OnKeyUp (rEvent);
}

bool nglWindow::CallOnTextInput (const nglString& rUnicodeString)
{
#ifdef _DEBUG_
  NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("TextInput: '%s' [%d chars]"), rUnicodeString.GetChars(), rUnicodeString.GetLength());
#endif // _DEBUG_
  return OnTextInput(rUnicodeString);
}

void nglWindow::CallOnTextInputCancelled ()
{
#ifdef _DEBUG_
  NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("TextInputCancelled"));
#endif // _DEBUG_
  OnTextInputCancelled();
}

bool nglWindow::CallOnMouseClick (nglMouseInfo& rInfo)
{
  //nuiStopWatch watch(_T("nglWindow::CallOnMouseClick"));
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Clic: %d,%d  button=%x"), rInfo.X, rInfo.Y, rInfo.Buttons); )
  return OnMouseClick (rInfo);
}

bool nglWindow::CallOnMouseUnclick (nglMouseInfo& rInfo)
{
  //nuiStopWatch watch(_T("nglWindow::CallOnMouseUnclick"));
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Unclic: %d,%d  button=%x"), rInfo.X, rInfo.Y, rInfo.Buttons); )
  return OnMouseUnclick (rInfo);
}

bool nglWindow::CallOnMouseMove (nglMouseInfo& rInfo)
{
  //nuiStopWatch watch(_T("nglWindow::CallOnMouseMove"));
  NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Motion: %d,%d"), rInfo.X, rInfo.Y); )
  return OnMouseMove (rInfo);
}

bool nglWindow::CallOnRotation(uint Angle)
{
  //nuiStopWatch watch(_T("nglWindow::CallOnRotation"));
  //NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_DEBUG, _T("Rotation: %d"), Angle); )
  if (GetAutoRotation())
  {
    if (OnRotation(Angle))
    {
      SetRotation(Angle);
      return true;
    }
  }
  return false;
}

void nglWindow::SetMainMenu(nuiMainMenu* pMenu)
{
  mpMainMenu = pMenu;
  mpMainMenu->RegisterFromWindow(this);
}

nuiMainMenu* nglWindow::GetMainMenu()
{
  return mpMainMenu;
}


bool nglWindow::IsInModalState()
{
  return (mInModalState != 0);
}


/*
 * Geometry
 */

uint nglWindow::GetRotation() const
{
  return mAngle;
}

void nglWindow::SetRotation(uint Angle)
{
  mAngle = Angle;
  Invalidate();
}


/*
 * From nglError
 */

extern const nglChar* gpWindowErrorTable[];

const nglChar* nglWindow::OnError (uint& rError) const
{
  return FetchError(gpWindowErrorTable, nglContext::OnError(rError), rError);
}

void nglWindow::ForceRepaint()
{
  CallOnPaint();
}

std::vector<nglWindow*> nglWindow::mpWindows;

void nglWindow::Register()
{
  mpWindows.push_back(this);
}

void nglWindow::Unregister()
{
  for (uint32 i = 0; i < mpWindows.size(); i++)
  {
    if (mpWindows[i] == this)
    {
      std::vector<nglWindow*>::iterator it = mpWindows.begin() + i;
      mpWindows.erase(it);
      return;
    }
  }

  // We should always be able to unregister a window!
  NGL_ASSERT(0);
}


void nglWindow::DestroyAllWindows()
{
  std::vector<nglWindow*> wins(mpWindows);
  std::reverse(wins.begin(), wins.end());
  for (int32 i = 0; i < mpWindows.size(); i++)
  {
    delete wins[i];
  }
}


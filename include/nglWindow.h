/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglWindow.h
\brief OpenGL viewport and rendering management

This class is not available if the _NOGFX_ symbol is defined.
*/

#ifndef __nglWindow_h__
#define __nglWindow_h__

//#include "nui.h"

#ifndef _NOGFX_

#include "nglError.h"
#include "nglKeyboard.h"
#include "nglString.h"
#include "nglContext.h"
#include "nglVideoMode.h"
#include "nuiMouseCursor.h"

#ifndef _NODND_
#include "nglDragAndDropObjects.h"
#endif

#ifdef _MULTI_TOUCHES_
typedef int nglTouchId;
#endif

class nuiMainMenu;


class NGL_API nglMouseInfo  ///< Mouse status description
{
public:
  enum Mode     ///< Select the metrics returned by mouse events (relative vs. absolute)
  {
    eAbsolute,  ///< Use absolute coordinates (ie. screen relative)
    eRelative   ///< Use relative coordinates (ie. mouse motions)
  };

  typedef uint Flags;  ///< Mouse buttons flags

  static const Flags ButtonNone;        ///< No button pressed, can be returned by GetMouse()
  static const Flags ButtonLeft;        ///< Left button pressed, this is the only button on most Mac
  static const Flags ButtonRight;       ///< Right button pressed
  static const Flags ButtonMiddle;      ///< Midle button pressed
  static const Flags ButtonWheelUp;     ///< Mouse wheel went one step 'up'
  static const Flags ButtonWheelDown;   ///< Mouse wheel went one step 'down'
  static const Flags ButtonWheelLeft;   ///< Mouse wheel went one step 'left'
  static const Flags ButtonWheelRight;  ///< Mouse wheel went one step 'right'
  static const Flags ButtonDoubleClick; ///< The current button has been hit consecutively twice in a short delay

  int   X;        ///< Horizontal coordinate
  int   Y;        ///< Vertical coordinate
  Flags Buttons;  ///< Buttons state

#ifdef _MULTI_TOUCHES_
  nglTouchId    mTouchId; ///< used to retrieve which finger acting
#endif

#ifdef _UIKIT_
  // Handle  specific touch screen events
  typedef enum
  {
    eNoSwipe = 0,
    eSwipeUp = 1,
    eSwipeDown = 2,
    eSwipeLeft = 4,
    eSwipeRight = 8
  } SwipeDirection;

  SwipeDirection SwipeInfo;
#endif //_UIKIT_
};


class nglWindowInfo;


/*
 * Error codes
 */
#define NGL_WINDOW_ENONE 0  ///< There is currently no error


//! OpenGL viewport and rendering management
/*!
A nglWindow gives you an OpenGL context for your GL work and a rendering window
for this context. You \e must instantiate a nglWindow before calling any OpenGL
functions. Since you can create any number of windows in an application, you
must specify the context of your OpenGL code : even if you have only one window,
enclose the OpenGL calls between BeginSession() and EndSession() calls.

The EndSession() method implicitly finishes the rendering (calling
\c glFinish()) and triggers a buffer swap if the window is multi-buffered.
If you render to multiple windows in parallel, it is legal to make successive
calls to BeginSession() without calling the corresponding EndSession(). In
other words, you can interleave BeginSession() and EndSession() calls at will.

Creating a window requires two pieces of information : OpenGL context info and
general window properties (geometry and behaviour). The first info is given
by the nglContextInfo object, while the second one is simply contained in a
nglWindowInfo object.

Choosing an OpenGL context is not necessarily an easy task, and mainly not a very
portable job. Keep in mind that the features you ask and get on your
configuration might not (and won't) be available on every user's graphics card
and OpenGL driver combination. Please have a closer look at the nglContextInfo
documentation for this purpose.

Filling the nglWindowInfo is pretty simple. Note that you can change most of
the properties later with various methods (excepted the Pos initial position
hint, which only makes sense at construction time).

When you just created a new nglWindow instance, you should do in this order :

- Check creation success with GetError(). There are many reasons why
a window won't spawn at run time, even if the context and global infos are
valid. The error is non-portable, meaning you cannot interpret the error code.
You should only check against NGL_WINDOW_ENONE, and report the error (if any)
with GetErrorStr(). If you encounter an error, you should delete
the nglWindow instance. An invalid nglWindow will silently fail on any call
on one of its method, and won't generate any events.

- Display the window with a call to SetState(nglWindow::eShow). You can also
display a window in its minimized state with nglWindow::eMinimize. As a default,
newly created window are in the nglWindow::eHidden state.

To effectively destroy the window and release the OpenGL context, simply
delete the object.
*/

class NGL_API nglWindow : public nglContext
{
public:
  enum StateChange  ///< See SetState()
  {
    eHide,     ///< Withdraw the window, including its icon on the desktop or in the taskbar
    eShow,     ///< Display a hidden or minize window
    eMinimize,  ///< Minimize a hidden or displayed window
    eMaximize  ///< Maximize a hidden or displayed window
  };

  enum StateInfo  ///< See GetState()
  {
    eHidden,   ///< The window is not visible (might be minimized or withdrawn)
    eVisible   ///< The window is displayed on the screen
  };


  class OSInfo ///< OS-specific properties
  {
  public:
#ifdef _WIN32_
    HWND Parent;        // Parent window handle. If NULL, creates a regular toplevel window.
    HWND WindowHandle;  // Window handle

    OSInfo();
#endif // _WIN32_

#ifdef _CARBON_
    WindowRef WindowHandle;  // Parent window handle. If NULL, creates a regular toplevel window.
    WindowRef Parent;  // Parent window handle. If NULL, creates a regular toplevel window.

    OSInfo();
#endif // _CARBON_

#ifdef _UIKIT_
    void* mpUIWindow; // Pointer to the nglUIWindow (Based on UIWindow from UIKit).
    OSInfo();
#endif
  };

  typedef uint Flags;      ///< Used during nglWindow construction, see nglWindowInfo
  typedef uint EventMask;  ///< Controls the reception of keyboard and mouse events, see SetEventMask()


  /*! Use defaults (all flags unset) */
  static const Flags NoFlag;

  /*! The window manager will prevent the user from resizing the window,
      however the programmer can still use nglWindow::SetSize with success. */
  static const Flags NoResize;

  /*! The window will have no decorations at all; often used for so called
      'splash screens'. This type of window is implicitly not user-resizable
      however the NoResize flag can stay (independently) unset. */
  static const Flags NoBorder;         

  /*! This window is full screen (a video mode switch occurs if necessary) */
  static const Flags FullScreen;       


  /*! Disable all events */
  static const EventMask NoEvents;

  /*! Enable mouse events : OnMouseClick(), OnMouseUnclick() and OnMouseMove()
      callbacks are controlled by this flag. */
  static const EventMask MouseEvents;  

  /*! Enable key events : OnKeyUp() and OnKeyDown() callbacks are controlled by
      this flag. */
  static const EventMask KeyEvents;    

  /*! Enable all events */
  static const EventMask AllEvents;


  static void ParseArgs (nglContextInfo& rContext, nglWindowInfo& rInfo);
  /*!< Command line options parser
    \param rContext is updated with a proper nglContext description
    \param rInfo is updated with a proper nglWindow description

    This method will parse some predefined arguments from the command line, and automatically
    fill in sensible values for a context and a window description. These options will be
    supported by your application :

    - <i>--geometry [width]x[height]</i> : set window (or fullscreen) resolution

    - <i>--fullscreen</i> : go fullscreen. Use current desktop resolution if <i>--size</i>
      is not present
  */


  /** @name Life cycle */
  //@{
  nglWindow (uint Width, uint Height, bool IsFullScreen = false);
  /*!<
    Simple constructor for the lazy ones.
    \param Width window width
    \param Height window height
    \param IsFullScreen if true, tries to go fullscreen using width*height as resolution

    This is equivalent to the following code :

\code
nglWindow*     window;
nglContextInfo context; // Fetch default context
nglWindowInfo  info;

info.Width  = Width;
info.Height = Height;
info.Flags  = IsFullScreen ? FullScreen : NoFlag;

window = new nglWindow (context, info, NULL);
\endcode

    See nglContextInfo and nglWindowInfo for default settings.

    \b Important : do not put your window init code in your inherited class
    constructor, use the OnCreation() callback for this purpose. The reason
    being that the system objects might not be ready for use from the constructor.
  */

  nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared = NULL);
  /*!<
    Full constructor for the mighty ones.
    \param rContext OpenGL context info, see nglContextInfo
    \param rInfo   Window options, see nglWindowInfo
    \param pShared Share the OpenGL context with *pShared's one.

    if \c pShared is non null, the newly created context will be shared with
    \c *pShared's one. Both windows will have distinct contexts working
    in parallel, but will share display lists and texture names.
  */

  virtual ~nglWindow();
  /*!<
    Immediatly destroy the window and release the OpenGL context.

    \b Important : you should put your window destroy code in the
    OnDestruction() callback rather than in the destructor. This callback
    will be called event if a delete is issued on the nglWindow object.
  */
  //@}

  /** @name Geometry */
  //@{
  uint GetWidth() const;   ///< Return the current (user area) width
  uint GetHeight() const;  ///< Return the current (user area) height
  void GetSize (uint& rWidth, uint& rHeight) const;
  /*!<
    Get the window (user area) size.
    \param rWidth current width
    \param rHeight current height
  */
  bool SetSize (uint Width, uint Height);
  /*!<
    Sets the window user area size.
    \param Width new width
    \param Height new height
    \return true if the window was resized

    In window'ed mode, \a Width and \a Height are always clamped with a minimum
    value of 1. If the window has been succesfuly resized, returns true.

    In fullscreen mode, this method is ignored.
  */
  void GetPosition (int& rXPos, int& rYPos) const;
  /*!<
    Get the window (user area) position in screen coordinate space.
    \param rXPos position abscissa
    \param rYPos position ordinate
  */
  bool SetPosition (int XPos, int YPos);
  /*!<
    Set the window (user area's top left corner) position in screen coordinate space.
    \param XPos position abscissa
    \param YPos position ordinate
    \return true if the window position was updated
  */
  bool GetResolution (float& rHorizontal, float& rVertical) const;
  /*!<
    Retrieve screen resolution in DPI (dots per inch).
    \param rHorizontal horizontal resolution in DPI
    \param rVertical vertical resolution in DPI
    \return true if \p rHorizontal and \p rVertical contain valid information
  */
  uint GetRotation() const;           ///< Return the current (user area) rotation angle
  virtual void SetRotation(uint Angle); ///< Set the current (user area) rotation angle
  void EnableAutoRotation(bool set); ///< Change the rotation and size of the screen to follow the device's screen orientation (this is the default behaviour).
  bool GetAutoRotation() const; ///< Change the rotation and size of the screen to follow the device's screen orientation (this is the default behaviour).
  //@}


  /** @name Appearance */
  //@{
  StateInfo GetState() const;
  /*!<
    \return the current window display status
  */
  void SetState (StateChange State);
  /*!<
    Change the window display state.
    \param State new state

    All state transitions are supported. For instance, since a window is
    initially created in the \a eHidden mode, it is possible to
    show it directly in minimized mode : simply ask the \a eMinimize
    mode.

    If the window is 'fullscreen', minimizing or withdrawing the window will
    restore the default desktop resolution, while showing it will switch back
    to the right video mode and restore the fullscreen mode.
  */

  nglString GetTitle() const;
  /*!<
    \return the window current title.
  */
  void SetTitle (const nglString& rTitle);
  /*!<
    Sets the window current title.
    \param rTitle new title

    It is legal to pass an empty string (""), the window will have no title.
  */
  
  void SetTitleBarVisible(bool visible);

  nuiMouseCursor GetCursor() const;
  /*!<
    \return the current mouse cursor model
  */
  bool SetCursor(nuiMouseCursor Cursor);
  /*!<
    Sets the window current cursor
    \param Cursor new cursor
    \return true if the cursor was succesfully changed
  */
  //@}

  /** @name Events control */
  //@{
  EventMask GetEventMask() const;
  /*!<
    \return the current event mask
  */
  void SetEventMask (EventMask Events);
  /*!<
    Sets a new event mask
    \param Events event mask (combination of EventMask constants)
  */
  //@}

  /** @name Extension */
  //@{
  const OSInfo* GetOSInfo() const;
  /*!<
    \return OS-specific informations (eg. for embedding purposes).
  */
  //@}

  /** @name Mouse */
  //@{
  nglMouseInfo::Mode GetMouseMode() const;
  /*!<
    \return mouse mode
  */
  void SetMouseMode (nglMouseInfo::Mode Mode);
  /*!<
    Sets current mouse mode
    \param Mode new mouse mode
  */

  void GetMouse (nglMouseInfo &rInfo, bool Local = true) const;
  /*!<
    Get the current mouse position and button state.
    \param rInfo mouse information holder
    \param Local if true, returns coordinate relative to the top left corner of the window client area;
           If false, returns the mouse coordinates relative to the top left corner of the screen

    If the mouse mode is \a eMouseRelative, the coordinates will always be both zero
    (only the OnMouseMove() callback can provide relative coordinates); the rInfo.Flags
    state is updated in any case.

    Any number of button flags can be set in the rInfo.Buttons bit mask, except
    \a ButtonDoubleClick which does not make sense in this context.
  */
  //@}

  /** @name Keyboard */
  //@{
  bool GetKeyRepeat() const;
  /*!<
    \return true if auto key-repeat is active
  */
  void SetKeyRepeat (bool AllowRepeat = true);
  /*!<
    Sets key repeat mode
    \param AllowRepeat if false, disable auto key repeat for this window
  */

  bool IsKeyDown (nglKeyCode rEvent) const;
  /*!<
    Test a physical key state.
    \param Key key code
    \return key status, true if currently pressed

    See the <a href="keyboard.html">nglKeyboard layouts description</a> for physical key names.

    The key code cannot be converted to a printable character, since it would
    require the key context (wether a modifier like Alt, Ctl or Apple was hold,
    or a more complex composing method such as used on asiatic layouts). Use
    the OnKeyDown() callback if you want to interpret key strokes for text
    composing purposes.

    When a window looses its focus (see OnDesactivation()), all key states are
    reset (the keys are virtually released at once).
  */
  //@}

  static const nglChar* GetKeyName (nglKeyCode rEvent);
  /*!<
    Retrieve the human readable name of a physical key
    \param Key key code
    \return key name

    See nglKeyboard.h and the <a href="keyboard.html">nglKeyboard layouts description</a>.
  */


  /** @name Rendering */
  //@{
  virtual void BeginSession();
  /*!<
    Set the window OpenGL context as the current context. All further OpenGL
    calls will refer to this window context.
  */

  virtual bool MakeCurrent() const;
  /*!<
    Set the window OpenGL context as the current context. All further OpenGL
    calls will refer to this window context.
  */


  virtual void EndSession();
  /*!<
    Finish the rendering on the current OpenGL context (implicitly calls
    \a glFinish()) and swap frame buffers if appropriate).
  */
  void Invalidate();
  /*!<
    Append a paint event to the event queue. It means the OnPaint() callback
    will be called as soon as possible when you give the control back to the
    main loop.
    
    Only one paint event can be queued. Hence you can call Invalidate()
    many times before returning from an event handler, it will only trigger
    one paint event.
  */
  //@}

  /** @name User callbacks */
  //@{
  virtual void OnCreation();
  /*!<
    This method is called when the window object creation succeeded.
    You should put your init code here, and \e not in the constructor.
  */
  virtual void OnDestruction();
  /*!<
    This method is called when the window object is destroyed.
    You should put your destroy code here, and \e not in the destructor.
  */
  virtual void OnActivation();
  /*!<
    This method is called when the window gets the focus, meaning it
    can receive keyboard and mouse events.
  */
  virtual void OnDesactivation();
  /*!<
    This method is called when the window looses the focus, meaning it
    won't receive further keyboard and mouse events.
  */
  virtual void OnClose();
  /*!<
    This method is called when the user wants to close a displayed or
    iconified window.
    
    The window will not be automatically destroyed, it is up to you to decide
    its fate. It is legal to call \c delete \c this, but don't forget to
    signal the holders of a pointer to this object that it is no longer
    available.
  */
  virtual void OnPaint();
  /*!<
    At least a part of the window must be redrawn. The window might have been
    explicitely invalidated by the programmer (see Invalidate()).
  */
  virtual void OnState (StateInfo State);
  /*!<
    This method is called when the window display state changes.
    \param State new display state

    State can be either eHidden (the window has been withdrawn or
    iconified) or eVisible (the window has been unhidden or
    un-iconified).
  */
  virtual void OnResize (uint Width, uint Height);
  /*!<
    This method is called when the user resizes the window.
    \param Width new width
    \param Height new height

    This method is only called if the NoResize flag is set or the programmer
    explicitely called SetSize().
  */
  virtual bool OnTextInput(const nglString& rUnicodeTextInput);
  /*!<
   This method is called when the user enters text input into the program. This is a high level method that takes a unicode character string parameter.
   Use OnKeyDown and OnKeyUp if you want to handle low level keycodes (cursor keys, return, backspace, delete, etc.).
   \param rUnicodeTextInput an nglString containing the input from the user. Beware that this string may contain more than one characters.
   \return true is the key was handled and false otherwise. If the key wasn't handled it is potentially sent back to the system or hosting application.
   */
  virtual bool OnKeyDown(const nglKeyEvent& rEvent);
  /*!<
  This method is called when a key is pressed. Use this method only to get Low Level keyboard input: if you are making a game or to catch dead keys/system keys (cursor keys).
  Using this method for text input will break unicode text input for complex scripts and non latin languages such as Japanese. Use OnTextInput instead.
  \param rEvent this object contains the info about the key that was depressed
  \return true is the key was handled and false otherwise. If the key wasn't handled it is potentially sent back to the system or hosting application.

  The key code refers to a language and layout independent code. This is
  useful to specify keys that are located physically on a known place
  on the keyboard. See <a href="keyboard.html">nglKeyboard layout>keyboard layouts</a>.
  */
  virtual bool OnKeyUp (const nglKeyEvent& rEvent);
  /*!<
   This method is called when a key is released.  Use this method only to get Low Level keyboard input: if you are making a game or to catch dead keys/system keys (cursor keys).
   Using this method for text input will break unicode text input for complex scripts and non latin languages such as Japanese. Use OnTextInput instead.
  \param rEvent this object contains the info about the key that was depressed
  \return true is the key was handled and false otherwise. If the key wasn't handled it is potentially sent back to the system or hosting application.

  See OnKeyDown() documentation.
  */
  virtual bool OnMouseClick (nglMouseInfo& rInfo);
  /*!<
    This method is called as soon as a mouse button is pressed.
    \param rInfo mouse status
		\return true is the mouse event was handled and false otherwise. If the key wasn't handled it is potentially sent back to the system or hosting application.

    See GetMouse() for X and Y values exact interpretation.

    \a ButtonDoubleClick and \a ButtonTripleClick bits might be set in \a aButton,
    together with the hit button flag.
  */
  virtual bool OnMouseUnclick (nglMouseInfo& rInfo);
  /*!<
    This method is called when a mouse button is released.
    \param rInfo mouse status
		\return true is the mouse event was handled and false otherwise. If the key wasn't handled it is potentially sent back to the system or hosting application.

    See GetMouse() for X and Y values exact interpretation.

    \a ButtonDoubleClick and \a ButtonTripleClick bits cannot be set in \a Buttons.
  */
  virtual bool OnMouseMove (nglMouseInfo& rInfo);
  /*!<
    This method is called when a mouse motion is detected.
    \param rInfo mouse status
		\return true is the mouse event was handled and false otherwise. If the key wasn't handled it is potentially sent back to the system or hosting application.

    The X and Y values are displacement measures when the \a eRelative flag
    is set, or window relative coordinates otherwise (the top left corner of
    the window is at 0, 0). You can get the current mouse buttons state with
    the GetMouse() method.

    This event is only triggered if the window has the focus (ie. has received
    a OnActivation() event).
  */
  virtual bool OnRotation(uint Angle);
  /*!<
   This method is called when a rotation of the device is detected.
   \param Angle
   \return true is accepting the new Angle
  */
  //@}


#ifndef _NODND_

  /** @name Drag and drop */
  //@{

  /*!<
   This method is called by the system whenever the mouse is dragging an object over the window. 
   This method should test objects for supported drag and drop types and return true if one of them is supported, false otherwise.
   This method also should fill the effect flag of the drop by calling nglDragAndDrop::SetDesiredEffect
   */
  virtual bool Drag(nglDragAndDrop* pDragObject); ///< Starts a Drag operation, pDragObject should have its types set.
  
  virtual void OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType); ///< called whenever the system request data for a drop operation after BeginDragging
  virtual void OnDragStop(bool canceled); ///< called when a drag operation is interupted or finished
  
  
  virtual void OnDragEnter();
  virtual void OnDragLeave();
  virtual bool OnCanDrop(nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button);
  
  /*!<
   This method is called whenever the mouse is dropping an object on the application window. 
   This method should retreive the dropped data from the system for supported drag and drop types by the widget under it.
   The mouse flag argument contains the mouse event that initiated the drop operation. 
   X & Y are the coordinates of the mouse inside the main window.
   */
  virtual void OnDropped(nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button);
  
  
  //@}

#endif//_NODND_

  void EnterModalState();
  bool IsInModalState();
  void ExitModalState();

  void ForceRepaint(); ///< Force the window to call OnPaint and to repaint itself NOW. Unlike Invalidate(), this call is blocking and returns only when the window has been repaint. Beware, most application don't need this!
  
  void SetMainMenu(nuiMainMenu* pMenu); ///< associate a nuiMainMenu to this window, to perform automatic deleting
  nuiMainMenu* GetMainMenu();
  
protected:
  virtual const nglChar* OnError (uint& rError) const;
  
  

private:
  OSInfo             mOSInfo;
  Flags              mFlags;
  EventMask          mEventMask;
  nuiMouseCursor     mCursor;
  nglMouseInfo::Mode mMouseMode;
  bool               mKeyRepeat;
  bool               mpKeyState[NGL_KEY_MAX];
  uint32             mInModalState;
  uint               mAngle;
  bool               mAutoRotate;

  nglWindow(const nglWindow&) {} // Undefined copy constructor

#if (defined _UIKIT_) || (defined _COCOA_)
  ///< #HACK: To be called from ObjectiveC interfaces
public:
#endif
  void CallOnCreation();
  void CallOnDestruction();
  void CallOnActivation();
  void CallOnDesactivation();
  void CallOnClose();
  void CallOnPaint();
  void CallOnState      (StateInfo State);
  void CallOnResize     (uint Width, uint Height);
  bool CallOnTextInput  (const nglString& rUnicodeString);
  bool CallOnKeyDown    (const nglKeyEvent& rEvent);
  bool CallOnKeyUp      (const nglKeyEvent& rEvent);
  bool CallOnMouseClick (nglMouseInfo& rInfo);
  bool CallOnMouseUnclick(nglMouseInfo& rInfo);
  bool CallOnMouseMove  (nglMouseInfo& rInfo);

#ifdef _X11_
public:
  Window         GetHandle();
  void           OnXEvent (const XEvent& rXEvent);

private:
  int            mScreen;
  GC             mGC;
  Colormap       mColormap;
  Atom           mWMProtocol;
  Atom           mWMDelete;
  Atom           mHintsAtom;
  Window         mWindow;
  Cursor         mXCursor;
  Cursor         mEmptyCursor;
  XComposeStatus mCompose;
  bool           mInitCalled;
  bool           mConfigureCalled;
  bool           mIsHidden;
  bool           mNeedGrab;
  bool           mWaitExpose;
  int            mXPos, mYPos;
  uint           mWidth, mHeight;
  int            mLastMouseX, mLastMouseY;
  bool           mWasMapped;
  int            mLastMappedX, mLastMappedY;
  Time           mLastClick;
  uint           mLastButton;
  int            mClickCnt;
  nglVideoMode   mSavedVideoMode;
  nglVideoMode   mVideoMode;
  bool           mFullScreen;

  void InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared);
  void DoMouseMove  (const XMotionEvent& rEvent);
  void DoMouseClick (const XButtonEvent& rEvent);
  bool DoKeyPress   (const XKeyEvent& rEvent);
  void SetDeco      (uint Deco);
  void Grab         (bool DoGrab);
  nglMouseInfo::Flags GetXButtons (uint Mask) const;
  Cursor              GetXCursor (nuiMouseCursor Cursor);
#endif //_X11_

#ifdef _CARBON_
private:

	friend OSErr nglDragReceiveHandler(WindowRef theWindow, void * handlerRefCon, DragRef theDrag);
	friend OSErr nglDragTrackingHandler (DragTrackingMessage message, WindowRef theWindow, void * handlerRefCon, DragRef theDrag);

  bool           mInited;
  nglString      mTitle;
  nglContext*    mpContext;
  nglVideoMode*  mpLastVMode;
  nglVideoMode*  mpVMode;
  WindowRef      mWindow;
  uint32         mModifiers; // Dead Keys (Modifiers Keys)
  bool           mInvalidatePosted;
  bool           mRedrawing;
  bool           mIsFakeChildWindow;
  int            mXOffset, mYOffset, mWindowWidth, mWindowHeight; //< those values are only valid in the case of a fake child window
  nglMouseInfo::Flags mLastButtonDown;
  

  EventLoopTimerRef mTimer;
  EventHandlerRef   mEventHandlerRef;
  EventHandlerRef   mKeyboardEventHandlerRef;
  friend pascal void nglWindowTimerAction(EventLoopTimerRef theTimer, void* userData);
  void TimerAction();  
  void InitDragAndDrop();
  
  void InternalInit (const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglContext* pShared);
  OSStatus WindowEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
  OSStatus WindowKeyboardEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
  void OnLoopInit   ();

  friend pascal OSStatus nglWindowEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
  friend pascal OSStatus nglWindowKeyboardEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
  friend class nglApplication;
#endif //_CARBON_

#ifdef _CARBON_
class nglCarbonDragAndDrop* mpCarbonDragAndDrop;
#endif //_CARBON_

#ifdef _UIKIT_
private:
  uint mWidth, mHeight;
  void InternalInit(const nglContextInfo& rContext, const nglWindowInfo& rInfo,
                    const nglContext* pShared);

  int32 mInSession;
  void* mpUIWindow;
  GLuint mRenderBuffer;
  GLuint mFrameBuffer;
  GLuint mDepthBuffer;

#endif

#ifdef _WIN32_
private:
#ifdef _NGL_WINDOWS_HACKS_ // Define _NGL_WINDOWS_HACKS_ if you want to be able to hack the windows. Highly dangerous!!!
protected:
#endif
  static ATOM mAtom;

  HWND          mHWnd;
  static bool   mClassDefined;
  nglVideoMode  mSavedVideoMode;
  StateInfo     mState;
  bool          mFullScreen;
  int           mOldMouseX;
  int           mOldMouseY;
  nglMouseInfo::Flags mMouseButtonStatus;

  bool InternalInit(const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared);
  void SetMouseButtonStatus(nglMouseInfo::Flags button);
  void UnsetMouseButtonStatus(nglMouseInfo::Flags button);
  bool DoKey         (bool IsUp, WPARAM wParam, LPARAM lParam);
  void DoMouseUnclick(nglMouseInfo::Flags Flags, HWND hWnd, WPARAM wParam, LPARAM lParam);
  void DoMouseClick  (nglMouseInfo::Flags Flags, HWND hWnd, WPARAM wParam, LPARAM lParam);
  HCURSOR GetCursorIDC (nuiMouseCursor Cursor);

  static void Exit(); ///< Called to unregister the windows class.

  LRESULT WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
  friend LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
  friend LRESULT CALLBACK KbdMsgProc(int code, WPARAM wParam, LPARAM lParam);

#endif // _WIN32_

#ifdef _WIN32_ // Win32 Drag And drop Support
  friend class          nglDropTarget;

  class nglDropTarget*  mpDropTarget;
  class nglDropSource*  mpDropSource;
  bool  mOnDragging;
  UINT  mDragMessageId;
  class nglPBuffer* mpPBuffer;
  bool mPBufferDisabled;
#endif//_WIN32_

  GLuint mFBO_BackBuffer;
  GLuint mFBO_BackBufferTexture;
  GLuint mFBO_BackBufferStencil;
  GLuint mFBO_BackBufferDepth;
  GLint mBackBufferTexture;
  int32 mBackTextureWidth;
  int32 mBackTextureHeight;
  bool mSkipBackBufferCopy;
  
  nuiMainMenu* mpMainMenu;
};


//! Window properties for nglWindow constructor
/*!
This structure is only used by the nglWindow constructor to provide the initial
properties of the window. See also nglContextInfo which is used in conjunction
with nglWindowInfo to create a nglWindow.

The \a Pos field is mandatory, and sets the initial positioning strategy.

The \a XPos and \a YPos items are only relevant if \a Pos is set to
\a ePosUser. They are position coordinates in the user's desktop space,
(0, 0) being the top left corner. You can set a position outside the desktop
(values greater than the desktop's own resolution or negative), however
this is generally not a good idea : platforms might behave differently, some
will simply preventing you from positioning a window outside the desktop.

The \a Width and \a Height fields are mandatory. They let you set the size
of the OpenGL rendering surface. These parameters do not take window decorations
(such as border or title bar) into account.

The \a Flags field is mandatory. If you want to use all defaults, use the
nglWindow::NoFlag value. Any combination of these flags can be used. See each
flag own documentation for more info.

If the the \a nglWindow::FullScreen flag is set in \a Flags, the window dimensions
should match a valid video mode (see nglVideoMode::Enum()), otherwise regular
window mode will be used as a fallback.
*/
class NGL_API nglWindowInfo
{
public:
  enum Position  ///<  Window initial position hint
  {
    ePosUser,    ///< User specified position, see nglWindowInfo X and Y items
    ePosCenter,  ///< Center the window on the screen
    ePosMouse,   ///< Center the window on the current mouse pointer location
    ePosAuto     ///< Let the window manager choose a position
  };
  
  nglWindowInfo(uint UserWidth = 640, uint UserHeight = 480, bool IsFullScreen = false);
  /*!< Simple constructor
    \param UserWidth window width
    \param UserHeight window height
    \param IsFullScreen if true, display full screen (might need a video mode switch)

    This is equivalent to this code snippet :

\code
Pos       = ePosAuto;
XPos      = 0;
YPos      = 0;
Width     = UserWidth;
Height    = UserHeight;
Flags     = IsFullScreen ? nglWindow::FullScreen : nglWindow::NoFlag;
Events    = nglWindow::AllEvents;
Cursor    = eCursorArrow;
MouseMode = nglMouseInfo::eAbsolute;
KeyRepeat = true;
Title     = App->GetName();
\endcode

    Note that \a XPos and \a YPos are defined but unused since automatic window position
    is requested.
  */

  Position                Pos;        ///< Initial position hint
  int                     XPos;       ///< X position 
  int                     YPos;       ///< Y position
  uint                    Width;      ///< Rendering area Width
  uint                    Height;     ///< Rendering area Height
  int                     MaxWidth;   ///< Rendering area Width
  int                     MaxHeight;  ///< Rendering area Height
  int                     MinWidth;   ///< Rendering area Width
  int                     MinHeight;  ///< Rendering area Height
  uint                    Rotate;     ///< Rendering area Rotation Angle
  nglWindow::Flags        Flags;      ///< Window flags
  nglWindow::EventMask    Events;     ///< Event mask
  nuiMouseCursor          Cursor;     ///< Cursor shape
  nglMouseInfo::Mode      MouseMode;  ///< Mouse mode
  bool                    KeyRepeat;  ///< Key repeat
  nglString               Title;      ///< Caption title
  nglWindow::OSInfo       OSInfo;     ///< OS specific options (opaque to the user)

  void Dump (uint Level = 0) const;  ///< Dumps informations to the application log using \p Level verbosity
};

#endif // !_NOGFX_

#endif // __nglWindow_h__

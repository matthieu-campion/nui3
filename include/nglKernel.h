/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglKernel.h
\brief Kernel, application abstraction base class
*/

#ifndef __nglKernel_h__
#define __nglKernel_h__

#include "nglPath.h"
#include "nglString.h"
#include "nuiMessageQueue.h"
#include "nuiEvent.h"

class nglKernel;
class nglPath;

extern NGL_API nglKernel* App;
/*!<
A well designed program should have only one global. This is the unique NGL global,
pointing to the unique instance of the kernel. Since nglKernel is an abstract class,
\a App real type is necessary a descendant of nglKernel. See nglApplication and
the NGL_APP_CREATE macro for instance.
*/

/* Default verbose level
*/
#ifdef _DEBUG_
// The mac (and GCC?) has brain dead assert system that does NOTHING.
# ifdef _CARBON_
#   define __ASSERT_SYS(test) { if (!(test)) Debugger(); }
# elif defined(_WIN32_)
#ifdef __NUI64__
#  define __ASSERT_SYS(test) { assert(test); }
#else
#  define __ASSERT_SYS(test) { if (!(test)) { __asm { int 3 } }; }
#endif
#else
#  define __ASSERT_SYS(test) assert(test);
# endif
# define NGL_LOG_DEFAULT NGL_LOG_INFO
#else
#  define __ASSERT_SYS(test)
#  define NGL_LOG_DEFAULT NGL_LOG_ERROR
#endif


#define __T(x) _T(x)
#define __TFILE__ __T(__FILE__)

#define __NGL_ASSERT(test) \
  if (!(test)) { NGL_OUT(_T("%ls(%d): ") _T(#test) _T(" failed\n"), __TFILE__, __LINE__); __ASSERT_SYS(test) }

#define __NGL_ASSERTR(test, result) \
  if (!(test)) { NGL_OUT(_T("%ls(%d): ") _T(#test) _T(" failed\n"), __TFILE__, __LINE__); __ASSERT_SYS(test) return result; }

/* ASSERT(R) facility
*/
#ifdef _DEBUG_
#  define NGL_ASSERT(x)    __NGL_ASSERT(x)
#  define NGL_ASSERTR(x,y) __NGL_ASSERTR(x,y)
#else
#  define NGL_ASSERT(x)
#  define NGL_ASSERTR(x,y)
#endif // _DEBUG_

/* DEBUG facility
*/
#ifdef _DEBUG_
#  define NGL_DEBUG(x) if (App->GetDebug()) { x }
#else
#  define NGL_DEBUG(x)
#endif



#include "nglEvent.h"
#include "nglError.h"
#include "nglString.h"
#include "nglLog.h"
#include "nglConsole.h"
#include "nglPath.h"
#include "nglTimer.h"

#include "nglClipBoard.h"
#include "nglDataTypesRegistry.h"

class nglDeviceInfo;

//! Kernel, application abstraction base class
/*!
NGL is a framework which can run in various environments. The most trivial one
is a regular application. A more advanced scenario would be running as a
plugin into a hosting application, such as a web browser for instance.

The kernel is the common interface to all these environments : wether your
program is compiled as a regular application or a plugin, one and only one
instance of nglKernel is always available in a single environment (to be
more precise, in a single process). It provides these essential services :

- initialisation and exit callbacks

- event manager interfaces, used internally by timers, windows, etc

- a global and unique <em>application log</em>, which is used by almost any
  instance of a regular NGL class

- a global and unique <em>application console</em>, which is the default
  output for the <em>application log</em>. See nglConsole for more info
  on the default console

- various other global services

nglKernel is an abstract class. You must refer to nglKernel derivates to
create an instance. See the ubiquitous nglApplication for instance.

The unique nglKernel object is always available via the \a App pointer.
*/
class NGL_API nglKernel : public nglError, public nglEvent, public nuiMessageQueue
{
public:
  typedef void (*ExitFunc) (void); ///< Cleanup callback type. See AddExit()

  /** @name Life cycle */
  //@{
  virtual void Quit(int Code = 0);
  /*!<
    Request application ending
    \param Code exit code

    Note that some instances (such as plugins) might ignore a call to this method.
  */
  virtual void AddExit (ExitFunc Func);
  /*!< Register a shutdown callback
    \param Func callback function, must be either a C function or a static C++ method

    Register a callback to be triggered when the application is shuting down.
    Handlers are called in reverse order of registering (last registered is called first).
  */
  virtual void DelExit (ExitFunc Func);
  /*!< Unregister a shutdown callback
    \param Func callback function

    Unregister a callback registered with AddExit().
  */
  //@}

  /** @name Runtime debugging */
  //@{
  virtual void SetDebug(bool Debug);
  /*!<
    Set run-time debug status

    Code enclosed in the DEBUG() macro can be (des)activated at run-time either with SetDebug(),
    either with the '--nodebug' command-line option if you invoke nglApplication::ParseDefaultArgs().
  */
  virtual bool GetDebug() { return mDebug; }
  /*!<
    Return run-time debug status

    See SetDebug().
  */
  //@}

  /** @name Logging and output */
  //@{
  virtual nglLog& GetLog();
  /*!<
    Gives the application log.

    Notably used by the NGL_OUT() and NGL_LOG() macros.
  */
  virtual nglConsole& GetConsole();
  /*!<
    Gives the application console.
  */
  virtual void SetConsole(nglConsole* pConsole);
  /*!<
    Set application console. User still owns the \p pConsole objects
    (ie. remember to delete it on application exit).
  */
  //@}

  /** @name Application context */
  //@{
  const nglPath& GetPath();
  /*!<
    \return current application's path (includes the binary file name)
  */
  const nglString& GetName();
  /*!<
    \return current application name

    The application name is implicitely obtained from the application's executable
    name. However it can be set explicitely with SetName() at any time. The application
    name serves at least two purposes :

    - allow per-application resources settings (TODO: resources!)
    - nglWindow objects will use the application name as their default title
  */
  int       GetArgCount();
  /*!<
    Return the number of user arguments passed along the execution of the
    program. \b Caution \b ! The executable name (or invocation name) is
    not included, use GetName() for this purpose.
  */
  const nglString& GetArg(int Index);
  /*!<
    Return one of the user arguments passed along the execution of the
    program. \a Index must be between 0 (first argument) and GetArgCount() - 1.
    \b Caution \b ! The executable name (or invocation name) is
    not included, use GetName() for this purpose.
    If \a Index is out of range, returns an empty string.
  */
  //@}

  /** @name Clipboard */
  //@{
  virtual nglClipBoard &GetClipBoard();
  virtual void GetClipboard(nglString& rClipboard);
  /*!<
    Get the clipboard text content
    \return If there is text in the clipboard, returns it, or return an empty string
  */
  virtual bool SetClipboard(const nglString& rString);
  /*!<
    Set the text content of the clipboard
    \param rString the text to put into the clipboard
    \return returns false if the clipboard is not available (maybe temporarily)
  */
  //@}

  /** @name DataTypesRegistry */
  //@{
  virtual nglDataTypesRegistry& GetDataTypesRegistry();
  //@}

  /** @name User callbacks */
  //@{
  virtual void OnInit();
  /*!<
    This method is called at startup time, as soon as the application is ready to run.

    There is always at least one argument, (ArgCnt > 0) and it contains the application
    name. User parameters are not available on all platforms (MacOS being one
    of them).
  */
  virtual void OnExit (int Code);
  /*!<
    This method is called when the application exits.
    \param Code exit code

    The exit code is returned by the process to the system. It should be 0
    for normal termination (ie. no error encountered), positive otherwise.

    The application only quits if :
    - Quit() has been explicitely called and the control has been given back to the event loop
    - the process received a signal (\e Unix \e only). HUP, INT, QUIT, PIPE and TERM are
      currently intercepted.
  */

  virtual void OnDeviceAdded(const nglDeviceInfo* pDeviceInfo);
  /*!<
    This method is called when a new device is connected to this computer. This can be used to detect new input devices and nglPathVolumes.
    \param rDeviceInfo a description of the device.
  */

  virtual void OnDeviceRemoved(const nglDeviceInfo* pDeviceInfo);
  /*!<
    This method is called when a device is removed from this computer. This can be used to detect input devices and nglPathVolumes removal.
    \param rDeviceInfo a description of the device.
  */

  virtual void OnMemoryWarning();
  /*!<
   This method is called when the system is running out of memory. You can try inform the user or try to free memory automatically. You can also ignore it and risk a crash 
   */
  
	virtual void OnWillExit();	
  /*!<
   Called right before the application will be terminated by iPhone OS multitasking.
   */
  //@}
  
  virtual void OnActivation();
  /*!<
   Called right before the application will be reactivated by the iPhone OS multitasking.
   */
  //@}
  
  
  virtual void OnDeactivation();
  /*!<
   Called right before the application will be deactivated by the iPhone OS multitasking.
   */
  //@}
  
  
  void NonBlockingHeartBeat(); ///< Keep the application event loop alive without waiting for new events. (i.e. only process the events that are already in the queue).
  
  static void SetCrashReportEmail(const nglString& rEmail);

protected:
  // Life cycle
  nglKernel();
  virtual ~nglKernel();
  void IncRef();
  void DecRef();
  void Init();
  void Exit(int32 ExitCode);

  void ProcessMessages(const nuiEvent& rEvent);
  nuiEventSink<nglKernel> mKernelEventSink;
  
  void SetName (const nglString& rName);
  void SetPath (const nglPath& rPath);
  void ParseCmdLine (char* pCmdLine);
  void AddArg (const nglString& rArg);
  void DelArg (int Pos, int Count = 1);

  // Internal callbacks
  void CallOnInit();
  void CallOnExit(int Code);
  void CallOnWillExit();
  void CallOnMemoryWarning();
  void CallOnActivation();
  void CallOnDeactivation();
  
  
  
  // From nglError
  virtual const nglChar* OnError (uint& rError) const;

private:
  typedef std::list<ExitFunc>    ExitFuncList;
  typedef std::vector<nglString> ArgList;

  uint          mRefCount;
  bool          mDebug;
  nglLog*       mpLog;
  nglConsole*   mpCon;

  nglClipBoard  mClipboard;

  bool          mOwnCon;
  ExitFuncList  mExitFuncs;
  nglPath       mPath;
  nglString     mName;
  ArgList       mArgs;

  nglDataTypesRegistry mDataTypesRegistry;

  nglKernel(const nglKernel&)
  : mKernelEventSink(this)
  {
  } // Undefined copy constructor

  friend bool nuiInit(void* OSHandle, class nuiKernel* pKernel);
  friend bool nuiUninit();

	
  
#ifdef _WIN32_
protected:
  friend class nglConsole;
  friend class nglTimer;
  friend class nglWindow;
  friend class nglInputDeviceDirectX;
  friend class nglContext;
  friend class nglOffscreenContext;
  friend class nglClipBoard;

public:
  virtual HINSTANCE GetHInstance() const;
  virtual HWND      GetHWnd() const;
protected:
  virtual HDC       GetDC() const;
  virtual HFONT     GetSystemFont() const;
  virtual LPDIRECT3D9 GetDirect3D() const;

  bool SysInit(HINSTANCE Inst);

private:
  mutable HINSTANCE  mHInstance;
  mutable HDC        mHDC;
  mutable HWND       mHWnd;
  mutable HFONT      mSystemFont;
  int        mResolutionChanged;
  bool       mShowConsole;
  mutable LPDIRECT3D9 mpDirect3D;

  static LRESULT CALLBACK MainWndProcStatic (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  LRESULT                 MainWndProc       (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
#endif // _WIN32_

  friend class nglPluginKernel;
  friend class nglPlugin;

#ifdef _UNIX_
public:
  virtual void  AddEvent (nglEvent* pEvent);
  virtual void  DelEvent (nglEvent* pEvent);
  virtual void  AddTimer (nglTimer* pTimer);
  virtual void  DelTimer (nglTimer* pTimer);

  /* To avoid complex design, we consider the windowing support API
   * as part of the core kernel API.
   */
  virtual void* GetDisplay();
  virtual void  AddWindow (class nglWindow* pWin);
  virtual void  DelWindow (class nglWindow* pWin);
  
protected:
  bool             SysInit();
  virtual void     OnEvent(uint Flags); // From nglEvent
  
  friend class nglWindow;
  /* This is needed under Linux because the events are processed using
   * a member of nglApplication, so only nglApplication is able to process
   * events. nglWindows modalState functions are just plain calls to these ones. 
   */ 
  virtual void  EnterModalState();
  virtual void  ExitModalState();
  
  bool             mModalState;

private:
  void             CatchSignal (int Signal, void (*pHandler)(int));
  static void      OnSignal (int Signal);
#endif // _UNIX_

#ifdef _CARBON_
protected:
  bool             mExitPosted;

  bool             SysInit();
  friend pascal OSStatus nglVolumeEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
  std::list<nglPathVolume> mVolumes;
  EventHandlerRef   mEventHandlerRef;
  EventHandlerProcPtr mEventHandlerUPP;
#endif // _CARBON_

#ifdef _UIKIT_
protected:
//#FIXME: Volume Handling

  bool SysInit();

  friend void objCCallOnInit(void* pUIApplication);
  friend void objCCallOnExit(int Code);
  friend void objCCallOnWillExit();
  friend void objCCallOnInitWithURL(void* pUIApplication, const nglString &url);
  friend void objCCallOnActivation();
  friend void objCCallOnDeactivation();
  friend void objCCallOnMemoryWarning();
  
  void* mpUIApplication;

public:
	void * GetUIApplication() { return mpUIApplication; }

#endif//_UIKIT_

#ifdef _COCOA_
protected:
  //#FIXME: Volume Handling
  
  bool SysInit();
  
  friend void objCCallOnInit(void* pNSApplication);
  friend void objCCallOnExit(int Code);
  friend void objCCallOnWillExit();
	
  void* mpNSApplication;
  
  friend void objCCallOnInitWithURL(void* pUIApplication, const nglString &url);  
  
public:
	void * GetNSApplication() { return mpNSApplication; }
  
#endif//_UIKIT_
  
};




/*
 * NGL kernel log/debug macros
 */

// NGL_OUT(_T("i=%d\n"), i)
#define NGL_OUT  App->GetConsole().Output
#define NGL_OUTV App->GetConsole().Outputv

// NGL_LOG(_T("net"), NGL_LOG_INFO, "i=%d\n", i)
#define NGL_LOG  App->GetLog().Log
#define NGL_LOGV App->GetLog().Logv

//! From C++ Coding Standards

//! allows fast cast in release and checked in debug

template<typename TTarget, typename TSource> inline TTarget checked_cast(TSource *src) {

  NGL_ASSERT(dynamic_cast<TTarget>(src) == static_cast<TTarget>(src) && "checked_cast failed");

  return static_cast<TTarget>(src);

}

#endif // __nglKernel_h__

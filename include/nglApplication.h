/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglApplication.h
\brief Application class

Abstraction of the running application.
*/

#ifndef __nglApplication_h__
#define __nglApplication_h__

//#include "nui.h"
#include "nglKernel.h"
#include "nuiInit.h"

#ifdef _X11_
#include <X11/Xlib.h>
#endif // _X11_


//! Application class
/*!
There must be one and only one application class in a NGL program. You cannot
link and run a NGL program if you don't instantiate an application class. You
must actually derivate your application class from \b nglApplication and
instantiate it with the NGL_APP_CREATE() or NGL_APP_CREATE_EX() macro.

\code
class MyApp : public nglApplication
{
public:
  void OnInit()
  {
    NGL_OUT (_T("Hello world !\n"));
    Quit();
  }
};

NGL_APP_CREATE (MyApp)
\endcode

This is a perfectly valid an minimal NGL running example. NGL will instantiate
one MyApp class for you and call the OnInit() method as soon as the application
is ready to run.

Note that the application instance is mostly accessed via a global reference called \a App,
which type is nglKernel* (pointer). Since NGL declares it for you, it can be used anywhere
provided you include <nglKernel.h>. For instance to exit the application you just write :

\code
App->Quit(); // Does not return immediatly, read the manual !
\endcode
*/
class NGL_API nglApplication : public nglKernel
{
public:

  /** @name Life cycle */
  //@{
  nglApplication();
  /*!<
    Never do real work in the constructor, use the OnInit() callback.
    Only initialize your attributes here.
  */
  virtual ~nglApplication();
  /*!<
    Never do real work in the destructor, use the OnExit() callback.
  */
  void Quit(int Code = 0);
  /*!<
    Request application ending
    \param Code exit code

    This function does not exit immediatly, but rather schedule an 'exit event'
    to be triggered as soon as possible. If you want to quit the application
    immediatly, you must return from the current event handler first in order
    to process the 'exit event'.

    The exit code is returned by the process to the system. It should be 0
    if the application naturally exited (ie. no error encountered). The
    OnExit() callback is the last processed event.
  */
  virtual bool OnQuitRequested();
  /*!<
   Called when the user requests to quit the application
   
   The user can request the application to quit (for example by choosing the Quit entry of the application menu on MacOSX).
   This is diferent than closig the main window. Returning true on this method quits the application, false refuse the exit.
   */
  //@}
	
  bool IsQuitRequested() const;
  /*!<
   Returns true if Quit has been called
   */
  //@}
  
  virtual void OpenDocuments(std::list<nglPath>& paths);
  /*!<
   Called when an "open documents" apple event is received (on Macintosh).
   */

  /** @name Command line */
  //@{
  bool ParseDefaultArgs();
  /*!<
    Parse built-in default parameters
    \return false if a command line error was encountered

    This method allows you to add a standard set of user option to your application. All
    command line arguments recognized and interpreted by this method are removed from
    the argument list (see GetArg()). This method should be called early in the
    OnInit() callback. It is also suggested to fail when this method returns false,
    by calling PrintUsage() and exiting from the application.

    Currently supported options :

    - <i>--log-level [domain[:\<level\>],... ]</i> : configure the verbose level of several log domains.
    For each <i>domain</i>, if a <i>level</i> is not specified, LOG_LEVEL is used. A special domain
    called <i>all</i> sets the given level to all domains simultaneously. Level assignations are
    read from left to right. Log domain aliases are expected, not display name (see nglLog).

    Example :

\code
myprogram --log-level all:0,net:info
\endcode

    Set all domain verbose level to 0 (unmaskable messages only), except 'net' which is set to 'info' level.
    Level can be any number or one of the predefined slot (defined at compile time) :
    \e error, \e warning, \e info or \e debug.

    - <i>--log-output [con:<on|off>,file:\<path\>]</i> : configure the default log outputs. You can turn
    console output on and off with the 'con:' prefix, or add any number of file ouputs with
    'file:path' directives. All output descriptions are coma separated.

    - <i>--no-debug</i> : don't run statements enclosed by the NGL_DEBUG() macro.
      This is useful to fake a 'release run' for a debug build (NGL_DEBUG enclosed statements
      are not compiled in in release mode).

    - <i>--help</i> : display NGL default args documentation.
    \b Note : this command is \e not removed from the application command line arguments, in
    order the user might add his own help code.
  */
  virtual void PrintUsage();
  /*!<
    Print to default console default NGL options documentation.

    This method is invoked by ParseDefaultArgs() when the \e --help option is encountered.
    It is strongly suggested to call this method when ParseDefaultArgs() fails.
  */
  //@}

  /** @name Idle events control */
  //@{
  virtual void SetIdle (bool UseIdle = true);
  /*!<
    Turn on/off idle event generation
    \param UseIdle idle event generation on/off

    If \a UseIdle is \e true, NGL will call OnIdle() repeatedly whenever there is no
    other system events to dispatch (such as timer, window or network events), in an
    infinite loop manner.

    If \a UseIdle is \e false, NGL will stop calling OnIdle() at once (and let the
    CPU breathe !).

    If you are using the OnIdle callback, you should be aware of multitasking and
    system scheduler issues. On some systems, using an idle event can make the
    whole system rather unresponsive. Since scheduling control (priority and such)
    is not very portable, NGL uses the default behaviour of every OS. Unix users can
    play with the \e nice command for instance.

    Important note : you should use idle events only if :

    - you have an autonomous application (which doesn't need user intervention to run)
    and you want to get the maximum frame rate. It can be OpenGL rendering or any
    other task. Games are programmed using this technique.

    - you must do heavy computations, but need your application to be still responsive.
      Until there is thread support in NGL, you can decompose your job in small 'slices'
      and call them from the idle loop. Every time you return from the idle loop, you
      give a chance to graphics events to be caught (and your callbacks to be called).

    When you are using the idle loop, you should use polling methods instead of
    callbacks if you want good coherence (time is a crucial parameter with idle events !).
    For instance, use nglWindow::IsKeyDown() and nglWindow::GetMouse() instead of their
    respective callback alternatives.

    A simple case of idle loop programming is implemented in the example/gears demo of
    the NGL distribution package.
  */
  bool GetIdle();
  /*!<
    \return true if the idle event generation is activated
  */
  //@}

  /** @name User callbacks */
  //@{
  virtual void OnIdle (nglTime Lap);
  /*!<
    The OnIdle event callback is triggered when the system event queue is empty.
    \param Lap the time elapsed since the last OnIdle() call

    Please see SetIdle() for more information. This callback \e won't be called
    until you invoke SetIdle().
  */
  //@}

  void OnDeactivation();

protected:
  const nglChar* OnError (uint& rError) const; // From nglError
  bool mExitPosted;

private:
  bool    mUseIdle;
  nglTime mLastIdleCall;

  nglApplication(const nglApplication&) {} // Undefined copy constructor

  void CallOnIdle();

#ifdef _WIN32_
private:
  friend int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

  void ParseArgs (char* pCmdLine);
  int  WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
  int  SysLoop();
#endif // _WIN32_

#ifdef _COCOA_
  int Main(int ArgCount, const char** pArgs);
  friend int main(int ArgCount, const char** pArgs);
  bool Init(int argc, const char** argv);
  int  Run();
//#endif

//#ifdef _CARBON_
#elif (defined _CARBON_)
protected:
  int  mExitCode;

  int  Main(int argc, const char** argv);
  bool Init(int argc, const char** argv);
  int  Run();
  void MakeMenu();
  void DoMenuCommand(long int command);

  static OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon );
  static OSErr OpenDocumentsAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon );

  friend int main(int argc, const char** argv);
  
#ifdef __MACHO__  
  friend pascal void TimerAction (EventLoopTimerRef  theTimer, EventLoopIdleTimerMessage inState, void* userData);
#else // CFM
  friend pascal void TimerAction (EventLoopTimerRef  theTimer, void* userData);
#endif
  EventLoopTimerRef mIdleTimer;
#endif // _CARBON_

#ifdef _UIKIT_
protected:
	//@{
	/*!<
	 Called when the system (iPhone OS) is sending memory warning to all processes.
	 You should free up memory when this happens, warn the user or quit the application.
	 */
	
  int  Main(int argc, const char** argv);
  bool Init(int argc, const char** argv);
  int  Run();

//  static OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon );
  friend int main(int argc, const char** argv);

///< Yes, it has to be public (called from an obj-c class ...)
public:
#endif//_UIKIT_

#ifdef _UNIX_
private:
  friend int main(int, const char**);

  int  Main(int ArgCnt, const char** pArg);
  bool Init(int ArgCnt, const char** pArg);
  void Exit();
  int  SysLoop();

  void AddEvent (nglEvent* pEvent);
  void DelEvent (nglEvent* pEvent);
  void AddTimer (nglTimer* pTimer);
  void DelTimer (nglTimer* pTimer);
  
  void  EnterModalState();
  void  ExitModalState();
  
#ifdef _X11_
  void AddWindow (class nglWindow* pWin);
  void DelWindow (class nglWindow* pWin);
  void* GetDisplay();
#endif // _X11_
  virtual void OnEvent(uint Flags); // From nglEvent

private:
  typedef std::list <class nglEvent*> EventList;
  typedef std::list <class nglTimer*> TimerList;
#ifdef _X11_
  typedef std::map <Window, class nglWindow*> WindowList;
#endif

  bool       mExitReq;
  int        mExitCode;
  EventList  mEvents;
  TimerList  mTimers;
#ifdef _X11_
  Display*   mpDisplay;
  WindowList mWindows;
#endif // _X11_
#endif // _UNIX_
};


/*!
NGL applications have only one global (besides the user ones), the application
instance itself. Most of the time you will use \p App which is a pointer
to your application instance, but downcasted to the base nglKernel* type.

If you want to keep a global reference to your application whith its proper
type, you will have to use the NGL_APP_CREATE_EX() macro in place of
NGL_APP_CREATE() \e and manage this global yourself.
*/
extern NGL_API class nglKernel* App;


/*
 * Internal (intermediate) macros
 */

#ifndef _DOXYGEN_

#ifdef _WIN32_
  #define __NGL_APP_MAINDECL int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
  #define __NGL_APP_MAINCALL WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd)
#endif // _WIN32_

#if defined(_COCOA_)
  #define __NGL_APP_MAINDECL int main(int argc, const char** argv)
  #define __NGL_APP_MAINCALL Main(argc, argv)
#endif // _COCOA_

//#ifdef _CARBON_
//#error "_CARBON_ shouldn't be defined"
//#endif
//#ifdef _UIKIT_
//#error "_UIKIT_ shouldn't be defined"
//#endif

#if defined(_UNIX_) || defined(_CARBON_) || defined(_UIKIT_)
  #define __NGL_APP_MAINDECL int main(int argc, const char** argv)
  #define __NGL_APP_MAINCALL Main(argc, argv)
#endif // _UNIX_

#if (defined _DEBUG_) && (defined _WIN32_)
  #define NGL_CHECK_MEMORY _CrtMemDumpAllObjectsSince( NULL );
#else
  #define NGL_CHECK_MEMORY
#endif

#define __NGL_APP_CREATE(UserAppClass, InstanceHook) \
  __NGL_APP_MAINDECL \
  { \
    int ret = 1; \
    if (!App) \
    { \
      UserAppClass* user InstanceHook new UserAppClass(); \
      nuiInit(NULL); \
      ret = ((nglApplication*)user)->__NGL_APP_MAINCALL; \
      delete user; \
      App = NULL; \
      nuiUninit(); \
    } \
    /*NGL_CHECK_MEMORY*/\
    return ret; \
  }
#endif // !_DOXYGEN_

/*!
NGL applications have only one global (besides the user ones), the application
instance itself. You create this instance by invoking the NGL_APP_CREATE() macro
from one of your .cpp files with your application class name.
*/
#define NGL_APP_CREATE(UserAppClass) \
  __NGL_APP_CREATE(UserAppClass, =)

/*!
If you need to keep a global pointer to your application class (using its
native type, not the downcast to nglKernel), you must provide your own
global variable to NGL_APP_CREATE_EX(). Example :

\code
// MyApplication.h

class MyApplication { ... };

#ifndef __MyApplication_cpp__
extern MyApplication* MyApp;
#endif
\endcode

\code
// MyApplication.cpp

#define __MyApplication_cpp__
#include "MyApplication.h"

MyApplication* MyApp;

NGL_DECLARE_APP_EX(MyApplication, MyApp);
\endcode
*/
#define NGL_APP_CREATE_EX(UserAppClass, UserAppInstance) \
  __NGL_APP_CREATE(UserAppClass, = UserAppInstance = )


#endif // __nglApplication_h__

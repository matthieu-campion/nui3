/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


/*!
\file  nglTimer.h
\brief Low resolution, general purpose timer
*/

#ifndef __nglTimer_h__
#define __nglTimer_h__

//#include "nui.h"
#include "nglTime.h"

#ifdef _WIN32_
  #ifdef NGL_USE_WIN32_MMTIMER
    #include <mmsystem.h>
  #endif
#endif

#if defined(_CORE_FOUNDATION_)
  #include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef _ANDROID_
#include <signal.h>
#endif // _ANDROID_

//! Low resolution, general purpose timer
/*!
nglTimer can trigger a user-event at a given frequency. Here is a simple example
of a timer class displaying the 'Tick !' string via default log every second :

\code
class TestTimer : public nglTimer
{
public:

  TestTimer() : nglTimer(1.0)
  {
    Start();
  }

  void OnTick (nglTime Lap)
  {
    NGL_OUT(_T("Tick !\n"));
  }
};
\endcode

At instantion time + one period (1 second here), the first 'Tick !' of a long
series (until you delete the instance !) will be displayed. You can invoke
Start(true, true) to fire the first event immediatly.

This timer is dedicated to 'low resolution' jobs, such as UI widgets : expect
a precision of 50 milliseconds at best. If you want high precision timing,
consider using nglTime time stamps differences, or the nglApplication::OnIdle()
event (please consult its documentation carefully).
*/
class NGL_API nglTimer
{
public:
  /** @name Life cycle */
  //@{
  nglTimer(nglTime Period);
  /*!<
    \param Period timer period in second

    The period can be modified later with SetPeriod(). You must call Start() to run
    the timer.
  */
  virtual ~nglTimer();  ///< Guess what ? Destroys the timer and hence stops its activity.
  //@}

  /** @name Settings */
  //@{
  static nglTime GetMinPeriod();      ///< Minimum period handled by timers
  uint    GetTickCount();             ///< Return event count since last instantiation or reset
  nglTime GetPeriod();                ///< Return timer period (in second)
  bool    SetPeriod(nglTime Period);  ///< Set timer period (in second)
  //@}

  /** @name Running control */
  //@{
  bool    Start (bool Immediate = true, bool Reset = true);
  /*!<
    \param Reset reset event counter
    \param Immediate trigger the first timer as soon as possible
    \return true if the timer was successfully started

    You must invoke Start() to run the timer, it does not run automatically at
    instantiation time. If \a Immediate is false, the first event will
    be triggered one period time after the Start() call, or as soon as possible
    otherwise.
  */
  void    Stop();       ///< Stop timer (preserve event counter)
  bool    IsRunning();  ///< Get timer status
  //@}

  /** @name User callbacks */
  //@{
  virtual void OnTick(nglTime Lap);
  /*!< Timer event callback
    \param Lap exact elapsed time since last call

    You can tell if an event was skipped by testing Lap > GetPeriod(). The number
    of missed events is floor(Lap/GetPeriod()).
  */
  //@}

private:
  nglTime mPeriod;    // Period in second
  uint    mCallCnt;   // User OnTick() calls counter
  bool    mRunning;   // Running status

  nglTimer(const nglTimer&) {} // Undefined copy constructor

#if defined(_UNIX_)
public:
  nglTime GetTimeOut (nglTime Now);
  void    Update();

private:
  nglTime mLastTick;
  nglTime mNextTick;

  void    CallOnTick(nglTime Lap);
#endif //_UNIX_

#if 0 // defined(_CARBON_)
private:
  nglTime mLastTime;
  EventLoopTimerUPP mTimerProc;
  EventLoopTimerRef mTimer;
  friend pascal void nglTimerAction(EventLoopTimerRef theTimer, void* userData);
  void TimerAction();
#endif

#if defined(_CORE_FOUNDATION_)
private:
  CFRunLoopRef      mpCFRunLoop;
  CFRunLoopTimerRef mpCFRunLoopTimer;
  nglTime mLastTime;
  friend void nglTimerAction(CFRunLoopTimerRef pTimer, void* pUserData);
  void TimerAction();
#endif

#ifdef _WIN32_
private:
/*
  friend LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam );
*/

  nglTime  mLastTick;
  void CallOnTick();
  static LRESULT WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  static std::list<nglTimer*> mTimers;

  uint32 mCounter;
  uint32 mRoundsPerTick;
  static void InitMainTimer();
  static void FreeMainTimer();
  friend VOID CALLBACK TimeProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
  static nglAtomic mQueuedEvents;
  static HANDLE mTimerQueueHandle;
  static HANDLE mTimerHandle;

  static void PostMessage();

  friend class nglKernel;
#endif // _WIN32_
  
  
#ifdef _ANDROID_
public:
  static void DispatchTimers();
  static double DispatchPeriod;
private:
  static nglTime sLastDispatch;
  static std::list<nglTimer*> sTimers;
  
  void CallOnDispatch();
  uint32 mCounter;
  uint32 mRoundsPerTick;
  
  nglTime mLastTime;
  void TimerAction();
  
  friend void nglTimerAndroidHandler(int sig, siginfo_t *si, void *uc);
#endif // _ANDROID_
};

#endif // __nglTimer_h__

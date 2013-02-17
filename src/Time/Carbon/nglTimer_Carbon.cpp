/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define TIMER_MIN_PERIOD     0.00001 // (somewhat 'reasonable')

static std::set<nglTimer*> gNGLTimers;

nglTimer::nglTimer(nglTime Period) : mPeriod(Period), mLastTime(0)
{
  mCallCnt = 0;
  mRunning = false;

  mTimer = NULL;
  mTimerProc = NULL;
  gNGLTimers.insert(this);
}

nglTimer::~nglTimer()
{
  Stop();
  gNGLTimers.erase(this);
}


uint nglTimer::GetTickCount()
{
  return mCallCnt;
}

nglTime nglTimer::GetMinPeriod()
{
  return nglTime(TIMER_MIN_PERIOD);
}

nglTime nglTimer::GetPeriod()
{
  return mPeriod;
}

bool nglTimer::SetPeriod(nglTime Period)
{
  if (Period < TIMER_MIN_PERIOD)
  {
    mPeriod = TIMER_MIN_PERIOD;
    return false;
  }
  mPeriod = Period;
  return true;
}

pascal void nglTimerAction(EventLoopTimerRef theTimer, void* userData)
{
  nglTimer* pTimer = (nglTimer*)userData;
  
  if (!pTimer)
    return;

  std::set<nglTimer*>::iterator it = gNGLTimers.find(pTimer);
  std::set<nglTimer*>::iterator end = gNGLTimers.end();
  if (it == end) 
    return;
    
  bool running = pTimer->IsRunning();
  if (!running)
    return;
  
  pTimer->TimerAction();
}

void nglTimer::TimerAction()
{
  nglTime now;
  OnTick(now - mLastTime);
  mLastTime = now;
  mCallCnt++;
}

bool nglTimer::Start(bool Immediate, bool Reset)
{
  Stop();
  nglTime now;

  if (Reset)
    mCallCnt = 0;

  mLastTime = now;

  EventLoopRef       mainLoop;

  mainLoop = GetMainEventLoop();
  mTimerProc = NewEventLoopTimerUPP(&nglTimerAction);

  mRunning = true;


  EventTimerInterval interval = mPeriod * kEventDurationSecond;
  InstallEventLoopTimer (mainLoop,
                         Immediate? 0 : interval,   // get fireDelay the right value, depending on the user's choice about Immediate parameter.
                         interval,
                         mTimerProc,
                         (void*)this,
                         &mTimer);

  // we don't need the following instruction since the fireDelay parameter
  // does the job. Question is: how does this stuff could work before this fix??
  //  if (Immediate)
  //    OnTick(0);
  
  return true;
}

void nglTimer::Stop()
{
  if (mTimer)
	  RemoveEventLoopTimer(mTimer);
  if (mTimerProc)
	  DisposeEventLoopTimerUPP(mTimerProc);
  mTimer = NULL;
  mTimerProc = NULL;
  mRunning = false;
}

bool nglTimer::IsRunning()
{
  return mRunning;
}



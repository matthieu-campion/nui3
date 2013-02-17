#include "nui.h"

#define TIMER_MIN_PERIOD     0.00001 // (somewhat 'reasonable')

static std::set<nglTimer*> gNGLTimers;

nglTimer::nglTimer(nglTime Period) : mPeriod(Period), mLastTime(0)
{
  mCallCnt = 0;
  mRunning = false;
  
  mpCFRunLoop = NULL;
  mpCFRunLoopTimer = NULL;
  gNGLTimers.insert(this);
}

nglTimer::~nglTimer()
{
  Stop();
  if (mpCFRunLoopTimer)
    CFRelease(mpCFRunLoopTimer);
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

void nglTimerAction(CFRunLoopTimerRef       pCFTimer,
                    void*                   pUserData)
{
  //nuiStopWatch watch(_T("nglTimerAction"));
  nglTimer* pTimer = (nglTimer*)(pUserData);
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

  if (!mpCFRunLoopTimer)
  {
    CFRunLoopTimerContext ctx;
    ctx.version = 0;
    ctx.info = (void*)this;
    ctx.retain = NULL;
    ctx.release = NULL;
    ctx.copyDescription = NULL;

    CFAbsoluteTime absTime = CFAbsoluteTimeGetCurrent() + (double)mPeriod;
    
    mpCFRunLoopTimer =
      CFRunLoopTimerCreate( kCFAllocatorDefault,
                            absTime,
                            mPeriod, 
                            0,// option flags, not implemented so far
                            0,// timer order, not implemented so far
                            &nglTimerAction,
                            &ctx);
  }
  NGL_ASSERT(mpCFRunLoopTimer);
  if (!mpCFRunLoopTimer)
    return false;
  
  mpCFRunLoop = CFRunLoopGetCurrent();
  NGL_ASSERT(mpCFRunLoop);
  
  mRunning = true;
  CFRunLoopAddTimer(mpCFRunLoop, mpCFRunLoopTimer, kCFRunLoopCommonModes);

  if (Immediate)
    TimerAction();
  
  return true;
}

void nglTimer::Stop()
{
  if (mRunning)
  {
    NGL_ASSERT(mpCFRunLoopTimer);
    CFRunLoopRef currentRunLoop = CFRunLoopGetCurrent();
    NGL_ASSERT(mpCFRunLoop == currentRunLoop);
    CFRunLoopRemoveTimer(currentRunLoop, mpCFRunLoopTimer, kCFRunLoopCommonModes);
    if (mpCFRunLoopTimer)
      CFRelease(mpCFRunLoopTimer);
    mpCFRunLoopTimer = NULL;
  }
  mRunning = false;
}

bool nglTimer::IsRunning()
{
  return mRunning;
}



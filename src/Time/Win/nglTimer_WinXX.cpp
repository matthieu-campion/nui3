/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglTimer.h"
#include "nglMath.h"
#include <math.h>

#define NGL_MAX_QUEUED_TIMER_EVENTS 1

MMRESULT nglTimer::mTimerID = -1;
void nglTimer::InitMainTimer()
{
  if (mTimerID == -1)
  {
    TIMECAPS caps;
    timeGetDevCaps(&caps,sizeof(TIMECAPS));
    timeBeginPeriod(caps.wPeriodMin); // set the minimum timer period
    mTimerID = timeSetEvent(caps.wPeriodMin, 0, TimeProc, (unsigned int)0, TIME_PERIODIC + TIME_CALLBACK_FUNCTION + TIME_KILL_SYNCHRONOUS);
  }
}

void nglTimer::FreeMainTimer()
{
  if (mTimers.empty())
  {
    TIMECAPS caps;
    timeGetDevCaps(&caps,sizeof(TIMECAPS));
    timeEndPeriod(caps.wPeriodMin); // set the minimum timer period
    timeKillEvent(mTimerID);
    mTimerID = -1;
  }
}

nglTimer::nglTimer(nglTime Period)
{
  mCounter = 0;
  mRoundsPerTick = 0;
  mCallCnt = 0;
  //mTimerID = 0;
  mRunning = false;
  SetPeriod(Period);

  mCounter = mRoundsPerTick;

  mLastTick = nglTime::GetTime();
  mTimers.push_back(this);
  InitMainTimer();
}

nglTimer::~nglTimer()
{
  Stop();
  mTimers.remove(this);
  FreeMainTimer();
}


uint nglTimer::GetTickCount()
{
  return mCallCnt;
}

nglTime nglTimer::GetMinPeriod()
{
  TIMECAPS caps;
  timeGetDevCaps(&caps,sizeof(TIMECAPS));
  return 0.001f * (nglTime)caps.wPeriodMin;
}

nglTime nglTimer::GetPeriod()
{
  return mPeriod;
}

bool nglTimer::SetPeriod(nglTime Period)
{
  if (mRunning) 
    return false;
  mPeriod = Period;
  mRoundsPerTick = ToBelow(mPeriod / GetMinPeriod());
  if (!mRoundsPerTick)
    mRoundsPerTick = 1;
  return true;
}

void CALLBACK TimeProc(UINT uID,UINT uMsg,DWORD_PTR dwUser,DWORD_PTR dw1,DWORD_PTR dw2)
{
//   static int c = 0;
//   static int d = 0;
// 
//   if (d & 15)
//   {
//     OutputDebugString(_T("."));
//     c++;
//   }
//   if (c > 50)
//   {
//     c = 0;
//     OutputDebugString(_T("\n"));
//   }
  nglTimer::PostMessage();
}

bool nglTimer::Start(bool Immediate, bool Reset)
{
  if (mRunning)
    return false;
  //mTimerID=timeSetEvent((unsigned int)(1000.0f*mPeriod),0,TimeProc,(unsigned int) this,TIME_PERIODIC + TIME_CALLBACK_FUNCTION);
  //if (mTimerID!=NULL)
  {
    mCounter = MIN(mRoundsPerTick, mCounter);
    if (Reset)
    {
      mCallCnt = 0;
      mCounter = mRoundsPerTick;
    }
    mRunning = true;
    mLastTick = nglTime::GetTime();
    if (Immediate)
      OnTick(0);
  }
/*
  else
    mRunning=false;
*/

  return mRunning;
}

void nglTimer::Stop()
{
  if (mRunning /*&& mTimerID*/)
  {
    //timeKillEvent(mTimerID);
    //mTimerID=NULL;
    mRunning = false;
  }
}

bool nglTimer::IsRunning()
{
  return mRunning;
}


/*
*  Internals:
*/

void nglTimer::CallOnTick()
{
  if (mRunning)
  {
    mCounter--;
    if (!mCounter)
    {
      nglTime tick;
      mCallCnt++;
      tick=nglTime::GetTime();

      OnTick(tick-mLastTick);

      mLastTick=tick;
      mCounter = mRoundsPerTick;
    }
  }
}

std::list<nglTimer*> nglTimer::mTimers;

nglAtomic nglTimer::mQueuedEvents = 0;

LRESULT nglTimer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  std::list<nglTimer*>::iterator it;
  std::list<nglTimer*>::iterator end = mTimers.end();
  for (it = mTimers.begin(); it != end; ++it)
  {
    nglTimer* pTimer = *it;
    if (pTimer->IsRunning())
      pTimer->CallOnTick();
  }

  ngl_atomic_dec(mQueuedEvents);
  return 0;
}

void nglTimer::PostMessage()
{
  if (App && mQueuedEvents < NGL_MAX_QUEUED_TIMER_EVENTS)
  {
    ngl_atomic_inc(mQueuedEvents);
    ::PostMessage(App->GetHWnd(), WM_NGLTIMER, 0, 0);
  }
  else
  {
//     wchar_t str[1024];
//     wsprintf(str, _T("f(%d)"), mQueuedEvents);
//     OutputDebugString(str);
// 
//     static int c = 0;
//     c++;
//     if (c > 20)
//     {
//       c = 0;
//       OutputDebugString(_T("\n"));
//     }
  }
}


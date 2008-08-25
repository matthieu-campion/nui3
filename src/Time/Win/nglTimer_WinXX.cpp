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
#include "nglKernel.h"
#include "nglTimer.h"
#include "nglMath.h"
#include <math.h>

#define TIMER_MIN_PERIOD     0.050
#define TIMER_ZERO_TOLERANCE 0.005 // _must_ be <= TIMER_MIN_PERIOD

#ifdef NGL_USE_WIN32_MMTIMER

MMRESULT nglTimer::mTimerID = -1;
void nglTimer::InitMainTimer()
{
  if (mTimerID == -1)
  {
    TIMECAPS caps;
    timeGetDevCaps(&caps,sizeof(TIMECAPS));
    timeBeginPeriod(caps.wPeriodMin); // set the minimum timer period
    mTimerID = timeSetEvent(caps.wPeriodMin,0,TimeProc,(unsigned int) 0,TIME_PERIODIC + TIME_CALLBACK_FUNCTION);
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
  mCallCnt=0;
  //mTimerID=0;
  mRunning=false;
  SetPeriod(Period);
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
  return 0.001f*(nglTime)caps.wPeriodMin;
}

nglTime nglTimer::GetPeriod()
{
  return mPeriod;
}

bool nglTimer::SetPeriod(nglTime Period)
{
  if (mRunning) 
    return false;
  mPeriod=Period;
  mRoundsPerTick = ToBelow(mPeriod / GetMinPeriod());
  if (!mRoundsPerTick)
    mRoundsPerTick = 1;
  return true;
}

void CALLBACK TimeProc(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
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
      mCallCnt=0;
      mCounter = mRoundsPerTick;
    }
    mRunning=true;
    mLastTick=nglTime::GetTime();
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
    mRunning=false;
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

LRESULT nglTimer::WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  std::list<nglTimer*>::iterator it;
  std::list<nglTimer*>::iterator end = mTimers.end();
  for (it = mTimers.begin(); it != end; ++it)
  {
    nglTimer* pTimer = *it;
    pTimer->CallOnTick();
  }
  return 0;
}

#else
nglTimer::nglTimer(nglTime Period)
{
  mCallCnt = 0;
  mTimerHandle = NULL;
  mRunning = false;
  SetPeriod(Period);
  mLastTick = nglTime::GetTime();
  mTimers.push_back(this);
}

nglTimer::~nglTimer()
{
  Stop();
  mTimers.remove(this);
}


uint nglTimer::GetTickCount()
{
  return mCallCnt;
}

nglTime nglTimer::GetMinPeriod()
{
  return 0.001; // 1 ms
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
  return true;
}

VOID CALLBACK TimeProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
  nglTimer::PostMessage();
}

bool nglTimer::Start(bool Immediate, bool Reset)
{
  //OutputDebugString("nglTimer::Start\n");
  if (mRunning)
    return false;

  int32 t = MAX(1, ToNearest(1000.0*mPeriod)); // The time should last at least 1ms as this is the limit of the Win32 API
  BOOL res = CreateTimerQueueTimer(&mTimerHandle, NULL /* Default timer queue */, &::TimeProc, this, t, t, WT_EXECUTEINTIMERTHREAD);

  if (mTimerHandle!=NULL)
  {
    if (Reset)
      mCallCnt = 0;
    mRunning = true;
    mLastTick = nglTime::GetTime();
    //OutputDebugString("nglTimer::Start OK\n");
    if (Immediate)
      CallOnTick();
  }
  else
    mRunning=false;

  return mRunning;
}

void nglTimer::Stop()
{
  //OutputDebugString("nglTimer::Stop\n");
  if (mRunning && mTimerHandle)
  {
    //OutputDebugString("nglTimer::Stop OK\n");
    DeleteTimerQueueTimer(NULL, mTimerHandle, INVALID_HANDLE_VALUE);
    mTimerHandle = NULL;
    mRunning=false;
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
    mCallCnt++;
    nglTime tick;
    //tick = nglTime::GetTime();

    //OutputDebugString("nglTimer::CallOnTick()\n");
    OnTick(tick-mLastTick);
    
    mLastTick = tick;
  }
  else
  {
    //OutputDebugString("nglTimer::CallOnTick() failed\n");
  }
}

std::list<nglTimer*> nglTimer::mTimers;

LRESULT nglTimer::WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  nglTimer* pTimer = (nglTimer*)lParam;
  if (pTimer)
  {
    std::list<nglTimer*>::iterator it;
    std::list<nglTimer*>::iterator end = mTimers.end();
    for (it = mTimers.begin(); it != end; ++it)
    {
      if (*it == pTimer) // This timer is still valid!
      {
        pTimer->CallOnTick();
        return 0;
      }
    }
  }
  //OutputDebugString("nglTimer::WndProc() for a DEAD timer\n");
  // We only get there if the tick comes from a dead timer...
  return 0;
}
#endif

void nglTimer::PostMessage()
{
  if (App)
  {
    //OutputDebugString("nglTimer::PostMessage()\n");
    ::PostMessage(App->GetHWnd(), WM_NGLTIMER, 0, 0);
  }
  else
  {
    //OutputDebugString("nglTimer::PostMessage() failed\n");
  }
}


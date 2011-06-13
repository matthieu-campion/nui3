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
#include "nglTimer.h"
#include <math.h>

#define TIMER_MIN_PERIOD     0.050 // 50ms (somewhat 'reasonable')
#define NGL_TIMER_ANDROID_SIGNAL SIGRTMIN

bool nglTimer::sMainSignalConnected = false;


void nglTimerAndroidHandler(int sig, siginfo_t *si, void *uc)
{
  nglTimer* pTimer = (nglTimer*)si->si_value.sival_ptr;
  pTimer->TimerAction();
}

nglTimer::nglTimer(nglTime Period) : mPeriod(Period), mLastTime(0)
{
  mCallCnt = 0;
  mRunning = false;
  
  mTimerID = 0;

  if (!sMainSignalConnected)
  {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = nglTimerAndroidHandler;
    
    if (sigaction(NGL_TIMER_ANDROID_SIGNAL, &sa, NULL) == -1)
    {
      return;
    }
    sMainSignalConnected = true;
  }
  
  sigevent event;
  event.sigev_notify = SIGEV_SIGNAL;
  event.sigev_signo = NGL_TIMER_ANDROID_SIGNAL;
  event.sigev_value.sival_ptr = this;
  
  int res = timer_create(CLOCK_REALTIME, &event, &mTimerID);

  App->AddTimer (this);
}

nglTimer::~nglTimer()
{
  App->DelTimer (this);
  if (mTimerID)
    timer_delete(mTimerID);
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

bool nglTimer::Start(bool Immediate, bool Reset)
{
  nglTime now;
  mLastTime = now;
  
  if (Reset)
    mCallCnt = 0;
  
  int sec = ToBelow(mPeriod.GetValue());
  int nanosec = ToBelow(((double)sec - mPeriod.GetValue()) * 1000000000);
  
  itimerspec its;
  its.it_interval.tv_sec = sec;
  its.it_interval.tv_nsec = nanosec;
  
  if (Immediate)
  {
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 1;
  }
  else
  {
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = nanosec;
  }
  
  
  if (mTimerID < 0)
  {
    return false;
  }
  
  if (timer_settime(mTimerID, 0, &its, NULL) == -1)
  {
    return false;
  }

  mRunning = true;
  return true;
}

void nglTimer::Stop()
{
  mRunning = false;
  
  itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  if (mTimerID < 0)
  {
    return;
  }
  
  timer_settime(mTimerID, 0, &its, NULL);
}

void nglTimer::TimerAction()
{
  nglTime now;
  OnTick(now - mLastTime);
  mLastTime = now;
  mCallCnt++;
}

bool nglTimer::IsRunning()
{
  return mRunning;
}

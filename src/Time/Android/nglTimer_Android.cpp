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

nglTime nglTimer::sLastDispatch;
std::list<nglTimer*> nglTimer::sTimers;
double nglTimer::DispatchPeriod = 1; // 1 millisecond

void nglTimer::DispatchTimers()
{
  nglTime now;
  
  double elapsed = now.GetValue() - sLastDispatch.GetValue();
//  LOGI("DispatchTimers elapsed = %lf milliseconds", elapsed * 1000.0);
  if ((elapsed) < 0.001) // at least 1 millisecond between to updates
  {
    return;
  }
  
  std::list<nglTimer*>::iterator it;
  std::list<nglTimer*>::iterator end = sTimers.end();
  for (it = sTimers.begin(); it != end; ++it)
  {
    nglTimer* pTimer = *it;
    if (pTimer->IsRunning())
      pTimer->CallOnDispatch();
  }
  
  sLastDispatch = now;
}

nglTimer::nglTimer(nglTime Period) : mLastTime(0)
{
  mCallCnt = 0;
  mRunning = false;
  mCounter = 0;
  mRoundsPerTick = 0;
  
  SetPeriod(Period);

  App->AddTimer (this);
  sTimers.push_back(this);
}

nglTimer::~nglTimer()
{
  App->DelTimer (this);
  sTimers.remove(this);
}


void nglTimer::CallOnDispatch()
{
  if (!mRunning)
  {
    return;
  }
  
  mCounter--;
  if (!mCounter)
  {
    TimerAction();
    mCounter = mRoundsPerTick;
  }
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
  if (mRunning) 
  {
    return false;
  }
  
  mPeriod = MAX(TIMER_MIN_PERIOD, Period.GetValue());
  
  mRoundsPerTick = ToBelow(mPeriod.GetValue() / (DispatchPeriod / 1000.0));
  if (!mRoundsPerTick)
    mRoundsPerTick = 1;
  return true;
}

bool nglTimer::Start(bool Immediate, bool Reset)
{
  if (mRunning)
  {
    return false;
  }
  
  nglTime now;
  mLastTime = now;
  
  if (mCounter == 0)
    mCounter = mRoundsPerTick;
  mCounter = MIN(mRoundsPerTick, mCounter);
  if (Reset)
  {
    mCallCnt = 0;
    mCounter = mRoundsPerTick;
  }
  
  if (Immediate)
    TimerAction();
  
  mRunning = true;
  return true;
}

void nglTimer::Stop()
{
  mRunning = false;
}

void nglTimer::TimerAction()
{
  nglTime now;
  double elapsed = now.GetValue() - mLastTime.GetValue();
  OnTick(elapsed);
  mLastTime = now;
  mCallCnt++;
}

bool nglTimer::IsRunning()
{
  return mRunning;
}

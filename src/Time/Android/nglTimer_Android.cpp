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


nglTimer::nglTimer(nglTime Period) : mPeriod(Period), mLastTick(0), mNextTick(0)
{
  mCallCnt = 0;
  mRunning = false;

  App->AddTimer (this);
}

nglTimer::~nglTimer()
{
  App->DelTimer (this);
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

  if (Reset)
    mCallCnt = 0;

  mLastTick = now;
  mNextTick = now;
  if (!Immediate)
    mNextTick += mPeriod;

  mRunning = true;
  return true;
}

void nglTimer::Stop()
{
  mRunning = false;
}

bool nglTimer::IsRunning()
{
  return mRunning;
}


/*
 * Internals
 */

nglTime nglTimer::GetTimeOut (nglTime Now)
{
  nglTime timeout = mNextTick - Now;

  return (timeout < nglTime::Zero) ? nglTime::Zero : timeout;
}

void nglTimer::Update()
{
  nglTime now;

  if (now < mNextTick) return;

  CallOnTick(now - mLastTick);

  mLastTick = now;
  mNextTick = now + mPeriod; // Simple reload
}

void nglTimer::CallOnTick(nglTime Elapsed)
{
  OnTick (Elapsed);
}

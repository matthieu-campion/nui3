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
uint32 WM_NGLTIMER = 0;
static volatile bool working = false;

HANDLE nglTimer::mTimerQueueHandle = NULL;
HANDLE nglTimer::mTimerHandle = NULL;

void nglTimer::InitMainTimer()
{
  if (!WM_NGLTIMER)
  {
    WM_NGLTIMER = RegisterWindowMessage(_T("nglTimerMessage"));
  }

  if (mTimerQueueHandle == NULL)
  {
    mTimerQueueHandle = CreateTimerQueue();
    NGL_ASSERT(mTimerQueueHandle);
    BOOL res = CreateTimerQueueTimer(&mTimerHandle, mTimerQueueHandle, TimeProc, NULL, 1, 1, WT_EXECUTEINUITHREAD);
    NGL_ASSERT(res);
  }
}

void nglTimer::FreeMainTimer()
{
  if (mTimers.empty())
  {
    DeleteTimerQueueTimer(mTimerQueueHandle, mTimerHandle, NULL);
//    DeleteTimerQueueEx(mTimerQueueHandle, NULL);
    while (working)
    {
      MSG msg;
      HWND hwnd = App->GetHWnd();

      PeekMessage(&msg, hwnd, WM_NGLTIMER, WM_NGLTIMER, PM_REMOVE);
      Sleep(2);
    }
   DeleteTimerQueueEx(mTimerQueueHandle, INVALID_HANDLE_VALUE);
    mTimerHandle = NULL;
    mTimerQueueHandle = NULL;
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

VOID CALLBACK TimeProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
  nglTimer::PostMessage();
}

bool nglTimer::Start(bool Immediate, bool Reset)
{
  if (mRunning)
    return false;

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

  return mRunning;
}

void nglTimer::Stop()
{
  if (mRunning)
  {
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
      tick = nglTime::GetTime();

      OnTick(tick-mLastTick);

      mLastTick=tick;
      mCounter = mRoundsPerTick;
    }
  }
}

std::list<nglTimer*> nglTimer::mTimers;

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

  return 0;
}

void nglTimer::PostMessage()
{
  if (App && !working)
  {
    OutputDebugString(_T("TimerIn\n"));
    working = true;
    HWND hwnd = App->GetHWnd();
    if (hwnd)
      ::SendMessage(hwnd, WM_NGLTIMER, 0, 0);
    working = false;
    OutputDebugString(_T("TimerOut\n"));
  }
}


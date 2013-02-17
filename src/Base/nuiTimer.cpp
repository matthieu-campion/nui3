/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

////////////////////////
// nuiTickEvent

nuiTickEvent::nuiTickEvent(nglTime Lap)
  : nuiEvent(nuiTimerTick)
{
  mLap = Lap;
}

nglTime nuiTickEvent::GetLap()
{
  return mLap;
}

void nuiTickEvent::SetLap(nglTime Lap)
{
  mLap = Lap;
}


////////////////////////
// nuiTimer
nuiTimer::nuiTimer(nglTime Period)
  : nglTimer(Period)
{
}

nuiTimer::~nuiTimer()
{
}

void nuiTimer::OnTick(nglTime Lap)
{
  Tick(nuiTickEvent(Lap));
}

bool nuiTimer::StartOnNextTick(bool Reset)
{
  nuiTask* pTask = nuiMakeTask(this, &nuiTimer::Start, false, true);
  //nuiTask* pTask = new nuiTask2<bool, bool, bool>(nuiMakeDelegate(this, &nuiTimer::Start), false, true);
  nuiAnimation::RunOnAnimationTick(pTask);
  return true;
}
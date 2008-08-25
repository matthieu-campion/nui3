/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiTimer.h"
#include "nuiEventRegistry.h"

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

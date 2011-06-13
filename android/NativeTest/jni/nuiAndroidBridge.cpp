/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiAndroidBridge.h"

void* gmpNUI_AndroidBridge = 0;



void nuiAndroidBridge::TimerTest()
{
  LOGI("create timer");
  nuiTimer* pTimer = new nuiTimer(1);
  LOGI("timer created %p", pTimer);
  
  
  mEventSink.Connect(pTimer->Tick, &nuiAndroidBridge::OnTimerTick);
  pTimer->Start();
}




void nuiAndroidBridge::OnTimerTick(const nuiEvent& rEvent)
{
  nglTime now;
  LOGI("nuiAndroidBridge::OnTimerTick (time %lf)", now.GetValue());
}
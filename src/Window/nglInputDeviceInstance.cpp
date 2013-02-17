/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"
#include "nglInputDeviceInstance.h"


nglInputDevice* nglInputDeviceInstance::CreateDevice (nglInputDeviceInstance* mpDevice)
{
  // nglInputDevice ctor is private but we are its friend
  return mpDevice ? new nglInputDevice(mpDevice) : NULL;
}

nglInputDeviceInstance::nglInputDeviceInstance()
{
  mPort = "(none)";
  mName = "(none)";
  mRefCnt = 0;
}

nglInputDeviceInstance::~nglInputDeviceInstance()
{
}


bool nglInputDeviceInstance::Update()
{
  return false;
}

nglString nglInputDeviceInstance::GetPort()
{
  return mPort;
}

nglString nglInputDeviceInstance::GetName()
{
  return mName;
}

uint nglInputDeviceInstance::GetButtonCount()
{
  return mButtons.size();
}

bool nglInputDeviceInstance::GetButtonState(uint Index)
{
  return Index < mButtons.size() ? mButtons[Index] : false;
}

uint nglInputDeviceInstance::GetAxeCount()
{
  return mAxes.size();
}

float nglInputDeviceInstance::GetAxeValue(uint Index)
{
  return Index < mAxes.size() ? mAxes[Index] : 0.f;
}

void nglInputDeviceInstance::IncRef()
{
  mRefCnt++;
}

void nglInputDeviceInstance::DecRef()
{
  NGL_ASSERT(mRefCnt > 0);

  mRefCnt--;
  if (mRefCnt == 0)
    delete this;
}

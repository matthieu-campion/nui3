/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"
#include "nglInputDeviceInstance.h"


/*
 * Life cycle
 */

nglInputDevice::nglInputDevice (nglInputDeviceInstance* pDevice)
{

  mpDevice = pDevice;
  mpDevice->IncRef();
}

nglInputDevice::~nglInputDevice()
{
  mpDevice->DecRef();
}


/*
 * Status polling
 */

bool nglInputDevice::Update()
{
  return mpDevice->Update();
}

nglString nglInputDevice::GetPort()
{
  return mpDevice->GetPort();
}

nglString nglInputDevice::GetName()
{
  return mpDevice->GetName();
}

uint nglInputDevice::GetButtonCount()
{
  return mpDevice->GetButtonCount();
}

bool nglInputDevice::GetButtonState(uint Index)
{
  return mpDevice->GetButtonState(Index);
}

uint nglInputDevice::GetAxeCount()
{
  return mpDevice->GetAxeCount();
}

float nglInputDevice::GetAxeValue(uint Index)
{
  return mpDevice->GetAxeValue(Index);
}

void nglInputDevice::Dump(uint Level) const
{
  NGL_LOG(_T("inputdev"), Level, _T("Joystick info"));
  NGL_LOG(_T("inputdev"), Level, _T("  Port    : %s"), mpDevice->GetPort().GetChars());
  NGL_LOG(_T("inputdev"), Level, _T("  Model   : %s"), mpDevice->GetName().GetChars());
  NGL_LOG(_T("inputdev"), Level, _T("  Axis    : %d"), mpDevice->GetAxeCount());
  NGL_LOG(_T("inputdev"), Level, _T("  Buttons : %d"), mpDevice->GetButtonCount());
}


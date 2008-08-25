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

#ifdef NGL_USE_DIRECTINPUT

#include "nglKernel.h"
#include "nglInputDeviceDirectX.h"
#include "nglMath.h"
#include "nglVector.h"


bool nglInputDeviceDirectX::mInitCalled = false;
nglInputDeviceDirectX::DeviceList nglInputDeviceDirectX::mDeviceList;
LPDIRECTINPUT8 nglInputDeviceDirectX::mpDInput = NULL;

BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef)
{
  return ((nglInputDeviceDirectX*)pvRef)->EnumAxesCallback(lpddoi);
}

BOOL CALLBACK EnumButtonsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef)
{
  return ((nglInputDeviceDirectX*)pvRef)->EnumButtonsCallback(lpddoi);
}


BOOL nglInputDeviceDirectX::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi)
{
  DIPROPRANGE diprg; 

  diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
  diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
  diprg.diph.dwHow        = DIPH_BYID; 
  diprg.diph.dwObj        = lpddoi->dwType; 
  diprg.lMin              = -32768; 
  diprg.lMax              = +32768; 
  HRESULT hr = mpDirectInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
  return DIENUM_CONTINUE;
}

BOOL nglInputDeviceDirectX::EnumButtonsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi)
{
  return DIENUM_CONTINUE;
}



nglInputDeviceDirectX::nglInputDeviceDirectX (IDirectInputDevice8* pDeviceInstance) : nglInputDeviceInstance(), nglEvent()
{
  mValid = false;
  mpDirectInputDevice = pDeviceInstance;
  if (!mpDirectInputDevice)
    return;
  mCaps.dwSize = sizeof(DIDEVCAPS);
  mpDirectInputDevice->GetCapabilities(&mCaps);

  // Get number of axes
  mAxes.resize(mCaps.dwAxes + mCaps.dwPOVs * 2);

  // Get number of buttons
  mButtons.resize(mCaps.dwButtons);

  DIDEVICEINSTANCE DevInfo;
  DevInfo.dwSize = sizeof(DIDEVICEINSTANCE);
  mpDirectInputDevice->GetDeviceInfo(&DevInfo);
  // Fetch name
  mName = nglString(DevInfo.tszInstanceName);

  mpDirectInputDevice->SetCooperativeLevel(App->GetHWnd(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
  mpDirectInputDevice->SetDataFormat(&c_dfDIJoystick2);

  mpDirectInputDevice->EnumObjects(::EnumAxesCallback, (VOID*)this, DIDFT_AXIS);
  mpDirectInputDevice->EnumObjects(::EnumButtonsCallback, (VOID*)this, DIDFT_BUTTON);

  // Synthetize port name
  DIPROPSTRING String;
  String.diph.dwSize = sizeof(DIPROPSTRING);
  String.diph.dwHeaderSize = sizeof(DIPROPHEADER);
  String.diph.dwHow = DIPH_DEVICE;
  String.diph.dwObj = 0;
  HRESULT hr = mpDirectInputDevice->GetProperty(DIPROP_GETPORTDISPLAYNAME, &String.diph);

  char string[MAX_PATH];
  WideCharToMultiByte(CP_ACP, 0, String.wsz, -1, string, MAX_PATH, NULL, NULL);
  mPort = nglString(string);


  mValid = true;
}

nglInputDeviceDirectX::~nglInputDeviceDirectX()
{
  mpDirectInputDevice->Release();
}

BOOL CALLBACK nglInputDeviceDirectX::DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi,  LPVOID pvRef)
{
  uint32 i = mDeviceList.size();
  mDeviceList.resize(i + 1);

  IDirectInputDevice8* pDev;
  mpDInput->CreateDevice(lpddi->guidInstance, &pDev, NULL);
  mDeviceList[i] = pDev;

  return DIENUM_CONTINUE;
}

bool nglInputDeviceDirectX::Init()
{
  if (mpDInput && mInitCalled)
    return true;

  HINSTANCE main_instance = App->GetHInstance();

  if (FAILED(DirectInput8Create(main_instance,DIRECTINPUT_VERSION,IID_IDirectInput8A ,(LPVOID*)&mpDInput,NULL)))
  {
    mInitCalled = false;
    return false;// ERROR
  }

  mpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);

  App->AddExit(Exit);
  mInitCalled = true;
  return true;
}

nglInputDevice* nglInputDeviceDirectX::Enum (uint Index)
{
  if (!Init())
    return NULL;

  if (Index >= mDeviceList.size())
    return NULL;

  // If device has no associated instance, create one now
  return CreateDevice(new nglInputDeviceDirectX(mDeviceList[Index]));
}

bool nglInputDeviceDirectX::IsValid()
{
  return mValid;
}

bool nglInputDeviceDirectX::Update()
{
  uint32 i;
  if (!mValid)
    return false;
  mpDirectInputDevice->Poll();
  DIJOYSTATE2 Data;
  mpDirectInputDevice->Acquire();
  mpDirectInputDevice->GetDeviceState(sizeof(DIJOYSTATE2),&Data);
  mpDirectInputDevice->Unacquire();

  for (i = 0; i<mButtons.size(); i++)
    mButtons[i] = Data.rgbButtons[i] ? true : false;

  for (i = 0; i<mCaps.dwAxes; i++)
  {
    if (i<6)
      mAxes[i] = (float)((&Data.lX)[i])/ 32767.f;
    else if (i<12)
      mAxes[i] = (float)((&Data.lVX)[i])/ 32767.f;
    else if (i<18)
      mAxes[i] = (float)((&Data.lAX)[i])/ 32767.f;
    else if (i<24)
      mAxes[i] = (float)((&Data.lFX)[i])/ 32767.f;
  }

  for (i = 0; i<mCaps.dwPOVs; i++)
  {
    if (Data.rgdwPOV[i] >= 0)
    {
      float angle = (float)(Data.rgdwPOV[i])/ (float)(M_PI_2 * 100.);
      nglVector2f vec(sinf(angle),cosf(angle));
      vec.Normalize();
      mAxes[mCaps.dwAxes + i*2] = vec[0];
      mAxes[mCaps.dwAxes + i*2 + 1] = vec[1];
    }
    else
    {
      mAxes[mCaps.dwAxes + i*2] = 0;
      mAxes[mCaps.dwAxes + i*2 + 1] = 0;
    }
  }
  return IsValid();
}

void nglInputDeviceDirectX::Exit()
{
/*
  DeviceList::iterator idev;

  for (idev = mDeviceList.begin(); idev != mDeviceList.end(); ++idev)
  {
    nglInputDeviceDirectX* dev = *idev;

    if (dev)
      delete dev;
  }
*/
}

#endif

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

#ifndef __nglInputDeviceDirectX_h__
#define __nglInputDeviceDirectX_h__

//#include "nui.h"
#include "nglInputDevice.h"
#include "../nglInputDeviceInstance.h"
#include <vector>

#ifdef NGL_USE_DIRECTINPUT
#ifndef DIRECTINPUT_VERSION
  #define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

class nglInputDeviceDirectX : public nglInputDeviceInstance
{
public:
  static nglInputDevice* Enum (uint Index);

  nglInputDeviceDirectX (IDirectInputDevice8* pDevice);
  virtual ~nglInputDeviceDirectX();

  bool IsValid();

protected:
  bool Update();
private:
  typedef std::vector<IDirectInputDevice8*> DeviceList;
  
  static bool       mInitCalled;
  static DeviceList mDeviceList;
  static LPDIRECTINPUT8 mpDInput;

  static BOOL CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi,  LPVOID pvRef);
  IDirectInputDevice8* mpDirectInputDevice;
  bool mValid;

  static bool Init();
  static void Exit();

  friend BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef);
  friend BOOL CALLBACK EnumButtonsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef);

  BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi);
  BOOL CALLBACK EnumButtonsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi);

  DIDEVCAPS mCaps;
};
#endif

#endif // __nglInputDeviceDirectX_h__

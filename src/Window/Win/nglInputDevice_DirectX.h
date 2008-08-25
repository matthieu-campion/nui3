/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
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

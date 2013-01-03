/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiAudioDevice.h"

#include <dsound.h>

class nuiAudioDeviceAPI_DirectSound : public nuiAudioDeviceAPI
{
public:
  nuiAudioDeviceAPI_DirectSound();

  virtual ~nuiAudioDeviceAPI_DirectSound();

  virtual uint32 GetDeviceCount() const;

  virtual nuiAudioDevice* GetDevice(uint32 index);
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName);
  virtual nglString GetDeviceName(uint32 index) const;
  virtual nuiAudioDevice* GetDefaultInputDevice();
  virtual nuiAudioDevice* GetDefaultOutputDevice();


protected:

  class DeviceDesc
  {
  public:
    DeviceDesc()
    {
      mInputGUID = DSDEVID_DefaultCapture;
      mOutputGUID = DSDEVID_DefaultPlayback;
    }

    GUID mInputGUID;
    GUID mOutputGUID;
    nglString mInName;
    nglString mInModule;
    nglString mOutName;
    nglString mOutModule;
  };


  mutable std::vector<DeviceDesc> mDeviceIDs;
  mutable std::map<nglString, int32> mDeviceMap;



private:
  BOOL RealDSEnumInputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule);

  static BOOL CALLBACK DSEnumInputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext);

  BOOL RealDSEnumOutputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule);

  static BOOL CALLBACK DSEnumOutputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext);
  
};

extern nuiAudioDeviceAPI_DirectSound DirectSoundAPI;

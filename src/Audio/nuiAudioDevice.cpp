/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAudioDevice.h"

/////////
// Audio Device:
nuiAudioDevice::nuiAudioDevice()
{
  mAPIName = _T("Unknown");
  mName = _T("Unknown");
  mManufacturer = _T("Unknown");
}

nuiAudioDevice::~nuiAudioDevice()
{
}

const nglString& nuiAudioDevice::GetAPIName() const
{
  return mAPIName;
}

const nglString& nuiAudioDevice::GetName() const
{
  return mName;
}

const nglString& nuiAudioDevice::GetManufacturer() const
{
  return mManufacturer;
}

const std::vector<double>& nuiAudioDevice::GetSampleRates() const
{
  return mSampleRates;
}

const std::vector<uint32>& nuiAudioDevice::GetBufferSizes() const
{
  return mBufferSizes;
}

uint32 nuiAudioDevice::GetInputChannelCount() const
{
  return mInputChannels.size();
}

uint32 nuiAudioDevice::GetOutputChannelCount() const
{
  return mOutputChannels.size();
}

const nglString& nuiAudioDevice::GetInputChannelName(uint32 InputChannel) const
{
  return mInputChannels[InputChannel];
}

const nglString& nuiAudioDevice::GetOutputChannelName(uint32 OutputChannel) const
{
  return mOutputChannels[OutputChannel];
}


bool nuiAudioDevice::IsPresent() const
{
  return mIsPresent;
}

/////////
// Audio Device API:
nuiAudioDeviceAPI::nuiAudioDeviceAPI()
{
  RegisterWithManager();
}

nuiAudioDeviceAPI::~nuiAudioDeviceAPI()
{
}

void nuiAudioDeviceAPI::RegisterWithManager()
{
  nuiAudioDeviceManager::Get().RegisterAPI(this);
}


/////////
// Audio Device Manager:

nuiAudioDeviceManager& nuiAudioDeviceManager::Get()
{
  return mManager;
}

nuiAudioDeviceManager::~nuiAudioDeviceManager()
{
}

#ifdef _CARBON_
  nuiAudioDeviceAPI* GetCoreAudioAPI(); //#FIXME Hack around compiler bug?
#endif

#ifdef _WIN32_
  nuiAudioDeviceAPI* GetDirectSoundAPI(); //#FIXME Hack around compiler bug?
#endif

void nuiAudioDeviceManager::Update()
{


#ifdef _CARBON_
  GetCoreAudioAPI(); //#FIXME Hack around linker strangeness?
#endif

#ifdef _WIN32_
  GetDirectSoundAPI();
#endif



  mDeviceCount = 0;
  for (uint32 i = 0; i < mAPIs.size(); i++)
  {
    uint32 count = mAPIs[i]->GetDeviceCount();
    mDeviceCount += count;
  }
}

uint32 nuiAudioDeviceManager::GetDeviceCount() const
{
  return mDeviceCount;
}

nuiAudioDevice* nuiAudioDeviceManager::GetDevice(uint32 DeviceIndex)
{
  uint32 ApiIndex = DeviceIndex >> 16;
  uint32 DevIndex = DeviceIndex & 0xffff;
  
  return mAPIs[ApiIndex]->GetDevice(DevIndex);
}

nuiAudioDevice* nuiAudioDeviceManager::GetDeviceWithNameAndAPI(const nglString& deviceName, const nglString& apiName)
{
  for (int32 i = 0; i < mDeviceCount; i++)
  {
    nuiAudioDevice* device = GetDevice(i);
    if (device->GetName() == deviceName && device->GetAPIName() == apiName)
      return device;
  }
  return NULL;
}

nuiAudioDevice* nuiAudioDeviceManager::GetDefaultOutputDevice()
{
  return mAPIs[0]->GetDefaultOutputDevice();
}

nuiAudioDevice* nuiAudioDeviceManager::GetDefaultInputDevice()
{
  return mAPIs[0]->GetDefaultInputDevice();
}

nuiAudioDeviceManager::nuiAudioDeviceManager()
{
  mDeviceCount = 0;
}

void nuiAudioDeviceManager::RegisterAPI(nuiAudioDeviceAPI* pAPI)
{
  mAPIs.push_back(pAPI);
}


nuiAudioDeviceManager nuiAudioDeviceManager::mManager;



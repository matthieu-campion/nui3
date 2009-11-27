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
}

nuiAudioDeviceAPI::~nuiAudioDeviceAPI()
{
}

void nuiAudioDeviceAPI::RegisterWithManager(nuiAudioDeviceManager& rManager)
{
  rManager.RegisterAPI(mName, this);
}


/////////
// Audio Device Manager:

nuiAudioDeviceManager& nuiAudioDeviceManager::Get()
{
  static nuiAudioDeviceManager manager;
  return manager;
}

nuiAudioDeviceManager::~nuiAudioDeviceManager()
{
}

void nuiAudioDeviceManager::Update()
{
  mDeviceCount = 0;
  APIMap::const_iterator end = mAPIs.end();
  for (APIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    uint32 count = it->second->GetDeviceCount();
    mDeviceCount += count;
  }
}

uint32 nuiAudioDeviceManager::GetDeviceCount() const
{
  return mDeviceCount;
}

nuiAudioDevice* nuiAudioDeviceManager::GetDevice(uint32 DeviceIndex)
{
  APIMap::const_iterator end = mAPIs.end();
  for (APIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiAudioDeviceAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetDeviceCount();
    if (DeviceIndex >= devcount)
    {
      DeviceIndex -= devcount;
    }
    else
    {
      return pAPI->GetDevice(DeviceIndex);
    }
  }
  NGL_ASSERT(0);
}

nglString nuiAudioDeviceManager::GetDeviceName(uint32 DeviceIndex)
{
  APIMap::const_iterator end = mAPIs.end();
  for (APIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiAudioDeviceAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetDeviceCount();
    if (DeviceIndex >= devcount)
    {
      DeviceIndex -= devcount;
    }
    else
    {
      return pAPI->GetDeviceName(DeviceIndex);
    }
  }
  NGL_ASSERT(0);
}

nglString nuiAudioDeviceManager::GetDeviceAPIName(uint32 DeviceIndex)
{
  APIMap::const_iterator end = mAPIs.end();
  for (APIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiAudioDeviceAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetDeviceCount();
    if (DeviceIndex >= devcount)
    {
      DeviceIndex -= devcount;
    }
    else
    {
      return pAPI->GetAPIName();
    }
  }
  NGL_ASSERT(0);
}

nuiAudioDevice* nuiAudioDeviceManager::GetDeviceWithNameAndAPI(const nglString& rDeviceName, const nglString& rApiName)
{
  APIMap::const_iterator end = mAPIs.end();
  APIMap::const_iterator it = mAPIs.find(rApiName);
  if (it != end)
  {
    return it->second->GetDevice(rDeviceName);
  }
  NGL_ASSERT(0);
}

nuiAudioDevice* nuiAudioDeviceManager::GetDefaultOutputDevice()
{
  return mAPIs.begin()->second->GetDefaultOutputDevice();
}

nuiAudioDevice* nuiAudioDeviceManager::GetDefaultInputDevice()
{
  return mAPIs.begin()->second->GetDefaultInputDevice();
}

nuiAudioDeviceManager::nuiAudioDeviceManager()
{
  mDeviceCount = 0;
  RegisterAPIS();
}

void nuiAudioDeviceManager::RegisterAPI(const nglString& rAPIName, nuiAudioDeviceAPI* pAPI)
{
  NGL_LOG(_T("nuiAudioDeviceManager"), 1, _T("RegisterAPI('%ls') [0x%x]\n"), rAPIName.GetChars(), pAPI);
  APIMap::const_iterator end = mAPIs.end();
  APIMap::const_iterator it = mAPIs.find(rAPIName);
  if (it != end)
  {
    nuiAudioDeviceAPI* pOldAPI = it->second;
    NGL_LOG(_T("nuiAudioDeviceManager"), 1, _T("\tkilling previous entry for this API [0x%p]\n"), pOldAPI);
    delete pOldAPI;
  }
  mAPIs[rAPIName] = pAPI;
  Update();
}

#ifdef _WIN32_
#include "nuiAudioDevice_DirectSound.h"
//#include "nuiAudioDevice_ASIO.h"
void nuiAudioDeviceManager::RegisterAPIS()
{
  DirectSoundAPI.RegisterWithManager(*this);
}
#elif (defined _CARBON_) || (defined _COCOA_)
#include "nuiAudioDevice_CoreAudio.h"
void nuiAudioDeviceManager::RegisterAPIS()
{
  CoreAudioAPI.RegisterWithManager(*this);
}
#elif (defined _UIKIT_)
#include "nuiAudioDevice_AudioUnit.h"
void nuiAudioDeviceManager::RegisterAPIS()
{
  AudioUnitAPI.RegisterWithManager(*this);
}
#else
void nuiAudioDeviceManager::RegisterAPIS()
{
}
#endif


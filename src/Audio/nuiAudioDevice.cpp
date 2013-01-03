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

const std::vector<int32>& nuiAudioDevice::GetBufferSizes() const
{
  return mBufferSizes;
}

int32 nuiAudioDevice::GetInputChannelCount() const
{
  return mInputChannels.size();
}

int32 nuiAudioDevice::GetOutputChannelCount() const
{
  return mOutputChannels.size();
}

const nglString& nuiAudioDevice::GetInputChannelName(int32 InputChannel) const
{
  return mInputChannels[InputChannel];
}

const nglString& nuiAudioDevice::GetOutputChannelName(int32 OutputChannel) const
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
  nuiAudioAPIMap::const_iterator end = mAPIs.end();
  for (nuiAudioAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    int32 count = it->second->GetDeviceCount();
    mDeviceCount += count;
  }
}

int32 nuiAudioDeviceManager::GetDeviceCount() const
{
  return mDeviceCount;
}

nuiAudioDevice* nuiAudioDeviceManager::GetDevice(int32 DeviceIndex)
{
  nuiAudioAPIMap::const_iterator end = mAPIs.end();
  for (nuiAudioAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiAudioDeviceAPI* pAPI = it->second;
    int32 devcount = pAPI->GetDeviceCount();
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

nglString nuiAudioDeviceManager::GetDeviceName(int32 DeviceIndex)
{
  nuiAudioAPIMap::const_iterator end = mAPIs.end();
  for (nuiAudioAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiAudioDeviceAPI* pAPI = it->second;
    int32 devcount = pAPI->GetDeviceCount();
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

nglString nuiAudioDeviceManager::GetDeviceAPIName(int32 DeviceIndex)
{
  nuiAudioAPIMap::const_iterator end = mAPIs.end();
  for (nuiAudioAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiAudioDeviceAPI* pAPI = it->second;
    int32 devcount = pAPI->GetDeviceCount();
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
  nuiAudioAPIMap::const_iterator end = mAPIs.end();
  nuiAudioAPIMap::const_iterator it = mAPIs.find(rApiName);
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
  NGL_LOG(_T("nuiAudioDeviceManager"), NGL_LOG_DEBUG, _T("RegisterAPI('%s') [0x%x]\n"), rAPIName.GetChars(), pAPI);
  nuiAudioAPIMap::const_iterator end = mAPIs.end();
  nuiAudioAPIMap::const_iterator it = mAPIs.find(rAPIName);
  if (it != end)
  {
    nuiAudioDeviceAPI* pOldAPI = it->second;
    NGL_LOG(_T("nuiAudioDeviceManager"), NGL_LOG_DEBUG, _T("\tkilling previous entry for this API [0x%p]\n"), pOldAPI);
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
//#elif (defined _UIKIT_)
//#include "nuiAudioDevice_AudioUnit.h"
//void nuiAudioDeviceManager::RegisterAPIS()
//{
//  AudioUnitAPI.RegisterWithManager(*this);
//}
#elif (defined _ANDROID_)
#include "nuiAudioDevice_Android.h"
void nuiAudioDeviceManager::RegisterAPIS()
{
  AndroidAudioAPI.RegisterWithManager(*this);
}
#else
void nuiAudioDeviceManager::RegisterAPIS()
{
}
#endif


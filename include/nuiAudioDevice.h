/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

class nuiAudioDeviceManager;

typedef nuiFastDelegate3<const std::vector<const float*>&, const std::vector<float*>&, int32> nuiAudioProcessFn; // the params are two arrays of float*: input and output

class nuiAudioDevice
{
public:
  virtual ~nuiAudioDevice();

  virtual bool Open(std::vector<int32>& rInputChannels, std::vector<int32>& rOutputChannels, double SampleRate, int32 BufferSize, nuiAudioProcessFn pProcessFunction) = 0;
  virtual bool Close() = 0;

  const nglString& GetAPIName() const; // CoreAudio / ASIO / DirectSound / MME ...
  const nglString& GetName() const;
  const nglString& GetManufacturer() const;
  const std::vector<double>& GetSampleRates() const;
  const std::vector<int32>& GetBufferSizes() const;
  int32 GetInputChannelCount() const;
  int32 GetOutputChannelCount() const;
  const nglString& GetInputChannelName(int32 InputChannel) const;
  const nglString& GetOutputChannelName(int32 OutputChannel) const;

  bool IsPresent() const;

protected:
  nuiAudioDevice();
  
  nglString mAPIName;
  nglString mName;
  nglString mManufacturer;
  std::vector<double> mSampleRates;
  std::vector<int32> mBufferSizes;
  std::vector<nglString> mInputChannels;
  std::vector<nglString> mOutputChannels;
  bool mIsPresent;
};

class nuiAudioDeviceAPI : public nuiNonCopyable
{
public:
  virtual ~nuiAudioDeviceAPI();
  
  const nglString& GetAPIName() const
  {
    return mName;
  }
  virtual int32 GetDeviceCount() const = 0;
  virtual nglString GetDeviceName(int32 index) const = 0;
  virtual nuiAudioDevice* GetDevice(int32 index) = 0;
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName) = 0;
  virtual nuiAudioDevice* GetDefaultInputDevice() = 0;
  virtual nuiAudioDevice* GetDefaultOutputDevice() = 0;
  
  void RegisterWithManager(nuiAudioDeviceManager& rManager);
protected:
  nuiAudioDeviceAPI();

  nglString mName;
};

typedef std::map<nglString, nuiAudioDeviceAPI*> nuiAudioAPIMap;

class nuiAudioDeviceManager
{
public:
  static nuiAudioDeviceManager& Get();
  virtual ~nuiAudioDeviceManager();
  
  void Update();
  
  nuiSimpleEventSource<0> DeviceConnected;
  nuiSimpleEventSource<0> DeviceDisconnected;

  int32 GetDeviceCount() const;
  nuiAudioDevice* GetDevice(int32 DeviceIndex);
  nglString       GetDeviceName(int32 DeviceIndex);
  nglString       GetDeviceAPIName(int32 DeviceIndex);
  nuiAudioDevice* GetDeviceWithNameAndAPI(const nglString& rDeviceName, const nglString& rApiName);
  nuiAudioDevice* GetDefaultOutputDevice();
  nuiAudioDevice* GetDefaultInputDevice();
  
  
protected:
  nuiAudioDeviceManager();
  void RegisterAPIS();
  
  friend void nuiAudioDeviceAPI::RegisterWithManager(nuiAudioDeviceManager& rManager);
  void RegisterAPI(const nglString& rAPIName, nuiAudioDeviceAPI* pAPI);
  nuiAudioAPIMap mAPIs;
  
  int32 mDeviceCount;
};




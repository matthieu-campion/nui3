/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

typedef nuiFastDelegate3<const std::vector<const float*>&, const std::vector<float*>&, uint32> nuiAudioProcessFn; // the params are two arrays of float*: input and output

class nuiAudioDevice
{
public:
  virtual ~nuiAudioDevice();

  virtual bool Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction) = 0;
  virtual bool Close() = 0;

  const nglString& GetAPIName() const; // CoreAudio / ASIO / DirectSound / MME ...
  const nglString& GetName() const;
  const nglString& GetManufacturer() const;
  const std::vector<double>& GetSampleRates() const;
  const std::vector<uint32>& GetBufferSizes() const;
  uint32 GetInputChannelCount() const;
  uint32 GetOutputChannelCount() const;
  const nglString& GetInputChannelName(uint32 InputChannel) const;
  const nglString& GetOutputChannelName(uint32 OutputChannel) const;

  bool IsPresent() const;

protected:
  nuiAudioDevice();
  
  nglString mAPIName;
  nglString mName;
  nglString mManufacturer;
  std::vector<double> mSampleRates;
  std::vector<uint32> mBufferSizes;
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
  virtual uint32 GetDeviceCount() const = 0;
  virtual nglString GetDeviceName(uint32 index) const = 0;
  virtual nuiAudioDevice* GetDevice(uint32 index) = 0;
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName) = 0;
  virtual nuiAudioDevice* GetDefaultInputDevice() = 0;
  virtual nuiAudioDevice* GetDefaultOutputDevice() = 0;
  
  void RegisterWithManager();
protected:
  nuiAudioDeviceAPI();

  nglString mName;
};

typedef std::map<nglString, nuiAudioDeviceAPI*> APIMap;

class nuiAudioDeviceManager
{
public:
  static nuiAudioDeviceManager& Get();
  ~nuiAudioDeviceManager();
  
  void Update();
  
  nuiSimpleEventSource<0> DeviceConnected;
  nuiSimpleEventSource<0> DeviceDisconnected;

  uint32 GetDeviceCount() const;
  nuiAudioDevice* GetDevice(uint32 DeviceIndex);
  nglString       GetDeviceName(uint32 DeviceIndex);
  nglString       GetDeviceAPIName(uint32 DeviceIndex);
  nuiAudioDevice* GetDeviceWithNameAndAPI(const nglString& rDeviceName, const nglString& rApiName);
  nuiAudioDevice* GetDefaultOutputDevice();
  nuiAudioDevice* GetDefaultInputDevice();
  
  
protected:
  nuiAudioDeviceManager();
  
  friend void nuiAudioDeviceAPI::RegisterWithManager();
  void RegisterAPI(const nglString& rAPIName, nuiAudioDeviceAPI* pAPI);
  APIMap mAPIs;
  
  int32 mDeviceCount;
};


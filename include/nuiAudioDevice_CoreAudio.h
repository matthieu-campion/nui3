/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiAudioDevice.h"
#include "nglThread.h"
#import <CoreAudio/CoreAudio.h>

class nuiAudioDevice_CoreAudio : public nuiAudioDevice
{
public:
  nuiAudioDevice_CoreAudio(AudioDeviceID id);
  
  virtual ~nuiAudioDevice_CoreAudio();
  
  virtual bool Open(std::vector<int32>& rInputChannels, std::vector<int32>& rOutputChannels, double SampleRate, int32 BufferSize, nuiAudioProcessFn pProcessFunction);
  
  virtual bool Close();
  
  
protected:
  AudioDeviceID mDeviceID;

  static OSStatus IOProc(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                         AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime, void* inClientData);
  OSStatus Process(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                   AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime);
  
  void EnumSampleRates();
  void EnumBufferSizes();
  nglString GetChannelName(bool IsInput, int32 index) const;
  void EnumChannels();
  UInt32  GetPropertyDataSize(UInt32 inChannel, bool inSection, AudioHardwarePropertyID inPropertyID) const;
  void GetPropertyData(UInt32 inChannel, bool inSection, AudioHardwarePropertyID inPropertyID, UInt32& ioDataSize, void* outData) const;
  void SetPropertyData(UInt32 inChannel, bool inSection, AudioHardwarePropertyID inPropertyID, UInt32 inDataSize, const void* inData, const AudioTimeStamp* inWhen);
  
  nuiAudioProcessFn mAudioProcessFn;
  std::vector<int32> mActiveInputChannels;
  std::vector<int32> mActiveOutputChannels;
  
  std::vector<std::vector<float> > mInputSamples;
  std::vector<std::vector<float> > mOutputSamples;

  std::vector<const float*> mInputBuffers;
  std::vector<float*> mOutputBuffers;
  
  double mSampleRate;
  int32 mBufferSize;

  std::vector<std::pair<int32, int32> > mInMap;
  std::vector<std::pair<int32, int32> > mOutMap;
};




class nuiAudioDeviceAPI_CoreAudio : public nuiAudioDeviceAPI
{
public:
  nuiAudioDeviceAPI_CoreAudio();
  virtual ~nuiAudioDeviceAPI_CoreAudio();
  
  virtual int32 GetDeviceCount() const;
  
  virtual nuiAudioDevice* GetDevice(int32 index);
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName);
  virtual nglString GetDeviceName(int32 index) const;
  virtual nuiAudioDevice* GetDefaultOutputDevice();
  virtual nuiAudioDevice* GetDefaultInputDevice();
  
protected:
  mutable std::vector<AudioDeviceID> mDeviceIDs;

};

extern nuiAudioDeviceAPI_CoreAudio CoreAudioAPI;




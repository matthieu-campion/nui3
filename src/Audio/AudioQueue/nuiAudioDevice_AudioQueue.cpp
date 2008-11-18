/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiAudioDevice.h"
#include "nglThread.h"
#include "nuiAudioDevice_AudioQueue.h"

//class nuiAudioDevice_AudioQueue : public nuiAudioDevice
nuiAudioDevice_AudioQueue::nuiAudioDevice_AudioQueue()
{  
  EnumSampleRates();
  EnumBufferSizes();
}

nuiAudioDevice_AudioQueue::~nuiAudioDevice_AudioQueue()
{
}

void nuiAudioDevice_AudioQueue::OutputCallback(void* inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
  
}

bool nuiAudioDevice_AudioQueue::Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction)
{
  OSStatus err;
  
  AudioStreamBasicDescription inFormat;
  inFormat.mSampleRate = SampleRate;
  inFormat.mFormatID = kAudioFormatLinearPCM;
  inFormat.mFormatFlags = kLinearPCMFormatFlagIsFloat | kAudioFormatFlagIsPacked;
  inFormat.mChannelsPerFrame = rOutputChannels.size();
  inFormat.mFramesPerPacket = BufferSize;
  inFormat.mBytesPerFrame = 4;
  inFormat.mBytesPerPacket = inFormat.mBytesPerFrame * inFormat.mFramesPerPacket;
  inFormat.mBitsPerChannel = 8 * inFormat.mBytesPerFrame / inFormat.mChannelsPerFrame;
  inFormat.mReserved = 0;
  err = AudioQueueNewOutput(&inFormat, &nuiAudioDevice_AudioQueue::OutputCallback , this, NULL, NULL, 0, &mOutQueue);
  return false;
}

bool nuiAudioDevice_AudioQueue::Close()
{
  return true;
}
    
    
void nuiAudioDevice_AudioQueue::EnumSampleRates()
{
}

void nuiAudioDevice_AudioQueue::EnumBufferSizes()
{
}

nglString nuiAudioDevice_AudioQueue::GetChannelName(bool IsInput, uint32 index) const
{
  if (IsInput || index > 0)
    return nglString::Null;
  
  return _T("Output");
}



//class nuiAudioDeviceAPI_AudioQueue : public nuiAudioDeviceAPI
nuiAudioDeviceAPI_AudioQueue::nuiAudioDeviceAPI_AudioQueue()
{
}

nuiAudioDeviceAPI_AudioQueue::~nuiAudioDeviceAPI_AudioQueue()
{
}


uint32 nuiAudioDeviceAPI_AudioQueue::GetDeviceCount() const
{
  return 1;
}


nuiAudioDevice* nuiAudioDeviceAPI_AudioQueue::GetDevice(uint32 index)
{
  if (index == 0)
    return new nuiAudioDevice_AudioQueue();
  return NULL;
}

nuiAudioDevice* nuiAudioDeviceAPI_AudioQueue::GetDevice(const nglString& rDeviceName)
{
  if (index == 0)
    return new nuiAudioDevice_AudioQueue();
  return NULL;
}

nglString nuiAudioDeviceAPI_AudioQueue::GetDeviceName(uint32 index) const
{
  if (index == 0)
    return nglString(_T("Output"));
  return nglString::Null;
}

nuiAudioDevice* nuiAudioDeviceAPI_AudioQueue::GetDefaultOutputDevice()
{
  return GetDevice(0);
}

nuiAudioDevice* nuiAudioDeviceAPI_AudioQueue::GetDefaultInputDevice()
{
  return GetDevice(0);
}


nuiAudioDeviceAPI_AudioQueue AudioQueueAPI;

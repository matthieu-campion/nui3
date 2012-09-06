/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiAudioDevice.h"
#include "nglThread.h"
#include <AudioToolbox/AudioToolbox.h>



class nuiAudioDevice_AudioUnit : public nuiAudioDevice
{
public:
  nuiAudioDevice_AudioUnit();
  
  virtual ~nuiAudioDevice_AudioUnit();
  
  virtual bool Open(std::vector<int32>& rInputChannels, std::vector<int32>& rOutputChannels, double SampleRate, int32 BufferSize, nuiAudioProcessFn pProcessFunction);
  
  virtual bool Close();
  
  void Process(uint uNumFrames, AudioBufferList* ioData);
  void ProcessInput(AudioUnitRenderActionFlags* ioActionFlags,
                    const AudioTimeStamp* inTimeStamp,
                    UInt32 inBusNumber,
                    uint uNumFrames, AudioBufferList* ioData);
  
protected:
  
  void EnumSampleRates();
  void EnumBufferSizes();
  nglString GetChannelName(bool IsInput, int32 index) const;
  
  
  
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

  AudioUnit mAudioUnit;
  AUGraph mProcessingGraph;
  AudioBufferList* mpIData;
};




class nuiAudioDeviceAPI_AudioUnit : public nuiAudioDeviceAPI
{
public:
  nuiAudioDeviceAPI_AudioUnit();
  virtual ~nuiAudioDeviceAPI_AudioUnit();
  
  virtual int32 GetDeviceCount() const;
  
  virtual nuiAudioDevice* GetDevice(int32 index);
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName);
  virtual nglString GetDeviceName(int32 index) const;
  virtual nuiAudioDevice* GetDefaultOutputDevice();
  virtual nuiAudioDevice* GetDefaultInputDevice();
};

extern nuiAudioDeviceAPI_AudioUnit AudioUnitAPI;

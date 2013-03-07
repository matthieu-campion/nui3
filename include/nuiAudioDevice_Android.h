/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiAudioDevice.h"
#include "nglThread.h"

#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

class nuiAudioDevice_Android : public nuiAudioDevice
{
public:
  nuiAudioDevice_Android();

  virtual ~nuiAudioDevice_Android();

  virtual bool Open(std::vector<int32>& rInputChannels, std::vector<int32>& rOutputChannels, double SampleRate, int32 BufferSize, nuiAudioProcessFn pProcessFunction);

  virtual bool Close();


protected:

  void EnumSampleRates();
  void EnumBufferSizes();

  void Process();

  nuiAudioProcessFn mAudioProcessFn;
  std::vector<int32> mActiveInputChannels;
  std::vector<int32> mActiveOutputChannels;

  std::vector<std::vector<float> > mInputSamples;
  std::vector<std::vector<float> > mOutputSamples;
  std::vector<int16> mOutputInterleaved;

  double mSampleRate;
  int32 mBufferSize;


  SLObjectItf mpEngine;
  SLEngineItf mpEngineItf;
  SLObjectItf mpOutputMix;
  SLObjectItf mpPlayer;
  SLPlayItf mpPlayItf;
  SLAndroidSimpleBufferQueueItf mpBufferQueue;

  friend void nuiAudioDevice_Android_IOProc(SLAndroidSimpleBufferQueueItf bufferqueue, void* context);


  nglTime mLast;
};




class nuiAudioDeviceAPI_Android : public nuiAudioDeviceAPI
{
public:
  nuiAudioDeviceAPI_Android();
  virtual ~nuiAudioDeviceAPI_Android();

  virtual int32 GetDeviceCount() const;

  virtual nuiAudioDevice* GetDevice(int32 index);
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName);
  virtual nglString GetDeviceName(int32 index) const;
  virtual nuiAudioDevice* GetDefaultOutputDevice();
  virtual nuiAudioDevice* GetDefaultInputDevice();

protected:

};

extern nuiAudioDeviceAPI_Android AndroidAudioAPI;




/*
 *  AudioEngine.h
 *  AudioPlayer
 *
 *  Created by mat on 9/24/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#pragma once

#include "nui.h"
#include "nuiAudioDevice.h"
#include "nuiAudioDecoder.h"

class AudioEngine : public nuiObject
{
public:
  AudioEngine(double SampleRate, uint32 BufferSize);
  virtual ~AudioEngine();
  
  bool LoadFile(nglPath FilePath);
  
  void Play();
  void Pause();
  bool IsPlaying() const;
  
  uint64 GetSampleFrames() const;
  uint64 GetPosition() const;
  void SetPosition(uint64 position);
  double GetSampleRate() const;
  
  float GetGain();
  void SetGain(float gain);
  
  bool IsMute();
  void SetMute(bool mute);
  
private:
  void Process(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames);
  void InitAttributes();
  
  nuiAudioDevice* mpAudioDevice;
  
  nglPath mInPath;
  nglIStream* mpIStream;
  nuiSampleReader* mpReader;
  nuiSampleInfo mInfo;
  
  bool mPlaying;
  double mSampleRate;
  uint32 mBufferSize;
  
  float mGain;
  bool mMute;
  
  nglCriticalSection mCs;
};

/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once

#include "nui.h"
#include "nuiAudioDevice.h"
#include "nuiAudioDecoder.h"
#include "nglRingBuffer.h"


class nuiVoice;
class nuiSound;

class nuiAudioEngine : public nuiObject
{
public:
  nuiAudioEngine(double SampleRate, uint32 BufferSize);
  virtual ~nuiAudioEngine();

  nuiVoice* PlaySound(const nglPath& path);
  nuiVoice* PlaySound(nuiSound* pSound);
  void StopSound(nuiVoice* pnuiVoice);
  
  float GetGain();
  void SetGain(float gain);
  float GetGainDb();
  void SetGainDb(float Db);

  bool IsMuted();
  void SetMute(bool mute);
  
  float GetPan();
  void SetPan(float pan);
  
  bool IsPlaying();
  void Play();
  void Pause();
  
    
protected:
  void ProcessAudioOutput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames);
  bool AudioInit();
  
  void SetPlay(bool play);

  void InitAttributes();
  
  double mSampleRate;
  uint32 mBufferSize;

  float mGain;
  bool mMute;
  float mPan;
  
  bool mPlaying;
  
  nuiAudioDevice* mpOutAudioDevice;
  
  nglCriticalSection mCs;

  std::vector<nuiVoice*> mVoices;
  std::vector<nuiVoice*> mAddVoicesQueue;
  std::vector<nuiVoice*> mRemoveVoicesQueue;
};

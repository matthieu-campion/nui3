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

#include "nuiSound.h"



class nuiVoice;
class nuiAudioDevice;

class nuiAudioEngine : public nuiObject
{
public:
  typedef nuiFastDelegate2<const std::vector<float*>&, uint32> OutputDelegate;
  typedef nuiFastDelegate2<const std::vector<const float*>&, uint32> InputDelegate;
  
  enum ChannelConfig
  {
    eNone = 0,
    eMono,
    eStereo
  };
  
  nuiAudioEngine(double SampleRate, uint32 BufferSize, ChannelConfig inputConfig = eNone);
  virtual ~nuiAudioEngine();
  
  double GetSampleRate() const;
  uint32 GetBufferSize() const;
  
  void SetInputProcessDelegate(const nuiAudioEngine::InputDelegate& rDelegate);
  void SetOutputProcessDelegate(const nuiAudioEngine::OutputDelegate& rDelegate);
  
  void UnsetInputProcessDelegate();
  void UnsetOutputProcessDelegate();

  void DeactivateOutputDevice();
  bool ActivateOutputDevice();
  void DeactivateInputDevice();
  bool ActivateInputDevice(ChannelConfig inputConfig);
  


  nuiVoice* PlaySound(const nglPath& path, nuiSound::Type type = nuiSound::eStream);
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
  void ProcessAudioInput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames);
  bool AudioInit(ChannelConfig config);
  
  void SetPlay(bool play);

  void InitAttributes();
  
  double mSampleRate;
  uint32 mBufferSize;

  float mGain;
  bool mMute;
  float mPan;
  
  bool mPlaying;
  
  nuiAudioDevice* mpOutAudioDevice;
  nuiAudioDevice* mpInAudioDevice;
  
  bool mInputDelegateSet;
  bool mOutputDelegateSet;
  nuiAudioEngine::InputDelegate mInputDelegate;
  nuiAudioEngine::OutputDelegate mOutputDelegate;
  
  nglCriticalSection mCs;

  std::vector<nuiVoice*> mVoices;
  std::vector<nuiVoice*> mAddVoicesQueue;
  std::vector<nuiVoice*> mRemoveVoicesQueue;
};

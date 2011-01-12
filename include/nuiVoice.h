/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSampleReader.h"

class nuiVoice : public nuiObject
{
public: 
  friend class nuiSound;
  
  nuiVoice& operator=(const nuiVoice& rVoice);
  
  bool IsValid() const;
  bool IsDone() const;
  
  void Process(const std::vector<float*>& rOutput, uint32 SampleFrames);
  
  void Play();
  void Pause();
  bool IsPlaying() const;
  
  void FadeIn(uint32 length);
  void FadeOut(uint32 length);
  
  void SetLoop(bool loop);
  bool IsLooping();
  
  uint64 GetSampleFrames() const;
  
  int64 GetPosition() const;
  void SetPosition(int64 position);
  
  float GetGainDb() const;
  void SetGainDb(float Db);
  float GetGain() const;
  void SetGain(float gain);
  
  bool IsMute() const;
  void SetMute(bool mute);
  
  float GetPan() const;
  void SetPan(float pan);
  
private:
  nuiVoice(nuiSound* pSound = NULL);
  nuiVoice(const nuiVoice& rVoice);
  
  virtual ~nuiVoice();
  
  void InitAttributes();
  
  bool Load();
  
  void SetPlay(bool play);
  
  nuiSound* mpSound;
  
  nglIStream* mpStream;
  nuiSampleReader* mpReader;
  nuiSampleInfo mInfo;
  
  bool mPlay;
  bool mDone;
  bool mLoop;
  bool mMute;
  float mGain;
  float mPan;
  int64 mPosition;
  
  bool mFadingIn;
  uint32 mFadeInPosition;
  uint32 mFadeInLength;
  
  bool mFadingOut;
  uint32 mFadeOutPosition;
  uint32 mFadeOutLength;
  
  nglCriticalSection mCs;
};
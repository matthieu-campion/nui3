/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiVoiceEvent
{
public:
  enum Type
  {
    FadeOut,
    FadeIn
  };
  nuiVoiceEvent(Type EventType, int64 position, int64 param = 0);
  
  Type mType;
  int64 mPosition;
  int64 mParam;
};


class nuiVoice : public nuiObject
{
public:   
  nuiVoice(const nuiVoice& rVoice);
  nuiVoice& operator=(const nuiVoice& rVoice);
  
  virtual bool IsValid() const = 0;
  bool IsDone() const;
  
  void Process(const std::vector<float*>& rOutput, int32 SampleFrames);
  
  void Play();
  void Pause();
  bool IsPlaying() const;
  
  void FadeIn(int32 length);
  void FadeOut(int32 length);
  
  void SetLoop(bool loop);
  bool IsLooping();
  
//  virtual int64 GetSampleFrames() const = 0;
  virtual int32 GetChannels() const = 0;
  
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
  
  void PostEvent(const nuiVoiceEvent& rEvent);
protected:
  virtual int32 ReadSamples(const std::vector<float*>& rOutput, int64 position, int32 SampleFrames) = 0;
  void ProcessInternal(const std::vector<float*>& rOutput, int32 SampleFrames);

  nuiVoice(nuiSound* pSound = NULL);  
  virtual ~nuiVoice();
  
  void InitAttributes();
  
  void SetPlay(bool play);
  
  virtual void SetPositionInternal(int64 position);
  
  nuiSound* mpSound;
  
  nuiSampleInfo mInfo;
  
  bool mPlay;
  bool mDone;
  bool mLoop;
  bool mMute;
  float mGain;
  float mPan;
  int64 mPosition;
  
  bool mFadingIn;
  int32 mFadeInPosition;
  int32 mFadeInLength;
  
  bool mFadingOut;
  int32 mFadeOutPosition;
  int32 mFadeOutLength;
  
  nglCriticalSection mCs;
  
  std::vector<nuiVoiceEvent> mEvents;
  nglCriticalSection mEventCs;
  
};



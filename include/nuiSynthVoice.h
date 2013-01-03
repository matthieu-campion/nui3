/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once
#include "nui.h"
#include "nuiSynthSound.h"

class nuiSynthVoice : public nuiVoice
{
public:
  friend class nuiSynthSound;
  
  nuiSynthVoice(const nuiSynthVoice& rVoice);
  nuiSynthVoice& operator=(const nuiSynthVoice& rVoice);
  
  virtual bool IsValid() const;
  virtual int32 GetChannels() const;
  
  
  nuiSynthSound::SignalType GetType() const;
  void SetType(nuiSynthSound::SignalType type);
  
  float GetFreq() const;
  void SetFreq(float freq);
  
  float GetPhase() const;
  void SetPhase(float phase);
  
  
protected:
  virtual int32 ReadSamples(const std::vector<float*>& rOutput, int64 position, int32 SampleFrames);
  
private:
  nuiSynthVoice(double SampleRate, double releaseTime, nuiSynthSound* pSound = NULL);
  virtual ~nuiSynthVoice();
  
  virtual void SetPositionInternal(int64 position);
  void UpdateWavePosition(float toadd);
  
  double mSampleRate;
  nuiSynthSound* mpSynthSound;
  
  nuiSynthSound::SignalType mSignalType;
  float mFreq;
  float mPhase;
  
  float mWavePosition;
  float mIncr;
  
  int64 mLength;
  
  float mLevel;
  float mLevelCoeff;
};
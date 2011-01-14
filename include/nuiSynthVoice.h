/*
 *  nuiSynthVoice.h
 *  nui3
 *
 *  Created by  mat on 1/14/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
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
  virtual uint32 GetChannels() const;
  
  
  nuiSynthSound::SignalType GetType() const;
  void SetType(nuiSynthSound::SignalType type);
  
  float GetFreq() const;
  void SetFreq(float freq);
  
  float GetPhase() const;
  void SetPhase(float phase);
  
  
protected:
  virtual uint32 ReadSamples(const std::vector<float*>& rOutput, int64 position, uint32 SampleFrames);
  
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
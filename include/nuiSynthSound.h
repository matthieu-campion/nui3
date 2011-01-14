/*
 *  nuiSynthSound.h
 *  nui3
 *
 *  Created by  mat on 1/14/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#pragma once
#include "nui.h"

#define SYNTH_SOUND_TABLE_SIZE 2048

class nuiSynthSound : public nuiSound
{
public:
  friend class nuiSoundManager;
  friend class nuiSynthVoice;
  
  enum SignalType 
  {
    eSinus = 0,
    eTriangle,
    eSaw,
    eSquare,
    eLastType
  };
  
  double GetSampleRate() const;
  void SetSampleRate(double samplerate);
  
  double GetReleaseTime() const;
  void SetReleaseTime(double releaseTime);
  
  nuiSynthSound::SignalType GetType() const;
  void SetType(nuiSynthSound::SignalType type);
  
  float GetFreq() const;
  void SetFreq(float freq);
  
  float GetPhase() const;
  void SetPhase(float phase);
  
protected:
  nuiSynthSound(double sampleRate = 44100, double releaseTime = 1);
  virtual ~nuiSynthSound();
  
  virtual nuiVoice* GetVoiceInternal();
  
  typedef std::vector<float> Wave;
  typedef std::vector<Wave> WaveTable;
  static std::vector<WaveTable> mWaveTables;
  static void InitTables();
  
  double mSampleRate;
  double mReleaseTime;
  float mFreq;
  float mPhase;
  SignalType mSignalType;
};
/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiSynthVoice::nuiSynthVoice(double SampleRate, double releaseTime, nuiSynthSound* pSound)
: nuiVoice(pSound),
  mSampleRate(SampleRate),
  mpSynthSound(pSound),
  mSignalType(nuiSynthSound::eSinus),
  mFreq(200),
  mPhase(0),
  mWavePosition(0),
  mIncr(0),
  mLength(releaseTime * SampleRate)
{
  mIncr = (float)SYNTH_SOUND_TABLE_SIZE * (mFreq / mSampleRate);
  mWavePosition = mPhase * (float)SYNTH_SOUND_TABLE_SIZE;
  
  mLevel = 1;
  mLevelCoeff = (log(0.001f) - log(1.f)) / (float)mLength;
}

nuiSynthVoice::~nuiSynthVoice()
{
  
}

nuiSynthVoice::nuiSynthVoice(const nuiSynthVoice& rVoice)
: nuiVoice(rVoice),
  mpSynthSound(NULL)
{
  *this = rVoice;
}

nuiSynthVoice& nuiSynthVoice::operator=(const nuiSynthVoice& rVoice)
{
  this->nuiVoice::operator=(rVoice);
  mSampleRate = rVoice.mSampleRate;
  mpSynthSound = rVoice.mpSynthSound;
  mSignalType = rVoice.mSignalType;
  mFreq = rVoice.mFreq;
  mPhase = rVoice.mPhase;
  mWavePosition = rVoice.mWavePosition;
  mIncr = rVoice.mIncr;
  mLength = rVoice.mLength;
  mLevel = rVoice.mLevel;
  mLevelCoeff = rVoice.mLevelCoeff;
  
  return *this;
}

bool nuiSynthVoice::IsValid() const
{
  return (mpSound != NULL) && (mSignalType < nuiSynthSound::eLastType) && (mFreq > 0) && (mPhase >= 0) && (mPhase <= 1);
}

int32 nuiSynthVoice::GetChannels() const
{
  return 1;
}

nuiSynthSound::SignalType nuiSynthVoice::GetType() const
{
  return mSignalType;
}

void nuiSynthVoice::SetType(nuiSynthSound::SignalType type)
{
  mSignalType = type;
}

float nuiSynthVoice::GetFreq() const
{
  return mFreq;
}

void nuiSynthVoice::SetFreq(float freq)
{
  mFreq = freq;
  mIncr = SYNTH_SOUND_TABLE_SIZE * (mFreq / mSampleRate);
}

float nuiSynthVoice::GetPhase() const
{
  return mPhase;
}

void nuiSynthVoice::SetPhase(float phase)
{
  float diff = (phase - mPhase) * SYNTH_SOUND_TABLE_SIZE;
  mPhase = phase;
  UpdateWavePosition(diff);
}

void nuiSynthVoice::SetPositionInternal(int64 position)
{
  int64 diff = position - mPosition;
  
  float samplesPerCycle = mSampleRate / mFreq;
  float waveDiff = diff / samplesPerCycle * (float)SYNTH_SOUND_TABLE_SIZE;
  UpdateWavePosition(waveDiff);
}

void nuiSynthVoice::UpdateWavePosition(float toadd)
{
  mWavePosition += toadd;
  while (mWavePosition >= SYNTH_SOUND_TABLE_SIZE) 
    mWavePosition -= SYNTH_SOUND_TABLE_SIZE;
  while (mWavePosition < 0)
    mWavePosition += SYNTH_SOUND_TABLE_SIZE;
}

int32 nuiSynthVoice::ReadSamples(const std::vector<float*>& rOutput, int64 position, int32 SampleFrames)
{
  if (mPosition > mLength)
    return 0;
  
  int32 todo = MIN(SampleFrames, mLength - mPosition);
  
  int posInt;
  float interp;
  float val;
  nuiSynthSound::Wave& rWave = nuiSynthSound::mWaveTables[(int)mSignalType][0];
  for (int32 i = 0; i < todo; i++)
  {
    posInt = ToBelow(mWavePosition);
    interp = mWavePosition - posInt;
    mLevel += mLevelCoeff * mLevel;
    val = interp * rWave[posInt] + (1.f - interp) * rWave[posInt + 1];
    val *= mLevel;
    for (int32 c = 0; c < rOutput.size(); c++)
    {
      rOutput[c][i] = val;
    }
      
    UpdateWavePosition(mIncr);
  }
  
  return todo;
}
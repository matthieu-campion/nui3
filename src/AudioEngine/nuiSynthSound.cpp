/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


nuiSynthSound::nuiSynthSound(double sampleRate, double releaseTime)
: mSampleRate(sampleRate),
  mReleaseTime(releaseTime),
  mFreq(200),
  mPhase(0),
  mSignalType(eSinus)
{
  mType = eSynth;
  
  if (mWaveTables.empty())
    InitTables();
  
  mID.Format("%p%s", this, nglString(SYNTH_SUFFIX).GetChars());
}

nuiSynthSound::~nuiSynthSound()
{
  
}

nuiVoice* nuiSynthSound::GetVoiceInternal()
{
  nuiSynthVoice* pVoice = new nuiSynthVoice(mSampleRate, mReleaseTime, this);
  pVoice->SetFreq(mFreq);
  pVoice->SetPhase(mPhase);
  pVoice->SetType(mSignalType);
  return pVoice;
}

double nuiSynthSound::GetSampleRate() const
{
  return mSampleRate;
}

void nuiSynthSound::SetSampleRate(double samplerate)
{
  mSampleRate = samplerate;
}

double nuiSynthSound::GetReleaseTime() const
{
  return mReleaseTime;
}

void nuiSynthSound::SetReleaseTime(double releaseTime)
{
  mReleaseTime = releaseTime;
}

nuiSynthSound::SignalType nuiSynthSound::GetType() const
{
  return mSignalType;
}

void nuiSynthSound::SetType(nuiSynthSound::SignalType type)
{
  mSignalType = type;
}

float nuiSynthSound::GetFreq() const
{
  return mFreq;
}

void nuiSynthSound::SetFreq(float freq)
{
  mFreq = freq;
}

float nuiSynthSound::GetPhase() const
{
  return mPhase;
}

void nuiSynthSound::SetPhase(float phase)
{
  mPhase = phase;
}



std::vector<nuiSynthSound::WaveTable> nuiSynthSound::mWaveTables;


void nuiSynthSound::InitTables()
{
  if (!mWaveTables.empty())
    return;
  
  for (int t = 0; t < eLastType; t++)
  {
    WaveTable table;
    
    
    std::vector<float> wave(SYNTH_SOUND_TABLE_SIZE + 1, 0.f);
    
    int size = SYNTH_SOUND_TABLE_SIZE;
    int size_1_2 = size / 2;
    int size_1_4 = size / 4;
    int size_3_4 = 3 * (size / 4);
    int i;
    float incr;
    float p;
    SignalType type = (SignalType)t;   
    switch (type) 
    {
        // Sinus
      case eSinus:
        for (i = 0; i < size; i++)
        {
          float p = ((float)i) * 2 * M_PI / (float)size;
          wave[i] = sin(p);
        }
        wave[i] = wave[0];
        
        break;
        
        
        // Square
      case eSquare:
        for (i = 0; i < size_1_2; i++)
        {
          wave[i] = 1.0f;
          wave[i + size_1_2] = -1.0f;
        }
        wave[i] = wave[0];
        break;
        
        
        // Triangle
      case eTriangle:
        incr = 1.0f / size_1_4;
        p = 0;
        for (i = 0; i < size_1_4; i++)
        {
          p = ((float)i) * incr;
          wave[i] = p;
          wave[i + size_3_4] = - p;
        }        
        for (i = 0; i < size_1_2; i++)
        {
          p = 1.0f - ((float)i) * incr;
          wave[size_1_4 + i] = p;
        }
        wave[size  +1] = wave[0];
        break;
        
        
        // Saw
      case eSaw:
        for (i = 0; i < size; i++)
        {
          float p = 1.0 - (((float)i) * (1.0f/(float)size));
          wave[i] = p;
        }
        wave[i] = wave[0];
        break;
        
        
      case eLastType:
      default:
        break;
    }
    
    table.push_back(wave);
    mWaveTables.push_back(table);
  }
}
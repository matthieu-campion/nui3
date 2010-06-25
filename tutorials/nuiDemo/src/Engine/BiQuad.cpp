/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "BiQuad.h"

BiQuad::BiQuad(float SampleRate, uint32 NbChannels)
: mSampleRate(SampleRate),
  mChannels(NbChannels)
{
  mN0.resize(mChannels);
  mN1.resize(mChannels);
  mN2.resize(mChannels);
  mN3.resize(mChannels);

  mType = (Type)-1;
  
  mOmega = 0;
  mFreq = 500;
  mQ = 0;
  mA = 0;
  mGain = 0;

  mChanged = true;
  
  Reset();
}

BiQuad::~BiQuad()
{
}


void BiQuad::SetQ(float q)
{
  mQ = q;
  mChanged = true; 
}

void BiQuad::SetFreq(float freq)
{
  mFreq = freq;
  mChanged = true; 
}

void BiQuad::SetGain(float gain)
{
  mGain = gain;
  mChanged = true; 
}

void BiQuad::SetSampleRate(double SampleRate)
{
  mSampleRate = SampleRate;
  mChanged = true; 
}

float BiQuad::GetQ()
{
  return mQ;
}

float BiQuad::GetFreq()
{
  return mFreq;
}

float BiQuad::GetGain()
{
  return mGain;
}

void BiQuad::GetACoefs(float &rA0, float &rA1, float &rA2) const
{
  rA0 = mA0;
  rA1 = mA1;
  rA2 = mA2;
}

void BiQuad::GetBCoefs(float &rB0, float &rB1, float &rB2) const
{
  rB0 = mB0;
  rB1 = mB1;
  rB2 = mB2;
}

//void BiQuad::Process(std::vector<std::vector<float> >& OutSamples, const std::vector<std::vector<float> >& InSamples)
//{
//  for (uint32 i = 0; i < mChannels; i++)
//  {
//    const float sample = pInSamples[i];
//    const float n0 = mN0[i];
//    
//    float calc = mB0 * sample;
//    mN0[i] = sample;
//    
//    calc += mB1 * n0;
//    calc += mB2 * mN1[i];
//    mN1[i] = n0; // input delay line
//    
//    const float n2 = mN2[i];
//    calc -= mA1 * n2;
//    calc -= mA2 * mN3[i];
//    mN3[i] = n2; // output delay line
//    
//    mN2[i] = calc;
//    
//    pOutSamples[i] = calc;
//  }
//}

void BiQuad::Process(std::vector<float*>& OutSamples, const std::vector<float*>& InSamples, uint32 SampleFrames)
{
  for (uint32 s = 0; s < SampleFrames; s++)
  {
    for (uint32 c = 0; c < mChannels; c++)
    {
      const float sample = InSamples[c][s];
      const float n0 = mN0[c];
      
      float calc = mB0 * sample;
      mN0[c] = sample;
      
      calc += mB1 * n0;
      calc += mB2 * mN1[c];
      mN1[c] = n0; // input delay line
      
      const float n2 = mN2[c];
      calc -= mA1 * n2;
      calc -= mA2 * mN3[c];
      mN3[c] = n2; // output delay line
      
      mN2[c] = calc;
      
      OutSamples[c][s] = calc;
    }
    
  }
}

void BiQuad::ComputeCoefficients()
{
  if (!mChanged)
    return;
  
  mOmega = 2.f * (float)M_PI * mFreq / mSampleRate;
  mSin = sinf(mOmega);
  mCos = cosf(mOmega);
    
  switch (mType)
  {
    case LowPass:
      ComputeCoefs_LowPass();
      break;
    case HighPass:
      ComputeCoefs_HighPass();
      break;
    case BandPass1:
      ComputeCoefs_BandPass1();
      break;
    case BandPass2:
      ComputeCoefs_BandPass2();
      break;
    case Notch:
      ComputeCoefs_Notch();
      break;
    case Peak:
      ComputeCoefs_Peak();
      break;
    case APF:
      ComputeCoefs_APF();
      break;
    case LowShelf:
      ComputeCoefs_LowShelf();
      break;
    case HighShelf:
      ComputeCoefs_HighShelf();
      break;
  }
}

void BiQuad::SetType(BiQuad::Type t)
{
  if (t != mType)
  {
    mType = t;
    ComputeCoefficients();
  }
}

BiQuad::Type BiQuad::GetType() const
{
  return mType;
}

void BiQuad::ResetCoefs()
{
  mA0 = mA1 = mA2 = mB0 = mB1 = mB2 = 0;
}

void BiQuad::ClearHist()
{
  for (uint32 i = 0; i < mChannels; i++)
  {
    mN0[i] = 0;    
    mN1[i] = 0;    
    mN2[i] = 0;    
    mN3[i] = 0;    
  }
}

void BiQuad::Reset()
{
  ClearHist();
  ResetCoefs();
}

void BiQuad::ComputeCoefs_LowPass()
{
  const float alpha = mSin / (( 12.f * mQ) + 1.f); //FIXME 12 or 24 ? long live magic numbers ..

  mA0 = 1.0f / (1.f + alpha);
  mA1 = (-2.f * mCos) * mA0;
  mA2 = (1.f - alpha) * mA0;
  
  mB1 = (1.f - mCos) * mA0;
  mB0 = (mB1 * .5f);
  mB2 = mB0;
}

void BiQuad::ComputeCoefs_HighPass()
{
  float alpha = mSin / (( 24.f * mQ) + 1.f);
  
  mA0 = 1.f / (1.f + alpha);
  mA1 = (-2.f * mCos) * mA0;
  mA2 = (1.f - alpha) * mA0;
  
  mB0 = ((1.f + mCos) * .5f) * mA0;
  mB1 = - (1.f + mCos) * mA0;
  mB2 = mB0;
}

void BiQuad::ComputeCoefs_BandPass1()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / (1.f + alpha);
  mA1 = (-2 * mCos) * mA0;
  mA2 = (1 - alpha) * mA0;
  
  mB0 = alpha * mA0;
  mB1 = 0.f;
  mB2 = -alpha * mA0;
}

void BiQuad::ComputeCoefs_BandPass2()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / (1.f + alpha);
  mA1 = (-2 * mCos) * mA0;
  mA2 = (1 - alpha) * mA0;
  
  mB0 = mQ * alpha * mA0;
  mB1 = 0.f;
  mB2 = mQ * -alpha * mA0;
}

void BiQuad::ComputeCoefs_Notch()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / (1.f + alpha);
  mA1 = - 2.f * mCos * mA0;
  mA2 = (1.f - alpha) * mA0;
  
  mB0 = mA0;
  mB1 = mA1;
  mB2 = mB0;
}

void BiQuad::ComputeCoefs_Peak()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / (1.f + alpha / mA);
  mA1 = -2.f * mCos * mA0;
  mA2 = (1.f - alpha / mA) * mA0;
  
  mB0 = (1 + alpha * mA) * mA0;
  mB1 = mA1;
  mB2 = (1 - alpha * mA) * mA0;
}

void BiQuad::ComputeCoefs_APF()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / (1.f + alpha);
  mA1 = - 2.f * mCos * mA0;
  mA2 = (1.f - alpha) * mA0;
  
  mB0 = mA2;
  mB1 = mA1;
  mB2 = (1.f + alpha) * mA0;
}

void BiQuad::ComputeCoefs_LowShelf()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / ((mA+1) + (mA-1) * mCos + 2 * sqrtf(mA) * alpha);
  mA1 = -2 * ((mA-1) + (mA+1) * mCos) * mA0;
  mA2 = ((mA+1) + (mA-1) * mCos - 2 * sqrtf(mA) * alpha) * mA0;
  
  mB0 = mA * ((mA+1) - (mA-1) * mCos + 2 * sqrtf(mA) * alpha) * mA0;
  mB1 = 2 * mA * ((mA-1) - (mA+1) * mCos) * mA0;
  mB2 = mA * ((mA+1) - (mA-1) * mCos - 2 * sqrtf(mA) * alpha) * mA0;
}

void BiQuad::ComputeCoefs_HighShelf()
{
  float alpha = mSin / (( 8.f * mQ) + 1.f);
  
  mA0 = 1.f / ((mA+1) - (mA-1) * mCos + 2 * sqrtf(mA) * alpha);
  mA1 = 2 * ((mA-1) - (mA+1) * mCos) * mA0;
  mA2 = ((mA+1) - (mA-1) * mCos - 2 * sqrtf(mA) * alpha) * mA0;
  
  mB0 = mA * ((mA+1) + (mA-1) * mCos + 2 * sqrtf(mA) * alpha) * mA0;
  mB1 = -2 * mA * ((mA-1) + (mA+1) * mCos) * mA0;
  mB2 = mA * ((mA+1) + (mA-1) * mCos - 2 * sqrtf(mA) * alpha) * mA0;
}


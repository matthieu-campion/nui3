/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nui.h"

class BiQuad
{
public:
  
  enum Type
  {
    LowPass,
    HighPass,
    BandPass1,
    BandPass2,
    Notch,
    Peak,
    APF,
    LowShelf,
    HighShelf    
  };
  
  BiQuad(float SampleRate, uint32 NbChannels);
  ~BiQuad();
  
  void SetType(BiQuad::Type t);
  BiQuad::Type GetType() const;
  void SetQ(float q);
  void SetFreq(float freq); ///< parameter in hertz, between 20 and 20000
  void SetGain(float gain);
  void SetSampleRate(double SampleRate);
  float GetQ();
  float GetFreq();
  float GetGain();
  void GetACoefs(float &rA0, float &rA1, float &rA2) const; ///< used only for visualization
  void GetBCoefs(float &rB0, float &rB1, float &rB2) const; ///< used only for visualization
  void Process(float* pOutSamples, const float* pInSamples, uint32 SampleFrames);
  void ComputeCoefficients();
  void ResetCoefs(); ///< clears coefficients
  void ClearHist(); ///< clears sample history
  void Reset(); ///< clears both coefs and history
  
private:
  Type mType;
  bool mChanged;
  uint32 mChannels;
  float mSampleRate;
  float mOmega;
  float mSin;
  float mCos;
  float mFreq; ///< cutoff frequency
  float mQ; ///< resonance
  float mA; ///< used for internal math only - don't modify this yourself
  float mGain; ///< gain, for peaking/shelving EQ only
  float mA0, mA1, mA2, mB0, mB1, mB2; ///< filter coefficients
  std::vector<float> mN0; ///< input sample - 1
  std::vector<float> mN1; ///< input sample - 2
  std::vector<float> mN2; ///< output sample - 1
  std::vector<float> mN3; ///< output sample - 2

  void ComputeCoefs_LowPass();
  void ComputeCoefs_HighPass();
  void ComputeCoefs_BandPass1();
  void ComputeCoefs_BandPass2();
  void ComputeCoefs_Notch();
  void ComputeCoefs_Peak();
  void ComputeCoefs_APF();
  void ComputeCoefs_LowShelf();
  void ComputeCoefs_HighShelf();

  void Process(float* pOutSamples, const float* pInSamples); // Process one sample frame
};


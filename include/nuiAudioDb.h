/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nglMath.h"

const double _log10 = log(10.0);
const double log_5 = log(0.5);
const float logf10 = logf(10.0f);

const double db_mul_const = 20.0 / _log10;
const float db_mul_constf = 20.0f / logf10;

inline double GainToDB(double Gain)
{
  if (Gain == 0.0)
    return (-144.0);
  else
  {
    double  dB;
    
    dB = log(Gain) * db_mul_const;
    
    if (dB < -144.0)
      dB = -144.0;
    return (dB);    // convert factor to dB
  }
}

inline double DBToGain(double dB)
{
  return pow(10.0, dB / 20.0);
}

inline float GainToDB(float Gain)
{
  if (Gain == 0.0f)
    return (-144.0f);
  else
  {
    float  dB;
    
    dB = logf(Gain) * db_mul_constf;
    
    if (dB < -144.0f)
      dB = -144.0f;
    return (dB);    // convert factor to dB
  }
}

inline float DBToGain(float dB)
{
  return powf(10.0f, dB / 20.0f);
}

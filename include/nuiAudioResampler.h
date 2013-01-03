/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"


enum nuiInterpolationMethod
{
  eInterpolNone = 0,    ///< No Interpolation, copy from a buffer to another
  eInterpolLinear = 1,  ///< The easiest method : Linear interpolation
  eInterpolCubic = 3
};

template <typename T>
class nuiAudioResampler
  {
  public:  
    enum InitArgument
    {
      eFactor = 0,
      eIncrement
    };
    
    nuiAudioResampler(nuiInterpolationMethod interpol = eInterpolLinear);
    virtual ~nuiAudioResampler();
    
    nuiInterpolationMethod GetInterpolationMethod() const;
    void SetInterpolationMethod(nuiInterpolationMethod interpol);
    int32 GetLatency();
    int32 Process(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position = 0.0, bool ReplaceElements = true, double mult = 1.0);
    void Reset();
    
  protected:
    T* mBuffer;
    nuiInterpolationMethod mInterpolationMethod;
    
    int32 ProcessNoInterpolation(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position = 0.0, bool ReplaceElements = true, double mult = 1.0);
    int32 ProcessLinear(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position = 0.0, bool ReplaceElements = true, double mult = 1.0);
    int32 ProcessCubic(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position = 0.0, bool ReplaceElements = true, double mult = 1.0);
  };

template <typename T>
nuiAudioResampler<T>::nuiAudioResampler(nuiInterpolationMethod interpol)
:mBuffer(NULL), mInterpolationMethod(interpol)
{
}

template <typename T>
nuiAudioResampler<T>::~nuiAudioResampler()
{
  delete mBuffer;
}

template <typename T>
int32 nuiAudioResampler<T>::Process(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position, bool ReplaceElements, double mult)
{
  if (sampleFramesToRead == sampleFramesToWrite)
  {
    return ProcessNoInterpolation(destBuffer, srcBuffer, sampleFramesToRead, sampleFramesToWrite, position, ReplaceElements, mult);
  }
  else
  {
    switch (mInterpolationMethod)
    {
      case eInterpolNone:
        return ProcessNoInterpolation(destBuffer, srcBuffer, sampleFramesToRead, sampleFramesToWrite, position, ReplaceElements, mult);
        
      case eInterpolLinear:
        return ProcessLinear(destBuffer, srcBuffer, sampleFramesToRead, sampleFramesToWrite, position, ReplaceElements, mult);
        
      case eInterpolCubic:
        return ProcessCubic(destBuffer, srcBuffer, sampleFramesToRead, sampleFramesToWrite, position, ReplaceElements, mult);
        
      default:
        return 0;
    }
  }
}

template <typename T>
int32 nuiAudioResampler<T>::ProcessNoInterpolation(T* destBuffer,const  T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position, bool ReplaceElements, double mult)
{
  int32 IntPosition = (int32)position;
  int32 i;
  srcBuffer = &(srcBuffer[IntPosition]);
  if (ReplaceElements)
  {
    for (i = 0; i < sampleFramesToRead; i++)
    {
      *(destBuffer++) = (T)(mult * (*(srcBuffer++)));
    }
  }
  else
  {
    for (i = 0; i < sampleFramesToRead; i++)
    {
      *(destBuffer++) = (T)(mult * (*(srcBuffer++)));
    }
  }
  return i;
}

template <typename T>
int32 nuiAudioResampler<T>::ProcessLinear(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position, bool ReplaceElements, double mult)
{
  if (!mBuffer)
  {
    mBuffer = new T;
    *mBuffer = 0.f;
  }
  
  int32 i = 0;
  double index = position;
  
  double increment = (double)sampleFramesToRead/(double)sampleFramesToWrite;
  if (ReplaceElements)
  {
    for (;index < 1.0;i++)
    {
      int32 idx = (int32)index;
      double x = index - idx;
      destBuffer[i] = (T)(mult * (mBuffer[0] + x * (srcBuffer[idx] - mBuffer[0])));
      
      index += increment;
    }
    
    for (;i<sampleFramesToWrite;i++)
    {
      int32 idx = (int32)index;
      double x = index - idx;
      destBuffer[i] = (T)(mult * (srcBuffer[idx - 1] + x * (srcBuffer[idx] - srcBuffer[idx - 1])));
      
      index += increment;
    }
  }
  else
  {
    for (;index < 1.0;i++)
    {
      int32 idx = (int32)index;
      double x = index - idx;
      destBuffer[i] += (T)(mult * (mBuffer[0] + x * (srcBuffer[idx] - mBuffer[0])));
      
      index += increment;
    }
    
    for (;i<sampleFramesToWrite;i++)
    {
      int32 idx = (int32)index;
      double x = index - idx;
      destBuffer[i] += (T)(mult * (srcBuffer[idx - 1] + x * (srcBuffer[idx] - srcBuffer[idx - 1])));
      
      index += increment;
    }
  }
  index -= increment;
  mBuffer[0] = srcBuffer[(int32)index];
  
  return i;
}

template <typename T>
int32 nuiAudioResampler<T>::ProcessCubic(T* destBuffer, const T* srcBuffer, int32 sampleFramesToRead, int32 sampleFramesToWrite, double position, bool ReplaceElements, double mult)
{
  int32 i;
  double index = position;
  double increment = (double)sampleFramesToRead/(double)sampleFramesToWrite;
  if (!mBuffer)
  {
    mBuffer = new T[3];
    for (i=0;index<2.0;i++)
    {
      if (ReplaceElements)
        destBuffer[i] = (T)0.0;
      index += increment;
    }
    
    T a = - srcBuffer[1] + srcBuffer[2];
    T b = -a;
    T c = -srcBuffer[0] + srcBuffer[1];
    T d = srcBuffer[0];
    for (;index<3.0;i++)
    {
      
      double x = index - (int)index;
      double x2 = x*x;
      
      if (ReplaceElements)
        destBuffer[i] = (T)(mult * ((a*x*x2) + (b*x2) + (c*x) + d));
      else
        destBuffer[i] += (T)(mult * ((a*x*x2) + (b*x2) + (c*x) + d));
      
      index += increment;
    }
  }
  else
  {
    T buf[] = {mBuffer[0], mBuffer[1], mBuffer[2], srcBuffer[0], srcBuffer[1], srcBuffer[2]};
    for (i=0;index<3.0;i++)
    {
      int32 idx = (int32)index;
      T a = -buf[idx] + buf[idx+1] - buf[idx+2] + buf[idx+3];
      T b = buf[idx] - buf[idx+1] - a;
      T c = -buf[idx] + buf[idx+2];
      T d = buf[idx+1];
      
      double x = index - idx;
      double x2 = x*x;
      
      if (ReplaceElements)
        destBuffer[i] = (T)(mult * ((a*x*x2) + (b*x2) + (c*x) + d));
      else
        destBuffer[i] += (T)(mult * ((a*x*x2) + (b*x2) + (c*x) + d));
      
      index += increment;
    }
  }
  
  
  
  for (;i<sampleFramesToWrite;i++)
  {
    int32 idx = (int32)index;
    T a = -srcBuffer[idx-3] + srcBuffer[idx-2] - srcBuffer[idx-1] + srcBuffer[idx];
    T b = srcBuffer[idx-3] - srcBuffer[idx-2] - a;
    T c = -srcBuffer[idx-3] + srcBuffer[idx-1];
    T d = srcBuffer[idx-2];
    
    double x = index - idx;
    double x2 = x*x;
    
    if (ReplaceElements)
      destBuffer[i] = (T)(mult * ((a*x*x2) + (b*x2) + (c*x) + d));
    else
      destBuffer[i] += (T)(mult * ((a*x*x2) + (b*x2) + (c*x) + d));
    
    index += increment;
  }
  
  mBuffer[0] = srcBuffer[(int32)index-2];
  mBuffer[1] = srcBuffer[(int32)index-1];
  mBuffer[2] = srcBuffer[(int32)index];
  
  return i;
}

template <typename T>
int32 nuiAudioResampler<T>::GetLatency()
{
  return mInterpolationMethod;
}

template <typename T>
void nuiAudioResampler<T>::Reset()
{
  delete mBuffer;
  mBuffer = NULL;
}

template <typename T>
nuiInterpolationMethod nuiAudioResampler<T>::GetInterpolationMethod() const
{
  return mInterpolationMethod;
}

template <typename T>
void nuiAudioResampler<T>::SetInterpolationMethod(nuiInterpolationMethod interpol)
{
  mInterpolationMethod = interpol;
}

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAudioConvert.h"


void nuiAudioConvert_INint16ToDEfloat(const int16* input, float* output, uint32 curChannel, uint32 nbChannels, uint32 nbSampleFrames)
{
  static const float mult1 = 1.0 / 32768.0f;
  static const float mult2 = 1.0 / 32767.0f;
  uint32 j;

  input += curChannel;
  for (j = 0; j < nbSampleFrames; j++)
  {
    int16 in = *input;

    if (in < 0)
      *output++ = (float)in * mult1;
    else
      *output++ = (float)in * mult2;

    input += nbChannels;
  }
}



void nuiAudioConvert_DEfloatToINint16(const float* input, int16* output, uint32 curChannel, uint32 nbChannels, uint32 nbSampleFrames) // de-interlaced float to interlaced int16
{
  static const float mult1 = 32768.0f ;
  static const float mult2 = 32767.0f ;
  uint32 j;

  output += curChannel;
  for (j=0; j < nbSampleFrames; j++)
  {
    float in = *input++;
    
    if (in < 0)
      *output = ToBelow(in * mult1);
    else
      *output = ToBelow(in * mult2);

    output += nbChannels;
  } 
}


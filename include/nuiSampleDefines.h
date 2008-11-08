/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

enum nuiAudioFileFormat
{
  eAudioWave,
  eAudioAiff,
  eAudioRex,
  eAudioUnknown,
};

///Enum used to choose compression format
enum WaveFormatTag
{
  eWaveFormatUnknown = 0,
  eWaveFormatPcm = 1,
  eWaveFormatIEEEfloat = 3
};

/// Enum used to choose format of samples
enum nuiSampleBitFormat
{
  eSampleInt16, ///< samples are represented by 16 bits int numbers
  eSampleFloat32, ///< samples are represented by float numbers (32 bits)
};

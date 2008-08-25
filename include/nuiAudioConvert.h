/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

void nuiAudioConvert_INint16ToDEfloat(const int16* input, float* output, uint32 curChannel, uint32 nbChannels, uint32 nbSampleFrames); // interlaced int16 to de-interlaced float

void nuiAudioConvert_DEfloatToINint16(const float* input, int16* output, uint32 curChannel, uint32 nbChannels, uint32 nbSampleFrames); // de-interlaced float to interlaced int16

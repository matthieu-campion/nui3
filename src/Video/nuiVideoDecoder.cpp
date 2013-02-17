/*
 *  nuiVideoDecoder.cpp
 *  nui3
 *
 *  Created by  mat on 12/28/10.
 *  Copyright 2010 libNUI. All rights reserved.
 *
 */
#include "nui.h"

double nuiVideoDecoder::GetDuration() const
{
  return mDuration;
}

uint32 nuiVideoDecoder::GetWidth() const
{
  return mWidth;
}

uint32 nuiVideoDecoder::GetHeight() const
{
  return mHeight;
}

float nuiVideoDecoder::GetRate() const
{
  return mRate;
}
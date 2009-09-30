/*
 *  TimeBar.h
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nui.h"

class TimeBar : public nuiSimpleContainer
{
public:
  TimeBar(nuiAttrib<uint64>& rPositionAttrib, nuiAttrib<uint64>& rLengthAttrib, double SampleRate);
  virtual ~TimeBar();
  
private:
  nuiAttrib<uint64> mPositionAttrib;
  nuiAttrib<uint64> mLengthAttrib;
  double mSampleRate;
};
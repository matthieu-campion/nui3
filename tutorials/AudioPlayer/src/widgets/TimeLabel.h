/*
 *  TimeLabel.h
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nui.h"

class TimeLabel : public nuiLabel
{
public:
  TimeLabel(nuiAttrib<uint64>& rPositionAttrib, nuiAttrib<uint64>& rLengthAttrib, double SampleRate);
  virtual ~TimeLabel();
  
private:
  bool OnTimerTick(const nuiEvent& rEvent);
  nglString GetPosition();
  nglString GetLength();
  
  nuiAttrib<uint64> mPosAttrib;
  nuiAttrib<uint64> mLengthAttrib;
  double mSampleRate;
  
  nuiTimer mTimer;
  nuiEventSink<TimeLabel> mEventSink;
};
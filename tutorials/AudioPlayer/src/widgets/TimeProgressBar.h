/*
 *  TimeProgressBar.h
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nui.h"

class TimeProgressBar : public nuiWidget 
{
public:
  TimeProgressBar(nuiAttrib<uint64>& rPositionAttrib, nuiAttrib<uint64>& rLengthAttrib, double SampleRate);
  virtual ~TimeProgressBar();
  
  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  
  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved    (nuiSize X, nuiSize Y);
  
private:
  bool OnTimerTick(const nuiEvent& rEvet);
  
  nuiAttrib<uint64> mPosAttrib;
  nuiAttrib<uint64> mLengthAttrib;
  double mSampleRate;
  
  bool mClicked;
  nuiTimer mTimer;
  nuiEventSink<TimeProgressBar> mEventSink;
};

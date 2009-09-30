/*
 *  TimeLabel.cpp
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "TimeLabel.h"

nglString TimeToString(double seconds)
{
  uint32 s = ToBelow(seconds);
  uint32 m = s / 60;
  s -= m * 60;
  uint32 h = m / 60;
  m -= h * 60;
  
  nglString res;
  res.Format("%d:%02d:%02d", h, m, s);
  return res;
}

TimeLabel::TimeLabel(nuiAttrib<uint64>& rPositionAttrib, nuiAttrib<uint64>& rLengthAttrib, double SampleRate)
: mPosAttrib(rPositionAttrib),
  mLengthAttrib(rLengthAttrib),
  mSampleRate(SampleRate),
  mTimer(1.f / 20.f),
  mEventSink(this)
{
  OnTimerTick(NULL);
  mEventSink.Connect(mTimer.Tick, &TimeLabel::OnTimerTick);
  mTimer.Start(false, true);
}

TimeLabel::~TimeLabel()
{
  
}

bool TimeLabel::OnTimerTick(const nuiEvent& rEvent)
{
  nglString text(GetPosition());
  text += (_T(" / "));
  text += GetLength();
  
  SetText(text);
  return true;
}

nglString TimeLabel::GetPosition()
{
  uint64 position = mPosAttrib.Get();
  double seconds = (double)position / mSampleRate;
  nglString res = TimeToString(seconds);
  return res;
}

nglString TimeLabel::GetLength()
{
  uint64 length = mLengthAttrib.Get();
  double seconds = (double)length / mSampleRate;
  nglString res = TimeToString(seconds);
  return res;
}
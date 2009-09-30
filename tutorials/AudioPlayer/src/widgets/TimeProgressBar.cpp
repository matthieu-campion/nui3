/*
 *  TimeProgressBar.cpp
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "TimeProgressBar.h"

TimeProgressBar::TimeProgressBar(nuiAttrib<uint64>& rPositionAttrib, nuiAttrib<uint64>& rLengthAttrib, double SampleRate)
: mPosAttrib(rPositionAttrib),
  mLengthAttrib(rLengthAttrib),
  mSampleRate(SampleRate),
  mClicked(false),
  mTimer(1.f / 25.f),
  mEventSink(this)
{
  SetObjectClass(_T("TimeProgressBar"));
  mEventSink.Connect(mTimer.Tick, &TimeProgressBar::OnTimerTick);
  mTimer.Start();
}

TimeProgressBar::~TimeProgressBar()
{
  
}

bool TimeProgressBar::Draw(nuiDrawContext* pContext)
{
  uint64 position = mPosAttrib.Get();
  uint64 length = mLengthAttrib.Get();
  float progress = (float)position / (float)length;
  if (length == 0)
    progress = 0;
  
  pContext->SetStrokeColor(nuiColor(100, 100, 100));
  
  nuiRect rect(GetRect().Size());
  pContext->DrawRect(rect, eStrokeShape);
  
  nuiRect progressRect(rect);
  progressRect.SetWidth(rect.GetWidth() * progress);
  
  pContext->SetStrokeColor(nuiColor(20, 20, 100));
  pContext->SetFillColor(nuiColor(20, 20, 150));
  pContext->DrawRect(progressRect, eStrokeAndFillShape);
  
  return true;
}

nuiRect TimeProgressBar::CalcIdealSize()
{
  return nuiRect(0,0, 100, 15);
}

bool TimeProgressBar::OnTimerTick(const nuiEvent& rEvet)
{
  Invalidate();
  return true;
}

bool TimeProgressBar::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClicked = true;
  Grab();
  return true;
}

bool TimeProgressBar::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mClicked)
    return true;
  
  uint64 length = mLengthAttrib.Get();
  if (length == 0)
    return true;
  
  nuiRect rect(GetRect());
  float progress = (X - rect.Left()) / (rect.GetWidth());
  progress = MIN(progress, 1.f);
  progress = MAX(progress, 0.f);
  
  uint64 newpos = ToBelow(progress * length);
  mPosAttrib.Set(newpos);
  
  mClicked = false;
  Ungrab();
  return true;
}

bool TimeProgressBar::MouseMoved(nuiSize X, nuiSize Y)
{
  if (!mClicked)
    return true;
  
  uint64 length = mLengthAttrib.Get();
  if (length == 0)
    return true;
  
  nuiRect rect(GetRect());
  float progress = (X - rect.Left()) / (rect.GetWidth());
  progress = MIN(progress, 1.f);
  progress = MAX(progress, 0.f);
  
  uint64 newpos = ToBelow(progress * length);
  mPosAttrib.Set(newpos);
  
  return true;
}
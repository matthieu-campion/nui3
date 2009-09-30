/*
 *  TimeBar.cpp
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "TimeBar.h"
#include "TimeLabel.h"
#include "TimeProgressBar.h"
#include "nuiHBox.h"

TimeBar::TimeBar(nuiAttrib<uint64>& rPositionAttrib, nuiAttrib<uint64>& rLengthAttrib, double SampleRate)
: mPositionAttrib(rPositionAttrib),
  mLengthAttrib(rLengthAttrib),
  mSampleRate(SampleRate)
{
  SetObjectClass(_T("TimeBar"));
  
  nuiHBox* pHBox = new nuiHBox();
  AddChild(pHBox);
  
  //
  {
    TimeProgressBar* pTimeProgressBar = new TimeProgressBar(mPositionAttrib, mLengthAttrib, mSampleRate);
    pHBox->AddCell(pTimeProgressBar);
  }
  
  //
  {
    TimeLabel* pTimeLabel = new TimeLabel(mPositionAttrib, mLengthAttrib, mSampleRate);
    pHBox->AddCell(pTimeLabel);
  }
}

TimeBar::~TimeBar()
{
  
}
/*
 *  VolumeController.cpp
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "VolumeController.h"
#include "nuiHBox.h"

VolumeController::VolumeController(nuiAttrib<float>& rGainAttrib, nuiAttrib<bool>& rMuteAttrib)
: mGainAttrib(rGainAttrib),
  mMuteAttrib(rMuteAttrib),
  mEventSink(this)
{
  SetObjectClass(_T("VolumeController"));
  
  nuiHBox* pHBox = new nuiHBox();
  AddChild(pHBox);
  
  {
    mpBtn = new nuiToggleButton(_T("mute"));
    mpBtn->SetObjectClass(_T("MuteButton"));
    mEventSink.Connect(mpBtn->Clicked, &VolumeController::OnBtnClicked);
    pHBox->AddCell(mpBtn);
  }
  
  {
    nuiRange range(mGainAttrib.Get(), -50, 6, 0.5, 2.0, 0.0);
    mpSlider = new nuiSlider(nuiHorizontal, range);
    mpSlider->SetObjectClass(_T("GainSlider"));
    mEventSink.Connect(mpSlider->GetRange().Changed, &VolumeController::OnSliderChanged);
    pHBox->AddCell(mpSlider);
  }
  
  mSlotsSink.Connect(mGainAttrib.GetChangedSignal(), nuiMakeDelegate(this, &VolumeController::OnGainAttribChanged));
  mSlotsSink.Connect(mMuteAttrib.GetChangedSignal(), nuiMakeDelegate(this, &VolumeController::OnMuteAttribChanged));
}

VolumeController::~VolumeController()
{
  
}

void VolumeController::OnGainAttribChanged(float gainDb)
{
  mpBtn->Clicked.Enable(false);
  mpSlider->GetRange().Changed.Enable(false);
  
  mpSlider->GetRange().SetValue(gainDb);
  
  nuiRange range(mpSlider->GetRange());
  if (range.GetValue() > range.GetMinimum())
  {
    mMuteAttrib.Set(false);
  }
  
  mpSlider->GetRange().Changed.Enable(true);
  mpBtn->Clicked.Enable(true);
}

void VolumeController::OnMuteAttribChanged(bool mute)
{
  mpBtn->Clicked.Enable(false);
  mpSlider->GetRange().Changed.Enable(false);
  
  mpBtn->SetPressed(mute);
  
  if (mute)
  {
    mpSlider->GetRange().SetValue(mpSlider->GetRange().GetMinimum());
  }
  else
  {
    mpSlider->GetRange().SetValue(mGainAttrib.Get());
  }
  
  mpSlider->GetRange().Changed.Enable(true);
  mpBtn->Clicked.Enable(true);
}

void VolumeController::OnBtnClicked(const nuiEvent& rEvent)
{
  bool pressed = mpBtn->IsPressed();
  mMuteAttrib.Set(pressed);
  rEvent.Cancel();
}

void VolumeController::OnSliderChanged(const nuiEvent& rEvent)
{
  nuiRange range(mpSlider->GetRange());
  float gainDb = range.GetValue();
  if (gainDb > range.GetMinimum())
  {
    mMuteAttrib.Set(false);
  }
  
  mGainAttrib.Set(gainDb);
  rEvent.Cancel();
}
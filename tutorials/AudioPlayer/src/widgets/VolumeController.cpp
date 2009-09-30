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
  mMuteAttrib(rMuteAttrib)
{
  SetObjectClass(_T("VolumeController"));
  
  nuiHBox* pHBox = new nuiHBox();
  AddChild(pHBox);
  
  {
    nuiAttributeEditor* pEditor = mMuteAttrib.GetEditor();
    pHBox->AddCell(pEditor);
  }
  
  {
    nuiAttributeEditor* pEditor = mGainAttrib.GetEditor();
    pHBox->AddCell(pEditor);
  }
  
}

VolumeController::~VolumeController()
{
  
}
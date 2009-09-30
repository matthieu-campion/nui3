/*
 *  VolumeController.h
 *  AudioPlayer
 *
 *  Created by mat on 9/28/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nui.h"

class VolumeController : public nuiSimpleContainer
{
public:
  VolumeController(nuiAttrib<float>& rGainAttrib, nuiAttrib<bool>& rMuteAttrib);
  virtual ~VolumeController();
  
private:
  nuiAttrib<float> mGainAttrib;
  nuiAttrib<bool> mMuteAttrib;
};
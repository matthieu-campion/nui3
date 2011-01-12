/*
 *  nuiFileSound.h
 *  nui3
 *
 *  Created by  mat on 1/12/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#pragma once
#include "nui.h"

class nuiFileSound : public nuiSound
{
public:
  friend class nuiSoundManager;
  
  virtual nuiVoice* GetVoice();
  
protected:
  nuiFileSound(const nglPath& rPath);
  virtual ~nuiFileSound();
  
};
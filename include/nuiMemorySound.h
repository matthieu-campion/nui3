/*
 *  nuiMemorySound.h
 *  nui3
 *
 *  Created by  mat on 1/12/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#pragma once
#include "nui.h"

class nuiMemorySound : public nuiSound
{
public:
  friend class nuiSoundManager;
  
  virtual nuiVoice* GetVoice();
  uint32 GetSampleFrames() const;
  uint32 GetChannels()const;
  
  uint32 ReadSamples(const std::vector<float*>& rBuffers, int64 position, uint32 SampleFrames);
  
protected:
  nuiMemorySound(const nglPath& rPath);
  virtual ~nuiMemorySound();
  
  bool LoadSamples();
  std::vector<float*> mSamples;
  int64 mLength;
};
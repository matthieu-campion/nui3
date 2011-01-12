/*
 *  nuiMemoryVoice.h
 *  nui3
 *
 *  Created by  mat on 1/12/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#pragma once
#include "nui.h"
#include "nuiMemorySound.h"

class nuiMemoryVoice : nuiVoice
{
public:
  friend class nuiMemorySound;
  
  nuiMemoryVoice(const nuiMemoryVoice& rVoice);
  nuiMemoryVoice& operator=(const nuiMemoryVoice& rVoice);
  
  virtual bool IsValid() const;
  virtual uint64 GetSampleFrames() const;
  virtual uint32 GetChannels() const;
  
protected:
  virtual uint32 ReadSamples(const std::vector<float*>& rOutput, int64 position, uint32 SampleFrames);
  
private:
  nuiMemoryVoice(nuiMemorySound* pSound = NULL);
  virtual ~nuiMemoryVoice();
  
  bool Init();
  
  nuiMemorySound* mpMemorySound;
};
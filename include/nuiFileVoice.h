/*
 *  nuiFileVoice.h
 *  nui3
 *
 *  Created by  mat on 1/12/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#pragma once
#include "nui.h"
#include "nuiSampleReader.h"
#include "nuiFileSound.h"


class nuiFileVoice : nuiVoice
{
public:
  friend class nuiFileSound;
  
  nuiFileVoice(const nuiFileVoice& rVoice);
  nuiFileVoice& operator=(const nuiFileVoice& rVoice);
  
  virtual bool IsValid() const;
  virtual uint64 GetSampleFrames() const;
  virtual uint32 GetChannels() const;
  
protected:
  virtual uint32 ReadSamples(const std::vector<float*>& rOutput, int64 position, uint32 SampleFrames);
  
private:
  nuiFileVoice(nuiFileSound* pSound = NULL);
  virtual ~nuiFileVoice();
  
  bool Init();
  
  nuiFileSound* mpFileSound;
  
  nglIStream* mpStream;
  nuiSampleReader* mpReader;
  nuiSampleInfo mInfo;
};
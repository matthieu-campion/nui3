/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once
#include "nui.h"
#include "nuiMemorySound.h"

class nuiMemoryVoice : public nuiVoice
{
public:
  friend class nuiMemorySound;
  
  nuiMemoryVoice(const nuiMemoryVoice& rVoice);
  nuiMemoryVoice& operator=(const nuiMemoryVoice& rVoice);
  
  virtual bool IsValid() const;
  virtual int32 GetChannels() const;
  int32 GetSampleFrames() const;
  
protected:
  virtual int32 ReadSamples(const std::vector<float*>& rOutput, int64 position, int32 SampleFrames);
  
private:
  nuiMemoryVoice(nuiMemorySound* pSound = NULL);
  virtual ~nuiMemoryVoice();
  
  bool Init();
  
  nuiMemorySound* mpMemorySound;
};
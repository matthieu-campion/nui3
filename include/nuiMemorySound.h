/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once
#include "nui.h"

class nuiMemorySound : public nuiSound
{
public:
  friend class nuiSoundManager;
  
  int32 GetSampleFrames() const;
  int32 GetChannels()const;
  
  int32 ReadSamples(const std::vector<float*>& rBuffers, int64 position, int32 SampleFrames);
  
protected:
  nuiMemorySound(const nglPath& rPath);
  nuiMemorySound(const nglString& rStringID, nglIStream* pStream);
  virtual ~nuiMemorySound();
  
  virtual nuiVoice* GetVoiceInternal();
  
  bool LoadSamples(nglIStream* pStream = NULL);
  std::vector<float*> mSamples;
  int64 mLength;
  nglPath mPath;
};

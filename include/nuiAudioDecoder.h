/*
 *  nuiAudioDecoder.h
 *  nui3
 *
 *  Created by mat on 12/18/08.
 *  Copyright 2008 Mxp4. All rights reserved.
 *
 */

#pragma once

#include "nui.h"
#include "nuiSampleInfo.h"

class nuiAudioDecoder 
{
public:
  nuiAudioDecoder(nglIStream& rStream);
  virtual ~nuiAudioDecoder();
  
  bool GetInfo(nuiSampleInfo& rInfo) const;
  uint32 Read(std::vector<float*> buffers, uint32 SampleFrames);
  
  bool SetPosition(uint64 pos);
  const uint64 GetPosition() const;
  
  bool BytesToSampleFrames(uint64 inBytes, uint64& outSampleFrames) const;
  bool SampleFramesToBytes(uint64 inSampleFrames, uint64& outBytes) const;
  
private:
  bool Init();
  bool ReadInfo();
  bool Seek(uint64 SampleFrame);
  void Clear();
  
  bool mInitialized;
  nglIStream& mrStream; 
  uint64 mPosition;
  nuiSampleInfo mInfo;
  
  class nuiAudioDecoderPrivate* mpPrivate;
};

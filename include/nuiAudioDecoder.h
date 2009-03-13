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
#include "nuiSampleReader.h"

class nuiAudioDecoder : public nuiSampleReader
{
public:
  nuiAudioDecoder(nglIStream& rStream);
  nuiAudioDecoder(const nuiAudioDecoder& rDecoder, nglIStream& rStream);
  virtual ~nuiAudioDecoder();
  
  virtual nuiSampleReader* Clone(nglIStream& rStream) const;
  
  virtual uint32 ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  virtual uint32 ReadIN(void* pBuffer, uint32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  
  virtual void SetPosition(uint32 position);
  
private:
  bool CreateAudioDecoderPrivate();
  virtual bool ReadInfo();
  bool Seek(uint64 SampleFrame);
  void Clear();

  class nuiAudioDecoderPrivate* mpPrivate;
};


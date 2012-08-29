/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
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
  
  virtual int32 ReadDE(std::vector<void*> buffers, int32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  virtual int32 ReadIN(void* pBuffer, int32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  
  virtual void SetPosition(int64 position);
  
private:
  bool CreateAudioDecoderPrivate();
  virtual bool ReadInfo();
  bool Seek(int64 SampleFrame);
  void Clear();

  class nuiAudioDecoderPrivate* mpPrivate;
};


/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSampleInfo.h"

class nuiSampleReader 
{
public:
  nuiSampleReader(nglIStream& rStream);
  nuiSampleReader(const nuiSampleReader& rReader, nglIStream& rStream);
  virtual ~nuiSampleReader();
  
  bool Init();
  
  virtual nuiSampleReader* Clone(nglIStream& rStream) const = 0;
  
  virtual bool GetInfo(nuiSampleInfo& rInfo) const;
  
  virtual int32 ReadDE(std::vector<void*> buffers, int32 sampleframes, nuiSampleBitFormat format = eSampleFloat32) = 0; ///< Returns the count sample frames actually read. Deinterlaced version.
  virtual int32 ReadIN(void* pBuffer, int32 sampleframes, nuiSampleBitFormat format = eSampleFloat32) = 0; ///< Returns the count sample frames actually read. Interlaced version.
  
  int64 GetPosition() const;
  virtual void SetPosition(int64 position);
  
  int64 BytesToSampleFrames(int64 inBytes) const;
  int64 SampleFramesToBytes(int64 inSampleFrames) const;
protected:
  virtual bool ReadInfo() = 0;
  
  bool mInitialized;
  nglIStream& mrStream; 
  int64 mPosition;
  nuiSampleInfo mInfo;
  
};

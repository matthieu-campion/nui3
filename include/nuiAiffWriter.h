/*
 *  nuiAiffWriter.h
 *  nui3
 *
 *  Created by mat on 1/21/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#pragma once

#include "nuiSampleWriter.h"


class nuiAiffWriter : public virtual nuiSampleWriter
{
public:
  nuiAiffWriter(nglOStream& rStream);
  virtual ~nuiAiffWriter();
  
  bool WriteInfo(const nuiSampleInfo& rInfos); 
  uint32 Write(const void* pBuffer, uint32 SampleFrames, nuiSampleBitFormat format);
  bool Finalize();
  
private:
  bool mInfoWritten;
};
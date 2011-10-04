/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiSampleWriter.h"


class nuiAiffWriter : public virtual nuiSampleWriter
{
public:
  nuiAiffWriter(nglOStream& rStream);
  virtual ~nuiAiffWriter();
  
  bool WriteInfo(const nuiSampleInfo& rInfos); 
  int32 Write(const void* pBuffer, int32 SampleFrames, nuiSampleBitFormat format);
  bool Finalize();
  
private:
  bool mInfoWritten;
};
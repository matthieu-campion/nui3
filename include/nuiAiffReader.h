/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiChunkSampleReader.h"

/// Structure that describes a extended float number (80 bits)
typedef struct
  {
    int16 exp[1];
    int16 man[4];
  } extended;

double ExtendedToDouble(extended x);
extended DoubleToExtended(double q);

class nuiAiffReader : public nuiChunkSampleReader
{
public:
  nuiAiffReader(nglIStream& rStream);
  nuiAiffReader(const nuiAiffReader& rReader, nglIStream& rStream);
  virtual ~nuiAiffReader();
  
  virtual nuiSampleReader* Clone(nglIStream& rStream) const;
  
  int32 ReadIN(void* pBuffer, int32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  
  virtual void SetPosition(int64 position);
private:
  virtual bool ReadInfo();
};



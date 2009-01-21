/*
 *  nuiAiffReader.h
 *  nui3
 *
 *  Created by mat on 1/19/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
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
  
  uint32 ReadIN(void* pBuffer, uint32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  
  virtual void SetPosition(uint32 position);
private:
  virtual bool ReadInfo();
};



/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglString.h"
#include "ConvertUTF.h"

typedef ConversionResult (*UTFConverter) (const char** sourceStart, const char* sourceEnd, char** targetStart, char* targetEnd, ConversionFlags flags);

ConversionResult NoConversion(const char** sourceStart, const char* sourceEnd, char** targetStart, char* targetEnd, ConversionFlags flags);

class NGL_API nglUTFStringConv : public nglStringConv
{
public:
  nglUTFStringConv(nglTextEncoding From, nglTextEncoding To, nglChar Default = nglStringConv::mUnknown);
  virtual ~nglUTFStringConv();
  
  virtual int32 Process(const char*& pSource, int32& rToRead, char*& pTarget, int32& rToWrite);
  
private:
  static UTFConverter GetUTFConverter(nglTextEncoding From, nglTextEncoding To);
  
  UTFConverter mpConverter;
};

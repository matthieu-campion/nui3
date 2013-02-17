/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

ConversionResult NoConversion(const char** sourceStart, const char* sourceEnd, char** targetStart, char* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const char* source = *sourceStart;
  char* target = *targetStart;
  while (source < sourceEnd)
  {
    char ch;
    const char* oldSource = source; /* In case we have to back up because of target overflow. */
    ch = *source++;

    *target = ch;
    target++;
    if (target > targetEnd)
    {
      source = oldSource; /* Back up source pointer! */
      target--;
      result = targetExhausted;
      break;
    }
  }
  *sourceStart = source;
  *targetStart = target;
  return result;
} 

nglUTFStringConv::nglUTFStringConv(const nglTextEncoding From, const nglTextEncoding To, nglChar Default)
: nglStringConv(From, To, Default, true)
{  
  mpConverter = nglUTFStringConv::GetUTFConverter(From, To);
  NGL_ASSERT(mpConverter);
}

nglUTFStringConv::~nglUTFStringConv()
{
}


UTFConverter nglUTFStringConv::GetUTFConverter(nglTextEncoding From, nglTextEncoding To)
{
  if (From == eEncodingNative)
    From = eUTF8;
  if (From == eEncodingInternal)
    From = eUTF8;
  if (To == eEncodingNative)
    To = eUTF8;
  if (To == eEncodingInternal)
    To = eUTF8;

  if (From == To)
    return (UTFConverter)NoConversion;

  switch (From)
  {
    case eUTF8:
      {
        switch (To)
        {
          case eUCS2:
            {
              return (UTFConverter)ConvertUTF8toUTF16;
            }
            break;
          case eUCS4:
            {
              return (UTFConverter)ConvertUTF8toUTF32;
            }
            break;
          default:
            break;
        }
      }
      break;
    case eUCS2:
      {
        switch (To)
        {
          case eUTF8:
            {
              return (UTFConverter)ConvertUTF16toUTF8;
            }
            break;
          case eUCS4:
            {
              return (UTFConverter)ConvertUTF16toUTF32;
            }
            break;
          default:
            break;
        }
      }
      break;
    case eUCS4:
      {
        switch (To)
        {
          case eUTF8:
            {
              return (UTFConverter)ConvertUTF32toUTF8;
            }
            break;
          case eUCS2:
            {
              return (UTFConverter)ConvertUTF32toUTF16;
            }
            break;
          default:
            break;
        }
      }
      break;
    default:
      break;
  }

  return NULL;
}

int32 nglUTFStringConv::Process(const char*& pSource, int32& rToRead, char*& pTarget, int32& rToWrite)
{
  const char* pSourceEnd = &pSource[rToRead];
  const char* pTargetEnd = &pTarget[rToWrite];
  ConversionResult res = mpConverter(&pSource, &pSource[rToRead], &pTarget, &pTarget[rToWrite], strictConversion);
  
  rToRead = pSourceEnd - pSource;
  rToWrite = pTargetEnd - pTarget;
  
  if (rToWrite < 0)
  {
    NGL_OUT("rToWrite < 0 !!!");
  }
  NGL_ASSERT(rToWrite >= 0);
  
  switch (res)
  {
    case conversionOK:
      mState = eStringConv_OK;
      break;
    case sourceExhausted:
      mState = eStringConv_NeedInput;
      return 1;
    case targetExhausted:
      mState = eStringConv_NeedOutput;
      return 1;
    case sourceIllegal:
      mState = eStringConv_InvalidConversion;
      return 1;
  }

  
  return 0;
}

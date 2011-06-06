/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

class nglStringCodec
{
public:
  TECObjectRef mHandle;
  nglTextEncoding mFrom;
  nglTextEncoding mTo;
  
  nglStringCodec(nglTextEncoding From, nglTextEncoding To);
  ~nglStringCodec();
  
  ::TextEncoding GetIANA(nglTextEncoding Encoding, bool NormalizedFormC) const;
  
};



nglStringConv::nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default)
{
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mState = eStringConv_OK;
  
  mpCodec  = new nglStringCodec(From, To);
}

nglStringConv::nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default, bool NoInit)
{
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mState = eStringConv_OK;

  mpCodec = NULL;
  if (!NoInit)
    mpCodec  = new nglStringCodec(From, To);
}

nglStringConv::~nglStringConv()
{
  delete mpCodec;
}

static OSStatus ConvertUnicodeToCanonical(
                                          Boolean precomposed,
                                          const UniChar *inputBuf, ByteCount inputBufLen,
                                          UniChar *outputBuf, ByteCount outputBufSize,
                                          ByteCount *outputBufLen)
/* As is standard with the Unicode Converter, all lengths are in bytes. */
{
  OSStatus            err;
  OSStatus            junk;
  TextEncodingVariant variant;
  UnicodeToTextInfo   uni;
  UnicodeMapping      map;
  ByteCount           junkRead;
  
  assert(inputBuf     != NULL);
  assert(outputBuf    != NULL);
  assert(outputBufLen != NULL);
  
  if (precomposed)
  {
    variant = kUnicodeCanonicalCompVariant;
  }
  else
  {
    variant = kUnicodeCanonicalDecompVariant;
  }
  map.unicodeEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset, kTextEncodingDefaultFormat);
  map.otherEncoding   = CreateTextEncoding(kTextEncodingUnicodeDefault, variant, kTextEncodingDefaultFormat);
  map.mappingVersion  = kUnicodeUseLatestMapping;
  
  uni = NULL;
  
  err = CreateUnicodeToTextInfo(&map, &uni);
  if (err == noErr)
  {
    err = ConvertFromUnicodeToText(uni, inputBufLen, inputBuf,
                                   kUnicodeDefaultDirectionMask,
                                   0, NULL, NULL, NULL,
                                   outputBufSize, &junkRead,
                                   outputBufLen, outputBuf);
  }
  
  if (uni != NULL)
  {
    junk = DisposeUnicodeToTextInfo(&uni);
    assert(junk == noErr);
  }
  
  return err;
}

int32 nglStringConv::Process(const char*& pSource, int32& rToRead, char*& pTarget, int32& rToWrite)
{  
  if (mpCodec->mFrom != mpCodec->mTo) 
  {
    
    if (GetState() == eStringConv_InvalidConversion) // Invalid conversion
      return -1;
    
    if (!pSource || rToRead < 0 ||
        !pTarget || rToWrite < 0)
    {
      return -1;
    }
    
    ConstTextPtr inputBuffer = (ConstTextPtr) pSource;
    TextPtr outputBuffer = (TextPtr) pTarget;
    ByteCount inputBufferLength = rToRead;
    ByteCount outputBufferLength = rToWrite;
    ByteCount actualOutputLength = 0;
    ByteCount actualInputLength = 0;
    
    if (mFrom == eUCS4)
    {
      int16* pInputBuffer = new int16[rToRead];
      inputBuffer = (ConstTextPtr)new int16[rToRead];
      for (int32 i = 0; i < rToRead / 4; i++)
        ((int16*)pInputBuffer)[i] = (int16)((int32*)pSource)[i];
      inputBufferLength /= 2;
      ByteCount res = 0;
      ConvertUnicodeToCanonical(false, (UniChar*)pInputBuffer, inputBufferLength, (UniChar*)inputBuffer, rToRead, &res);
      rToRead = res;
      inputBufferLength = res;
      
      delete pInputBuffer;
    }
    
    OSStatus result = TECConvertText(mpCodec->mHandle, inputBuffer, inputBufferLength, &actualInputLength, outputBuffer, outputBufferLength, &actualOutputLength);
    
    pSource += actualInputLength;
    rToRead -= actualInputLength;
    pTarget += actualOutputLength;
    rToWrite -= actualOutputLength;
    
    if (mTo == eUCS4)
    {
      int32 SkipBOM = 0;
      uint16 BOM = ((uint16*)outputBuffer)[0];
      if (BOM == 0xFFFE || BOM == 0xFEFF)
        SkipBOM = 1;
      
      ConvertUnicodeToCanonical(true, (UniChar*)outputBuffer, outputBufferLength, (UniChar*)outputBuffer, outputBufferLength, &outputBufferLength);
      
      for (int32 i = (outputBufferLength/sizeof(nglChar)); i >= SkipBOM; i--)
      {
        ((uint32*)outputBuffer)[i - SkipBOM] = (uint32)((uint16*)outputBuffer)[i];
        //printf("-> %c - %c\n", (char)((int32*)outputBuffer)[i], (char)((int16*)outputBuffer)[i]);
      }
      pTarget += actualOutputLength - SkipBOM;
      rToWrite -= actualOutputLength - SkipBOM;
    }
    
    if (mFrom == eUCS4)
    {
      pSource += actualInputLength;
      rToRead -= actualInputLength;
      delete inputBuffer;
    }
    
		if (result == noErr)
			return 0;
    
    return -1;
  }
  else 
  {
  	while(rToRead > 0 && rToWrite > 0) 
  	{
  		*pTarget++ = *pSource++;
  		rToRead--;
  		rToWrite--;
      
  	}
  	return 0;
  }
}

/*
 * Internals (nglStringCodec)
 */

nglStringCodec::nglStringCodec(nglTextEncoding From, nglTextEncoding To)
{
  mFrom = From;
  mTo = To;
  
  ::TextEncoding from = GetIANA(mFrom, false);
  ::TextEncoding to = GetIANA(mTo, true);
  
  OSStatus Result = 0;
  if (to != from)
  {
  	Result = TECCreateConverter(&mHandle, from, to);
  }
  else
  {
  	mHandle = NULL;
  }
}

nglStringCodec::~nglStringCodec()
{
  if (mHandle)
    TECDisposeConverter(mHandle);
}


::TextEncoding nglStringCodec::GetIANA(nglTextEncoding Encoding, bool NormalizedFormC) const
{
  TextEncodingVariant norm = NormalizedFormC ? kUnicodeNormalizationFormC : kUnicodeNoSubset;
  switch (Encoding)
  {
    // 8 bit encodings    
    case eISO8859_1 : return kTextEncodingISOLatin1;
    case eISO8859_2 : return kTextEncodingISOLatin2;
    case eISO8859_3 : return kTextEncodingISOLatin3;
    case eISO8859_4 : return kTextEncodingISOLatin4;
    case eISO8859_5 : return kTextEncodingISOLatinCyrillic;
    case eISO8859_6 : return kTextEncodingISOLatinArabic;
    case eISO8859_7 : return kTextEncodingISOLatinGreek;
    case eISO8859_8 : return kTextEncodingISOLatinHebrew;
    case eISO8859_9 : return kTextEncodingISOLatin5;
    case eISO8859_10: return kTextEncodingISOLatin6;
      // doesn t seems to exist !!!
      //case eISO8859_11: return ;
      //case eISO8859_12: return ;
    case eISO8859_13: return kTextEncodingISOLatin7;
    case eISO8859_14: return kTextEncodingISOLatin8;
    case eISO8859_15: return kTextEncodingISOLatin9;
      
      // Mac
    case eAppleRoman: return CreateTextEncoding(kTextEncodingMacRoman, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
      
      // Windows code pages
    case eCP1250: return kTextEncodingWindowsLatin2;
    case eCP1251: return kTextEncodingWindowsCyrillic;
    case eCP1252: return kTextEncodingWindowsLatin1;
    case eCP1253: return kTextEncodingWindowsGreek;
    case eCP1254: return kTextEncodingWindowsLatin5;
    case eCP1255: return kTextEncodingWindowsHebrew;
    case eCP1256: return kTextEncodingWindowsArabic;
    case eCP1257: return kTextEncodingWindowsBalticRim;
    case eCP1258: return kTextEncodingWindowsVietnamese;
    case eCP874 : return kTextEncodingDOSThai;
      
      // Multi-byte encodings
    case eKOI8R : return kTextEncodingKOI8_R;
    case eSJIS  : return kTextEncodingShiftJIS;
    case eEUCJP : return kTextEncodingEUC_JP;
    case eGB2312: return kTextEncodingHZ_GB_2312;
    case eBig5  : return kTextEncodingBig5;
      
      // Windows code pages
    case eCP932 : return kTextEncodingDOSJapanese;
    case eCP936 : return kTextEncodingDOSChineseSimplif;
    case eCP949 : return kTextEncodingDOSKorean;
    case eCP950 : return kTextEncodingDOSChineseTrad;
      
      // Unicode variants - the MacOS API is not very clear on those encodings
    case eUTF7 : return CreateTextEncoding(kTextEncodingUnicodeDefault, norm, kUnicodeUTF7Format); 
		case eUTF8 : return CreateTextEncoding(kTextEncodingUnicodeDefault, norm, kUnicodeUTF8Format);
    case eUCS2 : return CreateTextEncoding(kTextEncodingUnicodeDefault, norm, kUnicodeUTF16Format);
    case eUCS4 : return CreateTextEncoding(kTextEncodingUnicodeDefault, norm, kUnicodeUTF16Format); // not yet implemented on MacOSX so we cheat with 16 bit and we'll transcode to and from 32 bits by hand
                                                                                                                           // Others
      
      
    case eEncodingInternal: return CreateTextEncoding(kTextEncodingUnicodeDefault, norm, kUnicodeUTF8Format);
      
    case eEncodingNative  : return CreateTextEncoding(kTextEncodingUnicodeDefault, norm, kUnicodeUTF8Format);
      
    case eEncodingUnknown:
    default:
      return 0;
  }
}


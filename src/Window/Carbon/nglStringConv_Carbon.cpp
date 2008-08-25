/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include NGL_CONFIG_H

#include "nglKernel.h"
#include "nglStringConv.h"

class StringCodec
{
public:
  TECObjectRef mHandle;
  TextEncoding mFrom;
  TextEncoding mTo;

  StringCodec(nglTextEncoding From, nglTextEncoding To);
  ~StringCodec();

  TextEncoding GetIANA (nglTextEncoding Encoding) const;
};



nglStringConv::nglStringConv (const nglTextEncoding From, const nglTextEncoding To, nglChar Default)
{
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mpCodec  = new StringCodec(From, To);
}

nglStringConv::~nglStringConv()
{
  delete mpCodec;
}


int nglStringConv::Process (const char*& pSource, int& rToRead, char*& pTarget, int& rToWrite)
{
  if (mpCodec->mFrom != mpCodec->mTo) 
  {
	

    if (GetError() == NGL_STRINGCONV_ECONV) // Invalid conversion
      return -1;

    SetError(NGL_STRINGCONV_ENONE);

    if (!pSource || rToRead < 0 ||
        !pTarget || rToWrite < 0)
    {
      SetError("string", NGL_STRINGCONV_ERANGE);
      return -1;
    }

    ConstTextPtr inputBuffer = (ConstTextPtr) pSource;
    TextPtr outputBuffer = (TextPtr) pTarget;
    ByteCount inputBufferLength = rToRead;
    ByteCount outputBufferLength = rToWrite;
    ByteCount actualOutputLength = 0;
    ByteCount actualInputLength = 0;


    OSStatus result = TECConvertText(mpCodec->mHandle, inputBuffer, inputBufferLength, &actualInputLength, outputBuffer, outputBufferLength, &actualOutputLength);
  
    pSource += actualInputLength;
    rToRead -= actualInputLength;
    pTarget += actualOutputLength;
    rToWrite -= actualOutputLength;
  
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
 * Internals (StringCodec)
 */

StringCodec::StringCodec(nglTextEncoding From, nglTextEncoding To)
{
  mFrom = GetIANA(From);
  mTo = GetIANA(To);
  if (mTo != mFrom)
  {
  	OSStatus Result = TECCreateConverter(&mHandle, mFrom, mTo);
  }
  else
  {
  	mHandle = NULL;
  }
}

StringCodec::~StringCodec()
{
  if (mHandle)
    TECDisposeConverter(mHandle);
}


TextEncoding StringCodec::GetIANA (nglTextEncoding Encoding) const
{
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
    case eUTF7 : return CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant, kUnicodeUTF7Format); 
		case eUTF8 : return CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant, kUnicodeUTF8Format);
    case eUCS2 : return kTextEncodingUnicodeV2_0;
    case eUCS4 : return CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant, kUnicode32BitFormat); // not yet implemented on MacOSX
    // Others
     

#ifdef USE_WCHAR
    case eEncodingInternal: return CreateTextEncoding(kTextEncodingMacRoman, kTextEncodingDefaultVariant, kUnicode16BitFormat);
#else
    case eEncodingInternal: 
#endif

    case eEncodingNative  : return CreateTextEncoding(kTextEncodingMacRoman, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);

    case eEncodingUnknown:
    default:
      return 0;
  }
}

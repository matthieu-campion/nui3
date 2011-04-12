#include "nui.h"
#include <iconv.h>

class nglStringCodec 
{
public:
  iconv_t iconv_desc;
  nglStringCodec(nglTextEncoding From, nglTextEncoding To);
  ~nglStringCodec();
  
  const char *GetName(nglTextEncoding Encoding);
  
};

nglStringConv::nglStringConv (const nglTextEncoding From, const nglTextEncoding To, nglUChar Default)
{
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mState = eStringConv_OK;
  
  mpCodec  = new nglStringCodec(From, To);
}

nglStringConv::nglStringConv (const nglTextEncoding From, const nglTextEncoding To, nglUChar Default, bool NoInit)
{
  mState   = eStringConv_OK;
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  
  mpCodec = NULL;
  if(!NoInit)
	  mpCodec  = new nglStringCodec(From, To);
}

nglStringConv::~nglStringConv()
{
  if (mpCodec)
    delete mpCodec;
}


#if !defined(MAC_OS_X_VERSION_10_5) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5
#define BUILDING_ON_TIGER 1
#endif
#if !defined(MAC_OS_X_VERSION_10_6) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_6
#define BUILDING_ON_LEOPARD 1
#endif

int nglStringConv::Process (const char*& pSource, int& rToRead, char*& pTarget, int& rToWrite)
{
#if (!defined _CARBON_) && (!defined _COCOA_) || !(defined BUILDING_ON_TIGER)
  char* in = const_cast<char*>(pSource);
#else
  const char* in = pSource;
#endif
  char* out = pTarget;
  size_t inbytes = rToRead;
  size_t outbytes = rToWrite;
  size_t error_pos = 0;
  bool done = false;

  if (!pSource || rToRead < 0 ||
      !pTarget || rToWrite < 0)
    return -1;

  do
  {
    //LBDEBUG ICI
    if (iconv(mpCodec->iconv_desc, &in, &inbytes, &out, &outbytes) == (size_t)(-1))
    {
      switch (errno)
      {
        case EILSEQ:
        {
          done=true;
          // Invalid multibyte sequence is encountered in the input

          // Ignore error if the preceding error occured at the preceding input byte
          if (error_pos != (inbytes + 1))
          {
            /* Add the 'Default' char if
             *   - mDefault != '\0'
             *   - there's some room in the output buffer
             */
            if (mDefault && (outbytes > 0))
            {
              *out++ = (char)mDefault;
              outbytes--;
            }
          }
          error_pos = inbytes; // Remember last error offset

          // Proceed one byte in input, hoping to skip the error
          in++;
          inbytes--;
        }
        break;

        case EINVAL:
          // Incomplete multibyte sequence is encountered in the input
          done = true;
          break;

        case E2BIG:
          done = true;
          break;
      }
    }
    else
    {
      // The input byte sequence has been entirely converted
      done = true;
    }
  }
  while (!done);

  pSource = in;
  rToRead = inbytes;
  pTarget = out;
  rToWrite = outbytes;

  return 0;
}

/*
 * Internals (StringCodec)
 */

nglStringCodec::nglStringCodec(nglTextEncoding From, nglTextEncoding To)
{
  printf("create string codec (%p) from %s(%d) to %s(%d)\n", this, GetName(From), From, GetName(To), To);
  iconv_desc = iconv_open(GetName(To), GetName(From));
}

nglStringCodec::~nglStringCodec()
{
  iconv_close(iconv_desc);
}


const char *nglStringCodec::GetName(nglTextEncoding Encoding)
{
  switch (Encoding)
  {
    // 8 bit encodings
    case eISO8859_1 : return "ISO8859-1";
    case eISO8859_2 : return "ISO8859-2";
    case eISO8859_3 : return "ISO8859-3";
    case eISO8859_4 : return "ISO8859-4";
    case eISO8859_5 : return "ISO8859-5";
    case eISO8859_6 : return "ISO8859-6";
    case eISO8859_7 : return "ISO8859-7";
    case eISO8859_8 : return "ISO8859-8";
    case eISO8859_9 : return "ISO8859-9";
    case eISO8859_10: return "ISO8859-10";
    case eISO8859_11: return "ISO8859-11";
    case eISO8859_12: return "ISO8859-12";
    case eISO8859_13: return "ISO8859-13";
    case eISO8859_14: return "ISO8859-14";
    case eISO8859_15: return "ISO8859-15";
      
      // Mac
    //case eAppleRoman: return ;
      
      // Windows code pages
    case eCP1250: return "CP1250";
    case eCP1251: return "CP1251";
    case eCP1252: return "CP1252";
    case eCP1253: return "CP1253";
    case eCP1254: return "CP1254";
    case eCP1255: return "CP1255";
    case eCP1256: return "CP1256";
    case eCP1257: return "CP1257";
    case eCP1258: return "CP1258";
    case eCP874 : return "CP874";
      
      // Multi-byte encodings
    case eKOI8R : return "KOI18-R";
    case eSJIS  : return "SHIFT_JIS";
    case eEUCJP : return "EUC-JP";
    //case eGB2312: return kTextEncodingHZ_GB_2312;
    case eBig5  : return "BIG5";
      
      // Windows code pages
    case eCP932 : return "CP932";
    case eCP936 : return "CP936";
    case eCP949 : return "CP949";
    case eCP950 : return "CP950";
      
    // Unicode variants
    case eUTF7 : return "UTF-7";
    case eUTF8 : return "UTF-8";
    case eUCS2 : return "UCS-2";
    case eUCS4 : return "UCS-4";
    // Others
    //case eEncodingInternal: return "UTF-8";
    //case eEncodingNative  : return "UTF-8";
    case eEncodingInternal: return "UTF-8";
    case eEncodingNative  : return "UTF-8";
    case eEncodingUnknown:
    default:
      return 0;
  }
}


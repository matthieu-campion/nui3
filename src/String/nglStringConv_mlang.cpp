
#include "nui.h"

#include <mlang.h>

/* This is the mysterious 'Unicode codepage' the doc talks about everywhere
* without telling you which code it is (sigh).
*/
#define CP_WCHAR 1200

/*
* This is the MLang based implementation, see :
* - http://msdn.microsoft.com/workshop/misc/mlang/mlang.asp
* - http://www.batutis.com/i18n/papers/mlang/
*
* Why MLang ?
* - It requires IE4, something pretty straigthforward nowadays
* - Much better than WCToMB/MBToWC :
*   + relies on its own set of charmaps (system32/mlang.dat),
*     ie. don't need to have an ISO-8859 locale installed to support ISO-8859 conversions
*     (This limitation seems to only apply in Win95/98/Me and NT4)
*   + the only way to support ISO and other common encodings
*   + provide reusable conversion context (boost Console and Stream)
* - We currently rely on interface v1, but can benefit from v2 (IE5)
*   at runtime by adding some simple support code
*
* Notes:
* - We can't set the default char for error conversion
* - We can't locate/count conversion errors
* - MLangConvertnglCharset is very buggy, see comments
*/


class nglStringCodec
{
public:
  static IClassFactory* mpFactory;
  IMLangConvertCharset* mpInstance;
  UINT                  mFromCP;
  UINT                  mToCP;

  nglStringCodec(nglTextEncoding From, nglTextEncoding To);
  ~nglStringCodec();

  IClassFactory*        GetFactory();
  IMLangConvertCharset* GetInstance();
  void                  FreeInstance();

  static UINT           GetCP(nglTextEncoding Encoding);
  static void           Shutdown();

  typedef std::pair<nglTextEncoding, nglTextEncoding> EncodingPair;
  static std::map<EncodingPair, nglStringCodec*> mCodecs;

  static nglStringCodec* GetCodec(nglTextEncoding From, nglTextEncoding To)
  {
    EncodingPair p(From, To);
    std::map<EncodingPair, nglStringCodec*>::iterator it = mCodecs.find(p);
    if (it != mCodecs.end())
      return it->second;

    nglStringCodec* pCodec = new nglStringCodec(From, To);
    if (pCodec)
    {
      mCodecs[p] = pCodec;
    }
    return pCodec;
  }
};

IClassFactory* nglStringCodec::mpFactory = NULL;
std::map<nglStringCodec::EncodingPair, nglStringCodec*> nglStringCodec::mCodecs;

/*
* nglStringConv
*/

nglStringConv::nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default)
{
  mState   = eStringConv_OK;
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mpCodec  = nglStringCodec::GetCodec(From, To);
}

nglStringConv::nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default, bool NoInit)
{
  mState   = eStringConv_OK;
  mFrom    = From;
  mTo      = To;
  mDefault = Default;
  mpCodec = NULL;
  if (!NoInit)
    mpCodec  = nglStringCodec::GetCodec(From, To);
}


nglStringConv::~nglStringConv()
{
}

int32 nglStringConv::Process(const char*& pSource, int32& rToRead, char*& pTarget, int32& rToWrite)
{
  if (!pSource || rToRead < 0 ||
    !pTarget || rToWrite < 0)
  {
    return -1;
  }

  // Spare some work if possible (MLang refuse to do it anyway)
  if (mpCodec->mFromCP == mpCodec->mToCP)
  {
    int64 todo = MIN(rToRead, rToWrite);

    memcpy(pTarget, pSource, (size_t)todo);

    if (rToRead > rToWrite)
      mState = eStringConv_NeedOutput; //SetError("string", NGL_STRINGCONV_EMORE);

    pSource  += todo;
    rToRead  -= todo;
    pTarget  += todo;
    rToWrite -= todo;

    return 0;
  }

  // Fetch converter
  if (!mpCodec->GetInstance())
  {
    return -1;
  }

  /* Initialize some variables
  */
  int64 error_cnt = 0;
  bool done = false;

  BYTE* in      = (BYTE*) pSource;
  BYTE* out     = (BYTE*) pTarget;
  UINT to_read  = (UINT)rToRead;
  UINT inbytes  = (UINT)to_read;
  UINT outbytes = (UINT)rToWrite;

  while (!done)
  {
    HRESULT hr = mpCodec->mpInstance->DoConversion(in, &inbytes, out, &outbytes);

    if (hr != S_OK)
    {
      /* This is weird, but when there's not enough space in the output buffer :
      *
      *  - hr == E_FAIL
      *  - inbytes == rToRead (ie. inbytes is left untouched)
      *  - outbytes == rToWrite + 1
      *  - rToWrite bytes have been written to out[]
      *
      * Since we don't know how many input bytes were processed, we just
      * retry the conversion with a lessen inbytes until it fits the
      * output buffer.
      *
      * Maybe IMultiLanguage2::ConvertString() has not the bug, but it
      * requires IE5, and does not have the faintest notion of conversion
      * context.
      */
      if (outbytes > (UINT)rToWrite)
      {
        // Let's try again, but with less input bytes
        to_read -= (UINT)(rToRead / 4);
        inbytes  = (UINT)to_read;
        outbytes = (UINT)rToWrite;
      }
      else
        done = true;

      /* This is extra-weird : we can't reuse this instance once a call failed.
      * Even Initialize() won't do the trick. Of course, it destroys the whole
      * point of the conversion object. Quoting the doc :
      *
      * "To avoid creating and deleting many instances of the Conversion object,
      *  callers can reinitialize an object whenever a change in code pages is needed.
      *  A client should use this object to perform a number of conversions between the
      *  same two code pages."
      */
      mpCodec->FreeInstance();
      if (!mpCodec->GetInstance())
      {
        return -1;
      }
    }
    else
    {
      /* When an incomplete input sequence is encountered, it is not
      * reported as an error. However, if hr == S_OK, this is the
      * only reason left why less than to_read bytes have been read.
      */
/*
      if (inbytes < to_read)
        SetError("string", NGL_STRINGCONV_ELEFT);
*/

      // Stop bugging the converter, we're done
      done = true;
    }
  }

  pSource  += inbytes;
  rToRead  -= inbytes;
  pTarget  += outbytes;
  rToWrite -= outbytes;

  return 0 /*error_cnt*/;
}


/*
* Internals
*/



/*
* nglStringCodec
*/

nglStringCodec::nglStringCodec (nglTextEncoding From, nglTextEncoding To)
{
  mpInstance = NULL;
  mFromCP    = GetCP(From);
  mToCP      = GetCP(To);
}

nglStringCodec::~nglStringCodec()
{
  FreeInstance();
}

#define CONV_CTX (CLSCTX_INPROC_HANDLER | CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER)


IClassFactory* nglStringCodec::GetFactory()
{
  if (!mpFactory)
  {
#ifdef WINCE
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
    CoInitialize(NULL);
#endif
    if (S_OK != CoGetClassObject(CLSID_CMLangConvertCharset, CONV_CTX, NULL, IID_IClassFactory, (void**)&mpFactory))
    {
      mpFactory = NULL;
      CoUninitialize();
    }
/*  // Added a singleton class to handle the destruction of this object.
    else
      App->AddExit(Shutdown);
*/
  }

  return mpFactory;
}

IMLangConvertCharset* nglStringCodec::GetInstance()
{
  if (!mpInstance)
  {
    if (!mFromCP ||
      !mToCP ||
      !GetFactory() ||
      S_OK != mpFactory->CreateInstance(NULL, IID_IMLangConvertCharset, (void**)&mpInstance) ||
      S_OK != mpInstance->Initialize(mFromCP, mToCP, 0 /*MLCONVCHARF_NONE*/))
      mpInstance = NULL;
  }

  return mpInstance;
}

void nglStringCodec::FreeInstance()
{
  if (mpInstance)
  {
    mpInstance->Release();
    mpInstance = NULL;
  }
}

void nglStringCodec::Shutdown()
{
  std::map<EncodingPair, nglStringCodec*>::iterator it = mCodecs.begin();
  std::map<EncodingPair, nglStringCodec*>::iterator end = mCodecs.end();

  while (it != end)
  {
    delete it->second;
    ++it;
  }

  if (mpFactory)
  {
    mpFactory->Release();
    mpFactory = NULL;
    CoUninitialize();
  }
}

UINT nglStringCodec::GetCP (nglTextEncoding Encoding)
{
  switch (Encoding)
  {
    /* 8 bit encodings
    */
  case eISO8859_1 : return 28591;
  case eISO8859_2 : return 28592;
  case eISO8859_3 : return 28593;
  case eISO8859_4 : return 28594;
  case eISO8859_5 : return 28595;
  case eISO8859_6 : return 28596;
  case eISO8859_7 : return 28597;
  case eISO8859_8 : return 28598;
  case eISO8859_9 : return 28599;
  case eISO8859_10:
  case eISO8859_11:
  case eISO8859_12:
  case eISO8859_13:
  case eISO8859_14:
  case eISO8859_15: // FIXME ?
    return 0;

    // Mac
  case eAppleRoman: return CP_MACCP; // Actually incorrect

    // Windows code pages
  case eCP1250: return 1250;
  case eCP1251: return 1251;
  case eCP1252: return 1252;
  case eCP1253: return 1253;
  case eCP1254: return 1254;
  case eCP1255: return 1255;
  case eCP1256: return 1256;
  case eCP1257: return 1257;
  case eCP1258: return 1258;
  case eCP874 : return 874;

    /* Multi-byte encodings
    */
  case eKOI8R : return 20866;
  case eSJIS  : return 932;
  case eEUCJP : return 51932;
  case eGB2312: return 936;
  case eBig5  : return 950;

    // Windows code pages
  case eCP932  : return 932;
  case eCP936  : return 936;
  case eCP949  : return 949;
  case eCP950  : return 950;
  case eCP1200 : return 1200;

    // Unicode variants
  case eUTF7 : return CP_UTF7;
  case eUTF8 : return CP_UTF8;
  case eUCS2 : return CP_WCHAR;
  case eUCS4 : return 0;

    /* Others
    */
  case eEncodingInternal: return CP_WCHAR;
  case eEncodingNative  : return ::GetACP();

  case eEncodingUnknown:
  default:
    return 0;
  }
}

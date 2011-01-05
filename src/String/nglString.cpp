/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#pragma warning(disable : 4996)

#include "nglUTFStringConv.h"
#include "ucdata.h"

#ifdef WINCE
#define ngl_vsnwprintf	_vswprintf
#define ngl_snprintf	_snprintf
#define ngl_strcmp		wcscmp
#define ngl_stricmp 		wcsicmp
#define ngl_strncmp 		wcsncmp
#define ngl_strnicmp		wcsnicmp
#define ngl_mbs_stricmp	_stricmp
#elif defined _WIN32_
#define ngl_vsnwprintf	_vsnwprintf
#define ngl_snprintf	_snprintf
#define ngl_strcmp		wcscmp
#define ngl_stricmp 		wcsicmp
#define ngl_strncmp 		wcsncmp
#define ngl_strnicmp		wcsnicmp
#define ngl_mbs_stricmp	stricmp
#elif defined _CARBON_ || defined _UIKIT_ || defined _COCOA_
#define ngl_vsnwprintf vswprintf
#define ngl_snprintf	snprintf
#define ngl_strcmp wcscmp
#define ngl_stricmp wcscasecmp
#define ngl_strncmp wcsncmp
#define ngl_strnicmp wcsncasecmp
#define ngl_mbs_stricmp strcasecmp

static int wcscasecmp(const nglChar* s1, const nglChar* s2)
{
  nglChar c1;
  nglChar c2;
  do
  {
    int diff;

    c1 = *s1++;
    c2 = *s2++;
    if (!c1)
      break;
    diff = c1 - c2;
    if (diff)
    {
      if ((c1 >= L'A') && (c1 <= L'Z'))
        c1 += 0x20;
      if ((c2 >= L'A') && (c2 <= L'Z'))
        c2 += 0x20;
      if (c1 != c2)
        break;
    }
  } while (1);
  if (c1 < c2)
    return -1;
  return c1 - c2;
}

static int wcsncasecmp(const wchar_t* s1, const wchar_t* s2, int64 n)
{
  nglChar c1;
  nglChar c2;
  if (!n)
    return 0;
  do
  {
    int diff;

    c1 = *s1++;
    c2 = *s2++;
    if (!c1)
      break;
    diff = c1 - c2;
    if (diff)
    {
      if ((c1 >= L'A') && (c1 <= L'Z'))
        c1 += 0x20;
      if ((c2 >= L'A') && (c2 <= L'Z'))
        c2 += 0x20;
      if (c1 != c2)
        break;
    }
  } while (--n);
  if (c1 < c2)
    return -1;
  return c1 - c2;
}
#elif defined _LINUX_
#include <wctype.h>
#define ngl_vsnwprintf vswprintf
#define ngl_snprintf	snprintf
#define ngl_strcmp wcscmp
#define ngl_stricmp wcscasecmp
#define ngl_strncmp wcsncmp
#define ngl_strnicmp wcsncasecmp
#define ngl_mbs_stricmp strcasecmp
#endif

#ifdef WINCE
#define NO_LOCALE(x) \
{ \
  x; \
}
#else
#define NO_LOCALE(x) \
{ \
  int nui_locale_test = ngl_mbs_stricmp(setlocale(LC_NUMERIC, NULL), "C"); \
  if (nui_locale_test) setlocale(LC_NUMERIC, "C"); \
  x; \
  if (nui_locale_test) setlocale(LC_NUMERIC, ""); \
}
#endif

/////////////////////////////// Sometimes missing functions:

static void ngl_uitoa (uint64 x, uint32 base, nglString& _String)
{
  // Max Number is 33 digits long in binary
  if (base < 2)
    return;

  nglChar Temp[34];
  nglChar* pTemp = Temp + 33;

  *pTemp-- = 0;

  do
  {
    nglChar digit = (nglChar)(x % base);
    *pTemp = digit + (digit > 9 ? L'a' - 10 : L'0');
    x /= base;
    pTemp--;
  } while (x);

  pTemp++;
  _String = pTemp;
}

static void ngl_itoa (int64 x, int32 base, nglString& _String)
{
  if (base < 2)
    return;

  // Max Number is 33 digits long in binary
  nglChar Temp[34];
  nglChar* pTemp = Temp + 33;
  bool neg = false;

  *pTemp = 0;
  pTemp--;

  if (x < 0)
  {
    x = -x;
    neg = true;
  }

  do
  {
    nglChar digit = (nglChar)(x % base);
    *pTemp = digit + (digit > 9 ? L'a' - 10 : L'0');
    x /= base;
    pTemp--;
  } while (x);

  if (neg)
    *pTemp = L'-';
  else
    pTemp++;

  _String = pTemp;
}


static void ngl_ftoa(double x, nglString& _String, int32 precision, nglFloatFormatFlag flag)
{
  nglString fmt;

  fmt.Add(L'%');
  if (flag & ShowSign)
    fmt.Add(L'-');
  fmt.Add(L'.');
  fmt.Add((int64)precision);
  nglChar t = L'f';
  if (flag & Scientific)
    t = L'e';
  else if (flag & Condensed)
    t = L'g';

  fmt.Add(t);

  // Use the standard display with the 'C' locale:
  _String.CFormat(fmt.GetChars(), x);
}


static int ngl_getdigit(nglChar c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'z')
    return 10 + c - 'a';

  if (c >= 'A' && c <= 'Z')
    return 10 + c - 'A';

  return -1;
}

static uint64 ngl_atoui(const nglChar* str, int base);
static int64 ngl_atoi(const nglChar* str, int base);

static double ngl_atof(const nglChar* str)
{
  if (!str)
    return 0;

  bool neg = false;
  const nglChar* pStr = str;
  double value = 0;
  //  bool dotfound = false;

  if (*pStr == '-')
  {
    neg = true;
    pStr++;
  }

  nglChar c = ngl_getdigit(*pStr);
  while (*pStr && c >= 0 && c <= 9)
  {
    value = value * 10 + c;
    pStr++;
    c = ngl_getdigit(*pStr);
  }

  // Read decimal dot:
  if (*pStr == '.')
  {
    pStr++;
    double count = 1, v = 0;
    c = ngl_getdigit(*pStr);
    while (*pStr && c >= 0 && c <= 9) // Read decimal part:
    {
      v = v * 10 + c;
      count *= 10;
      pStr++;
      c = ngl_getdigit(*pStr);
    }

    if (count)
    {
      value += v / count;
    }
  }

  if (*pStr == 'e' || *pStr == 'E')
  {
    pStr++;
    int32 exponent = (int32)ngl_atoi(pStr, 10);
    value *= pow(10.0, exponent);
  }

  return neg ? -value : value;
}

static uint64 ngl_atoui(const nglChar* str, int base)
{
  if (!str)
    return 0;

  uint64 value = 0;
  const nglChar* pStr = str;

  while (*pStr)
  {
    nglChar c = ngl_getdigit(*pStr);
    if (c >= base || c < 0)
      return value;

    value = base * value + c;
    pStr++;
  }

  return value;
}

static int64 ngl_atoi(const nglChar* str, int base)
{
  if (!str)
    return 0;

  int64 value = 0;
  const nglChar* pStr = str;
  bool neg = false;

  if (*pStr == '-')
  {
    neg = true;
    pStr++;
  }

  value = ngl_atoui(pStr, base);

  if (neg)
    value = -value;

  return value;
}


////////////////////////////////////////////////////////////////

static inline int nat_isdigit(nglChar a)
{
  return iswdigit(a);
}


static inline int64 nat_isspace(nglChar a)
{
  return iswspace(a);
}


bool nglIsDigit (nglChar a)
{
  return (nat_isdigit(a) != 0);
}

bool nglIsSpace (nglChar a)
{
  return (nat_isspace(a) != 0);
}


static inline nglChar nat_toupper(nglChar a)
{
  return towupper(a);
}



static int32 compare_right(nglChar const *a, nglChar const *b)
{
  int bias = 0;

  /* The longest run of digits wins.  That aside, the greatest
  value wins, but we can't know that it will until we've scanned
  both numbers to know that they have the same magnitude, so we
  remember it in BIAS. */
  for (;; a++, b++) {
    if (!nat_isdigit(*a)  &&  !nat_isdigit(*b))
      return bias;
    else if (!nat_isdigit(*a))
      return -1;
    else if (!nat_isdigit(*b))
      return +1;
    else if (*a < *b)
    {
      if (!bias)
        bias = -1;
    }
    else if (*a > *b)
    {
      if (!bias)
        bias = +1;
    }
    else if (!*a  &&  !*b)
    {
      return bias;
    }
  }

  return 0;
}


static int32 compare_left(nglChar const *a, nglChar const *b)
{
  /* Compare two left-aligned numbers: the first to have a
  different value wins. */
  for (;; a++, b++)
  {
    if (!nat_isdigit(*a)  &&  !nat_isdigit(*b))
      return 0;
    else if (!nat_isdigit(*a))
      return -1;
    else if (!nat_isdigit(*b))
      return +1;
    else if (*a < *b)
      return -1;
    else if (*a > *b)
      return +1;
  }

  return 0;
}

static int32 strnatcmp0(nglChar const *a, nglChar const *b, int fold_case)
{
  int64 ai, bi;
  nglChar ca, cb;
  int32 fractional;
  int32 result;

  assert(a && b);
  ai = bi = 0;
  while (1)
  {
    ca = a[ai]; cb = b[bi];

    /* skip over leading spaces or zeros */
    while (nat_isspace(ca))
      ca = a[++ai];

    while (nat_isspace(cb))
      cb = b[++bi];

    /* process run of digits */
    if (nat_isdigit(ca)  &&  nat_isdigit(cb))
    {
      fractional = (ca == '0' || cb == '0');

      if (fractional)
      {
        if ((result = compare_left(a+ai, b+bi)) != 0)
          return result;
      }
      else
      {
        if ((result = compare_right(a+ai, b+bi)) != 0)
          return result;
      }
    }

    if (!ca && !cb)
    {
      /* The strings compare the same.  Perhaps the caller
      will want to call strcmp to break the tie. */
      return fold_case ? ngl_stricmp(a, b) : ngl_strcmp(a, b);
    }

    if (fold_case)
    {
      ca = nat_toupper(ca);
      cb = nat_toupper(cb);
    }

    if (ca < cb)
      return -1;
    else if (ca > cb)
      return +1;

    ++ai; ++bi;
  }

  return 0;
}

static int32 strnatcmp(nglChar const *a, nglChar const *b)
{
  return strnatcmp0(a, b, 0);
}


/* Compare, recognizing numeric string and ignoring case. */
static int32 strnatcasecmp(nglChar const *a, nglChar const *b)
{
  return strnatcmp0(a, b, 1);
}



const nglChar nglString::Zero = 0;
const nglChar nglString::Tab = '\t';
const nglChar nglString::NewLine = '\n';

nglString nglString::Null;
nglString nglString::Empty(L"");
nglString nglString::WhiteSpace(L" \n\t\r");

nglString::nglString()
: mIsNull(true)
{
}

nglString::nglString(nglChar Ch)
: mIsNull(false)
{
  mString = Ch;
}

nglString::nglString(int32 integer)
{
  SetCInt(integer);
  mIsNull = false;
}

nglString::nglString(uint32 integer)
{
  SetCUInt(integer);
  mIsNull = false;
}

nglString::nglString(float fl, int32 precision)
{
  SetCFloat(fl, precision);
  mIsNull = false;
}


nglString::nglString(double db, int32 precision)
{
  SetCDouble(db, precision);
  mIsNull = false;
}


nglString::nglString(const nglString& rSource)
: mIsNull(rSource.mIsNull),
  mString(rSource.mString)
{
}

nglString::nglString(const nglChar* pSource)
: mIsNull(pSource == NULL),
  mString(pSource ? std::wstring(pSource, wcslen(pSource)) : std::wstring())
{
}

nglString::nglString(const std::string& rSource, nglTextEncoding Encoding)
{
  Import(rSource.c_str(), Encoding);
  mIsNull = false;
}

nglString::nglString(const std::wstring& rSource)
: mIsNull(false),
  mString(rSource)
{
  mString = rSource;
  mIsNull = false;
}

nglString::nglString(const char* pSource, nglTextEncoding Encoding)
{
  mIsNull = true;
  if (!pSource)
    return;

  Import(pSource, Encoding);
  mIsNull = false;
}

nglString::nglString(const char* pSource, int32 Length, nglTextEncoding Encoding)
{
  mIsNull = true;
  if (!pSource)
    return;

  Import(pSource, Length, Encoding);
  mIsNull = false;
}

nglString::~nglString()
{
}

int32 nglString::GetLength() const
{
  return (int32)mString.size();
}

bool nglString::IsEmpty() const
{
  return mString.empty();
}

bool nglString::IsNull() const
{
  return mIsNull;
}

nglChar nglString::GetChar(uint32 Index) const
{
  if (IsEmpty())
    return 0;
  return mString[Index];
}

nglChar nglString::GetLastChar() const
{
  if (IsEmpty())
    return 0;
  return mString[(uint32)GetLength()-1];
}

const nglChar* nglString::GetChars() const
{
  return &mString[0];
}

std::string nglString::GetStdString(const nglTextEncoding Encoding) const
{
  char* pTemp = Export(Encoding);
  std::string tmp(pTemp);
  free(pTemp);
  return tmp;
}

std::wstring nglString::GetStdWString() const
{
  return mString;
}


/*
char* nglString::Export(const nglTextEncoding Encoding) const
{
int32 cw = (int32)mString.size();
int32 size = cw*2+1;
char*	psz = (char*)alloca(size);

#ifdef _WIN32_
int64 cc = WideCharToMultiByte(CP_ACP, 0, &mString[0], (int)cw, psz, (int)size, NULL, NULL);
#else
int64 cc = wcstombs(psz, mString.c_str(), (int)cw);
#endif

psz[cc] = '\0';
return strdup(psz);
}
*/

char* nglString::Export (const nglTextEncoding Encoding) const
{
  nglStringConv* pConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, Encoding)); // From=internal -> To=user 'Encoding'

  if (pConv->GetState() != eStringConv_OK)
    return NULL;

  bool done = false;

  // Source
  int32 to_read = GetLength() * sizeof(nglChar);
  const char* source = (const char*)&mString[0];

  // Output : let's try with a buffer size of GetLength() chars
  int32 written = 0;
  int32 buffer_size = GetLength() + 4;
  buffer_size *= 4 * sizeof(nglChar);
  char* buffer = (char*) malloc(buffer_size);
  memset(buffer, 0, buffer_size);
  if (!buffer)
    return NULL;

  do
  {
    char* target = buffer + written;
    *target = '\0';
    int32 to_write = buffer_size - 1 - written;
    int32 to_write0 = to_write;

#ifdef DEBUG_EXPORT
    printf("\nExport : to_read=%d  to_write=%d\n", to_read, to_write);
#endif
    pConv->Process(source, to_read, target, to_write);
    written += to_write0 - to_write;
#ifdef DEBUG_EXPORT
    printf("         to_read=%d  to_write=%d  written=%d (" SFORMAT ")\n", to_read, to_write, written, conv.GetErrorStr());
    printf("         '%ls'\n", buffer);
#endif

    switch (pConv->GetState())
    {
    case eStringConv_OK:
      // If the following errors occur, we can't go further :
    case eStringConv_RangeError:
    case eStringConv_InvalidConversion:
    case eStringConv_NeedInput:
      {
        done = true;
        break;
      }

    case eStringConv_NeedOutput:
      {
        // We need a larger output buffer. Heuristic : add half of the input buffer size
#ifdef DEBUG_EXPORT
        printf("         resize(%d -> %d)\n", buffer_size, buffer_size + (GetLength() + 1) / 2);
#endif
        int32 oldsize = buffer_size;
        buffer_size += (GetLength() + 4) / 2;
        buffer = (char*) realloc(buffer, buffer_size);
        memset(buffer + oldsize, 0, buffer_size - oldsize);
        if (!buffer)
          return NULL;
        break;
      }
    }
  }
  while (!done);

  buffer[written] = 0;
  return buffer;
}

int32 nglString::Export (int32& rOffset, char* pBuffer, int32& rToWrite, const nglTextEncoding Encoding) const
{
  if (rOffset < 0 || !pBuffer || rToWrite < 0)
    return -1;

  nglStringConv* pConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, Encoding)); // From=internal -> To=user 'Encoding'

  return Export(rOffset, pBuffer, rToWrite, *pConv);
}

int32 nglString::Export (int32& rOffset, char* pBuffer, int32& rToWrite, nglStringConv& rConv) const
{
  if (rOffset < 0 || rOffset >= GetLength() || !pBuffer || rToWrite < 0)
    return -1;

  int to_read = (GetLength() - rOffset) * sizeof(nglChar);
  int to_read0 = to_read;
  const char* source = (const char*)&mString[rOffset];

#ifdef DEBUG_EXPORT
  printf("\nExport*: to_read=%d  to_write=%d  offset=%d\n", to_read, rToWrite, rOffset);
#endif
  int errors = rConv.Process(source, to_read, pBuffer, rToWrite);
  rOffset += (to_read0 - to_read) / sizeof(nglChar);
#ifdef DEBUG_EXPORT
  printf("         to_read=%d  to_write=%d  offset=%d (" SFORMAT ")\n", to_read, rToWrite, rOffset, rConv.GetErrorStr());
#endif

  if (rConv.GetState() == eStringConv_RangeError ||
    rConv.GetState() == eStringConv_InvalidConversion)
    return -1;

  return errors;
}



nglString nglString::Extract(uint32 Index) const
{
  return Extract(Index, GetLength() - Index);
}

nglString nglString::Extract(int32 Index, int32 Length) const
{
  int32 len = GetLength();
  if (Index >= len)
    return nglString();
  if (Index + Length > len)
    Length = len - Index;
  return nglString(mString.substr(Index, Length));
}


nglString nglString::GetLeft(int32 Count) const
{
  return Extract(0, Count);
}

nglString nglString::GetRight(int32 Count) const
{
  int32 len = GetLength();
  if (Count > len)
    Count = len;
  return Extract(len - Count, Count);
}


int64 nglString::GetInt64(int Base) const
{
  return (IsEmpty() ? 0 : ngl_atoi(&mString[0], Base));
}

uint64 nglString::GetUInt64(int Base) const
{
  return (IsEmpty() ? 0 : ngl_atoui(&mString[0], Base));
}

float nglString::GetFloat() const
{
  return (IsEmpty() ? 0.0f : (float)ngl_atof(&mString[0]));
}

double nglString::GetDouble() const
{
  return (IsEmpty() ? 0.0 : ngl_atof(&mString[0]));
}


int64 nglString::GetCInt64(int Base) const
{

  return (IsEmpty() ? 0 : ngl_atoi(&mString[0], Base));
}

uint64 nglString::GetCUInt64(int Base) const
{
  return (IsEmpty() ? 0 : ngl_atoui(&mString[0], Base));
}

float nglString::GetCFloat() const
{
  return (IsEmpty() ? 0.0f : (float)ngl_atof(&mString[0]));
}

double nglString::GetCDouble() const
{
  return (IsEmpty() ? 0.0 : ngl_atof(&mString[0]));
}

bool nglString::SetChar(nglChar Ch, int32 Index)
{
  assert(Index < GetLength());
  mString[Index] = Ch;
  return true;
}


bool nglString::Fill(nglChar Pattern, int32 RepeatCount)
{
  mString.resize(RepeatCount);
  for (int32 i = 0; i < RepeatCount; i++)
    mString[i] = Pattern;
  mIsNull = false;
  return true;
}

bool nglString::Fill(const nglChar* pPattern, int32 RepeatCount)
{
  nglString str(pPattern);
  mIsNull = false;
  return Fill(str, RepeatCount);
}

bool nglString::Fill(const nglString& rPattern, int32 RepeatCount)
{
  mString.clear();
  mIsNull = false;
  for (int32 i = 0; i < RepeatCount; i++)
    mString += rPattern.GetStdWString();
  return true;
}

int32 nglString::Import(const char* pBuffer, const nglTextEncoding Encoding)
{
  if (!pBuffer)
    return false;

  mIsNull = false;
  int32 offset = 0;
  int32 len = (int32)strlen(pBuffer);

  return Import(offset, pBuffer, len, Encoding);
}

int32 nglString::Import (const char* pBuffer, int32 ByteCount, const nglTextEncoding Encoding)
{
  if (!pBuffer)
    return false;

  mIsNull = false;
  int32 offset = 0;

  return Import(offset, pBuffer, ByteCount, Encoding);
}

int32 nglString::Import (int32& rOffset, const char* pBuffer, int32& rToRead, const nglTextEncoding Encoding)
{
  if (!pBuffer || rToRead < 0)
    return -1;

  mIsNull = false;
  nglStringConv* pConv = nglString::GetStringConv(nglEncodingPair(Encoding, eEncodingInternal)); // From=user 'Encoding' -> To=internal

  return Import(rOffset, pBuffer, rToRead, *pConv);
}


int32 nglString::Import (int32& rOffset, const char* pBuffer, int32& rToRead, nglStringConv& rConv)
{
  if (!pBuffer || rToRead < 0)
    return -1;

  mIsNull = false;
  int32 errors = 0;
  int32 to_read = rToRead;
  bool done = false;

  // Simple heuristic : let's allocate rToRead nglChars (plus the untouched rOffset ones)
  mString.resize((size_t)(rOffset + rToRead));

  do
  {
    nglChar* target = (nglChar*) &mString[(size_t)rOffset];
    int32 to_write = ((int32)mString.size() - rOffset) * sizeof(nglChar);
    int32 to_write0 = to_write;

#ifdef DEBUG_IMPORT
    printf("\nImport : to_read=%d  to_write=%d  rOffset=%d\n", to_read, to_write, rOffset);
#endif
    errors += rConv.Process(pBuffer, to_read, (char*&)target, to_write);
    rOffset += (to_write0 - to_write) / sizeof(nglChar);
#ifdef DEBUG_IMPORT
    printf("         to_read=%d  to_write=%d  rOffset=%d (" SFORMAT ")\n", to_read, to_write, rOffset, rConv.GetErrorStr());
    printf("         '" SFORMAT "'\n", mpBuffer);
#endif

    switch (rConv.GetState())
    {
    case eStringConv_OK:
      // If the following errors occur, we can't go further :
    case eStringConv_RangeError:
    case eStringConv_InvalidConversion:
    case eStringConv_NeedInput:
      {
        mString.resize(rOffset);
        done = true;
        break;
      }

    case eStringConv_NeedOutput:
      {
        // We need a larger string buffer. Heuristic : add half of the input buffer size
#ifdef DEBUG_IMPORT
        printf("         resize(%d -> %d)\n", mBufferSize - 1, mBufferSize - 1 + (rToRead + 1) / 2);
#endif
        mString.resize((size_t)(mString.size() - 1 + (rToRead + 1) / 2));
        break;
      }
    }
  }
  while (!done);

  rToRead = to_read;
  return errors;
}




bool nglString::Copy(nglChar Ch)
{
  mIsNull = false;
  mString.resize(1);
  mString[0] = Ch;
  return true;
}

bool nglString::Copy(const nglChar* pSource)
{
  mIsNull = false;
  mString = pSource;
  return true;
}

bool nglString::Copy(const nglChar* pSource, int32 len)
{
  mIsNull = false;
  mString.resize(len);
  for (int32 i = 0; i < len; i++)
    mString[i] = pSource[i];
  //mString[len] = 0;
  return true;
}

bool nglString::Copy(const nglString& rSource)
{
  mIsNull = false;
  mString = rSource.mString;
  return true;
}

bool nglString::Insert(nglChar Ch, int32 Index)
{
  mIsNull = false;
  mString.insert(Index, 1, Ch);
  return true;
}

bool nglString::Insert(const nglChar* pSource, int32 Index)
{
  mIsNull = false;
  mString.insert(Index, pSource);
  return true;
}

bool nglString::Insert(const nglString& rSource, int32 Index)
{
  mIsNull = false;
  mString.insert(Index, rSource.mString);
  return true;
}

nglString& nglString::Append(nglChar Ch)
{
  mIsNull = false;
  Insert(Ch, GetLength());
  return *this;
}

nglString& nglString::Append(const nglChar* pSource)
{
  mIsNull = false;
  Insert(pSource, GetLength());
  return *this;
}

nglString& nglString::Append(const nglString& rSource)
{
  mIsNull = false;
  Insert(rSource, GetLength());
  return *this;
}

nglString& nglString::Prepend(nglChar Ch)
{
  mIsNull = false;
  Insert(Ch, 0);
  return *this;
}

nglString& nglString::Prepend(const nglChar* pSource)
{
  mIsNull = false;
  Insert(pSource, 0);
  return *this;
}

nglString& nglString::Prepend(const nglString& rSource)
{
  mIsNull = false;
  Insert(rSource, 0);
  return *this;
}

void nglString::SetCInt(int64 Value, int Base)
{
  mIsNull = false;
  ngl_itoa(Value, Base, *this);
}

void nglString::SetCUInt(uint64 Value, int Base)
{
  mIsNull = false;
  ngl_uitoa(Value, Base, *this);
}

void nglString::SetCFloat(float Value, int32 precision, nglFloatFormatFlag flag)
{
  mIsNull = false;
  ngl_ftoa(Value, *this, precision, flag);
}

void nglString::SetCDouble(double Value, int32 precision, nglFloatFormatFlag flag)
{
  mIsNull = false;
  ngl_ftoa(Value, *this, precision, flag);
}

void nglString::Nullify()
{
  mIsNull = true;
  mString.clear();
}

bool nglString::Wipe()
{
  mString.clear();
  return true;
}

bool nglString::Delete(int32 Index)
{
  assert(!Index  || (Index < (int32)mString.size()));
  mString.erase(Index, mString.size() - Index);
  return true;
}

bool nglString::Delete(int32 Index, int32 Length)
{
  assert(!Index  || (Index < (int32)mString.size()));
  assert(Index+Length <= (int32)mString.size());
  mString.erase(Index, Length);
  return true;
}

bool nglString::DeleteLeft(int32 Count)
{
  assert(Count <= (int32)mString.size());
  mString.erase(0, Count);
  return true;
}

bool nglString::DeleteRight(int32 Count)
{
  assert(Count <= GetLength());
  Delete(GetLength() - Count);
  return true;
}

bool nglString::Trim()
{
  TrimLeft(WhiteSpace);
  TrimRight(WhiteSpace);
  return true;
}

bool nglString::Trim (nglChar Ch)
{
  TrimLeft(Ch);
  TrimRight(Ch);
  return true;
}

bool nglString::Trim(const nglChar* pTrimSet)
{
  TrimLeft(pTrimSet);
  TrimRight(pTrimSet);
  return true;
}

bool nglString::Trim(const nglString& rTrimSet)
{
  TrimLeft(rTrimSet);
  TrimRight(rTrimSet);
  return true;
}


bool nglString::TrimLeft()
{
  TrimLeft(WhiteSpace);
  return true;
}

bool nglString::TrimLeft(nglChar Ch)
{
  nglChar tmp[2] = {Ch, 0};
  TrimLeft(tmp);
  return true;
}

bool nglString::TrimLeft(const nglChar* pTrimSet)
{
  int32 i = 0;
  int32 len = (int32)mString.size();
  int j = 0;
  while (i < len)
  {
    j = 0;
    while (pTrimSet[j] && pTrimSet[j] != mString[i])
      j++;
    if (!pTrimSet[j])
    {
      if (i)
        DeleteLeft(i);
      return true;
    }
    i++;
  }
  if (i)
    DeleteLeft(i);
  return false;
}

bool nglString::TrimLeft(const nglString& rTrimSet)
{
  TrimLeft(rTrimSet.GetChars());
  return true;
}


bool nglString::TrimRight()
{
  TrimRight(WhiteSpace);
  return true;
}

bool nglString::TrimRight(nglChar Ch)
{
  nglChar tmp[2] = {Ch, 0};
  TrimRight(tmp);
  return true;
}

bool nglString::TrimRight(const nglChar* pTrimSet)
{
  int32 len = (int32)mString.size();
  int32 i = len - 1;
  while (i >= 0)
  {
    int j = 0;
    while (pTrimSet[j] && pTrimSet[j] != mString[i])
      j++;
    if (!pTrimSet[j])
    {
      if (i != len - 1)
        DeleteRight(len - i - 1);
      return true;
    }
    i--;
  }
  return false;
}

bool nglString::TrimRight(const nglString& rTrimSet)
{
  TrimRight(rTrimSet.GetChars());
  return true;
}


bool nglString::Replace(int32 Index, int32 Length, nglChar Ch)
{
  mString.replace(Index, Length, 1, Ch);
  return true;
}

bool nglString::Replace(int32 Index, int32 Length, const nglChar* pSource)
{
  mString.replace(Index, Length, pSource);
  return true;
}


bool nglString::Replace(int32 Index, int32 Length, const nglString& rSource)
{
  mString.replace(Index, Length, rSource.GetStdWString());
  return true;
}


bool nglString::Replace(const nglChar Old, const nglChar New)
{
  bool changed = false;
  int32 len = (int32)mString.size();
  for (int32 i = 0; i < len; i++)
    if (mString[i] == Old)
    {
      mString[i] = New;
      changed = true;
    }
    return changed;
}

bool nglString::Replace(const nglChar* pOld, const nglChar* pNew)
{
  return Replace(nglString(pOld), nglString(pNew));
}

bool nglString::Replace(const nglString& rOld, const nglString& rNew)
{
  bool changed = false;
  int32 i = 0;
  int32 len = GetLength();
  int32 oldlen = rOld.GetLength();
  int32 newlen = rNew.GetLength();

  while (i < len && i >= 0)
  {
    i = Find(rOld, i);
    if (i < 0)
      return changed;
    Delete(i, oldlen);
    Insert(rNew, i);
    i += newlen;
    len = GetLength();
  }
  return changed;
}


nglString& nglString::ToUpper()
{
  int32 len = GetLength();
  for (int32 i = 0; i < len; i++)
    mString[i] = towupper(mString[i]);
  return *this;
}

nglString& nglString::ToUpper(int32 Index, int32 Length)
{
  int32 len = GetLength();
  if (Index + Length <= len)
    len = Length;
  for (int32 i = Index; i < len; i++)
    mString[i] = towupper(mString[i]);
  return *this;
}

nglString& nglString::ToLower()
{
  int32 len = GetLength();
  for (int32 i = 0; i < len; i++)
    mString[i] = towlower(mString[i]);
  return *this;
}

nglString& nglString::ToLower(int32 Index, int32 Length)
{
  int32 len = GetLength();
  if (Index + Length <= len)
    len = Length;
  for (int32 i = Index; i < len; i++)
    mString[i] = towlower(mString[i]);
  return *this;
}

std::set<char> nglString::gURLDontEncodeChars;

char* nglString::EncodeUrl()
{
  if (nglString::gURLDontEncodeChars.empty())
  {
    char c;
    for (c = 'a'; c <= 'z'; c++)
    {
      gURLDontEncodeChars.insert(c);
    }
    for (c = 'A'; c <= 'Z'; c++)
    {
      gURLDontEncodeChars.insert(c);
    }
    for (c = '0'; c <= '9'; c++)
    {
      gURLDontEncodeChars.insert(c);
    }
    gURLDontEncodeChars.insert('-');
    gURLDontEncodeChars.insert('_');
    gURLDontEncodeChars.insert('.');
    gURLDontEncodeChars.insert('*');
    gURLDontEncodeChars.insert(' ');
  }

  char* pExportChars = Export(eUTF8);
  int32 exportLen = strlen(pExportChars);
  int32 resultLen = exportLen + 1;
  char* pResultChars = (char*)malloc(resultLen);
  int j = 0;
  std::set<char>::const_iterator end = gURLDontEncodeChars.end();
  for (int i = 0; i < exportLen; i++)
  {
    char c = pExportChars[i];
    if (nglString::gURLDontEncodeChars.find(c) != end) {
      if (c == ' ')
      {
        pResultChars[j] = '+';
      }
      else
      {
        pResultChars[j] = c;
      }
      j++;
    }
    else
    {
      resultLen += 3;
      pResultChars = (char*)realloc(pResultChars, resultLen);
      ngl_snprintf(&pResultChars[j], 4, "%%%02hhX", c); // puts a '\0' at the end of snprintf, not a problem: enough space, and will be re-written
      j+=3;
    }
  }
  free(pExportChars);
  pResultChars[j] = NULL;
  return pResultChars;
}

static const char HEX2DEC[256] =
{
  /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
  /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

  /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

  /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

  /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

inline static nglChar Hex2Dec(nglChar c)
{
  if (c > 255)
    return -1;
  return HEX2DEC[c];
}

void nglString::DecodeUrl()
{
  // Note from RFC1630: "Sequences which start with a percent
  // sign but are not followed by two hexadecimal characters
  // (0-9, A-F) are reserved for future extension"

  const nglChar* pSrc = GetChars();
  const int32 SRC_LEN = GetLength();
  const nglChar* const SRC_END = pSrc + SRC_LEN;
  // last decodable '%'
  const nglChar * const SRC_LAST_DEC = SRC_END - 2;

  const nglChar* pStart = new nglChar[SRC_LEN + 1];
  nglChar* pEnd = const_cast<nglChar*>(pStart);

  while (pSrc < SRC_LAST_DEC)
  {
    if (*pSrc == '%')
    {
      char dec1, dec2;
      if (-1 != (dec1 = Hex2Dec(*(pSrc + 1)))
          && -1 != (dec2 = Hex2Dec(*(pSrc + 2))))
      {
        *pEnd++ = (dec1 << 4) + dec2;
        pSrc += 3;
        continue;
      }
    }

    *pEnd++ = *pSrc++;
  }

  // the last 2- chars
  while (pSrc < SRC_END)
    *pEnd++ = *pSrc++;

  *pEnd = 0;
  Copy(pStart);
  delete [] pStart;
}


void nglString::EncodeToXML()
{
  Replace(nglString((nglChar)'&'), nglString("&amp;"));
  Replace(nglString((nglChar)'\"'), nglString("&quot;"));
  Replace(nglString((nglChar)'\''), nglString("&apos;"));
  Replace(nglString((nglChar)'<'), nglString("&lt;"));
  Replace(nglString((nglChar)'>'), nglString("&gt;"));
}

void nglString::DecodeFromXML()
{
  Replace(nglString("&amp;")  , nglString((nglChar)'&'));
  Replace(nglString("&quot;") , nglString((nglChar)'\"'));
  Replace(nglString("&apos;") , nglString((nglChar)'\''));
  Replace(nglString("&lt;")   , nglString((nglChar)'<'));
  Replace(nglString("&gt;")   , nglString((nglChar)'>'));
  Replace(nglString("&eacute;"), nglString((nglChar)233));
}

void nglString::Unescape()
{
  nglString res;
  char* pExport = Export();
  char* pRead = pExport;
  char* pWrite = pExport;

  while (*pRead)
  {
    if (*pRead == '\\')
    {
      pRead++;

      nglString number;
      char c = *pRead++;
      while (c >= '0' && c <= '9')
      {
        number += c;
        pRead++;
        c = *pRead;
      }

      char unescaped = number.GetCInt();
      *pWrite++ += unescaped;
    }
    else
    {
      *pWrite++ = *pRead++;
    }
  }

  *pWrite = 0;

  Import(pExport);
  delete pExport;
}


//void nglString::EncodeToWeb()
//{
//  Replace(nglString((nglChar)'&'), nglString("&amp;"));
//}
//
//void nglString::DecodeFromWeb()
//{
//  Replace(nglString("&amp;")  , nglString((nglChar)'&'));
//}


nglString& nglString::Format(const nglString& rFormat, ...)
{
  if (rFormat.IsNull())
    return *this;

  mIsNull = false;
  va_list args;

  va_start(args, rFormat);
  Formatv(rFormat, args);
  va_end(args);

  return *this;
}


nglString& nglString::Format(const nglChar* pFormat, ...)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  va_list args;

  va_start(args, pFormat);
  Formatv(pFormat, args);
  va_end(args);

  return *this;
}

nglString& nglString::Format(const char* pFormat, ...)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  nglString str(pFormat);
  va_list args;

  va_start(args, pFormat);
  Formatv(str.GetChars(), args);
  va_end(args);

  return *this;
}

#define FORMAT_BUFSIZE (128)

nglString& nglString::Formatv(const nglChar* pFormat, va_list args)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  nglChar sbuffer[FORMAT_BUFSIZE+1];
  memset(sbuffer, 0, (FORMAT_BUFSIZE + 1) * sizeof(nglChar));
  int len;

#ifndef WIN32
  va_list args_copy;
  va_copy(args_copy, args);
#else 
//#define args_copy args
  va_list args_copy = args;
#endif

  // Try to render in stack buffer
  len = ngl_vsnwprintf(sbuffer, FORMAT_BUFSIZE, pFormat, args_copy);
  va_end(args_copy);

#ifdef __APPLE__
  // #HACK #TODO #FIXME vswprintf is broken on OSX! It sometimes returns -1 for no apparent reason!!!
  // Let's try to workaround the most simple problems while we find a real long term solution:
  if (len == -1)
  {
    int l = wcslen(sbuffer);
    if (l < FORMAT_BUFSIZE)
      len = l;
  }
#endif
  
  if (len >= 0 && len <= FORMAT_BUFSIZE)
  {
    /* Output fits in the stack buffer, copy it
    */
    Copy(sbuffer);
    return *this;
  }
  else
  {
    /* Output does not fit in the stack buffer.
    * Windows brain dead xxprintf implementation does not return needed space
    * (as ISOC99 mandates), so we guess more or less.
    * We iterate with a growing buffer, starting from 2*FORMAT_BUFSIZE.
    */
    int bufsize = FORMAT_BUFSIZE;

    do
    {
      // Try with a buffer twice as large
      bufsize *= 2;
      mString.resize(bufsize);

#ifndef WIN32
      va_copy(args_copy, args);
#else
      args_copy = args;
#endif
      len = ngl_vsnwprintf(&mString[0], bufsize, pFormat, args_copy);
      va_end(args_copy);

      if (len >= 0 && len <= bufsize)
      {
        /* Fits, hurra !
        * In the event where len==bufsize (no zero terminal written), we have to terminate the string.
        */
        mString.resize(len);
        return *this;
      }
    }
    while (len == -1); // Does not fit (yet), try again. Otherwise an error occured, exit.
  }

  return *this;
}

nglString& nglString::Formatv(const char* pFormat, va_list Args)
{
  mIsNull = false;
  nglString str(pFormat);
  return Formatv(str.GetChars(), Args);
}

nglString& nglString::Formatv(const nglString& rFormat, va_list Args)
{
  mIsNull = false;
  return Formatv(rFormat.GetChars(), Args);
}


nglString& nglString::CFormat(const nglChar* pFormat, ...)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  va_list args;

  va_start(args, pFormat);
  NO_LOCALE( Formatv(pFormat, args) );
  va_end(args);

  return *this;
}

nglString& nglString::CFormat(const nglString& rFormat, ...)
{
  if (rFormat.IsNull())
    return *this;

  mIsNull = false;
  va_list args;

  va_start(args, rFormat);
  NO_LOCALE( Formatv(rFormat, args) );
  va_end(args);

  return *this;
}

nglString& nglString::CFormat(const char* pFormat, ...)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  nglString str(pFormat);

  va_list args;

  va_start(args, pFormat);
  NO_LOCALE( Formatv(str.GetChars(), args) );
  va_end(args);

  return *this;
}


nglString& nglString::CFormatv(const nglChar* pFormat, va_list Args)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  NO_LOCALE( Formatv(pFormat, Args) );
  return *this;
}

nglString& nglString::CFormatv(const nglString& rFormat, va_list Args)
{
  if (rFormat.IsNull())
    return *this;

  mIsNull = false;
  NO_LOCALE( Formatv(rFormat, Args) );
  return *this;
}

nglString& nglString::CFormatv(const char* pFormat, va_list Args)
{
  if (!pFormat)
    return *this;

  mIsNull = false;
  NO_LOCALE( Formatv(pFormat, Args) );
  return *this;
}


nglString& nglString::AddFormat(const char* pString, ...)
{
  nglString tmp;
  nglString str(pString);

  va_list args;

  va_start(args, pString);
  NO_LOCALE( tmp.Formatv(str.GetChars(), args) );
  va_end(args);

  return Add(tmp);
}

nglString& nglString::AddFormat(const char* pString, va_list Args)
{
  nglString tmp;
  nglString str(pString);

  NO_LOCALE( tmp.Formatv(str.GetChars(), Args) );

  return Add(tmp);
}

nglString& nglString::AddFormat(const nglChar* pString, ...)
{
  nglString tmp;
  va_list args;

  va_start(args, pString);
  NO_LOCALE( tmp.Formatv(pString, args) );
  va_end(args);

  return Add(tmp);
}

nglString& nglString::AddFormat(const nglChar* pString, va_list Args)
{
  nglString tmp;

  NO_LOCALE( tmp.Formatv(pString, Args) );

  return Add(tmp);
}



nglString& nglString::Add(nglChar ch)
{
  mIsNull = false;
  Append(ch);
  return *this;
}

nglString& nglString::Add(int8 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(uint8 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCUInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(int16 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(uint16 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCUInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(int32 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(uint32 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCUInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(int64 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(uint64 s, int base)
{
  nglString str;
  mIsNull = false;
  str.SetCUInt(s, base);
  Append(str);
  return *this;
}

nglString& nglString::Add(float s, int32 precision, nglFloatFormatFlag flag)
{
  nglString str;
  mIsNull = false;
  str.SetCFloat(s, precision, flag);
  Append(str);
  return *this;
}

nglString& nglString::Add(double s, int32 precision, nglFloatFormatFlag flag)
{
  nglString str;
  mIsNull = false;
  str.SetCDouble(s, precision, flag);
  Append(str);
  return *this;
}

nglString& nglString::Add(const char* pString, int32 count)
{
  mIsNull = false;
  nglString str(pString);
  for (int32 i = 0; i < count; i++)
    Append(str);
  return *this;
}

nglString& nglString::Add(const nglChar* pString, int32 count)
{
  mIsNull = false;
  for (int32 i = 0; i < count; i++)
    Append(pString);
  return *this;
}

nglString& nglString::Add(const nglString& rString, int32 count)
{
  mIsNull = false;
  for (int32 i = 0; i < count; i++)
    Append(rString);
  return *this;
}

nglString& nglString::Add(void* pVoidPointer)
{
  mIsNull = false;
  nglString str;
  str.CFormat("0x%x", pVoidPointer);
  Append(str);
  return *this;
}

nglString& nglString::AddNewLine()
{
  mIsNull = false;
  Append(_T("\n"));
  return *this;
}


int32 nglString::HexDump(const char* pBuffer, int32 ByteCount, bool PrintChar, int32 Columns)
{
  nglString temp;
  int32 pos = 0;
  int32 line = 0;

  Wipe();

  while (pos < ByteCount)
  {
    int32 i, lpos;

    // Offset
    temp.Format(L"%.8x  ", pos);
    *this += temp;

    // Hex display
    lpos = pos;
    for (i = 0; i < Columns; i++)
    {
      if (lpos < ByteCount)
      {
        temp.Format(L"%.2x ", (unsigned char)pBuffer[lpos]);
        lpos++;
        *this += temp;
      }
      else
        *this += L"   ";

      if (i + 1 == Columns / 2)
        *this += L' ';
    }

    if (PrintChar)
    {
      // nglChar display
      lpos = pos;
      *this += L" |";
      for (i = 0; i < Columns; i++)
      {
        if (lpos < ByteCount)
        {
          nglChar c = (nglChar)pBuffer[lpos];

          *this += iswprint(c) ? c : L'.';
          lpos++;
        }
        else
          *this += L' ';
      }
      *this += L'|';
    }

    *this += L'\n';
    pos += Columns;
    line++;
  }

  return line;
}


int32 nglString::Compare(const nglChar* pSource, bool CaseSensitive) const
{
  const nglChar* pLocal = &mString[0];
  int result = (CaseSensitive) ?
    ngl_strcmp(pLocal, pSource) :
  ngl_stricmp(pLocal, pSource);

  if (result < 0) result = -1;
  if (result > 0) result = 1;
  return result;
}

int32 nglString::Compare(const nglString& rSource, bool CaseSensitive) const
{
  return Compare(&rSource.mString[0], CaseSensitive);
}

int32 nglString::Compare(const nglChar* pSource, int32 Index, int32 Length, bool CaseSensitive) const
{
  // Warning, no check against pSource bounds wrt. Index
  int result = (CaseSensitive) ?
    ngl_strncmp (&(mString[Index]), pSource, Length) :
  ngl_strnicmp(&(mString[Index]), pSource, (int64)Length);

  if (result < 0) result = -1;
  if (result > 0) result = 1;
  return result;
}

int32 nglString::Compare(const nglString& rSource, int32 Index, int32 Length, bool CaseSensitive) const
{
  return Compare(&rSource.mString[0], Index, Length, CaseSensitive);
}

int32 nglString::CompareNatural(const nglChar* pSource, bool CaseSensitive) const
{
  int32 result = (CaseSensitive) ?
    strnatcmp(&mString[0], pSource) :
  strnatcasecmp(&mString[0], pSource);

  if (result < 0) result = -1;
  if (result > 0) result = 1;
  return result;
}

int nglString::CompareNatural(const nglString& rSource, bool CaseSensitive) const
{
  return CompareNatural(&rSource.mString[0], CaseSensitive);
}

int32 nglString::CompareLeft(const nglChar* pSource, bool CaseSensitive) const
{
  if (CaseSensitive)
  {
    uint32 pos = 0;
    int t = 0;
    do
    {
      const nglChar c = mString[pos];
      const nglChar cc = pSource[pos];
      if (!cc)
        return 0;
      t = cc - c;
      pos++;
    } while (!t);
    return t;
  }

  // !Case Sens
  uint32 pos = 0;
  int t = 0;
  do
  {
    const nglChar c = towlower(mString[pos]);
    const nglChar cc = towlower(pSource[pos]);
    if (!cc)
      return 0;
    t = cc - c;
    pos++;
  } while (!t);
  return t;
}

int32 nglString::CompareLeft(const nglString& rSource, bool CaseSensitive) const
{
  return CompareLeft(&rSource.mString[0], CaseSensitive);
}

int32 nglString::Find(nglChar Ch, int32 Index, int32 End,  bool CaseSensitive) const
{
  int32 len = GetLength();
  int32 end = MIN(End, len);

  if (IsEmpty() || (Ch == Zero) || (Index >= len))
    return -1;

  int32 i = Index;
  if (CaseSensitive)
  {
    while ((i < end) && (mString[i] != Ch))
      i++;
  }
  else
  {
    nglChar up = towupper(Ch);
    nglChar lo = towlower(Ch);

    while ((i < end) && (mString[i] != up) && (mString[i] != lo))
      i++;
  }

  return (i < end) ? i : -1;

}

int32 nglString::Find(nglChar Ch, int32 Index, bool CaseSensitive) const
{
  int32 len = GetLength();
  if (IsEmpty() || (Ch == Zero) || (Index >= len))
    return -1;

  int32 i = Index;
  if (CaseSensitive)
  {
    while ((i < len) && (mString[i] != Ch))
      i++;
  }
  else
  {
    nglChar up = towupper(Ch);
    nglChar lo = towlower(Ch);

    while ((i < len) && (mString[i] != up) && (mString[i] != lo))
      i++;
  }

  return (i < len) ? i : -1;
}

int32 nglString::Find(const nglChar* pSource, int32 Index, bool CaseSensitive) const
{
  const nglChar* pFound = wcsstr(&mString[Index], pSource);
  if (!pFound)
    return -1;
  return Index + (int32)(pFound - &mString[Index]);
}

int32 nglString::Find(const nglString& rSource, int32 Index, bool CaseSensitive) const
{
  return Find(rSource.GetChars(), Index, CaseSensitive);
}

int32 nglString::FindLast(nglChar Ch, int32 Index, bool CaseSensitive) const
{
  if (IsEmpty() || (Ch == Zero)) return -1;

  int32 len = GetLength();
  if (Index < 0)
    Index += len;
  if ((Index < 0) || (Index >= len)) return -1;

  if (CaseSensitive)
  {
    while ((Index >= 0) && (mString[Index] != Ch))
      Index--;
  }
  else
  {
    nglChar up = towupper(Ch);
    nglChar lo = towlower(Ch);

    while ((Index >= 0) && (mString[Index] != up) && (mString[Index] != lo))
      Index--;
  }

  return Index;
}

int32 nglString::FindLast(const nglChar* pSource, int32 Index, bool CaseSensitive) const
{
  int32 SourceLength = (int32)wcslen(pSource);
  if (IsEmpty() || !pSource || (SourceLength <= 0))
    return -1;

  // In reverse mode, negative indexes become GetLength()-index
  if (Index < 0)
    Index += GetLength();

  if ((Index < 0) || (Index >= GetLength()))
    return -1;

  nglString* buffer_s = NULL;
  nglString* source_s = NULL;
  const nglChar* buffer = NULL;
  const nglChar* source = NULL;

  if (!CaseSensitive)
  {
    buffer_s = new nglString(*this);
    if (!buffer_s) return -1;

    source_s = new nglString(pSource);
    if (!source_s)
    {
      delete buffer_s;
      return -1;
    }

    buffer_s->ToLower();
    source_s->ToLower();
    buffer = buffer_s->GetChars();
    source = source_s->GetChars();
  }
  else
  {
    buffer = GetChars();
    source = pSource;
  }

  int32 pos;

  pos = Index + 1 - SourceLength;

  while ((pos >= 0) && ngl_strncmp(&(buffer[pos]), source, SourceLength))
    pos--;

  if (!CaseSensitive)
  {
    delete source_s;
    delete buffer_s;
  }

  return pos;
}

int32 nglString::FindLast(const nglString& rSource, int32 Index, bool CaseSensitive) const
{
  return FindLast(rSource.GetChars(), Index, CaseSensitive);
}


int32 nglString::Contains(nglChar Ch, int32 Index, int32 End, bool CaseSensitive) const
{
  if (IsEmpty() || (Ch == Zero)) return 0;

  int32 end = MIN(GetLength(), End);
  int32 match = 0;

  if (CaseSensitive)
  {
    int32 i;
    for (i = Index; i < end; i++)
      if (mString[i] == Ch) match++;
  }
  else
  {
    nglChar up = towupper(Ch);
    nglChar lo = towlower(Ch);
    int32 i;

    for (i = Index; i < end; i++)
      if ((mString[i] == up) || (mString[i] == lo)) match++;
  }
  return match;
}

int32 nglString::Contains(nglChar Ch, bool CaseSensitive) const
{
  if (IsEmpty() || (Ch == Zero))
    return 0;

  int32 len = GetLength(), match = 0;

  if (CaseSensitive)
  {
    int32 i;
    for (i = 0; i < len; i++)
      if (mString[i] == Ch) match++;
  }
  else
  {
    nglChar up = towupper(Ch);
    nglChar lo = towlower(Ch);
    int32 i;

    for (i = 0; i < len; i++)
      if ((mString[i] == up) || (mString[i] == lo)) match++;
  }

  return match;
}

int32 nglString::Contains(const nglChar* pSource, bool CaseSensitive) const
{
  int32 SourceLength = (int32)wcslen(pSource);
  if (IsEmpty() || !pSource || (SourceLength <= 0))
    return 0;

  nglString* buffer_s = NULL;
  nglString* source_s = NULL;
  const nglChar* buffer = NULL;
  const nglChar* source = NULL;

  if (!CaseSensitive)
  {
    buffer_s = new nglString(*this);
    if (!buffer_s) return -1;

    source_s = new nglString(pSource);
    if (!source_s)
    {
      delete buffer_s;
      return -1;
    }

    buffer_s->ToLower();
    source_s->ToLower();
    buffer = buffer_s->GetChars();
    source = source_s->GetChars();
  }
  else
  {
    buffer = GetChars();
    source = pSource;
  }

  int match = 0;
  const nglChar* ptr = buffer;

  while ((ptr = wcsstr(ptr, source)))
  {
    match++;
    ptr += SourceLength;
  }

  if (!CaseSensitive)
  {
    delete source_s;
    delete buffer_s;
  }

  return match;
}

int32 nglString::Contains(const nglString& rSource, bool CaseSensitive) const
{
  return Contains(rSource.GetChars());
}


int32 nglString::Tokenize(std::vector<nglString>& rTokens, bool CreateEmptyTokens) const
{
  return Tokenize(rTokens, WhiteSpace.GetChars(), CreateEmptyTokens);
}


int32 nglString::Tokenize(std::vector<nglString>& rTokens, nglChar Separator, bool CreateEmptyTokens) const
{
  nglChar sep[2];

  sep[0] = Separator;
  sep[1] = Zero;
  return Tokenize(rTokens, sep, CreateEmptyTokens);
}

int32 nglString::Tokenize(std::vector<nglString>& rTokens, const nglChar* pSeparators, bool CreateEmptyTokens) const
{
  if (IsEmpty() || !pSeparators/* || !*pSeparators*/)
    return -1;

  int32 index = 0, count = 0;
  while (index < GetLength())
  {
    int32 len = (int32) wcscspn(&(mString[index]), pSeparators);
    if (len > 0 || (!len && CreateEmptyTokens))
    {
      nglString token;

      if (len)
        token.Copy(&(mString[index]), len);

      rTokens.push_back(token);
      count++;
    }
    index += len + 1;
  }

  return count;
}

int32 nglString::Tokenize(std::vector<nglString>& rTokens, const nglString& rSeparators, bool CreateEmptyTokens) const
{
  return Tokenize(rTokens, rSeparators.GetChars(), CreateEmptyTokens);
}


// Assignment
const nglString& nglString::operator=(const nglChar Ch)
{
  Copy(Ch);
  return *this;
}

const nglString& nglString::operator=(const char* pSource)
{
  Copy(nglString(pSource));
  return *this;
}

const nglString& nglString::operator=(const nglChar* pSource)
{
  Copy(pSource);
  return *this;
}

const nglString& nglString::operator=(const nglString& rSource)
{
  Copy(rSource);
  return *this;
}

bool nglString::IsInt() const
{
  if (IsEmpty())
    return false;

  int32 index = 0;
  nglChar ch = mString[0];
  if (ch == '-')
    index++;

  for (; index < GetLength(); index++)
  {
    ch = mString[index];
    if (ch < '0' || ch > '9')
      return false;
  }

  return true;
}

bool nglString::IsFloat() const
{
  if (IsEmpty())
    return false;

  int32 index = 0;
  nglChar ch = mString[0];
  if (ch == '-' || ch == '+') // Pre minus sign
    index++;

  bool hasDigits = false;
  for (; index < GetLength(); index++)
  {
    ch = mString[index];
    if (ch < '0' || ch > '9') // Look for digits?
    {
      switch (ch)
      {
      case '.': // Decimal point, look for decimal digits or exponents
        hasDigits = false;
        index++;
        for (; index < GetLength(); index++)
        {
          ch = mString[index];
          if (ch < '0' || ch > '9') // Look for digits
          {
            if (ch == 'e' || ch == 'E')
            {
              if (!hasDigits)
                return false;
              hasDigits = false;
              index++;
              ch = mString[index];
              if (ch == '-' || ch == '+')
                index++;
              for (; index < GetLength(); index++)
              {
                ch = mString[index];
                if (ch < '0' || ch > '9')
                  return false;
                else
                  hasDigits = true;
              }
              return hasDigits;
            }
            else
            {
              return false;
            }
          }
          else
          {
            hasDigits = true;
          }
        }
        return hasDigits;
        break;
      case 'e':
      case 'E':
        if (!hasDigits)
          return false;
        hasDigits = false;
        index++;
        ch = mString[index];
        if (ch == '-' || ch == '+')
          index++;
        for (; index < GetLength(); index++)
        {
          ch = mString[index];
          if (ch < '0' || ch > '9')
            return false;
          else
            hasDigits = true;
        }
        return hasDigits;
        break;
      }
      return false;
    }
    else
    {
      hasDigits = true;
    }
  }

  return hasDigits;
}

nglChar nglStringConv::mUnknown = L'?';

nglStringConvMap nglString::gStringConvCache;

nglStringConv* nglString::GetStringConv(const nglEncodingPair& rEncodings)
{
  if (gStringConvCache.empty())
  {
    nglEncodingPair p = nglEncodingPair(eUTF8, eUCS2);
    nglStringConv* pConv = new nglUTFStringConv(eUTF8, eUCS2);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUTF8, eUCS4);
    pConv = new nglUTFStringConv(eUTF8, eUCS4);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS2, eUTF8);
    pConv = new nglUTFStringConv(eUCS2, eUTF8);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS2, eUCS4);
    pConv = new nglUTFStringConv(eUCS2, eUCS4);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS4, eUTF8);
    pConv = new nglUTFStringConv(eUCS4, eUTF8);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS4, eUCS2);
    pConv = new nglUTFStringConv(eUCS4, eUCS2);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));

    p = nglEncodingPair(eEncodingNative, eUTF8);
    pConv = new nglUTFStringConv(eEncodingNative, eUTF8);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eEncodingNative, eUCS2);
    pConv = new nglUTFStringConv(eEncodingNative, eUCS2);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eEncodingNative, eUCS4);
    pConv = new nglUTFStringConv(eEncodingNative, eUCS4);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUTF8, eEncodingNative);
    pConv = new nglUTFStringConv(eUTF8, eEncodingNative);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS2, eEncodingNative);
    pConv = new nglUTFStringConv(eUCS2, eEncodingNative);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS4, eEncodingNative);
    pConv = new nglUTFStringConv(eUCS4, eEncodingNative);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));

    p = nglEncodingPair(eUTF8, eEncodingInternal);
    pConv = new nglUTFStringConv(eUTF8, eEncodingInternal);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS2, eEncodingInternal);
    pConv = new nglUTFStringConv(eUCS2, eEncodingInternal);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eUCS4, eEncodingInternal);
    pConv = new nglUTFStringConv(eUCS4, eEncodingInternal);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eEncodingInternal, eUTF8);
    pConv = new nglUTFStringConv(eEncodingInternal, eUTF8);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eEncodingInternal, eUCS2);
    pConv = new nglUTFStringConv(eEncodingInternal, eUCS2);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eEncodingInternal, eUCS4);
    pConv = new nglUTFStringConv(eEncodingInternal, eUCS4);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));

    p = nglEncodingPair(eEncodingNative, eEncodingInternal);
    pConv = new nglUTFStringConv(eEncodingNative, eEncodingInternal);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
    p = nglEncodingPair(eEncodingInternal, eEncodingNative);
    pConv = new nglUTFStringConv(eEncodingInternal, eEncodingNative);
    gStringConvCache.insert(std::pair<const nglEncodingPair, nglStringConv*>(p, pConv));
  }

  nglStringConvMap::iterator it = gStringConvCache.find(rEncodings);
  if (it != gStringConvCache.end())
    return it->second;

  nglStringConv* pConverter = new nglStringConv(rEncodings.first, rEncodings.second);
  NGL_ASSERT(pConverter);

  if (pConverter->GetState() == eStringConv_OK)
  {
    gStringConvCache.insert(std::pair<nglEncodingPair, nglStringConv*>(rEncodings, pConverter));
    return pConverter;
  }

  delete pConverter;

  return NULL;
}

void nglString::ReleaseStringConvs()
{
  nglStringConvMap::iterator it = gStringConvCache.begin();
  nglStringConvMap::iterator end = gStringConvCache.end();

  while (it != end)
  {
    delete it->second;

    ++it;
  }
  gStringConvCache.clear();
}

uint32 nglString::GetLevenshteinDistance(const nglString& rSource, bool CaseSensitive)
{
  const size_t len1 = GetLength(), len2 = rSource.GetLength();
  std::vector<std::vector<uint32> > d(len1 + 1, std::vector<uint32>(len2 + 1));

  if (CaseSensitive)
  {
    for(size_t i = 1; i <= len1; ++i)
    {
      for(size_t j = 1; j <= len2; ++j)
      {
        d[i][j] = MIN(d[i - 1][j] + 1, MIN(d[i][j - 1] + 1, d[i - 1][j - 1] + (mString[i - 1] == rSource.mString[j - 1] ? 0 : 1)));
      }
    }
  }
  else
  {
    for(size_t i = 1; i <= len1; ++i)
    {
      for(size_t j = 1; j <= len2; ++j)
      {
        d[i][j] = MIN(d[i - 1][j] + 1, MIN(d[i][j - 1] + 1, d[i - 1][j - 1] + (toupper(mString[i - 1]) == toupper(rSource.mString[j - 1]) ? 0 : 1)));
      }
    }
  }

  return d[len1][len2];
}

void nglString::ToCanonicalComposition()
{
  uint32* exprt = (uint32*)Export(eUCS4);
  uint32 len = GetLength();
  uint32 reslen = uccanoncomp(exprt, len);
  Import((char*)exprt, reslen * sizeof(uint32), eUCS4);
  delete exprt;
}

void nglString::ToCanonicalDecomposition()
{
  uint32* exprt = (uint32*)Export(eUCS4);
  uint32 len = GetLength();
  int32 reslen = 0;
  uint32* resstr = NULL;
  int32 res = uccanondecomp(exprt, len, &resstr, &reslen);
  if (res >= 0 && resstr)
    Import((char*)resstr, reslen * sizeof(uint32), eUCS4);
  delete exprt;
  free(resstr);
}

/* Base64 code is stolen from http://www.adp-gmbh.ch/cpp/common/base64.html and adapted to nui's strings by SŽbastien MŽtrot.
 
 base64.cpp and base64.h
 
 Copyright (C) 2004-2008 RenŽ Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 RenŽ Nyffenegger rene.nyffenegger@adp-gmbh.ch
 
 */

static const std::wstring base64_chars = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");


static inline bool is_base64(nglChar c)
{
  return (isalnum(c) || (c == _T('+')) || (c == _T('/')));
}

void nglString::EncodeBase64(const uint8* bytes_to_encode, unsigned int in_len)
{
  Wipe();
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];
  
  int chars_in_line = 0;
  
  while (in_len--)
  {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3)
    {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      
      for(i = 0; (i <4) ; i++)
        Add((nglChar)base64_chars[char_array_4[i]]);    
      i = 0;
      
      chars_in_line++;
      if (chars_in_line > 60)
      {
        Add(_T("\r\n"));
        chars_in_line = 0;
      }
    }
  }
  
  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';
    
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    
    for (j = 0; (j < i + 1); j++)
    {
      Add((nglChar)base64_chars[char_array_4[j]]);

      chars_in_line++;
      if (chars_in_line > 60)
      {
        Add(_T("\r\n"));
        chars_in_line = 0;
      }
    }
    
    while((i++ < 3))
    {
      chars_in_line++;
      if (chars_in_line > 60)
      {
        Add(_T("\r\n"));
        chars_in_line = 0;
      }

      Add((nglChar)'=');
    }
    
  }

}

void nglString::DecodeBase64(std::vector<uint8>& rDecoded) const
{
  rDecoded.clear();
  int32 in_len = GetLength();
  rDecoded.reserve(in_len);
  int32 i = 0;
  int32 j = 0;
  int32 in_ = 0;
  nglChar char_array_4[4];
  nglChar char_array_3[3];
  
  while (in_len-- && ( GetChar(in_) != _T('=')) && is_base64(GetChar(in_)))
  {
    char_array_4[i++] = GetChar(in_);
    in_++;
    
    if (i == 4) 
    {
      for (i = 0; i < 4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);
      
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      
      for (i = 0; (i < 3); i++)
        rDecoded.push_back((uint8)char_array_3[i]);
      i = 0;
    }
  }
  
  if (i)
  {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;
    
    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);
    
    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
    
    for (j = 0; (j < i - 1); j++)
      rDecoded.push_back((uint8)char_array_3[j]);
  }
}

/* End of base64 code by RenŽ Nyffenegger */ 



#if defined _CARBON_ || defined _UIKIT_ || defined _COCOA_
CFStringRef nglString::ToCFString() const
{
  char* pStr = Export();
  CFStringRef cfStr = CFStringCreateWithCString(kCFAllocatorDefault, pStr, kCFStringEncodingUTF8);
  free(pStr);
  return cfStr;
}

nglString::nglString(CFStringRef str)
{
  CFIndex length = CFStringGetLength(str);
  UniChar* buffer = (UniChar*)malloc(length * sizeof(UniChar));
  CFStringGetCharacters(str, CFRangeMake(0, length), buffer);
  Import((char*)buffer, length * sizeof(UniChar), eUCS2);
  free(buffer);
}
#endif


// Appending to
#ifndef _DOXYGEN_
#define _OP_(x) Append(x); return *this;
#endif
const nglString& nglString::operator+=(const nglChar nglChar)       { _OP_(nglChar)    }
const nglString& nglString::operator+=(const nglChar* pSource)   { _OP_(pSource) }
const nglString& nglString::operator+=(const nglString& rSource) { _OP_(rSource) }
#undef _OP_

// Concatenation
#ifndef _DOXYGEN_
#define _OP_(x, y) nglString result(x); result.Append(y); return result;
#endif
nglString operator+(const nglString& rLeft, const nglChar Right)     { _OP_(rLeft, Right)  }
nglString operator+(const nglString& rLeft, const nglChar* pRight)   { _OP_(rLeft, pRight) }
nglString operator+(const nglString& rLeft, const nglString& rRight) { _OP_(rLeft, rRight) }
nglString operator+(const nglChar Left, const nglString& rRight)     { _OP_(Left, rRight)  }
nglString operator+(const nglChar* pLeft, const nglString& rRight)   { _OP_(pLeft, rRight) }
#undef _OP_

// Comparison (equal)
#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) == 0);
#endif
bool operator==(const nglString& rLeft, const nglChar Right)     { _OP_(rLeft, nglString(Right))  }
bool operator==(const nglString& rLeft, const nglChar* pRight)   { _OP_(rLeft, pRight) }
bool operator==(const nglString& rLeft, const nglString& rRight) { _OP_(rLeft, rRight) }
bool operator==(const nglChar Left, const nglString& rRight)     { _OP_(rRight, nglString(Left))  }
bool operator==(const nglChar* pLeft, const nglString& rRight)   { _OP_(rRight, pLeft) }
#undef _OP_

// Comparison (different)
#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) != 0);
#endif
bool operator!=(const nglString& rLeft, const nglChar Right)     { _OP_(rLeft, nglString(Right))  }
bool operator!=(const nglString& rLeft, const nglChar* pRight)   { _OP_(rLeft, pRight) }
bool operator!=(const nglString& rLeft, const nglString& rRight) { _OP_(rLeft, rRight) }
bool operator!=(const nglChar Left, const nglString& rRight)     { _OP_(rRight, nglString(Left))  }
bool operator!=(const nglChar* pLeft, const nglString& rRight)   { _OP_(rRight, pLeft) }
#undef _OP_

// Comparison (less than)
#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) < 0);
#endif
bool operator<(const nglString& rLeft, const nglChar Right)      { _OP_(rLeft, nglString(Right))  }
bool operator<(const nglString& rLeft, const nglChar* pRight)    { _OP_(rLeft, pRight) }
bool operator<(const nglString& rLeft, const nglString& rRight)  { _OP_(rLeft, rRight) }
#undef _OP_

#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) > 0);
#endif
bool operator<(const nglChar Left, const nglString& rRight)      { _OP_(rRight, nglString(Left))  }
bool operator<(const nglChar* pLeft, const nglString& rRight)    { _OP_(rRight, pLeft) }
#undef _OP_

// Comparison (less or equal)
#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) <= 0);
#endif
bool operator<=(const nglString& rLeft, const nglChar Right)     { _OP_(rLeft, nglString(Right))  }
bool operator<=(const nglString& rLeft, const nglChar* pRight)   { _OP_(rLeft, pRight) }
bool operator<=(const nglString& rLeft, const nglString& rRight) { _OP_(rLeft, rRight) }
#undef _OP_

#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) >= 0);
#endif
bool operator<=(const nglChar Left, const nglString& rRight)     { _OP_(rRight, nglString(Left))  }
bool operator<=(const nglChar* pLeft, const nglString& rRight)   { _OP_(rRight, pLeft) }
#undef _OP_

// Comparison (more than)
#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) > 0);
#endif
bool operator>(const nglString& rLeft, const nglChar Right)      { _OP_(rLeft, nglString(Right))  }
bool operator>(const nglString& rLeft, const nglChar* pRight)    { _OP_(rLeft, pRight) }
bool operator>(const nglString& rLeft, const nglString& rRight)  { _OP_(rLeft, rRight) }
#undef _OP_

#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) < 0);
#endif
bool operator>(const nglChar Left, const nglString& rRight)      { _OP_(rRight, nglString(Left))  }
bool operator>(const nglChar* pLeft, const nglString& rRight)    { _OP_(rRight, pLeft) }
#undef _OP_

// Comparison (more or equal)
#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) >= 0);
#endif
bool operator>=(const nglString& rLeft, const nglChar Right)     { _OP_(rLeft, nglString(Right))  }
bool operator>=(const nglString& rLeft, const nglChar* pRight)   { _OP_(rLeft, pRight) }
bool operator>=(const nglString& rLeft, const nglString& rRight) { _OP_(rLeft, rRight) }
#undef _OP_

#ifndef _DOXYGEN_
#define _OP_(x, y) return (x.Compare(y) <= 0);
#endif
bool operator>=(const nglChar Left, const nglString& rRight)     { _OP_(rRight, nglString(Left))  }
bool operator>=(const nglChar* pLeft, const nglString& rRight)   { _OP_(rRight, pLeft) }
#undef _OP_

nglChar nglString::operator[](uint32 Index) const
{
//#ifdef _DEBUG_
//	if ((Index < 0) || ((int32)Index >= GetLength())) return Zero;
//	return mString[Index];
//#else
//	// Relaxed checking, still buffer overflow safe
//	return (Index >= 0 && (int32)Index < (int32)mString.size()) ? mString[Index] : Zero;
//#endif // _DEBUG_
	return mString[Index];
}

// Array access
nglChar& nglString::operator[](uint32 Index)
{
	return mString[Index];
}

nglChar  nglString::operator[](int32 Index) const
{
	return mString[Index];
}

nglChar& nglString::operator[](int32 Index)
{
	return mString[Index];
}

bool nglCompareStrings(const nglString& rLeft, const nglString& rRight)
{
  return rLeft.Compare(rRight, true) < 0;
}

bool nglCompareStringsNoCase(const nglString& rLeft, const nglString& rRight)
{
  return rLeft.Compare(rRight, false) < 0;
}

bool nglCompareNaturalStrings(const nglString& rLeft, const nglString& rRight)
{
  return rLeft.CompareNatural(rRight, true) < 0;
}

bool nglCompareNaturalStringsNoCase(const nglString& rLeft, const nglString& rRight)
{
  return rLeft.CompareNatural(rRight, false) < 0;
}


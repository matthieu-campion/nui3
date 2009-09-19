/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglEndian.h
\brief Endianness definitions and macros
*/

#ifndef __nglEndian_h__
#define __nglEndian_h__

//#include "nui.h"


/*
 * Host endianness
 */

#ifdef _LINUX_
  #include <endian.h>
  #include <byteswap.h>
#else // _WIN32_ || _FREEBSD_ || _CYGWIN_
  #define __NEED_BYTESWAP_CODE

  #ifndef LITTLE_ENDIAN
    #define LITTLE_ENDIAN 1234
  #endif

  #ifndef BIG_ENDIAN
    #define BIG_ENDIAN    4321
  #endif

  #ifndef PDP_ENDIAN
    #define PDP_ENDIAN    3412
  #endif

  #ifndef BYTE_ORDER
    #if defined(_M_IX86) || defined(__i386) || (defined _M_X64) || (defined __x86_64__)
      #define BYTE_ORDER LITTLE_ENDIAN
    #else
      // FreeBSD, Darwin (non-x86 flavour)
      #define BYTE_ORDER BIG_ENDIAN
    #endif // _M_IX86 or __i386
  #endif
#endif // _LINUX_


/*
 * Endianness types
 */

enum nglEndian
{
  eEndianLittle,
  eEndianBig,
  eEndianPDP,
// Aliases
  eEndianNet      = eEndianBig,
  eEndianIntel    = eEndianLittle,
  eEndianMotorola = eEndianBig,
  eEndianPPC      = eEndianBig,
#if (BYTE_ORDER == LITTLE_ENDIAN)
  eEndianNative   = eEndianLittle
#elif (BYTE_ORDER == BIG_ENDIAN)
  eEndianNative   = eEndianBig
#elif (BYTE_ORDER == PDP_ENDIAN)
  eEndianNative   = eEndianPDP
#endif
};


/*
 * Byte swap
 */

#ifdef __NEED_BYTESWAP_CODE
static inline uint16 bswap_16(uint16 x)
{
  return (((x & 0x00ff) <<  8) |
          ((x & 0xff00) >>  8));
}

static inline uint32 bswap_32(uint32 x)
{
  return (((x & 0x000000ff) << 24) |
          ((x & 0x0000ff00) <<  8) |
          ((x & 0x00ff0000) >>  8) |
          ((x & 0xff000000) >> 24));
}

static inline uint64 bswap_64(uint64 x)
{
#ifdef _WIN32_
  return (((x & 0x00000000000000ff) << 56) |
          ((x & 0x000000000000ff00) << 40) |
          ((x & 0x0000000000ff0000) << 24) |
          ((x & 0x00000000ff000000) <<  8) |
          ((x & 0x000000ff00000000) >>  8) |
          ((x & 0x0000ff0000000000) >> 24) |
          ((x & 0x00ff000000000000) >> 40) |
          ((x & 0xff00000000000000) >> 56));
#else
  return (((x & 0x00000000000000ffLL) << 56) |
          ((x & 0x000000000000ff00LL) << 40) |
          ((x & 0x0000000000ff0000LL) << 24) |
          ((x & 0x00000000ff000000LL) <<  8) |
          ((x & 0x000000ff00000000LL) >>  8) |
          ((x & 0x0000ff0000000000LL) >> 24) |
          ((x & 0x00ff000000000000LL) >> 40) |
          ((x & 0xff00000000000000LL) >> 56));
#endif // _WIN32_
}
#endif // __NEED_BYTESWAP_CODE


/*
 * String swap
 */

static inline uint16* bswap_16_s(uint16* p, nglSize size)
{
  while (size-- > 0)
  {
    *p = bswap_16(*p);
    p++;
  }
  return p;
}

static inline uint32* bswap_32_s(uint32* p, nglSize size)
{
  while (size-- > 0)
  {
    *p = bswap_32(*p);
    p++;
  }
  return p;
}

static inline uint64* bswap_64_s(uint64* p, nglSize size)
{
  while (size-- > 0) 
  {
    *p = bswap_64(*p);
    p++;
  }
  return p;
}


/*
 * User-defined -> Host
 */

#if (BYTE_ORDER == LITTLE_ENDIAN)
  #define le16_to_cpu(x) (x)
  #define le32_to_cpu(x) (x)
  #define le64_to_cpu(x) (x)
  #define be16_to_cpu(x) bswap_16(x)
  #define be32_to_cpu(x) bswap_32(x)
  #define be64_to_cpu(x) bswap_64(x)

  #define le16_to_cpu_s(p,size) (p)
  #define le32_to_cpu_s(p,size) (p)
  #define le64_to_cpu_s(p,size) (p)
  #define be16_to_cpu_s(p,size) bswap_16_s(p,size)
  #define be32_to_cpu_s(p,size) bswap_32_s(p,size)
  #define be64_to_cpu_s(p,size) bswap_64_s(p,size)
#endif // (BYTE_ORDER == LITTLE_ENDIAN)

#if (BYTE_ORDER == BIG_ENDIAN)
  #define le16_to_cpu(x) bswap_16(x)
  #define le32_to_cpu(x) bswap_32(x)
  #define le64_to_cpu(x) bswap_64(x)
  #define be16_to_cpu(x) (x)
  #define be32_to_cpu(x) (x)
  #define be64_to_cpu(x) (x)

  #define le16_to_cpu_s(p,size) bswap_16_s(p,size)
  #define le32_to_cpu_s(p,size) bswap_32_s(p,size)
  #define le64_to_cpu_s(p,size) bswap_64_s(p,size)
  #define be16_to_cpu_s(p,size) (p)
  #define be32_to_cpu_s(p,size) (p)
  #define be64_to_cpu_s(p,size) (p)
#endif // (BYTE_ORDER == BIG_ENDIAN)

#if (BYTE_ORDER == PDP_ENDIAN)
#error PDP byte ordering macros not implemented
#endif // (BYTE_ORDER == PDP_ENDIAN)


/*
 * Host -> User-defined
 */

#define cpu_to_le16(x) le16_to_cpu(x)
#define cpu_to_le32(x) le32_to_cpu(x)
#define cpu_to_le64(x) le64_to_cpu(x)
#define cpu_to_be16(x) be16_to_cpu(x)
#define cpu_to_be32(x) be32_to_cpu(x)
#define cpu_to_be64(x) be64_to_cpu(x)

#define cpu_to_le16_s(p,size) le16_to_cpu_s(p,size)
#define cpu_to_le32_s(p,size) le32_to_cpu_s(p,size)
#define cpu_to_le64_s(p,size) le64_to_cpu_s(p,size)
#define cpu_to_be16_s(p,size) be16_to_cpu_s(p,size)
#define cpu_to_be32_s(p,size) be32_to_cpu_s(p,size)
#define cpu_to_be64_s(p,size) be64_to_cpu_s(p,size)

#endif // __nglEndian_h__

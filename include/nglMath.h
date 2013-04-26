/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglMath.h
\brief Mathematical constants, functions and helpers
*/

#ifndef __nglMath_h__
#define __nglMath_h__

#include <cmath>
#include "nglEndian.h"


#if defined(_WIN32_) || defined(__MWERKS__)
  // These defines are not present in <math.h> with VC6 sp3
  #define M_E            2.7182818284590452354   /* e */
  #define M_LOG2E        1.4426950408889634074   /* log_2 e */
  #define M_LOG10E       0.43429448190325182765  /* log_10 e */
  #define M_LN2          0.69314718055994530942  /* log_e 2 */
  #define M_LN10         2.30258509299404568402  /* log_e 10 */
  #define M_PI           3.14159265358979323846  /* pi */
  #define M_PI_2         1.57079632679489661923  /* pi/2 */
  #define M_PI_4         0.78539816339744830962  /* pi/4 */
  #define M_1_PI         0.31830988618379067154  /* 1/pi */
  #define M_2_PI         0.63661977236758134308  /* 2/pi */
  #define M_2_SQRTPI     1.12837916709551257390  /* 2/sqrt(pi) */
  #define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
  #define M_SQRT1_2      0.70710678118654752440  /* 1/sqrt(2) */
#endif // _WIN32 || __MWERKS__

#if (defined WIN32) || (defined _ANDROID_)
inline double log2(double x)
{
  return log(x)/0.30102999566398119521373889472449; // log2(x) = log10(x)/log10(2)
}

inline float log2f(float x)
{
  return logf(x)/0.30102999566398119521373889472449f; // log2(x) = log10(x)/log10(2)
}
#endif

#define DEG2RAD(x) ((x) * M_PI / 180.0f)
#define RAD2DEG(x) ((x) * 180.0f / M_PI)

#ifdef _MSC_VER
#define copysign   _copysign
#define chgsign    _chgsign
#define scalb      _scalb
#define logb       _logb
#define nextafter  _nextafter
#define finite     _finite
#define isnan      _isnan
#define fpclass    _fpclass
#pragma warning(push)
#pragma warning(disable: 4307)
#endif

#ifdef _UIKIT_ /// TMP FIX: not sure needed ...
#define finite(x) __inline_isfinited((double)(x))
#endif


uint NGL_API Log2Ceil(uint Value);
/*!< Retrieve the binary logarithm of \a Value (by excess)
  \return the smallest x such as \a Value <= 2^x

\code
Log2Ceil(30) : 5
Log2Ceil(40) : 6
Log2Ceil(64) : 6
Log2Ceil(65) : 7
\endcode
*/

uint NGL_API Log2Floor(uint Value);
/*!< Retrieve the binary logarithm of \a Value (by default)
  \return the smallest x such as \a 2^x <= Value < 2^(x+1)

\code
Log2Floor(30) : 4
Log2Floor(40) : 5
Log2Floor(64) : 6
Log2Floor(65) : 6
\endcode
*/


/* ISOC99 round() functions emulation when not supported
 */
#if defined(_WIN32_) || defined(_FREEBSD_)
  inline double round(double x)
  {
    double r = ceil(x);
    return r - x <= 0.5 ? r : r - 1.0;
  }

  inline float roundf(float x)
  {
    float r = (float)ceil((float)x);
    return r - x <= 0.5f ? r : r - 1.0f;
  }
#endif // _WIN32_ || _FREEBSD_


/* Float to Ints Conversion helpers
 */

#define USE_FAST_FLOAT_CONVERSION 0
//#FIXME: Always set fast float to 0: does not work in special cases on X86
//needs to be fixed
#if !(defined USE_FAST_FLOAT_CONVERSION)
  #if (defined _NGL_X86_) 
    #define USE_FAST_FLOAT_CONVERSION 1
  #else
    #define USE_FAST_FLOAT_CONVERSION 0
  #endif
#endif


#if (BYTE_ORDER == BIG_ENDIAN)
  #define __iexp 0
  #define __iman 1
#else
  #define __iexp 1
  #define __iman 0
#endif

#ifdef _NGL_PPC_
#include <ppc_intrinsics.h>
#endif // _NGL_PPC_

const double _nui_doublemagic         = double(6755399441055744.0);       //2^52 * 1.5,  uses limited precisicion to floor
const double _nui_doublemagicdelta    = (1.5e-8);                         //almost .5f = .5f + 1e^(number of exp bit)
const double _nui_doublemagicroundeps	= (.5f-_nui_doublemagicdelta);          //almost .5f = .5f - 1e^(number of exp bit)

union _nui_double
{
  double d;
  int32 i[2];
};

// fast versions:
inline int32 FastToNearest(double x) ///< Cast x to an int using "to zero" rounding mode (Normal Ansi C behaviour).
{
#ifdef __GNUC__
  _nui_double& rX(reinterpret_cast<_nui_double&>(x));
  rX.d	= rX.d + _nui_doublemagic;
	return rX.i[__iman];
#endif
#ifdef _MSC_VER
  x = x + _nui_doublemagic;
  return ((int32*)&x)[__iman];
#endif
}

inline int32 FastToNearest(float x) ///< Cast x to an int using "to zero" rounding mode (Normal Ansi C behaviour).
{
  return FastToNearest((double)x);
}

inline int32 FastToZero(double x)  ///< Cast x to an int using "to lower" rounding mode.
{
  return ((x < 0.0) ? FastToNearest(x + _nui_doublemagicroundeps) : FastToNearest(x - _nui_doublemagicroundeps));
}

inline int32 FastToZero(float x)  ///< Cast x to an int using "to lower" rounding mode.
{
  return FastToZero((double)x);
}

inline int32 FastToBelow(double x) ///< Cast x to an int using "to nearest" rounding mode.
{
  return FastToNearest(x - _nui_doublemagicroundeps);
}

inline int32 FastToBelow(float x) ///< Cast x to an int using "to nearest" rounding mode.
{
  return FastToBelow((double)x);
}

inline int32 FastToAbove(double x) ///< Cast x to an int using "to greater" rounding mode.
{
  return FastToNearest(x + _nui_doublemagicroundeps);
}

inline int32 FastToAbove(float x) ///< Cast x to an int using "to greater" rounding mode.
{
  return FastToAbove((double)x);
}

// Accurate versions:
#if USE_FAST_FLOAT_CONVERSION
inline int32 ToNearest(double x) ///< Cast x to an int using "to zero" rounding mode (Normal Ansi C behaviour).
{
  return FastToNearest(x);
}

inline int32 ToNearest(float x) ///< Cast x to an int using "to zero" rounding mode (Normal Ansi C behaviour).
{
  return FastToNearest((double)x);
}

inline int32 ToZero(double x)  ///< Cast x to an int using "to lower" rounding mode.
{
  return FastToZero(x);
}

inline int32 ToZero(float x)  ///< Cast x to an int using "to lower" rounding mode.
{
  return FastToZero(x);
}

inline int32 ToBelow(double x) ///< Cast x to an int using "to nearest" rounding mode.
{
  return FastToBelow(x);
}

inline int32 ToBelow(float x) ///< Cast x to an int using "to nearest" rounding mode.
{
  return FastToBelow(x);
}

inline int32 ToAbove(double x) ///< Cast x to an int using "to greater" rounding mode.
{
  return FastToAbove(x);
}

inline int32 ToAbove(float x) ///< Cast x to an int using "to greater" rounding mode.
{
  return FastToAbove(x);
}
#else
inline int32 ToNearest(double x) ///< Cast x to an int using "to zero" rounding mode (Normal Ansi C behaviour).
{
  return (int32)round(x);
}

inline int32 ToNearest(float x) ///< Cast x to an int using "to zero" rounding mode (Normal Ansi C behaviour).
{
  return (int32)round((double)x);
}

inline int32 ToZero(double x)  ///< Cast x to an int using "to lower" rounding mode.
{
  return (int32)x;
}

inline int32 ToZero(float x)  ///< Cast x to an int using "to lower" rounding mode.
{
  return (int32)x;
}

inline int32 ToBelow(double x) ///< Cast x to an int using "to nearest" rounding mode.
{
  return (int32)floor(x);
}

inline int32 ToBelow(float x) ///< Cast x to an int using "to nearest" rounding mode.
{
  return (int32)floor(x);
}

inline int32 ToAbove(double x) ///< Cast x to an int using "to greater" rounding mode.
{
  return (int32)ceil(x);
}

inline int32 ToAbove(float x) ///< Cast x to an int using "to greater" rounding mode.
{
  return (int32)ceil(x);
}
#endif


// Denormals:
inline void Undenormalise(float& sample) ///< if sample is stored as denormal float it will be set to 0
{
#ifdef _NGL_X86_
  if(((*(uint*)&sample)&0x7f800000)==0) 
    sample=0.0f;
#else
  // To nothing!
#endif
}

inline bool IsDenormal(float& sample) ///< if sample is stored as denormal float it will be set to 0
{
  return (((*(uint*)&sample)&0x7f800000)==0);
}

inline void Undenormalise(double& sample) ///< if sample is stored as denormal double it will be set to 0
{
  if (((((int *)&sample)[__iexp])&0x7fe00000) != 0)
    sample = 0.0;
}

inline bool IsDenormal(double& sample) ///< if sample is stored as denormal double it will be set to 0
{
  return (((((int *)&sample)[__iexp])&0x7fe00000) != 0);
}

// Sign operations:
inline float nuiAbs(float f) 
{
  int32 i = ((*(int32*)&f) & 0x7fffffff);
  return (*(float*)&i);
}

inline float nuiNeg(float f) 
{
  int32 i = ((*(int32*)&f) ^ 0x80000000);
  return (*(float*)&i);
}

inline int32 nuiSign(float f) 
{
  return 1 + (((*(int32*)&f) >> 31) << 1);
}

inline double nuiAbs(double f) 
{
  uint64 i = ((*(uint64*)&f) & (((uint64)1 << 63)-1));
  return (*(double*)&i);
}

inline double nuiNeg(double f) 
{
  int64 i = ((*(int64*)&f) ^ ((int64)1 << 63));
  return (*(double*)&i);
}

inline int64 nuiSign(double f) 
{
  return 1 + (((*(int64*)&f) >> 63) << 1);
}

template <class T>
inline T nuiClamp(T value, T Low, T High)
{
  if (value <= Low)
    return Low;
  if (value >= High)
    return High;
  return value;
}

#undef __iexp
#undef __iman


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // __nglMath_h__

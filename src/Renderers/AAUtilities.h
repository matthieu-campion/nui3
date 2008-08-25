/*
*  AAUtilities.h
*  glAArg version 0.2.2
*
*  Created by arekkusu on Fri Mar 26 2004.
*  Copyright (c) 2004 arekkusu. All rights reserved.
*
*/

#ifndef __AAUTILITIES__
#define __AAUTILITIES__

// useful macros
#ifndef MIN
#define MIN(A,B)  ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __a : __b; })
#endif
#ifndef MAX
#define MAX(A,B)  ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __b : __a; })
#endif
#ifndef CLAMP
#define CLAMP(X,lo,hi)  ({ __typeof__(X) __x = (X); __typeof__(lo) __l = (lo); __typeof__(hi) __h = (hi);\
  ((__x) > (__h)) ? (__h) : (((__x) < (__l)) ? (__l) : (__x));  })
#endif

#if !defined(_WIN32)      // for matching cross-platform gl.h typecasts
#ifndef APIENTRY
#define APIENTRY
#endif
#endif

#include <stdio.h>        // for printf debugging
#include <math.h>        // for log exp sqrt etc, and maybe sqrtf
#include <limits.h>        // for INT_MAX
#include <float.h>        // for FLT_MAX
#if defined(_NGL_PPC_)
#include <ppc_intrinsics.h>
// ---------------------------------------------------------------------
// fast reciprocal square root estimate with Newton-Raphson refinement
// ---------------------------------------------------------------------
static inline float frsqrtes_nr(float x) {
  float e = __frsqrtes(x);
  e *= (1.5f - (0.5f * x * e * e));
  return e;
}
#else
// ---------------------------------------------------------------------
// reciprocal square root at full 32 bit precision
// ---------------------------------------------------------------------
static inline float frsqrtes_nr(float x) {
  return 1.0f/sqrtf(x);
}
#endif


// ---------------------------------------------------------------------
// linear interpolation between two RGBA colors. t [0..1]
// ---------------------------------------------------------------------
static inline unsigned int lerpRGBA(const unsigned int d, const unsigned int s, const float t) {
  unsigned int ti = ToBelow(t*256);
  unsigned int dstga = d      & 0xFF00FF;
  unsigned int dstrb = d >> 8 & 0xFF00FF;
  unsigned int srcga = s      & 0xFF00FF;
  unsigned int srcrb = s >> 8 & 0xFF00FF;
  unsigned int dga = srcga - dstga;
  unsigned int drb = srcrb - dstrb;
  dga = (dga * ti) >> 8;  
  drb = (drb * ti) >> 8;  
  const unsigned int ga  = (dga + dstga)      & 0x00FF00FF;
  const unsigned int rb  = (drb + dstrb) << 8 & 0xFF00FF00;
  return ga | rb;
}


// ---------------------------------------------------------------------
// component scale and saturate RGB color. s [0..512]
// ---------------------------------------------------------------------
static inline unsigned int scalesatRGB(const unsigned int c, const unsigned short s) {
  unsigned int r =  ((c >> 24)               * s) >> 8;
  unsigned int g = (((c >> 16) & 0x000000FF) * s) >> 8;
  unsigned int b = (((c >>  8) & 0x000000FF) * s) >> 8;
  unsigned int a =   c         & 0x000000FF;
  r = MIN(r, (unsigned int)255);
  g = MIN(g, (unsigned int)255);
  b = MIN(b, (unsigned int)255);
  unsigned int end = (r<<24) | (g<<16) | (b<<8) | a;
  return end;
}


#ifndef glError
#ifdef DEBUG
#define glError() { \
  GLenum err = glGetError(); \
  if (GL_NO_ERROR != err) NGL_OUT(_T("glError: %s caught at %s:%u\n"), (char *)gluErrorString(err), __FILE__, __LINE__); \
}
#else
#define glError()
#endif
#endif

#ifdef  _MSC_VER
inline double log2(double x)
{
  return log(x)/0.30102999566398119521373889472449; // log2(x) = log10(x)/log10(2)
}

inline float log2f(float x)
{
  return logf(x)/0.30102999566398119521373889472449f; // log2(x) = log10(x)/log10(2)
}
#endif


void nui_glBegin(GLenum mode);
void nui_glEnd();
void nui_glColor4ubv(const GLubyte *v);
void nui_glTexCoord2f(GLfloat s, GLfloat t);
void nui_glVertex2f(GLfloat s, GLfloat t);

#endif //__AAUTILITIES__

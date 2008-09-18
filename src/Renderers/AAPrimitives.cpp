/*
*  AAPrimitives.c
*  glAArg version 0.2.2
*
*  Created by arekkusu on Thu Nov 20 2003.
*  Copyright (c) 2004 arekkusu. All rights reserved.
*  http://homepage.mac.com/arekkusu/bugs/invariance/
*
*  Version history:
*  0.2.2 Mon Jul 19 2004
*    * Point optimization for immediate mode (benchmarks ~30% faster)
*  0.2.1 Wed Jul 14 2004
*    * Reduce texture footprint and mipmap generation time by 75%. (Texture matrix scale must match.)
*    * Use APPLE_client_storage only if the extension is supported (i.e. on Mac)
*  0.2   Tue Jul 13 2004
*    * Initial pass at cross-platform compilation support for Mac OS X, Linux, and Windows (all using gcc 3.3)
*      * Changed glAAGenerateAATex 'alias' parameter to float, for mipmap filter control.
*  0.1.5 Tue Jun 29 2004
*    * Workaround for Radeon 7000 mipmap hardware bug. Thanks to ChrisB and PhilC at ATI for tracking it down.
*    * Fix for reduced frsqrte precision on G5.
*  0.1.4 Fri Apr 16 2004
*    * Add glError() debug macro and hide spurious error after fence finishing.
*  0.1.3 Thu Apr  7 2004
*    * Fix point light defaults. DTS submission for Radeon 7000 mipmap selection problem.
*  0.1.2 Thu Apr  6 2004
*    * VAR is now off by default to match regular immediate mode GL. Client must enable it (and handle flushes.)
*    * Fixed header includes and extern "C" problems.
*  0.1.1 Wed Mar 31 2004
*      * Changed glAAPointLight API to use [-1, 1] coordinates clamped inside a unit circle.
*      * Fixed miplevel brightness for negative falloff values.
*    * Optimized VAR usage pattern to reduce fence finishing.
*  0.1   Thu Mar 25 2004
*    * Initial public release for iDevGames Vectorized contest.
*    * Basic points and lines only.
*
*  To fix:
*    buggy line stipple wrap
*  To implement:
*    proper multi-ctx support
*    specifiable size for each texture
*    line end cap modes
*    triangles, tristrips, trifans
*    quads, quad strips
*    incoming texcoord fixup (requires multitexturing)
*    3D and transformed 2D projections in screen space
*    optimize for G3/G4/G5 (altivec, real sqrtf)
*    optimize for individual GPUs (CVA, ATI rev4, rage128)
*  To test:
*    verify the cases "fastest" hardware mipmap generation fails on some GPUs, to detect it introspectively
*
*  NOTE: this code requires OpenGL 1.1. It conditionally uses extensions such as GL_GENERATE_MIPMAP_SGIS when possible.
*    It should run on any Mac OS X system, however 10.2.5 or newer is reccomended for driver stability.
*    It has been compiled and tested on Slackware Linux 10 with gcc 3.3 and the software Mesa renderer,
*    and on Win2k with MINGW gcc 3.3 and MINGWGLUT32 and various renderers.
*
*  NOTE: compile this with optimizations. These gcc flags are good for OS X:
*    -O3 -mcpu=G3 -mtune=G4 -force_cpusubtype_ALL -ffast-math -mdynamic-no-pic --param max-gcse-passes=3
*    and these appear adequate for x86:
*    -O3 -mcpu=pentium -Wno-unknown-pragmas
*/


#include "nui.h"

#ifndef __NUI_NO_AA__

#include <stdlib.h>        // for malloc
#include <string.h>        // for strstr, because gluCheckExtension isn't available in libglu32 on windows

#if defined(__APPLE__)     // for ntohl byteswapping, should be a no-op on PPC and __bswap on x86.
  #include <arpa/inet.h>
#else
  #if defined(_WIN32)
    #include <winsock.h>
  #else
    #include <netinet/in.h>
  #endif
#endif

#include "nglMath.h"
#include "AAPrimitives.h"

#ifndef CLAMP
#define CLAMP(X,lo,hi)  ({ __typeof__(X) __x = (X); __typeof__(lo) __l = (lo); __typeof__(hi) __h = (hi);\
((__x) > (__h)) ? (__h) : (((__x) < (__l)) ? (__l) : (__x));  })
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
static inline float frsqrtes_nr(float x)
{
  float e = __frsqrtes(x);
  e *= (1.5f - (0.5f * x * e * e));
  return e;
}
#else
// ---------------------------------------------------------------------
// reciprocal square root at full 32 bit precision
// ---------------------------------------------------------------------
static inline float frsqrtes_nr(float x)
{
  return 1.0f/sqrtf(x);
}
#endif


// ---------------------------------------------------------------------
// linear interpolation between two RGBA colors. t [0..1]
// ---------------------------------------------------------------------
static inline unsigned int lerpRGBA(const unsigned int d, const unsigned int s, const float t)
{
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

// basic 2D types
typedef struct GLCoord2
{
  GLfloat  x;
  GLfloat  y;
} GLCoord2;

typedef struct Vertex
{
  GLuint  rgba;      // usage of this int assumes big-endian order. it is byteswapped on x86 before submission to GL.
  GLfloat  x;
  GLfloat  y;
  GLfloat  tx;
  GLfloat  ty;
} Vertex;   


// some convienence constants for the texture dimensions:
// point size, size minus one, half size squared, double size, point center, radius squared
#define psz (phf * 2)
#define psm (psz - 1)
#define phs (phf * phf)
#define pdb (psz * 2)
#define pct (psz + phf)
#define prs ((phf-1)*(phf-1))

// the original Radeon and the Radeon 7000 have a hardware bug (#3633400):
// adjacent vertices with identical texture coordinates break the mipmap selection policy.
// the workaround is to nudge some of the texture coords by a tiny amount.
#define GLAARG_RADEON7000_KLUDGE 0.0001f

// VAR convienence constants based on the hardcoded values in the header.
#define vsize (sizeof(Vertex) * VAR_size)
#define line_i (ToBelow((VAR_size*1.5)))


// exposed globals for client app
GLuint          glAA_texture;      // the AA sphere texIDs
unsigned char      *glAA_AAtex;        // AA texture buffer

// file-scope globals
static GLenum      glAA_mode, glAA_old_mode;   // state globals
static Vertex      glAA_v[4];          // temp stack space for three points (quad is biggest primitive)
static int        glAA_vi, glAA_vp, glAA_vcl; // vertex stack index, vertex previous index, vertex color last set
static int        glAA_vc[4];          // vertex color stack usage bools
static float      glAA_alpha_fix;        // for points/lines < 1.0 px wide or long

static float      glAA_point_size;      // point globals
static float      glAA_point_border;
static float      glAA_point_maxr;
static unsigned short  glAA_plight[4];

static float      glAA_line_width;      // line globals
static float      glAA_line_border;
static float      glAA_line_maxr;
static float      glAA_stipple[16], glAA_stipple_phase;
static int        glAA_stipple_count, glAA_stipple_idx;

static Vertex      *glAA_VAR;          // VAR globals
static unsigned short   *glAA_line_index;
static int        glAA_VAR_i, glAA_VAR_skip, glAA_fence;

static float      logtbl[pdb];        // for sphere lookups

// private helper functions

// -------------------------------------------------
// set the context for macros
// -------------------------------------------------
static inline void glAASetContext() 
{
}

// glAA APIs

// glAAInit/Exit should be called once at program begin/end to set up globals.
// NOTE: this version is NOT multiple-context clean. The client app is responsible for
// setting up GL ctx and state (PixelStore, texture matrix etc)
// THERE MUST BE A VALID CONTEXT CREATED BEFORE CALLING THIS (to set up VAR state etc)
//
void glAAInit() 
{
  int i;
  for (i=1; i<=pdb; i++)
  {
    // lookup table has 4x the bitmap resolution
    logtbl[i-1] = log((float)i/pdb);
  }

  glAA_texture = 0; 

  glAASetContext();

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &glAA_VAR[0].rgba);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &glAA_VAR[0].tx);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &glAA_VAR[0].x);

  // setup defaults
  glAA_old_mode = (GLenum)-1;
  glAALineWidth(1.0f);
  glAAColor1ui(0xFFFFFFFF);
}


void glAAExit() 
{
  int i;
  glAASetContext();

  // free texture data
  if (glAA_texture) 
  { 
    glDeleteTextures(1, &glAA_texture); 
    glAA_texture = 0; 
  }

  if (glAA_AAtex)
  { 
    free(glAA_AAtex);
    glAA_AAtex = 0; 
  }
}


// glAA texture functions.
//
// The concept here is simple but the implementation is a bit tricky. What we want is an AA circle for a point texture
// (lines/tris get strips of the circle stretched over them.) To work with fractional positioning, there must be
// a 1 px empty border around the texture to filter with. Unfortunately, hardware border support isn't reliable.
// Mipmapping further complicates the issue. There are some GL extensions that could make this easier but
// I use a slightly wasteful wraparound layout here that works on ALL renderers:
//
// BBb    B = empty blackness
// BOb    O = the AA circle
// bbb    b = virtual (wraparound) empty blackness
//
// This layout will retain a 1 px border at every mip level on all hardware.
// For a 32 x 32 circle, this means a  64x 64 alpha mip pyramid, or ~  5k of VRAM.
// For a 256x256 circle, this means a 512x512 alpha mip pyramid, or ~341k of VRAM.
//
inline float ifun(float x, float y, float F) 
{
  // compute falloff at x,y with exponent F [-1..1]
  float S = (x*x + y);
  float L;
  if (S) 
  {
    // estimate sqrt, accurate to about 1/4096
    L = frsqrtes_nr(S);
    L = L * S;
  }
  else 
    L = 0.0f;
  if (F == 0.0f) // intensity: [-1..0..1] = bloom..normal..fuzzy
    S = 0.0f;
  else if (F > 0.0f)
    // this is just pow(L/phf, F) but with a lookup table
    S = exp(logtbl[ToBelow(L*4.0f)]/F);
  else
    // this is a bit more complex-- logarithm ramp up to a solid dot with slight S blend at cusp
    S = L<(1.0f+F)*phf ? 0.0f : exp((1.0f+F*0.9f)/(L/phf-1.0f-F) * logtbl[ToBelow(L*4.0f)]);
  return 1.0f-S;
}


// 8-way symmetric macro
#define putpixel(x, y, I) {             \
  unsigned char c = I*255;           \
  texture[(pct   + x) + (pct   + y)*pdb] =   \
  texture[(pct-1 - x) + (pct   + y)*pdb] =   \
  texture[(pct   + x) + (pct-1 - y)*pdb] =   \
  texture[(pct-1 - x) + (pct-1 - y)*pdb] =   \
  texture[(pct   + y) + (pct   + x)*pdb] =   \
  texture[(pct-1 - y) + (pct   + x)*pdb] =   \
  texture[(pct   + y) + (pct-1 - x)*pdb] =   \
  texture[(pct-1 - y) + (pct-1 - x)*pdb] = c;\
}


void glAAGenerateAATex(float F, float alias) 
{      
  // compute the AA sphere
  {
    uint32 l2phf = ToBelow(log2(phf));
    uint32 l2psz = ToBelow(log2(psz));

    if (!glAA_AAtex) 
      glAA_AAtex = (unsigned char*)calloc(1, pdb*pdb);
    unsigned char *texture = glAA_AAtex;
    if (texture) 
    {
      int x = phf-1;
      int y = 0;
      int ax;
      float T = 0.0f;
      float ys = 0.0f;
      while (x > y) 
      {
        ys = (float)(y*y);
        float L = sqrt(prs - ys);
        float D = ceilf(L)-L;
        if (D < T) x--;
        for(ax=y; ax<x; ax++)
          putpixel(ax, y, (unsigned char)ifun((float)ax, ys, F));            // fill wedge
        putpixel(x, y, (unsigned char)((1.0f-D)*(ifun((float)x, ys, F))));          // outer edge
        T = D;
        y++;
      }

      glAASetContext();
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      if (!glAA_texture)
      {
        glAA_texture = -1;
        glGenTextures(1, &glAA_texture);
        glBindTexture(GL_TEXTURE_2D, glAA_texture);
        // Ideally we could use GL_CLAMP_TO_BORDER, but this is not available on GF2MX and GF4MX.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Clamp the max mip level to 2x2 (1 px with 1 px border all around...)
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, l2psz);
        {
          // Generate the entire mip pyramid slowly in software
          gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, pdb, pdb, GL_ALPHA, GL_UNSIGNED_BYTE, texture);
        }
      }
      else 
      {
        glBindTexture(GL_TEXTURE_2D, glAA_texture);
        {
          // Generate the entire mip pyramid slowly in software
          gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, pdb, pdb, GL_ALPHA, GL_UNSIGNED_BYTE, texture);
        }
      }
      // Set filtering depending on our aliasing mode.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (alias>0.66)?GL_NEAREST:GL_LINEAR);  
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (alias>0.33)?((alias>0.66)?GL_NEAREST:GL_LINEAR_MIPMAP_NEAREST):GL_LINEAR_MIPMAP_LINEAR);
      // now fix the upper mip levels so that 1 px points/lines stay bright:
      // at 1 px, the circle texture's default brightness is pi/4 = 0.78. This is geometrically
      // correct, but for usability reasons we want 1 px wide lines to display at full 1.0 brightness.
      // the following is an approximation sampling of the texture to do that, getting values that
      // "look ok" for exponential falloff in either direction.
      static unsigned char mipfix[4];
      unsigned char fix = (texture[ToBelow(psz+phf*0.2f)+pct*pdb]+texture[ToBelow(psz+phf*0.7f)+pct*pdb])>>1;
      mipfix[0]=mipfix[1]=mipfix[2]=mipfix[3]=fix;
      glTexSubImage2D(GL_TEXTURE_2D, l2phf, 2, 2, 2, 2, GL_ALPHA, GL_UNSIGNED_BYTE, mipfix);
      glTexSubImage2D(GL_TEXTURE_2D, l2psz, 1, 1, 1, 1, GL_ALPHA, GL_UNSIGNED_BYTE, mipfix);
    }
  }
  glError();
}


// glAA APIs mirroring OpenGL functionality
//
// The concept here is very simple, just buffer incoming vertex data until the current primitive is full,
// then calculate appropriate textured triangles for the primitive and submit in a group.
//
void glAABegin(GLenum mode) 
{
  glError();
  glAA_vi = glAA_vp = 0;
  if (glAA_old_mode != mode) 
  {
    switch (mode) 
    {
    case GL_POINTS: 
      {
        glAA_alpha_fix = MIN(1.0f, glAA_point_size);
        // we always have 4 vertex alignment
      }
      break;
    case GL_LINE_LOOP:
    case GL_LINE_STRIP:
    case GL_LINES: 
      {
        glAA_alpha_fix = MIN(1.0f, glAA_line_width);
        // force 8 vertex alignment
        glAA_VAR_skip = glAA_VAR_i&4;        
      }
      break;
    }
  }
  glAA_mode = mode;
  if (mode==GL_POINTS)
    nui_glBegin(GL_TRIANGLE_STRIP);
}

static int end_color;
static float end_tx1;
static float end_ty1;
static float end_x1;
static float end_y1;
static float end_tx2;
static float end_ty2;
static float end_x2;
static float end_y2;


void glAAEnd() 
{
  glAA_old_mode = glAA_mode;
  if ((glAA_mode == GL_LINE_STRIP) || (glAA_mode == GL_LINE_LOOP)) 
  {
    if (glAA_mode == GL_LINE_LOOP) 
      glAAVertex2f(glAA_v[3].x, glAA_v[3].y);
    glAA_vc[glAA_vp] = 0;
  }

  if (glAA_line_width > 1.0f && glAA_mode == GL_LINE_STRIP)
  {
    nui_glColor4ubv((const GLubyte *)&end_color);
    nui_glTexCoord2f(end_tx1,  end_ty1);
    nui_glVertex2f(end_x1,  end_y1);
    nui_glTexCoord2f(end_tx2,  end_ty2);
    nui_glVertex2f(end_x2,  end_y2);

    nui_glEnd();
  }

  glAA_vi = glAA_vp = 0;
  // reset stipple index
  glAA_stipple_idx = 0;
  if (glAA_mode == GL_POINTS)
    nui_glEnd();
  glError();
}


void glAALineWidth(float w) 
{
  float width = ((w>0.0f) ? w : 1.0f);

  if (width != glAA_line_width) 
  {
    float r = MAX(1.0f, width);
    // find min/max using float trick (faster than floor/ceil or casts, only works for positive IEEE 32 bit floats)
    float minr = (r+8388608.f)-8388608.f;
    if (r < minr) minr -= 1.0f;
    float maxr = (minr == r) ? minr : minr+1.0f;
    // calculate the tex coords for a 0.5px outset, doing two divides at once
    float den2 = 1.0f/(maxr*minr);
    float ratio = phf*minr*den2;
    glAA_line_border = ratio+(phf*maxr*den2*1.5f-ratio)*(maxr-r);
    glAA_line_maxr = maxr;
    glAA_line_width = width;
  }
  if ((glAA_mode == GL_LINES) || (glAA_mode == GL_LINE_LOOP) || (glAA_mode == GL_LINE_STRIP))  // allow changing before glAAEnd()
    glAA_alpha_fix = MIN(1.0f, width);
}


inline void glAAColor3f(float R, float G, float B)
{
  glAAColor4f(R, G, B, 1.0f);
}


void glAAColor4f(float R, float G, float B, float A)
{
  glAA_vc[glAA_vi] = 1;
  glAA_vcl = glAA_vi;
  glAA_v[glAA_vi].rgba = (ToBelow(R*255.0f)<<24) | (ToBelow(G*255.0f)<<16) | (ToBelow(B*255.0f)<<8) | ToBelow(A*255.0f);
}


void glAAColor4ubv(const GLubyte *c)
{
  if (c) 
  {
    glAA_vc[glAA_vi] = 1;
    glAA_vcl = glAA_vi;
    glAA_v[glAA_vi].rgba = *(GLuint *)c;
  }
}


void glAAColor1ui(GLuint c)
{
  glAA_vc[glAA_vi] = 1;
  glAA_vcl = glAA_vi;
  glAA_v[glAA_vi].rgba = c;
}


// here's where all the real work is done.
void glAAVertex2f(float x, float y) 
{
  switch (glAA_mode) 
  {
  case GL_POINTS: 
    {
      float bord = glAA_point_border;      // shadow globals
      float maxr = glAA_point_maxr;
      float alpha_fix = glAA_alpha_fix;

      float size = maxr+1.0f;
      float centx = x-(maxr+1.0f)*0.5f;
      float centy = y-(maxr+1.0f)*0.5f;

      // adjust vertex alpha if needed
      int c1 = glAA_v[glAA_vcl].rgba;
      if (alpha_fix < 1.0f) 
      {
        int int_alpha_fix = ToBelow(alpha_fix * 255.0f);
        c1 = (c1 & 0xFFFFFF00) | (((c1 & 0x000000FF)*int_alpha_fix)>>8);
      }
      int c2 = c1, c3 = c1, c4 = c1;

      c1 = ntohl(c1);
      c2 = ntohl(c2);
      c3 = ntohl(c3);
      c4 = ntohl(c4);

      nui_glColor4ubv((const GLubyte *)&c1);
      nui_glTexCoord2f(psz-bord, psz-bord);
      nui_glVertex2f(centx,      centy);
      nui_glColor4ubv((const GLubyte *)&c2);
      nui_glTexCoord2f(pdb+bord, psz-bord);
      nui_glVertex2f(centx+size, centy);
      nui_glColor4ubv((const GLubyte *)&c3);
      nui_glTexCoord2f(pdb+bord, pdb+bord);
      nui_glVertex2f(centx+size, centy+size);
      nui_glColor4ubv((const GLubyte *)&c4);
      nui_glTexCoord2f(psz-bord, pdb+bord);
      nui_glVertex2f(centx,      centy+size);
    }
    break;

  case GL_LINE_LOOP:
  case GL_LINE_STRIP:
  case GL_LINES: 
    {
      if (glAA_vi < 1) 
      {
        glAA_v[glAA_vi].x = x; glAA_v[glAA_vi++].y = y;
        if (glAA_mode == GL_LINE_LOOP) 
        { 
          // remember the first point for looping
          glAA_v[3].x = x; glAA_v[3].y = y;
        }
      } 
      else 
      {
        if (glAA_mode != GL_LINES)
        { 
          // remember the new point
          glAA_v[glAA_vi].x = x; glAA_v[glAA_vi].y = y;
        }
        float x1 = glAA_v[glAA_vp].x; // shadow globals
        float y1 = glAA_v[glAA_vp].y;
        float bord = glAA_line_border;
        float maxr = glAA_line_maxr;        
        float alpha_fix = glAA_alpha_fix;

        float perp_y = x1-x;
        float perp_x = y-y1;
        float perpd, leng;

        float factor = perp_y*perp_y+perp_x*perp_x;
        if (factor) 
        {
          perpd = frsqrtes_nr(factor);
          perp_y *= perpd; // normalize to 1px
          perp_x *= perpd;
          leng = 1.0f/perpd; // could do perpd*factor, or __fres(perpd) to get estimate of the
          // real sqrt. But the err is too high on the G5, so we need a divide here.
          alpha_fix *= MIN(1.0f, leng);
          leng = MAX(1.0f, leng);
          perpd= MIN(1.0f, perpd);
          x1 -= perp_y*0.5f;
          y1 += perp_x*0.5f;
        } 
        else 
        {
          perp_y = 0.0f;
          perp_x = 1.0f;
          perpd = leng = 1.0f;
        }

        float width = (maxr+1.0f)*0.5f;
        float unit_x = -perp_y;
        float unit_y = perp_x;
        perp_y *= width;
        perp_x *= width;
        float parl_x = -perp_y;
        float parl_y =  perp_x;

        float s = 0.0f;
        int b = glAA_stipple_idx;
        //    printf("start idx %d   ", b);
        int i = glAA_VAR_i;
        const float lengm1 = leng-1.0f;
        float dash = FLT_MAX, gap = 0;
        while (s <= lengm1) 
        {

          if (s >= leng-dash) 
          {
            dash = MAX(1.0f, leng-s); // clip final dash
          } 
          x = x1 + (dash-1.0f)*unit_x;
          y = y1 + (dash-1.0f)*unit_y;

          // find stipple weights
          float cw2 = MIN(1.0f, (s+0.5f)*perpd);
          float cp2 = MAX(0.0f, (s-width)*perpd);
          float cw4 = MIN(1.0f, (s-0.5f+dash)*perpd);
          float cp4 = MIN(1.0f, (s+dash+width)*perpd);

          // pick vertex colors and adjust alpha if needed
          int c1 = (glAA_vc[glAA_vp]) ? glAA_v[glAA_vp].rgba : glAA_v[glAA_vcl].rgba;
          int c2 = (glAA_vc[glAA_vi]) ? glAA_v[glAA_vi].rgba : glAA_v[glAA_vcl].rgba;
          if (alpha_fix < 1.0f) 
          {
            int int_alpha_fix = ToBelow(alpha_fix * 255.0f);
            c1 = (c1 & 0xFFFFFF00) | (((c1 & 0x000000FF)*int_alpha_fix)>>8);
            c2 = (c2 & 0xFFFFFF00) | (((c2 & 0x000000FF)*int_alpha_fix)>>8);
          }

          int cl1 = ntohl(lerpRGBA(c1, c2, cp2));
          int cl2 = ntohl(lerpRGBA(c1, c2, cw2));
          int cl3 = ntohl(lerpRGBA(c1, c2, cw4));
          int cl4 = ntohl(lerpRGBA(c1, c2, cp4));
          if (glAA_line_width <= 1.0f || glAA_mode != GL_LINE_STRIP)
          {
            nui_glBegin(GL_TRIANGLE_STRIP);
            nui_glColor4ubv((const GLubyte *)&cl1);
            nui_glTexCoord2f(pdb+bord,  psz-bord);
            nui_glVertex2f(x1-perp_x-parl_x, y1-perp_y-parl_y);
            nui_glTexCoord2f(psz-bord,  psz-bord);
            nui_glVertex2f(x1+perp_x-parl_x, y1+perp_y-parl_y);

            nui_glColor4ubv((const GLubyte *)&cl2);
            nui_glTexCoord2f(psz-bord,  pct);
            nui_glVertex2f(x1+perp_x, y1+perp_y);
            nui_glTexCoord2f(pdb+bord,  pct);
            nui_glVertex2f(x1-perp_x, y1-perp_y);

            nui_glColor4ubv((const GLubyte *)&cl3);        
            nui_glTexCoord2f(psz-bord,  pct + GLAARG_RADEON7000_KLUDGE);
            nui_glVertex2f(x+perp_x, y+perp_y);
            nui_glTexCoord2f(pdb+bord,  pct + GLAARG_RADEON7000_KLUDGE);
            nui_glVertex2f(x-perp_x, y-perp_y);

            nui_glColor4ubv((const GLubyte *)&cl4);
            nui_glTexCoord2f(psz-bord,  psz-bord);
            nui_glVertex2f(x+perp_x+parl_x, y+perp_y+parl_y);
            nui_glTexCoord2f(pdb+bord,  psz-bord);
            nui_glVertex2f(x-perp_x+parl_x, y-perp_y+parl_y);
            nui_glEnd();
          }
          else
          {
            if (glAA_vi < 2)
            {
              nui_glBegin(GL_TRIANGLE_STRIP);
              nui_glColor4ubv((const GLubyte *)&cl1);
              nui_glTexCoord2f(psz-bord,  psz-bord);
              nui_glVertex2f(x1+perp_x-parl_x, y1+perp_y-parl_y);
              nui_glTexCoord2f(pdb+bord,  psz-bord);
              nui_glVertex2f(x1-perp_x-parl_x, y1-perp_y-parl_y);
            }

            nui_glColor4ubv((const GLubyte *)&cl2);
            nui_glTexCoord2f(psz-bord,  pct);
            nui_glVertex2f(x1+perp_x, y1+perp_y);
            nui_glTexCoord2f(pdb+bord,  pct);
            nui_glVertex2f(x1-perp_x, y1-perp_y);

            nui_glColor4ubv((const GLubyte *)&cl3);        
            nui_glTexCoord2f(psz-bord,  pct + GLAARG_RADEON7000_KLUDGE);
            nui_glVertex2f(x+perp_x, y+perp_y);
            nui_glTexCoord2f(pdb+bord,  pct + GLAARG_RADEON7000_KLUDGE);
            nui_glVertex2f(x-perp_x, y-perp_y);

            {
              end_color = cl4;
              end_tx1 = pdb + bord;
              end_ty1 = psz - bord;
              end_x1 = x-perp_x+parl_x;
              end_y1 = y-perp_y+parl_y;
              end_tx2 = psz-bord;
              end_ty2 = psz-bord;
              end_x2 = x+perp_x+parl_x;
              end_y2 = y+perp_y+parl_y;
              nui_glColor4ubv((const GLubyte *)&cl4);
              nui_glTexCoord2f(psz-bord,  psz-bord);
              nui_glVertex2f(x+perp_x+parl_x, y+perp_y+parl_y);
              nui_glTexCoord2f(pdb+bord,  psz-bord);
              nui_glVertex2f(x-perp_x+parl_x, y-perp_y+parl_y);

            }
          }
          s  += dash+gap;
        }
        glAA_stipple_idx = b;

        if (glAA_mode == GL_LINES) 
        {
          glAA_vi = glAA_vc[0] = glAA_vc[1] = 0;
        }
        else 
        {
          glAA_vc[glAA_vp] = 0; // note, the other vertex color is cleaned in glEnd,
          glAA_vp = glAA_vi;    // so that interpolation works between strip vertices
          glAA_vi++; 
          if (glAA_vi == 3) 
            glAA_vi = 1;
        }
      }
    } break;
  }
}

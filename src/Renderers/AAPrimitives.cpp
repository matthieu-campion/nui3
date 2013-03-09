/*
*  AAPrimitives.c
*  glAArg version 0.2.2
*
*  Created by arekkusu on Thu Nov 20 2003.
*  Copyright (c) 2004 arekkusu. All rights reserved.
*  http://homepage.mac.com/arekkusu/bugs/invariance/
*
*/


#include "nui.h"

#ifndef __NUI_NO_AA__

#include "AAPrimitives.h"

#ifndef CLAMP
#define CLAMP(X,lo,hi)  ({ __typeof__(X) __x = (X); __typeof__(lo) __l = (lo); __typeof__(hi) __h = (hi);\
((__x) > (__h)) ? (__h) : (((__x) < (__l)) ? (__l) : (__x));  })
#endif

#endif

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
static inline uint32 lerpRGBA(const uint32 d, const uint32 s, const float t)
{
  uint32 ti = ToBelow(t*256);
  uint32 dstga = d      & 0xFF00FF;
  uint32 dstrb = d >> 8 & 0xFF00FF;
  uint32 srcga = s      & 0xFF00FF;
  uint32 srcrb = s >> 8 & 0xFF00FF;
  uint32 dga = srcga - dstga;
  uint32 drb = srcrb - dstrb;
  dga = (dga * ti) >> 8;  
  drb = (drb * ti) >> 8;  
  const uint32 ga  = (dga + dstga)      & 0x00FF00FF;
  const uint32 rb  = (drb + dstrb) << 8 & 0xFF00FF00;
  return ga | rb;
}


#ifndef glError
#if defined DEBUG && !(defined _UIKIT_) && !(defined _ANDROID_)
#define glError() { GLenum err = glGetError(); if (GL_NO_ERROR != err) NGL_OUT(_T("glError: %s caught at %s:%u\n"), (char *)gluErrorString(err), __FILE__, __LINE__); }
#else
#define glError()
#endif
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
//#define GLAARG_RADEON7000_KLUDGE 0.0001f
#define GLAARG_RADEON7000_KLUDGE 0.0001f

// VAR convienence constants based on the hardcoded values in the header.
#define vsize (sizeof(Vertex) * VAR_size)
#define line_i (ToBelow((VAR_size*1.5)))


// exposed globals for client app
GLuint          glAA_texture;      // the AA sphere texIDs
uint8      *glAA_AAtex;        // AA texture buffer

// file-scope globals
static GLenum      glAA_mode, glAA_old_mode;   // state globals
static Vertex      glAA_v[4];          // temp stack space for three points (quad is biggest primitive)
static int32        glAA_vi, glAA_vp, glAA_vcl; // vertex stack index, vertex previous index, vertex color last set
static int32        glAA_vc[4];          // vertex color stack usage bools
static float      glAA_alpha_fix;        // for points/lines < 1.0 px wide or long

static float      glAA_point_size;      // point globals
static float      glAA_point_border;
static float      glAA_point_maxr;
static uint16  glAA_plight[4];

static float      glAA_line_width;      // line globals
static float      glAA_line_border;
static float      glAA_line_maxr;

static Vertex      *glAA_VAR;          // VAR globals
static uint16   *glAA_line_index;
static int32        glAA_VAR_i, glAA_VAR_skip, glAA_fence;

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
  int32 i;
  for (i=1; i<=pdb; i++)
  {
    // lookup table has 4x the bitmap resolution
    logtbl[i-1] = log((float)i/pdb);
  }

  glAA_texture = 0; 

  glAASetContext();

  /*
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &glAA_VAR[0].rgba);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &glAA_VAR[0].tx);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &glAA_VAR[0].x);
   */

  // setup defaults
  glAA_old_mode = (GLenum)-1;
  glAALineWidth(1.0f);
  glAAColor1ui(0xFFFFFFFF);
}


void glAAExit() 
{
  int32 i;
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
float ifun(float x, float y, float F) 
{
  // compute falloff at x,y with exponent F [-1..1]
  float S = (x * x + y);
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
    S = exp(logtbl[ToBelow(L * 4.0f)] / F);
  else
    // this is a bit more complex-- logarithm ramp up to a solid dot with slight S blend at cusp
    S = L < (1.0f + F) * phf ? 0.0f : exp((1.0f + F * 0.9f)/(L / phf - 1.0f - F) * logtbl[ToBelow( L * 4.0f)]);
  return 1.0f - S;
}


// 8-way symmetric macro
#define putpixel(x, y, I) {             \
  uint8 c = I*255;           \
  texture[(pct   + x) + (pct   + y) * pdb * 1] =   \
  texture[(pct-1 - x) + (pct   + y) * pdb * 1] =   \
  texture[(pct   + x) + (pct-1 - y) * pdb * 1] =   \
  texture[(pct-1 - x) + (pct-1 - y) * pdb * 1] =   \
  texture[(pct   + y) + (pct   + x) * pdb * 1] =   \
  texture[(pct-1 - y) + (pct   + x) * pdb * 1] =   \
  texture[(pct   + y) + (pct-1 - x) * pdb * 1] =   \
  texture[(pct-1 - y) + (pct-1 - x) * pdb * 1] = c;\
}

void glAAGenerateAABuffer(float Falloff, float alias, uint8* texture) 
{      
  // compute the AA sphere
  {
    uint32 l2phf = ToBelow(log2(phf));
    uint32 l2psz = ToBelow(log2(psz));
    
    int32 x = phf - 1;
    int32 y = 0;
    int32 ax;
    float T = 0.0f;
    float ys = 0.0f;
    while (x > y) 
    {
      ys = (float)(y * y);
      float L = sqrt(prs - ys);
      float D = ceilf(L) - L;
      if (D < T) x--;
      for(ax = y; ax < x; ax++)
        putpixel(ax, y, (uint8)ifun((float)ax, ys, Falloff));            // fill wedge
      putpixel(x, y, (uint8)((1.0f - D) * (ifun((float)x, ys, Falloff))));          // outer edge
      T = D;
      y++;
    }
  }
}

void glAAGenerateAATex(float Falloff, float alias) 
{      
  // compute the AA sphere
  {
    uint32 l2phf = ToBelow(log2(phf));
    uint32 l2psz = ToBelow(log2(psz));

    if (!glAA_AAtex) 
      glAA_AAtex = (uint8*)malloc(pdb * pdb);
    uint8 *texture = glAA_AAtex;
    if (texture) 
    {
      glAAGenerateAABuffer(Falloff, alias, texture);
      
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
#if (!defined _UIKIT_) && (!defined _ANDROID_)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, l2psz);
#endif
      }

      glBindTexture(GL_TEXTURE_2D, glAA_texture);

      // Generate the entire mip pyramid slowly in software
#if (!defined _UIKIT_) && (!defined _ANDROID_)
      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE_ALPHA, pdb, pdb, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texture);
#else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, pdb, pdb, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texture);
#endif


      // Set filtering depending on our aliasing mode.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (alias > 0.66) ? GL_NEAREST : GL_LINEAR);  
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (alias > 0.33) ? ((alias > 0.66) ? GL_NEAREST : GL_LINEAR_MIPMAP_NEAREST) : GL_LINEAR_MIPMAP_LINEAR);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // now fix the upper mip levels so that 1 px points/lines stay bright:
      // at 1 px, the circle texture's default brightness is pi/4 = 0.78. This is geometrically
      // correct, but for usability reasons we want 1 px wide lines to display at full 1.0 brightness.
      // the following is an approximation sampling of the texture to do that, getting values that
      // "look ok" for exponential falloff in either direction.
      static uint8 mipfix[4];
      uint8 fix = (texture[ToBelow(psz + phf * 0.2f) + pct * pdb] + texture[ToBelow(psz + phf * 0.7f) + pct * pdb]) >> 1;
      mipfix[0] = mipfix[1] = mipfix[2] = mipfix[3] = fix;
      glTexSubImage2D(GL_TEXTURE_2D, l2phf, 2, 2, 2, 2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, mipfix);
      glTexSubImage2D(GL_TEXTURE_2D, l2psz, 1, 1, 1, 1, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, mipfix);
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

static int32 end_color;
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

  if (glAA_mode == GL_POINTS)
    nui_glEnd();
  glError();
}


void glAALineWidth(float w) 
{
  float width = ((w > 0.0f) ? w : 1.0f);

  if (width != glAA_line_width) 
  {
    float r = MAX(1.0f, width);
    // find min/max using float trick (faster than floor/ceil or casts, only works for positive IEEE 32 bit floats)
    float minr = (r + 8388608.f) - 8388608.f;
    if (r < minr)
      minr -= 1.0f;
    float maxr = (minr == r) ? minr : minr + 1.0f;
    // calculate the tex coords for a 0.5px outset, doing two divides at once
    float den2 = 1.0f / (maxr * minr);
    float ratio = phf * minr * den2;
    glAA_line_border = ratio + (phf * maxr * den2 * 1.5f - ratio) * (maxr - r);
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
  glAA_v[glAA_vi].rgba = (ToBelow(R * 255.0f) << 24) | (ToBelow(G * 255.0f) << 16) | (ToBelow(B * 255.0f) << 8) | ToBelow(A * 255.0f);
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

      float size = maxr + 1.0f;
      float centx = x - (maxr + 1.0f) * 0.5f;
      float centy = y - (maxr + 1.0f) * 0.5f;

      // adjust vertex alpha if needed
      int32 c1 = glAA_v[glAA_vcl].rgba;
      if (alpha_fix < 1.0f) 
      {
        int32 int_alpha_fix = ToBelow(alpha_fix * 255.0f);
        c1 = (c1 & 0xFFFFFF00) | (((c1 & 0x000000FF) * int_alpha_fix) >> 8);
      }

      nui_glColor4ubv((const GLubyte *)&c1);
      nui_glTexCoord2f(psz - bord, psz - bord);
      nui_glVertex2f(centx,      centy);
      nui_glColor4ubv((const GLubyte *)&c1);
      nui_glTexCoord2f(pdb + bord, psz - bord);
      nui_glVertex2f(centx + size, centy);
      nui_glColor4ubv((const GLubyte *)&c1);
      nui_glTexCoord2f(pdb+bord, pdb + bord);
      nui_glVertex2f(centx + size, centy+size);
      nui_glColor4ubv((const GLubyte *)&c1);
      nui_glTexCoord2f(psz - bord, pdb + bord);
      nui_glVertex2f(centx,      centy + size);
    }
    break;

  case GL_LINE_LOOP:
  case GL_LINE_STRIP:
  case GL_LINES: 
    {
      if (glAA_vi < 1) 
      {
        glAA_v[glAA_vi].x = x;
        glAA_v[glAA_vi++].y = y;
        if (glAA_mode == GL_LINE_LOOP) 
        { 
          // remember the first point for looping
          glAA_v[3].x = x;
          glAA_v[3].y = y;
        }
      } 
      else 
      {
        if (glAA_mode != GL_LINES)
        { 
          // remember the new point
          glAA_v[glAA_vi].x = x;
          glAA_v[glAA_vi].y = y;
        }
        float x1 = glAA_v[glAA_vp].x; // shadow globals
        float y1 = glAA_v[glAA_vp].y;
        float bord = glAA_line_border;
        float maxr = glAA_line_maxr;        
        float alpha_fix = glAA_alpha_fix;

        float perp_y = x1 - x;
        float perp_x = y - y1;
        float perpd, leng;

        float factor = perp_y * perp_y + perp_x * perp_x;
        if (factor) 
        {
          perpd = frsqrtes_nr(factor);
          perp_y *= perpd; // normalize to 1px
          perp_x *= perpd;
          leng = 1.0f / perpd; // could do perpd*factor, or __fres(perpd) to get estimate of the
          // real sqrt. But the err is too high on the G5, so we need a divide here.
          alpha_fix *= MIN(1.0f, leng);
          leng = MAX(1.0f, leng);
          perpd = MIN(1.0f, perpd);
          x1 -= perp_y * 0.5f;
          y1 += perp_x * 0.5f;
        } 
        else 
        {
          perp_y = 0.0f;
          perp_x = 1.0f;
          perpd = leng = 1.0f;
        }

        float width = (maxr + 1.0f) * 0.5f;
        float unit_x = -perp_y;
        float unit_y =  perp_x;
        perp_y *= width;
        perp_x *= width;
        float parl_x = -perp_y;
        float parl_y =  perp_x;

        float s = 0.0f;

        int32 i = glAA_VAR_i;
        const float lengm1 = leng - 1.0f;

        // pick vertex colors and adjust alpha if needed
        int32 c1 = (glAA_vc[glAA_vp]) ? glAA_v[glAA_vp].rgba : glAA_v[glAA_vcl].rgba;
        int32 c2 = (glAA_vc[glAA_vi]) ? glAA_v[glAA_vi].rgba : glAA_v[glAA_vcl].rgba;
        if (alpha_fix < 1.0f) 
        {
          int32 int_alpha_fix = ToBelow(alpha_fix * 255.0f);
          c1 = (c1 & 0xFFFFFF00) | (((c1 & 0x000000FF) * int_alpha_fix) >> 8);
          c2 = (c2 & 0xFFFFFF00) | (((c2 & 0x000000FF) * int_alpha_fix) >> 8);
        }

        if (glAA_line_width <= 1.0f || glAA_mode != GL_LINE_STRIP)
        {
          nui_glBegin(GL_TRIANGLE_STRIP);
          nui_glColor4ubv((const GLubyte *)&c1);
          nui_glTexCoord2f(pdb + bord,  psz - bord);
          nui_glVertex2f(x1 - perp_x - parl_x, y1 - perp_y - parl_y);
          nui_glTexCoord2f(psz - bord,  psz - bord);
          nui_glVertex2f(x1 + perp_x - parl_x, y1 + perp_y - parl_y);

          nui_glColor4ubv((const GLubyte *)&c1);
          nui_glTexCoord2f(psz - bord,  pct);
          nui_glVertex2f(x1 + perp_x, y1 + perp_y);
          nui_glTexCoord2f(pdb + bord,  pct);
          nui_glVertex2f(x1 - perp_x, y1 - perp_y);

          nui_glColor4ubv((const GLubyte *)&c2);        
          nui_glTexCoord2f(psz - bord,  pct + GLAARG_RADEON7000_KLUDGE);
          nui_glVertex2f(x + perp_x, y + perp_y);
          nui_glTexCoord2f(pdb + bord,  pct + GLAARG_RADEON7000_KLUDGE);
          nui_glVertex2f(x - perp_x, y - perp_y);

          nui_glColor4ubv((const GLubyte *)&c2);
          nui_glTexCoord2f(psz - bord,  psz - bord);
          nui_glVertex2f(x + perp_x + parl_x, y + perp_y + parl_y);
          nui_glTexCoord2f(pdb + bord,  psz - bord);
          nui_glVertex2f(x - perp_x + parl_x, y - perp_y + parl_y);
          nui_glEnd();
        }
        else
        {
          if (glAA_vi < 2)
          {
            nui_glBegin(GL_TRIANGLE_STRIP);
            nui_glColor4ubv((const GLubyte *)&c1);
            nui_glTexCoord2f(psz - bord,  psz - bord);
            nui_glVertex2f(x1 + perp_x - parl_x, y1 + perp_y - parl_y);
            nui_glTexCoord2f(pdb + bord,  psz - bord);
            nui_glVertex2f(x1 - perp_x - parl_x, y1 - perp_y - parl_y);
          }

          nui_glColor4ubv((const GLubyte *)&c1);
          nui_glTexCoord2f(psz - bord,  pct);
          nui_glVertex2f(x1 + perp_x, y1 + perp_y);
          nui_glTexCoord2f(pdb + bord,  pct);
          nui_glVertex2f(x1 - perp_x, y1 - perp_y);

          nui_glColor4ubv((const GLubyte *)&c2);        
          nui_glTexCoord2f(psz - bord,  pct + GLAARG_RADEON7000_KLUDGE);
          nui_glVertex2f(x + perp_x, y + perp_y);
          nui_glTexCoord2f(pdb + bord,  pct + GLAARG_RADEON7000_KLUDGE);
          nui_glVertex2f(x - perp_x, y - perp_y);

          end_color = c2;
          end_tx1 = pdb + bord;
          end_ty1 = psz - bord;
          end_x1 = x - perp_x + parl_x;
          end_y1 = y - perp_y + parl_y;
          end_tx2 = psz - bord;
          end_ty2 = psz - bord;
          end_x2 = x + perp_x + parl_x;
          end_y2 = y + perp_y + parl_y;
          nui_glColor4ubv((const GLubyte *)&c2);
          nui_glTexCoord2f(psz - bord,  psz - bord);
          nui_glVertex2f(x + perp_x + parl_x, y + perp_y + parl_y);
          nui_glTexCoord2f(pdb + bord,  psz - bord);
          nui_glVertex2f(x - perp_x + parl_x, y - perp_y + parl_y);
        }

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

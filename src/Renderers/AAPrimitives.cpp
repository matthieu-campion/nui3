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

#include <stdlib.h>        // for malloc
#include <string.h>        // for strstr, because gluCheckExtension isn't available in libglu32 on windows
#if defined(__APPLE__)      // for ntohl byteswapping, should be a no-op on PPC and __bswap on x86.
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
GLuint          glAA_texture[8];      // the AA sphere texIDs
unsigned char      *glAA_AAtex[8];        // AA texture buffer

// file-scope globals
static GLenum      glAA_mode, glAA_old_mode;   // state globals
static bool        glAA_enable_plight, glAA_enable_stipple;
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

static bool        hasMIPGEN, hasVAR,      // control globals
hasCLIENTTEX;
static bool        useMIPGEN, useVAR;
static float      logtbl[pdb];        // for sphere lookups

// private helper functions

// -------------------------------------------------
// set the context for macros
// -------------------------------------------------
static inline void glAASetContext() 
{
}


// -------------------------------------------------
// test hardware mipmap generation
// -------------------------------------------------
bool introspectMIPGEN() 
{
  bool workingMIPGEN = 0;
  return false; // temporarily always fail, until I figure out what is wrong with GF4MX
  if (hasMIPGEN) 
  {
    // test hardware mipmap generation to verify it does the right thing (some drivers don't)
    GLuint texID[1];
    const unsigned char tex[] = 
    { 
      0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,
      0,   0, 255, 255, 255, 127,   0,   0,
      0,   0, 255, 255, 255, 255,   0,   0,
      0,   0, 127, 127, 255, 255,   0,   0,
      0,   0, 127, 127, 255,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0 
    };

    glGenTextures(1, &texID[0]);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // Use hardware filtering and client storage for faster texture generation.
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_FASTEST);
    if (hasCLIENTTEX) 
      glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
    // Generate the full texture mipmap pyramid.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 8, 8, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
    if (hasCLIENTTEX) 
      glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 0);    

    // now check the results
    unsigned char tex1[16];
    glGetTexImage(GL_TEXTURE_2D, 1, GL_ALPHA, GL_UNSIGNED_BYTE, tex1);
    if (((tex1[0] | tex1[1] | tex1[2] | tex1[3] |
      tex1[4] |                     tex1[7] |
      tex1[8] |                     tex1[11]|
      tex1[12]| tex1[13]| tex1[14]| tex1[15]) == 0) &&
      (tex1[5] == 255) && (tex1[6] == 223) &&
      (tex1[9] == 127) && (tex1[10]== 191))
      workingMIPGEN = 1;
    // printf("glAArg: Using hardware mipmap generation: %d\n", workingMIPGEN);
    glDeleteTextures(1, &texID[0]);
  }
  // return workingMIPGEN;
}


// glAA APIs

// glAAInit/Exit should be called once at program begin/end to set up globals.
// NOTE: this version is NOT multiple-context clean. The client app is responsible for
// setting up GL ctx and state (PixelStore, texture matrix etc)
// THERE MUST BE A VALID CONTEXT CREATED BEFORE CALLING THIS (to set up VAR state etc)
//
void APIENTRY glAAInit() 
{
  int i;
  for (i=1; i<=pdb; i++)
  {
    // lookup table has 4x the bitmap resolution
    logtbl[i-1] = log((float)i/pdb);
  }

  for (i=0; i<8; i++)
  {
    glAA_texture[i] = 0; 
  }

  glAASetContext();
  // check renderer capabilities
  const GLubyte *extensions = NULL; //glGetString(GL_EXTENSIONS);
  hasVAR = hasMIPGEN = hasCLIENTTEX = 0;

  if (extensions) 
  {
#if 0 && defined(__APPLE__)
    // this should be true on all renderers except Rage 128 and Radeon 7000.
    hasVAR = (strstr((char *)extensions, "GL_APPLE_vertex_array_range") != NULL);
#endif  
    // on OS X, this should be true on 10.2.3 and later, but it does not work reliably on some GPUs.
    //hasMIPGEN = (strstr((char *)extensions, "GL_SGIS_generate_mipmap") != NULL);

    // this should be true on all OS X renderers, and not on any linux or windows renderers.
    //hasCLIENTTEX = (strstr((char *)extensions, "GL_APPLE_client_storage") != NULL);
  }
  useVAR = 0; // off by default. Client must turn on (and handle flushing.)
  useMIPGEN = introspectMIPGEN(); // use it only if it actually works

  // VAR buffers & state
  //
  // Currently, if the VAR extension is not available, we revert to immediate mode.
  // It is easy to modify this to use regular vertex array submission instead.
  // That provides a good speed boost on most systems, but it is actually slower on the Radeon 7000,
  // and on Mac OS X, that is one of the only two cards which do not have VAR.
  // A future update will provide VBO and/or non-interleaved vertex array submission. 
  //
  if (hasVAR) 
  {
    int j;
    if (!glAA_VAR)
      glAA_VAR = (Vertex *) malloc(vsize * VAR_bufs);
    if (!glAA_line_index) 
    {
      glAA_line_index = (unsigned short *)malloc(sizeof(unsigned short)*line_i*VAR_bufs);
      for (j=0; j<VAR_bufs; j++)
      {
        unsigned short v = VAR_size*j;
        for (i=line_i*j; i<line_i*(j+1); i+= 12)
        {
          glAA_line_index[i  ] = v;
          glAA_line_index[i+1] = v+1;
          glAA_line_index[i+2] = v+3;
          glAA_line_index[i+3] = v+2;
          glAA_line_index[i+4] = v+4;
          glAA_line_index[i+5] = v+5;
          glAA_line_index[i+6] = v+7;
          glAA_line_index[i+7] = v+6;
          glAA_line_index[i+8] = v+2;
          glAA_line_index[i+9] = v+3;
          glAA_line_index[i+10]= v+5;
          glAA_line_index[i+11]= v+4;
          v=v+8;
        }
      }
    }
    glAA_VAR_i = glAA_fence = 0;

#if 0 && defined(__APPLE__)
    glEnableClientState(GL_VERTEX_ARRAY_RANGE_APPLE);
    glVertexArrayRangeAPPLE(vsize * VAR_bufs, (GLvoid *)glAA_VAR);
    glGenFencesAPPLE(VAR_bufs, &glAA_fences[0]);
#endif
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &glAA_VAR[0].rgba);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &glAA_VAR[0].tx);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &glAA_VAR[0].x);
  }

  // setup defaults
  glAA_old_mode = (GLenum)-1;
  glAAPointSize(1.0f);
  GLCoord2 tempcoord = {0.0f, 0.0f};
  glAAPointLight(tempcoord);
  glAALineWidth(1.0f);
  glAALineStipplePhase(1.0f, 0xFFFF, 0.0f);
  glAADisable(GL_LINE_STIPPLE);
  glAAColor1ui(0xFFFFFFFF);
}


void APIENTRY glAAExit() 
{
  int i;
  glAASetContext();

  // dispose of VA buffers
  if (hasVAR) 
  {
#if 0 && defined(__APPLE__)
    glVertexArrayRangeAPPLE(0, 0);
    glDisableClientState(GL_VERTEX_ARRAY_RANGE_APPLE);
    glDeleteFencesAPPLE(VAR_bufs, &glAA_fences[0]);
#endif
    free (glAA_VAR); glAA_VAR = 0;
    free (glAA_line_index); glAA_line_index = 0;
  }

  // and texture data
  for (i=0; i<8; i++)
  {
    if (glAA_texture[i]) 
    { 
      glDeleteTextures(1, &glAA_texture[i]); 
      glAA_texture[i] = 0; 
    }

    if (glAA_AAtex[i])   
    { 
      free(glAA_AAtex[i]); 
      glAA_AAtex[i] = 0; 
    }
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


void APIENTRY glAAGenerateAATex(float F, GLuint id, float alias) 
{      
  // compute the AA sphere
  if (id < 8) 
  {
    uint32 l2phf = ToBelow(log2(phf));
    uint32 l2psz = ToBelow(log2(psz));

    if (!glAA_AAtex[id]) 
      glAA_AAtex[id] = (unsigned char*)calloc(1, pdb*pdb);
    unsigned char *texture = glAA_AAtex[id];
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
      if (!glAA_texture[id])
      {
        glAA_texture[id] = -1;
        glGenTextures(1, &glAA_texture[id]);
        glBindTexture(GL_TEXTURE_2D, glAA_texture[id]);
        // Ideally we could use GL_CLAMP_TO_BORDER, but this is not available on GF2MX and GF4MX.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Clamp the max mip level to 2x2 (1 px with 1 px border all around...)
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, l2psz);
/*
        if (useMIPGEN) 
        {
          // Use hardware filtering and client storage for faster texture generation.
          glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
          glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_FASTEST);
          if (hasCLIENTTEX)
            glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
          // Generate the full texture mipmap pyramid the first time.
          glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, pdb, pdb, 0, GL_ALPHA, GL_UNSIGNED_BYTE, texture);
        }
        else 
*/
        {
          // Generate the entire mip pyramid slowly in software
          gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, pdb, pdb, GL_ALPHA, GL_UNSIGNED_BYTE, texture);
        }
      }
      else 
      {
        glBindTexture(GL_TEXTURE_2D, glAA_texture[id]);
/*
        if (useMIPGEN) 
        {
          if (hasCLIENTTEX) glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
          // Update only the sphere area after the first time.
          glPixelStorei(GL_UNPACK_ROW_LENGTH, pdb);
          glTexSubImage2D(GL_TEXTURE_2D, 0, psz, psz, psz, psz, GL_ALPHA, GL_UNSIGNED_BYTE, texture+pdb*psz+psz);
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        }
        else 
*/
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
      if (useMIPGEN & hasCLIENTTEX) 
      {
//        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 0);
      }
    }
  }
  glError();
}


// glAA APIs mirroring OpenGL functionality
//
// The concept here is very simple, just buffer incoming vertex data until the current primitive is full,
// then calculate appropriate textured triangles for the primitive and submit in a group.
//
void APIENTRY glAABegin(GLenum mode) 
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
    glAAFlush();
  }
  glAA_mode = mode;
  if (!useVAR && (mode==GL_POINTS)) 
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


void APIENTRY glAAEnd() 
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
  if (!useVAR && (glAA_mode==GL_POINTS)) 
    nui_glEnd();
  glError();
}


void APIENTRY glAAFlush() 
{
#if 0 && defined(__APPLE__)
  if (useVAR)
  {
    static int glAA_VAR_ip = 0;
    int length = glAA_VAR_i-glAA_VAR_ip;

    if (length > 0)
    {
      switch (glAA_mode) 
      {
      case GL_POINTS: 
        {
          // if (glAA_VAR_ip &3) printf("glAArg: WARNING! flushing pnt  from bad align %x %d\n", glAA_VAR_ip, glAA_VAR_skip);
          // printf("glAArg: flushing %4d pnts  (%5d vertices) from %04x\n", length/4, length, glAA_VAR_ip);
          glFlushVertexArrayRangeAPPLE(length*sizeof(Vertex), &glAA_VAR[glAA_VAR_ip]);
          glDrawArrays(GL_QUADS, glAA_VAR_ip, length);
        }
        break;
      case GL_LINE_LOOP:
      case GL_LINE_STRIP:
      case GL_LINES: 
        {
          // if (glAA_VAR_ip &7) printf("glAArg: WARNING! flushing line from bad align %x %d\n", glAA_VAR_ip, glAA_VAR_skip);
          // printf("glAArg: flushing %4d lines (%5d vertices) from %04x\n", length/8, length, glAA_VAR_ip);
          // printf("drawing %5d line elements from %04x\n", length+(length>>1), glAA_VAR_ip+(glAA_VAR_ip>>1));
          glFlushVertexArrayRangeAPPLE(length*sizeof(Vertex), &glAA_VAR[glAA_VAR_ip]);
          glDrawElements(GL_QUADS, length+(length>>1), GL_UNSIGNED_SHORT, &glAA_line_index[glAA_VAR_ip+(glAA_VAR_ip>>1)]);
        }
        break;
      }
    }
    if (glAA_VAR_skip) 
    {
      glAA_VAR_i += glAA_VAR_skip;
      glAA_VAR_ip += glAA_VAR_skip;
      glAA_VAR_skip = 0;
    }

    if (glAA_VAR_i && !(glAA_VAR_i%VAR_size))
    {
      glSetFenceAPPLE(glAA_fences[glAA_fence]);
      glError();  // any error reported here in debug mode is probably from the client, not glAArg!
      glAA_fence++; 
      if (glAA_fence >= VAR_bufs) 
        glAA_fence = 0;
      glAA_VAR_i = glAA_VAR_ip = VAR_size*glAA_fence;
      // printf("glAArg: VAR index fenced to %04x\n", glAA_VAR_i);
      glFinishFenceAPPLE(glAA_fences[glAA_fence]);
      // Get rid of any spurious error that may occur after switching contexts, in order to not mislead clients...
      while(glGetError() != GL_NO_ERROR)
        ;
    }
    else glAA_VAR_ip = glAA_VAR_i;
  }
  else 
  { 
    // immediate mode, do nothing.
    // glFlush();
  }
#endif
}


void APIENTRY glAAEnable(GLenum cap) 
{
  if (cap == GL_LINE_STIPPLE) 
    glAA_enable_stipple = 1;
  else if (cap == GLAA_VERTEX_ARRAY) 
    useVAR = hasVAR;
  else 
    glEnable(cap);
}


void APIENTRY glAADisable(GLenum cap) 
{
  if (cap == GL_LINE_STIPPLE) 
    glAA_enable_stipple = 0;
  else if (cap == GLAA_VERTEX_ARRAY) 
    useVAR = 0;
  else 
    glDisable(cap);
}


void APIENTRY glAAPointSize(float s) 
{
  float size = ((s>0.0f) ? s: 1.0f);

  if (size != glAA_point_size) 
  {
    float r = MAX(1.0f, size);
    // find min/max using float trick (faster than floor/ceil or casts, only works for positive IEEE 32 bit floats)
    float minr = (r+8388608.f)-8388608.f;
    if (r < minr) minr -= 1.0f;
    float maxr = (minr == r) ? minr : minr+1.0f;
    // calculate the tex coords for a 0.5px outset, doing two divides at once
    float den2 = 1.0f/(maxr*minr);
    float ratio = phf*minr*den2;
    glAA_point_border = ratio+(phf*maxr*den2*1.5f-ratio)*(maxr-r);
    glAA_point_maxr = maxr;
    glAA_point_size = size;
  }
  if (glAA_mode == GL_POINTS)              // allow changing before glAAEnd()
    glAA_alpha_fix = MIN(1.0f, glAA_point_size);
}


void APIENTRY glAAPointLight(GLCoord2 light) 
{
  // passed in point normalized to -1..1
  float x = light.x, y = light.y;
  // contain coords within circle to avoid interpolation artifacts
  float factor = x*x+y*y;
  if (factor) 
  {
    float perpd = frsqrtes_nr(factor);
    if (perpd < 1)
    {
      x *= perpd;
      y *= perpd;
    }
  }
  x+=1.0f; y+=1.0f;

  glAA_plight[0] = (unsigned short)(256*((2.0f-x)*(2.0f-y)));
  glAA_plight[1] = (unsigned short)(256*(      x *(2.0f-y)));
  glAA_plight[2] = (unsigned short)(256*(      x *      y));
  glAA_plight[3] = (unsigned short)(256*((2.0f-x)*      y));
  glAA_enable_plight = (light.x || light.y);
}


void APIENTRY glAALineWidth(float w) 
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


inline void APIENTRY glAALineStipple (GLint factor, GLushort pattern) 
{
  glAALineStipplePhase((GLfloat)factor, pattern, 0.0f);
}


// stipple pattern is buggy. wrapping across segments and phase adjustment are broken.
void APIENTRY glAALineStipplePhase (GLfloat factor, GLushort pattern, GLfloat phase) 
{
  float fac = MAX(1.0f, factor), len = 0.0f;
  int b, i=0, pat = pattern, m=pat&1;
  for (b=0; b<16;)
  {
    int p=pat&1;
    while ((m==p) && (b<16))
    {
      len += fac;
      pat >>= 1;
      p=pat&1;
      b++;
    }
    glAA_stipple[i++] = m?len:-len;
    m = p;
    len = 0.0f;
  }
  if (i==1)
  {
    glAA_stipple[0] = FLT_MAX;
  }
  else if (i&1) 
  {
    // if odd number of dashes then we wrap, so adjust the phase
    // glAA_stipple[i+1] = 0.0f;
    i--;
    phase += fabsf(glAA_stipple[i]);
    glAA_stipple[0] += glAA_stipple[i];
  }
  glAA_stipple_phase = phase;
  glAA_stipple_count = i;
  //for (b=0; b<glAA_stipple_count; b++){ printf("%.0f ", glAA_stipple[b]); }
  //printf("  phase %.0f\n", glAA_stipple_phase);
}


inline void APIENTRY glAAColor3f(float R, float G, float B)
{
  glAAColor4f(R, G, B, 1.0f);
}


void APIENTRY glAAColor4f(float R, float G, float B, float A)
{
  glAA_vc[glAA_vi] = 1;
  glAA_vcl = glAA_vi;
  glAA_v[glAA_vi].rgba = (ToBelow(R*255.0f)<<24) | (ToBelow(G*255.0f)<<16) | (ToBelow(B*255.0f)<<8) | ToBelow(A*255.0f);
}


void APIENTRY glAAColor4ubv(const GLubyte *c)
{
  if (c) 
  {
    glAA_vc[glAA_vi] = 1;
    glAA_vcl = glAA_vi;
    glAA_v[glAA_vi].rgba = *(GLuint *)c;
  }
}


void APIENTRY glAAColor1ui(GLuint c)
{
  glAA_vc[glAA_vi] = 1;
  glAA_vcl = glAA_vi;
  glAA_v[glAA_vi].rgba = c;
}


// here's where all the real work is done.
void APIENTRY glAAVertex2f(float x, float y) 
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

      // apply saturation lighting if needed
      if (glAA_enable_plight) 
      {
        c1 = scalesatRGB(c1, glAA_plight[0]);
        c2 = scalesatRGB(c2, glAA_plight[1]);
        c3 = scalesatRGB(c3, glAA_plight[2]);
        c4 = scalesatRGB(c4, glAA_plight[3]);
      }
      c1 = ntohl(c1);
      c2 = ntohl(c2);
      c3 = ntohl(c3);
      c4 = ntohl(c4);

      if (useVAR)
      {
        int i = glAA_VAR_i;
        glAA_VAR[i  ].rgba = c1;
        glAA_VAR[i+1].rgba = c2;
        glAA_VAR[i+2].rgba = c3;
        glAA_VAR[i+3].rgba = c4;
        glAA_VAR[i  ].tx = glAA_VAR[i+3].tx =
          glAA_VAR[i  ].ty = glAA_VAR[i+1].ty = psz-bord;
        glAA_VAR[i+1].tx = glAA_VAR[i+2].tx = 
          glAA_VAR[i+2].ty = glAA_VAR[i+3].ty = pdb+bord;
        glAA_VAR[i  ].x = glAA_VAR[i+3].x = centx;
        glAA_VAR[i  ].y = glAA_VAR[i+1].y = centy;
        glAA_VAR[i+1].x = glAA_VAR[i+2].x = centx+size;
        glAA_VAR[i+2].y = glAA_VAR[i+3].y = centy+size;
        glAA_VAR_i += 4; 
        int room = VAR_size*(glAA_fence+1)-glAA_VAR_i;
        if (room < 4) { glAA_VAR_skip = room; glAAFlush();}
      }
      else 
      {  // immediate mode
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
    } break;

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
        bool enable_stipple = glAA_enable_stipple;

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

          if (enable_stipple)
          { // this chunk is buggy and in progress
            if (s==0.0f) 
            {
              dash = glAA_stipple[b];
              if (dash>0.0f) 
              {
                dash -= glAA_stipple_phase;
                b = (b+1)%glAA_stipple_count;
                gap = -glAA_stipple[b];
              }
              else 
              {
                s-=dash + glAA_stipple_phase;
                x1 -= unit_x*(dash + glAA_stipple_phase);
                y1 -= unit_y*(dash + glAA_stipple_phase);
                b = (b+1)%glAA_stipple_count;
                dash = glAA_stipple[b];
                b = (b+1)%glAA_stipple_count;
                gap = -glAA_stipple[b];
              }
            }
            else 
            {
              x1 += unit_x*(dash+gap);
              y1 += unit_y*(dash+gap);
              dash = fabsf(glAA_stipple[b]);
              b = (b+1)%glAA_stipple_count;
              gap =  fabsf(glAA_stipple[b]);
            }
            b = (b+1)%glAA_stipple_count;
          }

          if (s >= leng-dash) 
          {
            dash = MAX(1.0f, leng-s); // clip final dash
          } 
          // printf("glAArg: line w %.1f leng %.1f using(%d) %.1f dash [%d], s is %.1f, gap is %.1f [%d]\n", width, leng, glAA_enable_stipple, dash, b, s, gap, b+1);
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

          if (useVAR)
          {
            // should use altivec, here...
            glAA_VAR[i  ].rgba = glAA_VAR[i+1].rgba = ntohl(lerpRGBA(c1, c2, cp2));
            glAA_VAR[i+2].rgba = glAA_VAR[i+3].rgba = ntohl(lerpRGBA(c1, c2, cw2));
            glAA_VAR[i+4].rgba = glAA_VAR[i+5].rgba = ntohl(lerpRGBA(c1, c2, cw4));
            glAA_VAR[i+6].rgba = glAA_VAR[i+7].rgba = ntohl(lerpRGBA(c1, c2, cp4));

            glAA_VAR[i+3].tx = glAA_VAR[i+1].tx = glAA_VAR[i+5].tx = glAA_VAR[i+7].tx =
              glAA_VAR[i  ].ty = glAA_VAR[i+1].ty = glAA_VAR[i+6].ty = glAA_VAR[i+7].ty = psz-bord;
            glAA_VAR[i+2].tx = glAA_VAR[i  ].tx = glAA_VAR[i+4].tx = glAA_VAR[i+6].tx = pdb+bord;
            glAA_VAR[i+3].ty = glAA_VAR[i+2].ty = pct;
            glAA_VAR[i+5].ty = glAA_VAR[i+4].ty = pct + GLAARG_RADEON7000_KLUDGE;

            glAA_VAR[i  ].x = x1-perp_x-parl_x;    glAA_VAR[i  ].y = y1-perp_y-parl_y;
            glAA_VAR[i+1].x = x1+perp_x-parl_x;    glAA_VAR[i+1].y = y1+perp_y-parl_y;
            glAA_VAR[i+2].x = x1-perp_x;      glAA_VAR[i+2].y = y1-perp_y;
            glAA_VAR[i+3].x = x1+perp_x;      glAA_VAR[i+3].y = y1+perp_y;
            glAA_VAR[i+4].x = x-perp_x;        glAA_VAR[i+4].y = y-perp_y;
            glAA_VAR[i+5].x = x+perp_x;        glAA_VAR[i+5].y = y+perp_y;
            glAA_VAR[i+6].x = x-perp_x+parl_x;    glAA_VAR[i+6].y = y-perp_y+parl_y;
            glAA_VAR[i+7].x = x+perp_x+parl_x;    glAA_VAR[i+7].y = y+perp_y+parl_y;

            glAA_VAR_i += 8;
            int room = VAR_size*(glAA_fence+1)-glAA_VAR_i;
            if (room < 8) 
            { 
              glAA_VAR_skip = room; 
              glAAFlush();
            }

            i = glAA_VAR_i;
          }
          else 
          { // immediate  
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

              //if (!glAA_vi)
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

                //nui_glEnd();
              }

            }
          }
          s  += dash+gap;
        }
        glAA_stipple_idx = b;

        //  printf("glAArg: add line at %d alphafix is %f\n", glAA_VAR_i, glAA_alpha_fix);

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

/*
*  AAPrimitives.h
*  glAArg version 0.2.2
*
*  Created by arekkusu on Thu Nov 20 2003.
*  Copyright (c) 2004 arekkusu. All rights reserved.
*  http://homepage.mac.com/arekkusu/bugs/invariance/
*
*/

#ifndef __AAPRIMITIVES__
#define __AAPRIMITIVES__

#ifndef __NUI_NO_AA__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
#include "AAUtilities.h"


  // the point half-width in px. must be a power of two <= 256.
  // eventually this constant will probably be passed as an argument to glAAGenerateAATex() but it is hardcoded here for now.
#define phf 32

  // VAR constants; could be determined empirically at runtime depending on the GPU, but hardcoded here for now.
  // VAR_size*VAR_bufs must be <= 65536 to let the index be an unsigned short. VAR_bufs must be at least 2.
#define VAR_size 1024
#define VAR_bufs 64


  // basic 2D types
  typedef struct GLCoord2 {
    GLfloat  x;
    GLfloat  y;
  } GLCoord2;

  typedef struct Vertex {
    GLuint  rgba;      // usage of this int assumes big-endian order. it is byteswapped on x86 before submission to GL.
    GLfloat  x;
    GLfloat  y;
    GLfloat  tx;
    GLfloat  ty;
  } Vertex;   


  // expose these globals for client access
  extern GLuint      glAA_texture[8];
  extern unsigned char  *glAA_AAtex[8];


  // glAAEnable constants
#define GLAA_VERTEX_ARRAY  0x851D        // this tells glAArg to try to submit primitives via VA/VAR when possible


  // glAArg API
  void APIENTRY glAAInit();
  void APIENTRY glAAExit();
  void APIENTRY glAAGenerateAATex(float falloff, GLuint id, float alias);
  void APIENTRY glAABegin(GLenum mode);
  void APIENTRY glAAEnd();
  void APIENTRY glAAFlush();
  void APIENTRY glAAEnable(GLenum cap);
  void APIENTRY glAADisable(GLenum cap);
  void APIENTRY glAAPointSize(GLfloat size);
  void APIENTRY glAAPointLight(GLCoord2 light);
  void APIENTRY glAALineWidth(GLfloat width);
  void APIENTRY glAALineStipple (GLint factor, GLushort pattern);
  void APIENTRY glAALineStipplePhase (GLfloat factor, GLushort pattern, GLfloat phase); /* this is buggy */
  void APIENTRY glAAColor3f(float R, float G, float B);
  void APIENTRY glAAColor4f(float R, float G, float B, float A);
  void APIENTRY glAAColor4ubv(const GLubyte *c);
  void APIENTRY glAAColor1ui(GLuint c);
  void APIENTRY glAAVertex2f(GLfloat x, GLfloat y);


#ifdef __cplusplus
}
#endif

#endif//__NUI_NO_AA__

#endif //__AAPRIMITIVES__

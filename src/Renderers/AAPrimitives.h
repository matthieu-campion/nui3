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

// the point half-width in px. must be a power of two <= 256.
// eventually this constant will probably be passed as an argument to glAAGenerateAATex() but it is hardcoded here for now.
#define phf 32

// expose these globals for client access
extern GLuint glAA_texture;
extern uint8* glAA_AAtex;


// glAArg API
void glAAInit();
void glAAExit();
void glAAGenerateAATex(float falloff, float alias);
void glAABegin(GLenum mode);
void glAAEnd();
void glAALineWidth(GLfloat width);
void glAAColor3f(float R, float G, float B);
void glAAColor4f(float R, float G, float B, float A);
void glAAColor4ubv(const GLubyte *c);
void glAAColor1ui(GLuint c);
void glAAVertex2f(GLfloat x, GLfloat y);


#endif//__NUI_NO_AA__

#endif //__AAPRIMITIVES__

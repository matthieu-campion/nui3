/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/* This file is included and \b must only be included by nglContext.
 * These prototypes become part of nglContext's own methods.
 */

/* This file was generated with glext_gen.pl, please do not edit.  */

/** @name GL_VERSION_1_2
 *  See http://www.opengl.org/developers/documentation/Version1.2/OpenGL_spec_1.2.1.pdf
 */
//@{
void      (APIENTRY *glBlendColor) (GLclampf, GLclampf, GLclampf, GLclampf);
void      (APIENTRY *glBlendEquation) (GLenum);
void      (APIENTRY *glDrawRangeElements) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);
void      (APIENTRY *glColorTable) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glColorTableParameterfv) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glColorTableParameteriv) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glCopyColorTable) (GLenum, GLenum, GLint, GLint, GLsizei);
void      (APIENTRY *glGetColorTable) (GLenum, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetColorTableParameterfv) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetColorTableParameteriv) (GLenum, GLenum, GLint *);
void      (APIENTRY *glColorSubTable) (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glCopyColorSubTable) (GLenum, GLsizei, GLint, GLint, GLsizei);
void      (APIENTRY *glConvolutionFilter1D) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glConvolutionFilter2D) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glConvolutionParameterf) (GLenum, GLenum, GLfloat);
void      (APIENTRY *glConvolutionParameterfv) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glConvolutionParameteri) (GLenum, GLenum, GLint);
void      (APIENTRY *glConvolutionParameteriv) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glCopyConvolutionFilter1D) (GLenum, GLenum, GLint, GLint, GLsizei);
void      (APIENTRY *glCopyConvolutionFilter2D) (GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
void      (APIENTRY *glGetConvolutionFilter) (GLenum, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetConvolutionParameterfv) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetConvolutionParameteriv) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetSeparableFilter) (GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid *);
void      (APIENTRY *glSeparableFilter2D) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid *);
void      (APIENTRY *glGetHistogram) (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetHistogramParameterfv) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetHistogramParameteriv) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetMinmax) (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetMinmaxParameterfv) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetMinmaxParameteriv) (GLenum, GLenum, GLint *);
void      (APIENTRY *glHistogram) (GLenum, GLsizei, GLenum, GLboolean);
void      (APIENTRY *glMinmax) (GLenum, GLenum, GLboolean);
void      (APIENTRY *glResetHistogram) (GLenum);
void      (APIENTRY *glResetMinmax) (GLenum);
void      (APIENTRY *glTexImage3D) (GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glTexSubImage3D) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glCopyTexSubImage3D) (GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
//@}

/** @name GL_VERSION_1_3
 *  See http://www.opengl.org/developers/documentation/version1_3/glspec13.pdf
 */
//@{
void      (APIENTRY *glActiveTexture) (GLenum);
void      (APIENTRY *glClientActiveTexture) (GLenum);
void      (APIENTRY *glMultiTexCoord1d) (GLenum, GLdouble);
void      (APIENTRY *glMultiTexCoord1dv) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord1f) (GLenum, GLfloat);
void      (APIENTRY *glMultiTexCoord1fv) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord1i) (GLenum, GLint);
void      (APIENTRY *glMultiTexCoord1iv) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord1s) (GLenum, GLshort);
void      (APIENTRY *glMultiTexCoord1sv) (GLenum, const GLshort *);
void      (APIENTRY *glMultiTexCoord2d) (GLenum, GLdouble, GLdouble);
void      (APIENTRY *glMultiTexCoord2dv) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord2f) (GLenum, GLfloat, GLfloat);
void      (APIENTRY *glMultiTexCoord2fv) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord2i) (GLenum, GLint, GLint);
void      (APIENTRY *glMultiTexCoord2iv) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord2s) (GLenum, GLshort, GLshort);
void      (APIENTRY *glMultiTexCoord2sv) (GLenum, const GLshort *);
void      (APIENTRY *glMultiTexCoord3d) (GLenum, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glMultiTexCoord3dv) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord3f) (GLenum, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glMultiTexCoord3fv) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord3i) (GLenum, GLint, GLint, GLint);
void      (APIENTRY *glMultiTexCoord3iv) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord3s) (GLenum, GLshort, GLshort, GLshort);
void      (APIENTRY *glMultiTexCoord3sv) (GLenum, const GLshort *);
void      (APIENTRY *glMultiTexCoord4d) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glMultiTexCoord4dv) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord4f) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glMultiTexCoord4fv) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord4i) (GLenum, GLint, GLint, GLint, GLint);
void      (APIENTRY *glMultiTexCoord4iv) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord4s) (GLenum, GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glMultiTexCoord4sv) (GLenum, const GLshort *);
void      (APIENTRY *glLoadTransposeMatrixf) (const GLfloat *);
void      (APIENTRY *glLoadTransposeMatrixd) (const GLdouble *);
void      (APIENTRY *glMultTransposeMatrixf) (const GLfloat *);
void      (APIENTRY *glMultTransposeMatrixd) (const GLdouble *);
void      (APIENTRY *glSampleCoverage) (GLclampf, GLboolean);
void      (APIENTRY *glCompressedTexImage3D) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexImage2D) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexImage1D) (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexSubImage3D) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexSubImage2D) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexSubImage1D) (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glGetCompressedTexImage) (GLenum, GLint, GLvoid *);
//@}

/** @name GL_VERSION_1_4
 *  See http://www.opengl.org/developers/documentation/version1_4/glspec14.pdf
 */
//@{
void      (APIENTRY *glBlendFuncSeparate) (GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glFogCoordf) (GLfloat);
void      (APIENTRY *glFogCoordfv) (const GLfloat *);
void      (APIENTRY *glFogCoordd) (GLdouble);
void      (APIENTRY *glFogCoorddv) (const GLdouble *);
void      (APIENTRY *glFogCoordPointer) (GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glMultiDrawArrays) (GLenum, GLint *, GLsizei *, GLsizei);
void      (APIENTRY *glMultiDrawElements) (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);
void      (APIENTRY *glPointParameterf) (GLenum, GLfloat);
void      (APIENTRY *glPointParameterfv) (GLenum, const GLfloat *);
void      (APIENTRY *glPointParameteri) (GLenum, GLint);
void      (APIENTRY *glPointParameteriv) (GLenum, const GLint *);
void      (APIENTRY *glSecondaryColor3b) (GLbyte, GLbyte, GLbyte);
void      (APIENTRY *glSecondaryColor3bv) (const GLbyte *);
void      (APIENTRY *glSecondaryColor3d) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glSecondaryColor3dv) (const GLdouble *);
void      (APIENTRY *glSecondaryColor3f) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glSecondaryColor3fv) (const GLfloat *);
void      (APIENTRY *glSecondaryColor3i) (GLint, GLint, GLint);
void      (APIENTRY *glSecondaryColor3iv) (const GLint *);
void      (APIENTRY *glSecondaryColor3s) (GLshort, GLshort, GLshort);
void      (APIENTRY *glSecondaryColor3sv) (const GLshort *);
void      (APIENTRY *glSecondaryColor3ub) (GLubyte, GLubyte, GLubyte);
void      (APIENTRY *glSecondaryColor3ubv) (const GLubyte *);
void      (APIENTRY *glSecondaryColor3ui) (GLuint, GLuint, GLuint);
void      (APIENTRY *glSecondaryColor3uiv) (const GLuint *);
void      (APIENTRY *glSecondaryColor3us) (GLushort, GLushort, GLushort);
void      (APIENTRY *glSecondaryColor3usv) (const GLushort *);
void      (APIENTRY *glSecondaryColorPointer) (GLint, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glWindowPos2d) (GLdouble, GLdouble);
void      (APIENTRY *glWindowPos2dv) (const GLdouble *);
void      (APIENTRY *glWindowPos2f) (GLfloat, GLfloat);
void      (APIENTRY *glWindowPos2fv) (const GLfloat *);
void      (APIENTRY *glWindowPos2i) (GLint, GLint);
void      (APIENTRY *glWindowPos2iv) (const GLint *);
void      (APIENTRY *glWindowPos2s) (GLshort, GLshort);
void      (APIENTRY *glWindowPos2sv) (const GLshort *);
void      (APIENTRY *glWindowPos3d) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glWindowPos3dv) (const GLdouble *);
void      (APIENTRY *glWindowPos3f) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glWindowPos3fv) (const GLfloat *);
void      (APIENTRY *glWindowPos3i) (GLint, GLint, GLint);
void      (APIENTRY *glWindowPos3iv) (const GLint *);
void      (APIENTRY *glWindowPos3s) (GLshort, GLshort, GLshort);
void      (APIENTRY *glWindowPos3sv) (const GLshort *);
//@}

/** @name GL_VERSION_1_5
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/1_5.txt
 */
//@{
void      (APIENTRY *glGenQueries) (GLsizei, GLuint *);
void      (APIENTRY *glDeleteQueries) (GLsizei, const GLuint *);
GLboolean (APIENTRY *glIsQuery) (GLuint);
void      (APIENTRY *glBeginQuery) (GLenum, GLuint);
void      (APIENTRY *glEndQuery) (GLenum);
void      (APIENTRY *glGetQueryiv) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetQueryObjectiv) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetQueryObjectuiv) (GLuint, GLenum, GLuint *);
void      (APIENTRY *glBindBuffer) (GLenum, GLuint);
void      (APIENTRY *glDeleteBuffers) (GLsizei, const GLuint *);
void      (APIENTRY *glGenBuffers) (GLsizei, GLuint *);
GLboolean (APIENTRY *glIsBuffer) (GLuint);
void      (APIENTRY *glBufferData) (GLenum, GLsizeiptr, const GLvoid *, GLenum);
void      (APIENTRY *glBufferSubData) (GLenum, GLintptr, GLsizeiptr, const GLvoid *);
void      (APIENTRY *glGetBufferSubData) (GLenum, GLintptr, GLsizeiptr, GLvoid *);
GLvoid*   (APIENTRY *glMapBuffer) (GLenum, GLenum);
GLboolean (APIENTRY *glUnmapBuffer) (GLenum);
void      (APIENTRY *glGetBufferParameteriv) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetBufferPointerv) (GLenum, GLenum, GLvoid* *);
//@}

/** @name GL_VERSION_2_0
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/2_0.txt
 */
//@{
void      (APIENTRY *glBlendEquationSeparate) (GLenum, GLenum);
void      (APIENTRY *glDrawBuffers) (GLsizei, const GLenum *);
void      (APIENTRY *glStencilOpSeparate) (GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glStencilFuncSeparate) (GLenum, GLenum, GLint, GLuint);
void      (APIENTRY *glStencilMaskSeparate) (GLenum, GLuint);
void      (APIENTRY *glAttachShader) (GLuint, GLuint);
void      (APIENTRY *glBindAttribLocation) (GLuint, GLuint, const GLchar *);
void      (APIENTRY *glCompileShader) (GLuint);
GLuint    (APIENTRY *glCreateProgram) (void);
GLuint    (APIENTRY *glCreateShader) (GLenum);
void      (APIENTRY *glDeleteProgram) (GLuint);
void      (APIENTRY *glDeleteShader) (GLuint);
void      (APIENTRY *glDetachShader) (GLuint, GLuint);
void      (APIENTRY *glDisableVertexAttribArray) (GLuint);
void      (APIENTRY *glEnableVertexAttribArray) (GLuint);
void      (APIENTRY *glGetActiveAttrib) (GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
void      (APIENTRY *glGetActiveUniform) (GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
void      (APIENTRY *glGetAttachedShaders) (GLuint, GLsizei, GLsizei *, GLuint *);
GLint     (APIENTRY *glGetAttribLocation) (GLuint, const GLchar *);
void      (APIENTRY *glGetProgramiv) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetProgramInfoLog) (GLuint, GLsizei, GLsizei *, GLchar *);
void      (APIENTRY *glGetShaderiv) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetShaderInfoLog) (GLuint, GLsizei, GLsizei *, GLchar *);
void      (APIENTRY *glGetShaderSource) (GLuint, GLsizei, GLsizei *, GLchar *);
GLint     (APIENTRY *glGetUniformLocation) (GLuint, const GLchar *);
void      (APIENTRY *glGetUniformfv) (GLuint, GLint, GLfloat *);
void      (APIENTRY *glGetUniformiv) (GLuint, GLint, GLint *);
void      (APIENTRY *glGetVertexAttribdv) (GLuint, GLenum, GLdouble *);
void      (APIENTRY *glGetVertexAttribfv) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetVertexAttribiv) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetVertexAttribPointerv) (GLuint, GLenum, GLvoid* *);
GLboolean (APIENTRY *glIsProgram) (GLuint);
GLboolean (APIENTRY *glIsShader) (GLuint);
void      (APIENTRY *glLinkProgram) (GLuint);
void      (APIENTRY *glShaderSource) (GLuint, GLsizei, const GLchar* *, const GLint *);
void      (APIENTRY *glUseProgram) (GLuint);
void      (APIENTRY *glUniform1f) (GLint, GLfloat);
void      (APIENTRY *glUniform2f) (GLint, GLfloat, GLfloat);
void      (APIENTRY *glUniform3f) (GLint, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glUniform4f) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glUniform1i) (GLint, GLint);
void      (APIENTRY *glUniform2i) (GLint, GLint, GLint);
void      (APIENTRY *glUniform3i) (GLint, GLint, GLint, GLint);
void      (APIENTRY *glUniform4i) (GLint, GLint, GLint, GLint, GLint);
void      (APIENTRY *glUniform1fv) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform2fv) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform3fv) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform4fv) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform1iv) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniform2iv) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniform3iv) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniform4iv) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniformMatrix2fv) (GLint, GLsizei, GLboolean, const GLfloat *);
void      (APIENTRY *glUniformMatrix3fv) (GLint, GLsizei, GLboolean, const GLfloat *);
void      (APIENTRY *glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat *);
void      (APIENTRY *glValidateProgram) (GLuint);
void      (APIENTRY *glVertexAttrib1d) (GLuint, GLdouble);
void      (APIENTRY *glVertexAttrib1dv) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib1f) (GLuint, GLfloat);
void      (APIENTRY *glVertexAttrib1fv) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib1s) (GLuint, GLshort);
void      (APIENTRY *glVertexAttrib1sv) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib2d) (GLuint, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib2dv) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib2f) (GLuint, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib2fv) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib2s) (GLuint, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib2sv) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib3d) (GLuint, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib3dv) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib3f) (GLuint, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib3fv) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib3s) (GLuint, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib3sv) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4Nbv) (GLuint, const GLbyte *);
void      (APIENTRY *glVertexAttrib4Niv) (GLuint, const GLint *);
void      (APIENTRY *glVertexAttrib4Nsv) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4Nub) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
void      (APIENTRY *glVertexAttrib4Nubv) (GLuint, const GLubyte *);
void      (APIENTRY *glVertexAttrib4Nuiv) (GLuint, const GLuint *);
void      (APIENTRY *glVertexAttrib4Nusv) (GLuint, const GLushort *);
void      (APIENTRY *glVertexAttrib4bv) (GLuint, const GLbyte *);
void      (APIENTRY *glVertexAttrib4d) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib4dv) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib4f) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib4fv) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib4iv) (GLuint, const GLint *);
void      (APIENTRY *glVertexAttrib4s) (GLuint, GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib4sv) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4ubv) (GLuint, const GLubyte *);
void      (APIENTRY *glVertexAttrib4uiv) (GLuint, const GLuint *);
void      (APIENTRY *glVertexAttrib4usv) (GLuint, const GLushort *);
void      (APIENTRY *glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
//@}

/** @name GL_ARB_multitexture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/multitexture.txt
 */
//@{
void      (APIENTRY *glActiveTextureARB) (GLenum);
void      (APIENTRY *glClientActiveTextureARB) (GLenum);
void      (APIENTRY *glMultiTexCoord1dARB) (GLenum, GLdouble);
void      (APIENTRY *glMultiTexCoord1dvARB) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord1fARB) (GLenum, GLfloat);
void      (APIENTRY *glMultiTexCoord1fvARB) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord1iARB) (GLenum, GLint);
void      (APIENTRY *glMultiTexCoord1ivARB) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord1sARB) (GLenum, GLshort);
void      (APIENTRY *glMultiTexCoord1svARB) (GLenum, const GLshort *);
void      (APIENTRY *glMultiTexCoord2dARB) (GLenum, GLdouble, GLdouble);
void      (APIENTRY *glMultiTexCoord2dvARB) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord2fARB) (GLenum, GLfloat, GLfloat);
void      (APIENTRY *glMultiTexCoord2fvARB) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord2iARB) (GLenum, GLint, GLint);
void      (APIENTRY *glMultiTexCoord2ivARB) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord2sARB) (GLenum, GLshort, GLshort);
void      (APIENTRY *glMultiTexCoord2svARB) (GLenum, const GLshort *);
void      (APIENTRY *glMultiTexCoord3dARB) (GLenum, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glMultiTexCoord3dvARB) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord3fARB) (GLenum, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glMultiTexCoord3fvARB) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord3iARB) (GLenum, GLint, GLint, GLint);
void      (APIENTRY *glMultiTexCoord3ivARB) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord3sARB) (GLenum, GLshort, GLshort, GLshort);
void      (APIENTRY *glMultiTexCoord3svARB) (GLenum, const GLshort *);
void      (APIENTRY *glMultiTexCoord4dARB) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glMultiTexCoord4dvARB) (GLenum, const GLdouble *);
void      (APIENTRY *glMultiTexCoord4fARB) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glMultiTexCoord4fvARB) (GLenum, const GLfloat *);
void      (APIENTRY *glMultiTexCoord4iARB) (GLenum, GLint, GLint, GLint, GLint);
void      (APIENTRY *glMultiTexCoord4ivARB) (GLenum, const GLint *);
void      (APIENTRY *glMultiTexCoord4sARB) (GLenum, GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glMultiTexCoord4svARB) (GLenum, const GLshort *);
//@}

/** @name GL_ARB_transpose_matrix
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/transpose_matrix.txt
 */
//@{
void      (APIENTRY *glLoadTransposeMatrixfARB) (const GLfloat *);
void      (APIENTRY *glLoadTransposeMatrixdARB) (const GLdouble *);
void      (APIENTRY *glMultTransposeMatrixfARB) (const GLfloat *);
void      (APIENTRY *glMultTransposeMatrixdARB) (const GLdouble *);
//@}

/** @name GL_ARB_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/multisample.txt
 */
//@{
void      (APIENTRY *glSampleCoverageARB) (GLclampf, GLboolean);
//@}

/** @name GL_ARB_texture_compression
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_compression.txt
 */
//@{
void      (APIENTRY *glCompressedTexImage3DARB) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexImage2DARB) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexImage1DARB) (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexSubImage3DARB) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexSubImage2DARB) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glCompressedTexSubImage1DARB) (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glGetCompressedTexImageARB) (GLenum, GLint, GLvoid *);
//@}

/** @name GL_ARB_point_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/point_parameters.txt
 */
//@{
void      (APIENTRY *glPointParameterfARB) (GLenum, GLfloat);
void      (APIENTRY *glPointParameterfvARB) (GLenum, const GLfloat *);
//@}

/** @name GL_ARB_vertex_blend
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_blend.txt
 */
//@{
void      (APIENTRY *glWeightbvARB) (GLint, const GLbyte *);
void      (APIENTRY *glWeightsvARB) (GLint, const GLshort *);
void      (APIENTRY *glWeightivARB) (GLint, const GLint *);
void      (APIENTRY *glWeightfvARB) (GLint, const GLfloat *);
void      (APIENTRY *glWeightdvARB) (GLint, const GLdouble *);
void      (APIENTRY *glWeightubvARB) (GLint, const GLubyte *);
void      (APIENTRY *glWeightusvARB) (GLint, const GLushort *);
void      (APIENTRY *glWeightuivARB) (GLint, const GLuint *);
void      (APIENTRY *glWeightPointerARB) (GLint, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glVertexBlendARB) (GLint);
//@}

/** @name GL_ARB_matrix_palette
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/matrix_palette.txt
 */
//@{
void      (APIENTRY *glCurrentPaletteMatrixARB) (GLint);
void      (APIENTRY *glMatrixIndexubvARB) (GLint, const GLubyte *);
void      (APIENTRY *glMatrixIndexusvARB) (GLint, const GLushort *);
void      (APIENTRY *glMatrixIndexuivARB) (GLint, const GLuint *);
void      (APIENTRY *glMatrixIndexPointerARB) (GLint, GLenum, GLsizei, const GLvoid *);
//@}

/** @name GL_ARB_window_pos
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/window_pos.txt
 */
//@{
void      (APIENTRY *glWindowPos2dARB) (GLdouble, GLdouble);
void      (APIENTRY *glWindowPos2dvARB) (const GLdouble *);
void      (APIENTRY *glWindowPos2fARB) (GLfloat, GLfloat);
void      (APIENTRY *glWindowPos2fvARB) (const GLfloat *);
void      (APIENTRY *glWindowPos2iARB) (GLint, GLint);
void      (APIENTRY *glWindowPos2ivARB) (const GLint *);
void      (APIENTRY *glWindowPos2sARB) (GLshort, GLshort);
void      (APIENTRY *glWindowPos2svARB) (const GLshort *);
void      (APIENTRY *glWindowPos3dARB) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glWindowPos3dvARB) (const GLdouble *);
void      (APIENTRY *glWindowPos3fARB) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glWindowPos3fvARB) (const GLfloat *);
void      (APIENTRY *glWindowPos3iARB) (GLint, GLint, GLint);
void      (APIENTRY *glWindowPos3ivARB) (const GLint *);
void      (APIENTRY *glWindowPos3sARB) (GLshort, GLshort, GLshort);
void      (APIENTRY *glWindowPos3svARB) (const GLshort *);
//@}

/** @name GL_ARB_vertex_program
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_program.txt
 */
//@{
void      (APIENTRY *glVertexAttrib1dARB) (GLuint, GLdouble);
void      (APIENTRY *glVertexAttrib1dvARB) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib1fARB) (GLuint, GLfloat);
void      (APIENTRY *glVertexAttrib1fvARB) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib1sARB) (GLuint, GLshort);
void      (APIENTRY *glVertexAttrib1svARB) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib2dARB) (GLuint, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib2dvARB) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib2fARB) (GLuint, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib2fvARB) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib2sARB) (GLuint, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib2svARB) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib3dARB) (GLuint, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib3dvARB) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib3fARB) (GLuint, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib3fvARB) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib3sARB) (GLuint, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib3svARB) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4NbvARB) (GLuint, const GLbyte *);
void      (APIENTRY *glVertexAttrib4NivARB) (GLuint, const GLint *);
void      (APIENTRY *glVertexAttrib4NsvARB) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4NubARB) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
void      (APIENTRY *glVertexAttrib4NubvARB) (GLuint, const GLubyte *);
void      (APIENTRY *glVertexAttrib4NuivARB) (GLuint, const GLuint *);
void      (APIENTRY *glVertexAttrib4NusvARB) (GLuint, const GLushort *);
void      (APIENTRY *glVertexAttrib4bvARB) (GLuint, const GLbyte *);
void      (APIENTRY *glVertexAttrib4dARB) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib4dvARB) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib4fARB) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib4fvARB) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib4ivARB) (GLuint, const GLint *);
void      (APIENTRY *glVertexAttrib4sARB) (GLuint, GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib4svARB) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4ubvARB) (GLuint, const GLubyte *);
void      (APIENTRY *glVertexAttrib4uivARB) (GLuint, const GLuint *);
void      (APIENTRY *glVertexAttrib4usvARB) (GLuint, const GLushort *);
void      (APIENTRY *glVertexAttribPointerARB) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
void      (APIENTRY *glEnableVertexAttribArrayARB) (GLuint);
void      (APIENTRY *glDisableVertexAttribArrayARB) (GLuint);
void      (APIENTRY *glProgramStringARB) (GLenum, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glBindProgramARB) (GLenum, GLuint);
void      (APIENTRY *glDeleteProgramsARB) (GLsizei, const GLuint *);
void      (APIENTRY *glGenProgramsARB) (GLsizei, GLuint *);
void      (APIENTRY *glProgramEnvParameter4dARB) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glProgramEnvParameter4dvARB) (GLenum, GLuint, const GLdouble *);
void      (APIENTRY *glProgramEnvParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glProgramEnvParameter4fvARB) (GLenum, GLuint, const GLfloat *);
void      (APIENTRY *glProgramLocalParameter4dARB) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glProgramLocalParameter4dvARB) (GLenum, GLuint, const GLdouble *);
void      (APIENTRY *glProgramLocalParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glProgramLocalParameter4fvARB) (GLenum, GLuint, const GLfloat *);
void      (APIENTRY *glGetProgramEnvParameterdvARB) (GLenum, GLuint, GLdouble *);
void      (APIENTRY *glGetProgramEnvParameterfvARB) (GLenum, GLuint, GLfloat *);
void      (APIENTRY *glGetProgramLocalParameterdvARB) (GLenum, GLuint, GLdouble *);
void      (APIENTRY *glGetProgramLocalParameterfvARB) (GLenum, GLuint, GLfloat *);
void      (APIENTRY *glGetProgramivARB) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetProgramStringARB) (GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetVertexAttribdvARB) (GLuint, GLenum, GLdouble *);
void      (APIENTRY *glGetVertexAttribfvARB) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetVertexAttribivARB) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetVertexAttribPointervARB) (GLuint, GLenum, GLvoid* *);
GLboolean (APIENTRY *glIsProgramARB) (GLuint);
//@}

/** @name GL_ARB_vertex_buffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_buffer_object.txt
 */
//@{
void      (APIENTRY *glBindBufferARB) (GLenum, GLuint);
void      (APIENTRY *glDeleteBuffersARB) (GLsizei, const GLuint *);
void      (APIENTRY *glGenBuffersARB) (GLsizei, GLuint *);
GLboolean (APIENTRY *glIsBufferARB) (GLuint);
void      (APIENTRY *glBufferDataARB) (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
void      (APIENTRY *glBufferSubDataARB) (GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *);
void      (APIENTRY *glGetBufferSubDataARB) (GLenum, GLintptrARB, GLsizeiptrARB, GLvoid *);
GLvoid*   (APIENTRY *glMapBufferARB) (GLenum, GLenum);
GLboolean (APIENTRY *glUnmapBufferARB) (GLenum);
void      (APIENTRY *glGetBufferParameterivARB) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetBufferPointervARB) (GLenum, GLenum, GLvoid* *);
//@}

/** @name GL_ARB_occlusion_query
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/occlusion_query.txt
 */
//@{
void      (APIENTRY *glGenQueriesARB) (GLsizei, GLuint *);
void      (APIENTRY *glDeleteQueriesARB) (GLsizei, const GLuint *);
GLboolean (APIENTRY *glIsQueryARB) (GLuint);
void      (APIENTRY *glBeginQueryARB) (GLenum, GLuint);
void      (APIENTRY *glEndQueryARB) (GLenum);
void      (APIENTRY *glGetQueryivARB) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetQueryObjectivARB) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetQueryObjectuivARB) (GLuint, GLenum, GLuint *);
//@}

/** @name GL_ARB_shader_objects
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/shader_objects.txt
 */
//@{
void      (APIENTRY *glDeleteObjectARB) (GLhandleARB);
GLhandleARB (APIENTRY *glGetHandleARB) (GLenum);
void      (APIENTRY *glDetachObjectARB) (GLhandleARB, GLhandleARB);
GLhandleARB (APIENTRY *glCreateShaderObjectARB) (GLenum);
void      (APIENTRY *glShaderSourceARB) (GLhandleARB, GLsizei, const GLcharARB* *, const GLint *);
void      (APIENTRY *glCompileShaderARB) (GLhandleARB);
GLhandleARB (APIENTRY *glCreateProgramObjectARB) (void);
void      (APIENTRY *glAttachObjectARB) (GLhandleARB, GLhandleARB);
void      (APIENTRY *glLinkProgramARB) (GLhandleARB);
void      (APIENTRY *glUseProgramObjectARB) (GLhandleARB);
void      (APIENTRY *glValidateProgramARB) (GLhandleARB);
void      (APIENTRY *glUniform1fARB) (GLint, GLfloat);
void      (APIENTRY *glUniform2fARB) (GLint, GLfloat, GLfloat);
void      (APIENTRY *glUniform3fARB) (GLint, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glUniform4fARB) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glUniform1iARB) (GLint, GLint);
void      (APIENTRY *glUniform2iARB) (GLint, GLint, GLint);
void      (APIENTRY *glUniform3iARB) (GLint, GLint, GLint, GLint);
void      (APIENTRY *glUniform4iARB) (GLint, GLint, GLint, GLint, GLint);
void      (APIENTRY *glUniform1fvARB) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform2fvARB) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform3fvARB) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform4fvARB) (GLint, GLsizei, const GLfloat *);
void      (APIENTRY *glUniform1ivARB) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniform2ivARB) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniform3ivARB) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniform4ivARB) (GLint, GLsizei, const GLint *);
void      (APIENTRY *glUniformMatrix2fvARB) (GLint, GLsizei, GLboolean, const GLfloat *);
void      (APIENTRY *glUniformMatrix3fvARB) (GLint, GLsizei, GLboolean, const GLfloat *);
void      (APIENTRY *glUniformMatrix4fvARB) (GLint, GLsizei, GLboolean, const GLfloat *);
void      (APIENTRY *glGetObjectParameterfvARB) (GLhandleARB, GLenum, GLfloat *);
void      (APIENTRY *glGetObjectParameterivARB) (GLhandleARB, GLenum, GLint *);
void      (APIENTRY *glGetInfoLogARB) (GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
void      (APIENTRY *glGetAttachedObjectsARB) (GLhandleARB, GLsizei, GLsizei *, GLhandleARB *);
GLint     (APIENTRY *glGetUniformLocationARB) (GLhandleARB, const GLcharARB *);
void      (APIENTRY *glGetActiveUniformARB) (GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
void      (APIENTRY *glGetUniformfvARB) (GLhandleARB, GLint, GLfloat *);
void      (APIENTRY *glGetUniformivARB) (GLhandleARB, GLint, GLint *);
void      (APIENTRY *glGetShaderSourceARB) (GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
//@}

/** @name GL_ARB_vertex_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_shader.txt
 */
//@{
void      (APIENTRY *glBindAttribLocationARB) (GLhandleARB, GLuint, const GLcharARB *);
void      (APIENTRY *glGetActiveAttribARB) (GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
GLint     (APIENTRY *glGetAttribLocationARB) (GLhandleARB, const GLcharARB *);
//@}

/** @name GL_ARB_draw_buffers
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/draw_buffers.txt
 */
//@{
void      (APIENTRY *glDrawBuffersARB) (GLsizei, const GLenum *);
//@}

/** @name GL_ARB_color_buffer_float
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/color_buffer_float.txt
 */
//@{
void      (APIENTRY *glClampColorARB) (GLenum, GLenum);
//@}

/** @name GL_EXT_blend_color
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_color.txt
 */
//@{
void      (APIENTRY *glBlendColorEXT) (GLclampf, GLclampf, GLclampf, GLclampf);
//@}

/** @name GL_EXT_polygon_offset
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/polygon_offset.txt
 */
//@{
void      (APIENTRY *glPolygonOffsetEXT) (GLfloat, GLfloat);
//@}

/** @name GL_EXT_texture3D
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture3D.txt
 */
//@{
void      (APIENTRY *glTexImage3DEXT) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glTexSubImage3DEXT) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
//@}

/** @name GL_SGIS_texture_filter4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture_filter4.txt
 */
//@{
void      (APIENTRY *glGetTexFilterFuncSGIS) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glTexFilterFuncSGIS) (GLenum, GLenum, GLsizei, const GLfloat *);
//@}

/** @name GL_EXT_subtexture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/subtexture.txt
 */
//@{
void      (APIENTRY *glTexSubImage1DEXT) (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glTexSubImage2DEXT) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
//@}

/** @name GL_EXT_copy_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/copy_texture.txt
 */
//@{
void      (APIENTRY *glCopyTexImage1DEXT) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
void      (APIENTRY *glCopyTexImage2DEXT) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
void      (APIENTRY *glCopyTexSubImage1DEXT) (GLenum, GLint, GLint, GLint, GLint, GLsizei);
void      (APIENTRY *glCopyTexSubImage2DEXT) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
void      (APIENTRY *glCopyTexSubImage3DEXT) (GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
//@}

/** @name GL_EXT_histogram
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/histogram.txt
 */
//@{
void      (APIENTRY *glGetHistogramEXT) (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetHistogramParameterfvEXT) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetHistogramParameterivEXT) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetMinmaxEXT) (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetMinmaxParameterfvEXT) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetMinmaxParameterivEXT) (GLenum, GLenum, GLint *);
void      (APIENTRY *glHistogramEXT) (GLenum, GLsizei, GLenum, GLboolean);
void      (APIENTRY *glMinmaxEXT) (GLenum, GLenum, GLboolean);
void      (APIENTRY *glResetHistogramEXT) (GLenum);
void      (APIENTRY *glResetMinmaxEXT) (GLenum);
//@}

/** @name GL_EXT_convolution
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/convolution.txt
 */
//@{
void      (APIENTRY *glConvolutionFilter1DEXT) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glConvolutionFilter2DEXT) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glConvolutionParameterfEXT) (GLenum, GLenum, GLfloat);
void      (APIENTRY *glConvolutionParameterfvEXT) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glConvolutionParameteriEXT) (GLenum, GLenum, GLint);
void      (APIENTRY *glConvolutionParameterivEXT) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glCopyConvolutionFilter1DEXT) (GLenum, GLenum, GLint, GLint, GLsizei);
void      (APIENTRY *glCopyConvolutionFilter2DEXT) (GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
void      (APIENTRY *glGetConvolutionFilterEXT) (GLenum, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetConvolutionParameterfvEXT) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetConvolutionParameterivEXT) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetSeparableFilterEXT) (GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid *);
void      (APIENTRY *glSeparableFilter2DEXT) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid *);
//@}

/** @name GL_SGI_color_table
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGI/color_table.txt
 */
//@{
void      (APIENTRY *glColorTableSGI) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glColorTableParameterfvSGI) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glColorTableParameterivSGI) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glCopyColorTableSGI) (GLenum, GLenum, GLint, GLint, GLsizei);
void      (APIENTRY *glGetColorTableSGI) (GLenum, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetColorTableParameterfvSGI) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetColorTableParameterivSGI) (GLenum, GLenum, GLint *);
//@}

/** @name GL_SGIX_pixel_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/pixel_texture.txt
 */
//@{
void      (APIENTRY *glPixelTexGenSGIX) (GLenum);
//@}

/** @name GL_SGIS_pixel_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/pixel_texture.txt
 */
//@{
void      (APIENTRY *glPixelTexGenParameteriSGIS) (GLenum, GLint);
void      (APIENTRY *glPixelTexGenParameterivSGIS) (GLenum, const GLint *);
void      (APIENTRY *glPixelTexGenParameterfSGIS) (GLenum, GLfloat);
void      (APIENTRY *glPixelTexGenParameterfvSGIS) (GLenum, const GLfloat *);
void      (APIENTRY *glGetPixelTexGenParameterivSGIS) (GLenum, GLint *);
void      (APIENTRY *glGetPixelTexGenParameterfvSGIS) (GLenum, GLfloat *);
//@}

/** @name GL_SGIS_texture4D
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture4D.txt
 */
//@{
void      (APIENTRY *glTexImage4DSGIS) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glTexSubImage4DSGIS) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
//@}

/** @name GL_EXT_texture_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_object.txt
 */
//@{
GLboolean (APIENTRY *glAreTexturesResidentEXT) (GLsizei, const GLuint *, GLboolean *);
void      (APIENTRY *glBindTextureEXT) (GLenum, GLuint);
void      (APIENTRY *glDeleteTexturesEXT) (GLsizei, const GLuint *);
void      (APIENTRY *glGenTexturesEXT) (GLsizei, GLuint *);
GLboolean (APIENTRY *glIsTextureEXT) (GLuint);
void      (APIENTRY *glPrioritizeTexturesEXT) (GLsizei, const GLuint *, const GLclampf *);
//@}

/** @name GL_SGIS_detail_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/detail_texture.txt
 */
//@{
void      (APIENTRY *glDetailTexFuncSGIS) (GLenum, GLsizei, const GLfloat *);
void      (APIENTRY *glGetDetailTexFuncSGIS) (GLenum, GLfloat *);
//@}

/** @name GL_SGIS_sharpen_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/sharpen_texture.txt
 */
//@{
void      (APIENTRY *glSharpenTexFuncSGIS) (GLenum, GLsizei, const GLfloat *);
void      (APIENTRY *glGetSharpenTexFuncSGIS) (GLenum, GLfloat *);
//@}

/** @name GL_SGIS_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/multisample.txt
 */
//@{
void      (APIENTRY *glSampleMaskSGIS) (GLclampf, GLboolean);
void      (APIENTRY *glSamplePatternSGIS) (GLenum);
//@}

/** @name GL_EXT_vertex_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_array.txt
 */
//@{
void      (APIENTRY *glArrayElementEXT) (GLint);
void      (APIENTRY *glColorPointerEXT) (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
void      (APIENTRY *glDrawArraysEXT) (GLenum, GLint, GLsizei);
void      (APIENTRY *glEdgeFlagPointerEXT) (GLsizei, GLsizei, const GLboolean *);
void      (APIENTRY *glGetPointervEXT) (GLenum, GLvoid* *);
void      (APIENTRY *glIndexPointerEXT) (GLenum, GLsizei, GLsizei, const GLvoid *);
void      (APIENTRY *glNormalPointerEXT) (GLenum, GLsizei, GLsizei, const GLvoid *);
void      (APIENTRY *glTexCoordPointerEXT) (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
void      (APIENTRY *glVertexPointerEXT) (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
//@}

/** @name GL_EXT_blend_minmax
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_minmax.txt
 */
//@{
void      (APIENTRY *glBlendEquationEXT) (GLenum);
//@}

/** @name GL_SGIX_sprite
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/sprite.txt
 */
//@{
void      (APIENTRY *glSpriteParameterfSGIX) (GLenum, GLfloat);
void      (APIENTRY *glSpriteParameterfvSGIX) (GLenum, const GLfloat *);
void      (APIENTRY *glSpriteParameteriSGIX) (GLenum, GLint);
void      (APIENTRY *glSpriteParameterivSGIX) (GLenum, const GLint *);
//@}

/** @name GL_EXT_point_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/point_parameters.txt
 */
//@{
void      (APIENTRY *glPointParameterfEXT) (GLenum, GLfloat);
void      (APIENTRY *glPointParameterfvEXT) (GLenum, const GLfloat *);
//@}

/** @name GL_SGIS_point_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/point_parameters.txt
 */
//@{
void      (APIENTRY *glPointParameterfSGIS) (GLenum, GLfloat);
void      (APIENTRY *glPointParameterfvSGIS) (GLenum, const GLfloat *);
//@}

/** @name GL_SGIX_instruments
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/instruments.txt
 */
//@{
GLint     (APIENTRY *glGetInstrumentsSGIX) (void);
void      (APIENTRY *glInstrumentsBufferSGIX) (GLsizei, GLint *);
GLint     (APIENTRY *glPollInstrumentsSGIX) (GLint *);
void      (APIENTRY *glReadInstrumentsSGIX) (GLint);
void      (APIENTRY *glStartInstrumentsSGIX) (void);
void      (APIENTRY *glStopInstrumentsSGIX) (GLint);
//@}

/** @name GL_SGIX_framezoom
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/framezoom.txt
 */
//@{
void      (APIENTRY *glFrameZoomSGIX) (GLint);
//@}

/** @name GL_SGIX_tag_sample_buffer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/tag_sample_buffer.txt
 */
//@{
void      (APIENTRY *glTagSampleBufferSGIX) (void);
//@}

/** @name GL_SGIX_polynomial_ffd
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/polynomial_ffd.txt
 */
//@{
void      (APIENTRY *glDeformationMap3dSGIX) (GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *);
void      (APIENTRY *glDeformationMap3fSGIX) (GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *);
void      (APIENTRY *glDeformSGIX) (GLbitfield);
void      (APIENTRY *glLoadIdentityDeformationMapSGIX) (GLbitfield);
//@}

/** @name GL_SGIX_reference_plane
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/reference_plane.txt
 */
//@{
void      (APIENTRY *glReferencePlaneSGIX) (const GLdouble *);
//@}

/** @name GL_SGIX_flush_raster
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/flush_raster.txt
 */
//@{
void      (APIENTRY *glFlushRasterSGIX) (void);
//@}

/** @name GL_SGIS_fog_function
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/fog_function.txt
 */
//@{
void      (APIENTRY *glFogFuncSGIS) (GLsizei, const GLfloat *);
void      (APIENTRY *glGetFogFuncSGIS) (GLfloat *);
//@}

/** @name GL_HP_image_transform
 *  See http://oss.sgi.com/projects/ogl-sample/registry/HP/image_transform.txt
 */
//@{
void      (APIENTRY *glImageTransformParameteriHP) (GLenum, GLenum, GLint);
void      (APIENTRY *glImageTransformParameterfHP) (GLenum, GLenum, GLfloat);
void      (APIENTRY *glImageTransformParameterivHP) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glImageTransformParameterfvHP) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glGetImageTransformParameterivHP) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetImageTransformParameterfvHP) (GLenum, GLenum, GLfloat *);
//@}

/** @name GL_EXT_color_subtable
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/color_subtable.txt
 */
//@{
void      (APIENTRY *glColorSubTableEXT) (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glCopyColorSubTableEXT) (GLenum, GLsizei, GLint, GLint, GLsizei);
//@}

/** @name GL_PGI_misc_hints
 *  See http://oss.sgi.com/projects/ogl-sample/registry/PGI/misc_hints.txt
 */
//@{
void      (APIENTRY *glHintPGI) (GLenum, GLint);
//@}

/** @name GL_EXT_paletted_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/paletted_texture.txt
 */
//@{
void      (APIENTRY *glColorTableEXT) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
void      (APIENTRY *glGetColorTableEXT) (GLenum, GLenum, GLenum, GLvoid *);
void      (APIENTRY *glGetColorTableParameterivEXT) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetColorTableParameterfvEXT) (GLenum, GLenum, GLfloat *);
//@}

/** @name GL_SGIX_list_priority
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/list_priority.txt
 */
//@{
void      (APIENTRY *glGetListParameterfvSGIX) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetListParameterivSGIX) (GLuint, GLenum, GLint *);
void      (APIENTRY *glListParameterfSGIX) (GLuint, GLenum, GLfloat);
void      (APIENTRY *glListParameterfvSGIX) (GLuint, GLenum, const GLfloat *);
void      (APIENTRY *glListParameteriSGIX) (GLuint, GLenum, GLint);
void      (APIENTRY *glListParameterivSGIX) (GLuint, GLenum, const GLint *);
//@}

/** @name GL_EXT_index_material
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/index_material.txt
 */
//@{
void      (APIENTRY *glIndexMaterialEXT) (GLenum, GLenum);
//@}

/** @name GL_EXT_index_func
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/index_func.txt
 */
//@{
void      (APIENTRY *glIndexFuncEXT) (GLenum, GLclampf);
//@}

/** @name GL_EXT_compiled_vertex_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/compiled_vertex_array.txt
 */
//@{
void      (APIENTRY *glLockArraysEXT) (GLint, GLsizei);
void      (APIENTRY *glUnlockArraysEXT) (void);
//@}

/** @name GL_EXT_cull_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/cull_vertex.txt
 */
//@{
void      (APIENTRY *glCullParameterdvEXT) (GLenum, GLdouble *);
void      (APIENTRY *glCullParameterfvEXT) (GLenum, GLfloat *);
//@}

/** @name GL_SGIX_fragment_lighting
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/fragment_lighting.txt
 */
//@{
void      (APIENTRY *glFragmentColorMaterialSGIX) (GLenum, GLenum);
void      (APIENTRY *glFragmentLightfSGIX) (GLenum, GLenum, GLfloat);
void      (APIENTRY *glFragmentLightfvSGIX) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glFragmentLightiSGIX) (GLenum, GLenum, GLint);
void      (APIENTRY *glFragmentLightivSGIX) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glFragmentLightModelfSGIX) (GLenum, GLfloat);
void      (APIENTRY *glFragmentLightModelfvSGIX) (GLenum, const GLfloat *);
void      (APIENTRY *glFragmentLightModeliSGIX) (GLenum, GLint);
void      (APIENTRY *glFragmentLightModelivSGIX) (GLenum, const GLint *);
void      (APIENTRY *glFragmentMaterialfSGIX) (GLenum, GLenum, GLfloat);
void      (APIENTRY *glFragmentMaterialfvSGIX) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glFragmentMaterialiSGIX) (GLenum, GLenum, GLint);
void      (APIENTRY *glFragmentMaterialivSGIX) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glGetFragmentLightfvSGIX) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetFragmentLightivSGIX) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetFragmentMaterialfvSGIX) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetFragmentMaterialivSGIX) (GLenum, GLenum, GLint *);
void      (APIENTRY *glLightEnviSGIX) (GLenum, GLint);
//@}

/** @name GL_EXT_draw_range_elements
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/draw_range_elements.txt
 */
//@{
void      (APIENTRY *glDrawRangeElementsEXT) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);
//@}

/** @name GL_EXT_light_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/light_texture.txt
 */
//@{
void      (APIENTRY *glApplyTextureEXT) (GLenum);
void      (APIENTRY *glTextureLightEXT) (GLenum);
void      (APIENTRY *glTextureMaterialEXT) (GLenum, GLenum);
//@}

/** @name GL_SGIX_async
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/async.txt
 */
//@{
void      (APIENTRY *glAsyncMarkerSGIX) (GLuint);
GLint     (APIENTRY *glFinishAsyncSGIX) (GLuint *);
GLint     (APIENTRY *glPollAsyncSGIX) (GLuint *);
GLuint    (APIENTRY *glGenAsyncMarkersSGIX) (GLsizei);
void      (APIENTRY *glDeleteAsyncMarkersSGIX) (GLuint, GLsizei);
GLboolean (APIENTRY *glIsAsyncMarkerSGIX) (GLuint);
//@}

/** @name GL_INTEL_parallel_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/INTEL/parallel_arrays.txt
 */
//@{
void      (APIENTRY *glVertexPointervINTEL) (GLint, GLenum, const GLvoid* *);
void      (APIENTRY *glNormalPointervINTEL) (GLenum, const GLvoid* *);
void      (APIENTRY *glColorPointervINTEL) (GLint, GLenum, const GLvoid* *);
void      (APIENTRY *glTexCoordPointervINTEL) (GLint, GLenum, const GLvoid* *);
//@}

/** @name GL_EXT_pixel_transform
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/pixel_transform.txt
 */
//@{
void      (APIENTRY *glPixelTransformParameteriEXT) (GLenum, GLenum, GLint);
void      (APIENTRY *glPixelTransformParameterfEXT) (GLenum, GLenum, GLfloat);
void      (APIENTRY *glPixelTransformParameterivEXT) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glPixelTransformParameterfvEXT) (GLenum, GLenum, const GLfloat *);
//@}

/** @name GL_EXT_secondary_color
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/secondary_color.txt
 */
//@{
void      (APIENTRY *glSecondaryColor3bEXT) (GLbyte, GLbyte, GLbyte);
void      (APIENTRY *glSecondaryColor3bvEXT) (const GLbyte *);
void      (APIENTRY *glSecondaryColor3dEXT) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glSecondaryColor3dvEXT) (const GLdouble *);
void      (APIENTRY *glSecondaryColor3fEXT) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glSecondaryColor3fvEXT) (const GLfloat *);
void      (APIENTRY *glSecondaryColor3iEXT) (GLint, GLint, GLint);
void      (APIENTRY *glSecondaryColor3ivEXT) (const GLint *);
void      (APIENTRY *glSecondaryColor3sEXT) (GLshort, GLshort, GLshort);
void      (APIENTRY *glSecondaryColor3svEXT) (const GLshort *);
void      (APIENTRY *glSecondaryColor3ubEXT) (GLubyte, GLubyte, GLubyte);
void      (APIENTRY *glSecondaryColor3ubvEXT) (const GLubyte *);
void      (APIENTRY *glSecondaryColor3uiEXT) (GLuint, GLuint, GLuint);
void      (APIENTRY *glSecondaryColor3uivEXT) (const GLuint *);
void      (APIENTRY *glSecondaryColor3usEXT) (GLushort, GLushort, GLushort);
void      (APIENTRY *glSecondaryColor3usvEXT) (const GLushort *);
void      (APIENTRY *glSecondaryColorPointerEXT) (GLint, GLenum, GLsizei, const GLvoid *);
//@}

/** @name GL_EXT_texture_perturb_normal
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_perturb_normal.txt
 */
//@{
void      (APIENTRY *glTextureNormalEXT) (GLenum);
//@}

/** @name GL_EXT_multi_draw_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/multi_draw_arrays.txt
 */
//@{
void      (APIENTRY *glMultiDrawArraysEXT) (GLenum, GLint *, GLsizei *, GLsizei);
void      (APIENTRY *glMultiDrawElementsEXT) (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);
//@}

/** @name GL_EXT_fog_coord
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/fog_coord.txt
 */
//@{
void      (APIENTRY *glFogCoordfEXT) (GLfloat);
void      (APIENTRY *glFogCoordfvEXT) (const GLfloat *);
void      (APIENTRY *glFogCoorddEXT) (GLdouble);
void      (APIENTRY *glFogCoorddvEXT) (const GLdouble *);
void      (APIENTRY *glFogCoordPointerEXT) (GLenum, GLsizei, const GLvoid *);
//@}

/** @name GL_EXT_coordinate_frame
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/coordinate_frame.txt
 */
//@{
void      (APIENTRY *glTangent3bEXT) (GLbyte, GLbyte, GLbyte);
void      (APIENTRY *glTangent3bvEXT) (const GLbyte *);
void      (APIENTRY *glTangent3dEXT) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glTangent3dvEXT) (const GLdouble *);
void      (APIENTRY *glTangent3fEXT) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTangent3fvEXT) (const GLfloat *);
void      (APIENTRY *glTangent3iEXT) (GLint, GLint, GLint);
void      (APIENTRY *glTangent3ivEXT) (const GLint *);
void      (APIENTRY *glTangent3sEXT) (GLshort, GLshort, GLshort);
void      (APIENTRY *glTangent3svEXT) (const GLshort *);
void      (APIENTRY *glBinormal3bEXT) (GLbyte, GLbyte, GLbyte);
void      (APIENTRY *glBinormal3bvEXT) (const GLbyte *);
void      (APIENTRY *glBinormal3dEXT) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glBinormal3dvEXT) (const GLdouble *);
void      (APIENTRY *glBinormal3fEXT) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glBinormal3fvEXT) (const GLfloat *);
void      (APIENTRY *glBinormal3iEXT) (GLint, GLint, GLint);
void      (APIENTRY *glBinormal3ivEXT) (const GLint *);
void      (APIENTRY *glBinormal3sEXT) (GLshort, GLshort, GLshort);
void      (APIENTRY *glBinormal3svEXT) (const GLshort *);
void      (APIENTRY *glTangentPointerEXT) (GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glBinormalPointerEXT) (GLenum, GLsizei, const GLvoid *);
//@}

/** @name GL_SUNX_constant_data
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUNX/constant_data.txt
 */
//@{
void      (APIENTRY *glFinishTextureSUNX) (void);
//@}

/** @name GL_SUN_global_alpha
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/global_alpha.txt
 */
//@{
void      (APIENTRY *glGlobalAlphaFactorbSUN) (GLbyte);
void      (APIENTRY *glGlobalAlphaFactorsSUN) (GLshort);
void      (APIENTRY *glGlobalAlphaFactoriSUN) (GLint);
void      (APIENTRY *glGlobalAlphaFactorfSUN) (GLfloat);
void      (APIENTRY *glGlobalAlphaFactordSUN) (GLdouble);
void      (APIENTRY *glGlobalAlphaFactorubSUN) (GLubyte);
void      (APIENTRY *glGlobalAlphaFactorusSUN) (GLushort);
void      (APIENTRY *glGlobalAlphaFactoruiSUN) (GLuint);
//@}

/** @name GL_SUN_triangle_list
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/triangle_list.txt
 */
//@{
void      (APIENTRY *glReplacementCodeuiSUN) (GLuint);
void      (APIENTRY *glReplacementCodeusSUN) (GLushort);
void      (APIENTRY *glReplacementCodeubSUN) (GLubyte);
void      (APIENTRY *glReplacementCodeuivSUN) (const GLuint *);
void      (APIENTRY *glReplacementCodeusvSUN) (const GLushort *);
void      (APIENTRY *glReplacementCodeubvSUN) (const GLubyte *);
void      (APIENTRY *glReplacementCodePointerSUN) (GLenum, GLsizei, const GLvoid* *);
//@}

/** @name GL_SUN_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/vertex.txt
 */
//@{
void      (APIENTRY *glColor4ubVertex2fSUN) (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat);
void      (APIENTRY *glColor4ubVertex2fvSUN) (const GLubyte *, const GLfloat *);
void      (APIENTRY *glColor4ubVertex3fSUN) (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glColor4ubVertex3fvSUN) (const GLubyte *, const GLfloat *);
void      (APIENTRY *glColor3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glColor3fVertex3fvSUN) (const GLfloat *, const GLfloat *);
void      (APIENTRY *glNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glNormal3fVertex3fvSUN) (const GLfloat *, const GLfloat *);
void      (APIENTRY *glColor4fNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glColor4fNormal3fVertex3fvSUN) (const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glTexCoord2fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord2fVertex3fvSUN) (const GLfloat *, const GLfloat *);
void      (APIENTRY *glTexCoord4fVertex4fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord4fVertex4fvSUN) (const GLfloat *, const GLfloat *);
void      (APIENTRY *glTexCoord2fColor4ubVertex3fSUN) (GLfloat, GLfloat, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord2fColor4ubVertex3fvSUN) (const GLfloat *, const GLubyte *, const GLfloat *);
void      (APIENTRY *glTexCoord2fColor3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord2fColor3fVertex3fvSUN) (const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glTexCoord2fNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord2fNormal3fVertex3fvSUN) (const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glTexCoord2fColor4fNormal3fVertex3fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glTexCoord4fColor4fNormal3fVertex4fSUN) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glTexCoord4fColor4fNormal3fVertex4fvSUN) (const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiVertex3fvSUN) (const GLuint *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiColor4ubVertex3fSUN) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiColor4ubVertex3fvSUN) (const GLuint *, const GLubyte *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiColor3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiColor3fVertex3fvSUN) (const GLuint *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiNormal3fVertex3fvSUN) (const GLuint *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiColor4fNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiColor4fNormal3fVertex3fvSUN) (const GLuint *, const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiTexCoord2fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiTexCoord2fVertex3fvSUN) (const GLuint *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN) (const GLuint *, const GLfloat *, const GLfloat *, const GLfloat *);
void      (APIENTRY *glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLuint *, const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
//@}

/** @name GL_EXT_blend_func_separate
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_func_separate.txt
 */
//@{
void      (APIENTRY *glBlendFuncSeparateEXT) (GLenum, GLenum, GLenum, GLenum);
//@}

/** @name GL_INGR_blend_func_separate
 *  See http://oss.sgi.com/projects/ogl-sample/registry/INGR/blend_func_separate.txt
 */
//@{
void      (APIENTRY *glBlendFuncSeparateINGR) (GLenum, GLenum, GLenum, GLenum);
//@}

/** @name GL_EXT_vertex_weighting
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_weighting.txt
 */
//@{
void      (APIENTRY *glVertexWeightfEXT) (GLfloat);
void      (APIENTRY *glVertexWeightfvEXT) (const GLfloat *);
void      (APIENTRY *glVertexWeightPointerEXT) (GLsizei, GLenum, GLsizei, const GLvoid *);
//@}

/** @name GL_NV_vertex_array_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_array_range.txt
 */
//@{
void      (APIENTRY *glFlushVertexArrayRangeNV) (void);
void      (APIENTRY *glVertexArrayRangeNV) (GLsizei, const GLvoid *);
//@}

/** @name GL_NV_register_combiners
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/register_combiners.txt
 */
//@{
void      (APIENTRY *glCombinerParameterfvNV) (GLenum, const GLfloat *);
void      (APIENTRY *glCombinerParameterfNV) (GLenum, GLfloat);
void      (APIENTRY *glCombinerParameterivNV) (GLenum, const GLint *);
void      (APIENTRY *glCombinerParameteriNV) (GLenum, GLint);
void      (APIENTRY *glCombinerInputNV) (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glCombinerOutputNV) (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLboolean, GLboolean, GLboolean);
void      (APIENTRY *glFinalCombinerInputNV) (GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glGetCombinerInputParameterfvNV) (GLenum, GLenum, GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetCombinerInputParameterivNV) (GLenum, GLenum, GLenum, GLenum, GLint *);
void      (APIENTRY *glGetCombinerOutputParameterfvNV) (GLenum, GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetCombinerOutputParameterivNV) (GLenum, GLenum, GLenum, GLint *);
void      (APIENTRY *glGetFinalCombinerInputParameterfvNV) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetFinalCombinerInputParameterivNV) (GLenum, GLenum, GLint *);
//@}

/** @name GL_MESA_resize_buffers
 *  See http://oss.sgi.com/projects/ogl-sample/registry/MESA/resize_buffers.txt
 */
//@{
void      (APIENTRY *glResizeBuffersMESA) (void);
//@}

/** @name GL_MESA_window_pos
 *  See http://oss.sgi.com/projects/ogl-sample/registry/MESA/window_pos.txt
 */
//@{
void      (APIENTRY *glWindowPos2dMESA) (GLdouble, GLdouble);
void      (APIENTRY *glWindowPos2dvMESA) (const GLdouble *);
void      (APIENTRY *glWindowPos2fMESA) (GLfloat, GLfloat);
void      (APIENTRY *glWindowPos2fvMESA) (const GLfloat *);
void      (APIENTRY *glWindowPos2iMESA) (GLint, GLint);
void      (APIENTRY *glWindowPos2ivMESA) (const GLint *);
void      (APIENTRY *glWindowPos2sMESA) (GLshort, GLshort);
void      (APIENTRY *glWindowPos2svMESA) (const GLshort *);
void      (APIENTRY *glWindowPos3dMESA) (GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glWindowPos3dvMESA) (const GLdouble *);
void      (APIENTRY *glWindowPos3fMESA) (GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glWindowPos3fvMESA) (const GLfloat *);
void      (APIENTRY *glWindowPos3iMESA) (GLint, GLint, GLint);
void      (APIENTRY *glWindowPos3ivMESA) (const GLint *);
void      (APIENTRY *glWindowPos3sMESA) (GLshort, GLshort, GLshort);
void      (APIENTRY *glWindowPos3svMESA) (const GLshort *);
void      (APIENTRY *glWindowPos4dMESA) (GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glWindowPos4dvMESA) (const GLdouble *);
void      (APIENTRY *glWindowPos4fMESA) (GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glWindowPos4fvMESA) (const GLfloat *);
void      (APIENTRY *glWindowPos4iMESA) (GLint, GLint, GLint, GLint);
void      (APIENTRY *glWindowPos4ivMESA) (const GLint *);
void      (APIENTRY *glWindowPos4sMESA) (GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glWindowPos4svMESA) (const GLshort *);
//@}

/** @name GL_IBM_multimode_draw_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/IBM/multimode_draw_arrays.txt
 */
//@{
void      (APIENTRY *glMultiModeDrawArraysIBM) (const GLenum *, const GLint *, const GLsizei *, GLsizei, GLint);
void      (APIENTRY *glMultiModeDrawElementsIBM) (const GLenum *, const GLsizei *, GLenum, const GLvoid* const *, GLsizei, GLint);
//@}

/** @name GL_IBM_vertex_array_lists
 *  See http://oss.sgi.com/projects/ogl-sample/registry/IBM/vertex_array_lists.txt
 */
//@{
void      (APIENTRY *glColorPointerListIBM) (GLint, GLenum, GLint, const GLvoid* *, GLint);
void      (APIENTRY *glSecondaryColorPointerListIBM) (GLint, GLenum, GLint, const GLvoid* *, GLint);
void      (APIENTRY *glEdgeFlagPointerListIBM) (GLint, const GLboolean* *, GLint);
void      (APIENTRY *glFogCoordPointerListIBM) (GLenum, GLint, const GLvoid* *, GLint);
void      (APIENTRY *glIndexPointerListIBM) (GLenum, GLint, const GLvoid* *, GLint);
void      (APIENTRY *glNormalPointerListIBM) (GLenum, GLint, const GLvoid* *, GLint);
void      (APIENTRY *glTexCoordPointerListIBM) (GLint, GLenum, GLint, const GLvoid* *, GLint);
void      (APIENTRY *glVertexPointerListIBM) (GLint, GLenum, GLint, const GLvoid* *, GLint);
//@}

/** @name GL_3DFX_tbuffer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/3DFX/tbuffer.txt
 */
//@{
void      (APIENTRY *glTbufferMask3DFX) (GLuint);
//@}

/** @name GL_EXT_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/multisample.txt
 */
//@{
void      (APIENTRY *glSampleMaskEXT) (GLclampf, GLboolean);
void      (APIENTRY *glSamplePatternEXT) (GLenum);
//@}

/** @name GL_SGIS_texture_color_mask
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture_color_mask.txt
 */
//@{
void      (APIENTRY *glTextureColorMaskSGIS) (GLboolean, GLboolean, GLboolean, GLboolean);
//@}

/** @name GL_SGIX_igloo_interface
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/igloo_interface.txt
 */
//@{
void      (APIENTRY *glIglooInterfaceSGIX) (GLenum, const GLvoid *);
//@}

/** @name GL_NV_fence
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/fence.txt
 */
//@{
void      (APIENTRY *glDeleteFencesNV) (GLsizei, const GLuint *);
void      (APIENTRY *glGenFencesNV) (GLsizei, GLuint *);
GLboolean (APIENTRY *glIsFenceNV) (GLuint);
GLboolean (APIENTRY *glTestFenceNV) (GLuint);
void      (APIENTRY *glGetFenceivNV) (GLuint, GLenum, GLint *);
void      (APIENTRY *glFinishFenceNV) (GLuint);
void      (APIENTRY *glSetFenceNV) (GLuint, GLenum);
//@}

/** @name GL_NV_evaluators
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/evaluators.txt
 */
//@{
void      (APIENTRY *glMapControlPointsNV) (GLenum, GLuint, GLenum, GLsizei, GLsizei, GLint, GLint, GLboolean, const GLvoid *);
void      (APIENTRY *glMapParameterivNV) (GLenum, GLenum, const GLint *);
void      (APIENTRY *glMapParameterfvNV) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glGetMapControlPointsNV) (GLenum, GLuint, GLenum, GLsizei, GLsizei, GLboolean, GLvoid *);
void      (APIENTRY *glGetMapParameterivNV) (GLenum, GLenum, GLint *);
void      (APIENTRY *glGetMapParameterfvNV) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetMapAttribParameterivNV) (GLenum, GLuint, GLenum, GLint *);
void      (APIENTRY *glGetMapAttribParameterfvNV) (GLenum, GLuint, GLenum, GLfloat *);
void      (APIENTRY *glEvalMapsNV) (GLenum, GLenum);
//@}

/** @name GL_NV_register_combiners2
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/register_combiners2.txt
 */
//@{
void      (APIENTRY *glCombinerStageParameterfvNV) (GLenum, GLenum, const GLfloat *);
void      (APIENTRY *glGetCombinerStageParameterfvNV) (GLenum, GLenum, GLfloat *);
//@}

/** @name GL_NV_vertex_program
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_program.txt
 */
//@{
GLboolean (APIENTRY *glAreProgramsResidentNV) (GLsizei, const GLuint *, GLboolean *);
void      (APIENTRY *glBindProgramNV) (GLenum, GLuint);
void      (APIENTRY *glDeleteProgramsNV) (GLsizei, const GLuint *);
void      (APIENTRY *glExecuteProgramNV) (GLenum, GLuint, const GLfloat *);
void      (APIENTRY *glGenProgramsNV) (GLsizei, GLuint *);
void      (APIENTRY *glGetProgramParameterdvNV) (GLenum, GLuint, GLenum, GLdouble *);
void      (APIENTRY *glGetProgramParameterfvNV) (GLenum, GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetProgramivNV) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetProgramStringNV) (GLuint, GLenum, GLubyte *);
void      (APIENTRY *glGetTrackMatrixivNV) (GLenum, GLuint, GLenum, GLint *);
void      (APIENTRY *glGetVertexAttribdvNV) (GLuint, GLenum, GLdouble *);
void      (APIENTRY *glGetVertexAttribfvNV) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetVertexAttribivNV) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetVertexAttribPointervNV) (GLuint, GLenum, GLvoid* *);
GLboolean (APIENTRY *glIsProgramNV) (GLuint);
void      (APIENTRY *glLoadProgramNV) (GLenum, GLuint, GLsizei, const GLubyte *);
void      (APIENTRY *glProgramParameter4dNV) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glProgramParameter4dvNV) (GLenum, GLuint, const GLdouble *);
void      (APIENTRY *glProgramParameter4fNV) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glProgramParameter4fvNV) (GLenum, GLuint, const GLfloat *);
void      (APIENTRY *glProgramParameters4dvNV) (GLenum, GLuint, GLuint, const GLdouble *);
void      (APIENTRY *glProgramParameters4fvNV) (GLenum, GLuint, GLuint, const GLfloat *);
void      (APIENTRY *glRequestResidentProgramsNV) (GLsizei, const GLuint *);
void      (APIENTRY *glTrackMatrixNV) (GLenum, GLuint, GLenum, GLenum);
void      (APIENTRY *glVertexAttribPointerNV) (GLuint, GLint, GLenum, GLsizei, const GLvoid *);
void      (APIENTRY *glVertexAttrib1dNV) (GLuint, GLdouble);
void      (APIENTRY *glVertexAttrib1dvNV) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib1fNV) (GLuint, GLfloat);
void      (APIENTRY *glVertexAttrib1fvNV) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib1sNV) (GLuint, GLshort);
void      (APIENTRY *glVertexAttrib1svNV) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib2dNV) (GLuint, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib2dvNV) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib2fNV) (GLuint, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib2fvNV) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib2sNV) (GLuint, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib2svNV) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib3dNV) (GLuint, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib3dvNV) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib3fNV) (GLuint, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib3fvNV) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib3sNV) (GLuint, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib3svNV) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4dNV) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexAttrib4dvNV) (GLuint, const GLdouble *);
void      (APIENTRY *glVertexAttrib4fNV) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexAttrib4fvNV) (GLuint, const GLfloat *);
void      (APIENTRY *glVertexAttrib4sNV) (GLuint, GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexAttrib4svNV) (GLuint, const GLshort *);
void      (APIENTRY *glVertexAttrib4ubNV) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
void      (APIENTRY *glVertexAttrib4ubvNV) (GLuint, const GLubyte *);
void      (APIENTRY *glVertexAttribs1dvNV) (GLuint, GLsizei, const GLdouble *);
void      (APIENTRY *glVertexAttribs1fvNV) (GLuint, GLsizei, const GLfloat *);
void      (APIENTRY *glVertexAttribs1svNV) (GLuint, GLsizei, const GLshort *);
void      (APIENTRY *glVertexAttribs2dvNV) (GLuint, GLsizei, const GLdouble *);
void      (APIENTRY *glVertexAttribs2fvNV) (GLuint, GLsizei, const GLfloat *);
void      (APIENTRY *glVertexAttribs2svNV) (GLuint, GLsizei, const GLshort *);
void      (APIENTRY *glVertexAttribs3dvNV) (GLuint, GLsizei, const GLdouble *);
void      (APIENTRY *glVertexAttribs3fvNV) (GLuint, GLsizei, const GLfloat *);
void      (APIENTRY *glVertexAttribs3svNV) (GLuint, GLsizei, const GLshort *);
void      (APIENTRY *glVertexAttribs4dvNV) (GLuint, GLsizei, const GLdouble *);
void      (APIENTRY *glVertexAttribs4fvNV) (GLuint, GLsizei, const GLfloat *);
void      (APIENTRY *glVertexAttribs4svNV) (GLuint, GLsizei, const GLshort *);
void      (APIENTRY *glVertexAttribs4ubvNV) (GLuint, GLsizei, const GLubyte *);
//@}

/** @name GL_ATI_envmap_bumpmap
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/envmap_bumpmap.txt
 */
//@{
void      (APIENTRY *glTexBumpParameterivATI) (GLenum, const GLint *);
void      (APIENTRY *glTexBumpParameterfvATI) (GLenum, const GLfloat *);
void      (APIENTRY *glGetTexBumpParameterivATI) (GLenum, GLint *);
void      (APIENTRY *glGetTexBumpParameterfvATI) (GLenum, GLfloat *);
//@}

/** @name GL_ATI_fragment_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/fragment_shader.txt
 */
//@{
GLuint    (APIENTRY *glGenFragmentShadersATI) (GLuint);
void      (APIENTRY *glBindFragmentShaderATI) (GLuint);
void      (APIENTRY *glDeleteFragmentShaderATI) (GLuint);
void      (APIENTRY *glBeginFragmentShaderATI) (void);
void      (APIENTRY *glEndFragmentShaderATI) (void);
void      (APIENTRY *glPassTexCoordATI) (GLuint, GLuint, GLenum);
void      (APIENTRY *glSampleMapATI) (GLuint, GLuint, GLenum);
void      (APIENTRY *glColorFragmentOp1ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glColorFragmentOp2ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glColorFragmentOp3ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glAlphaFragmentOp1ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glAlphaFragmentOp2ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glAlphaFragmentOp3ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glSetFragmentShaderConstantATI) (GLuint, const GLfloat *);
//@}

/** @name GL_ATI_pn_triangles
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/pn_triangles.txt
 */
//@{
void      (APIENTRY *glPNTrianglesiATI) (GLenum, GLint);
void      (APIENTRY *glPNTrianglesfATI) (GLenum, GLfloat);
//@}

/** @name GL_ATI_vertex_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/vertex_array_object.txt
 */
//@{
GLuint    (APIENTRY *glNewObjectBufferATI) (GLsizei, const GLvoid *, GLenum);
GLboolean (APIENTRY *glIsObjectBufferATI) (GLuint);
void      (APIENTRY *glUpdateObjectBufferATI) (GLuint, GLuint, GLsizei, const GLvoid *, GLenum);
void      (APIENTRY *glGetObjectBufferfvATI) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetObjectBufferivATI) (GLuint, GLenum, GLint *);
void      (APIENTRY *glFreeObjectBufferATI) (GLuint);
void      (APIENTRY *glArrayObjectATI) (GLenum, GLint, GLenum, GLsizei, GLuint, GLuint);
void      (APIENTRY *glGetArrayObjectfvATI) (GLenum, GLenum, GLfloat *);
void      (APIENTRY *glGetArrayObjectivATI) (GLenum, GLenum, GLint *);
void      (APIENTRY *glVariantArrayObjectATI) (GLuint, GLenum, GLsizei, GLuint, GLuint);
void      (APIENTRY *glGetVariantArrayObjectfvATI) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetVariantArrayObjectivATI) (GLuint, GLenum, GLint *);
//@}

/** @name GL_EXT_vertex_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_shader.txt
 */
//@{
void      (APIENTRY *glBeginVertexShaderEXT) (void);
void      (APIENTRY *glEndVertexShaderEXT) (void);
void      (APIENTRY *glBindVertexShaderEXT) (GLuint);
GLuint    (APIENTRY *glGenVertexShadersEXT) (GLuint);
void      (APIENTRY *glDeleteVertexShaderEXT) (GLuint);
void      (APIENTRY *glShaderOp1EXT) (GLenum, GLuint, GLuint);
void      (APIENTRY *glShaderOp2EXT) (GLenum, GLuint, GLuint, GLuint);
void      (APIENTRY *glShaderOp3EXT) (GLenum, GLuint, GLuint, GLuint, GLuint);
void      (APIENTRY *glSwizzleEXT) (GLuint, GLuint, GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glWriteMaskEXT) (GLuint, GLuint, GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glInsertComponentEXT) (GLuint, GLuint, GLuint);
void      (APIENTRY *glExtractComponentEXT) (GLuint, GLuint, GLuint);
GLuint    (APIENTRY *glGenSymbolsEXT) (GLenum, GLenum, GLenum, GLuint);
void      (APIENTRY *glSetInvariantEXT) (GLuint, GLenum, const GLvoid *);
void      (APIENTRY *glSetLocalConstantEXT) (GLuint, GLenum, const GLvoid *);
void      (APIENTRY *glVariantbvEXT) (GLuint, const GLbyte *);
void      (APIENTRY *glVariantsvEXT) (GLuint, const GLshort *);
void      (APIENTRY *glVariantivEXT) (GLuint, const GLint *);
void      (APIENTRY *glVariantfvEXT) (GLuint, const GLfloat *);
void      (APIENTRY *glVariantdvEXT) (GLuint, const GLdouble *);
void      (APIENTRY *glVariantubvEXT) (GLuint, const GLubyte *);
void      (APIENTRY *glVariantusvEXT) (GLuint, const GLushort *);
void      (APIENTRY *glVariantuivEXT) (GLuint, const GLuint *);
void      (APIENTRY *glVariantPointerEXT) (GLuint, GLenum, GLuint, const GLvoid *);
void      (APIENTRY *glEnableVariantClientStateEXT) (GLuint);
void      (APIENTRY *glDisableVariantClientStateEXT) (GLuint);
GLuint    (APIENTRY *glBindLightParameterEXT) (GLenum, GLenum);
GLuint    (APIENTRY *glBindMaterialParameterEXT) (GLenum, GLenum);
GLuint    (APIENTRY *glBindTexGenParameterEXT) (GLenum, GLenum, GLenum);
GLuint    (APIENTRY *glBindTextureUnitParameterEXT) (GLenum, GLenum);
GLuint    (APIENTRY *glBindParameterEXT) (GLenum);
GLboolean (APIENTRY *glIsVariantEnabledEXT) (GLuint, GLenum);
void      (APIENTRY *glGetVariantBooleanvEXT) (GLuint, GLenum, GLboolean *);
void      (APIENTRY *glGetVariantIntegervEXT) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetVariantFloatvEXT) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetVariantPointervEXT) (GLuint, GLenum, GLvoid* *);
void      (APIENTRY *glGetInvariantBooleanvEXT) (GLuint, GLenum, GLboolean *);
void      (APIENTRY *glGetInvariantIntegervEXT) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetInvariantFloatvEXT) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetLocalConstantBooleanvEXT) (GLuint, GLenum, GLboolean *);
void      (APIENTRY *glGetLocalConstantIntegervEXT) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetLocalConstantFloatvEXT) (GLuint, GLenum, GLfloat *);
//@}

/** @name GL_ATI_vertex_streams
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/vertex_streams.txt
 */
//@{
void      (APIENTRY *glVertexStream1sATI) (GLenum, GLshort);
void      (APIENTRY *glVertexStream1svATI) (GLenum, const GLshort *);
void      (APIENTRY *glVertexStream1iATI) (GLenum, GLint);
void      (APIENTRY *glVertexStream1ivATI) (GLenum, const GLint *);
void      (APIENTRY *glVertexStream1fATI) (GLenum, GLfloat);
void      (APIENTRY *glVertexStream1fvATI) (GLenum, const GLfloat *);
void      (APIENTRY *glVertexStream1dATI) (GLenum, GLdouble);
void      (APIENTRY *glVertexStream1dvATI) (GLenum, const GLdouble *);
void      (APIENTRY *glVertexStream2sATI) (GLenum, GLshort, GLshort);
void      (APIENTRY *glVertexStream2svATI) (GLenum, const GLshort *);
void      (APIENTRY *glVertexStream2iATI) (GLenum, GLint, GLint);
void      (APIENTRY *glVertexStream2ivATI) (GLenum, const GLint *);
void      (APIENTRY *glVertexStream2fATI) (GLenum, GLfloat, GLfloat);
void      (APIENTRY *glVertexStream2fvATI) (GLenum, const GLfloat *);
void      (APIENTRY *glVertexStream2dATI) (GLenum, GLdouble, GLdouble);
void      (APIENTRY *glVertexStream2dvATI) (GLenum, const GLdouble *);
void      (APIENTRY *glVertexStream3sATI) (GLenum, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexStream3svATI) (GLenum, const GLshort *);
void      (APIENTRY *glVertexStream3iATI) (GLenum, GLint, GLint, GLint);
void      (APIENTRY *glVertexStream3ivATI) (GLenum, const GLint *);
void      (APIENTRY *glVertexStream3fATI) (GLenum, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexStream3fvATI) (GLenum, const GLfloat *);
void      (APIENTRY *glVertexStream3dATI) (GLenum, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexStream3dvATI) (GLenum, const GLdouble *);
void      (APIENTRY *glVertexStream4sATI) (GLenum, GLshort, GLshort, GLshort, GLshort);
void      (APIENTRY *glVertexStream4svATI) (GLenum, const GLshort *);
void      (APIENTRY *glVertexStream4iATI) (GLenum, GLint, GLint, GLint, GLint);
void      (APIENTRY *glVertexStream4ivATI) (GLenum, const GLint *);
void      (APIENTRY *glVertexStream4fATI) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glVertexStream4fvATI) (GLenum, const GLfloat *);
void      (APIENTRY *glVertexStream4dATI) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glVertexStream4dvATI) (GLenum, const GLdouble *);
void      (APIENTRY *glNormalStream3bATI) (GLenum, GLbyte, GLbyte, GLbyte);
void      (APIENTRY *glNormalStream3bvATI) (GLenum, const GLbyte *);
void      (APIENTRY *glNormalStream3sATI) (GLenum, GLshort, GLshort, GLshort);
void      (APIENTRY *glNormalStream3svATI) (GLenum, const GLshort *);
void      (APIENTRY *glNormalStream3iATI) (GLenum, GLint, GLint, GLint);
void      (APIENTRY *glNormalStream3ivATI) (GLenum, const GLint *);
void      (APIENTRY *glNormalStream3fATI) (GLenum, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glNormalStream3fvATI) (GLenum, const GLfloat *);
void      (APIENTRY *glNormalStream3dATI) (GLenum, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glNormalStream3dvATI) (GLenum, const GLdouble *);
void      (APIENTRY *glClientActiveVertexStreamATI) (GLenum);
void      (APIENTRY *glVertexBlendEnviATI) (GLenum, GLint);
void      (APIENTRY *glVertexBlendEnvfATI) (GLenum, GLfloat);
//@}

/** @name GL_ATI_element_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/element_array.txt
 */
//@{
void      (APIENTRY *glElementPointerATI) (GLenum, const GLvoid *);
void      (APIENTRY *glDrawElementArrayATI) (GLenum, GLsizei);
void      (APIENTRY *glDrawRangeElementArrayATI) (GLenum, GLuint, GLuint, GLsizei);
//@}

/** @name GL_SUN_mesh_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/mesh_array.txt
 */
//@{
void      (APIENTRY *glDrawMeshArraysSUN) (GLenum, GLint, GLsizei, GLsizei);
//@}

/** @name GL_NV_occlusion_query
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/occlusion_query.txt
 */
//@{
void      (APIENTRY *glGenOcclusionQueriesNV) (GLsizei, GLuint *);
void      (APIENTRY *glDeleteOcclusionQueriesNV) (GLsizei, const GLuint *);
GLboolean (APIENTRY *glIsOcclusionQueryNV) (GLuint);
void      (APIENTRY *glBeginOcclusionQueryNV) (GLuint);
void      (APIENTRY *glEndOcclusionQueryNV) (void);
void      (APIENTRY *glGetOcclusionQueryivNV) (GLuint, GLenum, GLint *);
void      (APIENTRY *glGetOcclusionQueryuivNV) (GLuint, GLenum, GLuint *);
//@}

/** @name GL_NV_point_sprite
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/point_sprite.txt
 */
//@{
void      (APIENTRY *glPointParameteriNV) (GLenum, GLint);
void      (APIENTRY *glPointParameterivNV) (GLenum, const GLint *);
//@}

/** @name GL_EXT_stencil_two_side
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/stencil_two_side.txt
 */
//@{
void      (APIENTRY *glActiveStencilFaceEXT) (GLenum);
//@}

/** @name GL_APPLE_element_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/element_array.txt
 */
//@{
void      (APIENTRY *glElementPointerAPPLE) (GLenum, const GLvoid *);
void      (APIENTRY *glDrawElementArrayAPPLE) (GLenum, GLint, GLsizei);
void      (APIENTRY *glDrawRangeElementArrayAPPLE) (GLenum, GLuint, GLuint, GLint, GLsizei);
void      (APIENTRY *glMultiDrawElementArrayAPPLE) (GLenum, const GLint *, const GLsizei *, GLsizei);
void      (APIENTRY *glMultiDrawRangeElementArrayAPPLE) (GLenum, GLuint, GLuint, const GLint *, const GLsizei *, GLsizei);
//@}

/** @name GL_APPLE_fence
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/fence.txt
 */
//@{
void      (APIENTRY *glGenFencesAPPLE) (GLsizei, GLuint *);
void      (APIENTRY *glDeleteFencesAPPLE) (GLsizei, const GLuint *);
void      (APIENTRY *glSetFenceAPPLE) (GLuint);
GLboolean (APIENTRY *glIsFenceAPPLE) (GLuint);
GLboolean (APIENTRY *glTestFenceAPPLE) (GLuint);
void      (APIENTRY *glFinishFenceAPPLE) (GLuint);
GLboolean (APIENTRY *glTestObjectAPPLE) (GLenum, GLuint);
void      (APIENTRY *glFinishObjectAPPLE) (GLenum, GLint);
//@}

/** @name GL_APPLE_vertex_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/vertex_array_object.txt
 */
//@{
void      (APIENTRY *glBindVertexArrayAPPLE) (GLuint);
void      (APIENTRY *glDeleteVertexArraysAPPLE) (GLsizei, const GLuint *);
void      (APIENTRY *glGenVertexArraysAPPLE) (GLsizei, const GLuint *);
GLboolean (APIENTRY *glIsVertexArrayAPPLE) (GLuint);
//@}

/** @name GL_APPLE_vertex_array_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/vertex_array_range.txt
 */
//@{
void      (APIENTRY *glVertexArrayRangeAPPLE) (GLsizei, GLvoid *);
void      (APIENTRY *glFlushVertexArrayRangeAPPLE) (GLsizei, GLvoid *);
void      (APIENTRY *glVertexArrayParameteriAPPLE) (GLenum, GLint);
//@}

/** @name GL_ATI_draw_buffers
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/draw_buffers.txt
 */
//@{
void      (APIENTRY *glDrawBuffersATI) (GLsizei, const GLenum *);
//@}

/** @name GL_NV_fragment_program
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/fragment_program.txt
 */
//@{
void      (APIENTRY *glProgramNamedParameter4fNV) (GLuint, GLsizei, const GLubyte *, GLfloat, GLfloat, GLfloat, GLfloat);
void      (APIENTRY *glProgramNamedParameter4dNV) (GLuint, GLsizei, const GLubyte *, GLdouble, GLdouble, GLdouble, GLdouble);
void      (APIENTRY *glProgramNamedParameter4fvNV) (GLuint, GLsizei, const GLubyte *, const GLfloat *);
void      (APIENTRY *glProgramNamedParameter4dvNV) (GLuint, GLsizei, const GLubyte *, const GLdouble *);
void      (APIENTRY *glGetProgramNamedParameterfvNV) (GLuint, GLsizei, const GLubyte *, GLfloat *);
void      (APIENTRY *glGetProgramNamedParameterdvNV) (GLuint, GLsizei, const GLubyte *, GLdouble *);
//@}

/** @name GL_NV_half_float
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/half_float.txt
 */
//@{
void      (APIENTRY *glVertex2hNV) (GLhalfNV, GLhalfNV);
void      (APIENTRY *glVertex2hvNV) (const GLhalfNV *);
void      (APIENTRY *glVertex3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glVertex3hvNV) (const GLhalfNV *);
void      (APIENTRY *glVertex4hNV) (GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glVertex4hvNV) (const GLhalfNV *);
void      (APIENTRY *glNormal3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glNormal3hvNV) (const GLhalfNV *);
void      (APIENTRY *glColor3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glColor3hvNV) (const GLhalfNV *);
void      (APIENTRY *glColor4hNV) (GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glColor4hvNV) (const GLhalfNV *);
void      (APIENTRY *glTexCoord1hNV) (GLhalfNV);
void      (APIENTRY *glTexCoord1hvNV) (const GLhalfNV *);
void      (APIENTRY *glTexCoord2hNV) (GLhalfNV, GLhalfNV);
void      (APIENTRY *glTexCoord2hvNV) (const GLhalfNV *);
void      (APIENTRY *glTexCoord3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glTexCoord3hvNV) (const GLhalfNV *);
void      (APIENTRY *glTexCoord4hNV) (GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glTexCoord4hvNV) (const GLhalfNV *);
void      (APIENTRY *glMultiTexCoord1hNV) (GLenum, GLhalfNV);
void      (APIENTRY *glMultiTexCoord1hvNV) (GLenum, const GLhalfNV *);
void      (APIENTRY *glMultiTexCoord2hNV) (GLenum, GLhalfNV, GLhalfNV);
void      (APIENTRY *glMultiTexCoord2hvNV) (GLenum, const GLhalfNV *);
void      (APIENTRY *glMultiTexCoord3hNV) (GLenum, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glMultiTexCoord3hvNV) (GLenum, const GLhalfNV *);
void      (APIENTRY *glMultiTexCoord4hNV) (GLenum, GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glMultiTexCoord4hvNV) (GLenum, const GLhalfNV *);
void      (APIENTRY *glFogCoordhNV) (GLhalfNV);
void      (APIENTRY *glFogCoordhvNV) (const GLhalfNV *);
void      (APIENTRY *glSecondaryColor3hNV) (GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glSecondaryColor3hvNV) (const GLhalfNV *);
void      (APIENTRY *glVertexWeighthNV) (GLhalfNV);
void      (APIENTRY *glVertexWeighthvNV) (const GLhalfNV *);
void      (APIENTRY *glVertexAttrib1hNV) (GLuint, GLhalfNV);
void      (APIENTRY *glVertexAttrib1hvNV) (GLuint, const GLhalfNV *);
void      (APIENTRY *glVertexAttrib2hNV) (GLuint, GLhalfNV, GLhalfNV);
void      (APIENTRY *glVertexAttrib2hvNV) (GLuint, const GLhalfNV *);
void      (APIENTRY *glVertexAttrib3hNV) (GLuint, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glVertexAttrib3hvNV) (GLuint, const GLhalfNV *);
void      (APIENTRY *glVertexAttrib4hNV) (GLuint, GLhalfNV, GLhalfNV, GLhalfNV, GLhalfNV);
void      (APIENTRY *glVertexAttrib4hvNV) (GLuint, const GLhalfNV *);
void      (APIENTRY *glVertexAttribs1hvNV) (GLuint, GLsizei, const GLhalfNV *);
void      (APIENTRY *glVertexAttribs2hvNV) (GLuint, GLsizei, const GLhalfNV *);
void      (APIENTRY *glVertexAttribs3hvNV) (GLuint, GLsizei, const GLhalfNV *);
void      (APIENTRY *glVertexAttribs4hvNV) (GLuint, GLsizei, const GLhalfNV *);
//@}

/** @name GL_NV_pixel_data_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/pixel_data_range.txt
 */
//@{
void      (APIENTRY *glPixelDataRangeNV) (GLenum, GLsizei, GLvoid *);
void      (APIENTRY *glFlushPixelDataRangeNV) (GLenum);
//@}

/** @name GL_NV_primitive_restart
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/primitive_restart.txt
 */
//@{
void      (APIENTRY *glPrimitiveRestartNV) (void);
void      (APIENTRY *glPrimitiveRestartIndexNV) (GLuint);
//@}

/** @name GL_ATI_map_object_buffer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/map_object_buffer.txt
 */
//@{
GLvoid*   (APIENTRY *glMapObjectBufferATI) (GLuint);
void      (APIENTRY *glUnmapObjectBufferATI) (GLuint);
//@}

/** @name GL_ATI_separate_stencil
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/separate_stencil.txt
 */
//@{
void      (APIENTRY *glStencilOpSeparateATI) (GLenum, GLenum, GLenum, GLenum);
void      (APIENTRY *glStencilFuncSeparateATI) (GLenum, GLenum, GLint, GLuint);
//@}

/** @name GL_ATI_vertex_attrib_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/vertex_attrib_array_object.txt
 */
//@{
void      (APIENTRY *glVertexAttribArrayObjectATI) (GLuint, GLint, GLenum, GLboolean, GLsizei, GLuint, GLuint);
void      (APIENTRY *glGetVertexAttribArrayObjectfvATI) (GLuint, GLenum, GLfloat *);
void      (APIENTRY *glGetVertexAttribArrayObjectivATI) (GLuint, GLenum, GLint *);
//@}

/** @name GL_EXT_depth_bounds_test
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/depth_bounds_test.txt
 */
//@{
void      (APIENTRY *glDepthBoundsEXT) (GLclampd, GLclampd);
//@}

/** @name GL_EXT_blend_equation_separate
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_equation_separate.txt
 */
//@{
void      (APIENTRY *glBlendEquationSeparateEXT) (GLenum, GLenum);
//@}

/** @name GL_EXT_framebuffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
 */
//@{
GLboolean (APIENTRY *glIsRenderbufferEXT) (GLuint);
void      (APIENTRY *glBindRenderbufferEXT) (GLenum, GLuint);
void      (APIENTRY *glDeleteRenderbuffersEXT) (GLsizei, const GLuint *);
void      (APIENTRY *glGenRenderbuffersEXT) (GLsizei, GLuint *);
void      (APIENTRY *glRenderbufferStorageEXT) (GLenum, GLenum, GLsizei, GLsizei);
void      (APIENTRY *glGetRenderbufferParameterivEXT) (GLenum, GLenum, GLint *);
GLboolean (APIENTRY *glIsFramebufferEXT) (GLuint);
void      (APIENTRY *glBindFramebufferEXT) (GLenum, GLuint);
void      (APIENTRY *glDeleteFramebuffersEXT) (GLsizei, const GLuint *);
void      (APIENTRY *glGenFramebuffersEXT) (GLsizei, GLuint *);
GLenum    (APIENTRY *glCheckFramebufferStatusEXT) (GLenum);
void      (APIENTRY *glFramebufferTexture1DEXT) (GLenum, GLenum, GLenum, GLuint, GLint);
void      (APIENTRY *glFramebufferTexture2DEXT) (GLenum, GLenum, GLenum, GLuint, GLint);
void      (APIENTRY *glFramebufferTexture3DEXT) (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
void      (APIENTRY *glFramebufferRenderbufferEXT) (GLenum, GLenum, GLenum, GLuint);
void      (APIENTRY *glGetFramebufferAttachmentParameterivEXT) (GLenum, GLenum, GLenum, GLint *);
void      (APIENTRY *glGenerateMipmapEXT) (GLenum);
//@}

/** @name GL_GREMEDY_string_marker
 *  See http://oss.sgi.com/projects/ogl-sample/registry/GREMEDY/string_marker.txt
 */
//@{
void      (APIENTRY *glStringMarkerGREMEDY) (GLsizei, const GLvoid *);
//@}



/* 116 extensions, 1086 total functions */

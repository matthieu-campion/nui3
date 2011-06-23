/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* This file is included and \b must only be included by nglContext.
 * These prototypes become part of nglContext's own methods.
 */

/* This file was generated with glext_gen.pl, please do not edit.  */

/** @name GL_VERSION_1_2
 *  See http://www.opengl.org/developers/documentation/Version1.2/OpenGL_spec_1.2.1.pdf
 */
//@{
void      (APIENTRY *glBlendColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void      (APIENTRY *glBlendEquation) (GLenum mode);
void      (APIENTRY *glDrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
void      (APIENTRY *glTexImage3D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glCopyTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//@}

/** @name GL_VERSION_1_2_DEPRECATED
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/1_2_DEPRECATED.txt
 */
//@{
void      (APIENTRY *glColorTable) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void      (APIENTRY *glColorTableParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glColorTableParameteriv) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glCopyColorTable) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glGetColorTable) (GLenum target, GLenum format, GLenum type, GLvoid *table);
void      (APIENTRY *glGetColorTableParameterfv) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetColorTableParameteriv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glColorSubTable) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
void      (APIENTRY *glCopyColorSubTable) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glConvolutionFilter1D) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
void      (APIENTRY *glConvolutionFilter2D) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
void      (APIENTRY *glConvolutionParameterf) (GLenum target, GLenum pname, GLfloat params);
void      (APIENTRY *glConvolutionParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glConvolutionParameteri) (GLenum target, GLenum pname, GLint params);
void      (APIENTRY *glConvolutionParameteriv) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glCopyConvolutionFilter1D) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glCopyConvolutionFilter2D) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glGetConvolutionFilter) (GLenum target, GLenum format, GLenum type, GLvoid *image);
void      (APIENTRY *glGetConvolutionParameterfv) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetConvolutionParameteriv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetSeparableFilter) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
void      (APIENTRY *glSeparableFilter2D) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
void      (APIENTRY *glGetHistogram) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void      (APIENTRY *glGetHistogramParameterfv) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetHistogramParameteriv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetMinmax) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void      (APIENTRY *glGetMinmaxParameterfv) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMinmaxParameteriv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glHistogram) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
void      (APIENTRY *glMinmax) (GLenum target, GLenum internalformat, GLboolean sink);
void      (APIENTRY *glResetHistogram) (GLenum target);
void      (APIENTRY *glResetMinmax) (GLenum target);
//@}

/** @name GL_VERSION_1_3
 *  See http://www.opengl.org/developers/documentation/version1_3/glspec13.pdf
 */
//@{
void      (APIENTRY *glActiveTexture) (GLenum texture);
void      (APIENTRY *glSampleCoverage) (GLclampf value, GLboolean invert);
void      (APIENTRY *glCompressedTexImage3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexImage1D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glGetCompressedTexImage) (GLenum target, GLint level, GLvoid *img);
//@}

/** @name GL_VERSION_1_3_DEPRECATED
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/1_3_DEPRECATED.txt
 */
//@{
void      (APIENTRY *glClientActiveTexture) (GLenum texture);
void      (APIENTRY *glMultiTexCoord1d) (GLenum target, GLdouble s);
void      (APIENTRY *glMultiTexCoord1dv) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord1f) (GLenum target, GLfloat s);
void      (APIENTRY *glMultiTexCoord1fv) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord1i) (GLenum target, GLint s);
void      (APIENTRY *glMultiTexCoord1iv) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord1s) (GLenum target, GLshort s);
void      (APIENTRY *glMultiTexCoord1sv) (GLenum target, const GLshort *v);
void      (APIENTRY *glMultiTexCoord2d) (GLenum target, GLdouble s, GLdouble t);
void      (APIENTRY *glMultiTexCoord2dv) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord2f) (GLenum target, GLfloat s, GLfloat t);
void      (APIENTRY *glMultiTexCoord2fv) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord2i) (GLenum target, GLint s, GLint t);
void      (APIENTRY *glMultiTexCoord2iv) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord2s) (GLenum target, GLshort s, GLshort t);
void      (APIENTRY *glMultiTexCoord2sv) (GLenum target, const GLshort *v);
void      (APIENTRY *glMultiTexCoord3d) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
void      (APIENTRY *glMultiTexCoord3dv) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord3f) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
void      (APIENTRY *glMultiTexCoord3fv) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord3i) (GLenum target, GLint s, GLint t, GLint r);
void      (APIENTRY *glMultiTexCoord3iv) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord3s) (GLenum target, GLshort s, GLshort t, GLshort r);
void      (APIENTRY *glMultiTexCoord3sv) (GLenum target, const GLshort *v);
void      (APIENTRY *glMultiTexCoord4d) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void      (APIENTRY *glMultiTexCoord4dv) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord4f) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void      (APIENTRY *glMultiTexCoord4fv) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord4i) (GLenum target, GLint s, GLint t, GLint r, GLint q);
void      (APIENTRY *glMultiTexCoord4iv) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord4s) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void      (APIENTRY *glMultiTexCoord4sv) (GLenum target, const GLshort *v);
void      (APIENTRY *glLoadTransposeMatrixf) (const GLfloat *m);
void      (APIENTRY *glLoadTransposeMatrixd) (const GLdouble *m);
void      (APIENTRY *glMultTransposeMatrixf) (const GLfloat *m);
void      (APIENTRY *glMultTransposeMatrixd) (const GLdouble *m);
//@}

/** @name GL_VERSION_1_4
 *  See http://www.opengl.org/developers/documentation/version1_4/glspec14.pdf
 */
//@{
void      (APIENTRY *glBlendFuncSeparate) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void      (APIENTRY *glMultiDrawArrays) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
void      (APIENTRY *glMultiDrawElements) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);
void      (APIENTRY *glPointParameterf) (GLenum pname, GLfloat param);
void      (APIENTRY *glPointParameterfv) (GLenum pname, const GLfloat *params);
void      (APIENTRY *glPointParameteri) (GLenum pname, GLint param);
void      (APIENTRY *glPointParameteriv) (GLenum pname, const GLint *params);
//@}

/** @name GL_VERSION_1_4_DEPRECATED
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/1_4_DEPRECATED.txt
 */
//@{
void      (APIENTRY *glFogCoordf) (GLfloat coord);
void      (APIENTRY *glFogCoordfv) (const GLfloat *coord);
void      (APIENTRY *glFogCoordd) (GLdouble coord);
void      (APIENTRY *glFogCoorddv) (const GLdouble *coord);
void      (APIENTRY *glFogCoordPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glSecondaryColor3b) (GLbyte red, GLbyte green, GLbyte blue);
void      (APIENTRY *glSecondaryColor3bv) (const GLbyte *v);
void      (APIENTRY *glSecondaryColor3d) (GLdouble red, GLdouble green, GLdouble blue);
void      (APIENTRY *glSecondaryColor3dv) (const GLdouble *v);
void      (APIENTRY *glSecondaryColor3f) (GLfloat red, GLfloat green, GLfloat blue);
void      (APIENTRY *glSecondaryColor3fv) (const GLfloat *v);
void      (APIENTRY *glSecondaryColor3i) (GLint red, GLint green, GLint blue);
void      (APIENTRY *glSecondaryColor3iv) (const GLint *v);
void      (APIENTRY *glSecondaryColor3s) (GLshort red, GLshort green, GLshort blue);
void      (APIENTRY *glSecondaryColor3sv) (const GLshort *v);
void      (APIENTRY *glSecondaryColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
void      (APIENTRY *glSecondaryColor3ubv) (const GLubyte *v);
void      (APIENTRY *glSecondaryColor3ui) (GLuint red, GLuint green, GLuint blue);
void      (APIENTRY *glSecondaryColor3uiv) (const GLuint *v);
void      (APIENTRY *glSecondaryColor3us) (GLushort red, GLushort green, GLushort blue);
void      (APIENTRY *glSecondaryColor3usv) (const GLushort *v);
void      (APIENTRY *glSecondaryColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glWindowPos2d) (GLdouble x, GLdouble y);
void      (APIENTRY *glWindowPos2dv) (const GLdouble *v);
void      (APIENTRY *glWindowPos2f) (GLfloat x, GLfloat y);
void      (APIENTRY *glWindowPos2fv) (const GLfloat *v);
void      (APIENTRY *glWindowPos2i) (GLint x, GLint y);
void      (APIENTRY *glWindowPos2iv) (const GLint *v);
void      (APIENTRY *glWindowPos2s) (GLshort x, GLshort y);
void      (APIENTRY *glWindowPos2sv) (const GLshort *v);
void      (APIENTRY *glWindowPos3d) (GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glWindowPos3dv) (const GLdouble *v);
void      (APIENTRY *glWindowPos3f) (GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glWindowPos3fv) (const GLfloat *v);
void      (APIENTRY *glWindowPos3i) (GLint x, GLint y, GLint z);
void      (APIENTRY *glWindowPos3iv) (const GLint *v);
void      (APIENTRY *glWindowPos3s) (GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glWindowPos3sv) (const GLshort *v);
//@}

/** @name GL_VERSION_1_5
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/1_5.txt
 */
//@{
void      (APIENTRY *glGenQueries) (GLsizei n, GLuint *ids);
void      (APIENTRY *glDeleteQueries) (GLsizei n, const GLuint *ids);
GLboolean (APIENTRY *glIsQuery) (GLuint id);
void      (APIENTRY *glBeginQuery) (GLenum target, GLuint id);
void      (APIENTRY *glEndQuery) (GLenum target);
void      (APIENTRY *glGetQueryiv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetQueryObjectiv) (GLuint id, GLenum pname, GLint *params);
void      (APIENTRY *glGetQueryObjectuiv) (GLuint id, GLenum pname, GLuint *params);
void      (APIENTRY *glBindBuffer) (GLenum target, GLuint buffer);
void      (APIENTRY *glDeleteBuffers) (GLsizei n, const GLuint *buffers);
void      (APIENTRY *glGenBuffers) (GLsizei n, GLuint *buffers);
GLboolean (APIENTRY *glIsBuffer) (GLuint buffer);
void      (APIENTRY *glBufferData) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void      (APIENTRY *glBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void      (APIENTRY *glGetBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
GLvoid*   (APIENTRY *glMapBuffer) (GLenum target, GLenum access);
GLboolean (APIENTRY *glUnmapBuffer) (GLenum target);
void      (APIENTRY *glGetBufferParameteriv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetBufferPointerv) (GLenum target, GLenum pname, GLvoid* *params);
//@}

/** @name GL_VERSION_2_0
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/2_0.txt
 */
//@{
void      (APIENTRY *glBlendEquationSeparate) (GLenum modeRGB, GLenum modeAlpha);
void      (APIENTRY *glDrawBuffers) (GLsizei n, const GLenum *bufs);
void      (APIENTRY *glStencilOpSeparate) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void      (APIENTRY *glStencilFuncSeparate) (GLenum face, GLenum func, GLint ref, GLuint mask);
void      (APIENTRY *glStencilMaskSeparate) (GLenum face, GLuint mask);
void      (APIENTRY *glAttachShader) (GLuint program, GLuint shader);
void      (APIENTRY *glBindAttribLocation) (GLuint program, GLuint index, const GLchar *name);
void      (APIENTRY *glCompileShader) (GLuint shader);
GLuint    (APIENTRY *glCreateProgram) (void);
GLuint    (APIENTRY *glCreateShader) (GLenum type);
void      (APIENTRY *glDeleteProgram) (GLuint program);
void      (APIENTRY *glDeleteShader) (GLuint shader);
void      (APIENTRY *glDetachShader) (GLuint program, GLuint shader);
void      (APIENTRY *glDisableVertexAttribArray) (GLuint index);
void      (APIENTRY *glEnableVertexAttribArray) (GLuint index);
void      (APIENTRY *glGetActiveAttrib) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void      (APIENTRY *glGetActiveUniform) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void      (APIENTRY *glGetAttachedShaders) (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
GLint     (APIENTRY *glGetAttribLocation) (GLuint program, const GLchar *name);
void      (APIENTRY *glGetProgramiv) (GLuint program, GLenum pname, GLint *params);
void      (APIENTRY *glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void      (APIENTRY *glGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
void      (APIENTRY *glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void      (APIENTRY *glGetShaderSource) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLint     (APIENTRY *glGetUniformLocation) (GLuint program, const GLchar *name);
void      (APIENTRY *glGetUniformfv) (GLuint program, GLint location, GLfloat *params);
void      (APIENTRY *glGetUniformiv) (GLuint program, GLint location, GLint *params);
void      (APIENTRY *glGetVertexAttribdv) (GLuint index, GLenum pname, GLdouble *params);
void      (APIENTRY *glGetVertexAttribfv) (GLuint index, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetVertexAttribiv) (GLuint index, GLenum pname, GLint *params);
void      (APIENTRY *glGetVertexAttribPointerv) (GLuint index, GLenum pname, GLvoid* *pointer);
GLboolean (APIENTRY *glIsProgram) (GLuint program);
GLboolean (APIENTRY *glIsShader) (GLuint shader);
void      (APIENTRY *glLinkProgram) (GLuint program);
void      (APIENTRY *glShaderSource) (GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
void      (APIENTRY *glUseProgram) (GLuint program);
void      (APIENTRY *glUniform1f) (GLint location, GLfloat v0);
void      (APIENTRY *glUniform2f) (GLint location, GLfloat v0, GLfloat v1);
void      (APIENTRY *glUniform3f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void      (APIENTRY *glUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void      (APIENTRY *glUniform1i) (GLint location, GLint v0);
void      (APIENTRY *glUniform2i) (GLint location, GLint v0, GLint v1);
void      (APIENTRY *glUniform3i) (GLint location, GLint v0, GLint v1, GLint v2);
void      (APIENTRY *glUniform4i) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void      (APIENTRY *glUniform1fv) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform2fv) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform3fv) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform4fv) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform1iv) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniform2iv) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniform3iv) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniform4iv) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glValidateProgram) (GLuint program);
void      (APIENTRY *glVertexAttrib1d) (GLuint index, GLdouble x);
void      (APIENTRY *glVertexAttrib1dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib1f) (GLuint index, GLfloat x);
void      (APIENTRY *glVertexAttrib1fv) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib1s) (GLuint index, GLshort x);
void      (APIENTRY *glVertexAttrib1sv) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib2d) (GLuint index, GLdouble x, GLdouble y);
void      (APIENTRY *glVertexAttrib2dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib2f) (GLuint index, GLfloat x, GLfloat y);
void      (APIENTRY *glVertexAttrib2fv) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib2s) (GLuint index, GLshort x, GLshort y);
void      (APIENTRY *glVertexAttrib2sv) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib3d) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glVertexAttrib3dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib3f) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glVertexAttrib3fv) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib3s) (GLuint index, GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glVertexAttrib3sv) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4Nbv) (GLuint index, const GLbyte *v);
void      (APIENTRY *glVertexAttrib4Niv) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttrib4Nsv) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4Nub) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void      (APIENTRY *glVertexAttrib4Nubv) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttrib4Nuiv) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttrib4Nusv) (GLuint index, const GLushort *v);
void      (APIENTRY *glVertexAttrib4bv) (GLuint index, const GLbyte *v);
void      (APIENTRY *glVertexAttrib4d) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glVertexAttrib4dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib4f) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glVertexAttrib4fv) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib4iv) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttrib4s) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void      (APIENTRY *glVertexAttrib4sv) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4ubv) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttrib4uiv) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttrib4usv) (GLuint index, const GLushort *v);
void      (APIENTRY *glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
//@}

/** @name GL_VERSION_2_1
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/2_1.txt
 */
//@{
void      (APIENTRY *glUniformMatrix2x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix3x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix2x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix4x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix3x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix4x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//@}

/** @name GL_VERSION_3_0
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/3_0.txt
 */
//@{
void      (APIENTRY *glColorMaski) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void      (APIENTRY *glGetBooleani_v) (GLenum target, GLuint index, GLboolean *data);
void      (APIENTRY *glGetIntegeri_v) (GLenum target, GLuint index, GLint *data);
void      (APIENTRY *glEnablei) (GLenum target, GLuint index);
void      (APIENTRY *glDisablei) (GLenum target, GLuint index);
GLboolean (APIENTRY *glIsEnabledi) (GLenum target, GLuint index);
void      (APIENTRY *glBeginTransformFeedback) (GLenum primitiveMode);
void      (APIENTRY *glEndTransformFeedback) (void);
void      (APIENTRY *glBindBufferRange) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void      (APIENTRY *glBindBufferBase) (GLenum target, GLuint index, GLuint buffer);
void      (APIENTRY *glTransformFeedbackVaryings) (GLuint program, GLsizei count, const GLchar* *varyings, GLenum bufferMode);
void      (APIENTRY *glGetTransformFeedbackVarying) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
void      (APIENTRY *glClampColor) (GLenum target, GLenum clamp);
void      (APIENTRY *glBeginConditionalRender) (GLuint id, GLenum mode);
void      (APIENTRY *glEndConditionalRender) (void);
void      (APIENTRY *glVertexAttribIPointer) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glGetVertexAttribIiv) (GLuint index, GLenum pname, GLint *params);
void      (APIENTRY *glGetVertexAttribIuiv) (GLuint index, GLenum pname, GLuint *params);
void      (APIENTRY *glVertexAttribI1i) (GLuint index, GLint x);
void      (APIENTRY *glVertexAttribI2i) (GLuint index, GLint x, GLint y);
void      (APIENTRY *glVertexAttribI3i) (GLuint index, GLint x, GLint y, GLint z);
void      (APIENTRY *glVertexAttribI4i) (GLuint index, GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glVertexAttribI1ui) (GLuint index, GLuint x);
void      (APIENTRY *glVertexAttribI2ui) (GLuint index, GLuint x, GLuint y);
void      (APIENTRY *glVertexAttribI3ui) (GLuint index, GLuint x, GLuint y, GLuint z);
void      (APIENTRY *glVertexAttribI4ui) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void      (APIENTRY *glVertexAttribI1iv) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI2iv) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI3iv) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI4iv) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI1uiv) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI2uiv) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI3uiv) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI4uiv) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI4bv) (GLuint index, const GLbyte *v);
void      (APIENTRY *glVertexAttribI4sv) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttribI4ubv) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttribI4usv) (GLuint index, const GLushort *v);
void      (APIENTRY *glGetUniformuiv) (GLuint program, GLint location, GLuint *params);
void      (APIENTRY *glBindFragDataLocation) (GLuint program, GLuint color, const GLchar *name);
GLint     (APIENTRY *glGetFragDataLocation) (GLuint program, const GLchar *name);
void      (APIENTRY *glUniform1ui) (GLint location, GLuint v0);
void      (APIENTRY *glUniform2ui) (GLint location, GLuint v0, GLuint v1);
void      (APIENTRY *glUniform3ui) (GLint location, GLuint v0, GLuint v1, GLuint v2);
void      (APIENTRY *glUniform4ui) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void      (APIENTRY *glUniform1uiv) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glUniform2uiv) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glUniform3uiv) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glUniform4uiv) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glTexParameterIiv) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glTexParameterIuiv) (GLenum target, GLenum pname, const GLuint *params);
void      (APIENTRY *glGetTexParameterIiv) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetTexParameterIuiv) (GLenum target, GLenum pname, GLuint *params);
void      (APIENTRY *glClearBufferiv) (GLenum buffer, GLint drawbuffer, const GLint *value);
void      (APIENTRY *glClearBufferuiv) (GLenum buffer, GLint drawbuffer, const GLuint *value);
void      (APIENTRY *glClearBufferfv) (GLenum buffer, GLint drawbuffer, const GLfloat *value);
void      (APIENTRY *glClearBufferfi) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
const GLubyte* (APIENTRY *glGetStringi) (GLenum name, GLuint index);
//@}

/** @name GL_VERSION_3_1
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/3_1.txt
 */
//@{
void      (APIENTRY *glDrawArraysInstanced) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
void      (APIENTRY *glDrawElementsInstanced) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
void      (APIENTRY *glTexBuffer) (GLenum target, GLenum internalformat, GLuint buffer);
void      (APIENTRY *glPrimitiveRestartIndex) (GLuint index);
//@}

/** @name GL_VERSION_3_2
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/3_2.txt
 */
//@{
void      (APIENTRY *glGetInteger64i_v) (GLenum target, GLuint index, GLint64 *data);
void      (APIENTRY *glGetBufferParameteri64v) (GLenum target, GLenum pname, GLint64 *params);
void      (APIENTRY *glFramebufferTexture) (GLenum target, GLenum attachment, GLuint texture, GLint level);
//@}

/** @name GL_VERSION_3_3
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/3_3.txt
 */
//@{
void      (APIENTRY *glVertexAttribDivisor) (GLuint index, GLuint divisor);
//@}

/** @name GL_VERSION_4_0
 *  See http://oss.sgi.com/projects/ogl-sample/registry/VERSION/4_0.txt
 */
//@{
void      (APIENTRY *glMinSampleShading) (GLclampf value);
void      (APIENTRY *glBlendEquationi) (GLuint buf, GLenum mode);
void      (APIENTRY *glBlendEquationSeparatei) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
void      (APIENTRY *glBlendFunci) (GLuint buf, GLenum src, GLenum dst);
void      (APIENTRY *glBlendFuncSeparatei) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
//@}

/** @name GL_ARB_multitexture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/multitexture.txt
 */
//@{
void      (APIENTRY *glActiveTextureARB) (GLenum texture);
void      (APIENTRY *glClientActiveTextureARB) (GLenum texture);
void      (APIENTRY *glMultiTexCoord1dARB) (GLenum target, GLdouble s);
void      (APIENTRY *glMultiTexCoord1dvARB) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord1fARB) (GLenum target, GLfloat s);
void      (APIENTRY *glMultiTexCoord1fvARB) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord1iARB) (GLenum target, GLint s);
void      (APIENTRY *glMultiTexCoord1ivARB) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord1sARB) (GLenum target, GLshort s);
void      (APIENTRY *glMultiTexCoord1svARB) (GLenum target, const GLshort *v);
void      (APIENTRY *glMultiTexCoord2dARB) (GLenum target, GLdouble s, GLdouble t);
void      (APIENTRY *glMultiTexCoord2dvARB) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord2fARB) (GLenum target, GLfloat s, GLfloat t);
void      (APIENTRY *glMultiTexCoord2fvARB) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord2iARB) (GLenum target, GLint s, GLint t);
void      (APIENTRY *glMultiTexCoord2ivARB) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord2sARB) (GLenum target, GLshort s, GLshort t);
void      (APIENTRY *glMultiTexCoord2svARB) (GLenum target, const GLshort *v);
void      (APIENTRY *glMultiTexCoord3dARB) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
void      (APIENTRY *glMultiTexCoord3dvARB) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord3fARB) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
void      (APIENTRY *glMultiTexCoord3fvARB) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord3iARB) (GLenum target, GLint s, GLint t, GLint r);
void      (APIENTRY *glMultiTexCoord3ivARB) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord3sARB) (GLenum target, GLshort s, GLshort t, GLshort r);
void      (APIENTRY *glMultiTexCoord3svARB) (GLenum target, const GLshort *v);
void      (APIENTRY *glMultiTexCoord4dARB) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void      (APIENTRY *glMultiTexCoord4dvARB) (GLenum target, const GLdouble *v);
void      (APIENTRY *glMultiTexCoord4fARB) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void      (APIENTRY *glMultiTexCoord4fvARB) (GLenum target, const GLfloat *v);
void      (APIENTRY *glMultiTexCoord4iARB) (GLenum target, GLint s, GLint t, GLint r, GLint q);
void      (APIENTRY *glMultiTexCoord4ivARB) (GLenum target, const GLint *v);
void      (APIENTRY *glMultiTexCoord4sARB) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void      (APIENTRY *glMultiTexCoord4svARB) (GLenum target, const GLshort *v);
//@}

/** @name GL_ARB_transpose_matrix
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/transpose_matrix.txt
 */
//@{
void      (APIENTRY *glLoadTransposeMatrixfARB) (const GLfloat *m);
void      (APIENTRY *glLoadTransposeMatrixdARB) (const GLdouble *m);
void      (APIENTRY *glMultTransposeMatrixfARB) (const GLfloat *m);
void      (APIENTRY *glMultTransposeMatrixdARB) (const GLdouble *m);
//@}

/** @name GL_ARB_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/multisample.txt
 */
//@{
void      (APIENTRY *glSampleCoverageARB) (GLclampf value, GLboolean invert);
//@}

/** @name GL_ARB_texture_compression
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_compression.txt
 */
//@{
void      (APIENTRY *glCompressedTexImage3DARB) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexImage2DARB) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexImage1DARB) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexSubImage3DARB) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexSubImage2DARB) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glCompressedTexSubImage1DARB) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
void      (APIENTRY *glGetCompressedTexImageARB) (GLenum target, GLint level, GLvoid *img);
//@}

/** @name GL_ARB_point_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/point_parameters.txt
 */
//@{
void      (APIENTRY *glPointParameterfARB) (GLenum pname, GLfloat param);
void      (APIENTRY *glPointParameterfvARB) (GLenum pname, const GLfloat *params);
//@}

/** @name GL_ARB_vertex_blend
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_blend.txt
 */
//@{
void      (APIENTRY *glWeightbvARB) (GLint size, const GLbyte *weights);
void      (APIENTRY *glWeightsvARB) (GLint size, const GLshort *weights);
void      (APIENTRY *glWeightivARB) (GLint size, const GLint *weights);
void      (APIENTRY *glWeightfvARB) (GLint size, const GLfloat *weights);
void      (APIENTRY *glWeightdvARB) (GLint size, const GLdouble *weights);
void      (APIENTRY *glWeightubvARB) (GLint size, const GLubyte *weights);
void      (APIENTRY *glWeightusvARB) (GLint size, const GLushort *weights);
void      (APIENTRY *glWeightuivARB) (GLint size, const GLuint *weights);
void      (APIENTRY *glWeightPointerARB) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glVertexBlendARB) (GLint count);
//@}

/** @name GL_ARB_matrix_palette
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/matrix_palette.txt
 */
//@{
void      (APIENTRY *glCurrentPaletteMatrixARB) (GLint index);
void      (APIENTRY *glMatrixIndexubvARB) (GLint size, const GLubyte *indices);
void      (APIENTRY *glMatrixIndexusvARB) (GLint size, const GLushort *indices);
void      (APIENTRY *glMatrixIndexuivARB) (GLint size, const GLuint *indices);
void      (APIENTRY *glMatrixIndexPointerARB) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//@}

/** @name GL_ARB_window_pos
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/window_pos.txt
 */
//@{
void      (APIENTRY *glWindowPos2dARB) (GLdouble x, GLdouble y);
void      (APIENTRY *glWindowPos2dvARB) (const GLdouble *v);
void      (APIENTRY *glWindowPos2fARB) (GLfloat x, GLfloat y);
void      (APIENTRY *glWindowPos2fvARB) (const GLfloat *v);
void      (APIENTRY *glWindowPos2iARB) (GLint x, GLint y);
void      (APIENTRY *glWindowPos2ivARB) (const GLint *v);
void      (APIENTRY *glWindowPos2sARB) (GLshort x, GLshort y);
void      (APIENTRY *glWindowPos2svARB) (const GLshort *v);
void      (APIENTRY *glWindowPos3dARB) (GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glWindowPos3dvARB) (const GLdouble *v);
void      (APIENTRY *glWindowPos3fARB) (GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glWindowPos3fvARB) (const GLfloat *v);
void      (APIENTRY *glWindowPos3iARB) (GLint x, GLint y, GLint z);
void      (APIENTRY *glWindowPos3ivARB) (const GLint *v);
void      (APIENTRY *glWindowPos3sARB) (GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glWindowPos3svARB) (const GLshort *v);
//@}

/** @name GL_ARB_vertex_program
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_program.txt
 */
//@{
void      (APIENTRY *glVertexAttrib1dARB) (GLuint index, GLdouble x);
void      (APIENTRY *glVertexAttrib1dvARB) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib1fARB) (GLuint index, GLfloat x);
void      (APIENTRY *glVertexAttrib1fvARB) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib1sARB) (GLuint index, GLshort x);
void      (APIENTRY *glVertexAttrib1svARB) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib2dARB) (GLuint index, GLdouble x, GLdouble y);
void      (APIENTRY *glVertexAttrib2dvARB) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib2fARB) (GLuint index, GLfloat x, GLfloat y);
void      (APIENTRY *glVertexAttrib2fvARB) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib2sARB) (GLuint index, GLshort x, GLshort y);
void      (APIENTRY *glVertexAttrib2svARB) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib3dARB) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glVertexAttrib3dvARB) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib3fARB) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glVertexAttrib3fvARB) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib3sARB) (GLuint index, GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glVertexAttrib3svARB) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4NbvARB) (GLuint index, const GLbyte *v);
void      (APIENTRY *glVertexAttrib4NivARB) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttrib4NsvARB) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4NubARB) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void      (APIENTRY *glVertexAttrib4NubvARB) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttrib4NuivARB) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttrib4NusvARB) (GLuint index, const GLushort *v);
void      (APIENTRY *glVertexAttrib4bvARB) (GLuint index, const GLbyte *v);
void      (APIENTRY *glVertexAttrib4dARB) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glVertexAttrib4dvARB) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib4fARB) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glVertexAttrib4fvARB) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib4ivARB) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttrib4sARB) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void      (APIENTRY *glVertexAttrib4svARB) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4ubvARB) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttrib4uivARB) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttrib4usvARB) (GLuint index, const GLushort *v);
void      (APIENTRY *glVertexAttribPointerARB) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glEnableVertexAttribArrayARB) (GLuint index);
void      (APIENTRY *glDisableVertexAttribArrayARB) (GLuint index);
void      (APIENTRY *glProgramStringARB) (GLenum target, GLenum format, GLsizei len, const GLvoid *string);
void      (APIENTRY *glBindProgramARB) (GLenum target, GLuint program);
void      (APIENTRY *glDeleteProgramsARB) (GLsizei n, const GLuint *programs);
void      (APIENTRY *glGenProgramsARB) (GLsizei n, GLuint *programs);
void      (APIENTRY *glProgramEnvParameter4dARB) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glProgramEnvParameter4dvARB) (GLenum target, GLuint index, const GLdouble *params);
void      (APIENTRY *glProgramEnvParameter4fARB) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glProgramEnvParameter4fvARB) (GLenum target, GLuint index, const GLfloat *params);
void      (APIENTRY *glProgramLocalParameter4dARB) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glProgramLocalParameter4dvARB) (GLenum target, GLuint index, const GLdouble *params);
void      (APIENTRY *glProgramLocalParameter4fARB) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glProgramLocalParameter4fvARB) (GLenum target, GLuint index, const GLfloat *params);
void      (APIENTRY *glGetProgramEnvParameterdvARB) (GLenum target, GLuint index, GLdouble *params);
void      (APIENTRY *glGetProgramEnvParameterfvARB) (GLenum target, GLuint index, GLfloat *params);
void      (APIENTRY *glGetProgramLocalParameterdvARB) (GLenum target, GLuint index, GLdouble *params);
void      (APIENTRY *glGetProgramLocalParameterfvARB) (GLenum target, GLuint index, GLfloat *params);
void      (APIENTRY *glGetProgramivARB) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetProgramStringARB) (GLenum target, GLenum pname, GLvoid *string);
void      (APIENTRY *glGetVertexAttribdvARB) (GLuint index, GLenum pname, GLdouble *params);
void      (APIENTRY *glGetVertexAttribfvARB) (GLuint index, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetVertexAttribivARB) (GLuint index, GLenum pname, GLint *params);
void      (APIENTRY *glGetVertexAttribPointervARB) (GLuint index, GLenum pname, GLvoid* *pointer);
GLboolean (APIENTRY *glIsProgramARB) (GLuint program);
//@}

/** @name GL_ARB_vertex_buffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_buffer_object.txt
 */
//@{
void      (APIENTRY *glBindBufferARB) (GLenum target, GLuint buffer);
void      (APIENTRY *glDeleteBuffersARB) (GLsizei n, const GLuint *buffers);
void      (APIENTRY *glGenBuffersARB) (GLsizei n, GLuint *buffers);
GLboolean (APIENTRY *glIsBufferARB) (GLuint buffer);
void      (APIENTRY *glBufferDataARB) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
void      (APIENTRY *glBufferSubDataARB) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
void      (APIENTRY *glGetBufferSubDataARB) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
GLvoid*   (APIENTRY *glMapBufferARB) (GLenum target, GLenum access);
GLboolean (APIENTRY *glUnmapBufferARB) (GLenum target);
void      (APIENTRY *glGetBufferParameterivARB) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetBufferPointervARB) (GLenum target, GLenum pname, GLvoid* *params);
//@}

/** @name GL_ARB_occlusion_query
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/occlusion_query.txt
 */
//@{
void      (APIENTRY *glGenQueriesARB) (GLsizei n, GLuint *ids);
void      (APIENTRY *glDeleteQueriesARB) (GLsizei n, const GLuint *ids);
GLboolean (APIENTRY *glIsQueryARB) (GLuint id);
void      (APIENTRY *glBeginQueryARB) (GLenum target, GLuint id);
void      (APIENTRY *glEndQueryARB) (GLenum target);
void      (APIENTRY *glGetQueryivARB) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetQueryObjectivARB) (GLuint id, GLenum pname, GLint *params);
void      (APIENTRY *glGetQueryObjectuivARB) (GLuint id, GLenum pname, GLuint *params);
//@}

/** @name GL_ARB_shader_objects
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/shader_objects.txt
 */
//@{
void      (APIENTRY *glDeleteObjectARB) (GLhandleARB obj);
GLhandleARB (APIENTRY *glGetHandleARB) (GLenum pname);
void      (APIENTRY *glDetachObjectARB) (GLhandleARB containerObj, GLhandleARB attachedObj);
GLhandleARB (APIENTRY *glCreateShaderObjectARB) (GLenum shaderType);
void      (APIENTRY *glShaderSourceARB) (GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
void      (APIENTRY *glCompileShaderARB) (GLhandleARB shaderObj);
GLhandleARB (APIENTRY *glCreateProgramObjectARB) (void);
void      (APIENTRY *glAttachObjectARB) (GLhandleARB containerObj, GLhandleARB obj);
void      (APIENTRY *glLinkProgramARB) (GLhandleARB programObj);
void      (APIENTRY *glUseProgramObjectARB) (GLhandleARB programObj);
void      (APIENTRY *glValidateProgramARB) (GLhandleARB programObj);
void      (APIENTRY *glUniform1fARB) (GLint location, GLfloat v0);
void      (APIENTRY *glUniform2fARB) (GLint location, GLfloat v0, GLfloat v1);
void      (APIENTRY *glUniform3fARB) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void      (APIENTRY *glUniform4fARB) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void      (APIENTRY *glUniform1iARB) (GLint location, GLint v0);
void      (APIENTRY *glUniform2iARB) (GLint location, GLint v0, GLint v1);
void      (APIENTRY *glUniform3iARB) (GLint location, GLint v0, GLint v1, GLint v2);
void      (APIENTRY *glUniform4iARB) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void      (APIENTRY *glUniform1fvARB) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform2fvARB) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform3fvARB) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform4fvARB) (GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glUniform1ivARB) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniform2ivARB) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniform3ivARB) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniform4ivARB) (GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glUniformMatrix2fvARB) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix3fvARB) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glUniformMatrix4fvARB) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glGetObjectParameterfvARB) (GLhandleARB obj, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetObjectParameterivARB) (GLhandleARB obj, GLenum pname, GLint *params);
void      (APIENTRY *glGetInfoLogARB) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
void      (APIENTRY *glGetAttachedObjectsARB) (GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
GLint     (APIENTRY *glGetUniformLocationARB) (GLhandleARB programObj, const GLcharARB *name);
void      (APIENTRY *glGetActiveUniformARB) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
void      (APIENTRY *glGetUniformfvARB) (GLhandleARB programObj, GLint location, GLfloat *params);
void      (APIENTRY *glGetUniformivARB) (GLhandleARB programObj, GLint location, GLint *params);
void      (APIENTRY *glGetShaderSourceARB) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
//@}

/** @name GL_ARB_vertex_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_shader.txt
 */
//@{
void      (APIENTRY *glBindAttribLocationARB) (GLhandleARB programObj, GLuint index, const GLcharARB *name);
void      (APIENTRY *glGetActiveAttribARB) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
GLint     (APIENTRY *glGetAttribLocationARB) (GLhandleARB programObj, const GLcharARB *name);
//@}

/** @name GL_ARB_draw_buffers
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/draw_buffers.txt
 */
//@{
void      (APIENTRY *glDrawBuffersARB) (GLsizei n, const GLenum *bufs);
//@}

/** @name GL_ARB_color_buffer_float
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/color_buffer_float.txt
 */
//@{
void      (APIENTRY *glClampColorARB) (GLenum target, GLenum clamp);
//@}

/** @name GL_ARB_draw_instanced
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/draw_instanced.txt
 */
//@{
void      (APIENTRY *glDrawArraysInstancedARB) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
void      (APIENTRY *glDrawElementsInstancedARB) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
//@}

/** @name GL_ARB_framebuffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/framebuffer_object.txt
 */
//@{
GLboolean (APIENTRY *glIsRenderbuffer) (GLuint renderbuffer);
void      (APIENTRY *glBindRenderbuffer) (GLenum target, GLuint renderbuffer);
void      (APIENTRY *glDeleteRenderbuffers) (GLsizei n, const GLuint *renderbuffers);
void      (APIENTRY *glGenRenderbuffers) (GLsizei n, GLuint *renderbuffers);
void      (APIENTRY *glRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void      (APIENTRY *glGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint *params);
GLboolean (APIENTRY *glIsFramebuffer) (GLuint framebuffer);
void      (APIENTRY *glBindFramebuffer) (GLenum target, GLuint framebuffer);
void      (APIENTRY *glDeleteFramebuffers) (GLsizei n, const GLuint *framebuffers);
void      (APIENTRY *glGenFramebuffers) (GLsizei n, GLuint *framebuffers);
GLenum    (APIENTRY *glCheckFramebufferStatus) (GLenum target);
void      (APIENTRY *glFramebufferTexture1D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void      (APIENTRY *glFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void      (APIENTRY *glFramebufferTexture3D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void      (APIENTRY *glFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void      (APIENTRY *glGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
void      (APIENTRY *glGenerateMipmap) (GLenum target);
void      (APIENTRY *glBlitFramebuffer) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
void      (APIENTRY *glRenderbufferStorageMultisample) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void      (APIENTRY *glFramebufferTextureLayer) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
//@}

/** @name GL_ARB_geometry_shader4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/geometry_shader4.txt
 */
//@{
void      (APIENTRY *glProgramParameteriARB) (GLuint program, GLenum pname, GLint value);
void      (APIENTRY *glFramebufferTextureARB) (GLenum target, GLenum attachment, GLuint texture, GLint level);
void      (APIENTRY *glFramebufferTextureLayerARB) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void      (APIENTRY *glFramebufferTextureFaceARB) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
//@}

/** @name GL_ARB_instanced_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/instanced_arrays.txt
 */
//@{
void      (APIENTRY *glVertexAttribDivisorARB) (GLuint index, GLuint divisor);
//@}

/** @name GL_ARB_map_buffer_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/map_buffer_range.txt
 */
//@{
GLvoid*   (APIENTRY *glMapBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void      (APIENTRY *glFlushMappedBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length);
//@}

/** @name GL_ARB_texture_buffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_buffer_object.txt
 */
//@{
void      (APIENTRY *glTexBufferARB) (GLenum target, GLenum internalformat, GLuint buffer);
//@}

/** @name GL_ARB_vertex_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_array_object.txt
 */
//@{
void      (APIENTRY *glBindVertexArray) (GLuint array);
void      (APIENTRY *glDeleteVertexArrays) (GLsizei n, const GLuint *arrays);
void      (APIENTRY *glGenVertexArrays) (GLsizei n, GLuint *arrays);
GLboolean (APIENTRY *glIsVertexArray) (GLuint array);
//@}

/** @name GL_ARB_uniform_buffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/uniform_buffer_object.txt
 */
//@{
void      (APIENTRY *glGetUniformIndices) (GLuint program, GLsizei uniformCount, const GLchar* *uniformNames, GLuint *uniformIndices);
void      (APIENTRY *glGetActiveUniformsiv) (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
void      (APIENTRY *glGetActiveUniformName) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
GLuint    (APIENTRY *glGetUniformBlockIndex) (GLuint program, const GLchar *uniformBlockName);
void      (APIENTRY *glGetActiveUniformBlockiv) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
void      (APIENTRY *glGetActiveUniformBlockName) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
void      (APIENTRY *glUniformBlockBinding) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
//@}

/** @name GL_ARB_copy_buffer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/copy_buffer.txt
 */
//@{
void      (APIENTRY *glCopyBufferSubData) (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
//@}

/** @name GL_ARB_draw_elements_base_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/draw_elements_base_vertex.txt
 */
//@{
void      (APIENTRY *glDrawElementsBaseVertex) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
void      (APIENTRY *glDrawRangeElementsBaseVertex) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
void      (APIENTRY *glDrawElementsInstancedBaseVertex) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
void      (APIENTRY *glMultiDrawElementsBaseVertex) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, const GLint *basevertex);
//@}

/** @name GL_ARB_provoking_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/provoking_vertex.txt
 */
//@{
void      (APIENTRY *glProvokingVertex) (GLenum mode);
//@}

/** @name GL_ARB_sync
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/sync.txt
 */
//@{
GLsync    (APIENTRY *glFenceSync) (GLenum condition, GLbitfield flags);
GLboolean (APIENTRY *glIsSync) (GLsync sync);
void      (APIENTRY *glDeleteSync) (GLsync sync);
GLenum    (APIENTRY *glClientWaitSync) (GLsync sync, GLbitfield flags, GLuint64 timeout);
void      (APIENTRY *glWaitSync) (GLsync sync, GLbitfield flags, GLuint64 timeout);
void      (APIENTRY *glGetInteger64v) (GLenum pname, GLint64 *params);
void      (APIENTRY *glGetSynciv) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
//@}

/** @name GL_ARB_texture_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_multisample.txt
 */
//@{
void      (APIENTRY *glTexImage2DMultisample) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void      (APIENTRY *glTexImage3DMultisample) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void      (APIENTRY *glGetMultisamplefv) (GLenum pname, GLuint index, GLfloat *val);
void      (APIENTRY *glSampleMaski) (GLuint index, GLbitfield mask);
//@}

/** @name GL_ARB_draw_buffers_blend
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/draw_buffers_blend.txt
 */
//@{
void      (APIENTRY *glBlendEquationiARB) (GLuint buf, GLenum mode);
void      (APIENTRY *glBlendEquationSeparateiARB) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
void      (APIENTRY *glBlendFunciARB) (GLuint buf, GLenum src, GLenum dst);
void      (APIENTRY *glBlendFuncSeparateiARB) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
//@}

/** @name GL_ARB_sample_shading
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/sample_shading.txt
 */
//@{
void      (APIENTRY *glMinSampleShadingARB) (GLclampf value);
//@}

/** @name GL_ARB_shading_language_include
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/shading_language_include.txt
 */
//@{
void      (APIENTRY *glNamedStringARB) (GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
void      (APIENTRY *glDeleteNamedStringARB) (GLint namelen, const GLchar *name);
void      (APIENTRY *glCompileShaderIncludeARB) (GLuint shader, GLsizei count, const GLchar* *path, const GLint *length);
GLboolean (APIENTRY *glIsNamedStringARB) (GLint namelen, const GLchar *name);
void      (APIENTRY *glGetNamedStringARB) (GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
void      (APIENTRY *glGetNamedStringivARB) (GLint namelen, const GLchar *name, GLenum pname, GLint *params);
//@}

/** @name GL_ARB_blend_func_extended
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/blend_func_extended.txt
 */
//@{
void      (APIENTRY *glBindFragDataLocationIndexed) (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
GLint     (APIENTRY *glGetFragDataIndex) (GLuint program, const GLchar *name);
//@}

/** @name GL_ARB_sampler_objects
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/sampler_objects.txt
 */
//@{
void      (APIENTRY *glGenSamplers) (GLsizei count, GLuint *samplers);
void      (APIENTRY *glDeleteSamplers) (GLsizei count, const GLuint *samplers);
GLboolean (APIENTRY *glIsSampler) (GLuint sampler);
void      (APIENTRY *glBindSampler) (GLuint unit, GLuint sampler);
void      (APIENTRY *glSamplerParameteri) (GLuint sampler, GLenum pname, GLint param);
void      (APIENTRY *glSamplerParameteriv) (GLuint sampler, GLenum pname, const GLint *param);
void      (APIENTRY *glSamplerParameterf) (GLuint sampler, GLenum pname, GLfloat param);
void      (APIENTRY *glSamplerParameterfv) (GLuint sampler, GLenum pname, const GLfloat *param);
void      (APIENTRY *glSamplerParameterIiv) (GLuint sampler, GLenum pname, const GLint *param);
void      (APIENTRY *glSamplerParameterIuiv) (GLuint sampler, GLenum pname, const GLuint *param);
void      (APIENTRY *glGetSamplerParameteriv) (GLuint sampler, GLenum pname, GLint *params);
void      (APIENTRY *glGetSamplerParameterIiv) (GLuint sampler, GLenum pname, GLint *params);
void      (APIENTRY *glGetSamplerParameterfv) (GLuint sampler, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetSamplerParameterIuiv) (GLuint sampler, GLenum pname, GLuint *params);
//@}

/** @name GL_ARB_timer_query
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/timer_query.txt
 */
//@{
void      (APIENTRY *glQueryCounter) (GLuint id, GLenum target);
void      (APIENTRY *glGetQueryObjecti64v) (GLuint id, GLenum pname, GLint64 *params);
void      (APIENTRY *glGetQueryObjectui64v) (GLuint id, GLenum pname, GLuint64 *params);
//@}

/** @name GL_ARB_vertex_type_2_10_10_10_rev
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_type_2_10_10_10_rev.txt
 */
//@{
void      (APIENTRY *glVertexP2ui) (GLenum type, GLuint value);
void      (APIENTRY *glVertexP2uiv) (GLenum type, const GLuint *value);
void      (APIENTRY *glVertexP3ui) (GLenum type, GLuint value);
void      (APIENTRY *glVertexP3uiv) (GLenum type, const GLuint *value);
void      (APIENTRY *glVertexP4ui) (GLenum type, GLuint value);
void      (APIENTRY *glVertexP4uiv) (GLenum type, const GLuint *value);
void      (APIENTRY *glTexCoordP1ui) (GLenum type, GLuint coords);
void      (APIENTRY *glTexCoordP1uiv) (GLenum type, const GLuint *coords);
void      (APIENTRY *glTexCoordP2ui) (GLenum type, GLuint coords);
void      (APIENTRY *glTexCoordP2uiv) (GLenum type, const GLuint *coords);
void      (APIENTRY *glTexCoordP3ui) (GLenum type, GLuint coords);
void      (APIENTRY *glTexCoordP3uiv) (GLenum type, const GLuint *coords);
void      (APIENTRY *glTexCoordP4ui) (GLenum type, GLuint coords);
void      (APIENTRY *glTexCoordP4uiv) (GLenum type, const GLuint *coords);
void      (APIENTRY *glMultiTexCoordP1ui) (GLenum texture, GLenum type, GLuint coords);
void      (APIENTRY *glMultiTexCoordP1uiv) (GLenum texture, GLenum type, const GLuint *coords);
void      (APIENTRY *glMultiTexCoordP2ui) (GLenum texture, GLenum type, GLuint coords);
void      (APIENTRY *glMultiTexCoordP2uiv) (GLenum texture, GLenum type, const GLuint *coords);
void      (APIENTRY *glMultiTexCoordP3ui) (GLenum texture, GLenum type, GLuint coords);
void      (APIENTRY *glMultiTexCoordP3uiv) (GLenum texture, GLenum type, const GLuint *coords);
void      (APIENTRY *glMultiTexCoordP4ui) (GLenum texture, GLenum type, GLuint coords);
void      (APIENTRY *glMultiTexCoordP4uiv) (GLenum texture, GLenum type, const GLuint *coords);
void      (APIENTRY *glNormalP3ui) (GLenum type, GLuint coords);
void      (APIENTRY *glNormalP3uiv) (GLenum type, const GLuint *coords);
void      (APIENTRY *glColorP3ui) (GLenum type, GLuint color);
void      (APIENTRY *glColorP3uiv) (GLenum type, const GLuint *color);
void      (APIENTRY *glColorP4ui) (GLenum type, GLuint color);
void      (APIENTRY *glColorP4uiv) (GLenum type, const GLuint *color);
void      (APIENTRY *glSecondaryColorP3ui) (GLenum type, GLuint color);
void      (APIENTRY *glSecondaryColorP3uiv) (GLenum type, const GLuint *color);
void      (APIENTRY *glVertexAttribP1ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
void      (APIENTRY *glVertexAttribP1uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void      (APIENTRY *glVertexAttribP2ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
void      (APIENTRY *glVertexAttribP2uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void      (APIENTRY *glVertexAttribP3ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
void      (APIENTRY *glVertexAttribP3uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void      (APIENTRY *glVertexAttribP4ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
void      (APIENTRY *glVertexAttribP4uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
//@}

/** @name GL_ARB_draw_indirect
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/draw_indirect.txt
 */
//@{
void      (APIENTRY *glDrawArraysIndirect) (GLenum mode, const GLvoid *indirect);
void      (APIENTRY *glDrawElementsIndirect) (GLenum mode, GLenum type, const GLvoid *indirect);
//@}

/** @name GL_ARB_gpu_shader_fp64
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/gpu_shader_fp64.txt
 */
//@{
void      (APIENTRY *glUniform1d) (GLint location, GLdouble x);
void      (APIENTRY *glUniform2d) (GLint location, GLdouble x, GLdouble y);
void      (APIENTRY *glUniform3d) (GLint location, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glUniform4d) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glUniform1dv) (GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glUniform2dv) (GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glUniform3dv) (GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glUniform4dv) (GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glUniformMatrix2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix2x3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix2x4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix3x2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix3x4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix4x2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glUniformMatrix4x3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glGetUniformdv) (GLuint program, GLint location, GLdouble *params);
//@}

/** @name GL_ARB_shader_subroutine
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/shader_subroutine.txt
 */
//@{
GLint     (APIENTRY *glGetSubroutineUniformLocation) (GLuint program, GLenum shadertype, const GLchar *name);
GLuint    (APIENTRY *glGetSubroutineIndex) (GLuint program, GLenum shadertype, const GLchar *name);
void      (APIENTRY *glGetActiveSubroutineUniformiv) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
void      (APIENTRY *glGetActiveSubroutineUniformName) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
void      (APIENTRY *glGetActiveSubroutineName) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
void      (APIENTRY *glUniformSubroutinesuiv) (GLenum shadertype, GLsizei count, const GLuint *indices);
void      (APIENTRY *glGetUniformSubroutineuiv) (GLenum shadertype, GLint location, GLuint *params);
void      (APIENTRY *glGetProgramStageiv) (GLuint program, GLenum shadertype, GLenum pname, GLint *values);
//@}

/** @name GL_ARB_tessellation_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/tessellation_shader.txt
 */
//@{
void      (APIENTRY *glPatchParameteri) (GLenum pname, GLint value);
void      (APIENTRY *glPatchParameterfv) (GLenum pname, const GLfloat *values);
//@}

/** @name GL_ARB_transform_feedback2
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/transform_feedback2.txt
 */
//@{
void      (APIENTRY *glBindTransformFeedback) (GLenum target, GLuint id);
void      (APIENTRY *glDeleteTransformFeedbacks) (GLsizei n, const GLuint *ids);
void      (APIENTRY *glGenTransformFeedbacks) (GLsizei n, GLuint *ids);
GLboolean (APIENTRY *glIsTransformFeedback) (GLuint id);
void      (APIENTRY *glPauseTransformFeedback) (void);
void      (APIENTRY *glResumeTransformFeedback) (void);
void      (APIENTRY *glDrawTransformFeedback) (GLenum mode, GLuint id);
//@}

/** @name GL_ARB_transform_feedback3
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/transform_feedback3.txt
 */
//@{
void      (APIENTRY *glDrawTransformFeedbackStream) (GLenum mode, GLuint id, GLuint stream);
void      (APIENTRY *glBeginQueryIndexed) (GLenum target, GLuint index, GLuint id);
void      (APIENTRY *glEndQueryIndexed) (GLenum target, GLuint index);
void      (APIENTRY *glGetQueryIndexediv) (GLenum target, GLuint index, GLenum pname, GLint *params);
//@}

/** @name GL_ARB_ES2_compatibility
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/ES2_compatibility.txt
 */
//@{
void      (APIENTRY *glReleaseShaderCompiler) (void);
void      (APIENTRY *glShaderBinary) (GLsizei count, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
void      (APIENTRY *glGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
void      (APIENTRY *glDepthRangef) (GLclampf n, GLclampf f);
void      (APIENTRY *glClearDepthf) (GLclampf d);
//@}

/** @name GL_ARB_get_program_binary
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/get_program_binary.txt
 */
//@{
void      (APIENTRY *glGetProgramBinary) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
void      (APIENTRY *glProgramBinary) (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
void      (APIENTRY *glProgramParameteri) (GLuint program, GLenum pname, GLint value);
//@}

/** @name GL_ARB_separate_shader_objects
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/separate_shader_objects.txt
 */
//@{
void      (APIENTRY *glUseProgramStages) (GLuint pipeline, GLbitfield stages, GLuint program);
void      (APIENTRY *glActiveShaderProgram) (GLuint pipeline, GLuint program);
GLuint    (APIENTRY *glCreateShaderProgramv) (GLenum type, GLsizei count, const GLchar* *strings);
void      (APIENTRY *glBindProgramPipeline) (GLuint pipeline);
void      (APIENTRY *glDeleteProgramPipelines) (GLsizei n, const GLuint *pipelines);
void      (APIENTRY *glGenProgramPipelines) (GLsizei n, GLuint *pipelines);
GLboolean (APIENTRY *glIsProgramPipeline) (GLuint pipeline);
void      (APIENTRY *glGetProgramPipelineiv) (GLuint pipeline, GLenum pname, GLint *params);
void      (APIENTRY *glProgramUniform1i) (GLuint program, GLint location, GLint v0);
void      (APIENTRY *glProgramUniform1iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform1f) (GLuint program, GLint location, GLfloat v0);
void      (APIENTRY *glProgramUniform1fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform1d) (GLuint program, GLint location, GLdouble v0);
void      (APIENTRY *glProgramUniform1dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform1ui) (GLuint program, GLint location, GLuint v0);
void      (APIENTRY *glProgramUniform1uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniform2i) (GLuint program, GLint location, GLint v0, GLint v1);
void      (APIENTRY *glProgramUniform2iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform2f) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
void      (APIENTRY *glProgramUniform2fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform2d) (GLuint program, GLint location, GLdouble v0, GLdouble v1);
void      (APIENTRY *glProgramUniform2dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform2ui) (GLuint program, GLint location, GLuint v0, GLuint v1);
void      (APIENTRY *glProgramUniform2uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniform3i) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void      (APIENTRY *glProgramUniform3iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform3f) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void      (APIENTRY *glProgramUniform3fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform3d) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
void      (APIENTRY *glProgramUniform3dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform3ui) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void      (APIENTRY *glProgramUniform3uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniform4i) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void      (APIENTRY *glProgramUniform4iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform4f) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void      (APIENTRY *glProgramUniform4fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform4d) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
void      (APIENTRY *glProgramUniform4dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform4ui) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void      (APIENTRY *glProgramUniform4uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniformMatrix2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix2x3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix3x2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix2x4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix4x2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix3x4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix4x3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix2x3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix3x2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix2x4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix4x2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix3x4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix4x3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glValidateProgramPipeline) (GLuint pipeline);
void      (APIENTRY *glGetProgramPipelineInfoLog) (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
//@}

/** @name GL_ARB_vertex_attrib_64bit
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_attrib_64bit.txt
 */
//@{
void      (APIENTRY *glVertexAttribL1d) (GLuint index, GLdouble x);
void      (APIENTRY *glVertexAttribL2d) (GLuint index, GLdouble x, GLdouble y);
void      (APIENTRY *glVertexAttribL3d) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glVertexAttribL4d) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glVertexAttribL1dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribL2dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribL3dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribL4dv) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribLPointer) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glGetVertexAttribLdv) (GLuint index, GLenum pname, GLdouble *params);
//@}

/** @name GL_ARB_viewport_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/viewport_array.txt
 */
//@{
void      (APIENTRY *glViewportArrayv) (GLuint first, GLsizei count, const GLfloat *v);
void      (APIENTRY *glViewportIndexedf) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void      (APIENTRY *glViewportIndexedfv) (GLuint index, const GLfloat *v);
void      (APIENTRY *glScissorArrayv) (GLuint first, GLsizei count, const GLint *v);
void      (APIENTRY *glScissorIndexed) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
void      (APIENTRY *glScissorIndexedv) (GLuint index, const GLint *v);
void      (APIENTRY *glDepthRangeArrayv) (GLuint first, GLsizei count, const GLclampd *v);
void      (APIENTRY *glDepthRangeIndexed) (GLuint index, GLclampd n, GLclampd f);
void      (APIENTRY *glGetFloati_v) (GLenum target, GLuint index, GLfloat *data);
void      (APIENTRY *glGetDoublei_v) (GLenum target, GLuint index, GLdouble *data);
//@}

/** @name GL_ARB_cl_event
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/cl_event.txt
 */
//@{
GLsync    (APIENTRY *glCreateSyncFromCLeventARB) (struct _cl_context * context, struct _cl_event * event, GLbitfield flags);
//@}

/** @name GL_ARB_debug_output
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/debug_output.txt
 */
//@{
void      (APIENTRY *glDebugMessageControlARB) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
void      (APIENTRY *glDebugMessageInsertARB) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
void      (APIENTRY *glDebugMessageCallbackARB) (GLDEBUGPROCARB callback, const GLvoid *userParam);
GLuint    (APIENTRY *glGetDebugMessageLogARB) (GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
//@}

/** @name GL_ARB_robustness
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ARB/robustness.txt
 */
//@{
GLenum    (APIENTRY *glGetGraphicsResetStatusARB) (void);
void      (APIENTRY *glGetnMapdvARB) (GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
void      (APIENTRY *glGetnMapfvARB) (GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
void      (APIENTRY *glGetnMapivARB) (GLenum target, GLenum query, GLsizei bufSize, GLint *v);
void      (APIENTRY *glGetnPixelMapfvARB) (GLenum map, GLsizei bufSize, GLfloat *values);
void      (APIENTRY *glGetnPixelMapuivARB) (GLenum map, GLsizei bufSize, GLuint *values);
void      (APIENTRY *glGetnPixelMapusvARB) (GLenum map, GLsizei bufSize, GLushort *values);
void      (APIENTRY *glGetnPolygonStippleARB) (GLsizei bufSize, GLubyte *pattern);
void      (APIENTRY *glGetnColorTableARB) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *table);
void      (APIENTRY *glGetnConvolutionFilterARB) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *image);
void      (APIENTRY *glGetnSeparableFilterARB) (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid *row, GLsizei columnBufSize, GLvoid *column, GLvoid *span);
void      (APIENTRY *glGetnHistogramARB) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
void      (APIENTRY *glGetnMinmaxARB) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
void      (APIENTRY *glGetnTexImageARB) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *img);
void      (APIENTRY *glReadnPixelsARB) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid *data);
void      (APIENTRY *glGetnCompressedTexImageARB) (GLenum target, GLint lod, GLsizei bufSize, GLvoid *img);
void      (APIENTRY *glGetnUniformfvARB) (GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
void      (APIENTRY *glGetnUniformivARB) (GLuint program, GLint location, GLsizei bufSize, GLint *params);
void      (APIENTRY *glGetnUniformuivARB) (GLuint program, GLint location, GLsizei bufSize, GLuint *params);
void      (APIENTRY *glGetnUniformdvARB) (GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
//@}

/** @name GL_EXT_blend_color
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_color.txt
 */
//@{
void      (APIENTRY *glBlendColorEXT) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
//@}

/** @name GL_EXT_polygon_offset
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/polygon_offset.txt
 */
//@{
void      (APIENTRY *glPolygonOffsetEXT) (GLfloat factor, GLfloat bias);
//@}

/** @name GL_EXT_texture3D
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture3D.txt
 */
//@{
void      (APIENTRY *glTexImage3DEXT) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTexSubImage3DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
//@}

/** @name GL_SGIS_texture_filter4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture_filter4.txt
 */
//@{
void      (APIENTRY *glGetTexFilterFuncSGIS) (GLenum target, GLenum filter, GLfloat *weights);
void      (APIENTRY *glTexFilterFuncSGIS) (GLenum target, GLenum filter, GLsizei n, const GLfloat *weights);
//@}

/** @name GL_EXT_subtexture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/subtexture.txt
 */
//@{
void      (APIENTRY *glTexSubImage1DEXT) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTexSubImage2DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
//@}

/** @name GL_EXT_copy_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/copy_texture.txt
 */
//@{
void      (APIENTRY *glCopyTexImage1DEXT) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void      (APIENTRY *glCopyTexImage2DEXT) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void      (APIENTRY *glCopyTexSubImage1DEXT) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glCopyTexSubImage2DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glCopyTexSubImage3DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//@}

/** @name GL_EXT_histogram
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/histogram.txt
 */
//@{
void      (APIENTRY *glGetHistogramEXT) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void      (APIENTRY *glGetHistogramParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetHistogramParameterivEXT) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetMinmaxEXT) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void      (APIENTRY *glGetMinmaxParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMinmaxParameterivEXT) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glHistogramEXT) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
void      (APIENTRY *glMinmaxEXT) (GLenum target, GLenum internalformat, GLboolean sink);
void      (APIENTRY *glResetHistogramEXT) (GLenum target);
void      (APIENTRY *glResetMinmaxEXT) (GLenum target);
//@}

/** @name GL_EXT_convolution
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/convolution.txt
 */
//@{
void      (APIENTRY *glConvolutionFilter1DEXT) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
void      (APIENTRY *glConvolutionFilter2DEXT) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
void      (APIENTRY *glConvolutionParameterfEXT) (GLenum target, GLenum pname, GLfloat params);
void      (APIENTRY *glConvolutionParameterfvEXT) (GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glConvolutionParameteriEXT) (GLenum target, GLenum pname, GLint params);
void      (APIENTRY *glConvolutionParameterivEXT) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glCopyConvolutionFilter1DEXT) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glCopyConvolutionFilter2DEXT) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glGetConvolutionFilterEXT) (GLenum target, GLenum format, GLenum type, GLvoid *image);
void      (APIENTRY *glGetConvolutionParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetConvolutionParameterivEXT) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetSeparableFilterEXT) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
void      (APIENTRY *glSeparableFilter2DEXT) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
//@}

/** @name GL_SGI_color_table
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGI/color_table.txt
 */
//@{
void      (APIENTRY *glColorTableSGI) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void      (APIENTRY *glColorTableParameterfvSGI) (GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glColorTableParameterivSGI) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glCopyColorTableSGI) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glGetColorTableSGI) (GLenum target, GLenum format, GLenum type, GLvoid *table);
void      (APIENTRY *glGetColorTableParameterfvSGI) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetColorTableParameterivSGI) (GLenum target, GLenum pname, GLint *params);
//@}

/** @name GL_SGIX_pixel_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/pixel_texture.txt
 */
//@{
void      (APIENTRY *glPixelTexGenSGIX) (GLenum mode);
//@}

/** @name GL_SGIS_pixel_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/pixel_texture.txt
 */
//@{
void      (APIENTRY *glPixelTexGenParameteriSGIS) (GLenum pname, GLint param);
void      (APIENTRY *glPixelTexGenParameterivSGIS) (GLenum pname, const GLint *params);
void      (APIENTRY *glPixelTexGenParameterfSGIS) (GLenum pname, GLfloat param);
void      (APIENTRY *glPixelTexGenParameterfvSGIS) (GLenum pname, const GLfloat *params);
void      (APIENTRY *glGetPixelTexGenParameterivSGIS) (GLenum pname, GLint *params);
void      (APIENTRY *glGetPixelTexGenParameterfvSGIS) (GLenum pname, GLfloat *params);
//@}

/** @name GL_SGIS_texture4D
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture4D.txt
 */
//@{
void      (APIENTRY *glTexImage4DSGIS) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTexSubImage4DSGIS) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid *pixels);
//@}

/** @name GL_EXT_texture_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_object.txt
 */
//@{
GLboolean (APIENTRY *glAreTexturesResidentEXT) (GLsizei n, const GLuint *textures, GLboolean *residences);
void      (APIENTRY *glBindTextureEXT) (GLenum target, GLuint texture);
void      (APIENTRY *glDeleteTexturesEXT) (GLsizei n, const GLuint *textures);
void      (APIENTRY *glGenTexturesEXT) (GLsizei n, GLuint *textures);
GLboolean (APIENTRY *glIsTextureEXT) (GLuint texture);
void      (APIENTRY *glPrioritizeTexturesEXT) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
//@}

/** @name GL_SGIS_detail_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/detail_texture.txt
 */
//@{
void      (APIENTRY *glDetailTexFuncSGIS) (GLenum target, GLsizei n, const GLfloat *points);
void      (APIENTRY *glGetDetailTexFuncSGIS) (GLenum target, GLfloat *points);
//@}

/** @name GL_SGIS_sharpen_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/sharpen_texture.txt
 */
//@{
void      (APIENTRY *glSharpenTexFuncSGIS) (GLenum target, GLsizei n, const GLfloat *points);
void      (APIENTRY *glGetSharpenTexFuncSGIS) (GLenum target, GLfloat *points);
//@}

/** @name GL_SGIS_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/multisample.txt
 */
//@{
void      (APIENTRY *glSampleMaskSGIS) (GLclampf value, GLboolean invert);
void      (APIENTRY *glSamplePatternSGIS) (GLenum pattern);
//@}

/** @name GL_EXT_vertex_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_array.txt
 */
//@{
void      (APIENTRY *glArrayElementEXT) (GLint i);
void      (APIENTRY *glColorPointerEXT) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void      (APIENTRY *glDrawArraysEXT) (GLenum mode, GLint first, GLsizei count);
void      (APIENTRY *glEdgeFlagPointerEXT) (GLsizei stride, GLsizei count, const GLboolean *pointer);
void      (APIENTRY *glGetPointervEXT) (GLenum pname, GLvoid* *params);
void      (APIENTRY *glIndexPointerEXT) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void      (APIENTRY *glNormalPointerEXT) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void      (APIENTRY *glTexCoordPointerEXT) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void      (APIENTRY *glVertexPointerEXT) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
//@}

/** @name GL_EXT_blend_minmax
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_minmax.txt
 */
//@{
void      (APIENTRY *glBlendEquationEXT) (GLenum mode);
//@}

/** @name GL_SGIX_sprite
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/sprite.txt
 */
//@{
void      (APIENTRY *glSpriteParameterfSGIX) (GLenum pname, GLfloat param);
void      (APIENTRY *glSpriteParameterfvSGIX) (GLenum pname, const GLfloat *params);
void      (APIENTRY *glSpriteParameteriSGIX) (GLenum pname, GLint param);
void      (APIENTRY *glSpriteParameterivSGIX) (GLenum pname, const GLint *params);
//@}

/** @name GL_EXT_point_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/point_parameters.txt
 */
//@{
void      (APIENTRY *glPointParameterfEXT) (GLenum pname, GLfloat param);
void      (APIENTRY *glPointParameterfvEXT) (GLenum pname, const GLfloat *params);
//@}

/** @name GL_SGIS_point_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/point_parameters.txt
 */
//@{
void      (APIENTRY *glPointParameterfSGIS) (GLenum pname, GLfloat param);
void      (APIENTRY *glPointParameterfvSGIS) (GLenum pname, const GLfloat *params);
//@}

/** @name GL_SGIX_instruments
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/instruments.txt
 */
//@{
GLint     (APIENTRY *glGetInstrumentsSGIX) (void);
void      (APIENTRY *glInstrumentsBufferSGIX) (GLsizei size, GLint *buffer);
GLint     (APIENTRY *glPollInstrumentsSGIX) (GLint *marker_p);
void      (APIENTRY *glReadInstrumentsSGIX) (GLint marker);
void      (APIENTRY *glStartInstrumentsSGIX) (void);
void      (APIENTRY *glStopInstrumentsSGIX) (GLint marker);
//@}

/** @name GL_SGIX_framezoom
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/framezoom.txt
 */
//@{
void      (APIENTRY *glFrameZoomSGIX) (GLint factor);
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
void      (APIENTRY *glDeformationMap3dSGIX) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points);
void      (APIENTRY *glDeformationMap3fSGIX) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points);
void      (APIENTRY *glDeformSGIX) (GLbitfield mask);
void      (APIENTRY *glLoadIdentityDeformationMapSGIX) (GLbitfield mask);
//@}

/** @name GL_SGIX_reference_plane
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/reference_plane.txt
 */
//@{
void      (APIENTRY *glReferencePlaneSGIX) (const GLdouble *equation);
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
void      (APIENTRY *glFogFuncSGIS) (GLsizei n, const GLfloat *points);
void      (APIENTRY *glGetFogFuncSGIS) (GLfloat *points);
//@}

/** @name GL_HP_image_transform
 *  See http://oss.sgi.com/projects/ogl-sample/registry/HP/image_transform.txt
 */
//@{
void      (APIENTRY *glImageTransformParameteriHP) (GLenum target, GLenum pname, GLint param);
void      (APIENTRY *glImageTransformParameterfHP) (GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY *glImageTransformParameterivHP) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glImageTransformParameterfvHP) (GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glGetImageTransformParameterivHP) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetImageTransformParameterfvHP) (GLenum target, GLenum pname, GLfloat *params);
//@}

/** @name GL_EXT_color_subtable
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/color_subtable.txt
 */
//@{
void      (APIENTRY *glColorSubTableEXT) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
void      (APIENTRY *glCopyColorSubTableEXT) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
//@}

/** @name GL_PGI_misc_hints
 *  See http://oss.sgi.com/projects/ogl-sample/registry/PGI/misc_hints.txt
 */
//@{
void      (APIENTRY *glHintPGI) (GLenum target, GLint mode);
//@}

/** @name GL_EXT_paletted_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/paletted_texture.txt
 */
//@{
void      (APIENTRY *glColorTableEXT) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void      (APIENTRY *glGetColorTableEXT) (GLenum target, GLenum format, GLenum type, GLvoid *data);
void      (APIENTRY *glGetColorTableParameterivEXT) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetColorTableParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
//@}

/** @name GL_SGIX_list_priority
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/list_priority.txt
 */
//@{
void      (APIENTRY *glGetListParameterfvSGIX) (GLuint list, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetListParameterivSGIX) (GLuint list, GLenum pname, GLint *params);
void      (APIENTRY *glListParameterfSGIX) (GLuint list, GLenum pname, GLfloat param);
void      (APIENTRY *glListParameterfvSGIX) (GLuint list, GLenum pname, const GLfloat *params);
void      (APIENTRY *glListParameteriSGIX) (GLuint list, GLenum pname, GLint param);
void      (APIENTRY *glListParameterivSGIX) (GLuint list, GLenum pname, const GLint *params);
//@}

/** @name GL_EXT_index_material
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/index_material.txt
 */
//@{
void      (APIENTRY *glIndexMaterialEXT) (GLenum face, GLenum mode);
//@}

/** @name GL_EXT_index_func
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/index_func.txt
 */
//@{
void      (APIENTRY *glIndexFuncEXT) (GLenum func, GLclampf ref);
//@}

/** @name GL_EXT_compiled_vertex_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/compiled_vertex_array.txt
 */
//@{
void      (APIENTRY *glLockArraysEXT) (GLint first, GLsizei count);
void      (APIENTRY *glUnlockArraysEXT) (void);
//@}

/** @name GL_EXT_cull_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/cull_vertex.txt
 */
//@{
void      (APIENTRY *glCullParameterdvEXT) (GLenum pname, GLdouble *params);
void      (APIENTRY *glCullParameterfvEXT) (GLenum pname, GLfloat *params);
//@}

/** @name GL_SGIX_fragment_lighting
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/fragment_lighting.txt
 */
//@{
void      (APIENTRY *glFragmentColorMaterialSGIX) (GLenum face, GLenum mode);
void      (APIENTRY *glFragmentLightfSGIX) (GLenum light, GLenum pname, GLfloat param);
void      (APIENTRY *glFragmentLightfvSGIX) (GLenum light, GLenum pname, const GLfloat *params);
void      (APIENTRY *glFragmentLightiSGIX) (GLenum light, GLenum pname, GLint param);
void      (APIENTRY *glFragmentLightivSGIX) (GLenum light, GLenum pname, const GLint *params);
void      (APIENTRY *glFragmentLightModelfSGIX) (GLenum pname, GLfloat param);
void      (APIENTRY *glFragmentLightModelfvSGIX) (GLenum pname, const GLfloat *params);
void      (APIENTRY *glFragmentLightModeliSGIX) (GLenum pname, GLint param);
void      (APIENTRY *glFragmentLightModelivSGIX) (GLenum pname, const GLint *params);
void      (APIENTRY *glFragmentMaterialfSGIX) (GLenum face, GLenum pname, GLfloat param);
void      (APIENTRY *glFragmentMaterialfvSGIX) (GLenum face, GLenum pname, const GLfloat *params);
void      (APIENTRY *glFragmentMaterialiSGIX) (GLenum face, GLenum pname, GLint param);
void      (APIENTRY *glFragmentMaterialivSGIX) (GLenum face, GLenum pname, const GLint *params);
void      (APIENTRY *glGetFragmentLightfvSGIX) (GLenum light, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetFragmentLightivSGIX) (GLenum light, GLenum pname, GLint *params);
void      (APIENTRY *glGetFragmentMaterialfvSGIX) (GLenum face, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetFragmentMaterialivSGIX) (GLenum face, GLenum pname, GLint *params);
void      (APIENTRY *glLightEnviSGIX) (GLenum pname, GLint param);
//@}

/** @name GL_EXT_draw_range_elements
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/draw_range_elements.txt
 */
//@{
void      (APIENTRY *glDrawRangeElementsEXT) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
//@}

/** @name GL_EXT_light_texture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/light_texture.txt
 */
//@{
void      (APIENTRY *glApplyTextureEXT) (GLenum mode);
void      (APIENTRY *glTextureLightEXT) (GLenum pname);
void      (APIENTRY *glTextureMaterialEXT) (GLenum face, GLenum mode);
//@}

/** @name GL_SGIX_async
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/async.txt
 */
//@{
void      (APIENTRY *glAsyncMarkerSGIX) (GLuint marker);
GLint     (APIENTRY *glFinishAsyncSGIX) (GLuint *markerp);
GLint     (APIENTRY *glPollAsyncSGIX) (GLuint *markerp);
GLuint    (APIENTRY *glGenAsyncMarkersSGIX) (GLsizei range);
void      (APIENTRY *glDeleteAsyncMarkersSGIX) (GLuint marker, GLsizei range);
GLboolean (APIENTRY *glIsAsyncMarkerSGIX) (GLuint marker);
//@}

/** @name GL_INTEL_parallel_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/INTEL/parallel_arrays.txt
 */
//@{
void      (APIENTRY *glVertexPointervINTEL) (GLint size, GLenum type, const GLvoid* *pointer);
void      (APIENTRY *glNormalPointervINTEL) (GLenum type, const GLvoid* *pointer);
void      (APIENTRY *glColorPointervINTEL) (GLint size, GLenum type, const GLvoid* *pointer);
void      (APIENTRY *glTexCoordPointervINTEL) (GLint size, GLenum type, const GLvoid* *pointer);
//@}

/** @name GL_EXT_pixel_transform
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/pixel_transform.txt
 */
//@{
void      (APIENTRY *glPixelTransformParameteriEXT) (GLenum target, GLenum pname, GLint param);
void      (APIENTRY *glPixelTransformParameterfEXT) (GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY *glPixelTransformParameterivEXT) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glPixelTransformParameterfvEXT) (GLenum target, GLenum pname, const GLfloat *params);
//@}

/** @name GL_EXT_secondary_color
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/secondary_color.txt
 */
//@{
void      (APIENTRY *glSecondaryColor3bEXT) (GLbyte red, GLbyte green, GLbyte blue);
void      (APIENTRY *glSecondaryColor3bvEXT) (const GLbyte *v);
void      (APIENTRY *glSecondaryColor3dEXT) (GLdouble red, GLdouble green, GLdouble blue);
void      (APIENTRY *glSecondaryColor3dvEXT) (const GLdouble *v);
void      (APIENTRY *glSecondaryColor3fEXT) (GLfloat red, GLfloat green, GLfloat blue);
void      (APIENTRY *glSecondaryColor3fvEXT) (const GLfloat *v);
void      (APIENTRY *glSecondaryColor3iEXT) (GLint red, GLint green, GLint blue);
void      (APIENTRY *glSecondaryColor3ivEXT) (const GLint *v);
void      (APIENTRY *glSecondaryColor3sEXT) (GLshort red, GLshort green, GLshort blue);
void      (APIENTRY *glSecondaryColor3svEXT) (const GLshort *v);
void      (APIENTRY *glSecondaryColor3ubEXT) (GLubyte red, GLubyte green, GLubyte blue);
void      (APIENTRY *glSecondaryColor3ubvEXT) (const GLubyte *v);
void      (APIENTRY *glSecondaryColor3uiEXT) (GLuint red, GLuint green, GLuint blue);
void      (APIENTRY *glSecondaryColor3uivEXT) (const GLuint *v);
void      (APIENTRY *glSecondaryColor3usEXT) (GLushort red, GLushort green, GLushort blue);
void      (APIENTRY *glSecondaryColor3usvEXT) (const GLushort *v);
void      (APIENTRY *glSecondaryColorPointerEXT) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//@}

/** @name GL_EXT_texture_perturb_normal
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_perturb_normal.txt
 */
//@{
void      (APIENTRY *glTextureNormalEXT) (GLenum mode);
//@}

/** @name GL_EXT_multi_draw_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/multi_draw_arrays.txt
 */
//@{
void      (APIENTRY *glMultiDrawArraysEXT) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
void      (APIENTRY *glMultiDrawElementsEXT) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);
//@}

/** @name GL_EXT_fog_coord
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/fog_coord.txt
 */
//@{
void      (APIENTRY *glFogCoordfEXT) (GLfloat coord);
void      (APIENTRY *glFogCoordfvEXT) (const GLfloat *coord);
void      (APIENTRY *glFogCoorddEXT) (GLdouble coord);
void      (APIENTRY *glFogCoorddvEXT) (const GLdouble *coord);
void      (APIENTRY *glFogCoordPointerEXT) (GLenum type, GLsizei stride, const GLvoid *pointer);
//@}

/** @name GL_EXT_coordinate_frame
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/coordinate_frame.txt
 */
//@{
void      (APIENTRY *glTangent3bEXT) (GLbyte tx, GLbyte ty, GLbyte tz);
void      (APIENTRY *glTangent3bvEXT) (const GLbyte *v);
void      (APIENTRY *glTangent3dEXT) (GLdouble tx, GLdouble ty, GLdouble tz);
void      (APIENTRY *glTangent3dvEXT) (const GLdouble *v);
void      (APIENTRY *glTangent3fEXT) (GLfloat tx, GLfloat ty, GLfloat tz);
void      (APIENTRY *glTangent3fvEXT) (const GLfloat *v);
void      (APIENTRY *glTangent3iEXT) (GLint tx, GLint ty, GLint tz);
void      (APIENTRY *glTangent3ivEXT) (const GLint *v);
void      (APIENTRY *glTangent3sEXT) (GLshort tx, GLshort ty, GLshort tz);
void      (APIENTRY *glTangent3svEXT) (const GLshort *v);
void      (APIENTRY *glBinormal3bEXT) (GLbyte bx, GLbyte by, GLbyte bz);
void      (APIENTRY *glBinormal3bvEXT) (const GLbyte *v);
void      (APIENTRY *glBinormal3dEXT) (GLdouble bx, GLdouble by, GLdouble bz);
void      (APIENTRY *glBinormal3dvEXT) (const GLdouble *v);
void      (APIENTRY *glBinormal3fEXT) (GLfloat bx, GLfloat by, GLfloat bz);
void      (APIENTRY *glBinormal3fvEXT) (const GLfloat *v);
void      (APIENTRY *glBinormal3iEXT) (GLint bx, GLint by, GLint bz);
void      (APIENTRY *glBinormal3ivEXT) (const GLint *v);
void      (APIENTRY *glBinormal3sEXT) (GLshort bx, GLshort by, GLshort bz);
void      (APIENTRY *glBinormal3svEXT) (const GLshort *v);
void      (APIENTRY *glTangentPointerEXT) (GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glBinormalPointerEXT) (GLenum type, GLsizei stride, const GLvoid *pointer);
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
void      (APIENTRY *glGlobalAlphaFactorbSUN) (GLbyte factor);
void      (APIENTRY *glGlobalAlphaFactorsSUN) (GLshort factor);
void      (APIENTRY *glGlobalAlphaFactoriSUN) (GLint factor);
void      (APIENTRY *glGlobalAlphaFactorfSUN) (GLfloat factor);
void      (APIENTRY *glGlobalAlphaFactordSUN) (GLdouble factor);
void      (APIENTRY *glGlobalAlphaFactorubSUN) (GLubyte factor);
void      (APIENTRY *glGlobalAlphaFactorusSUN) (GLushort factor);
void      (APIENTRY *glGlobalAlphaFactoruiSUN) (GLuint factor);
//@}

/** @name GL_SUN_triangle_list
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/triangle_list.txt
 */
//@{
void      (APIENTRY *glReplacementCodeuiSUN) (GLuint code);
void      (APIENTRY *glReplacementCodeusSUN) (GLushort code);
void      (APIENTRY *glReplacementCodeubSUN) (GLubyte code);
void      (APIENTRY *glReplacementCodeuivSUN) (const GLuint *code);
void      (APIENTRY *glReplacementCodeusvSUN) (const GLushort *code);
void      (APIENTRY *glReplacementCodeubvSUN) (const GLubyte *code);
void      (APIENTRY *glReplacementCodePointerSUN) (GLenum type, GLsizei stride, const GLvoid* *pointer);
//@}

/** @name GL_SUN_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/vertex.txt
 */
//@{
void      (APIENTRY *glColor4ubVertex2fSUN) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
void      (APIENTRY *glColor4ubVertex2fvSUN) (const GLubyte *c, const GLfloat *v);
void      (APIENTRY *glColor4ubVertex3fSUN) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glColor4ubVertex3fvSUN) (const GLubyte *c, const GLfloat *v);
void      (APIENTRY *glColor3fVertex3fSUN) (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glColor3fVertex3fvSUN) (const GLfloat *c, const GLfloat *v);
void      (APIENTRY *glNormal3fVertex3fSUN) (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glNormal3fVertex3fvSUN) (const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glColor4fNormal3fVertex3fSUN) (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glColor4fNormal3fVertex3fvSUN) (const GLfloat *c, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glTexCoord2fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glTexCoord2fVertex3fvSUN) (const GLfloat *tc, const GLfloat *v);
void      (APIENTRY *glTexCoord4fVertex4fSUN) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glTexCoord4fVertex4fvSUN) (const GLfloat *tc, const GLfloat *v);
void      (APIENTRY *glTexCoord2fColor4ubVertex3fSUN) (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glTexCoord2fColor4ubVertex3fvSUN) (const GLfloat *tc, const GLubyte *c, const GLfloat *v);
void      (APIENTRY *glTexCoord2fColor3fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glTexCoord2fColor3fVertex3fvSUN) (const GLfloat *tc, const GLfloat *c, const GLfloat *v);
void      (APIENTRY *glTexCoord2fNormal3fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glTexCoord2fNormal3fVertex3fvSUN) (const GLfloat *tc, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glTexCoord2fColor4fNormal3fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glTexCoord4fColor4fNormal3fVertex4fSUN) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glTexCoord4fColor4fNormal3fVertex4fvSUN) (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiVertex3fSUN) (GLuint rc, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiVertex3fvSUN) (const GLuint *rc, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiColor4ubVertex3fSUN) (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiColor4ubVertex3fvSUN) (const GLuint *rc, const GLubyte *c, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiColor3fVertex3fSUN) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiColor3fVertex3fvSUN) (const GLuint *rc, const GLfloat *c, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiNormal3fVertex3fSUN) (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiNormal3fVertex3fvSUN) (const GLuint *rc, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiColor4fNormal3fVertex3fSUN) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiColor4fNormal3fVertex3fvSUN) (const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiTexCoord2fVertex3fSUN) (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiTexCoord2fVertex3fvSUN) (const GLuint *rc, const GLfloat *tc, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN) (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN) (const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
void      (APIENTRY *glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN) (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
//@}

/** @name GL_EXT_blend_func_separate
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_func_separate.txt
 */
//@{
void      (APIENTRY *glBlendFuncSeparateEXT) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
//@}

/** @name GL_INGR_blend_func_separate
 *  See http://oss.sgi.com/projects/ogl-sample/registry/INGR/blend_func_separate.txt
 */
//@{
void      (APIENTRY *glBlendFuncSeparateINGR) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
//@}

/** @name GL_EXT_vertex_weighting
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_weighting.txt
 */
//@{
void      (APIENTRY *glVertexWeightfEXT) (GLfloat weight);
void      (APIENTRY *glVertexWeightfvEXT) (const GLfloat *weight);
void      (APIENTRY *glVertexWeightPointerEXT) (GLsizei size, GLenum type, GLsizei stride, const GLvoid *pointer);
//@}

/** @name GL_NV_vertex_array_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_array_range.txt
 */
//@{
void      (APIENTRY *glFlushVertexArrayRangeNV) (void);
void      (APIENTRY *glVertexArrayRangeNV) (GLsizei length, const GLvoid *pointer);
//@}

/** @name GL_NV_register_combiners
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/register_combiners.txt
 */
//@{
void      (APIENTRY *glCombinerParameterfvNV) (GLenum pname, const GLfloat *params);
void      (APIENTRY *glCombinerParameterfNV) (GLenum pname, GLfloat param);
void      (APIENTRY *glCombinerParameterivNV) (GLenum pname, const GLint *params);
void      (APIENTRY *glCombinerParameteriNV) (GLenum pname, GLint param);
void      (APIENTRY *glCombinerInputNV) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
void      (APIENTRY *glCombinerOutputNV) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
void      (APIENTRY *glFinalCombinerInputNV) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
void      (APIENTRY *glGetCombinerInputParameterfvNV) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetCombinerInputParameterivNV) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
void      (APIENTRY *glGetCombinerOutputParameterfvNV) (GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetCombinerOutputParameterivNV) (GLenum stage, GLenum portion, GLenum pname, GLint *params);
void      (APIENTRY *glGetFinalCombinerInputParameterfvNV) (GLenum variable, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetFinalCombinerInputParameterivNV) (GLenum variable, GLenum pname, GLint *params);
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
void      (APIENTRY *glWindowPos2dMESA) (GLdouble x, GLdouble y);
void      (APIENTRY *glWindowPos2dvMESA) (const GLdouble *v);
void      (APIENTRY *glWindowPos2fMESA) (GLfloat x, GLfloat y);
void      (APIENTRY *glWindowPos2fvMESA) (const GLfloat *v);
void      (APIENTRY *glWindowPos2iMESA) (GLint x, GLint y);
void      (APIENTRY *glWindowPos2ivMESA) (const GLint *v);
void      (APIENTRY *glWindowPos2sMESA) (GLshort x, GLshort y);
void      (APIENTRY *glWindowPos2svMESA) (const GLshort *v);
void      (APIENTRY *glWindowPos3dMESA) (GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glWindowPos3dvMESA) (const GLdouble *v);
void      (APIENTRY *glWindowPos3fMESA) (GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glWindowPos3fvMESA) (const GLfloat *v);
void      (APIENTRY *glWindowPos3iMESA) (GLint x, GLint y, GLint z);
void      (APIENTRY *glWindowPos3ivMESA) (const GLint *v);
void      (APIENTRY *glWindowPos3sMESA) (GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glWindowPos3svMESA) (const GLshort *v);
void      (APIENTRY *glWindowPos4dMESA) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glWindowPos4dvMESA) (const GLdouble *v);
void      (APIENTRY *glWindowPos4fMESA) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glWindowPos4fvMESA) (const GLfloat *v);
void      (APIENTRY *glWindowPos4iMESA) (GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glWindowPos4ivMESA) (const GLint *v);
void      (APIENTRY *glWindowPos4sMESA) (GLshort x, GLshort y, GLshort z, GLshort w);
void      (APIENTRY *glWindowPos4svMESA) (const GLshort *v);
//@}

/** @name GL_IBM_multimode_draw_arrays
 *  See http://oss.sgi.com/projects/ogl-sample/registry/IBM/multimode_draw_arrays.txt
 */
//@{
void      (APIENTRY *glMultiModeDrawArraysIBM) (const GLenum *mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
void      (APIENTRY *glMultiModeDrawElementsIBM) (const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei primcount, GLint modestride);
//@}

/** @name GL_IBM_vertex_array_lists
 *  See http://oss.sgi.com/projects/ogl-sample/registry/IBM/vertex_array_lists.txt
 */
//@{
void      (APIENTRY *glColorPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void      (APIENTRY *glSecondaryColorPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void      (APIENTRY *glEdgeFlagPointerListIBM) (GLint stride, const GLboolean* *pointer, GLint ptrstride);
void      (APIENTRY *glFogCoordPointerListIBM) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void      (APIENTRY *glIndexPointerListIBM) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void      (APIENTRY *glNormalPointerListIBM) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void      (APIENTRY *glTexCoordPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void      (APIENTRY *glVertexPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
//@}

/** @name GL_3DFX_tbuffer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/3DFX/tbuffer.txt
 */
//@{
void      (APIENTRY *glTbufferMask3DFX) (GLuint mask);
//@}

/** @name GL_EXT_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/multisample.txt
 */
//@{
void      (APIENTRY *glSampleMaskEXT) (GLclampf value, GLboolean invert);
void      (APIENTRY *glSamplePatternEXT) (GLenum pattern);
//@}

/** @name GL_SGIS_texture_color_mask
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture_color_mask.txt
 */
//@{
void      (APIENTRY *glTextureColorMaskSGIS) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
//@}

/** @name GL_SGIX_igloo_interface
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SGIX/igloo_interface.txt
 */
//@{
void      (APIENTRY *glIglooInterfaceSGIX) (GLenum pname, const GLvoid *params);
//@}

/** @name GL_NV_fence
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/fence.txt
 */
//@{
void      (APIENTRY *glDeleteFencesNV) (GLsizei n, const GLuint *fences);
void      (APIENTRY *glGenFencesNV) (GLsizei n, GLuint *fences);
GLboolean (APIENTRY *glIsFenceNV) (GLuint fence);
GLboolean (APIENTRY *glTestFenceNV) (GLuint fence);
void      (APIENTRY *glGetFenceivNV) (GLuint fence, GLenum pname, GLint *params);
void      (APIENTRY *glFinishFenceNV) (GLuint fence);
void      (APIENTRY *glSetFenceNV) (GLuint fence, GLenum condition);
//@}

/** @name GL_NV_evaluators
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/evaluators.txt
 */
//@{
void      (APIENTRY *glMapControlPointsNV) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
void      (APIENTRY *glMapParameterivNV) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glMapParameterfvNV) (GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glGetMapControlPointsNV) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
void      (APIENTRY *glGetMapParameterivNV) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetMapParameterfvNV) (GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMapAttribParameterivNV) (GLenum target, GLuint index, GLenum pname, GLint *params);
void      (APIENTRY *glGetMapAttribParameterfvNV) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
void      (APIENTRY *glEvalMapsNV) (GLenum target, GLenum mode);
//@}

/** @name GL_NV_register_combiners2
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/register_combiners2.txt
 */
//@{
void      (APIENTRY *glCombinerStageParameterfvNV) (GLenum stage, GLenum pname, const GLfloat *params);
void      (APIENTRY *glGetCombinerStageParameterfvNV) (GLenum stage, GLenum pname, GLfloat *params);
//@}

/** @name GL_NV_vertex_program
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_program.txt
 */
//@{
GLboolean (APIENTRY *glAreProgramsResidentNV) (GLsizei n, const GLuint *programs, GLboolean *residences);
void      (APIENTRY *glBindProgramNV) (GLenum target, GLuint id);
void      (APIENTRY *glDeleteProgramsNV) (GLsizei n, const GLuint *programs);
void      (APIENTRY *glExecuteProgramNV) (GLenum target, GLuint id, const GLfloat *params);
void      (APIENTRY *glGenProgramsNV) (GLsizei n, GLuint *programs);
void      (APIENTRY *glGetProgramParameterdvNV) (GLenum target, GLuint index, GLenum pname, GLdouble *params);
void      (APIENTRY *glGetProgramParameterfvNV) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetProgramivNV) (GLuint id, GLenum pname, GLint *params);
void      (APIENTRY *glGetProgramStringNV) (GLuint id, GLenum pname, GLubyte *program);
void      (APIENTRY *glGetTrackMatrixivNV) (GLenum target, GLuint address, GLenum pname, GLint *params);
void      (APIENTRY *glGetVertexAttribdvNV) (GLuint index, GLenum pname, GLdouble *params);
void      (APIENTRY *glGetVertexAttribfvNV) (GLuint index, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetVertexAttribivNV) (GLuint index, GLenum pname, GLint *params);
void      (APIENTRY *glGetVertexAttribPointervNV) (GLuint index, GLenum pname, GLvoid* *pointer);
GLboolean (APIENTRY *glIsProgramNV) (GLuint id);
void      (APIENTRY *glLoadProgramNV) (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
void      (APIENTRY *glProgramParameter4dNV) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glProgramParameter4dvNV) (GLenum target, GLuint index, const GLdouble *v);
void      (APIENTRY *glProgramParameter4fNV) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glProgramParameter4fvNV) (GLenum target, GLuint index, const GLfloat *v);
void      (APIENTRY *glProgramParameters4dvNV) (GLenum target, GLuint index, GLsizei count, const GLdouble *v);
void      (APIENTRY *glProgramParameters4fvNV) (GLenum target, GLuint index, GLsizei count, const GLfloat *v);
void      (APIENTRY *glRequestResidentProgramsNV) (GLsizei n, const GLuint *programs);
void      (APIENTRY *glTrackMatrixNV) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
void      (APIENTRY *glVertexAttribPointerNV) (GLuint index, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glVertexAttrib1dNV) (GLuint index, GLdouble x);
void      (APIENTRY *glVertexAttrib1dvNV) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib1fNV) (GLuint index, GLfloat x);
void      (APIENTRY *glVertexAttrib1fvNV) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib1sNV) (GLuint index, GLshort x);
void      (APIENTRY *glVertexAttrib1svNV) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib2dNV) (GLuint index, GLdouble x, GLdouble y);
void      (APIENTRY *glVertexAttrib2dvNV) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib2fNV) (GLuint index, GLfloat x, GLfloat y);
void      (APIENTRY *glVertexAttrib2fvNV) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib2sNV) (GLuint index, GLshort x, GLshort y);
void      (APIENTRY *glVertexAttrib2svNV) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib3dNV) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glVertexAttrib3dvNV) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib3fNV) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glVertexAttrib3fvNV) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib3sNV) (GLuint index, GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glVertexAttrib3svNV) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4dNV) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glVertexAttrib4dvNV) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttrib4fNV) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glVertexAttrib4fvNV) (GLuint index, const GLfloat *v);
void      (APIENTRY *glVertexAttrib4sNV) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void      (APIENTRY *glVertexAttrib4svNV) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttrib4ubNV) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void      (APIENTRY *glVertexAttrib4ubvNV) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttribs1dvNV) (GLuint index, GLsizei count, const GLdouble *v);
void      (APIENTRY *glVertexAttribs1fvNV) (GLuint index, GLsizei count, const GLfloat *v);
void      (APIENTRY *glVertexAttribs1svNV) (GLuint index, GLsizei count, const GLshort *v);
void      (APIENTRY *glVertexAttribs2dvNV) (GLuint index, GLsizei count, const GLdouble *v);
void      (APIENTRY *glVertexAttribs2fvNV) (GLuint index, GLsizei count, const GLfloat *v);
void      (APIENTRY *glVertexAttribs2svNV) (GLuint index, GLsizei count, const GLshort *v);
void      (APIENTRY *glVertexAttribs3dvNV) (GLuint index, GLsizei count, const GLdouble *v);
void      (APIENTRY *glVertexAttribs3fvNV) (GLuint index, GLsizei count, const GLfloat *v);
void      (APIENTRY *glVertexAttribs3svNV) (GLuint index, GLsizei count, const GLshort *v);
void      (APIENTRY *glVertexAttribs4dvNV) (GLuint index, GLsizei count, const GLdouble *v);
void      (APIENTRY *glVertexAttribs4fvNV) (GLuint index, GLsizei count, const GLfloat *v);
void      (APIENTRY *glVertexAttribs4svNV) (GLuint index, GLsizei count, const GLshort *v);
void      (APIENTRY *glVertexAttribs4ubvNV) (GLuint index, GLsizei count, const GLubyte *v);
//@}

/** @name GL_ATI_envmap_bumpmap
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/envmap_bumpmap.txt
 */
//@{
void      (APIENTRY *glTexBumpParameterivATI) (GLenum pname, const GLint *param);
void      (APIENTRY *glTexBumpParameterfvATI) (GLenum pname, const GLfloat *param);
void      (APIENTRY *glGetTexBumpParameterivATI) (GLenum pname, GLint *param);
void      (APIENTRY *glGetTexBumpParameterfvATI) (GLenum pname, GLfloat *param);
//@}

/** @name GL_ATI_fragment_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/fragment_shader.txt
 */
//@{
GLuint    (APIENTRY *glGenFragmentShadersATI) (GLuint range);
void      (APIENTRY *glBindFragmentShaderATI) (GLuint id);
void      (APIENTRY *glDeleteFragmentShaderATI) (GLuint id);
void      (APIENTRY *glBeginFragmentShaderATI) (void);
void      (APIENTRY *glEndFragmentShaderATI) (void);
void      (APIENTRY *glPassTexCoordATI) (GLuint dst, GLuint coord, GLenum swizzle);
void      (APIENTRY *glSampleMapATI) (GLuint dst, GLuint interp, GLenum swizzle);
void      (APIENTRY *glColorFragmentOp1ATI) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
void      (APIENTRY *glColorFragmentOp2ATI) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
void      (APIENTRY *glColorFragmentOp3ATI) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
void      (APIENTRY *glAlphaFragmentOp1ATI) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
void      (APIENTRY *glAlphaFragmentOp2ATI) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
void      (APIENTRY *glAlphaFragmentOp3ATI) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
void      (APIENTRY *glSetFragmentShaderConstantATI) (GLuint dst, const GLfloat *value);
//@}

/** @name GL_ATI_pn_triangles
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/pn_triangles.txt
 */
//@{
void      (APIENTRY *glPNTrianglesiATI) (GLenum pname, GLint param);
void      (APIENTRY *glPNTrianglesfATI) (GLenum pname, GLfloat param);
//@}

/** @name GL_ATI_vertex_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/vertex_array_object.txt
 */
//@{
GLuint    (APIENTRY *glNewObjectBufferATI) (GLsizei size, const GLvoid *pointer, GLenum usage);
GLboolean (APIENTRY *glIsObjectBufferATI) (GLuint buffer);
void      (APIENTRY *glUpdateObjectBufferATI) (GLuint buffer, GLuint offset, GLsizei size, const GLvoid *pointer, GLenum preserve);
void      (APIENTRY *glGetObjectBufferfvATI) (GLuint buffer, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetObjectBufferivATI) (GLuint buffer, GLenum pname, GLint *params);
void      (APIENTRY *glFreeObjectBufferATI) (GLuint buffer);
void      (APIENTRY *glArrayObjectATI) (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
void      (APIENTRY *glGetArrayObjectfvATI) (GLenum array, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetArrayObjectivATI) (GLenum array, GLenum pname, GLint *params);
void      (APIENTRY *glVariantArrayObjectATI) (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
void      (APIENTRY *glGetVariantArrayObjectfvATI) (GLuint id, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetVariantArrayObjectivATI) (GLuint id, GLenum pname, GLint *params);
//@}

/** @name GL_EXT_vertex_shader
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_shader.txt
 */
//@{
void      (APIENTRY *glBeginVertexShaderEXT) (void);
void      (APIENTRY *glEndVertexShaderEXT) (void);
void      (APIENTRY *glBindVertexShaderEXT) (GLuint id);
GLuint    (APIENTRY *glGenVertexShadersEXT) (GLuint range);
void      (APIENTRY *glDeleteVertexShaderEXT) (GLuint id);
void      (APIENTRY *glShaderOp1EXT) (GLenum op, GLuint res, GLuint arg1);
void      (APIENTRY *glShaderOp2EXT) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
void      (APIENTRY *glShaderOp3EXT) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
void      (APIENTRY *glSwizzleEXT) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
void      (APIENTRY *glWriteMaskEXT) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
void      (APIENTRY *glInsertComponentEXT) (GLuint res, GLuint src, GLuint num);
void      (APIENTRY *glExtractComponentEXT) (GLuint res, GLuint src, GLuint num);
GLuint    (APIENTRY *glGenSymbolsEXT) (GLenum datatype, GLenum storagetype, GLenum range, GLuint components);
void      (APIENTRY *glSetInvariantEXT) (GLuint id, GLenum type, const GLvoid *addr);
void      (APIENTRY *glSetLocalConstantEXT) (GLuint id, GLenum type, const GLvoid *addr);
void      (APIENTRY *glVariantbvEXT) (GLuint id, const GLbyte *addr);
void      (APIENTRY *glVariantsvEXT) (GLuint id, const GLshort *addr);
void      (APIENTRY *glVariantivEXT) (GLuint id, const GLint *addr);
void      (APIENTRY *glVariantfvEXT) (GLuint id, const GLfloat *addr);
void      (APIENTRY *glVariantdvEXT) (GLuint id, const GLdouble *addr);
void      (APIENTRY *glVariantubvEXT) (GLuint id, const GLubyte *addr);
void      (APIENTRY *glVariantusvEXT) (GLuint id, const GLushort *addr);
void      (APIENTRY *glVariantuivEXT) (GLuint id, const GLuint *addr);
void      (APIENTRY *glVariantPointerEXT) (GLuint id, GLenum type, GLuint stride, const GLvoid *addr);
void      (APIENTRY *glEnableVariantClientStateEXT) (GLuint id);
void      (APIENTRY *glDisableVariantClientStateEXT) (GLuint id);
GLuint    (APIENTRY *glBindLightParameterEXT) (GLenum light, GLenum value);
GLuint    (APIENTRY *glBindMaterialParameterEXT) (GLenum face, GLenum value);
GLuint    (APIENTRY *glBindTexGenParameterEXT) (GLenum unit, GLenum coord, GLenum value);
GLuint    (APIENTRY *glBindTextureUnitParameterEXT) (GLenum unit, GLenum value);
GLuint    (APIENTRY *glBindParameterEXT) (GLenum value);
GLboolean (APIENTRY *glIsVariantEnabledEXT) (GLuint id, GLenum cap);
void      (APIENTRY *glGetVariantBooleanvEXT) (GLuint id, GLenum value, GLboolean *data);
void      (APIENTRY *glGetVariantIntegervEXT) (GLuint id, GLenum value, GLint *data);
void      (APIENTRY *glGetVariantFloatvEXT) (GLuint id, GLenum value, GLfloat *data);
void      (APIENTRY *glGetVariantPointervEXT) (GLuint id, GLenum value, GLvoid* *data);
void      (APIENTRY *glGetInvariantBooleanvEXT) (GLuint id, GLenum value, GLboolean *data);
void      (APIENTRY *glGetInvariantIntegervEXT) (GLuint id, GLenum value, GLint *data);
void      (APIENTRY *glGetInvariantFloatvEXT) (GLuint id, GLenum value, GLfloat *data);
void      (APIENTRY *glGetLocalConstantBooleanvEXT) (GLuint id, GLenum value, GLboolean *data);
void      (APIENTRY *glGetLocalConstantIntegervEXT) (GLuint id, GLenum value, GLint *data);
void      (APIENTRY *glGetLocalConstantFloatvEXT) (GLuint id, GLenum value, GLfloat *data);
//@}

/** @name GL_ATI_vertex_streams
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/vertex_streams.txt
 */
//@{
void      (APIENTRY *glVertexStream1sATI) (GLenum stream, GLshort x);
void      (APIENTRY *glVertexStream1svATI) (GLenum stream, const GLshort *coords);
void      (APIENTRY *glVertexStream1iATI) (GLenum stream, GLint x);
void      (APIENTRY *glVertexStream1ivATI) (GLenum stream, const GLint *coords);
void      (APIENTRY *glVertexStream1fATI) (GLenum stream, GLfloat x);
void      (APIENTRY *glVertexStream1fvATI) (GLenum stream, const GLfloat *coords);
void      (APIENTRY *glVertexStream1dATI) (GLenum stream, GLdouble x);
void      (APIENTRY *glVertexStream1dvATI) (GLenum stream, const GLdouble *coords);
void      (APIENTRY *glVertexStream2sATI) (GLenum stream, GLshort x, GLshort y);
void      (APIENTRY *glVertexStream2svATI) (GLenum stream, const GLshort *coords);
void      (APIENTRY *glVertexStream2iATI) (GLenum stream, GLint x, GLint y);
void      (APIENTRY *glVertexStream2ivATI) (GLenum stream, const GLint *coords);
void      (APIENTRY *glVertexStream2fATI) (GLenum stream, GLfloat x, GLfloat y);
void      (APIENTRY *glVertexStream2fvATI) (GLenum stream, const GLfloat *coords);
void      (APIENTRY *glVertexStream2dATI) (GLenum stream, GLdouble x, GLdouble y);
void      (APIENTRY *glVertexStream2dvATI) (GLenum stream, const GLdouble *coords);
void      (APIENTRY *glVertexStream3sATI) (GLenum stream, GLshort x, GLshort y, GLshort z);
void      (APIENTRY *glVertexStream3svATI) (GLenum stream, const GLshort *coords);
void      (APIENTRY *glVertexStream3iATI) (GLenum stream, GLint x, GLint y, GLint z);
void      (APIENTRY *glVertexStream3ivATI) (GLenum stream, const GLint *coords);
void      (APIENTRY *glVertexStream3fATI) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glVertexStream3fvATI) (GLenum stream, const GLfloat *coords);
void      (APIENTRY *glVertexStream3dATI) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glVertexStream3dvATI) (GLenum stream, const GLdouble *coords);
void      (APIENTRY *glVertexStream4sATI) (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
void      (APIENTRY *glVertexStream4svATI) (GLenum stream, const GLshort *coords);
void      (APIENTRY *glVertexStream4iATI) (GLenum stream, GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glVertexStream4ivATI) (GLenum stream, const GLint *coords);
void      (APIENTRY *glVertexStream4fATI) (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glVertexStream4fvATI) (GLenum stream, const GLfloat *coords);
void      (APIENTRY *glVertexStream4dATI) (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glVertexStream4dvATI) (GLenum stream, const GLdouble *coords);
void      (APIENTRY *glNormalStream3bATI) (GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz);
void      (APIENTRY *glNormalStream3bvATI) (GLenum stream, const GLbyte *coords);
void      (APIENTRY *glNormalStream3sATI) (GLenum stream, GLshort nx, GLshort ny, GLshort nz);
void      (APIENTRY *glNormalStream3svATI) (GLenum stream, const GLshort *coords);
void      (APIENTRY *glNormalStream3iATI) (GLenum stream, GLint nx, GLint ny, GLint nz);
void      (APIENTRY *glNormalStream3ivATI) (GLenum stream, const GLint *coords);
void      (APIENTRY *glNormalStream3fATI) (GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz);
void      (APIENTRY *glNormalStream3fvATI) (GLenum stream, const GLfloat *coords);
void      (APIENTRY *glNormalStream3dATI) (GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz);
void      (APIENTRY *glNormalStream3dvATI) (GLenum stream, const GLdouble *coords);
void      (APIENTRY *glClientActiveVertexStreamATI) (GLenum stream);
void      (APIENTRY *glVertexBlendEnviATI) (GLenum pname, GLint param);
void      (APIENTRY *glVertexBlendEnvfATI) (GLenum pname, GLfloat param);
//@}

/** @name GL_ATI_element_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/element_array.txt
 */
//@{
void      (APIENTRY *glElementPointerATI) (GLenum type, const GLvoid *pointer);
void      (APIENTRY *glDrawElementArrayATI) (GLenum mode, GLsizei count);
void      (APIENTRY *glDrawRangeElementArrayATI) (GLenum mode, GLuint start, GLuint end, GLsizei count);
//@}

/** @name GL_SUN_mesh_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/SUN/mesh_array.txt
 */
//@{
void      (APIENTRY *glDrawMeshArraysSUN) (GLenum mode, GLint first, GLsizei count, GLsizei width);
//@}

/** @name GL_NV_occlusion_query
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/occlusion_query.txt
 */
//@{
void      (APIENTRY *glGenOcclusionQueriesNV) (GLsizei n, GLuint *ids);
void      (APIENTRY *glDeleteOcclusionQueriesNV) (GLsizei n, const GLuint *ids);
GLboolean (APIENTRY *glIsOcclusionQueryNV) (GLuint id);
void      (APIENTRY *glBeginOcclusionQueryNV) (GLuint id);
void      (APIENTRY *glEndOcclusionQueryNV) (void);
void      (APIENTRY *glGetOcclusionQueryivNV) (GLuint id, GLenum pname, GLint *params);
void      (APIENTRY *glGetOcclusionQueryuivNV) (GLuint id, GLenum pname, GLuint *params);
//@}

/** @name GL_NV_point_sprite
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/point_sprite.txt
 */
//@{
void      (APIENTRY *glPointParameteriNV) (GLenum pname, GLint param);
void      (APIENTRY *glPointParameterivNV) (GLenum pname, const GLint *params);
//@}

/** @name GL_EXT_stencil_two_side
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/stencil_two_side.txt
 */
//@{
void      (APIENTRY *glActiveStencilFaceEXT) (GLenum face);
//@}

/** @name GL_APPLE_element_array
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/element_array.txt
 */
//@{
void      (APIENTRY *glElementPointerAPPLE) (GLenum type, const GLvoid *pointer);
void      (APIENTRY *glDrawElementArrayAPPLE) (GLenum mode, GLint first, GLsizei count);
void      (APIENTRY *glDrawRangeElementArrayAPPLE) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
void      (APIENTRY *glMultiDrawElementArrayAPPLE) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
void      (APIENTRY *glMultiDrawRangeElementArrayAPPLE) (GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);
//@}

/** @name GL_APPLE_fence
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/fence.txt
 */
//@{
void      (APIENTRY *glGenFencesAPPLE) (GLsizei n, GLuint *fences);
void      (APIENTRY *glDeleteFencesAPPLE) (GLsizei n, const GLuint *fences);
void      (APIENTRY *glSetFenceAPPLE) (GLuint fence);
GLboolean (APIENTRY *glIsFenceAPPLE) (GLuint fence);
GLboolean (APIENTRY *glTestFenceAPPLE) (GLuint fence);
void      (APIENTRY *glFinishFenceAPPLE) (GLuint fence);
GLboolean (APIENTRY *glTestObjectAPPLE) (GLenum object, GLuint name);
void      (APIENTRY *glFinishObjectAPPLE) (GLenum object, GLint name);
//@}

/** @name GL_APPLE_vertex_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/vertex_array_object.txt
 */
//@{
void      (APIENTRY *glBindVertexArrayAPPLE) (GLuint array);
void      (APIENTRY *glDeleteVertexArraysAPPLE) (GLsizei n, const GLuint *arrays);
void      (APIENTRY *glGenVertexArraysAPPLE) (GLsizei n, GLuint *arrays);
GLboolean (APIENTRY *glIsVertexArrayAPPLE) (GLuint array);
//@}

/** @name GL_APPLE_vertex_array_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/vertex_array_range.txt
 */
//@{
void      (APIENTRY *glVertexArrayRangeAPPLE) (GLsizei length, GLvoid *pointer);
void      (APIENTRY *glFlushVertexArrayRangeAPPLE) (GLsizei length, GLvoid *pointer);
void      (APIENTRY *glVertexArrayParameteriAPPLE) (GLenum pname, GLint param);
//@}

/** @name GL_ATI_draw_buffers
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/draw_buffers.txt
 */
//@{
void      (APIENTRY *glDrawBuffersATI) (GLsizei n, const GLenum *bufs);
//@}

/** @name GL_NV_fragment_program
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/fragment_program.txt
 */
//@{
void      (APIENTRY *glProgramNamedParameter4fNV) (GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glProgramNamedParameter4dNV) (GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glProgramNamedParameter4fvNV) (GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v);
void      (APIENTRY *glProgramNamedParameter4dvNV) (GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v);
void      (APIENTRY *glGetProgramNamedParameterfvNV) (GLuint id, GLsizei len, const GLubyte *name, GLfloat *params);
void      (APIENTRY *glGetProgramNamedParameterdvNV) (GLuint id, GLsizei len, const GLubyte *name, GLdouble *params);
//@}

/** @name GL_NV_half_float
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/half_float.txt
 */
//@{
void      (APIENTRY *glVertex2hNV) (GLhalfNV x, GLhalfNV y);
void      (APIENTRY *glVertex2hvNV) (const GLhalfNV *v);
void      (APIENTRY *glVertex3hNV) (GLhalfNV x, GLhalfNV y, GLhalfNV z);
void      (APIENTRY *glVertex3hvNV) (const GLhalfNV *v);
void      (APIENTRY *glVertex4hNV) (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
void      (APIENTRY *glVertex4hvNV) (const GLhalfNV *v);
void      (APIENTRY *glNormal3hNV) (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
void      (APIENTRY *glNormal3hvNV) (const GLhalfNV *v);
void      (APIENTRY *glColor3hNV) (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
void      (APIENTRY *glColor3hvNV) (const GLhalfNV *v);
void      (APIENTRY *glColor4hNV) (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
void      (APIENTRY *glColor4hvNV) (const GLhalfNV *v);
void      (APIENTRY *glTexCoord1hNV) (GLhalfNV s);
void      (APIENTRY *glTexCoord1hvNV) (const GLhalfNV *v);
void      (APIENTRY *glTexCoord2hNV) (GLhalfNV s, GLhalfNV t);
void      (APIENTRY *glTexCoord2hvNV) (const GLhalfNV *v);
void      (APIENTRY *glTexCoord3hNV) (GLhalfNV s, GLhalfNV t, GLhalfNV r);
void      (APIENTRY *glTexCoord3hvNV) (const GLhalfNV *v);
void      (APIENTRY *glTexCoord4hNV) (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
void      (APIENTRY *glTexCoord4hvNV) (const GLhalfNV *v);
void      (APIENTRY *glMultiTexCoord1hNV) (GLenum target, GLhalfNV s);
void      (APIENTRY *glMultiTexCoord1hvNV) (GLenum target, const GLhalfNV *v);
void      (APIENTRY *glMultiTexCoord2hNV) (GLenum target, GLhalfNV s, GLhalfNV t);
void      (APIENTRY *glMultiTexCoord2hvNV) (GLenum target, const GLhalfNV *v);
void      (APIENTRY *glMultiTexCoord3hNV) (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
void      (APIENTRY *glMultiTexCoord3hvNV) (GLenum target, const GLhalfNV *v);
void      (APIENTRY *glMultiTexCoord4hNV) (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
void      (APIENTRY *glMultiTexCoord4hvNV) (GLenum target, const GLhalfNV *v);
void      (APIENTRY *glFogCoordhNV) (GLhalfNV fog);
void      (APIENTRY *glFogCoordhvNV) (const GLhalfNV *fog);
void      (APIENTRY *glSecondaryColor3hNV) (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
void      (APIENTRY *glSecondaryColor3hvNV) (const GLhalfNV *v);
void      (APIENTRY *glVertexWeighthNV) (GLhalfNV weight);
void      (APIENTRY *glVertexWeighthvNV) (const GLhalfNV *weight);
void      (APIENTRY *glVertexAttrib1hNV) (GLuint index, GLhalfNV x);
void      (APIENTRY *glVertexAttrib1hvNV) (GLuint index, const GLhalfNV *v);
void      (APIENTRY *glVertexAttrib2hNV) (GLuint index, GLhalfNV x, GLhalfNV y);
void      (APIENTRY *glVertexAttrib2hvNV) (GLuint index, const GLhalfNV *v);
void      (APIENTRY *glVertexAttrib3hNV) (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
void      (APIENTRY *glVertexAttrib3hvNV) (GLuint index, const GLhalfNV *v);
void      (APIENTRY *glVertexAttrib4hNV) (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
void      (APIENTRY *glVertexAttrib4hvNV) (GLuint index, const GLhalfNV *v);
void      (APIENTRY *glVertexAttribs1hvNV) (GLuint index, GLsizei n, const GLhalfNV *v);
void      (APIENTRY *glVertexAttribs2hvNV) (GLuint index, GLsizei n, const GLhalfNV *v);
void      (APIENTRY *glVertexAttribs3hvNV) (GLuint index, GLsizei n, const GLhalfNV *v);
void      (APIENTRY *glVertexAttribs4hvNV) (GLuint index, GLsizei n, const GLhalfNV *v);
//@}

/** @name GL_NV_pixel_data_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/pixel_data_range.txt
 */
//@{
void      (APIENTRY *glPixelDataRangeNV) (GLenum target, GLsizei length, GLvoid *pointer);
void      (APIENTRY *glFlushPixelDataRangeNV) (GLenum target);
//@}

/** @name GL_NV_primitive_restart
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/primitive_restart.txt
 */
//@{
void      (APIENTRY *glPrimitiveRestartNV) (void);
void      (APIENTRY *glPrimitiveRestartIndexNV) (GLuint index);
//@}

/** @name GL_ATI_map_object_buffer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/map_object_buffer.txt
 */
//@{
GLvoid*   (APIENTRY *glMapObjectBufferATI) (GLuint buffer);
void      (APIENTRY *glUnmapObjectBufferATI) (GLuint buffer);
//@}

/** @name GL_ATI_separate_stencil
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/separate_stencil.txt
 */
//@{
void      (APIENTRY *glStencilOpSeparateATI) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void      (APIENTRY *glStencilFuncSeparateATI) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
//@}

/** @name GL_ATI_vertex_attrib_array_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/ATI/vertex_attrib_array_object.txt
 */
//@{
void      (APIENTRY *glVertexAttribArrayObjectATI) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);
void      (APIENTRY *glGetVertexAttribArrayObjectfvATI) (GLuint index, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetVertexAttribArrayObjectivATI) (GLuint index, GLenum pname, GLint *params);
//@}

/** @name GL_EXT_depth_bounds_test
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/depth_bounds_test.txt
 */
//@{
void      (APIENTRY *glDepthBoundsEXT) (GLclampd zmin, GLclampd zmax);
//@}

/** @name GL_EXT_blend_equation_separate
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/blend_equation_separate.txt
 */
//@{
void      (APIENTRY *glBlendEquationSeparateEXT) (GLenum modeRGB, GLenum modeAlpha);
//@}

/** @name GL_EXT_framebuffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
 */
//@{
GLboolean (APIENTRY *glIsRenderbufferEXT) (GLuint renderbuffer);
void      (APIENTRY *glBindRenderbufferEXT) (GLenum target, GLuint renderbuffer);
void      (APIENTRY *glDeleteRenderbuffersEXT) (GLsizei n, const GLuint *renderbuffers);
void      (APIENTRY *glGenRenderbuffersEXT) (GLsizei n, GLuint *renderbuffers);
void      (APIENTRY *glRenderbufferStorageEXT) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void      (APIENTRY *glGetRenderbufferParameterivEXT) (GLenum target, GLenum pname, GLint *params);
GLboolean (APIENTRY *glIsFramebufferEXT) (GLuint framebuffer);
void      (APIENTRY *glBindFramebufferEXT) (GLenum target, GLuint framebuffer);
void      (APIENTRY *glDeleteFramebuffersEXT) (GLsizei n, const GLuint *framebuffers);
void      (APIENTRY *glGenFramebuffersEXT) (GLsizei n, GLuint *framebuffers);
GLenum    (APIENTRY *glCheckFramebufferStatusEXT) (GLenum target);
void      (APIENTRY *glFramebufferTexture1DEXT) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void      (APIENTRY *glFramebufferTexture2DEXT) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void      (APIENTRY *glFramebufferTexture3DEXT) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void      (APIENTRY *glFramebufferRenderbufferEXT) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void      (APIENTRY *glGetFramebufferAttachmentParameterivEXT) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
void      (APIENTRY *glGenerateMipmapEXT) (GLenum target);
//@}

/** @name GL_GREMEDY_string_marker
 *  See http://oss.sgi.com/projects/ogl-sample/registry/GREMEDY/string_marker.txt
 */
//@{
void      (APIENTRY *glStringMarkerGREMEDY) (GLsizei len, const GLvoid *string);
//@}

/** @name GL_EXT_stencil_clear_tag
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/stencil_clear_tag.txt
 */
//@{
void      (APIENTRY *glStencilClearTagEXT) (GLsizei stencilTagBits, GLuint stencilClearTag);
//@}

/** @name GL_EXT_framebuffer_blit
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_blit.txt
 */
//@{
void      (APIENTRY *glBlitFramebufferEXT) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
//@}

/** @name GL_EXT_framebuffer_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_multisample.txt
 */
//@{
void      (APIENTRY *glRenderbufferStorageMultisampleEXT) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
//@}

/** @name GL_EXT_timer_query
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/timer_query.txt
 */
//@{
void      (APIENTRY *glGetQueryObjecti64vEXT) (GLuint id, GLenum pname, GLint64EXT *params);
void      (APIENTRY *glGetQueryObjectui64vEXT) (GLuint id, GLenum pname, GLuint64EXT *params);
//@}

/** @name GL_EXT_gpu_program_parameters
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/gpu_program_parameters.txt
 */
//@{
void      (APIENTRY *glProgramEnvParameters4fvEXT) (GLenum target, GLuint index, GLsizei count, const GLfloat *params);
void      (APIENTRY *glProgramLocalParameters4fvEXT) (GLenum target, GLuint index, GLsizei count, const GLfloat *params);
//@}

/** @name GL_APPLE_flush_buffer_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/flush_buffer_range.txt
 */
//@{
void      (APIENTRY *glBufferParameteriAPPLE) (GLenum target, GLenum pname, GLint param);
void      (APIENTRY *glFlushMappedBufferRangeAPPLE) (GLenum target, GLintptr offset, GLsizeiptr size);
//@}

/** @name GL_NV_gpu_program4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/gpu_program4.txt
 */
//@{
void      (APIENTRY *glProgramLocalParameterI4iNV) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glProgramLocalParameterI4ivNV) (GLenum target, GLuint index, const GLint *params);
void      (APIENTRY *glProgramLocalParametersI4ivNV) (GLenum target, GLuint index, GLsizei count, const GLint *params);
void      (APIENTRY *glProgramLocalParameterI4uiNV) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void      (APIENTRY *glProgramLocalParameterI4uivNV) (GLenum target, GLuint index, const GLuint *params);
void      (APIENTRY *glProgramLocalParametersI4uivNV) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
void      (APIENTRY *glProgramEnvParameterI4iNV) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glProgramEnvParameterI4ivNV) (GLenum target, GLuint index, const GLint *params);
void      (APIENTRY *glProgramEnvParametersI4ivNV) (GLenum target, GLuint index, GLsizei count, const GLint *params);
void      (APIENTRY *glProgramEnvParameterI4uiNV) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void      (APIENTRY *glProgramEnvParameterI4uivNV) (GLenum target, GLuint index, const GLuint *params);
void      (APIENTRY *glProgramEnvParametersI4uivNV) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
void      (APIENTRY *glGetProgramLocalParameterIivNV) (GLenum target, GLuint index, GLint *params);
void      (APIENTRY *glGetProgramLocalParameterIuivNV) (GLenum target, GLuint index, GLuint *params);
void      (APIENTRY *glGetProgramEnvParameterIivNV) (GLenum target, GLuint index, GLint *params);
void      (APIENTRY *glGetProgramEnvParameterIuivNV) (GLenum target, GLuint index, GLuint *params);
//@}

/** @name GL_NV_geometry_program4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/geometry_program4.txt
 */
//@{
void      (APIENTRY *glProgramVertexLimitNV) (GLenum target, GLint limit);
void      (APIENTRY *glFramebufferTextureEXT) (GLenum target, GLenum attachment, GLuint texture, GLint level);
void      (APIENTRY *glFramebufferTextureLayerEXT) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void      (APIENTRY *glFramebufferTextureFaceEXT) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
//@}

/** @name GL_EXT_geometry_shader4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/geometry_shader4.txt
 */
//@{
void      (APIENTRY *glProgramParameteriEXT) (GLuint program, GLenum pname, GLint value);
//@}

/** @name GL_NV_vertex_program4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_program4.txt
 */
//@{
void      (APIENTRY *glVertexAttribI1iEXT) (GLuint index, GLint x);
void      (APIENTRY *glVertexAttribI2iEXT) (GLuint index, GLint x, GLint y);
void      (APIENTRY *glVertexAttribI3iEXT) (GLuint index, GLint x, GLint y, GLint z);
void      (APIENTRY *glVertexAttribI4iEXT) (GLuint index, GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glVertexAttribI1uiEXT) (GLuint index, GLuint x);
void      (APIENTRY *glVertexAttribI2uiEXT) (GLuint index, GLuint x, GLuint y);
void      (APIENTRY *glVertexAttribI3uiEXT) (GLuint index, GLuint x, GLuint y, GLuint z);
void      (APIENTRY *glVertexAttribI4uiEXT) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void      (APIENTRY *glVertexAttribI1ivEXT) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI2ivEXT) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI3ivEXT) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI4ivEXT) (GLuint index, const GLint *v);
void      (APIENTRY *glVertexAttribI1uivEXT) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI2uivEXT) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI3uivEXT) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI4uivEXT) (GLuint index, const GLuint *v);
void      (APIENTRY *glVertexAttribI4bvEXT) (GLuint index, const GLbyte *v);
void      (APIENTRY *glVertexAttribI4svEXT) (GLuint index, const GLshort *v);
void      (APIENTRY *glVertexAttribI4ubvEXT) (GLuint index, const GLubyte *v);
void      (APIENTRY *glVertexAttribI4usvEXT) (GLuint index, const GLushort *v);
void      (APIENTRY *glVertexAttribIPointerEXT) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glGetVertexAttribIivEXT) (GLuint index, GLenum pname, GLint *params);
void      (APIENTRY *glGetVertexAttribIuivEXT) (GLuint index, GLenum pname, GLuint *params);
//@}

/** @name GL_EXT_gpu_shader4
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/gpu_shader4.txt
 */
//@{
void      (APIENTRY *glGetUniformuivEXT) (GLuint program, GLint location, GLuint *params);
void      (APIENTRY *glBindFragDataLocationEXT) (GLuint program, GLuint color, const GLchar *name);
GLint     (APIENTRY *glGetFragDataLocationEXT) (GLuint program, const GLchar *name);
void      (APIENTRY *glUniform1uiEXT) (GLint location, GLuint v0);
void      (APIENTRY *glUniform2uiEXT) (GLint location, GLuint v0, GLuint v1);
void      (APIENTRY *glUniform3uiEXT) (GLint location, GLuint v0, GLuint v1, GLuint v2);
void      (APIENTRY *glUniform4uiEXT) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void      (APIENTRY *glUniform1uivEXT) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glUniform2uivEXT) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glUniform3uivEXT) (GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glUniform4uivEXT) (GLint location, GLsizei count, const GLuint *value);
//@}

/** @name GL_EXT_draw_instanced
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/draw_instanced.txt
 */
//@{
void      (APIENTRY *glDrawArraysInstancedEXT) (GLenum mode, GLint start, GLsizei count, GLsizei primcount);
void      (APIENTRY *glDrawElementsInstancedEXT) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
//@}

/** @name GL_EXT_texture_buffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_buffer_object.txt
 */
//@{
void      (APIENTRY *glTexBufferEXT) (GLenum target, GLenum internalformat, GLuint buffer);
//@}

/** @name GL_NV_depth_buffer_float
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/depth_buffer_float.txt
 */
//@{
void      (APIENTRY *glDepthRangedNV) (GLdouble zNear, GLdouble zFar);
void      (APIENTRY *glClearDepthdNV) (GLdouble depth);
void      (APIENTRY *glDepthBoundsdNV) (GLdouble zmin, GLdouble zmax);
//@}

/** @name GL_NV_framebuffer_multisample_coverage
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/framebuffer_multisample_coverage.txt
 */
//@{
void      (APIENTRY *glRenderbufferStorageMultisampleCoverageNV) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
//@}

/** @name GL_NV_parameter_buffer_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/parameter_buffer_object.txt
 */
//@{
void      (APIENTRY *glProgramBufferParametersfvNV) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);
void      (APIENTRY *glProgramBufferParametersIivNV) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
void      (APIENTRY *glProgramBufferParametersIuivNV) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);
//@}

/** @name GL_EXT_draw_buffers2
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/draw_buffers2.txt
 */
//@{
void      (APIENTRY *glColorMaskIndexedEXT) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void      (APIENTRY *glGetBooleanIndexedvEXT) (GLenum target, GLuint index, GLboolean *data);
void      (APIENTRY *glGetIntegerIndexedvEXT) (GLenum target, GLuint index, GLint *data);
void      (APIENTRY *glEnableIndexedEXT) (GLenum target, GLuint index);
void      (APIENTRY *glDisableIndexedEXT) (GLenum target, GLuint index);
GLboolean (APIENTRY *glIsEnabledIndexedEXT) (GLenum target, GLuint index);
//@}

/** @name GL_NV_transform_feedback
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/transform_feedback.txt
 */
//@{
void      (APIENTRY *glBeginTransformFeedbackNV) (GLenum primitiveMode);
void      (APIENTRY *glEndTransformFeedbackNV) (void);
void      (APIENTRY *glTransformFeedbackAttribsNV) (GLuint count, const GLint *attribs, GLenum bufferMode);
void      (APIENTRY *glBindBufferRangeNV) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void      (APIENTRY *glBindBufferOffsetNV) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
void      (APIENTRY *glBindBufferBaseNV) (GLenum target, GLuint index, GLuint buffer);
void      (APIENTRY *glTransformFeedbackVaryingsNV) (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);
void      (APIENTRY *glActiveVaryingNV) (GLuint program, const GLchar *name);
GLint     (APIENTRY *glGetVaryingLocationNV) (GLuint program, const GLchar *name);
void      (APIENTRY *glGetActiveVaryingNV) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
void      (APIENTRY *glGetTransformFeedbackVaryingNV) (GLuint program, GLuint index, GLint *location);
void      (APIENTRY *glTransformFeedbackStreamAttribsNV) (GLsizei count, const GLint *attribs, GLsizei nbuffers, const GLint *bufstreams, GLenum bufferMode);
//@}

/** @name GL_EXT_bindable_uniform
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/bindable_uniform.txt
 */
//@{
void      (APIENTRY *glUniformBufferEXT) (GLuint program, GLint location, GLuint buffer);
GLint     (APIENTRY *glGetUniformBufferSizeEXT) (GLuint program, GLint location);
GLintptr  (APIENTRY *glGetUniformOffsetEXT) (GLuint program, GLint location);
//@}

/** @name GL_EXT_texture_integer
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_integer.txt
 */
//@{
void      (APIENTRY *glTexParameterIivEXT) (GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glTexParameterIuivEXT) (GLenum target, GLenum pname, const GLuint *params);
void      (APIENTRY *glGetTexParameterIivEXT) (GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetTexParameterIuivEXT) (GLenum target, GLenum pname, GLuint *params);
void      (APIENTRY *glClearColorIiEXT) (GLint red, GLint green, GLint blue, GLint alpha);
void      (APIENTRY *glClearColorIuiEXT) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
//@}

/** @name GL_GREMEDY_frame_terminator
 *  See http://oss.sgi.com/projects/ogl-sample/registry/GREMEDY/frame_terminator.txt
 */
//@{
void      (APIENTRY *glFrameTerminatorGREMEDY) (void);
//@}

/** @name GL_NV_conditional_render
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/conditional_render.txt
 */
//@{
void      (APIENTRY *glBeginConditionalRenderNV) (GLuint id, GLenum mode);
void      (APIENTRY *glEndConditionalRenderNV) (void);
//@}

/** @name GL_NV_present_video
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/present_video.txt
 */
//@{
void      (APIENTRY *glPresentFrameKeyedNV) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);
void      (APIENTRY *glPresentFrameDualFillNV) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
void      (APIENTRY *glGetVideoivNV) (GLuint video_slot, GLenum pname, GLint *params);
void      (APIENTRY *glGetVideouivNV) (GLuint video_slot, GLenum pname, GLuint *params);
void      (APIENTRY *glGetVideoi64vNV) (GLuint video_slot, GLenum pname, GLint64EXT *params);
void      (APIENTRY *glGetVideoui64vNV) (GLuint video_slot, GLenum pname, GLuint64EXT *params);
//@}

/** @name GL_EXT_transform_feedback
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/transform_feedback.txt
 */
//@{
void      (APIENTRY *glBeginTransformFeedbackEXT) (GLenum primitiveMode);
void      (APIENTRY *glEndTransformFeedbackEXT) (void);
void      (APIENTRY *glBindBufferRangeEXT) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void      (APIENTRY *glBindBufferOffsetEXT) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
void      (APIENTRY *glBindBufferBaseEXT) (GLenum target, GLuint index, GLuint buffer);
void      (APIENTRY *glTransformFeedbackVaryingsEXT) (GLuint program, GLsizei count, const GLchar* *varyings, GLenum bufferMode);
void      (APIENTRY *glGetTransformFeedbackVaryingEXT) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
//@}

/** @name GL_EXT_direct_state_access
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/direct_state_access.txt
 */
//@{
void      (APIENTRY *glClientAttribDefaultEXT) (GLbitfield mask);
void      (APIENTRY *glPushClientAttribDefaultEXT) (GLbitfield mask);
void      (APIENTRY *glMatrixLoadfEXT) (GLenum mode, const GLfloat *m);
void      (APIENTRY *glMatrixLoaddEXT) (GLenum mode, const GLdouble *m);
void      (APIENTRY *glMatrixMultfEXT) (GLenum mode, const GLfloat *m);
void      (APIENTRY *glMatrixMultdEXT) (GLenum mode, const GLdouble *m);
void      (APIENTRY *glMatrixLoadIdentityEXT) (GLenum mode);
void      (APIENTRY *glMatrixRotatefEXT) (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glMatrixRotatedEXT) (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glMatrixScalefEXT) (GLenum mode, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glMatrixScaledEXT) (GLenum mode, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glMatrixTranslatefEXT) (GLenum mode, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY *glMatrixTranslatedEXT) (GLenum mode, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glMatrixFrustumEXT) (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void      (APIENTRY *glMatrixOrthoEXT) (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void      (APIENTRY *glMatrixPopEXT) (GLenum mode);
void      (APIENTRY *glMatrixPushEXT) (GLenum mode);
void      (APIENTRY *glMatrixLoadTransposefEXT) (GLenum mode, const GLfloat *m);
void      (APIENTRY *glMatrixLoadTransposedEXT) (GLenum mode, const GLdouble *m);
void      (APIENTRY *glMatrixMultTransposefEXT) (GLenum mode, const GLfloat *m);
void      (APIENTRY *glMatrixMultTransposedEXT) (GLenum mode, const GLdouble *m);
void      (APIENTRY *glTextureParameterfEXT) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY *glTextureParameterfvEXT) (GLuint texture, GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glTextureParameteriEXT) (GLuint texture, GLenum target, GLenum pname, GLint param);
void      (APIENTRY *glTextureParameterivEXT) (GLuint texture, GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glTextureImage1DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTextureImage2DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTextureSubImage1DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTextureSubImage2DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glCopyTextureImage1DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void      (APIENTRY *glCopyTextureImage2DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void      (APIENTRY *glCopyTextureSubImage1DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glCopyTextureSubImage2DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glGetTextureImageEXT) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void      (APIENTRY *glGetTextureParameterfvEXT) (GLuint texture, GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetTextureParameterivEXT) (GLuint texture, GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetTextureLevelParameterfvEXT) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetTextureLevelParameterivEXT) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params);
void      (APIENTRY *glTextureImage3DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glTextureSubImage3DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glCopyTextureSubImage3DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glMultiTexParameterfEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY *glMultiTexParameterfvEXT) (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glMultiTexParameteriEXT) (GLenum texunit, GLenum target, GLenum pname, GLint param);
void      (APIENTRY *glMultiTexParameterivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glMultiTexImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glMultiTexImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glMultiTexSubImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glMultiTexSubImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glCopyMultiTexImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void      (APIENTRY *glCopyMultiTexImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void      (APIENTRY *glCopyMultiTexSubImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void      (APIENTRY *glCopyMultiTexSubImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glGetMultiTexImageEXT) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void      (APIENTRY *glGetMultiTexParameterfvEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMultiTexParameterivEXT) (GLenum texunit, GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetMultiTexLevelParameterfvEXT) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMultiTexLevelParameterivEXT) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params);
void      (APIENTRY *glMultiTexImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glMultiTexSubImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY *glCopyMultiTexSubImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void      (APIENTRY *glBindMultiTextureEXT) (GLenum texunit, GLenum target, GLuint texture);
void      (APIENTRY *glEnableClientStateIndexedEXT) (GLenum array, GLuint index);
void      (APIENTRY *glDisableClientStateIndexedEXT) (GLenum array, GLuint index);
void      (APIENTRY *glMultiTexCoordPointerEXT) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glMultiTexEnvfEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY *glMultiTexEnvfvEXT) (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params);
void      (APIENTRY *glMultiTexEnviEXT) (GLenum texunit, GLenum target, GLenum pname, GLint param);
void      (APIENTRY *glMultiTexEnvivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glMultiTexGendEXT) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
void      (APIENTRY *glMultiTexGendvEXT) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params);
void      (APIENTRY *glMultiTexGenfEXT) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
void      (APIENTRY *glMultiTexGenfvEXT) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params);
void      (APIENTRY *glMultiTexGeniEXT) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
void      (APIENTRY *glMultiTexGenivEXT) (GLenum texunit, GLenum coord, GLenum pname, const GLint *params);
void      (APIENTRY *glGetMultiTexEnvfvEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMultiTexEnvivEXT) (GLenum texunit, GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetMultiTexGendvEXT) (GLenum texunit, GLenum coord, GLenum pname, GLdouble *params);
void      (APIENTRY *glGetMultiTexGenfvEXT) (GLenum texunit, GLenum coord, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetMultiTexGenivEXT) (GLenum texunit, GLenum coord, GLenum pname, GLint *params);
void      (APIENTRY *glGetFloatIndexedvEXT) (GLenum target, GLuint index, GLfloat *data);
void      (APIENTRY *glGetDoubleIndexedvEXT) (GLenum target, GLuint index, GLdouble *data);
void      (APIENTRY *glGetPointerIndexedvEXT) (GLenum target, GLuint index, GLvoid* *data);
void      (APIENTRY *glCompressedTextureImage3DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedTextureImage2DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedTextureImage1DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedTextureSubImage3DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedTextureSubImage2DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedTextureSubImage1DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glGetCompressedTextureImageEXT) (GLuint texture, GLenum target, GLint lod, GLvoid *img);
void      (APIENTRY *glCompressedMultiTexImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedMultiTexImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedMultiTexImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedMultiTexSubImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedMultiTexSubImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glCompressedMultiTexSubImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *bits);
void      (APIENTRY *glGetCompressedMultiTexImageEXT) (GLenum texunit, GLenum target, GLint lod, GLvoid *img);
void      (APIENTRY *glNamedProgramStringEXT) (GLuint program, GLenum target, GLenum format, GLsizei len, const GLvoid *string);
void      (APIENTRY *glNamedProgramLocalParameter4dEXT) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glNamedProgramLocalParameter4dvEXT) (GLuint program, GLenum target, GLuint index, const GLdouble *params);
void      (APIENTRY *glNamedProgramLocalParameter4fEXT) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void      (APIENTRY *glNamedProgramLocalParameter4fvEXT) (GLuint program, GLenum target, GLuint index, const GLfloat *params);
void      (APIENTRY *glGetNamedProgramLocalParameterdvEXT) (GLuint program, GLenum target, GLuint index, GLdouble *params);
void      (APIENTRY *glGetNamedProgramLocalParameterfvEXT) (GLuint program, GLenum target, GLuint index, GLfloat *params);
void      (APIENTRY *glGetNamedProgramivEXT) (GLuint program, GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetNamedProgramStringEXT) (GLuint program, GLenum target, GLenum pname, GLvoid *string);
void      (APIENTRY *glNamedProgramLocalParameters4fvEXT) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params);
void      (APIENTRY *glNamedProgramLocalParameterI4iEXT) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
void      (APIENTRY *glNamedProgramLocalParameterI4ivEXT) (GLuint program, GLenum target, GLuint index, const GLint *params);
void      (APIENTRY *glNamedProgramLocalParametersI4ivEXT) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params);
void      (APIENTRY *glNamedProgramLocalParameterI4uiEXT) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void      (APIENTRY *glNamedProgramLocalParameterI4uivEXT) (GLuint program, GLenum target, GLuint index, const GLuint *params);
void      (APIENTRY *glNamedProgramLocalParametersI4uivEXT) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params);
void      (APIENTRY *glGetNamedProgramLocalParameterIivEXT) (GLuint program, GLenum target, GLuint index, GLint *params);
void      (APIENTRY *glGetNamedProgramLocalParameterIuivEXT) (GLuint program, GLenum target, GLuint index, GLuint *params);
void      (APIENTRY *glTextureParameterIivEXT) (GLuint texture, GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glTextureParameterIuivEXT) (GLuint texture, GLenum target, GLenum pname, const GLuint *params);
void      (APIENTRY *glGetTextureParameterIivEXT) (GLuint texture, GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetTextureParameterIuivEXT) (GLuint texture, GLenum target, GLenum pname, GLuint *params);
void      (APIENTRY *glMultiTexParameterIivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLint *params);
void      (APIENTRY *glMultiTexParameterIuivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLuint *params);
void      (APIENTRY *glGetMultiTexParameterIivEXT) (GLenum texunit, GLenum target, GLenum pname, GLint *params);
void      (APIENTRY *glGetMultiTexParameterIuivEXT) (GLenum texunit, GLenum target, GLenum pname, GLuint *params);
void      (APIENTRY *glProgramUniform1fEXT) (GLuint program, GLint location, GLfloat v0);
void      (APIENTRY *glProgramUniform2fEXT) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
void      (APIENTRY *glProgramUniform3fEXT) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void      (APIENTRY *glProgramUniform4fEXT) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void      (APIENTRY *glProgramUniform1iEXT) (GLuint program, GLint location, GLint v0);
void      (APIENTRY *glProgramUniform2iEXT) (GLuint program, GLint location, GLint v0, GLint v1);
void      (APIENTRY *glProgramUniform3iEXT) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void      (APIENTRY *glProgramUniform4iEXT) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void      (APIENTRY *glProgramUniform1fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform2fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform3fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform4fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
void      (APIENTRY *glProgramUniform1ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform2ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform3ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniform4ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
void      (APIENTRY *glProgramUniformMatrix2fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix3fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix4fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix2x3fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix3x2fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix2x4fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix4x2fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix3x4fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniformMatrix4x3fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY *glProgramUniform1uiEXT) (GLuint program, GLint location, GLuint v0);
void      (APIENTRY *glProgramUniform2uiEXT) (GLuint program, GLint location, GLuint v0, GLuint v1);
void      (APIENTRY *glProgramUniform3uiEXT) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void      (APIENTRY *glProgramUniform4uiEXT) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void      (APIENTRY *glProgramUniform1uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniform2uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniform3uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glProgramUniform4uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
void      (APIENTRY *glNamedBufferDataEXT) (GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
void      (APIENTRY *glNamedBufferSubDataEXT) (GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
GLvoid*   (APIENTRY *glMapNamedBufferEXT) (GLuint buffer, GLenum access);
GLboolean (APIENTRY *glUnmapNamedBufferEXT) (GLuint buffer);
GLvoid*   (APIENTRY *glMapNamedBufferRangeEXT) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
void      (APIENTRY *glFlushMappedNamedBufferRangeEXT) (GLuint buffer, GLintptr offset, GLsizeiptr length);
void      (APIENTRY *glNamedCopyBufferSubDataEXT) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void      (APIENTRY *glGetNamedBufferParameterivEXT) (GLuint buffer, GLenum pname, GLint *params);
void      (APIENTRY *glGetNamedBufferPointervEXT) (GLuint buffer, GLenum pname, GLvoid* *params);
void      (APIENTRY *glGetNamedBufferSubDataEXT) (GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid *data);
void      (APIENTRY *glTextureBufferEXT) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
void      (APIENTRY *glMultiTexBufferEXT) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
void      (APIENTRY *glNamedRenderbufferStorageEXT) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
void      (APIENTRY *glGetNamedRenderbufferParameterivEXT) (GLuint renderbuffer, GLenum pname, GLint *params);
GLenum    (APIENTRY *glCheckNamedFramebufferStatusEXT) (GLuint framebuffer, GLenum target);
void      (APIENTRY *glNamedFramebufferTexture1DEXT) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void      (APIENTRY *glNamedFramebufferTexture2DEXT) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void      (APIENTRY *glNamedFramebufferTexture3DEXT) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void      (APIENTRY *glNamedFramebufferRenderbufferEXT) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void      (APIENTRY *glGetNamedFramebufferAttachmentParameterivEXT) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
void      (APIENTRY *glGenerateTextureMipmapEXT) (GLuint texture, GLenum target);
void      (APIENTRY *glGenerateMultiTexMipmapEXT) (GLenum texunit, GLenum target);
void      (APIENTRY *glFramebufferDrawBufferEXT) (GLuint framebuffer, GLenum mode);
void      (APIENTRY *glFramebufferDrawBuffersEXT) (GLuint framebuffer, GLsizei n, const GLenum *bufs);
void      (APIENTRY *glFramebufferReadBufferEXT) (GLuint framebuffer, GLenum mode);
void      (APIENTRY *glGetFramebufferParameterivEXT) (GLuint framebuffer, GLenum pname, GLint *params);
void      (APIENTRY *glNamedRenderbufferStorageMultisampleEXT) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void      (APIENTRY *glNamedRenderbufferStorageMultisampleCoverageEXT) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
void      (APIENTRY *glNamedFramebufferTextureEXT) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
void      (APIENTRY *glNamedFramebufferTextureLayerEXT) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
void      (APIENTRY *glNamedFramebufferTextureFaceEXT) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
void      (APIENTRY *glTextureRenderbufferEXT) (GLuint texture, GLenum target, GLuint renderbuffer);
void      (APIENTRY *glMultiTexRenderbufferEXT) (GLenum texunit, GLenum target, GLuint renderbuffer);
void      (APIENTRY *glProgramUniform1dEXT) (GLuint program, GLint location, GLdouble x);
void      (APIENTRY *glProgramUniform2dEXT) (GLuint program, GLint location, GLdouble x, GLdouble y);
void      (APIENTRY *glProgramUniform3dEXT) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glProgramUniform4dEXT) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glProgramUniform1dvEXT) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform2dvEXT) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform3dvEXT) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniform4dvEXT) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix2dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix3dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix4dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix2x3dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix2x4dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix3x2dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix3x4dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix4x2dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void      (APIENTRY *glProgramUniformMatrix4x3dvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//@}

/** @name GL_NV_explicit_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/explicit_multisample.txt
 */
//@{
void      (APIENTRY *glGetMultisamplefvNV) (GLenum pname, GLuint index, GLfloat *val);
void      (APIENTRY *glSampleMaskIndexedNV) (GLuint index, GLbitfield mask);
void      (APIENTRY *glTexRenderbufferNV) (GLenum target, GLuint renderbuffer);
//@}

/** @name GL_NV_transform_feedback2
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/transform_feedback2.txt
 */
//@{
void      (APIENTRY *glBindTransformFeedbackNV) (GLenum target, GLuint id);
void      (APIENTRY *glDeleteTransformFeedbacksNV) (GLsizei n, const GLuint *ids);
void      (APIENTRY *glGenTransformFeedbacksNV) (GLsizei n, GLuint *ids);
GLboolean (APIENTRY *glIsTransformFeedbackNV) (GLuint id);
void      (APIENTRY *glPauseTransformFeedbackNV) (void);
void      (APIENTRY *glResumeTransformFeedbackNV) (void);
void      (APIENTRY *glDrawTransformFeedbackNV) (GLenum mode, GLuint id);
//@}

/** @name GL_AMD_performance_monitor
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/performance_monitor.txt
 */
//@{
void      (APIENTRY *glGetPerfMonitorGroupsAMD) (GLint *numGroups, GLsizei groupsSize, GLuint *groups);
void      (APIENTRY *glGetPerfMonitorCountersAMD) (GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters);
void      (APIENTRY *glGetPerfMonitorGroupStringAMD) (GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString);
void      (APIENTRY *glGetPerfMonitorCounterStringAMD) (GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString);
void      (APIENTRY *glGetPerfMonitorCounterInfoAMD) (GLuint group, GLuint counter, GLenum pname, GLvoid *data);
void      (APIENTRY *glGenPerfMonitorsAMD) (GLsizei n, GLuint *monitors);
void      (APIENTRY *glDeletePerfMonitorsAMD) (GLsizei n, GLuint *monitors);
void      (APIENTRY *glSelectPerfMonitorCountersAMD) (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *counterList);
void      (APIENTRY *glBeginPerfMonitorAMD) (GLuint monitor);
void      (APIENTRY *glEndPerfMonitorAMD) (GLuint monitor);
void      (APIENTRY *glGetPerfMonitorCounterDataAMD) (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten);
//@}

/** @name GL_AMD_vertex_shader_tesselator
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/vertex_shader_tesselator.txt
 */
//@{
void      (APIENTRY *glTessellationFactorAMD) (GLfloat factor);
void      (APIENTRY *glTessellationModeAMD) (GLenum mode);
//@}

/** @name GL_EXT_provoking_vertex
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/provoking_vertex.txt
 */
//@{
void      (APIENTRY *glProvokingVertexEXT) (GLenum mode);
//@}

/** @name GL_AMD_draw_buffers_blend
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/draw_buffers_blend.txt
 */
//@{
void      (APIENTRY *glBlendFuncIndexedAMD) (GLuint buf, GLenum src, GLenum dst);
void      (APIENTRY *glBlendFuncSeparateIndexedAMD) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void      (APIENTRY *glBlendEquationIndexedAMD) (GLuint buf, GLenum mode);
void      (APIENTRY *glBlendEquationSeparateIndexedAMD) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
//@}

/** @name GL_APPLE_texture_range
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/texture_range.txt
 */
//@{
void      (APIENTRY *glTextureRangeAPPLE) (GLenum target, GLsizei length, const GLvoid *pointer);
void      (APIENTRY *glGetTexParameterPointervAPPLE) (GLenum target, GLenum pname, GLvoid* *params);
//@}

/** @name GL_APPLE_vertex_program_evaluators
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/vertex_program_evaluators.txt
 */
//@{
void      (APIENTRY *glEnableVertexAttribAPPLE) (GLuint index, GLenum pname);
void      (APIENTRY *glDisableVertexAttribAPPLE) (GLuint index, GLenum pname);
GLboolean (APIENTRY *glIsVertexAttribEnabledAPPLE) (GLuint index, GLenum pname);
void      (APIENTRY *glMapVertexAttrib1dAPPLE) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void      (APIENTRY *glMapVertexAttrib1fAPPLE) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void      (APIENTRY *glMapVertexAttrib2dAPPLE) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void      (APIENTRY *glMapVertexAttrib2fAPPLE) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
//@}

/** @name GL_APPLE_object_purgeable
 *  See http://oss.sgi.com/projects/ogl-sample/registry/APPLE/object_purgeable.txt
 */
//@{
GLenum    (APIENTRY *glObjectPurgeableAPPLE) (GLenum objectType, GLuint name, GLenum option);
GLenum    (APIENTRY *glObjectUnpurgeableAPPLE) (GLenum objectType, GLuint name, GLenum option);
void      (APIENTRY *glGetObjectParameterivAPPLE) (GLenum objectType, GLuint name, GLenum pname, GLint *params);
//@}

/** @name GL_NV_video_capture
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/video_capture.txt
 */
//@{
void      (APIENTRY *glBeginVideoCaptureNV) (GLuint video_capture_slot);
void      (APIENTRY *glBindVideoCaptureStreamBufferNV) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
void      (APIENTRY *glBindVideoCaptureStreamTextureNV) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
void      (APIENTRY *glEndVideoCaptureNV) (GLuint video_capture_slot);
void      (APIENTRY *glGetVideoCaptureivNV) (GLuint video_capture_slot, GLenum pname, GLint *params);
void      (APIENTRY *glGetVideoCaptureStreamivNV) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint *params);
void      (APIENTRY *glGetVideoCaptureStreamfvNV) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat *params);
void      (APIENTRY *glGetVideoCaptureStreamdvNV) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble *params);
GLenum    (APIENTRY *glVideoCaptureNV) (GLuint video_capture_slot, GLuint *sequence_num, GLuint64EXT *capture_time);
void      (APIENTRY *glVideoCaptureStreamParameterivNV) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint *params);
void      (APIENTRY *glVideoCaptureStreamParameterfvNV) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat *params);
void      (APIENTRY *glVideoCaptureStreamParameterdvNV) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble *params);
//@}

/** @name GL_NV_copy_image
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/copy_image.txt
 */
//@{
void      (APIENTRY *glCopyImageSubDataNV) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
//@}

/** @name GL_EXT_separate_shader_objects
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/separate_shader_objects.txt
 */
//@{
void      (APIENTRY *glUseShaderProgramEXT) (GLenum type, GLuint program);
void      (APIENTRY *glActiveProgramEXT) (GLuint program);
GLuint    (APIENTRY *glCreateShaderProgramEXT) (GLenum type, const GLchar *string);
//@}

/** @name GL_NV_shader_buffer_load
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/shader_buffer_load.txt
 */
//@{
void      (APIENTRY *glMakeBufferResidentNV) (GLenum target, GLenum access);
void      (APIENTRY *glMakeBufferNonResidentNV) (GLenum target);
GLboolean (APIENTRY *glIsBufferResidentNV) (GLenum target);
void      (APIENTRY *glMakeNamedBufferResidentNV) (GLuint buffer, GLenum access);
void      (APIENTRY *glMakeNamedBufferNonResidentNV) (GLuint buffer);
GLboolean (APIENTRY *glIsNamedBufferResidentNV) (GLuint buffer);
void      (APIENTRY *glGetBufferParameterui64vNV) (GLenum target, GLenum pname, GLuint64EXT *params);
void      (APIENTRY *glGetNamedBufferParameterui64vNV) (GLuint buffer, GLenum pname, GLuint64EXT *params);
void      (APIENTRY *glGetIntegerui64vNV) (GLenum value, GLuint64EXT *result);
void      (APIENTRY *glUniformui64NV) (GLint location, GLuint64EXT value);
void      (APIENTRY *glUniformui64vNV) (GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glGetUniformui64vNV) (GLuint program, GLint location, GLuint64EXT *params);
void      (APIENTRY *glProgramUniformui64NV) (GLuint program, GLint location, GLuint64EXT value);
void      (APIENTRY *glProgramUniformui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
//@}

/** @name GL_NV_vertex_buffer_unified_memory
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_buffer_unified_memory.txt
 */
//@{
void      (APIENTRY *glBufferAddressRangeNV) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
void      (APIENTRY *glVertexFormatNV) (GLint size, GLenum type, GLsizei stride);
void      (APIENTRY *glNormalFormatNV) (GLenum type, GLsizei stride);
void      (APIENTRY *glColorFormatNV) (GLint size, GLenum type, GLsizei stride);
void      (APIENTRY *glIndexFormatNV) (GLenum type, GLsizei stride);
void      (APIENTRY *glTexCoordFormatNV) (GLint size, GLenum type, GLsizei stride);
void      (APIENTRY *glEdgeFlagFormatNV) (GLsizei stride);
void      (APIENTRY *glSecondaryColorFormatNV) (GLint size, GLenum type, GLsizei stride);
void      (APIENTRY *glFogCoordFormatNV) (GLenum type, GLsizei stride);
void      (APIENTRY *glVertexAttribFormatNV) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
void      (APIENTRY *glVertexAttribIFormatNV) (GLuint index, GLint size, GLenum type, GLsizei stride);
void      (APIENTRY *glGetIntegerui64i_vNV) (GLenum value, GLuint index, GLuint64EXT *result);
//@}

/** @name GL_NV_texture_barrier
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/texture_barrier.txt
 */
//@{
void      (APIENTRY *glTextureBarrierNV) (void);
//@}

/** @name GL_EXT_shader_image_load_store
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/shader_image_load_store.txt
 */
//@{
void      (APIENTRY *glBindImageTextureEXT) (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
void      (APIENTRY *glMemoryBarrierEXT) (GLbitfield barriers);
//@}

/** @name GL_EXT_vertex_attrib_64bit
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/vertex_attrib_64bit.txt
 */
//@{
void      (APIENTRY *glVertexAttribL1dEXT) (GLuint index, GLdouble x);
void      (APIENTRY *glVertexAttribL2dEXT) (GLuint index, GLdouble x, GLdouble y);
void      (APIENTRY *glVertexAttribL3dEXT) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
void      (APIENTRY *glVertexAttribL4dEXT) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void      (APIENTRY *glVertexAttribL1dvEXT) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribL2dvEXT) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribL3dvEXT) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribL4dvEXT) (GLuint index, const GLdouble *v);
void      (APIENTRY *glVertexAttribLPointerEXT) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void      (APIENTRY *glGetVertexAttribLdvEXT) (GLuint index, GLenum pname, GLdouble *params);
void      (APIENTRY *glVertexArrayVertexAttribLOffsetEXT) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//@}

/** @name GL_NV_gpu_program5
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/gpu_program5.txt
 */
//@{
void      (APIENTRY *glProgramSubroutineParametersuivNV) (GLenum target, GLsizei count, const GLuint *params);
void      (APIENTRY *glGetProgramSubroutineParameteruivNV) (GLenum target, GLuint index, GLuint *param);
//@}

/** @name GL_NV_gpu_shader5
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/gpu_shader5.txt
 */
//@{
void      (APIENTRY *glUniform1i64NV) (GLint location, GLint64EXT x);
void      (APIENTRY *glUniform2i64NV) (GLint location, GLint64EXT x, GLint64EXT y);
void      (APIENTRY *glUniform3i64NV) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
void      (APIENTRY *glUniform4i64NV) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
void      (APIENTRY *glUniform1i64vNV) (GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glUniform2i64vNV) (GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glUniform3i64vNV) (GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glUniform4i64vNV) (GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glUniform1ui64NV) (GLint location, GLuint64EXT x);
void      (APIENTRY *glUniform2ui64NV) (GLint location, GLuint64EXT x, GLuint64EXT y);
void      (APIENTRY *glUniform3ui64NV) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
void      (APIENTRY *glUniform4ui64NV) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
void      (APIENTRY *glUniform1ui64vNV) (GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glUniform2ui64vNV) (GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glUniform3ui64vNV) (GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glUniform4ui64vNV) (GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glGetUniformi64vNV) (GLuint program, GLint location, GLint64EXT *params);
void      (APIENTRY *glProgramUniform1i64NV) (GLuint program, GLint location, GLint64EXT x);
void      (APIENTRY *glProgramUniform2i64NV) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
void      (APIENTRY *glProgramUniform3i64NV) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
void      (APIENTRY *glProgramUniform4i64NV) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
void      (APIENTRY *glProgramUniform1i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glProgramUniform2i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glProgramUniform3i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glProgramUniform4i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
void      (APIENTRY *glProgramUniform1ui64NV) (GLuint program, GLint location, GLuint64EXT x);
void      (APIENTRY *glProgramUniform2ui64NV) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
void      (APIENTRY *glProgramUniform3ui64NV) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
void      (APIENTRY *glProgramUniform4ui64NV) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
void      (APIENTRY *glProgramUniform1ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glProgramUniform2ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glProgramUniform3ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
void      (APIENTRY *glProgramUniform4ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
//@}

/** @name GL_NV_vertex_attrib_integer_64bit
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vertex_attrib_integer_64bit.txt
 */
//@{
void      (APIENTRY *glVertexAttribL1i64NV) (GLuint index, GLint64EXT x);
void      (APIENTRY *glVertexAttribL2i64NV) (GLuint index, GLint64EXT x, GLint64EXT y);
void      (APIENTRY *glVertexAttribL3i64NV) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
void      (APIENTRY *glVertexAttribL4i64NV) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
void      (APIENTRY *glVertexAttribL1i64vNV) (GLuint index, const GLint64EXT *v);
void      (APIENTRY *glVertexAttribL2i64vNV) (GLuint index, const GLint64EXT *v);
void      (APIENTRY *glVertexAttribL3i64vNV) (GLuint index, const GLint64EXT *v);
void      (APIENTRY *glVertexAttribL4i64vNV) (GLuint index, const GLint64EXT *v);
void      (APIENTRY *glVertexAttribL1ui64NV) (GLuint index, GLuint64EXT x);
void      (APIENTRY *glVertexAttribL2ui64NV) (GLuint index, GLuint64EXT x, GLuint64EXT y);
void      (APIENTRY *glVertexAttribL3ui64NV) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
void      (APIENTRY *glVertexAttribL4ui64NV) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
void      (APIENTRY *glVertexAttribL1ui64vNV) (GLuint index, const GLuint64EXT *v);
void      (APIENTRY *glVertexAttribL2ui64vNV) (GLuint index, const GLuint64EXT *v);
void      (APIENTRY *glVertexAttribL3ui64vNV) (GLuint index, const GLuint64EXT *v);
void      (APIENTRY *glVertexAttribL4ui64vNV) (GLuint index, const GLuint64EXT *v);
void      (APIENTRY *glGetVertexAttribLi64vNV) (GLuint index, GLenum pname, GLint64EXT *params);
void      (APIENTRY *glGetVertexAttribLui64vNV) (GLuint index, GLenum pname, GLuint64EXT *params);
void      (APIENTRY *glVertexAttribLFormatNV) (GLuint index, GLint size, GLenum type, GLsizei stride);
//@}

/** @name GL_AMD_name_gen_delete
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/name_gen_delete.txt
 */
//@{
void      (APIENTRY *glGenNamesAMD) (GLenum identifier, GLuint num, GLuint *names);
void      (APIENTRY *glDeleteNamesAMD) (GLenum identifier, GLuint num, const GLuint *names);
GLboolean (APIENTRY *glIsNameAMD) (GLenum identifier, GLuint name);
//@}

/** @name GL_AMD_debug_output
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/debug_output.txt
 */
//@{
void      (APIENTRY *glDebugMessageEnableAMD) (GLenum category, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
void      (APIENTRY *glDebugMessageInsertAMD) (GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar *buf);
void      (APIENTRY *glDebugMessageCallbackAMD) (GLDEBUGPROCAMD callback, GLvoid *userParam);
GLuint    (APIENTRY *glGetDebugMessageLogAMD) (GLuint count, GLsizei bufsize, GLenum *categories, GLuint *severities, GLuint *ids, GLsizei *lengths, GLchar *message);
//@}

/** @name GL_NV_vdpau_interop
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/vdpau_interop.txt
 */
//@{
void      (APIENTRY *glVDPAUInitNV) (const GLvoid *vdpDevice, const GLvoid *getProcAddress);
void      (APIENTRY *glVDPAUFiniNV) (void);
GLvdpauSurfaceNV (APIENTRY *glVDPAURegisterVideoSurfaceNV) (GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLvdpauSurfaceNV (APIENTRY *glVDPAURegisterOutputSurfaceNV) (GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
void      (APIENTRY *glVDPAUIsSurfaceNV) (GLvdpauSurfaceNV surface);
void      (APIENTRY *glVDPAUUnregisterSurfaceNV) (GLvdpauSurfaceNV surface);
void      (APIENTRY *glVDPAUGetSurfaceivNV) (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
void      (APIENTRY *glVDPAUSurfaceAccessNV) (GLvdpauSurfaceNV surface, GLenum access);
void      (APIENTRY *glVDPAUMapSurfacesNV) (GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces);
void      (APIENTRY *glVDPAUUnmapSurfacesNV) (GLsizei numSurface, const GLvdpauSurfaceNV *surfaces);
//@}

/** @name GL_NV_texture_multisample
 *  See http://oss.sgi.com/projects/ogl-sample/registry/NV/texture_multisample.txt
 */
//@{
void      (APIENTRY *glTexImage2DMultisampleCoverageNV) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
void      (APIENTRY *glTexImage3DMultisampleCoverageNV) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
void      (APIENTRY *glTextureImage2DMultisampleNV) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
void      (APIENTRY *glTextureImage3DMultisampleNV) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
void      (APIENTRY *glTextureImage2DMultisampleCoverageNV) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
void      (APIENTRY *glTextureImage3DMultisampleCoverageNV) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
//@}

/** @name GL_AMD_sample_positions
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/sample_positions.txt
 */
//@{
void      (APIENTRY *glSetMultisamplefvAMD) (GLenum pname, GLuint index, const GLfloat *val);
//@}

/** @name GL_EXT_x11_sync_object
 *  See http://oss.sgi.com/projects/ogl-sample/registry/EXT/x11_sync_object.txt
 */
//@{
GLsync    (APIENTRY *glImportSyncEXT) (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);
//@}

/** @name GL_AMD_multi_draw_indirect
 *  See http://oss.sgi.com/projects/ogl-sample/registry/AMD/multi_draw_indirect.txt
 */
//@{
void      (APIENTRY *glMultiDrawArraysIndirectAMD) (GLenum mode, const GLvoid *indirect, GLsizei primcount, GLsizei stride);
void      (APIENTRY *glMultiDrawElementsIndirectAMD) (GLenum mode, GLenum type, const GLvoid *indirect, GLsizei primcount, GLsizei stride);
//@}



/* 211 extensions, 1943 total functions */

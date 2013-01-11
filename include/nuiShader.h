//
//  nuiShader.h
//  nui3
//
//  Created by Sébastien Métrot on 1/10/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#pragma once

#include "nui.h"

enum nuiShaderKind
{
  eVertexShader = GL_VERTEX_SHADER,
  eFragmentShader = GL_FRAGMENT_SHADER
};

class nuiShader : public nuiRefCount
{
public:
  nuiShader(nuiShaderKind kind, const nglString& rSource);
  nuiShader(nuiShaderKind kind, nglIStream& rSource);
  virtual ~nuiShader();

  bool Load();
  void Delete();

  GLuint GetShader() const;
  bool IsValid() const;
  const nglString& GetError() const;
private:
  nuiShaderKind mKind;
  nglString mSource;
  nglString mError;
  GLuint mShader;
};

class nuiShaderProgram
{
public:
  nuiShaderProgram();
  virtual ~nuiShaderProgram();

  bool AddShader(nuiShader* pShader);


  // Geometry Shader: Input Type, Output and Number of Vertices out
  void       SetInputPrimitiveType(int InputPrimitiveType);   //!< Set the input primitive type for the geometry shader
  void       SetOutputPrimitiveType(int OutputPrimitiveType); //!< Set the output primitive type for the geometry shader
  void       SetVerticesOut(int VerticesOut);                 //!< Set the maximal number of vertices the geometry shader can output

  GLint       GetUniformLocation(const char *name);  //!< Retrieve Location (index) of a Uniform Variable
  GLint       GetUniformLocation(const nglString& name);  //!< Retrieve Location (index) of a Uniform Variable

  // Submitting Uniform Variables. You can set varname to 0 and specifiy index retrieved with GetUniformLocation (best performance)
  void       SetUniform1f(const char* varname, GLfloat v0);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
  void       SetUniform2f(const char* varname, GLfloat v0, GLfloat v1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
  void       SetUniform3f(const char* varname, GLfloat v0, GLfloat v1, GLfloat v2);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
  void       SetUniform4f(const char* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.

  void       SetUniform1i(const char* varname, GLint v0);  //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
  void       SetUniform2i(const char* varname, GLint v0, GLint v1); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
  void       SetUniform3i(const char* varname, GLint v0, GLint v1, GLint v2); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
  void       SetUniform4i(const char* varname, GLint v0, GLint v1, GLint v2, GLint v3); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.

  // Arrays
  void       SetUniform1fv(const char* varname, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform2fv(const char* varname, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform3fv(const char* varname, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform4fv(const char* varname, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.

  void       SetUniform1iv(const char* varname, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform2iv(const char* varname, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform3iv(const char* varname, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform4iv(const char* varname, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.

  void       SetUniformMatrix2fv(const char* varname, GLsizei count, GLboolean transpose, GLfloat *value); //!< Specify values of uniform 2x2 matrix. \param varname The name of the uniform variable.
  void       SetUniformMatrix3fv(const char* varname, GLsizei count, GLboolean transpose, GLfloat *value); //!< Specify values of uniform 3x3 matrix. \param varname The name of the uniform variable.
  void       SetUniformMatrix4fv(const char* varname, GLsizei count, GLboolean transpose, GLfloat *value); //!< Specify values of uniform 4x4 matrix. \param varname The name of the uniform variable.

  // Receive Uniform variables:
  void       GetUniformfv(const char* varname, GLfloat* values); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
  void       GetUniformiv(const char* varname, GLint* values); //!< Receive value of uniform variable. \param varname The name of the uniform variable.

  ////////////////// Set Uniforms with index instead of name:
  // Submitting Uniform Variables. You can set varname to 0 and specifiy index retrieved with GetUniformLocation (best performance)
  void       SetUniform1f(GLint index, GLfloat v0);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
  void       SetUniform2f(GLint index, GLfloat v0, GLfloat v1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
  void       SetUniform3f(GLint index, GLfloat v0, GLfloat v1, GLfloat v2);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
  void       SetUniform4f(GLint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.

  void       SetUniform1i(GLint index, GLint v0);  //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
  void       SetUniform2i(GLint index, GLint v0, GLint v1); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
  void       SetUniform3i(GLint index, GLint v0, GLint v1, GLint v2); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
  void       SetUniform4i(GLint index, GLint v0, GLint v1, GLint v2, GLint v3); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.

  // Arrays
  void       SetUniform1fv(GLint index, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform2fv(GLint index, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform3fv(GLint index, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform4fv(GLint index, GLsizei count, GLfloat *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.

  void       SetUniform1iv(GLint index, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform2iv(GLint index, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform3iv(GLint index, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.
  void       SetUniform4iv(GLint index, GLsizei count, GLint *value); //!< Specify values of uniform array. \param varname The name of the uniform variable.

  void       SetUniformMatrix2fv(GLint index, GLsizei count, GLboolean transpose, GLfloat *value); //!< Specify values of uniform 2x2 matrix. \param varname The name of the uniform variable.
  void       SetUniformMatrix3fv(GLint index, GLsizei count, GLboolean transpose, GLfloat *value); //!< Specify values of uniform 3x3 matrix. \param varname The name of the uniform variable.
  void       SetUniformMatrix4fv(GLint index, GLsizei count, GLboolean transpose, GLfloat *value); //!< Specify values of uniform 4x4 matrix. \param varname The name of the uniform variable.

  // Receive Uniform variables:
  void       GetUniformfv(GLint index, GLfloat* values); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
  void       GetUniformiv(GLint index, GLint* values); //!< Receive value of uniform variable. \param varname The name of the uniform variable.



  /*! Bind Vertex Attribute Location
   Warning: NVidia implementation is different than the GLSL standard:
   GLSL attempts to eliminate aliasing of vertex attributes but this is
   integral to NVIDIAís hardware approach and necessary for maintaining
   compatibility with existing OpenGL applications that NVIDIA customers rely on.
   NVIDIAís GLSL implementation therefore does not allow built-in vertex attributes
   to collide with a generic vertex attributes that is assigned to a particular vertex
   attribute index with glBindAttribLocation. For example, you should not use gl_Normal
   (a built-in vertex attribute) and also use glBindAttribLocation to bind a generic
   vertex attribute named "whatever" to vertex attribute index 2 because gl_Normal aliases to index 2.
   \verbatim
   gl_Vertex                0
   gl_Normal                2
   gl_Color                 3
   gl_SecondaryColor        4
   gl_FogCoord              5
   gl_MultiTexCoord0        8
   gl_MultiTexCoord1        9
   gl_MultiTexCoord2       10
   gl_MultiTexCoord3       11
   gl_MultiTexCoord4       12
   gl_MultiTexCoord5       13
   gl_MultiTexCoord6       14
   gl_MultiTexCoord7       15
   \endverbatim

   \param index Index of the variable
   \param name Name of the attribute.
   */
  //! This method simply calls glBindAttribLocation for the current ProgramObject.
  void        BindAttribLocation(GLint index, GLchar* name);

  //GLfloat
  void        SetVertexAttrib1f(GLuint index, GLfloat v0); //!< Specify value of attribute.
  void        SetVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1); //!< Specify value of attribute.
  void        SetVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2); //!< Specify value of attribute.
  void        SetVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); //!< Specify value of attribute.

#ifndef GL_ES_VERSION_2_0
  //GLdouble
  void        SetVertexAttrib1d(GLuint index, GLdouble v0); //!< Specify value of attribute.
  void        SetVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1); //!< Specify value of attribute.
  void        SetVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2); //!< Specify value of attribute.
  void        SetVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3); //!< Specify value of attribute.

  //GLshort
  void        SetVertexAttrib1s(GLuint index, GLshort v0); //!< Specify value of attribute.
  void        SetVertexAttrib2s(GLuint index, GLshort v0, GLshort v1); //!< Specify value of attribute.
  void        SetVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2); //!< Specify value of attribute.
  void        SetVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3); //!< Specify value of attribute.

  // Normalized Byte (for example for RGBA colors)
  void        SetVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3); //!< Specify value of attribute. Values will be normalized.
#endif

private:
  GLuint mProgram;
};
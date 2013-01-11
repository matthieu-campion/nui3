//
//  nuiShader.cpp
//  nui3
//
//  Created by Sébastien Métrot on 1/10/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#include "nui.h"
#include "nuiShader.h"

nuiShader::nuiShader(nuiShaderKind kind, const nglString& rSource)
: mKind(kind), mShader(0), mSource(rSource)
{

}

nuiShader::nuiShader(nuiShaderKind kind, nglIStream& rSource)
: mKind(kind), mShader(0)
{
  rSource.ReadText(mSource);
}

nuiShader::~nuiShader()
{
  if (IsValid())
    Delete();
}

bool nuiShader::Load()
{
  NGL_ASSERT(mShader == 0);

  GLint compiled;
  // Create the shader object
  mShader = glCreateShader((GLenum)mKind);
  if (mShader == 0)
    return false;
  // Load the shader source
  const char* src = mSource.GetChars();
  glShaderSource(mShader, 1, &src, NULL);
  // Compile the shader
  glCompileShader(mShader);
  // Check the compile status
  glGetShaderiv(mShader, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
  {
    GLint infoLen = 0;
    glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1)
    {
      char* infoLog = (char*)malloc(sizeof(char) * infoLen);
      glGetShaderInfoLog(mShader, infoLen, NULL, infoLog);
      mError = infoLog;
      free(infoLog);
    }
    glDeleteShader(mShader);
    mShader = 0;
    return false;
  }

  return true;
}

void nuiShader::Delete()
{
  NGL_ASSERT(mShader != 0);
  glDeleteShader(mShader);
  mShader = 0;
}

GLuint nuiShader::GetShader() const
{
  return mShader;
}

bool nuiShader::IsValid() const
{
  return mShader != 0;
}

const nglString& nuiShader::GetError() const
{
  return mError;
}


////////////////////// nuiShader Program:
void nuiShaderProgram::SetInputPrimitiveType(int InputPrimitiveType)
{

}

void nuiShaderProgram::SetOutputPrimitiveType(int OutputPrimitiveType)
{

}

void nuiShaderProgram::SetVerticesOut(int VerticesOut)
{

}

GLint nuiShaderProgram::GetUniformLocation(const char *name)
{
	return glGetUniformLocation(mProgram, name);
}

GLint nuiShaderProgram::GetUniformLocation(const nglString& name)
{
	return glGetUniformLocation(mProgram, name.GetChars());
}

void nuiShaderProgram::SetUniform1f(const char* varname, GLfloat v0)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform1f(index, v0);
}

void nuiShaderProgram::SetUniform2f(const char* varname, GLfloat v0, GLfloat v1)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform2f(index, v0, v1);
}

void nuiShaderProgram::SetUniform3f(const char* varname, GLfloat v0, GLfloat v1, GLfloat v2)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform3f(index, v0, v1, v2);
}

void nuiShaderProgram::SetUniform4f(const char* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform4f(index, v0, v1, v2, v3);
}


void nuiShaderProgram::SetUniform1i(const char* varname, GLint v0)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform1i(index, v0);
}

void nuiShaderProgram::SetUniform2i(const char* varname, GLint v0, GLint v1)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform2i(index, v0, v1);
}

void nuiShaderProgram::SetUniform3i(const char* varname, GLint v0, GLint v1, GLint v2)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform3i(index, v0, v1, v2);
}

void nuiShaderProgram::SetUniform4i(const char* varname, GLint v0, GLint v1, GLint v2, GLint v3)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform4i(index, v0, v1, v2, v3);
}

// Arrays
void nuiShaderProgram::SetUniform1fv(const char* varname, GLsizei count, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform1fv(index, count, value);
}

void nuiShaderProgram::SetUniform2fv(const char* varname, GLsizei count, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform2fv(index, count, value);
}

void nuiShaderProgram::SetUniform3fv(const char* varname, GLsizei count, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform3fv(index, count, value);
}

void nuiShaderProgram::SetUniform4fv(const char* varname, GLsizei count, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform4fv(index, count, value);
}


void nuiShaderProgram::SetUniform1iv(const char* varname, GLsizei count, GLint *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform1iv(index, count, value);
}

void nuiShaderProgram::SetUniform2iv(const char* varname, GLsizei count, GLint *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform2iv(index, count, value);
}

void nuiShaderProgram::SetUniform3iv(const char* varname, GLsizei count, GLint *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform3iv(index, count, value);
}


void nuiShaderProgram::SetUniform4iv(const char* varname, GLsizei count, GLint *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniform4iv(index, count, value);
}

void nuiShaderProgram::SetUniformMatrix2fv(const char* varname, GLsizei count, GLboolean transpose, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniformMatrix2fv(index, count, transpose, value);
}

void nuiShaderProgram::SetUniformMatrix3fv(const char* varname, GLsizei count, GLboolean transpose, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniformMatrix3fv(index, count, transpose, value);
}

void nuiShaderProgram::SetUniformMatrix4fv(const char* varname, GLsizei count, GLboolean transpose, GLfloat *value)
{
  GLint index = GetUniformLocation(varname);
  if (index == -1)
    return false;  // can't find variable / invalid index

  glUniformMatrix4fv(index, count, transpose, value);
}


// Receive Uniform variables:
void nuiShaderProgram::GetUniformfv(const char* varname, GLfloat* values)
{
  GLint index = GetUniformLocation(varname);
  NGL_ASSERT(index == -1);
	glGetUniformfv(mProgram, index, values);
}

void nuiShaderProgram::GetUniformiv(const char* varname, GLint* values)
{
  GLint index = GetUniformLocation(varname);
  NGL_ASSERT(index == -1);
	glGetUniformiv(mProgram, index, values);
}


////////////////// Set Uniforms with index instead of name:
void nuiShaderProgram::SetUniform1f(GLint index, GLfloat v0)
{
  glUniform1f(index, v0);
}

void nuiShaderProgram::SetUniform2f(GLint index, GLfloat v0, GLfloat v1)
{
  glUniform2f(index, v0, v1);
}

void nuiShaderProgram::SetUniform3f(GLint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
  glUniform3f(index, v0, v1, v2);
}

void nuiShaderProgram::SetUniform4f(GLint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  glUniform4f(index, v0, v1, v2, v3);
}


void nuiShaderProgram::SetUniform1i(GLint index, GLint v0)
{
  glUniform1i(index, v0);
}

void nuiShaderProgram::SetUniform2i(GLint index, GLint v0, GLint v1)
{
  glUniform2i(index, v0, v1);
}

void nuiShaderProgram::SetUniform3i(GLint index, GLint v0, GLint v1, GLint v2)
{
  glUniform3i(index, v0, v1, v2);
}

void nuiShaderProgram::SetUniform4i(GLint index, GLint v0, GLint v1, GLint v2, GLint v3)
{
  glUniform4i(index, v0, v1, v2, v3);
}


// Arrays
void nuiShaderProgram::SetUniform1fv(GLint index, GLsizei count, GLfloat *value)
{
  glUniform1fv(index, count, value);
}

void nuiShaderProgram::SetUniform2fv(GLint index, GLsizei count, GLfloat *value)
{
  glUniform2fv(index, count, value);
}

void nuiShaderProgram::SetUniform3fv(GLint index, GLsizei count, GLfloat *value)
{
  glUniform3fv(index, count, value);
}

void nuiShaderProgram::SetUniform4fv(GLint index, GLsizei count, GLfloat *value)
{
  glUniform4fv(index, count, value);
}


void nuiShaderProgram::SetUniform1iv(GLint index, GLsizei count, GLint *value)
{
  glUniform1iv(index, count, value);
}

void nuiShaderProgram::SetUniform2iv(GLint index, GLsizei count, GLint *value)
{
  glUniform2iv(index, count, value);
}

void nuiShaderProgram::SetUniform3iv(GLint index, GLsizei count, GLint *value)
{
  glUniform3iv(index, count, value);
}

void nuiShaderProgram::SetUniform4iv(GLint index, GLsizei count, GLint *value)
{
  glUniform4iv(index, count, value);
}


void nuiShaderProgram::SetUniformMatrix2fv(GLint index, GLsizei count, GLboolean transpose, GLfloat *value)
{
  glUniformMatrix2fv(index, count, transpose, value);
}

void nuiShaderProgram::SetUniformMatrix3fv(GLint index, GLsizei count, GLboolean transpose, GLfloat *value)
{
  glUniformMatrix3fv(index, count, transpose, value);
}

void nuiShaderProgram::SetUniformMatrix4fv(GLint index, GLsizei count, GLboolean transpose, GLfloat *value)
{
  glUniformMatrix4fv(index, count, transpose, value);
}


// Receive Uniform variables:
void nuiShaderProgram::GetUniformfv(GLint index, GLfloat* values)
{
  glGetUniformfv(mProgram, index, values);
}

void nuiShaderProgram::GetUniformiv(GLint index, GLint* values)
{
  glGetUniformiv(mProgram, index, values);
}

//////////////////////////////////////////////////////////////////////
void nuiShaderProgram::BindAttribLocation(GLint index, GLchar* name)
{
  glBindAttribLocation(mProgram, index, name);
}

//GLfloat
void nuiShaderProgram::SetVertexAttrib1f(GLuint index, GLfloat v0)
{
  glVertexAttrib1f(index, v0);
}

void nuiShaderProgram::SetVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
{
  glVertexAttrib2f(index, v0, v1);
}

void nuiShaderProgram::SetVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
  glVertexAttrib3f(index, v0, v1, v2);
}

void nuiShaderProgram::SetVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  glVertexAttrib4f(index, v0, v1, v2, v3);
}


//GLdouble
#ifndef GL_ES_VERSION_2_0
void nuiShaderProgram::SetVertexAttrib1d(GLuint index, GLdouble v0)
{
  glVertexAttrib1d(index, v0);
}

void nuiShaderProgram::SetVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1)
{
  glVertexAttrib2d(index, v0, v1);
}

void nuiShaderProgram::SetVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2)
{
  glVertexAttrib3d(index, v0, v1, v2);
}

void nuiShaderProgram::SetVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
{
  glVertexAttrib4d(index, v0, v1, v2, v3);
}


//GLshort
void nuiShaderProgram::SetVertexAttrib1s(GLuint index, GLshort v0)
{
  glVertexAttrib1s(index, v0);
}

void nuiShaderProgram::SetVertexAttrib2s(GLuint index, GLshort v0, GLshort v1)
{
  glVertexAttrib2s(index, v0, v1);
}

void nuiShaderProgram::SetVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2)
{
  glVertexAttrib3s(index, v0, v1, v2);
}

void nuiShaderProgram::SetVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3)
{
  glVertexAttrib4s(index, v0, v1, v2, v3);
}


// Normalized Byte (for example for RGBA colors)
void nuiShaderProgram::SetVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3)
{
  glVertexAttrib4Nub(index, v0, v1, v2, v3);
}
#endif



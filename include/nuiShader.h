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

enum nuiUniformType
{
  nuiUniformFloat,
  nuiUniformDouble,
  nuiUniformInt,
  nuiUniformFMat,
  nuiUniformDMat,
};

class nuiShader;
class nuiShaderProgram;

class nuiUniformDesc
{
public:
  nuiUniformDesc()
  : mType(0), mLocation(0)
  {}

  nuiUniformDesc(const nglString& rName, GLenum Type, GLuint Location)
  : mName(rName), mType(Type), mLocation(Location)
  {
  }

  nglString mName;
  GLenum mType;
  GLuint mLocation;
};


class nuiShaderState : public nuiRefCount
{
public:
  nuiShaderState(const nuiShaderState& rOriginal);
  virtual ~nuiShaderState();

  void Clear();

  void Set(const nglString& rName, const GLfloat* pVal, int32 size);
  void Set(const nglString& rName, const GLint* pVal, int32 size);
  void Set(const nglString& rName, const nuiMatrix& rMatrix);
  void Set(GLint loc, const GLfloat* pVal, int32 size);
  void Set(GLint loc, const GLint* pVal, int32 size);
  void Set(GLint loc, const nuiMatrix& rMatrix);

  const GLfloat* GetFloat(const nglString& rName, int32& size) const;
  const GLint* GetInt(const nglString& rName, int32& size) const;
  bool GetMatrix(const nglString& rName, nuiMatrix& rMatrix) const;
  const GLfloat* GetFloat(GLint loc, int32& size) const;
  const GLint* GetInt(GLint loc, int32& size) const;
  bool GetMatrix(GLint loc, nuiMatrix& rMatrix) const;
private:
  std::map<GLuint, std::vector<GLfloat> > mFloats;
  std::map<GLuint, std::vector<GLint> > mInts;
  std::map<GLuint, nuiMatrix > mMatrices;
  nuiShaderProgram* mpProgram;

  friend class nuiShaderProgram;
  nuiShaderState(nuiShaderProgram* pProgram);

};




class nuiShaderProgram : public nuiRefCount
{
public:
  nuiShaderProgram();
  virtual ~nuiShaderProgram();

  void AddShader(nuiShaderKind shaderType, nglIStream& rStream);
  void AddShader(nuiShaderKind shaderType, const nglString& rSrc);

  void LoadDefaultShaders();

  bool Link();

  const nuiShaderState& GetDefaultState() const;
  void GetState(nuiShaderState& rState) const;

  // Geometry Shader: Input Type, Output and Number of Vertices out
  void       SetInputPrimitiveType(int InputPrimitiveType);   //!< Set the input primitive type for the geometry shader
  void       SetOutputPrimitiveType(int OutputPrimitiveType); //!< Set the output primitive type for the geometry shader
  void       SetVerticesOut(int VerticesOut);                 //!< Set the maximal number of vertices the geometry shader can output

  GLint       GetUniformLocation(const char *name);  //!< Retrieve Location (index) of a Uniform Variable
  GLint       GetUniformLocation(const nglString& name);  //!< Retrieve Location (index) of a Uniform Variable
  const std::map<nglString, nuiUniformDesc>& GetUniforms() const;

  
protected:
  // Receive Uniform variables:
  void       GetUniformfv(const char* varname, GLfloat* values); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
  void       GetUniformiv(const char* varname, GLint* values); //!< Receive value of uniform variable. \param varname The name of the uniform variable.


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
  void Init();
  GLuint mProgram;

  std::map<nglString, nuiUniformDesc> mUniformMap;
  std::map<GLenum, nuiShader*> mShaders;
  nuiShaderState mDefaultState;

  static std::map<GLenum, std::pair<GLenum, GLint> > gParamTypeMap;
};


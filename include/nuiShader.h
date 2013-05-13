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

#define NUI_PROJECTION_MATRIX_NAME "ProjectionMatrix"
#define NUI_MODELVIEW_MATRIX_NAME "ModelViewMatrix"
#define NUI_SURFACE_MATRIX_NAME "SurfaceMatrix"
#define NUI_OFFSET_NAME "Offset"
#define NUI_TEXTURE_TRANSLATE_NAME "TextureTranslate"
#define NUI_TEXTURE_SCALE_NAME "TextureScale"
#define NUI_DIFUSE_COLOR_NAME "DifuseColor"

class nuiShader;
class nuiShaderProgram;


class nuiShaderState : public nuiRefCount
{
public:
  nuiShaderState(nuiShaderProgram* pProgram, std::map<GLuint, int32>& rIndexMap);
  nuiShaderState(const nuiShaderState& rOriginal);
  virtual ~nuiShaderState();

  void InitWithProgram(nuiShaderProgram* pProgram, std::map<GLuint, int32>& rIndexMap);

  void Clear();

  void Set(const nglString& rName, const float* pV, int32 count, bool apply = false);
  void Set(const nglString& rName, const std::vector<float>& rV, bool apply = false);

  void Set(const nglString& rName, const int32* pV, int32 count, bool apply = false);
  void Set(const nglString& rName, const std::vector<int32>& rV, bool apply = false);

  void Set(const nglString& rName, float v1, bool apply = false);
  void Set(const nglString& rName, float v1, float v2, bool apply = false);
  void Set(const nglString& rName, float v1, float v2, float v3, bool apply = false);
  void Set(const nglString& rName, float v1, float v2, float v3, float v4, bool apply = false);
  void Set(const nglString& rName, const nglVector2f& rVec, bool apply = false);
  void Set(const nglString& rName, const nglVector3f& rVec, bool apply = false);
  void Set(const nglString& rName, const nglVectorf& rVec, bool apply = false);
  void Set(const nglString& rName, const nuiColor& rColor, bool apply = false);
  void Set(const nglString& rName, int32 v1, bool apply = false);
  void Set(const nglString& rName, int32 v1, int32 v2, bool apply = false);
  void Set(const nglString& rName, int32 v1, int32 v2, int32 v3, bool apply = false);
  void Set(const nglString& rName, int32 v1, int32 v2, int32 v3, int32 v4, bool apply = false);
  void Set(const nglString& rName, const nglMatrixf& rMat, bool apply = false);

  ///
  void Set(GLint loc, const float* pV, int32 count, bool apply = false);
  void Set(GLint loc, const std::vector<float>& rV, bool apply = false);

  void Set(GLint loc, const int32* pV, int32 count, bool apply = false);
  void Set(GLint loc, const std::vector<int32>& rV, bool apply = false);

  void Set(GLint loc, float v1, bool apply = false);
  void Set(GLint loc, float v1, float v2, bool apply = false);
  void Set(GLint loc, float v1, float v2, float v3, bool apply = false);
  void Set(GLint loc, float v1, float v2, float v3, float v4, bool apply = false);
  void Set(GLint loc, const nglVector2f& rVec, bool apply = false);
  void Set(GLint loc, const nglVector3f& rVec, bool apply = false);
  void Set(GLint loc, const nglVectorf& rVec, bool apply = false);
  void Set(GLint loc, const nuiColor& rColor, bool apply = false);
  void Set(GLint loc, int32 v1, bool apply = false);
  void Set(GLint loc, int32 v1, int32 v2, bool apply = false);
  void Set(GLint loc, int32 v1, int32 v2, int32 v3, bool apply = false);
  void Set(GLint loc, int32 v1, int32 v2, int32 v3, int32 v4, bool apply = false);
  void Set(GLint loc, const nglMatrixf& rMat, bool apply = false);

  void SetProjectionMatrix(const nglMatrixf& rMat, bool Apply);
  void SetModelViewMatrix(const nglMatrixf& rMat, bool Apply);
  void SetSurfaceMatrix(const nglMatrixf& rMat, bool Apply);
  void SetOffset(float OffsetX, float OffsetY, bool Apply);
  void SetTextureTranslate(const nglVector2f& rTranslate, bool Apply);
  void SetTextureScale(const nglVector2f& rScale, bool Apply);
  void SetDifuseColor(const nuiColor& rDifuseColor, bool Apply);

  const GLfloat* GetFloat(const nglString& rName, int32& size) const;
  const GLint* GetInt(const nglString& rName, int32& size) const;
  bool GetMatrix(const nglString& rName, nuiMatrix& rMatrix) const;
  const GLfloat* GetFloat(GLint loc, int32& size) const;
  const GLint* GetInt(GLint loc, int32& size) const;
  bool GetMatrix(GLint loc, nuiMatrix& rMatrix) const;

  bool operator == (const nuiShaderState& rState) const;
  nuiShaderState& operator= (const nuiShaderState& rState);

  void Set(const nuiShaderState& rState);

  void Dump() const;

private:
  friend class nuiShaderProgram;
  void Apply() const;

  std::vector<nuiUniformDesc> mUniforms;
  nuiShaderProgram* mpProgram;

  GLint mProjectionMatrix;
  GLint mModelViewMatrix;
  GLint mSurfaceMatrix;
  GLint mOffset;
  GLint mTextureScale;
  GLint mTextureTranslate;
  GLint mDifuseColor;
};




class nuiShaderProgram : public nuiRefCount
{
public:
  static nuiShaderProgram* GetProgram(const nglString& rName);

  nuiShaderProgram(const nglString& rName);
  virtual ~nuiShaderProgram();

  void SetPrefix(const nglString& rPrefix);
  static void SetDefaultPrefix(const nglString& rDefaultPrefix);
  void AddShaderFromPath(nuiShaderKind shaderType, const nglPath& rPath);
  void AddShader(nuiShaderKind shaderType, nglIStream& rStream);
  void AddShader(nuiShaderKind shaderType, const nglString& rSrc);

  void LoadDefaultShaders();

  bool Link();
  bool Validate() const;

  nuiShaderState* GetCurrentState() const;
  nuiShaderState* CopyCurrentState() const;
  void SetState(const nuiShaderState& rState, bool apply = false);
  void ApplyState();

  int32       GetUniformIndex(const char *name); ///< Get index in nuiShaderState 
  int32       GetUniformIndex(const nglString& name);
  GLint       GetUniformLocation(const char *name);  //!< Retrieve Location (index) of a Uniform Variable
  GLint       GetUniformLocation(const nglString& name);  //!< Retrieve Location (index) of a Uniform Variable
  GLint       GetVertexAttribLocation(const char *name);  //!< Retrieve Location (index) of a Vertex Attribute stream
  GLint       GetVertexAttribLocation(const nglString& name);  //!< Retrieve Location (index) of a Vertex Attribute stream


  GLint GetProgram() const;

  static void ClearAll();
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

  friend class nuiGL2Painter;

  GLint GetVAPositionLocation() const;
  GLint GetVATexCoordLocation() const;
  GLint GetVAColorLocation() const;
  GLint GetVANormalLocation() const;

private:
  nglString mName;
  static std::map<nglString, nuiShaderProgram*> gpPrograms;
  void Init();
  GLuint mProgram;

  std::map<nglString, nuiVertexAttribDesc> mAttribMap;
  std::map<GLuint, int32> mUniformMap;

  std::map<GLenum, nuiShader*> mShaders;
  nuiShaderState* mpCurrentState;

  GLint mVA_Position;
  GLint mVA_TexCoord;
  GLint mVA_Color;
  GLint mVA_Normal;

  nglString mPrefix;
  static nglString mDefaultPrefix;
  static std::map<GLenum, std::pair<GLenum, GLint> > gParamTypeMap;
};


//
//  nuiShader.cpp
//  nui3
//
//  Created by Sébastien Métrot on 1/10/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#include "nui.h"

#define CHECK_GL_ERRORS \
{\
GLenum err = glGetError();\
if (err != GL_NO_ERROR)\
  NGL_LOG("painter", NGL_LOG_ERROR,  "%s:%d openGL error %d", __FILE__, __LINE__, err);\
}

static const char* defaultVertexShader =
"attribute vec4 Position;\
attribute vec2 TexCoord;\
attribute vec4 Color;\
uniform mat4 gl_ModelViewMatrix;\
uniform mat4 gl_ProjectionMatrix;\
varying vec2 TexCoordVar;\
varying vec4 ColorVar;\
void main()\
{\
vec4 p = gl_ModelViewMatrix * Position;\
gl_Position = gl_ProjectionMatrix * p;\
TexCoordVar = TexCoord;\
ColorVar = Color;\
}"
;

static const char* defaultFragmentShader =
"uniform sampler2D texture;\
varying vec4 ColorVar;\
varying vec2 TexCoordVar;\
void main()\
{\
  gl_FragColor = ColorVar * texture2D( texture, TexCoordVar);\
}"
;



struct TypeDesc
{
  GLenum mEnum;
  nuiUniformType mType;
  GLint mSize;
} ShaderParamTypeDesc[] =
{
  {GL_FLOAT	,	nuiUniformFloat,	1},
  {GL_FLOAT_VEC2	,	nuiUniformFloat,	2},
  {GL_FLOAT_VEC3	,	nuiUniformFloat,	3},
  {GL_FLOAT_VEC4	,	nuiUniformFloat,	4},
  {GL_INT, nuiUniformInt, 1},
  {GL_INT_VEC2, nuiUniformInt, 2},
  {GL_INT_VEC3, nuiUniformInt, 3},
  {GL_INT_VEC4, nuiUniformInt, 4},
  {GL_UNSIGNED_INT, nuiUniformInt, 1},
  {GL_BOOL, nuiUniformInt, 1},
  {GL_BOOL_VEC2, nuiUniformInt, 2},
  {GL_BOOL_VEC3, nuiUniformInt, 3},
  {GL_BOOL_VEC4, nuiUniformInt, 4},
  {GL_FLOAT_MAT2	,	nuiUniformFMat,	4},
  {GL_FLOAT_MAT3	,	nuiUniformFMat,	9},
  {GL_FLOAT_MAT4	,	nuiUniformFMat,	16},
  {GL_SAMPLER_2D, nuiUniformInt, 1},
  {GL_SAMPLER_CUBE, nuiUniformInt, 1},
  {GL_ZERO, nuiUniformFloat, 0}
};

/////////////////////////////////////////////////////
nuiShaderState::nuiShaderState(nuiShaderProgram* pProgram)
: mpProgram(pProgram),
  mProjectionMatrix(pProgram->GetUniformLocation(NUI_PROJECTION_MATRIX_NAME)),
  mModelViewMatrix(pProgram->GetUniformLocation(NUI_MODELVIEW_ATRIX_NAME))
{

}

nuiShaderState::nuiShaderState(const nuiShaderState& rOriginal)
: mpProgram(rOriginal.mpProgram), mUniforms(rOriginal.mUniforms)
{
}

nuiShaderState::~nuiShaderState()
{
}

void nuiShaderState::Clear()
{
  mUniforms.clear();
}

void nuiShaderState::Set(const nglString& rName, const float* pV, int32 count)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, pV, count);
}

void nuiShaderState::Set(const nglString& rName, const std::vector<float>& rV)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, &rV[0], rV.size());
}

void nuiShaderState::Set(const nglString& rName, const int32* pV, int32 count)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, pV, count);
}

void nuiShaderState::Set(const nglString& rName, const std::vector<int32>& rV)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rV);
}

////
void nuiShaderState::Set(const nglString& rName, float v1)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1);
}

void nuiShaderState::Set(const nglString& rName, float v1, float v2)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2);
}

void nuiShaderState::Set(const nglString& rName, float v1, float v2, float v3)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3);
}

void nuiShaderState::Set(const nglString& rName, float v1, float v2, float v3, float v4)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3, v4);
}

void nuiShaderState::Set(const nglString& rName, const nglVector2f& rVec)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rVec);
}

void nuiShaderState::Set(const nglString& rName, const nglVector3f& rVec)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rVec);
}

void nuiShaderState::Set(const nglString& rName, const nglVectorf& rVec)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rVec);
}

void nuiShaderState::Set(const nglString& rName, const nuiColor& rColor)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rColor);
}

void nuiShaderState::Set(const nglString& rName, int32 v1)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, int32 v2)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, int32 v2, int32 v3)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, int32 v2, int32 v3, int32 v4)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3, v4);
}

void nuiShaderState::Set(const nglString& rName, const nglMatrixf& rMat)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rMat);
}


///
void nuiShaderState::Set(GLint loc, const float* pV, int32 count)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(pV, count);
}

void nuiShaderState::Set(GLint loc, const std::vector<float>& rV)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rV);
}

void nuiShaderState::Set(GLint loc, const int32* pV, int32 count)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(pV, count);
}

void nuiShaderState::Set(GLint loc, const std::vector<int32>& rV)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rV);
}


void nuiShaderState::Set(GLint loc, float v1)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1);
}

void nuiShaderState::Set(GLint loc, float v1, float v2)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2);
}

void nuiShaderState::Set(GLint loc, float v1, float v2, float v3)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3);
}

void nuiShaderState::Set(GLint loc, float v1, float v2, float v3, float v4)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3, v4);
}

void nuiShaderState::Set(GLint loc, const nglVector2f& rVec)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rVec);
}

void nuiShaderState::Set(GLint loc, const nglVector3f& rVec)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rVec);
}

void nuiShaderState::Set(GLint loc, const nglVectorf& rVec)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rVec);
}

void nuiShaderState::Set(GLint loc, const nuiColor& rColor)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rColor);
}

void nuiShaderState::Set(GLint loc, int32 v1)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1);
}

void nuiShaderState::Set(GLint loc, int32 v1, int32 v2)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2);
}

void nuiShaderState::Set(GLint loc, int32 v1, int32 v2, int32 v3)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3);
}

void nuiShaderState::Set(GLint loc, int32 v1, int32 v2, int32 v3, int32 v4)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3, v4);
}

void nuiShaderState::Set(GLint loc, const nglMatrixf& rMat)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rMat);
}

void nuiShaderState::SetProjectionMatrix(const nglMatrixf& rMat)
{
  NGL_ASSERT(mProjectionMatrix >= 0);
  Set(mProjectionMatrix, rMat);
}

void nuiShaderState::SetModelViewMatrix(const nglMatrixf& rMat)
{
  NGL_ASSERT(mModelViewMatrix >= 0);
  Set(mModelViewMatrix, rMat);
}


void nuiShaderState::Apply() const
{
  std::map<GLuint, nuiUniformDesc>::const_iterator it = mUniforms.begin();
  std::map<GLuint, nuiUniformDesc>::const_iterator end = mUniforms.end();

  while (it != end)
  {
    it->second.Apply();
    ++it;
  }
}



/////////////////////////////////////////////////////
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
nuiShaderProgram::nuiShaderProgram()
: mProgram(0), mDefaultState(this)
{
  Init();
}

std::map<GLenum, std::pair<GLenum, GLint> > nuiShaderProgram::gParamTypeMap;

void nuiShaderProgram::Init()
{
  if (!gParamTypeMap.empty())
    return;

  for (int i = 0; ShaderParamTypeDesc[i].mEnum != GL_ZERO; i++)
  {
    gParamTypeMap[ShaderParamTypeDesc[i].mEnum] = std::make_pair(ShaderParamTypeDesc[i].mType, ShaderParamTypeDesc[i].mSize);
  }
}

nuiShaderProgram::~nuiShaderProgram()
{
  std::map<GLenum, nuiShader*>::iterator it = mShaders.begin();
  std::map<GLenum, nuiShader*>::iterator end = mShaders.end();
  while (it != end)
  {
    delete it->second;
    ++it;
  }

  if (mProgram)
    glDeleteProgram(mProgram);
}

void nuiShaderProgram::AddShader(nuiShaderKind shaderType, nglIStream& rStream)
{
  nuiShader* pShader = new nuiShader(shaderType, rStream);
  if (mShaders[shaderType] != NULL)
    delete mShaders[shaderType];
  mShaders[shaderType] = pShader;
}

void nuiShaderProgram::AddShader(nuiShaderKind shaderType, const nglString& rSrc)
{
  nuiShader* pShader = new nuiShader(shaderType, rSrc);
  mShaders[shaderType] = pShader;
}

void nuiShaderProgram::LoadDefaultShaders()
{
  AddShader(eVertexShader, defaultVertexShader);
  AddShader(eFragmentShader, defaultFragmentShader);
}


const nuiShaderState& nuiShaderProgram::GetDefaultState() const
{
  return mDefaultState;
}


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

bool nuiShaderProgram::Link()
{
  mProgram = glCreateProgram();

  nuiShaderKind kinds[] =
  {
    eVertexShader,
    eFragmentShader
  };
  for (int i = 0; i < 2; i++)
  {
    nuiShaderKind k = kinds[i];
    std::map<GLenum, nuiShader*>::iterator it = mShaders.find(k);
    if (it != mShaders.end())
    {
      nuiShader* pShader = it->second;
      if (!pShader->Load())
      {
        CHECK_GL_ERRORS;
        NGL_LOG("painter", NGL_LOG_ERROR, "nuiShaderProgram::Link() Unable to load shader: %s", pShader->GetError().GetChars());
        return false;
      }

      glAttachShader(mProgram, pShader->GetShader());
      CHECK_GL_ERRORS;
    }
  }


  glLinkProgram(mProgram);
  CHECK_GL_ERRORS;

  // 3
  GLint linkSuccess;
  glGetProgramiv(mProgram, GL_LINK_STATUS, &linkSuccess);
  CHECK_GL_ERRORS;
  if (linkSuccess == GL_FALSE)
  {
    GLchar messages[256];
    glGetProgramInfoLog(mProgram, sizeof(messages), 0, &messages[0]);
    CHECK_GL_ERRORS;
    NGL_LOG("painter", NGL_LOG_ERROR, "nuiShaderProgram::Link() %s", messages);
    return false;
  }

  glUseProgram(mProgram);
  CHECK_GL_ERRORS;


  int total = -1;
  glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &total);
  for (int i = 0; i < total; ++i)
  {
    int name_len = -1;
    int num = -1;
    GLenum type = GL_ZERO;
    char name[100];
    glGetActiveUniform(mProgram, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name);
    name[name_len] = 0;
    GLuint location = glGetUniformLocation(mProgram, name);

    mUniformMap[name] = nuiUniformDesc(name, type, num, location);
  }

  GetState(mDefaultState);

  return true;
}

void nuiShaderProgram::GetState(nuiShaderState& rState) const
{
  std::map<nglString, nuiUniformDesc>::const_iterator it = mUniformMap.begin();
  std::map<nglString, nuiUniformDesc>::const_iterator end = mUniformMap.end();

  while (it != end)
  {
    const nuiUniformDesc& desc(it->second);

    std::map<GLenum, std::pair<GLenum, GLint> >::const_iterator itt = gParamTypeMap.find(desc.mType);
    std::vector<GLfloat> fval;
    std::vector<GLint> ival;
    if (itt != gParamTypeMap.end())
    {
      const std::pair<GLenum, GLint>& d(itt->second);
      if (d.first == GL_FLOAT)
      {
        int size = d.second;
        fval.resize(size);
        glGetUniformfv(mProgram, desc.mLocation, &fval[0]);
        rState.Set(desc.mName, &fval[0], size);
      }
      else if (d.first == GL_INT)
      {
        int size = d.second;
        ival.resize(size);
        glGetUniformiv(mProgram, desc.mLocation, &ival[0]);
        rState.Set(desc.mName, &ival[0], size);
      }
    }
    ++it;
  }
}




const std::map<nglString, nuiUniformDesc>& nuiShaderProgram::GetUniforms() const
{
  return mUniformMap;
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



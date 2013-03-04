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
uniform mat4 ModelView;\
uniform mat4 Projection;\
varying vec2 TexCoordVar;\
varying vec4 ColorVar;\
void main()\
{\
vec4 p = ModelView * Position;\
gl_Position = Projection * p;\
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
  {GL_DOUBLE	,	nuiUniformDouble,	1},
  {GL_DOUBLE_VEC2	,	nuiUniformDouble,	2},
  {GL_DOUBLE_VEC3	,	nuiUniformDouble,	3},
  {GL_DOUBLE_VEC4	,	nuiUniformDouble,	4},
  {GL_INT, nuiUniformInt, 1},
  {GL_INT_VEC2, nuiUniformInt, 2},
  {GL_INT_VEC3, nuiUniformInt, 3},
  {GL_INT_VEC4, nuiUniformInt, 4},
  {GL_UNSIGNED_INT, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_VEC2, nuiUniformInt, 2},
  {GL_UNSIGNED_INT_VEC3, nuiUniformInt, 3},
  {GL_UNSIGNED_INT_VEC4, nuiUniformInt, 4},
  {GL_BOOL, nuiUniformInt, 1},
  {GL_BOOL_VEC2, nuiUniformInt, 2},
  {GL_BOOL_VEC3, nuiUniformInt, 3},
  {GL_BOOL_VEC4, nuiUniformInt, 4},
  {GL_FLOAT_MAT2	,	nuiUniformFMat,	4},
  {GL_FLOAT_MAT3	,	nuiUniformFMat,	9},
  {GL_FLOAT_MAT4	,	nuiUniformFMat,	16},
  {GL_FLOAT_MAT2x3	,	nuiUniformFMat,	6},
  {GL_FLOAT_MAT2x4	,	nuiUniformFMat,	8},
  {GL_FLOAT_MAT3x2	,	nuiUniformFMat,	6},
  {GL_FLOAT_MAT3x4	,	nuiUniformFMat,	12},
  {GL_FLOAT_MAT4x2	,	nuiUniformFMat,	8},
  {GL_FLOAT_MAT4x3	,	nuiUniformFMat,	12},
  {GL_DOUBLE_MAT2	,	nuiUniformDMat,	4},
  {GL_DOUBLE_MAT3	,	nuiUniformDMat,	9},
  {GL_DOUBLE_MAT4	,	nuiUniformDMat,	16},
  {GL_DOUBLE_MAT2x3	,	nuiUniformDMat,	6},
  {GL_DOUBLE_MAT2x4	,	nuiUniformDMat,	8},
  {GL_DOUBLE_MAT3x2	,	nuiUniformDMat,	6},
  {GL_DOUBLE_MAT3x4	,	nuiUniformDMat,	12},
  {GL_DOUBLE_MAT4x2	,	nuiUniformDMat,	8},
  {GL_DOUBLE_MAT4x3	,	nuiUniformDMat,	12},
  {GL_SAMPLER_1D, nuiUniformInt, 1},
  {GL_SAMPLER_2D, nuiUniformInt, 1},
  {GL_SAMPLER_3D, nuiUniformInt, 1},
  {GL_SAMPLER_CUBE, nuiUniformInt, 1},
  {GL_SAMPLER_1D_SHADOW, nuiUniformInt, 1},
  {GL_SAMPLER_2D_SHADOW, nuiUniformInt, 1},
  {GL_SAMPLER_1D_ARRAY, nuiUniformInt, 1},
  {GL_SAMPLER_2D_ARRAY, nuiUniformInt, 1},
  {GL_SAMPLER_1D_ARRAY_SHADOW, nuiUniformInt, 1},
  {GL_SAMPLER_2D_ARRAY_SHADOW, nuiUniformInt, 1},
  {GL_SAMPLER_2D_MULTISAMPLE, nuiUniformInt, 1},
  {GL_SAMPLER_2D_MULTISAMPLE_ARRAY, nuiUniformInt, 1},
  {GL_SAMPLER_CUBE_SHADOW, nuiUniformInt, 1},
  {GL_SAMPLER_BUFFER, nuiUniformInt, 1},
  {GL_SAMPLER_2D_RECT, nuiUniformInt, 1},
  {GL_SAMPLER_2D_RECT_SHADOW, nuiUniformInt, 1},
  {GL_INT_SAMPLER_1D, nuiUniformInt, 1},
  {GL_INT_SAMPLER_2D, nuiUniformInt, 1},
  {GL_INT_SAMPLER_3D, nuiUniformInt, 1},
  {GL_INT_SAMPLER_CUBE, nuiUniformInt, 1},
  {GL_INT_SAMPLER_1D_ARRAY, nuiUniformInt, 1},
  {GL_INT_SAMPLER_2D_ARRAY, nuiUniformInt, 1},
  {GL_INT_SAMPLER_2D_MULTISAMPLE, nuiUniformInt, 1},
  {GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, nuiUniformInt, 1},
  {GL_INT_SAMPLER_BUFFER, nuiUniformInt, 1},
  {GL_INT_SAMPLER_2D_RECT, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_1D, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_3D, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_CUBE, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_BUFFER, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_RECT, nuiUniformInt, 1},
  /*
  {GL_IMAGE_1D, nuiUniformInt, 1},
  {GL_IMAGE_2D, nuiUniformInt, 1},
  {GL_IMAGE_3D, nuiUniformInt, 1},
  {GL_IMAGE_2D_RECT, nuiUniformInt, 1},
  {GL_IMAGE_CUBE, nuiUniformInt, 1},
  {GL_IMAGE_BUFFER, nuiUniformInt, 1},
  {GL_IMAGE_1D_ARRAY, nuiUniformInt, 1},
  {GL_IMAGE_2D_ARRAY, nuiUniformInt, 1},
  {GL_IMAGE_2D_MULTISAMPLE, nuiUniformInt, 1},
  {GL_IMAGE_2D_MULTISAMPLE_ARRAY, nuiUniformInt, 1},
  {GL_INT_IMAGE_1D, nuiUniformInt, 1},
  {GL_INT_IMAGE_2D, nuiUniformInt, 1},
  {GL_INT_IMAGE_3D, nuiUniformInt, 1},
  {GL_INT_IMAGE_2D_RECT, nuiUniformInt, 1},
  {GL_INT_IMAGE_CUBE, nuiUniformInt, 1},
  {GL_INT_IMAGE_BUFFER, nuiUniformInt, 1},
  {GL_INT_IMAGE_1D_ARRAY, nuiUniformInt, 1},
  {GL_INT_IMAGE_2D_ARRAY, nuiUniformInt, 1},
  {GL_INT_IMAGE_2D_MULTISAMPLE, nuiUniformInt, 1},
  {GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_1D, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_2D, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_3D, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_RECT, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_CUBE, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_BUFFER, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_1D_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY, nuiUniformInt, 1},
  {GL_UNSIGNED_INT_ATOMIC_COUNTER, nuiUniformInt, 1}
   */
  {GL_ZERO, nuiUniformFloat, 0}
};

/////////////////////////////////////////////////////
nuiShaderState::nuiShaderState(nuiShaderProgram* pProgram)
: mpProgram(pProgram)
{
}

nuiShaderState::nuiShaderState(const nuiShaderState& rOriginal)
: mpProgram(rOriginal.mpProgram), mInts(rOriginal.mInts), mFloats(rOriginal.mFloats)
{
}

nuiShaderState::~nuiShaderState()
{
}

void nuiShaderState::Clear()
{
  mFloats.clear();
  mInts.clear();
}

void nuiShaderState::Set(const nglString& rName, const GLfloat* pVal, int32 count)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, pVal, count);
}

void nuiShaderState::Set(const nglString& rName, const GLint* pVal, int32 count)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, pVal, count);
}

void nuiShaderState::Set(const nglString& rName, const nuiMatrix& rMatrix)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rMatrix);
}

void nuiShaderState::Set(GLint loc, const GLfloat* pVal, int32 size)
{
  std::vector<GLfloat>& rF(mFloats[loc]);
  rF.resize(size);
  for (int32 i = 0; i < size; i++)
    rF[i] = pVal[i];
}

void nuiShaderState::Set(GLint loc, const GLint* pVal, int32 size)
{
  std::vector<GLint>& rI(mInts[loc]);
  rI.resize(size);
  for (int32 i = 0; i < size; i++)
    rI[i] = pVal[i];
}

void nuiShaderState::Set(GLint loc, const nuiMatrix& rMatrix)
{
  mMatrices[loc] = rMatrix;
}

//=============
const GLfloat* nuiShaderState::GetFloat(const nglString& rName, int32& size) const
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  return GetFloat(loc, size);
}

const GLint* nuiShaderState::GetInt(const nglString& rName, int32& size) const
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  return GetInt(loc, size);
}

bool nuiShaderState::GetMatrix(const nglString& rName, nuiMatrix& rMatrix) const
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  return GetMatrix(loc, rMatrix);
}

//-----------------
const GLfloat* nuiShaderState::GetFloat(GLint loc, int32& size) const
{
  std::map<GLuint, std::vector<GLfloat> >::const_iterator it = mFloats.find(loc);
  if (it == mFloats.end())
  {
    size = 0;
    return NULL;
  }

  return &(it->second[0]);
}

const GLint* nuiShaderState::GetInt(GLint loc, int32& size) const
{
  std::map<GLuint, std::vector<GLint> >::const_iterator it = mInts.find(loc);
  if (it == mInts.end())
  {
    size = 0;
    return NULL;
  }

  return &(it->second[0]);
}

bool nuiShaderState::GetMatrix(GLint loc, nuiMatrix& rMatrix) const
{
  std::map<GLuint, nuiMatrix >::const_iterator it = mMatrices.find(loc);
  if (it == mMatrices.end())
  {
    return false;
  }

  rMatrix = it->second;
  return true;
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

    mUniformMap[name] = nuiUniformDesc(name, type, location);
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



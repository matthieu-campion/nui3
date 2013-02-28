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
  GLenum mType;
  GLint mSize;
} ShaderParamTypeDesc[] =
{
  {GL_FLOAT	,	GL_FLOAT,	1},
  {GL_FLOAT_VEC2	,	GL_FLOAT,	2},
  {GL_FLOAT_VEC3	,	GL_FLOAT,	3},
  {GL_FLOAT_VEC4	,	GL_FLOAT,	4},
  {GL_DOUBLE	,	GL_DOUBLE,	1},
  {GL_DOUBLE_VEC2	,	GL_DOUBLE,	2},
  {GL_DOUBLE_VEC3	,	GL_DOUBLE,	3},
  {GL_DOUBLE_VEC4	,	GL_DOUBLE,	4},
  {GL_INT, GL_INT, 1},
  {GL_INT_VEC2, GL_INT, 2},
  {GL_INT_VEC3, GL_INT, 3},
  {GL_INT_VEC4, GL_INT, 4},
  {GL_UNSIGNED_INT, GL_INT, 1},
  {GL_UNSIGNED_INT_VEC2, GL_INT, 2},
  {GL_UNSIGNED_INT_VEC3, GL_INT, 3},
  {GL_UNSIGNED_INT_VEC4, GL_INT, 4},
  {GL_BOOL, GL_INT, 1},
  {GL_BOOL_VEC2, GL_INT, 2},
  {GL_BOOL_VEC3, GL_INT, 3},
  {GL_BOOL_VEC4, GL_INT, 4},
  {GL_FLOAT_MAT2	,	GL_FLOAT,	4},
  {GL_FLOAT_MAT3	,	GL_FLOAT,	9},
  {GL_FLOAT_MAT4	,	GL_FLOAT,	16},
  {GL_FLOAT_MAT2x3	,	GL_FLOAT,	6},
  {GL_FLOAT_MAT2x4	,	GL_FLOAT,	8},
  {GL_FLOAT_MAT3x2	,	GL_FLOAT,	6},
  {GL_FLOAT_MAT3x4	,	GL_FLOAT,	12},
  {GL_FLOAT_MAT4x2	,	GL_FLOAT,	8},
  {GL_FLOAT_MAT4x3	,	GL_FLOAT,	12},
  {GL_DOUBLE_MAT2	,	GL_DOUBLE,	4},
  {GL_DOUBLE_MAT3	,	GL_DOUBLE,	9},
  {GL_DOUBLE_MAT4	,	GL_DOUBLE,	16},
  {GL_DOUBLE_MAT2x3	,	GL_DOUBLE,	6},
  {GL_DOUBLE_MAT2x4	,	GL_DOUBLE,	8},
  {GL_DOUBLE_MAT3x2	,	GL_DOUBLE,	6},
  {GL_DOUBLE_MAT3x4	,	GL_DOUBLE,	12},
  {GL_DOUBLE_MAT4x2	,	GL_DOUBLE,	8},
  {GL_DOUBLE_MAT4x3	,	GL_DOUBLE,	12},
  {GL_SAMPLER_1D, GL_INT, 1},
  {GL_SAMPLER_2D, GL_INT, 1},
  {GL_SAMPLER_3D, GL_INT, 1},
  {GL_SAMPLER_CUBE, GL_INT, 1},
  {GL_SAMPLER_1D_SHADOW, GL_INT, 1},
  {GL_SAMPLER_2D_SHADOW, GL_INT, 1},
  {GL_SAMPLER_1D_ARRAY, GL_INT, 1},
  {GL_SAMPLER_2D_ARRAY, GL_INT, 1},
  {GL_SAMPLER_1D_ARRAY_SHADOW, GL_INT, 1},
  {GL_SAMPLER_2D_ARRAY_SHADOW, GL_INT, 1},
  {GL_SAMPLER_2D_MULTISAMPLE, GL_INT, 1},
  {GL_SAMPLER_2D_MULTISAMPLE_ARRAY, GL_INT, 1},
  {GL_SAMPLER_CUBE_SHADOW, GL_INT, 1},
  {GL_SAMPLER_BUFFER, GL_INT, 1},
  {GL_SAMPLER_2D_RECT, GL_INT, 1},
  {GL_SAMPLER_2D_RECT_SHADOW, GL_INT, 1},
  {GL_INT_SAMPLER_1D, GL_INT, 1},
  {GL_INT_SAMPLER_2D, GL_INT, 1},
  {GL_INT_SAMPLER_3D, GL_INT, 1},
  {GL_INT_SAMPLER_CUBE, GL_INT, 1},
  {GL_INT_SAMPLER_1D_ARRAY, GL_INT, 1},
  {GL_INT_SAMPLER_2D_ARRAY, GL_INT, 1},
  {GL_INT_SAMPLER_2D_MULTISAMPLE, GL_INT, 1},
  {GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, GL_INT, 1},
  {GL_INT_SAMPLER_BUFFER, GL_INT, 1},
  {GL_INT_SAMPLER_2D_RECT, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_1D, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_3D, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_CUBE, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_BUFFER, GL_INT, 1},
  {GL_UNSIGNED_INT_SAMPLER_2D_RECT, GL_INT, 1},
  /*
  {GL_IMAGE_1D, GL_INT, 1},
  {GL_IMAGE_2D, GL_INT, 1},
  {GL_IMAGE_3D, GL_INT, 1},
  {GL_IMAGE_2D_RECT, GL_INT, 1},
  {GL_IMAGE_CUBE, GL_INT, 1},
  {GL_IMAGE_BUFFER, GL_INT, 1},
  {GL_IMAGE_1D_ARRAY, GL_INT, 1},
  {GL_IMAGE_2D_ARRAY, GL_INT, 1},
  {GL_IMAGE_2D_MULTISAMPLE, GL_INT, 1},
  {GL_IMAGE_2D_MULTISAMPLE_ARRAY, GL_INT, 1},
  {GL_INT_IMAGE_1D, GL_INT, 1},
  {GL_INT_IMAGE_2D, GL_INT, 1},
  {GL_INT_IMAGE_3D, GL_INT, 1},
  {GL_INT_IMAGE_2D_RECT, GL_INT, 1},
  {GL_INT_IMAGE_CUBE, GL_INT, 1},
  {GL_INT_IMAGE_BUFFER, GL_INT, 1},
  {GL_INT_IMAGE_1D_ARRAY, GL_INT, 1},
  {GL_INT_IMAGE_2D_ARRAY, GL_INT, 1},
  {GL_INT_IMAGE_2D_MULTISAMPLE, GL_INT, 1},
  {GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_1D, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_2D, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_3D, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_RECT, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_CUBE, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_BUFFER, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_1D_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE, GL_INT, 1},
  {GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY, GL_INT, 1},
  {GL_UNSIGNED_INT_ATOMIC_COUNTER, GL_INT, 1}
   */
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
  std::vector<GLfloat>& rF(mFloats[loc]);
  rF.resize(count);
  for (int32 i = 0; i < count; i++)
    rF[i] = pVal[i];
}

void nuiShaderState::Set(const nglString& rName, const GLint* pVal, int32 count)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  std::vector<GLint>& rI(mInts[loc]);
  rI.resize(count);
  for (int32 i = 0; i < count; i++)
    rI[i] = pVal[i];
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




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
"attribute vec4 Position;\n\
attribute vec2 TexCoord;\n\
attribute vec4 Color;\n\
uniform vec4 DiffuseColor;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
varying vec2 TexCoordVar;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
TexCoordVar = TexCoord;\n\
ColorVar = DiffuseColor * Color;\n\
}"
;

static const char* defaultFragmentShader =
"uniform sampler2D texture;\n\
varying vec4 ColorVar;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
  vec4 _gl_FragColor = ColorVar * texture2D(texture, TexCoordVar);\n\
  gl_FragColor = ColorVar * texture2D(texture, TexCoordVar);\n\
  //float v = texture2D(texture, TexCoordVar)[3];\
  //gl_FragColor = ColorVar * vec4(v, v, v, v);\n\
  //gl_FragColor = ColorVar;\n\
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
  {GL_INT, GL_INT, 1},
  {GL_INT_VEC2, GL_INT, 2},
  {GL_INT_VEC3, GL_INT, 3},
  {GL_INT_VEC4, GL_INT, 4},
  {GL_UNSIGNED_INT, GL_INT, 1},
  {GL_BOOL, GL_INT, 1},
  {GL_BOOL_VEC2, GL_INT, 2},
  {GL_BOOL_VEC3, GL_INT, 3},
  {GL_BOOL_VEC4, GL_INT, 4},
  {GL_FLOAT_MAT2,	GL_FLOAT,	4},
  {GL_FLOAT_MAT3,	GL_FLOAT,	9},
  {GL_FLOAT_MAT4,	GL_FLOAT,	16},
  {GL_SAMPLER_2D, GL_INT, 1},
  {GL_SAMPLER_CUBE, GL_INT, 1},
  {GL_ZERO, GL_ZERO, 0}
};

/////////////////////////////////////////////////////
nuiShaderState::nuiShaderState(nuiShaderProgram* pProgram)
: mpProgram(NULL),
  mProjectionMatrix(-1),
  mModelViewMatrix(-1)
{
  if (pProgram)
    InitWithProgram(pProgram);
}

nuiShaderState::nuiShaderState(const nuiShaderState& rOriginal)
: mpProgram(rOriginal.mpProgram), mUniforms(rOriginal.mUniforms), mProjectionMatrix(rOriginal.mProjectionMatrix), mModelViewMatrix(rOriginal.mModelViewMatrix)
{
}

nuiShaderState::~nuiShaderState()
{
}

void nuiShaderState::Clear()
{
  mUniforms.clear();
}

void nuiShaderState::Set(const nglString& rName, const float* pV, int32 count, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, pV, count, Apply);
}

void nuiShaderState::Set(const nglString& rName, const std::vector<float>& rV, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, &rV[0], rV.size(), Apply);
}

void nuiShaderState::Set(const nglString& rName, const int32* pV, int32 count, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, pV, count, Apply);
}

void nuiShaderState::Set(const nglString& rName, const std::vector<int32>& rV, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rV, Apply);
}

////
void nuiShaderState::Set(const nglString& rName, float v1, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, Apply);
}

void nuiShaderState::Set(const nglString& rName, float v1, float v2, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, Apply);
}

void nuiShaderState::Set(const nglString& rName, float v1, float v2, float v3, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3, Apply);
}

void nuiShaderState::Set(const nglString& rName, float v1, float v2, float v3, float v4, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3, v4, Apply);
}

void nuiShaderState::Set(const nglString& rName, const nglVector2f& rVec, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rVec, Apply);
}

void nuiShaderState::Set(const nglString& rName, const nglVector3f& rVec, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rVec, Apply);
}

void nuiShaderState::Set(const nglString& rName, const nglVectorf& rVec, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rVec, Apply);
}

void nuiShaderState::Set(const nglString& rName, const nuiColor& rColor, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rColor, Apply);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, Apply);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, int32 v2, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, Apply);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, int32 v2, int32 v3, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3, Apply);
}

void nuiShaderState::Set(const nglString& rName, int32 v1, int32 v2, int32 v3, int32 v4, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, v1, v2, v3, v4, Apply);
}

void nuiShaderState::Set(const nglString& rName, const nglMatrixf& rMat, bool Apply)
{
  GLint loc = mpProgram->GetUniformLocation(rName);
  Set(loc, rMat, Apply);
}

static const char* Type2String(GLenum type)
{
  switch (type)
  {
    case GL_FLOAT:        return "float"; break;
    case GL_FLOAT_VEC2:   return "vec2"; break;
    case GL_FLOAT_VEC3:   return "vec3"; break;
    case GL_FLOAT_VEC4:   return "vec4"; break;

    case GL_INT:          return "int"; break;
    case GL_INT_VEC2:     return "ivec2"; break;
    case GL_INT_VEC3:     return "ivec3"; break;
    case GL_INT_VEC4:     return "ivec4"; break;
    case GL_UNSIGNED_INT: return "uint"; break;
    case GL_SAMPLER_2D:   return "sampler2d"; break;
    case GL_SAMPLER_CUBE: return "samplerCube"; break;

    case GL_FLOAT_MAT2:   return "mat2"; break;
    case GL_FLOAT_MAT3:   return "mat3"; break;
    case GL_FLOAT_MAT4:   return "mat4"; break;

    default:
      NGL_ASSERT(0);
  }

  return "???";
}

void nuiShaderState::InitWithProgram(nuiShaderProgram* pProgram)
{
  Clear();
  mpProgram = pProgram;
  mProjectionMatrix = mpProgram->GetUniformLocation(NUI_PROJECTION_MATRIX_NAME);
  mModelViewMatrix = mpProgram->GetUniformLocation(NUI_MODELVIEW_MATRIX_NAME);

  GLuint pgm = mpProgram->GetProgram();
  {
    int total = -1;
    glGetProgramiv(pgm, GL_ACTIVE_UNIFORMS, &total);
    for (int i = 0; i < total; ++i)
    {
      int name_len = -1;
      int num = -1;
      GLenum type = GL_ZERO;
      char name[100];
      glGetActiveUniform(pgm, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name);
      name[name_len] = 0;
      GLuint location = glGetUniformLocation(pgm, name);

      NGL_OUT("ShaderProgram %p Uniform:  %s %s[%d]\n", pProgram, Type2String(type), name , num);
      mUniforms.insert(std::make_pair(location, nuiUniformDesc(name, type, num, location, pProgram)));
    }
  }
}

///
void nuiShaderState::Set(GLint loc, const float* pV, int32 count, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(pV, count, Apply);
}

void nuiShaderState::Set(GLint loc, const std::vector<float>& rV, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rV, Apply);
}

void nuiShaderState::Set(GLint loc, const int32* pV, int32 count, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(pV, count, Apply);
}

void nuiShaderState::Set(GLint loc, const std::vector<int32>& rV, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rV, Apply);
}


void nuiShaderState::Set(GLint loc, float v1, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, Apply);
}

void nuiShaderState::Set(GLint loc, float v1, float v2, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, Apply);
}

void nuiShaderState::Set(GLint loc, float v1, float v2, float v3, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3, Apply);
}

void nuiShaderState::Set(GLint loc, float v1, float v2, float v3, float v4, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3, v4, Apply);
}

void nuiShaderState::Set(GLint loc, const nglVector2f& rVec, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rVec, Apply);
}

void nuiShaderState::Set(GLint loc, const nglVector3f& rVec, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rVec, Apply);
}

void nuiShaderState::Set(GLint loc, const nglVectorf& rVec, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rVec, Apply);
}

void nuiShaderState::Set(GLint loc, const nuiColor& rColor, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rColor, Apply);
}

void nuiShaderState::Set(GLint loc, int32 v1, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, Apply);
}

void nuiShaderState::Set(GLint loc, int32 v1, int32 v2, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, Apply);
}

void nuiShaderState::Set(GLint loc, int32 v1, int32 v2, int32 v3, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3, Apply);
}

void nuiShaderState::Set(GLint loc, int32 v1, int32 v2, int32 v3, int32 v4, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(v1, v2, v3, v4, Apply);
}

void nuiShaderState::Set(GLint loc, const nglMatrixf& rMat, bool Apply)
{
  NGL_ASSERT(mUniforms.find(loc) != mUniforms.end());
  mUniforms[loc].Set(rMat, Apply);
}

void nuiShaderState::SetProjectionMatrix(const nglMatrixf& rMat, bool Apply)
{
  NGL_ASSERT(mProjectionMatrix >= 0);
  Set(mProjectionMatrix, rMat);
  if (Apply)
    mUniforms.find(mProjectionMatrix)->second.Apply();
}

void nuiShaderState::SetModelViewMatrix(const nglMatrixf& rMat, bool Apply)
{
  NGL_ASSERT(mModelViewMatrix >= 0);
  Set(mModelViewMatrix, rMat);
  if (Apply)
    mUniforms.find(mModelViewMatrix)->second.Apply();
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


bool nuiShaderState::operator == (const nuiShaderState& rState) const
{
  if (mpProgram != rState.mpProgram)
    return false;

  if (mProjectionMatrix != rState.mProjectionMatrix)
    return false;

  if (mModelViewMatrix != rState.mModelViewMatrix)
    return false;

  return mUniforms == rState.mUniforms;
}

nuiShaderState& nuiShaderState::operator= (const nuiShaderState& rState)
{
  mpProgram = rState.mpProgram;
  mProjectionMatrix = rState.mProjectionMatrix;
  mModelViewMatrix = rState.mModelViewMatrix;
  mUniforms = rState.mUniforms;
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

nuiShaderState& nuiShaderProgram::GetDefaultState()
{
  return mDefaultState;
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


  // Enumerate Uniforms:
  mDefaultState.InitWithProgram(this);

  // Enumerate Vertex Attributes:
  {
    int total = -1;
    glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &total);
    for (int i = 0; i < total; ++i)
    {
      int name_len = -1;
      int num = -1;
      GLenum type = GL_ZERO;
      char name[100];
      glGetActiveAttrib(mProgram, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name);
      name[name_len] = 0;
      GLuint location = glGetAttribLocation(mProgram, name);

      mAttribMap[name] = nuiVertexAttribDesc(name, type, num, location);
    }
  }

  mVA_Position = glGetAttribLocation(mProgram, "Position");
  mVA_TexCoord = glGetAttribLocation(mProgram, "TexCoord");
  mVA_Color = glGetAttribLocation(mProgram, "Color");


  return true;
}

GLint nuiShaderProgram::GetProgram() const
{
  return mProgram;
}

void nuiShaderProgram::GetState(nuiShaderState& rState) const
{
  rState = mDefaultState;
}


void nuiShaderProgram::SetVertexPointers(const nuiRenderArray& rArray)
{
  if (mVA_Position != -1)
  {
    glEnableVertexAttribArray(mVA_Position);
    glVertexAttribPointer(mVA_Position, 3, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mX);
  }

  if (mVA_TexCoord != -1)
  {
    glEnableVertexAttribArray(mVA_TexCoord);
    glVertexAttribPointer(mVA_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mTX);
  }

  if (mVA_Color != -1)
  {
    glEnableVertexAttribArray(mVA_Color);
    glVertexAttribPointer(mVA_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mR);
  }
}

void nuiShaderProgram::ResetVertexPointers(const nuiRenderArray& rArray)
{
  if (mVA_Position != -1)
  {
    glDisableVertexAttribArray(mVA_Position);
  }

  if (mVA_TexCoord != -1)
  {
    glDisableVertexAttribArray(mVA_TexCoord);
  }

  if (mVA_Color != -1)
  {
    glDisableVertexAttribArray(mVA_Color);
  }
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



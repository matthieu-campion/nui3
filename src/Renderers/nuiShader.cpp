/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiShader.h"
#include "nglContext.h"

const char* gpNUIUniformTextures = 
"//-NUI Shader Declarations------\n"\
"//-1D Textures------------------\n"\
"uniform sampler1D nuiTexture1D0;\n"\
"uniform sampler1D nuiTexture1D1;\n"\
"uniform sampler1D nuiTexture1D2;\n"\
"uniform sampler1D nuiTexture1D3;\n"\
"uniform sampler1D nuiTexture1D4;\n"\
"uniform sampler1D nuiTexture1D5;\n"\
"uniform sampler1D nuiTexture1D6;\n"\
"uniform sampler1D nuiTexture1D7;\n"\
"//-2D Textures------------------\n"\
"uniform sampler2D nuiTexture2D0;\n"\
"uniform sampler2D nuiTexture2D1;\n"\
"uniform sampler2D nuiTexture2D2;\n"\
"uniform sampler2D nuiTexture2D3;\n"\
"uniform sampler2D nuiTexture2D4;\n"\
"uniform sampler2D nuiTexture2D5;\n"\
"uniform sampler2D nuiTexture2D6;\n"\
"uniform sampler2D nuiTexture2D7;\n"\
"//-NUI Shader Declarations End--\n\n\n";


nuiShader::nuiShader(nglContext* pContext, const nglString& rName, const nglString& rVertexShader, const nglString& rFragmentShader)
: mpContext(NULL),
  mpVertexShader(NULL),
  mpFragmentShader(NULL)
{
#ifdef GL_ARB_shader_objects
  mpContext = pContext;
  mpVertexShader = NULL;
  mpFragmentShader = NULL;

  mProgram = 0;
  mVS = 0;
  mFS = 0;

  mVertexShader = rVertexShader;
  mFragmentShader = rFragmentShader;
  mErrorMessage = nglString::Empty;

  if (!mpContext->CheckExtension(_T("GL_ARB_shader_objects")))
  {
    mErrorMessage = _T("GL_ARB_shader_objects extension not present. Shaders disabled.");
    mpContext = NULL;
    return;
  }
  else if (!mpContext->CheckExtension(_T("GL_ARB_vertex_shader")))
  {
    mErrorMessage = _T("GL_ARB_vertex_shader extension not present. Shaders disabled.");
    mpContext = NULL;
    return;
  }
  else if (!mpContext->CheckExtension(_T("GL_ARB_fragment_shader")))
  {
    mErrorMessage = _T("GL_ARB_fragment_shader extension not present. Shaders disabled.");
    mpContext = NULL;
    return;
  }
  else if (!mpContext->CheckExtension(_T("GL_ARB_shading_language_100")))
  {
    mErrorMessage = _T("GL_ARB_shading_language_100 extension not present. Shaders disabled.");
    mpContext = NULL;
    return;
  }
  
  mProgram = mpContext->glCreateProgramObjectARB();
  mVS = mpContext->glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  mFS = mpContext->glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

  // Load source code strings into shaders
  if (!mVertexShader.IsEmpty())
  {
    //mVertexShader.Insert(gpNUIUniformTextures, 0);
    mpVertexShader = mVertexShader.Export();
    mpContext->glShaderSourceARB(mVS, 1, &mpVertexShader, NULL);
  }

  if (!mFragmentShader.IsEmpty())
  {
    mFragmentShader.Insert(nglString(gpNUIUniformTextures), 0);
    mpFragmentShader = mFragmentShader.Export();
    mpContext->glShaderSourceARB(mFS, 1, &mpFragmentShader, NULL);
  }
#else
  mErrorMessage = _T("GL_ARB_shader_objects extension not present at compile time. Shaders disabled.";
#endif
}

nuiShader::~nuiShader()
{
#ifdef GL_ARB_shader_objects
  if (mpContext && mProgram)
  {
    mpContext->glDeleteObjectARB(mProgram);
  }
#endif
  delete mpVertexShader;
}

bool nuiShader::Compile()
{
  if (!mpContext)
    return false;
#ifdef GL_ARB_shader_objects
  if (mpVertexShader)
  {
    mpContext->glCompileShaderARB(mVS);
    GLint res;
    mpContext->glGetObjectParameterivARB(mVS, GL_OBJECT_COMPILE_STATUS_ARB, &res);
    if (!res)
    {
      char pLog[2049];
      memset(pLog, 0, 2049);

      mpContext->glGetInfoLogARB(mVS, 2048, NULL, pLog);
      mErrorMessage = pLog;
      mpContext = NULL;
      return false;
    }

    mpContext->glAttachObjectARB(mProgram, mVS);
  }

  if (mpFragmentShader)
  {
    mpContext->glCompileShaderARB(mFS);

    GLint res;
    mpContext->glGetObjectParameterivARB(mFS, GL_OBJECT_COMPILE_STATUS_ARB, &res);
    if (!res)
    {
      char pLog[2049];
      memset(pLog, 0, 2049);

      mpContext->glGetInfoLogARB(mFS, 2048, NULL, pLog);
      mErrorMessage = pLog;
      mpContext = NULL;
      return false;
    }

    mpContext->glAttachObjectARB(mProgram, mFS);
  }

  mpContext->glLinkProgramARB(mProgram);

  GLint res;
  mpContext->glGetObjectParameterivARB(mProgram, GL_OBJECT_LINK_STATUS_ARB, &res);
  if (!res)
  {
    char pLog[2049];
    memset(pLog, 0, 2049);

    mpContext->glGetInfoLogARB(mProgram, 2048, NULL, pLog);
    mErrorMessage = pLog;
    mpContext = NULL;
    return false;
  }
#endif
  return true;
}

bool nuiShader::Bind()
{
  if (!mpContext)
    return false;

#ifdef GL_ARB_shader_objects
  mpContext->glUseProgramObjectARB(mProgram);
#endif

  return true;
}

bool nuiShader::UnBind()
{
  if (!mpContext)
    return false;

#ifdef GL_ARB_shader_objects
  mpContext->glUseProgramObjectARB(0);
#endif
  return true;
}

nglString nuiShader::GetErrorMessage()
{
  return mErrorMessage;
}

void nuiShader::SetTexture1D(int num, GLuint TextureID)
{
  if (!mpContext)
    return;

#ifdef GL_ARB_shader_objects
  char* texturename = (char*) _T("nuiTexture1D0");
  texturename[12] = '0' + num;

  GLint pos = mpContext->glGetUniformLocationARB(mProgram, texturename);
  mpContext->glUniform1iARB(pos, num);
#endif
}

void nuiShader::SetTexture2D(int num, GLuint TextureID)
{
  if (!mpContext)
    return;

#ifdef GL_ARB_shader_objects
  nglString tmp;
  tmp.CFormat(_T("nuiTexture2D%c"), '0' + num);
  std::string str(tmp.GetStdString());

  GLint pos = mpContext->glGetUniformLocationARB(mProgram, str.c_str());
  mpContext->glUniform1iARB(pos, num);
#endif
}

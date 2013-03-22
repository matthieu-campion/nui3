/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "AAPrimitives.h"

#ifndef __NUI_NO_GL__

//#define NUI_RETURN_IF_RENDERING_DISABLED return;
#define NUI_RETURN_IF_RENDERING_DISABLED
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6

//#define NUI_USE_ANTIALIASING
#ifdef NUI_USE_ANTIALIASING
#define NUI_USE_MULTISAMPLE_AA
#endif

static const char* TextureVertexColor_VTX =
"attribute vec4 Position;\n\
attribute vec2 TexCoord;\n\
attribute vec4 Color;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
TexCoordVar = (TexCoord + TextureTranslate) * TextureScale;\n\
ColorVar = Color;\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* TextureVertexColor_FGT =
"uniform sampler2D texture;\n\
varying vec4 ColorVar;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
gl_FragColor = ColorVar * texture2D(texture, TexCoordVar);\n\
}"
;

////////////////////////////////////////////////////////////////////////////////
static const char* TextureAlphaVertexColor_VTX =
"attribute vec4 Position;\n\
attribute vec2 TexCoord;\n\
attribute vec4 Color;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
TexCoordVar = (TexCoord + TextureTranslate) * TextureScale;\n\
ColorVar = Color;\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* TextureAlphaVertexColor_FGT =
"uniform sampler2D texture;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec4 ColorVar;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
float v = texture2D(texture, TexCoordVar)[3];\
gl_FragColor = ColorVar * v;\n\
}"
;

//////////////////////////////////////////////////////////////////////////////////
static const char* TextureDifuseColor_VTX =
"attribute vec4 Position;\n\
attribute vec2 TexCoord;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
TexCoordVar = (TexCoord + TextureTranslate) * TextureScale;\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* TextureDifuseColor_FGT =
"uniform sampler2D texture;\n\
uniform vec4 DifuseColor;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
gl_FragColor = DifuseColor * texture2D(texture, TexCoordVar);\n\
//gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n\
}"
;

//////////////////////////////////////////////////////////////////////////////////
static const char* TextureAlphaDifuseColor_VTX =
"attribute vec4 Position;\n\
attribute vec2 TexCoord;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
TexCoordVar = (TexCoord + TextureTranslate) * TextureScale;\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* TextureAlphaDifuseColor_FGT =
"uniform sampler2D texture;\n\
uniform vec4 DifuseColor;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
float v = texture2D(texture, TexCoordVar)[3];\
gl_FragColor = DifuseColor * vec4(v, v, v, v);\n\
//gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n\
}"
;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No texture cases:
static const char* VertexColor_VTX =
"attribute vec4 Position;\n\
attribute vec4 Color;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
ColorVar = Color;\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* VertexColor_FGT =
"uniform sampler2D texture;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
gl_FragColor = ColorVar;\n\
//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n\
}"
;

//////////////////////////////////////////////////////////////////////////////////
static const char* DifuseColor_VTX =
"attribute vec4 Position;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
void main()\n\
{\n\
gl_Position = (ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* DifuseColor_FGT =
"uniform sampler2D texture;\n\
uniform vec4 DifuseColor;\n\
void main()\n\
{\n\
gl_FragColor = DifuseColor;\n\
//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n\
}"
;




#ifdef _OPENGL_ES_

#define glCheckFramebufferStatusNUI   glCheckFramebufferStatusOES
#define glFramebufferRenderbufferNUI  glFramebufferRenderbufferOES
#define glRenderbufferStorageNUI      glRenderbufferStorageOES
#define glGenFramebuffersNUI          glGenFramebuffersOES
#define glDeleteFramebuffersNUI       glDeleteFramebuffersOES
#define glBindFramebufferNUI          glBindFramebufferOES
#define glGenRenderbuffersNUI         glGenRenderbuffersOES
#define glDeleteRenderbuffersNUI      glDeleteRenderbuffersOES
#define glBindRenderbufferNUI         glBindRenderbufferOES
#define glFramebufferTexture2DNUI     glFramebufferTexture2DOES

#define GL_FRAMEBUFFER_NUI                                GL_FRAMEBUFFER_OES
#define GL_RENDERBUFFER_NUI                               GL_RENDERBUFFER_OES
#define GL_FRAMEBUFFER_BINDING_NUI                        GL_FRAMEBUFFER_BINDING_OES
#define GL_RENDERBUFFER_BINDING_NUI                       GL_RENDERBUFFER_BINDING_OES

// FBO attachement points
#define GL_STENCIL_ATTACHMENT_NUI                         GL_STENCIL_ATTACHMENT_OES
#define GL_DEPTH_ATTACHMENT_NUI                           GL_DEPTH_ATTACHMENT_OES
#define GL_COLOR_ATTACHMENT0_NUI                          GL_COLOR_ATTACHMENT0_OES

// FBO errors:
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_NUI          GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_NUI  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_NUI          GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_NUI             GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
#define GL_FRAMEBUFFER_UNSUPPORTED_NUI                    GL_FRAMEBUFFER_UNSUPPORTED_OES
#define GL_FRAMEBUFFER_COMPLETE_NUI                       GL_FRAMEBUFFER_COMPLETE_OES

#define GL_DEPTH_COMPONENT16                              GL_DEPTH_COMPONENT16_OES
#define GL_DEPTH_COMPONENT24                              GL_DEPTH_COMPONENT24_OES

//#elif defined(_OPENGL_)
#else

#ifdef _MACOSX_
#define glCheckFramebufferStatusNUI   glCheckFramebufferStatusEXT
#define glFramebufferRenderbufferNUI  glFramebufferRenderbufferEXT
#define glRenderbufferStorageNUI      glRenderbufferStorageEXT
#define glGenFramebuffersNUI          glGenFramebuffersEXT
#define glDeleteFramebuffersNUI       glDeleteFramebuffersEXT
#define glBindFramebufferNUI          glBindFramebufferEXT
#define glGenRenderbuffersNUI         glGenRenderbuffersEXT
#define glDeleteRenderbuffersNUI      glDeleteRenderbuffersEXT
#define glBindRenderbufferNUI         glBindRenderbufferEXT
#define glFramebufferTexture2DNUI     glFramebufferTexture2DEXT
#else
#define glCheckFramebufferStatusNUI   mpContext->glCheckFramebufferStatusEXT
#define glFramebufferRenderbufferNUI  mpContext->glFramebufferRenderbufferEXT
#define glRenderbufferStorageNUI      mpContext->glRenderbufferStorageEXT
#define glGenFramebuffersNUI          mpContext->glGenFramebuffersEXT
#define glDeleteFramebuffersNUI       mpContext->glDeleteFramebuffersEXT
#define glBindFramebufferNUI          mpContext->glBindFramebufferEXT
#define glGenRenderbuffersNUI         mpContext->glGenRenderbuffersEXT
#define glDeleteRenderbuffersNUI      mpContext->glDeleteRenderbuffersEXT
#define glBindRenderbufferNUI         mpContext->glBindRenderbufferEXT
#define glFramebufferTexture2DNUI     mpContext->glFramebufferTexture2DEXT
#endif

#define GL_FRAMEBUFFER_NUI                                GL_FRAMEBUFFER_EXT
#define GL_RENDERBUFFER_NUI                               GL_RENDERBUFFER_EXT
#define GL_FRAMEBUFFER_BINDING_NUI                        GL_FRAMEBUFFER_BINDING_EXT
#define GL_RENDERBUFFER_BINDING_NUI                       GL_RENDERBUFFER_BINDING_EXT

// FBO attachement points
#define GL_STENCIL_ATTACHMENT_NUI                         GL_STENCIL_ATTACHMENT_EXT
#define GL_DEPTH_ATTACHMENT_NUI                           GL_DEPTH_ATTACHMENT_EXT
#define GL_COLOR_ATTACHMENT0_NUI                          GL_COLOR_ATTACHMENT0_EXT

// FBO errors:
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_NUI          GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_NUI  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_NUI          GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_NUI             GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
#define GL_FRAMEBUFFER_UNSUPPORTED_NUI                    GL_FRAMEBUFFER_UNSUPPORTED_EXT
#define GL_FRAMEBUFFER_COMPLETE_NUI                       GL_FRAMEBUFFER_COMPLETE_EXT

//#else
//#error "bleh"
#endif

void nuiGL2Painter::BlendFuncSeparate(GLenum src, GLenum dst, GLenum srcalpha, GLenum dstalpha)
{
  mSrcColor = src;
  mDstColor = dst;
  mSrcAlpha = srcalpha;
  mDstAlpha = dstalpha;
#ifndef _OPENGL_ES_
  if (mpContext->glBlendFuncSeparate)
  {
    mpContext->glBlendFuncSeparate(src, dst, srcalpha, dstalpha);
    mTwoPassBlend = false;
  }
  else 
  {
    glBlendFunc(src, dst);
    mTwoPassBlend = true;
  }
#else
#if GL_OES_blend_equation_separate
  //    if (glBlendFuncSeparateOES)
  //    {
  //      glBlendFuncSeparateOES(src, dst, srcalpha, dstalpha);
  //      mTwoPassBlend = false;
  //    }
  //    else
  {
    glBlendFunc(src, dst);
    mTwoPassBlend = true;
  }
#else
  glBlendFunc(src, dst);
#endif
#endif
}


bool nuiGL2Painter::CheckFramebufferStatus()
{
  //  return true;
#if 1
  GLint status = glCheckFramebufferStatusNUI(GL_FRAMEBUFFER_NUI);
#if defined(NGL_DEBUG)
  switch (status)
  {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_NUI:
    {
      NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"));
    } break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_NUI:
    {
      NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"));
    } break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_NUI:
    {
      NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n"));
    } break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_NUI:
    {
      NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_FORMATS\n"));
    } break;
    case GL_FRAMEBUFFER_UNSUPPORTED_NUI:
    {
      NGL_OUT(_T("GL_FRAMEBUFFER_UNSUPPORTED\n"));
    } break;
  }
#endif  
  return (status == GL_FRAMEBUFFER_COMPLETE_NUI);
#endif
}

uint32 nuiGL2Painter::mActiveContexts = 0;

static uint32 mins = 30000;
static uint32 maxs = 0;
static uint32 totalinframe = 0;
static uint32 total = 0;

nuiGL2Painter::nuiGL2Painter(nglContext* pContext, const nuiRect& rRect)
: nuiPainter(rRect, pContext)
{
  mCanRectangleTexture = 0;
  mTextureTarget = GL_TEXTURE_2D;
  mTwoPassBlend = false;
  mDefaultFramebuffer = 0;
  mDefaultRenderbuffer = 0;
  mForceApply = false;
  mMatrixChanged = true;
  mR = -1;
  mG = -1;
  mB = -1;
  mA = -1;
  mTexEnvMode = 0;
  mViewPort[0] = 0;
  mViewPort[1] = 0;
  mViewPort[2] = 0;
  mViewPort[3] = 0;
  
  
  mpContext = pContext;
  if (mpContext)
  {
    mpContext->BeginSession();
    const char* ext0 = (const char*)glGetString(GL_EXTENSIONS);
    nglString exts(ext0);
    //NGL_OUT(_T("Extensions: %s\n"), exts.GetChars());
    
    
    mpContext->CheckExtension(_T("GL_VERSION_1_2"));
    nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_1_3"));
    nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_1_4"));
    nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_1_5"));
    nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_2_0"));
    nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_ARB_vertex_buffer_object"));
    nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_EXT_framebuffer_object"));
    nuiCheckForGLErrors();
    
    mpContext->CheckExtension(_T("GL_ARB_framebuffer_object"));
    nuiCheckForGLErrors();
    
    if (mpContext->CheckExtension(_T("GL_ARB_texture_non_power_of_two")))
    {
      mCanRectangleTexture = 1;
    }
    else if (mpContext->CheckExtension(_T("GL_EXT_texture_rectangle")) || mpContext->CheckExtension(_T("GL_ARB_texture_rectangle")) || mpContext->CheckExtension(_T("GL_NV_texture_rectangle")))
    {
      mCanRectangleTexture = 2;
    }
    //mCanRectangleTexture = 0;
    
    
    if (!mActiveContexts)
    {
      glAAInit();
      nuiCheckForGLErrors();
    }
    
    
    nuiCheckForGLErrors();
  }

#ifdef _OPENGL_ES_
  nglString pre = "precision mediump float;\n";
#else
  nglString pre;
#endif

  mpShader_TextureVertexColor = new nuiShaderProgram();
  mpShader_TextureVertexColor->Acquire();
  mpShader_TextureVertexColor->AddShader(eVertexShader, pre + TextureVertexColor_VTX);
  mpShader_TextureVertexColor->AddShader(eFragmentShader, pre + TextureVertexColor_FGT);
  mpShader_TextureVertexColor->Link();
  mpShader_TextureVertexColor->GetDefaultState().Set("Offset", 0.0f, 0.0f);

  mpShader_TextureAlphaVertexColor = new nuiShaderProgram();
  mpShader_TextureAlphaVertexColor->Acquire();
  mpShader_TextureAlphaVertexColor->AddShader(eVertexShader, pre + TextureAlphaVertexColor_VTX);
  mpShader_TextureAlphaVertexColor->AddShader(eFragmentShader, pre + TextureAlphaVertexColor_FGT);
  mpShader_TextureAlphaVertexColor->Link();
  mpShader_TextureAlphaVertexColor->GetDefaultState().Set("Offset", 0.0f, 0.0f);

  mpShader_TextureDifuseColor = new nuiShaderProgram();
  mpShader_TextureDifuseColor->Acquire();
  mpShader_TextureDifuseColor->AddShader(eVertexShader, pre + TextureDifuseColor_VTX);
  mpShader_TextureDifuseColor->AddShader(eFragmentShader, pre + TextureDifuseColor_FGT);
  mpShader_TextureDifuseColor->Link();
  mpShader_TextureDifuseColor->GetDefaultState().Set("DifuseColor", nuiColor(255, 255, 255, 255));
  mpShader_TextureDifuseColor->GetDefaultState().Set("Offset", 0.0f, 0.0f);

  mpShader_TextureAlphaDifuseColor = new nuiShaderProgram();
  mpShader_TextureAlphaDifuseColor->Acquire();
  mpShader_TextureAlphaDifuseColor->AddShader(eVertexShader, pre + TextureAlphaDifuseColor_VTX);
  mpShader_TextureAlphaDifuseColor->AddShader(eFragmentShader, pre + TextureAlphaDifuseColor_FGT);
  mpShader_TextureAlphaDifuseColor->Link();
  mpShader_TextureAlphaDifuseColor->GetDefaultState().Set("DifuseColor", nuiColor(255, 255, 255, 255));
  mpShader_TextureAlphaDifuseColor->GetDefaultState().Set("Offset", 0.0f, 0.0f);

  mpShader_VertexColor = new nuiShaderProgram();
  mpShader_VertexColor->Acquire();
  mpShader_VertexColor->AddShader(eVertexShader, pre + VertexColor_VTX);
  mpShader_VertexColor->AddShader(eFragmentShader, pre + VertexColor_FGT);
  mpShader_VertexColor->Link();
  mpShader_VertexColor->GetDefaultState().Set("Offset", 0.0f, 0.0f);

  mpShader_DifuseColor = new nuiShaderProgram();
  mpShader_DifuseColor->Acquire();
  mpShader_DifuseColor->AddShader(eVertexShader, pre + DifuseColor_VTX);
  mpShader_DifuseColor->AddShader(eFragmentShader, pre + DifuseColor_FGT);
  mpShader_DifuseColor->Link();
  mpShader_DifuseColor->GetDefaultState().Set("DifuseColor", nuiColor(255, 255, 255, 255));
  mpShader_DifuseColor->GetDefaultState().Set("Offset", 0.0f, 0.0f);

#ifdef _OPENGL_ES_
  //  mDefaultFramebuffer = 0;
  //  mDefaultRenderbuffer = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_NUI, &mDefaultFramebuffer);
  glGetIntegerv(GL_RENDERBUFFER_BINDING_NUI, (GLint *) &mDefaultRenderbuffer);
#endif
  
  mActiveContexts++;
}

nuiGL2Painter::~nuiGL2Painter()
{
  mActiveContexts--;
  mpShader_TextureVertexColor->Release();
  mpShader_TextureDifuseColor->Release();
  mpShader_TextureAlphaVertexColor->Release();
  mpShader_TextureAlphaDifuseColor->Release();
  mpShader_DifuseColor->Release();
  mpShader_VertexColor->Release();
  if (mActiveContexts == 0)
    glAAExit();
}

void nuiGL2Painter::SetViewport()
{
  //GetAngle(), GetCurrentWidth(), GetCurrentHeight(), mProjectionViewportStack.top(), mProjectionMatrixStack.top());
  GLint Angle = GetAngle();
  GLint Width = GetCurrentWidth();
  GLint Height = GetCurrentHeight();
  const nuiRect& rViewport(mProjectionViewportStack.top());
  const nuiMatrix& rMatrix = mProjectionMatrixStack.top();
  
  uint32 x, y, w, h;
  
  nuiRect r(rViewport);
  if (Angle == 90 || Angle == 270)
  {
    uint32 tmp = Width;
    Width = Height;
    Height = tmp;
    r.Set(r.Top(), r.Left(), r.GetHeight(), r.GetWidth());
  }
  
  
  x = ToBelow(r.Left());
  w = ToBelow(r.GetWidth());
  y = Height - ToBelow(r.Bottom());
  h = ToBelow(r.GetHeight());
  
  //printf("set projection matrix (%d %d - %d %d)\n", x, y, w, h);
  if (!mpSurface)
  {
    //glViewport(x * nuiGetScaleFactor(), y * nuiGetScaleFactor(), w * nuiGetScaleFactor(), h * nuiGetScaleFactor());
    x *= nuiGetScaleFactor();
    y *= nuiGetScaleFactor();
    w *= nuiGetScaleFactor();
    h *= nuiGetScaleFactor();
  }
//  else
//    glViewport(x, y, w, h);

//  if (mViewPort[0] != x || mViewPort[1] != y || mViewPort[2] != w || mViewPort[3] != h)
  nuiCheckForGLErrors();
  {
    mViewPort[0] = x;
    mViewPort[1] = y;
    mViewPort[2] = w;
    mViewPort[3] = h;
    glViewport(mViewPort[0], mViewPort[1], mViewPort[2], mViewPort[3]);
  }
  
  nuiCheckForGLErrors();
  
  nuiCheckForGLErrors();
}

void nuiGL2Painter::StartRendering()
{
  BeginSession();
  //NUI_RETURN_IF_RENDERING_DISABLED;
  nuiCheckForGLErrors();
  
  mScissorX = -1;
  mScissorY = -1;
  mScissorW = -1;
  mScissorH = -1;
  mScissorOn = false;
  
  nuiPainter::StartRendering();
  
  SetViewport();
//  glLoadIdentity();
  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_STENCIL_TEST);
  //glDisable(GL_BLEND);
  glEnable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  BlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  
  mMatrixChanged = true;
  mR = -1;
  mG = -1;
  mB = -1;
  mA = -1;
  mTexEnvMode = 0;
  
  nuiCheckForGLErrors();
}

void nuiGL2Painter::ApplyState(const nuiRenderState& rState, bool ForceApply)
{
  //TEST_FBO_CREATION();
  NUI_RETURN_IF_RENDERING_DISABLED;
  //ForceApply = true;
  nuiCheckForGLErrors();

  // blending
  if (ForceApply || mFinalState.mBlending != rState.mBlending)
  {
    mFinalState.mBlending = rState.mBlending;
    if (mFinalState.mBlending)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }
  }
  
  if (ForceApply || mFinalState.mBlendFunc != rState.mBlendFunc)
  {
    mFinalState.mBlendFunc = rState.mBlendFunc;
    GLenum src, dst;
    nuiGetBlendFuncFactors(rState.mBlendFunc, src, dst);
    BlendFuncSeparate(src, dst);
    nuiCheckForGLErrors();
  }
  
  if (ForceApply || mFinalState.mDepthTest != rState.mDepthTest)
  {
    mFinalState.mDepthTest = rState.mDepthTest;
    if (mFinalState.mDepthTest)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }
  
  if (ForceApply || mFinalState.mDepthWrite != rState.mDepthWrite)
  {
    mFinalState.mDepthWrite = rState.mDepthWrite;
    glDepthMask(mFinalState.mDepthWrite);
  }

  // We don't care about the font in the lower layer of rendering
  //nuiFont* mpFont;
  // 
  
  ApplyTexture(rState, ForceApply);  
  
  // Rendering buffers:
  if (ForceApply || mFinalState.mColorBuffer != rState.mColorBuffer)
  {
    mFinalState.mColorBuffer = rState.mColorBuffer;
    GLboolean m = mFinalState.mColorBuffer ? GL_TRUE : GL_FALSE;
    glColorMask(m, m, m, m);
    nuiCheckForGLErrors();
  }
  
  if (mClip.mEnabled || ForceApply)    
  {
    uint32 width = mWidth;
    uint32 height = mHeight;
    
    if (mpSurface)
    {
      width = mpSurface->GetWidth();
      height = mpSurface->GetHeight();
    }
    
    nuiRect clip(mClip);
    
    int x,y,w,h;
    uint angle = (mpSurface && mpSurface->GetRenderToTexture()) ? 0 : mAngle;
    if (angle == 90)
    {
      x = ToBelow(clip.Top());
      y = ToBelow(clip.Left());
      w = ToBelow(clip.GetHeight());
      h = ToBelow(clip.GetWidth());
    }
    else if (angle == 180)
    {
      w = ToBelow(clip.GetWidth());
      h = ToBelow(clip.GetHeight());
      x = ToBelow(width - w - clip.Left());
      y = ToBelow(clip.Top());
    }
    else if (angle == 270)
    {
      w = ToBelow(clip.GetHeight());
      h = ToBelow(clip.GetWidth());
      x = ToBelow(height - clip.Top() - w);
      y = ToBelow(width - clip.Left() - h);
    }
    else
    {
      NGL_ASSERT(!angle);
      x = ToBelow(clip.Left());
      y = ToBelow(height - clip.Bottom());
      w = ToBelow(clip.GetWidth());
      h = ToBelow(clip.GetHeight());
    }
    //NGL_OUT(_T("To Screen Clip {%d, %d, %d, %d}\n"), x,y,w,h);
    
    if (!mScissorOn || ForceApply)
    {
      glEnable(GL_SCISSOR_TEST);
      mScissorOn = true;
    }
    
    if (mScissorX != x || mScissorY != y || mScissorW != w || mScissorH != h || ForceApply)
    {
      mScissorX = x;
      mScissorY = y;
      mScissorW = w;
      mScissorH = h;
      
      if (!mpSurface)
      {
        x *= nuiGetScaleFactor();
        y *= nuiGetScaleFactor();
        w *= nuiGetScaleFactor();
        h *= nuiGetScaleFactor();
      }
      glScissor(x, y, w, h);
    }
    nuiCheckForGLErrors();
  }
  else
  {
    if (mScissorOn || ForceApply)
    {
      glDisable(GL_SCISSOR_TEST);
      mScissorOn = false;
    }
  }
  
  mFinalState.mClearColor = rState.mClearColor;
  mFinalState.mStrokeColor = rState.mStrokeColor;
  mFinalState.mFillColor = rState.mFillColor;
  
  mForceApply = false;
  
  nuiCheckForGLErrors();
}

void nuiGL2Painter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  //TEST_FBO_CREATION();
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  mState = rState;
  mForceApply |= ForceApply;
  //ApplyState(rState, ForceApply);
}

void nuiGL2Painter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  mWidth = w;
  mHeight = h;
}

void nuiGL2Painter::ApplyTexture(const nuiRenderState& rState, bool ForceApply)
{
  //  if ((rState.mTexturing && !rState.mpTexture) || (!rState.mTexturing && rState.mpTexture))
  //  {
  //    printf("bleh!\n");
  //    char* bleh = NULL;
  //    bleh[0] = 0;
  //  }
  
  // 2D Textures: 
  auto it = mTextures.find(rState.mpTexture);
  bool uptodate = (it == mTextures.end()) ? false : ( !it->second.mReload && it->second.mTexture >= 0 );
  if (ForceApply || (mFinalState.mpTexture != rState.mpTexture) || (mFinalState.mpTexture && !uptodate))
  { 
    GLenum intarget = 0;
    GLenum outtarget = 0;
    
    if (mFinalState.mpTexture)
    {      
      outtarget = GetTextureTarget(mFinalState.mpTexture->IsPowerOfTwo());
      if (mFinalState.mpTexture->GetTextureID())
        outtarget = mFinalState.mpTexture->GetTarget();
      
      //mFinalState.mpTexture->UnapplyGL(this); #TODO Un apply the texture
      nuiCheckForGLErrors();
      mFinalState.mpTexture->Release();
      nuiCheckForGLErrors();
    }
    
    //NGL_OUT(_T("Change texture to 0x%x (%s)\n"), rState.mpTexture, rState.mpTexture?rState.mpTexture->GetSource().GetChars() : nglString::Empty.GetChars());
    mFinalState.mpTexture = rState.mpTexture ;
    
    if (mFinalState.mpTexture)
    {
      if (mFinalState.mpTexture->GetTextureID())
        intarget = mFinalState.mpTexture->GetTarget();
      else
        intarget = GetTextureTarget(mFinalState.mpTexture->IsPowerOfTwo());
      
      mFinalState.mpTexture->Acquire();
      
      nuiSurface* pSurface = mFinalState.mpTexture->GetSurface();
      if (pSurface)
      {
        std::map<nuiSurface*, FramebufferInfo>::const_iterator it = mFramebuffers.find(pSurface);
        bool create = (it == mFramebuffers.end()) ? true : false;  
        if (create || pSurface->IsDirty())
        {
          PushClipping();
          nuiRenderState s(mState);// PushState();
          PushProjectionMatrix();
          PushMatrix();
          
#ifdef _OPENGL_ES_
          if (mpSurfaceStack.empty())
          {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING_NUI, &mDefaultFramebuffer);
            glGetIntegerv(GL_RENDERBUFFER_BINDING_NUI, (GLint *) &mDefaultRenderbuffer);
          }
#endif
          
          PushSurface();
          
          SetState(nuiRenderState());
          ResetClipRect();
          mClip.Set(0, 0, pSurface->GetWidth(), pSurface->GetHeight());
          
          LoadMatrix(nglMatrixf());
          
          NGL_ASSERT(pSurface);
          SetSurface(pSurface);
          nuiMatrix m;
          m.Translate(-1.0f, 1.0f, 0.0f);
          m.Scale(2.0f / pSurface->GetWidth(), -2.0f / pSurface->GetHeight(), 1.0f);
          LoadProjectionMatrix(nuiRect(pSurface->GetWidth(), pSurface->GetHeight()), m);

          //////////////////////////////          
          nuiDrawContext Ctx(nuiRect(pSurface->GetWidth(), pSurface->GetHeight()));
          Ctx.SetPainter(this);
          //pSurface->Realize(&Ctx);
          glDisable(GL_SCISSOR_TEST);
//          glClearColor(1, 0, 1, 1);
          //mFinalState.mShaderState.SetProjectionMatrix(mProjectionMatrixStack.top(), true);
          //mFinalState.mShaderState.SetModelViewMatrix(mMatrixStack.top(), true);
//          glClear(GL_COLOR_BUFFER_BIT);
//          glColor3f(0, 0, 1);
//          glBegin(GL_TRIANGLE_FAN);
//          glVertex2f(0, 0);
//          glVertex2f(10, 0);
//          glVertex2f(10, 10);
//          glVertex2f(0, 10);
//          glEnd();

          Ctx.SetPainter(NULL);
          //////////////////////////////
          
          PopSurface();
          PopMatrix();
          PopProjectionMatrix();
          SetState(s);
          PopClipping();
        }
      }
      
      UploadTexture(mFinalState.mpTexture);
      nuiCheckForGLErrors();
    }
    
    //NGL_OUT(_T("Change texture type from 0x%x to 0x%x\n"), outtarget, intarget);
    
    mTextureTarget = intarget;
    if (intarget != outtarget)
    {
      // Texture Target has changed
      if (outtarget)
      {
//        glDisable(outtarget);
        nuiCheckForGLErrors();
      }
      //NGL_OUT(_T("disable outtarget\n"));
      if (intarget && mFinalState.mTexturing && mFinalState.mpTexture)
      {
        mFinalState.mTexturing = rState.mTexturing;
        //NGL_OUT(_T("enable intarget\n"));
//        glEnable(intarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      // Texture Target have not changed     
      if (mFinalState.mTexturing != rState.mTexturing) // Have texture on/off changed?
      {
        // Should enable or disable texturing
        mFinalState.mTexturing = rState.mTexturing;
        if (mFinalState.mTexturing)
        {
//          glEnable(mTextureTarget);
          nuiCheckForGLErrors();
        }
        else
        {
//          glDisable(mTextureTarget);
          nuiCheckForGLErrors();
        }
      }
    }
  }
  
  if (ForceApply || (mFinalState.mTexturing != rState.mTexturing))
  {
    // Texture have not changed, but texturing may have been enabled / disabled
    mFinalState.mTexturing = rState.mTexturing;
    
    if (mFinalState.mpTexture)
    {
      if (mTextureTarget && mFinalState.mTexturing)
      {
        //NGL_OUT(_T("Enable 0x%x\n"), mTextureTarget);
//        glEnable(mTextureTarget);
        nuiCheckForGLErrors();
      }
      else
      {
        //NGL_OUT(_T("Disable 0x%x\n"), mTextureTarget);
//        glDisable(mTextureTarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      if (mTextureTarget)
      {
        //NGL_OUT(_T("Disable 0x%x\n"), mTextureTarget);
//        glDisable(mTextureTarget);
      }
      nuiCheckForGLErrors();
    }
  }
  
}


void nuiGL2Painter::ClearColor()
{
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  glClearColor(mState.mClearColor.Red(),mState.mClearColor.Green(),mState.mClearColor.Blue(),mState.mClearColor.Alpha());
  glClear(GL_COLOR_BUFFER_BIT);
  nuiCheckForGLErrors();
}

#define LOGENUM(XXX) case XXX: { NGL_OUT(_T("%s\n"), #XXX); } break;

void nuiGL2Painter::DrawArray(nuiRenderArray* pArray)
{
  mRenderOperations++;
  mBatches++;

  if (!mEnableDrawArray)
  {
    pArray->Release();
    return;
  }

  
  static uint32 ops = 0;
  static uint32 skipped_ops = 0;
  
  ops++;
  const nuiMatrix& rM(mMatrixStack.top());
  float bounds[6];
  pArray->GetBounds(bounds);
  
  if (rM.Elt.M11 == 1.0f
      && rM.Elt.M22 == 1.0f
      && rM.Elt.M33 == 1.0f
      && rM.Elt.M44 == 1.0f
      
      && rM.Elt.M12 == 0.0f
      && rM.Elt.M13 == 0.0f
      //&& rM.Elt.M14 == 0.0f
      
      && rM.Elt.M21 == 0.0f
      && rM.Elt.M23 == 0.0f
      //&& rM.Elt.M24 == 0.0f
      
      && rM.Elt.M31 == 0.0f
      && rM.Elt.M32 == 0.0f
      && rM.Elt.M34 == 0.0f
      
      && rM.Elt.M41 == 0.0f
      && rM.Elt.M42 == 0.0f
      && rM.Elt.M43 == 0.0f)
  {
    bounds[0] += rM.Elt.M14;
    bounds[1] += rM.Elt.M24;
    //bounds[2] += rM.Elt.M34;
    bounds[3] += rM.Elt.M14;
    bounds[4] += rM.Elt.M24;
    //bounds[5] += rM.Elt.M34;
    
    if (
        (bounds[0] > mClip.Right()) ||
        (bounds[1] > mClip.Bottom()) ||
        (bounds[3] < mClip.Left()) ||
        (bounds[4] < mClip.Top())
        )
    {
      pArray->Release();
      skipped_ops++;
      
      //      #ifdef _DEBUG
      //      if (!(skipped_ops % 100))
      //        printf("optim (%d / %d) - %2.2f%%\n", skipped_ops, ops, (float)skipped_ops * 100.0f / (float)ops);
      //      #endif
      
      return;
    }
  }

  // Shader selection:
  if (mForceApply || mFinalState.mpShader != mState.mpShader || !mFinalState.mpShader)
  {
    if (mState.mpShader)
      mState.mpShader->Acquire();
    if (mFinalState.mpShader)
      mFinalState.mpShader->Release();

    mFinalState.mpShader = mState.mpShader;

    if (mFinalState.mpShader == NULL)
    {
      nuiShaderProgram* pShader = NULL;
      if (pArray->IsArrayEnabled(nuiRenderArray::eColor))
      {
        // Vertex Colors is on
        if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
        {
          // texture on
          if (mState.mpTexture->GetPixelFormat() == eImagePixelAlpha)
            pShader = mpShader_TextureAlphaVertexColor;
          else
            pShader = mpShader_TextureVertexColor;
        }
        else
        {
          pShader = mpShader_VertexColor;
        }
      }
      else
      {
        // Vertex color off -> Difuse Color
        if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
        {
          // texture on
          if (mState.mpTexture->GetPixelFormat() == eImagePixelAlpha)
            pShader = mpShader_TextureAlphaDifuseColor;
          else
            pShader = mpShader_TextureDifuseColor;
        }
        else
        {
          pShader = mpShader_DifuseColor;
        }
      }

      pShader->Acquire();
      mFinalState.mpShader = pShader;
      mFinalState.mShaderState = pShader->GetDefaultState();
    }

    NGL_ASSERT(mFinalState.mpShader != NULL);

    glUseProgram(mFinalState.mpShader->GetProgram());

  }

  if (mFinalState.mpTexture && pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    mFinalState.mpShader->GetDefaultState().Set("TextureTranslate", mTextureTranslate, true);
    mFinalState.mpShader->GetDefaultState().Set("TextureScale", mTextureScale, true);
  }

  mFinalState.mShaderState.SetProjectionMatrix(mProjectionMatrixStack.top(), false);
  mFinalState.mShaderState.SetModelViewMatrix(mMatrixStack.top(), false);
  mFinalState.mShaderState.Apply();

  ApplyState(mState, mForceApply);


  uint32 s = pArray->GetSize();
  
  total += s;
  totalinframe += s;
  mins = MIN(mins, s);
  maxs = MAX(maxs, s);

  if (!s)
  {
    pArray->Release();
    return;
  }
  
  if (mClip.GetWidth() <= 0 || mClip.GetHeight() <= 0)
  {
    pArray->Release();
    return;
  }
  
  mVertices += s;
  GLenum mode = pArray->GetMode();
  
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  if (mMatrixChanged)
  {
    //glLoadMatrixf(mMatrixStack.top().Array);
    mMatrixChanged = false;
  }
  
  
  bool NeedTranslateHack = pArray->IsShape() || ((mode == GL_POINTS || mode == GL_LINES || mode == GL_LINE_LOOP || mode == GL_LINE_STRIP) && !pArray->Is3DMesh());
  float hackX = 0;
  float hackY = 0;
  if (NeedTranslateHack)
  {
    const float ratio = nuiGetInvScaleFactor() / 2.f;
#ifdef _UIKIT_
    hackX = ratio;
    hackY = ratio;
#else
    if (mAngle == 0)
    {
      hackX = ratio;
      hackY = ratio;
    }
    else if (mAngle == 90)
    {
      hackX = 0;
      hackY = ratio;
    }
    else if (mAngle == 180)
    {
      hackX = 0;
      hackY = 0;
    }
    else/*mAngle == 270*/
    {
      hackX = ratio;
      hackY = 0;
    }
#endif
  }

  mFinalState.mpShader->GetDefaultState().Set("Offset", hackX, hackY, true);

  mFinalState.mpShader->SetVertexPointers(*pArray);

  if (!pArray->IsArrayEnabled(nuiRenderArray::eColor))
  {
    nuiColor c;
    switch (pArray->GetMode())
    {
      case GL_POINTS:
      case GL_LINES:
      case GL_LINE_LOOP:
      case GL_LINE_STRIP:
        c = mFinalState.mStrokeColor;
        break;

      case GL_TRIANGLES:
      case GL_TRIANGLE_STRIP:
      case GL_TRIANGLE_FAN:
        c = mFinalState.mFillColor;
        break;
    }

    mR = c.Red();
    mG = c.Green();
    mB = c.Blue();
    mA = c.Alpha();

    mFinalState.mpShader->GetDefaultState().Set("DifuseColor", nuiColor(mR, mG, mB, mA), true);
  }

  nuiCheckForGLErrors();
  
  if (mpSurface && mTwoPassBlend)
  {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    uint32 arraycount = pArray->GetIndexArrayCount();
    
    if (!arraycount)
    {
      glDrawArrays(mode, 0, s);
    }
    else
    {
      for (uint32 i = 0; i < arraycount; i++)
      {
        nuiRenderArray::IndexArray& array(pArray->GetIndexArray(i));
#if (defined _UIKIT_) || (defined _ANDROID_)
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
#else
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
#endif
      }
    }
    nuiCheckForGLErrors();
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    glBlendFunc(mSrcAlpha, mDstAlpha);
    if (!arraycount)
    {
      glDrawArrays(mode, 0, s);
    }
    else
    {
      for (uint32 i = 0; i < arraycount; i++)
      {
        nuiRenderArray::IndexArray& array(pArray->GetIndexArray(i));
#if (defined _UIKIT_) || (defined _ANDROID_)
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
#else
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
#endif
      }
    }
    glBlendFunc(mSrcColor, mDstColor);
    nuiCheckForGLErrors();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  }
  else
  {
    uint32 arraycount = pArray->GetIndexArrayCount();
    
    if (!arraycount)
    {
      glDrawArrays(mode, 0, s);
    }
    else
    {      
      for (uint32 i = 0; i < arraycount; i++)
      {
        nuiRenderArray::IndexArray& array(pArray->GetIndexArray(i));
#if (defined _UIKIT_) || (defined _ANDROID_)
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
#else
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
#endif
      }
    }
    nuiCheckForGLErrors();
  }
  
  
  mFinalState.mpShader->ResetVertexPointers(*pArray);
  pArray->Release();
  nuiCheckForGLErrors();
}

void nuiGL2Painter::BeginSession()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  mpContext->MakeCurrent();
  nuiCheckForGLErrors();
  
  totalinframe = 0;
}

void nuiGL2Painter::EndSession()
{
  // Bleh!
  NUI_RETURN_IF_RENDERING_DISABLED;
  //printf("min = %d max = %d total in frame = %d total = %d\n", mins, maxs, totalinframe, total);
}

void nuiGL2Painter::LoadMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::LoadMatrix(rMatrix);
  mMatrixChanged = true;
  nuiCheckForGLErrors();
}

void nuiGL2Painter::MultMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::MultMatrix(rMatrix);
  mMatrixChanged = true;
  nuiCheckForGLErrors();
}

void nuiGL2Painter::PushMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PushMatrix();
  mMatrixChanged = true;
  nuiCheckForGLErrors();
}

void nuiGL2Painter::PopMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PopMatrix();
  mMatrixChanged = true;
  nuiCheckForGLErrors();
}







void nuiGL2Painter::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::LoadProjectionMatrix(rViewport, rMatrix);
  
  SetViewport();
}

void nuiGL2Painter::MultProjectionMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::MultProjectionMatrix(rMatrix);
  
  SetViewport();
}

void nuiGL2Painter::PushProjectionMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PushProjectionMatrix();
}

void nuiGL2Painter::PopProjectionMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PopProjectionMatrix();
  SetViewport();
}



#ifdef glDeleteBuffersARB
#undef glDeleteBuffersARB
#endif

void nuiGL2Painter::ReleaseCacheObject(void* pHandle)
{
  /* Not 64 bit-safe and 'dead code' as Meeloo said
   GLint array = (GLint)pHandle;
   mpContext->glDeleteBuffersARB(1, &array);
   nuiCheckForGLErrors();
   */
}

uint32 nuiGL2Painter::GetRectangleTextureSupport() const
{
  return mCanRectangleTexture;
}

GLenum nuiGL2Painter::GetTextureTarget(bool POT) const
{
  GLenum target = GL_TEXTURE_2D;
  
  if (!POT)
  {
    switch (GetRectangleTextureSupport())
    {
      case 0:
        break;
      case 1:
        break;
      case 2:
#ifndef _OPENGL_ES_
        target = GL_TEXTURE_RECTANGLE_ARB;
#endif
        break;
    }
  }
  
  return target;
}

nuiGL2Painter::TextureInfo::TextureInfo()
{
  mReload = false;
  mTexture = -1;
}

void nuiGL2Painter::CreateTexture(nuiTexture* pTexture)
{
}

void nuiGL2Painter::UploadTexture(nuiTexture* pTexture)
{
  glActiveTexture(GL_TEXTURE0);
  
  nuiTexture* pProxy = pTexture->GetProxyTexture();
  if (pProxy)
    pTexture = pProxy;
  nuiSurface* pSurface = pTexture->GetSurface();
  
  float Width = pTexture->GetUnscaledWidth();
  float Height = pTexture->GetUnscaledHeight();
  GLenum target = GetTextureTarget(pTexture->IsPowerOfTwo());
  
  bool changedctx = false;
  
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    it = mTextures.insert(mTextures.begin(), std::pair<nuiTexture*, TextureInfo>(pTexture, TextureInfo()));
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);
  
  GLint id = pTexture->GetTextureID();
  if (id)
  {
    info.mReload = false;
    info.mTexture = id;
    target = pTexture->GetTarget();
  }
  
  nuiCheckForGLErrors();
  
  if (!pTexture->IsPowerOfTwo())
  {
    switch (GetRectangleTextureSupport())
    {
      case 0:
        Width = pTexture->GetWidthPOT();
        Height = pTexture->GetHeightPOT();
        break;
      case 1:
      case 2:
        break;
    }
  }
  
  //NGL_OUT(_T("Apply Target: 0x%x\n"), target);
  nglImage* pImage = pTexture->GetImage();
  
  {
    bool firstload = false;
    bool reload = info.mReload;
    if (!pSurface && !(pImage && pImage->GetPixelSize()) && !id)
      return;
    
    uint i;
    if (info.mTexture == (GLint)-1)
    { // Generate a texture
      //      if (mpSharedContext)
      //      {
      //        mpSharedContext->MakeCurrent();
      //        nuiCheckForGLErrors();
      //        changedctx = true;
      //      }
      
      glGenTextures(1, (GLuint*)&info.mTexture);
      //NGL_OUT(_T("nuiGL2Painter::UploadTexture 0x%x : '%s' / %d\n"), pTexture, pTexture->GetSource().GetChars(), info.mTexture);
      nuiCheckForGLErrors();
      firstload = true;
      reload = true;
    }
    
    glBindTexture(target, info.mTexture);
    nuiCheckForGLErrors();
    
    if (reload)
    {
      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, pTexture->GetMinFilter());
      nuiCheckForGLErrors();
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, pTexture->GetMagFilter());
      nuiCheckForGLErrors();
      glTexParameteri(target, GL_TEXTURE_WRAP_S, pTexture->GetWrapS());
      nuiCheckForGLErrors();
      glTexParameteri(target, GL_TEXTURE_WRAP_T, pTexture->GetWrapT());
      nuiCheckForGLErrors();

      int type = 8;
      GLint pixelformat = 0;
      GLint internalPixelformat = 0;
      GLbyte* pBuffer = NULL;
      bool allocated = false;
      
      if (pImage)
      {
        type = pImage->GetBitDepth();
        pixelformat = pImage->GetPixelFormat();
        internalPixelformat = pImage->GetPixelFormat();
        pBuffer = (GLbyte*)pImage->GetBuffer();
        
//#ifndef NUI_IOS
#if (!defined NUI_IOS) && (!defined _ANDROID_)
        if (pixelformat == GL_BGR)
          internalPixelformat = GL_RGB;
#endif
        
        if (!GetRectangleTextureSupport())
        {
          nuiCheckForGLErrors();
          if (!pTexture->IsPowerOfTwo())
          {        
            pBuffer = (GLbyte*)malloc( (uint)(Width * Height * pImage->GetPixelSize()));
            if (!pBuffer) 
              return;
            allocated = true;
            memset(pBuffer,0, (uint)(Width*Height * pImage->GetPixelSize()));
            
            for (i=0; i < pImage->GetHeight(); i++)
            {
              
              memcpy
              (
               pBuffer + (uint)(Width * i * pImage->GetPixelSize()), 
               pImage->GetBuffer() + pImage->GetBytesPerLine()*i, 
               pImage->GetBytesPerLine()
               );
            }
          }
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        nuiCheckForGLErrors();
        
        switch (type)
        {
          case 16:
          case 15:
            type = GL_UNSIGNED_SHORT_5_5_5_1;
            break;
          case 8:
          case 24:
          case 32:
            type = GL_UNSIGNED_BYTE;
            break;
        }
        
#if !defined(_OPENGL_ES_) && defined(_MACOSX_)
        glTexParameteri(target, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE);
        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
#endif
      }
      else
      {
        NGL_ASSERT(pSurface);
#if !defined(_OPENGL_ES_) && defined(_MACOSX_)
        internalPixelformat = pSurface->GetPixelFormat();
        if (internalPixelformat == GL_RGBA)
        {
          internalPixelformat = GL_RGBA;
          pixelformat = GL_BGRA;
          type = GL_UNSIGNED_INT_8_8_8_8_REV;
        }
        else if (internalPixelformat == GL_RGB)
        {
          internalPixelformat = GL_RGB;
          pixelformat = GL_BGR;
          type = GL_UNSIGNED_BYTE;
        }
        else
        {
          pixelformat = pSurface->GetPixelFormat();
          type = GL_UNSIGNED_BYTE;
        }
        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_FALSE);
#else
        internalPixelformat = pSurface->GetPixelFormat();
        pixelformat = pSurface->GetPixelFormat();
        type = GL_UNSIGNED_BYTE;
#endif
      }
      
      
#if (!defined _MACOSX_)
      if (!firstload)
      {
        glTexSubImage2D
        (  
         target,
         0,
         0,0,
         (int)Width,
         (int)Height,
         pixelformat,
         type,
         pBuffer
         );
        nuiCheckForGLErrors();
        pTexture->ResetForceReload();
      }
      else
#endif
      {
#ifndef _OPENGL_ES_
        if (pTexture->GetAutoMipMap())
        {
#ifdef _MACOSX_
          glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_FALSE);
          nuiCheckForGLErrors();
#endif
          glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, (int)log2(Width));
          nuiCheckForGLErrors();
          gluBuild2DMipmaps(target, internalPixelformat, (int)Width, (int)Height, pixelformat, type, pBuffer);          
          nuiCheckForGLErrors();
        }
        else
#endif
        {
          glTexImage2D(target, 0, internalPixelformat, (int)Width, (int)Height, 0, pixelformat, type, pBuffer);
        }
        nuiCheckForGLErrors();
      }
      
      info.mReload = false;
      
      if (allocated)
        free(pBuffer);

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
      if (!pTexture->IsBufferRetained())
      { 
        pTexture->ReleaseBuffer();
      }
#endif

    }
  }
  
  if (mTexEnvMode != pTexture->GetEnvMode())
  {
    mTexEnvMode = pTexture->GetEnvMode();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mTexEnvMode);
  }

  nuiCheckForGLErrors();
  
  if (changedctx)
  {
    mpContext->BeginSession();
    nuiCheckForGLErrors();
    glBindTexture(target, info.mTexture);
    nuiCheckForGLErrors();
    if (mTexEnvMode != pTexture->GetEnvMode())
    {
      mTexEnvMode = pTexture->GetEnvMode();
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mTexEnvMode);
    }
    nuiCheckForGLErrors();
  }
  
  uint32 rectangle = GetRectangleTextureSupport();
  nuiCheckForGLErrors();
  
  double rx = 1;
  double ry = 1;
  if (rectangle != 1 || pTexture->GetTextureID())
  {
    rx = pTexture->GetUnscaledWidth() / Width;
    ry = pTexture->GetUnscaledHeight() / Height;
#ifndef _OPENGL_ES_
    if (target == GL_TEXTURE_RECTANGLE_ARB)
    {
      rx *= pTexture->GetUnscaledWidth();
      ry *= pTexture->GetUnscaledHeight();
    }
#endif
  }
  
  if (pSurface)
  {
    mTextureTranslate = nglVector2f(0.0f, ry);
    ry = -ry;    
  }
  else
  {
    mTextureTranslate = nglVector2f(0.0f, 0.0f);
  }
  
  mTextureScale = nglVector2f(rx, ry);

  nuiCheckForGLErrors();
}

void nuiGL2Painter::DestroyTexture(nuiTexture* pTexture)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);
  if (info.mTexture <= 0)
    return;
  //NGL_OUT(_T("nuiGL2Painter::DestroyTexture 0x%x : '%s' / %d\n"), pTexture, pTexture->GetSource().GetChars(), info.mTexture);
  
  mpContext->BeginSession();
  glDeleteTextures(1, (GLuint*)&info.mTexture);
  mTextures.erase(it);
}

void nuiGL2Painter::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);
  info.mReload = true;
  if (!ForceReload && info.mTexture != -1)
  {
    glDeleteTextures(1, (GLuint*)&info.mTexture);
    info.mTexture = -1;
  }
}

nuiGL2Painter::FramebufferInfo::FramebufferInfo()
{
  mReload = true;
  mFramebuffer = -1;
  mRenderbuffer = -1;
  mTexture = -1;
  mDepthbuffer = -1;
  mStencilbuffer = -1;
}

void nuiGL2Painter::CreateSurface(nuiSurface* pSurface)
{
}

void nuiGL2Painter::DestroySurface(nuiSurface* pSurface)
{
  std::map<nuiSurface*, FramebufferInfo>::iterator it = mFramebuffers.find(pSurface);
  if (it == mFramebuffers.end())
  {
    return;
  }
  FramebufferInfo info = it->second;
  
  NGL_ASSERT(info.mFramebuffer >= 0);
  glDeleteFramebuffersNUI(1, (GLuint*)&info.mFramebuffer);
  if (info.mRenderbuffer >= 0)
    glDeleteRenderbuffersNUI(1, (GLuint*)&info.mRenderbuffer);
  if (info.mDepthbuffer >= 0)
    glDeleteRenderbuffersNUI(1, (GLuint*)&info.mDepthbuffer);
  if (info.mStencilbuffer >= 0)
    glDeleteRenderbuffersNUI(1, (GLuint*)&info.mStencilbuffer);
  
  mFramebuffers.erase(it);  
}

void nuiGL2Painter::InvalidateSurface(nuiSurface* pSurface, bool ForceReload)
{
}

void nuiGL2Painter::SetSurface(nuiSurface* pSurface)
{
  if (mpSurface == pSurface)
    return;
  
  if (pSurface)
    pSurface->Acquire();
  if (mpSurface)
    mpSurface->Release();
  mpSurface = pSurface;
  
  if (pSurface)
  {
    std::map<nuiSurface*, FramebufferInfo>::const_iterator it = mFramebuffers.find(pSurface);
    bool create = (it == mFramebuffers.end()) ? true : false;  
    
    GLint width = (GLint)pSurface->GetWidth();
    GLint height = (GLint)pSurface->GetHeight();
    
    nuiTexture* pTexture = pSurface->GetTexture();
    if (pTexture && !pTexture->IsPowerOfTwo())
    {
      switch (GetRectangleTextureSupport())
      {
        case 0:
          width = (GLint)pTexture->GetWidthPOT();
          height= (GLint)pTexture->GetHeightPOT();
          break;
        case 1:
        case 2:
          break;
      }
    }
    
    
    FramebufferInfo info;
    
    if (create)
    {      
      glGenFramebuffersNUI(1, (GLuint*)&info.mFramebuffer);
      //printf("glGenFramebuffersNUI -> %d\n", info.mFramebuffer);
      
      nuiCheckForGLErrors();
      glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, info.mFramebuffer);
      nuiCheckForGLErrors();
      glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, 0);
      nuiCheckForGLErrors();
      
      ///< Do we need a depth buffer
      if (pSurface->GetDepth())
      {
        glGenRenderbuffersNUI(1, (GLuint*)&info.mDepthbuffer);
        nuiCheckForGLErrors();
        glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mDepthbuffer);
        nuiCheckForGLErrors();
        
        glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI,
                                 GL_DEPTH_COMPONENT16,
                                 width, height);
        nuiCheckForGLErrors();
        
        glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, 0);
        nuiCheckForGLErrors();
        
        glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
                                     GL_DEPTH_ATTACHMENT_NUI,
                                     GL_RENDERBUFFER_NUI,
                                     info.mDepthbuffer);
        nuiCheckForGLErrors();
      }
      
      ///< Do we need a stencil buffer
      if (pSurface->GetStencil())
      {
        NGL_ASSERT(!"Stencil attachement not supported");
#ifndef _OPENGL_ES_
        glGenRenderbuffersNUI(1, (GLuint*)&info.mStencilbuffer);
        nuiCheckForGLErrors();
        
        glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mStencilbuffer);
        nuiCheckForGLErrors();
        
        glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI,
                                 GL_STENCIL_INDEX,
                                 width, height);
        nuiCheckForGLErrors();
        
        glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, 0);
        nuiCheckForGLErrors();
        
        glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
                                     GL_STENCIL_ATTACHMENT_NUI,
                                     GL_RENDERBUFFER_NUI,
                                     info.mStencilbuffer);
        nuiCheckForGLErrors();
#endif
      }
      
      ///< We definetly need a color attachement, either a texture, or a renderbuffer
      if (pTexture && pSurface->GetRenderToTexture())
      {
        GLint oldTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &oldTexture);
        
        UploadTexture(pTexture);
        
        std::map<nuiTexture*, TextureInfo>::iterator tex_it = mTextures.find(pTexture);
        NGL_ASSERT(tex_it != mTextures.end());
        TextureInfo& tex_info(tex_it->second);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        nuiCheckForGLErrors();
        
        glFramebufferTexture2DNUI(GL_FRAMEBUFFER_NUI,
                                  GL_COLOR_ATTACHMENT0_NUI,
                                  GetTextureTarget(pTexture->IsPowerOfTwo()),
                                  tex_info.mTexture,
                                  0);
        //printf("surface texture -> %d\n", tex_info.mTexture);
        nuiCheckForGLErrors();
      }
      else
      {
        glGenRenderbuffersNUI(1, (GLuint*)&info.mRenderbuffer);
        nuiCheckForGLErrors();
        
        glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mRenderbuffer);
        nuiCheckForGLErrors();
        
        GLint pixelformat = pSurface->GetPixelFormat();
        GLint internalPixelformat = pSurface->GetPixelFormat();
        internalPixelformat = GL_RGBA;
#if !defined(_OPENGL_ES_) && defined(_MACOSX_)
        if (internalPixelformat == GL_RGBA)
        {
          pixelformat = GL_BGRA;
        }
        else if (internalPixelformat == GL_RGB)
        {
          pixelformat = GL_BGR;
        }
#endif
        
        glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI, pixelformat, width, height);
        nuiCheckForGLErrors();
        
        glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
                                     GL_COLOR_ATTACHMENT0_NUI,
                                     GL_RENDERBUFFER_NUI,
                                     info.mRenderbuffer);
        //printf("surface render buffer -> %d\n", info.mRenderbuffer);
        nuiCheckForGLErrors();
      }

#ifdef DEBUG
      CheckFramebufferStatus();
#endif
      nuiCheckForGLErrors();
      mFramebuffers[pSurface] = info;
    }
    else
    {
      /// !create
      info = it->second;
      glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, info.mFramebuffer);
      if (info.mRenderbuffer >= 0)
        glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mRenderbuffer);
      //printf("glBindFramebufferNUI -> %d\n", info.mFramebuffer);
      //printf("glBindRenderbufferNUI -> %d\n", info.mRenderbuffer);
      
      nuiCheckForGLErrors();
#ifdef DEBUG
      CheckFramebufferStatus();
#endif
    }
  }
  else
  {
    /// !pSurface
    glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, mDefaultFramebuffer);
    //printf("UNBIND glBindFramebufferNUI -> %d\n", mDefaultFramebuffer);
    glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, mDefaultRenderbuffer);
    //printf("UNBIND glBindRenderbufferNUI -> %d\n", mDefaultRenderbuffer);
    
    nuiCheckForGLErrors();
#ifdef DEBUG
    CheckFramebufferStatus();
#endif
  }
}


#endif //   #ifndef __NUI_NO_GL__

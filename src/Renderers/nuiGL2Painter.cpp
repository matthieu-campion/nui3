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
uniform mat4 SurfaceMatrix;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
TexCoordVar = TexCoord * TextureScale + TextureTranslate;\n\
ColorVar = Color;\n\
gl_Position = (SurfaceMatrix * ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
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
uniform mat4 SurfaceMatrix;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
TexCoordVar = TexCoord * TextureScale + TextureTranslate;\n\
ColorVar = Color;\n\
gl_Position = (SurfaceMatrix * ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
}"
;

static const char* TextureAlphaVertexColor_FGT =
"uniform sampler2D texture;\n\
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
uniform mat4 SurfaceMatrix;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
TexCoordVar = TexCoord * TextureScale + TextureTranslate;\n\
gl_Position = (SurfaceMatrix * ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
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
uniform mat4 SurfaceMatrix;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
uniform vec2 TextureTranslate;\n\
uniform vec2 TextureScale;\n\
varying vec2 TexCoordVar;\n\
void main()\n\
{\n\
TexCoordVar = TexCoord * TextureScale + TextureTranslate;\n\
gl_Position = (SurfaceMatrix * ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
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
uniform mat4 SurfaceMatrix;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
varying vec4 ColorVar;\n\
void main()\n\
{\n\
ColorVar = Color;\n\
gl_Position = (SurfaceMatrix * ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
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
uniform mat4 SurfaceMatrix;\n\
uniform mat4 ModelViewMatrix;\n\
uniform mat4 ProjectionMatrix;\n\
uniform vec4 Offset;\n\
void main()\n\
{\n\
gl_Position = (SurfaceMatrix * ProjectionMatrix * ModelViewMatrix * (Position  + Offset));\n\
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

static uint32 mins = 30000;
static uint32 maxs = 0;
static uint32 totalinframe = 0;
static uint32 total = 0;

nuiGL2Painter::nuiGL2Painter(nglContext* pContext)
: nuiGLPainter(pContext)
{
  mUseShaders = true;

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
}

nuiGL2Painter::~nuiGL2Painter()
{
  mpShader_TextureVertexColor->Release();
  mpShader_TextureDifuseColor->Release();
  mpShader_TextureAlphaVertexColor->Release();
  mpShader_TextureAlphaDifuseColor->Release();
  mpShader_DifuseColor->Release();
  mpShader_VertexColor->Release();
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

void nuiGL2Painter::ResetOpenGLState()
{
  BeginSession();
  //NUI_RETURN_IF_RENDERING_DISABLED;
  nuiCheckForGLErrors();
  
  mScissorX = -1;
  mScissorY = -1;
  mScissorW = -1;
  mScissorH = -1;
  mScissorOn = false;
  

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

//  mTextureTranslate = nglVector2f(0.0f, 0.0f);
//  mTextureScale = nglVector2f(1, 1);

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
  if (mState.mpShader == NULL)
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
    mState.mpShader = pShader;
    mState.mShaderState = pShader->GetDefaultState();
  }

  NGL_ASSERT(mState.mpShader != NULL);

  ApplyState(mState, mForceApply);

  if (mFinalState.mpTexture && pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    if (mTextureScale[1] < 0)
    {
      NGL_ASSERT(mFinalState.mpTexture->GetSurface() != NULL);
//      printf("REVERSED SURFACE TEXTURE");
    }
    mFinalState.mpShader->GetDefaultState().Set("TextureTranslate", mTextureTranslate, false);
    mFinalState.mpShader->GetDefaultState().Set("TextureScale", mTextureScale, false);
  }

  nglMatrixf m;
  if (mpSurface)
  {
    m.SetScaling(1.0f, -1.0f, 1.0f);
  }
  else
  {
    m.SetIdentity();
  }
  
  mFinalState.mShaderState.Set("SurfaceMatrix", m, false);
  mFinalState.mShaderState.SetProjectionMatrix(mProjectionMatrixStack.top(), false);
  mFinalState.mShaderState.SetModelViewMatrix(mMatrixStack.top(), false);
  mFinalState.mShaderState.Apply();

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

  if (mpSurface)
    mFinalState.mpShader->GetDefaultState().Set("Offset", -hackX, -hackY, true);
  else
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



#endif //   #ifndef __NUI_NO_GL__

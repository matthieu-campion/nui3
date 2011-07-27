/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nui.h"
#include "nuiGLPainter.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nglMatrix.h"
#include "AAPrimitives.h"
#include "nuiTexture.h"

float NUI_SCALE_FACTOR = 1.0f;
float NUI_INV_SCALE_FACTOR = 1.0f / NUI_SCALE_FACTOR;

#ifndef __NUI_NO_GL__

//#define NUI_RETURN_IF_RENDERING_DISABLED return;
#define NUI_RETURN_IF_RENDERING_DISABLED
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6

//#define NUI_USE_ANTIALIASING
#ifdef NUI_USE_ANTIALIASING
#define NUI_USE_MULTISAMPLE_AA
#endif


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

void nuiGLLoadMatrix(const float* pMatrix)
{
  glLoadMatrixf(pMatrix);
}

void nuiGLLoadMatrix(const double* pMatrix)
{
#ifndef _OPENGL_ES_
  glLoadMatrixd(pMatrix);
#else
  NGL_ASSERT(!"no glLoadMatrixd in gles");
#endif
}



void nuiGLPainter::BlendFuncSeparate(GLenum src, GLenum dst, GLenum srcalpha, GLenum dstalpha)
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


bool nuiGLPainter::CheckFramebufferStatus()
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

uint32 nuiGLPainter::mActiveContexts = 0;

static uint32 mins = 30000;
static uint32 maxs = 0;
static uint32 totalinframe = 0;
static uint32 total = 0;

nuiGLPainter::nuiGLPainter(nglContext* pContext, const nuiRect& rRect)
: nuiPainter(rRect, pContext)
{
  mCanRectangleTexture = 0;
  mTextureTarget = GL_TEXTURE_2D;
  mTwoPassBlend = false;
  mDefaultFramebuffer = 0;
  mDefaultRenderbuffer = 0;
  mForceApply = false;
  mClientVertex = false;
  mClientColor = false;
  mClientTexCoord = false;
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
    //NGL_OUT(_T("Extensions: %ls\n"), exts.GetChars());
    
    
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
  //  mDefaultFramebuffer = 0;
  //  mDefaultRenderbuffer = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_NUI, &mDefaultFramebuffer);
  glGetIntegerv(GL_RENDERBUFFER_BINDING_NUI, (GLint *) &mDefaultRenderbuffer);
#endif
  
  mActiveContexts++;
}

nuiGLPainter::~nuiGLPainter()
{
  mActiveContexts--;
  if (mActiveContexts == 0)
    glAAExit();
}

void nuiGLPainter::SetViewport()
{
  //GetAngle(), GetCurrentWidth(), GetCurrentHeight(), mProjectionViewportStack.top(), mProjectionMatrixStack.top());
  GLuint Angle = GetAngle();
  GLuint Width = GetCurrentWidth();
  GLuint Height = GetCurrentHeight();
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
    //glViewport(x * NUI_SCALE_FACTOR, y * NUI_SCALE_FACTOR, w * NUI_SCALE_FACTOR, h * NUI_SCALE_FACTOR);
    x *= NUI_SCALE_FACTOR;
    y *= NUI_SCALE_FACTOR;
    w *= NUI_SCALE_FACTOR;
    h *= NUI_SCALE_FACTOR;
  }
//  else
//    glViewport(x, y, w, h);

//  if (mViewPort[0] != x || mViewPort[1] != y || mViewPort[2] != w || mViewPort[3] != h)
  {
    mViewPort[0] = x;
    mViewPort[1] = y;
    mViewPort[2] = w;
    mViewPort[3] = h;
    glViewport(mViewPort[0], mViewPort[1], mViewPort[2], mViewPort[3]);
  }
  
  nuiCheckForGLErrors();
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glScalef(NUI_SCALE_FACTOR, NUI_SCALE_FACTOR, 1.0f);
  if (Angle != 0.0f)
  {
    glRotatef(Angle, 0.f,0.f,1.f);
    glMultMatrixf(rMatrix.Array);
  }
  else
  {
    glLoadMatrixf(rMatrix.Array);
  }
  
  nuiCheckForGLErrors();
  
  glMatrixMode (GL_MODELVIEW);
  nuiCheckForGLErrors();
}

void nuiGLPainter::StartRendering()
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
  glLoadIdentity();
  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_TEXTURE_2D);
  
#ifndef _OPENGL_ES_
  if (mCanRectangleTexture == 2)
  {
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
  }
#endif
  
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  BlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  
  //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
  
  glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
  
  glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
  
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_ALPHA);
 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
  
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
  
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  mClientVertex = false;
  mClientColor = false;
  mClientTexCoord = false;
  mMatrixChanged = true;
  mR = -1;
  mG = -1;
  mB = -1;
  mA = -1;
  mTexEnvMode = 0;
  
  nuiCheckForGLErrors();
}

void nuiGLPainter::ApplyState(const nuiRenderState& rState, bool ForceApply)
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
        x *= NUI_SCALE_FACTOR;
        y *= NUI_SCALE_FACTOR;
        w *= NUI_SCALE_FACTOR;
        h *= NUI_SCALE_FACTOR;
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

void nuiGLPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  //TEST_FBO_CREATION();
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  mState = rState;
  mForceApply |= ForceApply;
  ApplyState(rState, ForceApply);
}

void nuiGLPainter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  mWidth = w;
  mHeight = h;
}

void nuiGLPainter::ApplyTexture(const nuiRenderState& rState, bool ForceApply)
{
  //  if ((rState.mTexturing && !rState.mpTexture) || (!rState.mTexturing && rState.mpTexture))
  //  {
  //    printf("bleh!\n");
  //    char* bleh = NULL;
  //    bleh[0] = 0;
  //  }
  
  // 2D Textures: 
  std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
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
    
    //NGL_OUT(_T("Change texture to 0x%x (%ls)\n"), rState.mpTexture, rState.mpTexture?rState.mpTexture->GetSource().GetChars() : nglString::Empty.GetChars());
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
          pSurface->Realize(&Ctx);
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
        glDisable(outtarget);
        nuiCheckForGLErrors();
      }
      //NGL_OUT(_T("disable outtarget\n"));
      if (intarget && mFinalState.mTexturing && mFinalState.mpTexture)
      {
        mFinalState.mTexturing = rState.mTexturing;
        //NGL_OUT(_T("enable intarget\n"));
        glEnable(intarget);
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
          glEnable(mTextureTarget);
          nuiCheckForGLErrors();
        }
        else
        {
          glDisable(mTextureTarget);
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
        glEnable(mTextureTarget);
        nuiCheckForGLErrors();
      }
      else
      {
        //NGL_OUT(_T("Disable 0x%x\n"), mTextureTarget);
        glDisable(mTextureTarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      if (mTextureTarget)
      {
        //NGL_OUT(_T("Disable 0x%x\n"), mTextureTarget);
        glDisable(mTextureTarget);
      }
      nuiCheckForGLErrors();
    }
  }
}


void nuiGLPainter::ClearColor()
{
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  glClearColor(mState.mClearColor.Red(),mState.mClearColor.Green(),mState.mClearColor.Blue(),mState.mClearColor.Alpha());
  glClear(GL_COLOR_BUFFER_BIT);
  nuiCheckForGLErrors();
}

/*
 void nuiGLPainter::BlurRect(const nuiRect& rRect, uint Strength)
 {
 nuiRect Rect = rRect;
 if (mClippingRect.mEnabled)
 Rect.Intersect(mClippingRect,rRect);
 nuiRect size = Rect.Size();
 
 nuiTexture* pScratchPad = GetScratchPad(ToZero(size.GetWidth()), ToZero(size.GetHeight()));
 
 if (!pScratchPad)
 return;
 
 SetTexture(pScratchPad);
 
 glPushMatrix();
 glLoadIdentity();
 
 EnableBlending(true);
 EnableTexturing(true);
 SetBlendFunc(nuiBlendTransp);
 
 do
 {
 glCopyTexSubImage2D(
 GL_TEXTURE_2D, 0, 
 0, 0, 
 ToZero(rRect.mLeft), ToZero(mHeight) - 1 - ToZero(rRect.mTop) - ToZero(rRect.GetHeight()), 
 ToZero(rRect.GetWidth()), ToZero(rRect.GetHeight())
 );
 
 SetFillColor(nuiColor(1,1,1,.15f));
 nuiRect rect = Rect;
 
 rect.Move(-1,-1);
 DrawImage(rect,size);
 rect.Move(1,0);
 DrawImage(rect,size);
 rect.Move(1,0);
 DrawImage(rect,size);
 
 rect.Move(-2,1);
 DrawImage(rect,size);
 rect.Move(1,0);
 DrawImage(rect,size);
 rect.Move(1,0);
 DrawImage(rect,size);
 
 rect.Move(-2,1);
 DrawImage(rect,size);
 rect.Move(0,1);
 DrawImage(rect,size);
 rect.Move(0,1);
 DrawImage(rect,size);
 } while ((long)(Strength--) > 0);
 
 EnableBlending(false);
 EnableTexturing(false);
 
 glPopMatrix();
 }
 */


#define LOGENUM(XXX) case XXX: { NGL_OUT(_T("%ls\n"), #XXX); } break;

void nuiGLPainter::DrawArray(nuiRenderArray* pArray)
{
  mRenderOperations++;
  mBatches++;
  
  //glEnable(GL_TEXTURE_2D);
  //glEnable(GL_TEXTURE_2D);
  
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
  //  else
  //  {
  //    nglVectorf v1(bounds[0], bounds[1], 0); // topleft(x, y)
  //    nglVectorf v2(bounds[3], bounds[4], 0); // bottomright(x, y)
  //    v1 = rM * v1;
  //    v2 = rM * v2;
  //    
  //    if (
  //        (v1[0] > mClip.Right()) ||
  //        (v1[1] > mClip.Bottom()) ||
  //        (v2[0] < mClip.Left()) ||
  //        (v2[1] < mClip.Top())
  //       )
  //    {
  //      return;
  //    }
  //  }
  
  uint32 s = pArray->GetSize();
  
  total += s;
  totalinframe += s;
  mins = MIN(mins, s);
  maxs = MAX(maxs, s);
  //printf("DA (%d) min = %d  max = %d\n", s, mins, maxs);
  
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
  
  //ApplyState(mState, mForceApply);
  
  mVertices += s;
  GLenum mode = pArray->GetMode();
  //NGL_OUT(_T("GL Array Mode: %d   vertice count %d\n"), mode, size);
  
  /*
   switch (pArray->GetMode())
   {
   LOGENUM(GL_POINTS);
   LOGENUM(GL_LINES);
   LOGENUM(GL_LINE_LOOP);
   LOGENUM(GL_LINE_STRIP);
   //LOGENUM(GL_TRIANGLES);
   LOGENUM(GL_TRIANGLE_STRIP);
   LOGENUM(GL_TRIANGLE_FAN);
   //LOGENUM(GL_QUADS);
   LOGENUM(GL_QUAD_STRIP);
   LOGENUM(GL_POLYGON);
   }
   */
  
  
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  if (mMatrixChanged)
  {
    nuiGLLoadMatrix(mMatrixStack.top().Array);
    mMatrixChanged = false;
  }
  
  
  bool NeedTranslateHack = pArray->IsShape() || ((mode == GL_POINTS || mode == GL_LINES || mode == GL_LINE_LOOP || mode == GL_LINE_STRIP) && !pArray->Is3DMesh());
  float hackX;
  float hackY;
  if (NeedTranslateHack)
  {
    //    const float ratio=0.5f;
    const float ratio= NUI_INV_SCALE_FACTOR/2.f;
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
    glTranslatef(hackX, hackY, 0);
  }
  
  
#ifdef NUI_USE_ANTIALIASING
  if (mFinalState.mAntialiasing)
  {
#ifdef NUI_USE_MULTISAMPLE_AA
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_MULTISAMPLE_ARB);
    nuiCheckForGLErrors();
#else
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    BlendFuncSeparate(GL_SRC_ALPHA_SATURATE, GL_ONE);
    nuiCheckForGLErrors();
#endif
  }
#endif // NUI_USE_ANTIALIASING
  
  if (pArray->IsArrayEnabled(nuiRenderArray::eVertex))
  {
    if (!mClientVertex)
      glEnableClientState(GL_VERTEX_ARRAY);
    mClientVertex = true;
    glVertexPointer(3, GL_FLOAT, sizeof(nuiRenderArray::Vertex), &pArray->GetVertices()[0].mX);
    nuiCheckForGLErrors();
  }
  else
  {
    if (mClientVertex)
      glDisableClientState(GL_VERTEX_ARRAY);
    mClientVertex = false;
  }
  
  float r = mR, g = mG, b = mB, a = mA;
  if (pArray->IsArrayEnabled(nuiRenderArray::eColor))
  {
    if (!mClientColor)
      glEnableClientState(GL_COLOR_ARRAY);
    mClientColor = true;
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(nuiRenderArray::Vertex), &pArray->GetVertices()[0].mR);
    nuiCheckForGLErrors();
  }
  else
  {
    if (mClientColor)
      glDisableClientState(GL_COLOR_ARRAY);
    mClientColor = false;
    
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
#ifndef _OPENGL_ES_
      case GL_QUADS:
      case GL_QUAD_STRIP:
      case GL_POLYGON:
#endif
        c = mFinalState.mFillColor;
        break;
    }
    
    r = c.Red();
    g = c.Green();
    b = c.Blue();
    a = c.Alpha();
    nuiCheckForGLErrors();
  }
  
  if (mR != r || mG != g || mB != b || mA != a)
  {
    glColor4f(r, g, b, a);
    mR = r;
    mG = g;
    mB = b;
    mA = a;
  }
  
  if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    if (!mClientTexCoord)
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    mClientTexCoord = true;
    glTexCoordPointer(2, GL_FLOAT, sizeof(nuiRenderArray::Vertex), &pArray->GetVertices()[0].mTX);
    nuiCheckForGLErrors();
  }
  else
  {
    if (mClientTexCoord)
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    mClientTexCoord = false;
  }
  
  /*
   if (pArray->IsArrayEnabled(nuiRenderArray::eNormal))
   {
   glEnableClientState(GL_NORMAL_ARRAY);
   glNormalPointer(GL_FLOAT, sizeof(nuiRenderArray::Vertex)-12, &pArray->GetVertices()[0].mNX);
   nuiCheckForGLErrors();
   }
   else
   glDisableClientState(GL_NORMAL_ARRAY);
   */
  
  /*
   if (pArray->IsArrayEnabled(nuiRenderArray::eEdgeFlag))
   {
   glEnableClientState(GL_EDGE_FLAG_ARRAY);
   glEdgeFlagPointer(sizeof(nuiRenderArray::Vertex), &pArray->GetVertices()[0].mEdgeFlag);
   nuiCheckForGLErrors();
   }
   else
   glDisableClientState(GL_EDGE_FLAG_ARRAY);
   */
  
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
#ifdef _UIKIT_
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
#ifdef _UIKIT_
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
#ifdef _UIKIT_
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
#else
        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
#endif
      }
    }
    nuiCheckForGLErrors();
  }
  
  
#ifdef NUI_USE_ANTIALIASING
  if (mFinalState.mAntialiasing)
  {
#ifdef NUI_USE_MULTISAMPLE_AA
    glDisable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    BlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
    nuiCheckForGLErrors();
  }
  else
#endif // NUI_USE_ANTIALIASING
  {
    //#TEST meeloo disabling AA texture
    //     if (pArray->UseGLAATexture())
    //     {
    //       glMatrixMode(GL_TEXTURE);
    //       glPopMatrix();
    //       glMatrixMode(GL_MODELVIEW);
    //       glPopMatrix();
    // 
    //       if (mFinalState.mpTexture && mFinalState.mTexturing)
    //       {
    //         if (mTextureTarget != GL_TEXTURE_2D)
    //         {
    //           glDisable(GL_TEXTURE_2D);
    //           glEnable(mTextureTarget);
    //         }
    // 
    //         UploadTexture(mFinalState.mpTexture);
    //       }
    //       else
    //       {
    //         glDisable(GL_TEXTURE_2D);
    //       }
    // 
    //       if (!mFinalState.mBlending)
    //         glDisable(GL_BLEND);
    //       if (mFinalState.mBlendFunc != nuiBlendTransp)
    //       {
    //         GLenum src, dst;
    //         nuiGetBlendFuncFactors(mFinalState.mBlendFunc, src, dst);
    //         glBlendFunc(src, dst);
    //       }
    //       //ApplyTexture(mState, true);
    //     }
    //     else
    
    if (NeedTranslateHack)
      glTranslatef(-hackX, -hackY, 0);
  }
  
  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  
  pArray->Release();
  nuiCheckForGLErrors();
}

void nuiGLPainter::BeginSession()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  mpContext->MakeCurrent();
  nuiCheckForGLErrors();
  
  totalinframe = 0;
}

void nuiGLPainter::EndSession()
{
  // Bleh!
  NUI_RETURN_IF_RENDERING_DISABLED;
  //printf("min = %d max = %d total in frame = %d total = %d\n", mins, maxs, totalinframe, total);
}

void nuiGLPainter::LoadMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::LoadMatrix(rMatrix);
  //nuiGLLoadMatrix(rMatrix.Array);
  //nuiGLLoadMatrix(mMatrixStack.top().Array);
  
  mMatrixChanged = true;
  
  nuiCheckForGLErrors();
}

void nuiGLPainter::MultMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::MultMatrix(rMatrix);
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

void nuiGLPainter::PushMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PushMatrix();
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

void nuiGLPainter::PopMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PopMatrix();
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}







void nuiGLPainter::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::LoadProjectionMatrix(rViewport, rMatrix);
  
  SetViewport();
}

void nuiGLPainter::MultProjectionMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::MultProjectionMatrix(rMatrix);
  
  SetViewport();
}

void nuiGLPainter::PushProjectionMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PushProjectionMatrix();
}

void nuiGLPainter::PopProjectionMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PopProjectionMatrix();
  SetViewport();
}



#ifdef glDeleteBuffersARB
#undef glDeleteBuffersARB
#endif

void nuiGLPainter::ReleaseCacheObject(void* pHandle)
{
  /* Not 64 bit-safe and 'dead code' as Meeloo said
   GLuint array = (GLuint)pHandle;
   mpContext->glDeleteBuffersARB(1, &array);
   nuiCheckForGLErrors();
   */
}

uint32 nuiGLPainter::GetRectangleTextureSupport() const
{
  return mCanRectangleTexture;
}

GLenum nuiGLPainter::GetTextureTarget(bool POT) const
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

nuiGLPainter::TextureInfo::TextureInfo()
{
  mReload = false;
  mTexture = -1;
}

void nuiGLPainter::CreateTexture(nuiTexture* pTexture)
{
  
}

void nuiGLPainter::UploadTexture(nuiTexture* pTexture)
{
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
  
  GLuint id = pTexture->GetTextureID();
  if (id)
  {
    info.mReload = false;
    info.mTexture = id;
    target = pTexture->GetTarget();
    printf("upload texture %d %x\n", id, target);
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
    if (info.mTexture == (GLuint)-1)
    { // Generate a texture
      //      if (mpSharedContext)
      //      {
      //        mpSharedContext->MakeCurrent();
      //        nuiCheckForGLErrors();
      //        changedctx = true;
      //      }
      
      glGenTextures(1, &info.mTexture);
      //NGL_OUT(_T("nuiGLPainter::UploadTexture 0x%x : '%ls' / %d\n"), pTexture, pTexture->GetSource().GetChars(), info.mTexture);
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
        
#ifndef NUI_IOS
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
      
      
#ifndef _MACOSX_
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
      #ifdef _UIKIT_
      if (!pTexture->IsBufferRetained()) 
      { 
        pTexture->ReleaseBuffer();
      }
      #endif
      
    }
  }
  
  if (pTexture->GetPixelFormat() == eImagePixelAlpha)
  {
    if (mTexEnvMode != GL_COMBINE)
    {
      mTexEnvMode = GL_COMBINE;
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mTexEnvMode);
    }
  }
  else
  {
    if (mTexEnvMode != pTexture->GetEnvMode())
    {
      mTexEnvMode = pTexture->GetEnvMode();
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mTexEnvMode);
    }
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
  
  glMatrixMode(GL_TEXTURE);
  nuiCheckForGLErrors();
  glLoadIdentity();
  nuiCheckForGLErrors();
  
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
    glTranslatef(0, ry, 0);
    ry = -ry;    
  }
  
#ifndef _OPENGL_ES_
  glScaled(rx, ry, 1);
#else
  glScalef((float)rx, (float)ry, 1);
#endif
  nuiCheckForGLErrors();
  
  glMatrixMode(GL_MODELVIEW);
  nuiCheckForGLErrors();
}

void nuiGLPainter::DestroyTexture(nuiTexture* pTexture)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);
  if (info.mTexture <= 0)
    return;
  //NGL_OUT(_T("nuiGLPainter::DestroyTexture 0x%x : '%ls' / %d\n"), pTexture, pTexture->GetSource().GetChars(), info.mTexture);
  
  mpContext->BeginSession();
  glDeleteTextures(1, &info.mTexture);
  mTextures.erase(it);
}

void nuiGLPainter::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);
  info.mReload = true;
  if (!ForceReload && info.mTexture != -1)
  {
    glDeleteTextures(1, &info.mTexture);
    info.mTexture = -1;
  }
}

nuiGLPainter::FramebufferInfo::FramebufferInfo()
{
  mReload = true;
  mFramebuffer = 0;
  mRenderbuffer = 0;
  mTexture = 0;
  mDepthbuffer = 0;
  mStencilbuffer = 0;
}

void nuiGLPainter::CreateSurface(nuiSurface* pSurface)
{
  
}

void nuiGLPainter::DestroySurface(nuiSurface* pSurface)
{
  std::map<nuiSurface*, FramebufferInfo>::iterator it = mFramebuffers.find(pSurface);
  if (it == mFramebuffers.end())
  {
    return;
  }
  FramebufferInfo info = it->second;
  
  NGL_ASSERT(info.mFramebuffer > 0);
  glDeleteFramebuffersNUI(1, &info.mFramebuffer);
  if (info.mRenderbuffer > 0)
    glDeleteRenderbuffersNUI(1, &info.mRenderbuffer);
  if (info.mDepthbuffer > 0)
    glDeleteRenderbuffersNUI(1, &info.mDepthbuffer);
  if (info.mStencilbuffer > 0)
    glDeleteRenderbuffersNUI(1, &info.mStencilbuffer);
  
  mFramebuffers.erase(it);  
}

void nuiGLPainter::InvalidateSurface(nuiSurface* pSurface, bool ForceReload)
{
  
}

void nuiGLPainter::SetSurface(nuiSurface* pSurface)
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
    
    GLuint width = (GLuint)pSurface->GetWidth();
    GLuint height = (GLuint)pSurface->GetHeight();
    
    nuiTexture* pTexture = pSurface->GetTexture();
    if (pTexture && !pTexture->IsPowerOfTwo())
    {
      switch (GetRectangleTextureSupport())
      {
        case 0:
          width = (GLuint)pTexture->GetWidthPOT();
          height= (GLuint)pTexture->GetHeightPOT();
          break;
        case 1:
        case 2:
          break;
      }
    }
    
    
    FramebufferInfo info;
    
    if (create)
    {      
      glGenFramebuffersNUI(1, &info.mFramebuffer);
      //printf("glGenFramebuffersNUI -> %d\n", info.mFramebuffer);
      
      nuiCheckForGLErrors();
      glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, info.mFramebuffer);
      nuiCheckForGLErrors();
      glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, 0);
      nuiCheckForGLErrors();
      
      ///< Do we need a depth buffer
      if (pSurface->GetDepth())
      {
        glGenRenderbuffersNUI(1, &info.mDepthbuffer);
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
        glGenRenderbuffersNUI(1, &info.mStencilbuffer);
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
        
        //        glBindTexture(GL_TEXTURE_2D, oldTexture);
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
        glGenRenderbuffersNUI(1, &info.mRenderbuffer);
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

void nuiCheckForGLErrors()
{
#if 0 // Globally enable/disable OpenGL error checking
#ifdef _DEBUG_
  bool error = false;
  GLenum err = glGetError();
  while (err != GL_NO_ERROR)
  {
    switch (err)
    {
        /*
         case GL_NO_ERROR:
         NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, "error has been recorded. The value of this symbolic constant is guaranteed to be zero.");
         */
        break;
      case GL_INVALID_ENUM: 
        NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, _T("An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag."));
        NGL_ASSERT(0);
        break;
      case GL_INVALID_VALUE: 
        NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, _T("A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag."));
        NGL_ASSERT(0);
        break;
      case GL_INVALID_OPERATION:
        NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, _T("The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag."));
        //NGL_ASSERT(0);
        break;
      case GL_STACK_OVERFLOW:
        NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, _T("This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag."));
        NGL_ASSERT(0);
        break;
      case GL_STACK_UNDERFLOW:
        NGL_ASSERT(0);
        NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, _T("This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag."));
        NGL_ASSERT(0);
        break;
      case GL_OUT_OF_MEMORY:
        NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ERROR, _T("There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded."));
        NGL_ASSERT(0);
        break;
    }
    err = glGetError();
  }
#endif
#endif
}


#endif //   #ifndef __NUI_NO_GL__
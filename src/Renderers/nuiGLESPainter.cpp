/*
NUI - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 S√àbastien M√àtrot

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nuiGLESPainter.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nglMatrix.h"
#include "nuiTexture.h"


#ifndef __NUI_NO_GLES__

#define NUI_RETURN_IF_RENDERING_DISABLED
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6

//#define NUI_USE_ANTIALIASING
#ifdef NUI_USE_ANTIALIASING
#define NUI_USE_MULTISAMPLE_AA
#endif

uint32 nuiGLESPainter::mActiveContexts = 0;

static uint32 mins = 30000;
static uint32 maxs = 0;
static uint32 totalinframe = 0;
static uint32 total = 0;


nuiGLESPainter::nuiGLESPainter(nglContext* pContext, const nuiRect& rRect)
: nuiPainter(rRect, pContext)
{
//NGL_OUT(_T("[nuiGLESPainter::nuiGLESPainter] NEW %#x\n"), this);
  mCanRectangleTexture = 0;
  mTextureTarget = GL_TEXTURE_2D;

  mpContext = pContext;
  if (mpContext)
  {
    mpContext->BeginSession();
    nuiCheckForGLErrors();
  }

  mActiveContexts++;
}

nuiGLESPainter::~nuiGLESPainter()
{
  mActiveContexts--;
  //if (mActiveContexts == 0)
    //glAAExit();
}


void nuiGLESPainter::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  BeginSession();
  nuiCheckForGLErrors();

  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::StartRendering(ClipOffsetX, ClipOffsetY);

  glMatrixMode(GL_PROJECTION);
  glViewport(0,0, mWidth, mHeight);

  nuiCheckForGLErrors();
  glLoadIdentity();
  //  glScalef (1, -1, 1);
  glTranslatef(-1.0f, 1.0f, 0.0f );

  glScalef (2.0f/(float)mWidth, -2.0f/(float)mHeight, 1.0f);

  nuiCheckForGLErrors();

  if (mDrawOrigin == nuiRight) {
    glTranslatef(mWidth, 0.f, 0.f);
    glRotatef(90.f, 0.f,0.f,1.f);
  }
  else if (mDrawOrigin == nuiLeft) {
    glTranslatef(0.f, mHeight, 0.f);
    glRotatef(-90.f, 0.f,0.f,1.f);
  }
  else if (mDrawOrigin == nuiBottom) {
    glTranslatef(mWidth, mHeight, 0.f);
    glRotatef(180.f, 0.f,0.f,1.f);
  }
  
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_TEXTURE_2D);
/*  
  if (mCanRectangleTexture == 2)
  {
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
  }
*/
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  nuiCheckForGLErrors();
}

/*
void nuiGLESPainter::BeginSession()
{ 
  mpContext->BeginSession();
}


void nuiGLESPainter::StopRendering()
{
  //glFlush();

}
*/


void nuiGLESPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiCheckForGLErrors();

  // blending
  if (ForceApply || mState.mBlending != rState.mBlending)
  {
    mState.mBlending = rState.mBlending;
    if (mState.mBlending)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }
  }

  if (ForceApply || mState.mBlendFunc != rState.mBlendFunc)
  {
    mState.mBlendFunc = rState.mBlendFunc;
    GLenum src, dst;
    nuiGetBlendFuncFactors(rState.mBlendFunc, src, dst);
    glBlendFunc(src, dst);
    nuiCheckForGLErrors();
  }

  // Stencil test:
  if (ForceApply || mState.mStencilMode != rState.mStencilMode || mState.mStencilValue != rState.mStencilValue)
  {
    mState.mStencilMode = rState.mStencilMode;
    mState.mStencilValue = rState.mStencilValue;
    switch (mState.mStencilMode)
    {
    case nuiIgnoreStencil:
      //NGL_OUT(_T("nuiIgnoreStencil Value %d\n"), mState.mStencilValue);
      glDisable(GL_STENCIL_TEST);
      glStencilMask(0);
      glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      break;
    case nuiReadStencil:
      //NGL_OUT(_T("nuiReadStencil Value %d\n"), mState.mStencilValue);
      glEnable(GL_STENCIL_TEST);
      glStencilMask(0);
      glStencilFunc(GL_EQUAL, mState.mStencilValue, 255);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      break;
    case nuiAddToStencil:
      //NGL_OUT(_T("nuiAddToStencil Value %d\n"), mState.mStencilValue);
      glEnable(GL_STENCIL_TEST);
      glStencilMask(~0);
      glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      break;
    case nuiBlendToStencil:
      //NGL_OUT(_T("nuiBlendToStencil Value %d\n"), mState.mStencilValue);
      glEnable(GL_STENCIL_TEST);
      glStencilMask(~0);
      glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      glStencilOp(GL_INCR, GL_INCR, GL_INCR);
      break;
    default:
      NGL_ASSERT(false);
      break;
    }
    nuiCheckForGLErrors();
  }

  // We don't care about the font in the lower layer of rendering
  //nuiFont* mpFont;
  // 
/*
  if (ForceApply || mState.mpShader != rState.mpShader)
  {
    if (mState.mpShader)
      mState.mpShader->UnBind();
    nuiCheckForGLErrors();

    mState.mpShader = rState.mpShader; 

    if (mState.mpShader)
      mState.mpShader->Bind();
    nuiCheckForGLErrors();
  }
*/
  ApplyTexture(rState, ForceApply);

  // Rendering buffers:
  if (ForceApply || mState.mColorBuffer != rState.mColorBuffer)
  {
    mState.mColorBuffer = rState.mColorBuffer;
    GLboolean m = mState.mColorBuffer?GL_TRUE:GL_FALSE;
    glColorMask(m, m, m, m);
    nuiCheckForGLErrors();
  }


  if (mClip.mEnabled)    
  {
    glEnable(GL_SCISSOR_TEST);
    nuiRect clip(mClip);
    clip.Move(-mClipOffsetX, -mClipOffsetY);
      
    float x,y,w,h;
    
    if (mDrawOrigin == nuiLeft) {
      x = clip.Top();
      y = clip.Left();
      w = ToBelow(clip.GetHeight());
      h = ToBelow(clip.GetWidth());
    }
    else if (mDrawOrigin == nuiRight) {
      x = mWidth - clip.Bottom();
      y = mHeight - clip.Right();
      w = ToBelow(clip.GetHeight());
      h = ToBelow(clip.GetWidth());
    }
    else if (mDrawOrigin == nuiBottom) {
      x = mWidth - clip.Right();
      y = clip.Top();
      w = ToBelow(clip.GetWidth());
      h = ToBelow(clip.GetHeight());
    }
    else { //(mDrawOrigin == nuiTop)
      x = clip.Left();
      y = mHeight - clip.Bottom();
      w = ToBelow(clip.GetWidth());
      h = ToBelow(clip.GetHeight());
    }
    
//  NGL_OUT(_T("Clip {%f, %f, %f, %f}\n"), x, y, w, h);
    glScissor(x, y, w, h);
    nuiCheckForGLErrors();
  }
  else
    glDisable(GL_SCISSOR_TEST);

  mState.mClearColor = rState.mClearColor;
  mState.mStrokeColor = rState.mStrokeColor;
  mState.mFillColor = rState.mFillColor;

  nuiCheckForGLErrors();
}

void nuiGLESPainter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  mWidth = w;
  mHeight = h;

  glViewport (0, 0, w, h);
  nuiCheckForGLErrors();
}

/*
void nuiGLESPainter::ApplySurface(const nuiRenderState& rState)
{
  NGL_ASSERT(rState.mpSurface);

  std::map<nuiSurface*, FramebufferInfo>::const_iterator it = mFramebuffers.find(rState.mpSurface);
  bool create = (it == mSurfaces.end()) ? true : false;  

  nuiSize width = rState.mpSurface->GetWidth();
  nuiSize height = rState.mpSurface->GetHeight();
  
  if (create)
  {
    FramebufferInfo info;
     
    glGenFramebuffersOES(1, &info.mFramebuffer);
    glGenTextures(1, &info.mTexture);

///< Do we need a depth buffer
    if (rState.mpSurface->IsDepthEnabled())
      glGenRenderbuffersOES(1, &mDepthBuffer);
    else
      info.mDepthbuffer = (GLuint)-1;

///< Do we need a stencil buffer
    if (rState.mpSurface->IsStencilEnabled())
      glGenRenderbuffersOES(1, &info.mStencilBuffer);
    else
      info.mStencilbuffer = (GLuint)-1;
    
///< bind buffers to framebuffer
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, info.mTexture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      width, height,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      NULL ///< Doesn't upload data
    );

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFrameBuffer);

    glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES,
                              GL_COLOR_ATTACHMENT0_OES,
                              GL_TEXTURE_2D,
                              info.mTexture,
                              0);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    mFramebuffers[rState.mpSurface] = info;
    it = mFramebuffers.find(rState.mpSurface);
    NGL_ASSERT(it != mFramebuffers.end());
  }

///< Bind

//	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &mOldRenderBuffer);
//	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &mOldFrameBuffer);
//  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &mOldTexture);
  FramebufferInfo& rInfo = it->second;

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, rInfo.mTexture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA,
    width, height, 0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    NULL
  );

//glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, rInfo.mFramebuffer);

  glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES,
                            GL_COLOR_ATTACHMENT0_OES,
                    	      GL_TEXTURE_2D,
                            rInfo.mTexture,
                            NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  GLint completeness = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
  switch (completeness) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES: {
      NGL_DEBUG(_T("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES\n"));
    } break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES: {
      NGL_DEBUG(_T("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES\n"));
    } break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES: {
      NGL_DEBUG(_T("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES\n"));
    } break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES: {
      NGL_DEBUG(_T("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES\n"));
    } break;
    case GL_FRAMEBUFFER_UNSUPPORTED_OES: {
      NGL_DEBUG(_T("GL_FRAMEBUFFER_UNSUPPORTED_OES\n"));
    } break;
    default: {
      NGL_ASSERT(completeness == GL_FRAMEBUFFER_COMPLETE_OES);
    }
  }
  glViewport(0,0, width, height);
}

void nuiGLESPainter::DestroySurface(nuiSurface* pSurface, bool DestroyTexture)
{
  std::map<nuiSurface*, FramebufferInfo>::const_iterator it = mFramebuffers.find(rState.mpSurface);
  if (it == mFramebuffers.end()) {
    return;
  }
  FramebufferInfo& rInfo = it->second;

  NGL_ASSERT(rInfo.mFramebuffer >= 0);
  glDeleteFramebuffersOES(1, &rInfo.mFramebuffer);
  if (rInfo.mTexture >= 0 && DestroyTexture)
    glDeleteRenderbuffersOES(1, &rInfo.mTexture);
  if (rInfo.mDepthbuffer >= 0)
    glDeleteRenderbuffersOES(1, &rInfo.mDepthbuffer);
  if (rInfo.mStencilbuffer >= 0)
    glDeleteRenderbuffersOES(1, &rInfo.mStencilbuffer);
  
  mFramebuffers.erase(it);
}
///< UnBind()
{
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, mOldFrameBuffer);

  if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
  {
    MI31_OUT(_T("Framebuffer status incomplete\n"));
    return;
  }

  mIsSet = false;
}
*/

void nuiGLESPainter::ApplyTexture(const nuiRenderState& rState, bool ForceApply)
{
  // 2D Textures: 
  std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
  bool uptodate = (it == mTextures.end()) ? false : ( !it->second.mReload && it->second.mTexture >= 0 );
  if (ForceApply || (mState.mpTexture != rState.mpTexture) || (mState.mpTexture && !uptodate))
  { 
    GLenum intarget = 0;
    GLenum outtarget = 0;

    if (mState.mpTexture)
    {      
      outtarget = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());

      //mState.mpTexture->UnapplyGL(this); #TODO Un apply the texture
      nuiCheckForGLErrors();
      mState.mpTexture->Release();
      nuiCheckForGLErrors();
    }

    mState.mpTexture = rState.mpTexture ;

    if (mState.mpTexture)
    {
      intarget = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());

      mState.mpTexture->Acquire();
      UploadTexture(mState.mpTexture);
      nuiCheckForGLErrors();
/*
      if (mState.mpShader)
      {
        std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
        mState.mpShader->SetTexture2D(0, it->second.mTexture);
      }
      nuiCheckForGLErrors();
*/
    }

    //NGL_OUT(_T("Change texture from 0x%x to 0x%x\n"), outtarget, intarget);

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
      if (intarget && mState.mTexturing && mState.mpTexture)
      {
        mState.mTexturing = rState.mTexturing;
        //NGL_OUT(_T("enable intarget\n"));
        glEnable(intarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      // Texture Target have not changed     
      if (mState.mTexturing != rState.mTexturing) // Have texture on/off changed?
      {
        // Should enable or disable texturing
        mState.mTexturing = rState.mTexturing;
        if (mState.mTexturing)
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

  if (ForceApply || (mState.mTexturing != rState.mTexturing))
  {
    // Texture have not changed, but texturing may have been enabled / disabled
    mState.mTexturing = rState.mTexturing;

    GLenum target = 0;
    if (mState.mpTexture)
    {
      target = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());

      if (target && mState.mTexturing)
      {
        //NGL_OUT(_T("Enable 0x%x\n"), target);
        glEnable(mTextureTarget);
        nuiCheckForGLErrors();
      }
      else
      {
        //NGL_OUT(_T("Disable 0x%x\n"), target);
        glDisable(mTextureTarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      //NGL_OUT(_T("Disable 0x%x\n"), target);
      if (mTextureTarget)
        glDisable(mTextureTarget);
      nuiCheckForGLErrors();
    }
  }
}

void nuiGLESPainter::ClearColor()
{
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;

  glClearColor(mState.mClearColor.Red(),mState.mClearColor.Green(),mState.mClearColor.Blue(),mState.mClearColor.Alpha());
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  nuiCheckForGLErrors();
}

void nuiGLESPainter::ClearStencil(uint8 value)
{
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;

  glStencilMask(~0);
  glClearStencil(value);
  glClear(GL_STENCIL_BUFFER_BIT);
  glStencilMask(0);
  nuiCheckForGLErrors();
}


/*
void nuiGLESPainter::BlurRect(const nuiRect& rRect, uint Strength)
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

void nuiGLESPainter::DrawArray(const nuiRenderArray& rArray)
{
  mRenderOperations++;
  mBatches++;
  uint32 size = rArray.GetSize();
  
  total += size;
  totalinframe += size;
  mins = MIN(mins, size);
  maxs = MAX(maxs, size);
  //printf("DA (%d) min = %d  max = %d\n", size, mins, maxs);
  
  if (!size)
    return;

  if (mClip.GetWidth() <= 0 || mClip.GetHeight() <= 0)
    return;

  mVertices += size;

/*
  switch (rArray.GetMode())
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

#ifdef NUI_USE_ANTIALIASING
  if (mState.mAntialiasing)
  {
#ifdef NUI_USE_MULTISAMPLE_AA
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_MULTISAMPLE_ARB);
    nuiCheckForGLErrors();
#else
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    nuiCheckForGLErrors();
#endif
  }
  else
#endif // NUI_USE_ANTIALIASING
  {
    if (rArray.UseGLAATexture())
    {
      if (mState.mTexturing && mTextureTarget != GL_TEXTURE_2D)
        glDisable(mTextureTarget);
      if (!mState.mTexturing || (mState.mTexturing && mTextureTarget != GL_TEXTURE_2D))
        glEnable(GL_TEXTURE_2D);

      if (!mState.mBlending)
        glEnable(GL_BLEND);
      if (mState.mBlendFunc != nuiBlendTransp)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// glBindTexture(GL_TEXTURE_2D, glAA_texture[0]);

      glMatrixMode(GL_TEXTURE);
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);

      glPushMatrix();
      glTranslatef(0.5f, 0.5f, 0);
    }
    else if (!mState.mTexturing)
    {
      glPushMatrix();
      glTranslatef(0.5f, 0.5f, 0);
      nuiCheckForGLErrors();
    }
  }

  {
    if (rArray.IsArrayEnabled(nuiRenderArray::eVertex))
    {
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mX);
      nuiCheckForGLErrors();
    }
    else
      glDisableClientState(GL_VERTEX_ARRAY);

    if (rArray.IsArrayEnabled(nuiRenderArray::eColor))
    {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mR);
      nuiCheckForGLErrors();
    }
    else
    {
      glDisableClientState(GL_COLOR_ARRAY);
      nuiColor c;
      switch (rArray.GetMode())
      {
      case GL_POINTS:
      case GL_LINES:
      case GL_LINE_LOOP:
      case GL_LINE_STRIP:
        c = mState.mStrokeColor;
        break;

      case GL_TRIANGLES:
      case GL_TRIANGLE_STRIP:
      case GL_TRIANGLE_FAN:
        c = mState.mFillColor;
        break;
      default:
//      case GL_QUADS:
//      case GL_QUAD_STRIP:
//      case GL_POLYGON: 
        {
          NGL_ASSERT(!"GLES doesnt handle primitives: GL_QUADS || GL_QUAD_STRIP || GL_POLYGON");
        } break;

      }
      glColor4f(c.Red(), c.Green(), c.Blue(), c.Alpha());
      nuiCheckForGLErrors();
    }

    if (rArray.IsArrayEnabled(nuiRenderArray::eTexCoord))
    {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mTX);
      nuiCheckForGLErrors();
    }
    else
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);

/*
    if (rArray.IsArrayEnabled(nuiRenderArray::eNormal))
    {
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, sizeof(nuiRenderArray::Vertex)-12, &rArray.GetVertices()[0].mNX);
      nuiCheckForGLErrors();
    }
    else
      glDisableClientState(GL_NORMAL_ARRAY);
*/

/*
    if (rArray.IsArrayEnabled(nuiRenderArray::eEdgeFlag))
    {
      glEnableClientState(GL_EDGE_FLAG_ARRAY);
      glEdgeFlagPointer(sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mEdgeFlag);
      nuiCheckForGLErrors();
    }
    else
      glDisableClientState(GL_EDGE_FLAG_ARRAY);
*/
    nuiCheckForGLErrors();
    glDrawArrays(rArray.GetMode(), 0, size);
    nuiCheckForGLErrors();
  }

#ifdef NUI_USE_ANTIALIASING
  if (mState.mAntialiasing)
  {
#ifdef NUI_USE_MULTISAMPLE_AA
    glDisable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
    nuiCheckForGLErrors();
  }
  else
#endif // NUI_USE_ANTIALIASING
  {
    if (rArray.UseGLAATexture())
    {
      glMatrixMode(GL_TEXTURE);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();

      if (mState.mpTexture && mState.mTexturing)
      {
        if (mTextureTarget != GL_TEXTURE_2D)
        {
          glDisable(GL_TEXTURE_2D);
          glEnable(mTextureTarget);
        }

        UploadTexture(mState.mpTexture);
      }
      else
      {
        glDisable(GL_TEXTURE_2D);
      }

      if (!mState.mBlending)
        glDisable(GL_BLEND);
      if (mState.mBlendFunc != nuiBlendTransp)
      {
        GLenum src, dst;
        nuiGetBlendFuncFactors(mState.mBlendFunc, src, dst);
        glBlendFunc(src, dst);
      }
      //ApplyTexture(mState, true);
    }
    else if (!mState.mTexturing)
    {
      glPopMatrix();
    }
    nuiCheckForGLErrors();
  }

  //glColor3f(1.0f, 1.0f, 1.0f);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  
  nuiCheckForGLErrors();
}

void nuiGLESPainter::BeginSession()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  mpContext->MakeCurrent();
  nuiCheckForGLErrors();
  
  totalinframe = 0;
}

void nuiGLESPainter::EndSession()
{
  // Bleh!
  NUI_RETURN_IF_RENDERING_DISABLED;
  //printf("min = %d max = %d total in frame = %d total = %d\n", mins, maxs, totalinframe, total);
}

void nuiGLESPainter::LoadMatrix(const nglMatrixf& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::LoadMatrix(rMatrix);
  glLoadMatrixf(rMatrix.Array);

  nuiCheckForGLErrors();
}

void nuiGLESPainter::MultMatrix(const nglMatrixf& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::MultMatrix(rMatrix);
  //glMultMatrixf(rMatrix.Array);
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

void nuiGLESPainter::PushMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::PushMatrix();
  //glPushMatrix();
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

void nuiGLESPainter::PopMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::PopMatrix();
  //glPopMatrix();
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

#ifdef glDeleteBuffersARB
  #undef glDeleteBuffersARB
#endif

void nuiGLESPainter::ReleaseCacheObject(void* pHandle)
{
  GLuint array = (GLuint)pHandle;
//  mpContext->glDeleteBuffersARB(1, &array);
  nuiCheckForGLErrors();
}

uint32 nuiGLESPainter::GetRectangleTextureSupport() const
{
  return mCanRectangleTexture;
}

GLenum nuiGLESPainter::GetTextureTarget(bool POT) const
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
 //       target = GL_TEXTURE_RECTANGLE_ARB;
        break;
    }
  }
  
  return target;
}

nuiGLESPainter::TextureInfo::TextureInfo()
{
  mReload = false;
  mTexture = -1;
}

nuiTexture* nuiGLESPainter::CreateTexture(const nglString& rName, const nuiSize& rWidth, const nuiSize& rHeight)
{
  
  return NULL;
}

void nuiGLESPainter::UploadTexture(nuiTexture* pTexture)
{
  float Width = pTexture->GetWidth();
  float Height = pTexture->GetHeight();
  GLenum target = GetTextureTarget(pTexture->IsPowerOfTwo());
  bool changedctx = false;

  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    it = mTextures.insert(mTextures.begin(), std::pair<nuiTexture*, TextureInfo>(pTexture, TextureInfo()));
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);

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
    if (!pImage || !pImage->GetPixelSize())
      return;
    
    int type = pImage->GetBitDepth();
    
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
      nuiCheckForGLErrors();
      firstload = true;
      reload = true;
    }
    
    glBindTexture(target, info.mTexture);
    nuiCheckForGLErrors();
    
    if (reload)
    {
      GLbyte* pBuffer = NULL;
      bool allocated = false;
      
      pBuffer = (GLbyte*)pImage->GetBuffer();
      
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
      
      if (type == 8)
        type=GL_UNSIGNED_BYTE;
      else if (type==16 || type==15)
        type=GL_UNSIGNED_SHORT_5_5_5_1;
      else if (type==24)
        type=GL_UNSIGNED_BYTE;
      else if (type==32)
        type=GL_UNSIGNED_BYTE;
      
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      nuiCheckForGLErrors();
      
      GLuint pixelformat = pImage->GetPixelFormat();
      
//      NGL_OUT(_T("UPLOADED IMAGE !!\n"));
      {
        glTexImage2D
        (  
         target,
         0,
         pImage->GetPixelFormat(),
         (int)Width,
         (int)Height,
         0,
         pixelformat,
         type,
         pBuffer
         );
        nuiCheckForGLErrors();
      }

      info.mReload = false;

      if (allocated)
        free(pBuffer);
   
      if (!pTexture->IsBufferRetained()) {
        pTexture->ReleaseBuffer();
      }

    }
  }
  
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, pTexture->GetMinFilter());
  nuiCheckForGLErrors();
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, pTexture->GetMagFilter());
  nuiCheckForGLErrors();
  glTexParameteri(target, GL_TEXTURE_WRAP_S, pTexture->GetWrapS());
  nuiCheckForGLErrors();
  glTexParameteri(target, GL_TEXTURE_WRAP_T, pTexture->GetWrapT());
  nuiCheckForGLErrors();
  
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pTexture->GetEnvMode());
  nuiCheckForGLErrors();
  
  if (changedctx)
  {
    mpContext->BeginSession();
    nuiCheckForGLErrors();
    glBindTexture(target, info.mTexture);
    nuiCheckForGLErrors();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pTexture->GetEnvMode());
    nuiCheckForGLErrors();
  }
  
  glMatrixMode(GL_TEXTURE);
  nuiCheckForGLErrors();
  glLoadIdentity();
  nuiCheckForGLErrors();
  
  uint32 rectangle = GetRectangleTextureSupport();
  nuiCheckForGLErrors();
  if (rectangle != 1)
  {
    
    float rx = pTexture->GetWidth() / Width;
    float ry = pTexture->GetHeight() / Height;
/*
    if (target == GL_TEXTURE_RECTANGLE_ARB)
    {
      rx *= pTexture->GetWidth();
      ry *= pTexture->GetHeight();
    }
*/    
    glScalef(rx, ry, 1);

    nuiCheckForGLErrors();
  }
  
  glMatrixMode(GL_MODELVIEW);
  nuiCheckForGLErrors();
}

void nuiGLESPainter::DestroyTexture(nuiTexture* pTexture)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
  NGL_ASSERT(it != mTextures.end());
  
  TextureInfo& info(it->second);
  glDeleteTextures(1, &info.mTexture);
  mTextures.erase(it);
}

void nuiGLESPainter::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
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


#endif //   #ifndef __NUI_NO_GL__

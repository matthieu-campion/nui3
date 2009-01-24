/*
 NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiGLESPainter.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nglMatrix.h"
#include "nuiTexture.h"
#include "nuiSurface.h"

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
  mOldFramebuffer = 0;

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

inline void nuiSetViewport(GLuint Angle, GLuint Width, GLuint Height)
{
  glMatrixMode(GL_PROJECTION);    
  
  if (Angle == 90 || Angle == 270)
  {
    glViewport(0,0, Height, Width);
  }
  else
  {
    glViewport(0,0, Width, Height);
  }
  
  nuiCheckForGLErrors();
  glLoadIdentity();
  
  glRotatef(Angle, 0.f,0.f,1.f);
  
  glTranslatef(-1.0f, 1.0f, 0.0f );
  glScalef (2.0f/(float)Width, -2.0f/(float)Height, 1.0f);
  
  nuiCheckForGLErrors();
  
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
}

void nuiGLESPainter::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  BeginSession();
  nuiCheckForGLErrors();

  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::StartRendering(ClipOffsetX, ClipOffsetY);

  nuiSetViewport(mAngle, mWidth, mHeight);

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

  ApplySurface(rState, ForceApply);

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
    
    int x,y,w,h;
    uint angle = (rState.mpSurface && rState.mpSurface->GetRenderToTexture()) ? 0 : mAngle;
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
      x = ToBelow(mWidth - w - clip.Left());
      y = ToBelow(clip.Top());
    }
    else if (angle == 270)
    {
      w = ToBelow(clip.GetHeight());
      h = ToBelow(clip.GetWidth());
      x = ToBelow(mHeight - clip.Top() - w);
      y = ToBelow(mWidth - clip.Left() - h);
    }
    else
    {
      NGL_ASSERT(!angle);
      x = ToBelow(clip.Left());
      y = ToBelow(mHeight - clip.Bottom());
      w = ToBelow(clip.GetWidth());
      h = ToBelow(clip.GetHeight());
    }
//NGL_OUT(_T("To Screen Clip {%d, %d, %d, %d}\n"), x,y,w,h);
    glScissor(x, y, w, h);

    nuiCheckForGLErrors();
  }
  else
  {
    glDisable(GL_SCISSOR_TEST);
  }

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

  nuiSetViewport (mAngle, w, h);
  nuiCheckForGLErrors();
}

inline bool nuiCheckFramebufferStatus()
{
  GLint status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);

#if defined(NGL_DEBUG)

  switch (status)
  {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES:
      {
        NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES\n"));
      }
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES:
      {
        NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES\n"));
      }
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES:
      {
        NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES\n"));
      }
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES:
      {
        NGL_OUT(_T("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES\n"));
      }
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_OES:
      {
        NGL_OUT(_T("GL_FRAMEBUFFER_UNSUPPORTED_OES\n"));
      }
      break;
  }

#endif

  return (status == GL_FRAMEBUFFER_COMPLETE_OES);
}

void nuiGLESPainter::ApplySurface(const nuiRenderState& rState, bool ForceApply)
{
  nuiSurface* pSurface = rState.mpSurface;
  
  if (pSurface == mState.mpSurface && !ForceApply)
    return;

  mState.mpSurface = pSurface;

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

    GLint oldFramebuffer=0, oldTexture=0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES,   &oldFramebuffer);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &oldTexture);

    if (!mOldFramebuffer)
      mOldFramebuffer = oldFramebuffer;

    FramebufferInfo info;

    if (create)
    {
      glGenFramebuffersOES(1, &info.mFramebuffer);
      nuiCheckForGLErrors();
      glBindFramebufferOES(GL_FRAMEBUFFER_OES, info.mFramebuffer);
      nuiCheckForGLErrors();

///< Do we need a depth buffer
      if (pSurface->GetDepth())
      {
        glGenRenderbuffersOES(1, &info.mDepthbuffer);
        nuiCheckForGLErrors();
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, info.mDepthbuffer);
        nuiCheckForGLErrors();

        glRenderbufferStorageOES(GL_RENDERBUFFER_OES,
                                 GL_DEPTH_COMPONENT16_OES,// || GL_DEPTH_COMPONENT24_OES
                                 width, height);
        nuiCheckForGLErrors();

        glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
        nuiCheckForGLErrors();

        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                     GL_DEPTH_ATTACHMENT_OES,
                                     GL_RENDERBUFFER_OES,
                                     info.mDepthbuffer);
        nuiCheckForGLErrors();
      }
      else
      {
        info.mDepthbuffer = 0;
      }

///< Do we need a stencil buffer
      if (pSurface->GetStencil())
      {
        glGenRenderbuffersOES(1, &info.mStencilbuffer);
        nuiCheckForGLErrors();

        glBindRenderbufferOES(GL_RENDERBUFFER_OES, info.mStencilbuffer);
        nuiCheckForGLErrors();

        glRenderbufferStorageOES(GL_RENDERBUFFER_OES,
                                 GL_STENCIL_ATTACHMENT_OES, ///< seems odd
                                 width, height);
        nuiCheckForGLErrors();

        glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
        nuiCheckForGLErrors();

        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                     GL_STENCIL_ATTACHMENT_OES,
                                     GL_RENDERBUFFER_OES,
                                     info.mStencilbuffer);
        nuiCheckForGLErrors();
      }
      else
      {
        info.mStencilbuffer = 0;
      }

///< We definetly need a color attachement
///< render to texture
      if (pTexture && pSurface->GetRenderToTexture())
      {
        UploadTexture(pTexture);

        std::map<nuiTexture*, TextureInfo>::iterator tex_it = mTextures.find(pTexture);
        NGL_ASSERT(tex_it != mTextures.end());
        TextureInfo& tex_info(tex_it->second);
        
        glBindTexture(GL_TEXTURE_2D, oldTexture);
        nuiCheckForGLErrors();

        glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES,
                                  GL_COLOR_ATTACHMENT0_OES,
                                  GetTextureTarget(pTexture->IsPowerOfTwo()),
                                  tex_info.mTexture,
                                  0);
        nuiCheckForGLErrors();
      }
      else
      {
        ///< TODO: renderbuffer (can be used for the screen framebuffer)
      }

      nuiCheckFramebufferStatus();
      nuiCheckForGLErrors();
      mFramebuffers[pSurface] = info;
    }
    else
    {
      /// !create
      info = it->second;
      glBindFramebufferOES(GL_FRAMEBUFFER_OES, info.mFramebuffer);
      nuiCheckForGLErrors();
      nuiCheckFramebufferStatus();
    }
    nuiSetViewport(0, pSurface->GetWidth(), pSurface->GetHeight());
  }
  else
  {
    /// !pSurface
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, mOldFramebuffer);
    nuiCheckForGLErrors();
    nuiCheckFramebufferStatus();
    nuiSetViewport(mAngle, mWidth, mHeight);
  }
}


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
  
  if (!mEnableDrawArray)
    return;
  
  uint32 s = rArray.GetSize();
  
  total += s;
  totalinframe += s;
  mins = MIN(mins, s);
  maxs = MAX(maxs, s);
  //printf("DA (%d) min = %d  max = %d\n", s, mins, maxs);
  
  if (!s)
    return;
  
  if (mClip.GetWidth() <= 0 || mClip.GetHeight() <= 0)
    return;
  
  mVertices += s;
  GLenum mode = rArray.GetMode();
  //NGL_OUT(_T("GL Array Mode: %d   vertice count %d\n"), mode, size);
  
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
  
  bool NeedTranslateHack = mode == GL_LINES || mode == GL_LINE_LOOP || mode == GL_LINE_STRIP;
  float hackX, hackY;
  if (NeedTranslateHack) {
    if (mAngle == 0)        { hackX = 1; hackY = 1; }
    else if (mAngle == 90)  { hackX = 0; hackY = 1; }
    else if (mAngle == 180) { hackX = 0; hackY = 0; }
    else/*mAngle == 270*/   { hackX = 1; hackY = 0; }
    glTranslatef(hackX, hackY, 0);
  }

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
#endif // NUI_USE_ANTIALIASING

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
    glDrawArrays(mode, 0, s);
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
    if (NeedTranslateHack)
      glTranslatef(-hackX, -hackY, 0);
  }
  
  glColor4f(1.0f, 1.0f, 1.0f, 1.f);
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

void nuiGLESPainter::CreateTexture(nuiTexture* pTexture)
{
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
  nuiSurface* pSurface = pTexture->GetSurface();

  {
    bool firstload = false;
    bool reload = info.mReload;
    if (!pSurface && !(pImage && pImage->GetPixelSize()))
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
      nuiCheckForGLErrors();
      firstload = true;
      reload = true;
    }
    
    glBindTexture(target, info.mTexture);
    nuiCheckForGLErrors();

    if (reload)
    {
      int type = 8;
      GLuint pixelformat = 0;
      GLuint internalPixelformat = 0;
      GLbyte* pBuffer = NULL;
      bool allocated = false;
      
      if (pImage)
      {        
        type = pImage->GetBitDepth();
        pixelformat = pImage->GetPixelFormat();
        internalPixelformat = pImage->GetPixelFormat();
        
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
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        nuiCheckForGLErrors();
      }
      else
      {
        NGL_ASSERT(pSurface);
        type = 32;//8;//pSurface->GetBitDepth();
        pixelformat = GL_RGBA;//pSurface->GetPixelFormat()
        internalPixelformat = GL_RGBA;//pSurface->GetPixelFormat();
      }

      if (type == 8)
        type=GL_UNSIGNED_BYTE;
      else if (type==16 || type==15)
        type=GL_UNSIGNED_SHORT_5_5_5_1;
      else if (type==24)
        type=GL_UNSIGNED_BYTE;
      else if (type==32)
        type=GL_UNSIGNED_BYTE;
        
  //      NGL_OUT(_T("UPLOADED IMAGE !!\n"));
      {
        glTexImage2D
        (  
         target,
         0,
         internalPixelformat,
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

      if (allocated && pBuffer)
        free(pBuffer);
   
      if (!pTexture->IsBufferRetained()) {
        pTexture->ReleaseBuffer();
      }
    }///reload
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

//  if (pSurface)
//    return;

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

nuiGLESPainter::FramebufferInfo::FramebufferInfo()
{
  mReload = true;
  mFramebuffer = 0;
  mTexture = 0;
  mDepthbuffer = 0;
  mStencilbuffer = 0;
}

void nuiGLESPainter::CreateSurface(nuiSurface* pSurface)
{
  
}

void nuiGLESPainter::DestroySurface(nuiSurface* pSurface)
{
  std::map<nuiSurface*, FramebufferInfo>::iterator it = mFramebuffers.find(pSurface);
  if (it == mFramebuffers.end()) {
    return;
  }
  FramebufferInfo info = it->second;
  
  NGL_ASSERT(info.mFramebuffer > 0);
  glDeleteFramebuffersOES(1, &info.mFramebuffer);
  if (info.mDepthbuffer > 0)
    glDeleteRenderbuffersOES(1, &info.mDepthbuffer);
  if (info.mStencilbuffer > 0)
    glDeleteRenderbuffersOES(1, &info.mStencilbuffer);
  
  mFramebuffers.erase(it);  
}

void nuiGLESPainter::InvalidateSurface(nuiSurface* pSurface, bool ForceReload)
{

}


#endif //   #ifndef __NUI_NO_GL__

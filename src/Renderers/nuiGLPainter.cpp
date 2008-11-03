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
#include "nuiShader.h"
#include "nuiTexture.h"

#ifndef __NUI_NO_GL__

#define NUI_RETURN_IF_RENDERING_DISABLED
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6

//#define NUI_USE_ANTIALIASING
#ifdef NUI_USE_ANTIALIASING
#define NUI_USE_MULTISAMPLE_AA
#endif

uint32 nuiGLPainter::mActiveContexts = 0;

static uint32 mins = 30000;
static uint32 maxs = 0;
static uint32 totalinframe = 0;
static uint32 total = 0;

//
//  //	generate the buffer ID and bind it
//  glGenBuffersARB( 1, &vertexBufferID );
//  glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertexBufferID );
//
//  //	tell OpenGL how much data you've got
//  //	and whether it is static (GL_STATIC_DRAW_ARB),
//  //	dynamic (GL_DYNAMIC_DRAW_ARB),
//  //	or streamed (GL_STREAM_DRAW_ARB)
//  //	according to the ARB spec, streamed means
//  //	"specify once and use once or perhaps only a few times"
//  glBufferDataARB( GL_ARRAY_BUFFER_ARB, dataSize,
//                  NULL, GL_STATIC_DRAW_ARB );
//
//  //	ask OpenGL for a buffer to store your data
//  GLubyte *dest = glMapBufferARB( GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB );
//
//  //	write your data into the buffer once
//
//  //	flush the buffer object and let OpenGL cache it
//  glUnmapBufferARB( GL_ARRAY_BUFFER_ARB );
//
//  //	unbind the VBO
//  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );

nuiGLPainter::nuiGLPainter(nglContext* pContext, const nuiRect& rRect)
: nuiPainter(rRect, pContext)
{
  mCanRectangleTexture = 0;
  mTextureTarget = GL_TEXTURE_2D;

  
  mpContext = pContext;
  if (mpContext)
  {
    mpContext->BeginSession();

    mpContext->CheckExtension(_T("GL_VERSION_1_2"));
	nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_1_3"));
	nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_1_4"));
	nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_VERSION_1_5"));
	nuiCheckForGLErrors();
    mpContext->CheckExtension(_T("GL_ARB_vertex_buffer_object"));
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
//		glAAGenerateAATex(0.0f, 0.0f);
//		nuiCheckForGLErrors();

		// texture init
		//#ifndef __APPLE__
		//glAAEnable(GLAA_VERTEX_ARRAY); // we want VAR acceleration and we will handle flushing
		//#endif
	}


    nuiCheckForGLErrors();
  }

  mActiveContexts++;
}

nuiGLPainter::~nuiGLPainter()
{
  mActiveContexts--;
  if (mActiveContexts == 0)
    glAAExit();
}


void nuiGLPainter::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
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

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_TEXTURE_2D);
  if (mCanRectangleTexture == 2)
  {
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
  }
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  nuiCheckForGLErrors();
}

/*
void nuiGLPainter::BeginSession()
{ 
  mpContext->BeginSession();
}


void nuiGLPainter::StopRendering()
{
  //glFlush();

}
*/


void nuiGLPainter::SetState(const nuiRenderState& rState, bool ForceApply)
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
    glScissor(ToBelow(clip.Left()),
      ToBelow(mHeight - clip.Bottom()),
      ToBelow(clip.GetWidth()),
      ToBelow(clip.GetHeight()) );
    //NGL_OUT(_T("Clip {%f, %f, %f, %f}\n"), mClip.mLeft, mClip.mTop, mClip.GetWidth(), mClip.GetHeight());
    nuiCheckForGLErrors();
  }
  else
    glDisable(GL_SCISSOR_TEST);

  mState.mClearColor = rState.mClearColor;
  mState.mStrokeColor = rState.mStrokeColor;
  mState.mFillColor = rState.mFillColor;

  nuiCheckForGLErrors();
}

void nuiGLPainter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  mWidth = w;
  mHeight = h;

  glViewport (0, 0, w, h);
  nuiCheckForGLErrors();
}

void nuiGLPainter::ApplyTexture(const nuiRenderState& rState, bool ForceApply)
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
      if (mState.mpShader)
      {
        std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
        mState.mpShader->SetTexture2D(0, it->second.mTexture);
      }
      nuiCheckForGLErrors();
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

void nuiGLPainter::ClearColor()
{
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;

  glClearColor(mState.mClearColor.Red(),mState.mClearColor.Green(),mState.mClearColor.Blue(),mState.mClearColor.Alpha());
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  nuiCheckForGLErrors();
}

void nuiGLPainter::ClearStencil(uint8 value)
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

void nuiGLPainter::DrawArray(const nuiRenderArray& rArray)
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
//#TEST meeloo disabling AA texture
//     if (rArray.UseGLAATexture())
//     {
//       if (mState.mTexturing && mTextureTarget != GL_TEXTURE_2D)
//         glDisable(mTextureTarget);
//       if (!mState.mTexturing || (mState.mTexturing && mTextureTarget != GL_TEXTURE_2D))
//         glEnable(GL_TEXTURE_2D);
// 
//       if (!mState.mBlending)
//         glEnable(GL_BLEND);
//       if (mState.mBlendFunc != nuiBlendTransp)
//         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 
//       glBindTexture(GL_TEXTURE_2D, glAA_texture);
// 
//       glMatrixMode(GL_TEXTURE);
//       glPushMatrix();
//       glLoadIdentity();
//       glMatrixMode(GL_MODELVIEW);
// 
//       glPushMatrix();
//       glTranslatef(0.5f, 0.5f, 0);
//     }
//     else
    if (!mState.mTexturing)
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
      case GL_QUADS:
      case GL_QUAD_STRIP:
      case GL_POLYGON:
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
//#TEST meeloo disabling AA texture
//     if (rArray.UseGLAATexture())
//     {
//       glMatrixMode(GL_TEXTURE);
//       glPopMatrix();
//       glMatrixMode(GL_MODELVIEW);
//       glPopMatrix();
// 
//       if (mState.mpTexture && mState.mTexturing)
//       {
//         if (mTextureTarget != GL_TEXTURE_2D)
//         {
//           glDisable(GL_TEXTURE_2D);
//           glEnable(mTextureTarget);
//         }
// 
//         UploadTexture(mState.mpTexture);
//       }
//       else
//       {
//         glDisable(GL_TEXTURE_2D);
//       }
// 
//       if (!mState.mBlending)
//         glDisable(GL_BLEND);
//       if (mState.mBlendFunc != nuiBlendTransp)
//       {
//         GLenum src, dst;
//         nuiGetBlendFuncFactors(mState.mBlendFunc, src, dst);
//         glBlendFunc(src, dst);
//       }
//       //ApplyTexture(mState, true);
//     }
//     else
    if (!mState.mTexturing)
    {
      glPopMatrix();
    }
    nuiCheckForGLErrors();
  }

  glColor3f(1.0f, 1.0f, 1.0f);
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

void nuiGLLoadMatrix(const float* pMatrix)
{
  glLoadMatrixf(pMatrix);
}

void nuiGLLoadMatrix(const double* pMatrix)
{
  glLoadMatrixd(pMatrix);
}

void nuiGLPainter::LoadMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::LoadMatrix(rMatrix);
  nuiGLLoadMatrix(rMatrix.Array);

  nuiCheckForGLErrors();
}

void nuiGLPainter::MultMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::MultMatrix(rMatrix);
  //glMultMatrixf(rMatrix.Array);
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

void nuiGLPainter::PushMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::PushMatrix();
  //glPushMatrix();
  LoadMatrix(mMatrixStack.top());
  nuiCheckForGLErrors();
}

void nuiGLPainter::PopMatrix()
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

void nuiGLPainter::ReleaseCacheObject(void* pHandle)
{
  GLuint array = (GLuint)pHandle;
  mpContext->glDeleteBuffersARB(1, &array);
  nuiCheckForGLErrors();
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
        target = GL_TEXTURE_RECTANGLE_ARB;
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
      
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      nuiCheckForGLErrors();
      
#ifdef _CARBON_
      glTexParameteri(target,
                      GL_TEXTURE_STORAGE_HINT_APPLE,
                      GL_STORAGE_CACHED_APPLE);
      glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
#endif
      
      GLuint pixelformat = pImage->GetPixelFormat();
      
#ifndef _CARBON_
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
        if (pTexture->GetAutoMipMap())
        {
          glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, (int)log2(Width));
          gluBuild2DMipmaps(target, pImage->GetPixelFormat(), (int)Width, (int)Height, pixelformat, type, pBuffer);          
        }
        else
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
        }
        nuiCheckForGLErrors();
      }

      info.mReload = false;

      if (allocated)
        free(pBuffer);
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
    double rx = pTexture->GetWidth() / Width;
    double ry = pTexture->GetHeight() / Height;
    if (target == GL_TEXTURE_RECTANGLE_ARB)
    {
      rx *= pTexture->GetWidth();
      ry *= pTexture->GetHeight();
    }
    
    glScaled(rx, ry, 1);
    nuiCheckForGLErrors();
  }
  
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


#endif //   #ifndef __NUI_NO_GL__

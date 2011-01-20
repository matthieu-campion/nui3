/*
NUI - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sébastien Métrot

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
#include "nui.h"
#include "nuiGLDrawContext.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nglMatrix.h"
#include "AAPrimitives.h"
#include "nuiShader.h"

#ifndef __NUI_NOGL__

#ifdef __APPLE__
#      include <OpenGL/CGLMacro.h>
#endif

//#define NUI_PLANE_CLIPPING
#define NUI_SCISSOR_CLIPPING
#define NUI_USE_OPENGL_LIST_CACHE 0
//#define NUI_SHAPE_OPTIMS

//#define DBG_SHOW_CLIP_RECT

uint32 nuiGLDrawContext::mActiveContexts = 0;

nuiGLDrawContext::nuiGLDrawContext(nglContext* pContext, const nuiRect& rRect)
: nuiDrawContext(rRect)
{
  if (mActiveContexts == 0)
  {
    glAAInit();
    glAAGenerateAATex(0.0f, 0, 0.0f);
    // texture init
    //#ifndef __APPLE__
    glAAEnable(GLAA_VERTEX_ARRAY); // we want VAR acceleration and we will handle flushing
    //#endif
  }

  mActiveContexts++;

  mpContext = pContext;
  if (mpContext)
  {
    mpContext->BeginSession();
    mpContext->CheckExtension("GL_VERSION_1_2");
    mpContext->CheckExtension("GL_VERSION_1_3");
    mpContext->CheckExtension("GL_VERSION_1_4");
    mpContext->CheckExtension("GL_VERSION_1_5");
  }
  nuiGLDrawContext::RetrieveRenderState();

  mClippingRect.mEnabled=glIsEnabled(GL_SCISSOR_TEST)==GL_TRUE;
}

nuiGLDrawContext::~nuiGLDrawContext()
{
  mActiveContexts--;
  if (mActiveContexts == 0)
    glAAExit();
}


void nuiGLDrawContext::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  mpContext->BeginSession();

  mClipOffsetX = ClipOffsetX;
  mClipOffsetY = ClipOffsetY;

  glMatrixMode(GL_PROJECTION);
  glViewport(0,0, ToBelow(mWidth), ToBelow(mHeight));
  glLoadIdentity();
  //  glScalef (1, -1, 1);
  glTranslatef(-1.0f, 1.0f, 0.0f );
  glScalef (2.0f/mWidth, -2.0f/mHeight, 1.0f);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
}

void nuiGLDrawContext::BeginSession()
{ 
  mpContext->BeginSession();
}


void nuiGLDrawContext::StopRendering()
{
  //glFlush();

  SetTexture(NULL);
}



/****************************************************************************
*
* Clipping Support
*
****************************************************************************/
// Clipping using glScissor :
#ifdef NUI_SCISSOR_CLIPPING
void nuiGLDrawContext::ActivateClipper()
{
  EnableScissorTest(mClippingRect.mEnabled);
  nuiRect clip(mClippingRect);
  //  clip.Move(-mClipOffsetX, -mClipOffsetY);
  clip.Move(-mClipOffsetX, -mClipOffsetY);
  glScissor((int) (clip.Left()),
    (int) (mHeight - clip.Bottom()),
    (int) (clip.GetWidth()),
    (int) (clip.GetHeight()) );
}

bool nuiGLDrawContext::AddClipShape(nuiShape& rShape, bool Invert)
{
  EnableStencilTest(true);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glStencilMask(~0);

  // Now draw the shape in the stencil buffer:
  GLenum value = Invert? 0: mClipShapeValue;
  glStencilFunc(GL_ALWAYS, value, 255);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
  DrawShape(&rShape, eFillShape);

  // Now lets get back to the mode we were in before the stencill fun:
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glStencilMask(0);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilFunc(GL_EQUAL, mClipShapeValue, 255);
  return true;
}

bool nuiGLDrawContext::BlendClipShape(nuiShape& rShape, bool Invert)
{
  EnableStencilTest(true);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glStencilMask(~0);

  // Now draw the shape in the stencil buffer:
  GLenum value = Invert? 0: mClipShapeValue;
  glStencilFunc(GL_ALWAYS, value, 255);
  glStencilOp(GL_INCR, GL_INCR, GL_INCR);
  DrawShape(&rShape, eFillShape);

  mClipShapeValue += Invert?-1:1;

  // Now lets get back to the mode we were in before the stencill fun:
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glStencilMask(0);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilFunc(GL_EQUAL, mClipShapeValue, 255);
  return true;
}

bool nuiGLDrawContext::ResetClipShape(bool Invert)
{
  EnableStencilTest(true);
  //glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glStencilMask(~0);
  mClipShapeValue = 1;
  glClearStencil(Invert?mClipShapeValue:0);
  glClear(GL_STENCIL_BUFFER_BIT);

  EnableStencilTest(false);
  //glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glStencilMask(0);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilFunc(GL_EQUAL, mClipShapeValue, 255);
  return true;
}

#endif // NUI_SCISSOR_CLIPPING

// Clipping using glClipPlane :
#ifdef NUI_PLANE_CLIPPING
void nuiGLDrawContext::ActivateClipper()
{
  bool val = mClippingRect.mEnabled;

  pContext->PushMatrix();
  pContext->LoadIdentity();
  //  glRotatef(sin(rot)*4,0,0,1);
  //  glScalef(1+sin(rot)*0.1,1+cos(rot2)*0.1,1);

#ifdef DBG_SHOW_CLIP_RECT
  EnableClipPlane(0,false);
  EnableClipPlane(1,false);
  EnableClipPlane(2,false);
  EnableClipPlane(3,false);
  nuiColor c(1,0,0,0);
  mClippingRect.Draw(nuiRect::Lines,&c);
#endif

  EnableClipPlane(0,val);
  EnableClipPlane(1,val);
  EnableClipPlane(2,val);
  EnableClipPlane(3,val);


  GLdouble plane0[4]={1,0,0,-mClippingRect.mLeft};
  glClipPlane(GL_CLIP_PLANE0,plane0);

  GLdouble plane1[4]={-1,0,0,mClippingRect.mRight};
  glClipPlane(GL_CLIP_PLANE1,plane1);

  GLdouble plane2[4]={0,1,0,-mClippingRect.mTop};
  glClipPlane(GL_CLIP_PLANE2,plane2);

  GLdouble plane3[4]={0,-1,0,mClippingRect.mBottom};
  glClipPlane(GL_CLIP_PLANE3,plane3);

  pContext->PopMatrix();
}
#endif // NUI_PLANE_CLIPPING

/****************************************************************************
*
* Render state manipulation
*
****************************************************************************/

void nuiGLDrawContext::RetrieveRenderState()
{
  mCurrentState.mBlending = glIsEnabled(GL_BLEND) == GL_TRUE ? true : false;
  mCurrentState.mAlphaTest = glIsEnabled(GL_ALPHA_TEST) == GL_TRUE ? true : false;
  mCurrentState.mScissorTest = glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE ? true : false;
  mCurrentState.mStencilTest = glIsEnabled(GL_STENCIL_TEST) == GL_TRUE ? true : false;
  mCurrentState.mTexture2D = glIsEnabled(GL_TEXTURE_2D) == GL_TRUE ? true : false;
  mCurrentState.mTexture1D = glIsEnabled(GL_TEXTURE_1D) == GL_TRUE ? true : false;

  glGetIntegerv(GL_BLEND_SRC, (GLint*)&(mCurrentState.mBlendSourceFactor));
  glGetIntegerv(GL_BLEND_DST, (GLint*)&(mCurrentState.mBlendDestinationFactor));

  /*  FIXME: Implement matching texture names with actual nuiTextures... For the moment we will just consider that no nuiTexture is valid... */
  mCurrentState.mpTexture = NULL;

  mCurrentState.mGroup1Changed = false;
  mCurrentState.mGroup2Changed = false;
  mCurrentState.mGroup3Changed = false;
}

bool nuiGLDrawContext::SetBlendFunc(GLenum sfactor, GLenum dfactor) 
{ 
  if ( (sfactor == mCurrentState.mBlendSourceFactor) && (dfactor == mCurrentState.mBlendDestinationFactor)) 
    return false;

  mCurrentState.mBlendSourceFactor = sfactor;
  mCurrentState.mBlendDestinationFactor = dfactor;
  /*
  if (mpContext && mpContext->glBlendFuncSeparate)
  {
  mpContext->glBlendFuncSeparate(mCurrentState.mBlendSourceFactor, mCurrentState.mBlendDestinationFactor, GL_ONE, GL_ONE);
  mpContext->glBlendEquation(GL_FUNC_ADD);
  }
  else
  */
  glBlendFunc(mCurrentState.mBlendSourceFactor, mCurrentState.mBlendDestinationFactor);

  mCurrentState.mGroup1Changed = true;
  return true;
}

/****************************************************************************
*
* Matrix operations
*
****************************************************************************/
void nuiGLDrawContext::PushMatrix()
{
  glPushMatrix();
}

void nuiGLDrawContext::PopMatrix()
{
  glPopMatrix();
}

void nuiGLDrawContext::LoadMatrix(const nglMatrixf& Matrix)
{
  glLoadMatrixf(Matrix.Array);
}

void nuiGLDrawContext::MultMatrix(const nglMatrixf& Matrix)
{
  glMultMatrixf(Matrix.Array);
}

void nuiGLDrawContext::Translate(const nglVector2f& Vector)
{
  glTranslatef(Vector[0],Vector[1],0);
}

void nuiGLDrawContext::Translate(nuiSize X, nuiSize Y)
{
  glTranslatef(X,Y,0);
}

void nuiGLDrawContext::Scale(nuiSize X, nuiSize Y)
{
  glScalef(X,Y,1);
}

void nuiGLDrawContext::LoadIdentity()
{
  glLoadIdentity();
}

void nuiGLDrawContext::GetMatrix(nglMatrixf& rMatrix)
{
  rMatrix.GetMatrix(GL_MODELVIEW_MATRIX);
}

nglMatrixf nuiGLDrawContext::GetMatrix()
{
  nglMatrixf Matrix;
  Matrix.GetMatrix(GL_MODELVIEW_MATRIX);
  //  Matrix.Dump(0,"DrawContext Matrix");
  return Matrix;
}

/****************************************************************************
*
* Texture manipulation
*
****************************************************************************/

bool nuiGLDrawContext::SetTexture (nuiTexture* pTex) 
{ 
  if ( mCurrentState.mpTexture != pTex || (pTex && pTex->mForceReload)) 
  { 
    if (mCurrentState.mpTexture)
      mCurrentState.mpTexture->Unapply(this);
    mCurrentState.mpTexture = pTex ;
    mCurrentState.mGroup1Changed = true;
    if (pTex)
    {
      pTex->Apply(this);
      if (mCurrentState.mpShader)
        mCurrentState.mpShader->SetTexture2D(0, pTex->GetGLTexture());
    }

  }
  return true;
}


/****************************************************************************
*
* Drawing function
*
****************************************************************************/

void nuiGLDrawContext::SetSize(int w, int h)
{
  mWidth = (nuiSize)w;
  mHeight = (nuiSize)h;
  mClippingRect.mLeft   = 0;
  mClippingRect.mRight  = (nuiSize) w;
  mClippingRect.mTop    = 0;
  mClippingRect.mBottom = (nuiSize) h;

  glViewport (0, 0, w, h);
}

void nuiGLDrawContext::DrawImage(const nuiRect& rDest, const nuiRect& rSource)
{
  nuiSize tx,ty,tw,th;

  tx = rSource.mLeft;
  ty = rSource.mTop;
  //  tw = tx + rSource.GetWidth();
  tw = rSource.mRight;
  th = rSource.mBottom;
  //  th = ty + rSource.GetHeight();

  mCurrentState.mpTexture->ImageToTextureCoord(tx,ty);
  mCurrentState.mpTexture->ImageToTextureCoord(tw,th);

  mCurrentState.mFillColor.Apply();
  glBegin(GL_QUADS);

  glTexCoord2f(tx,ty); 
  glVertex2f(rDest.mLeft,rDest.mTop);
  //  glVertex2i(ToNearest(rDest.mLeft),ToNearest(rDest.mTop));

  glTexCoord2f(tw,ty); 
  glVertex2f(rDest.mRight,rDest.mTop);
  //  glVertex2i(ToNearest(rDest.mRight),ToNearest(rDest.mTop));

  glTexCoord2f(tw,th); 
  glVertex2f(rDest.mRight,rDest.mBottom);
  //  glVertex2i(ToNearest(rDest.mRight),ToNearest(rDest.mBottom));

  glTexCoord2f(tx,th); 
  glVertex2f(rDest.mLeft,rDest.mBottom);
  //  glVertex2i(ToNearest(rDest.mLeft),ToNearest(rDest.mBottom));

  glEnd();
}

void nuiGLDrawContext::DrawImageQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource)
{
  nuiSize tx,ty,tw,th;

  tx = rSource.mLeft;
  ty = rSource.mTop;
  //  tw = tx + rSource.GetWidth();
  tw = rSource.mRight;
  th = rSource.mBottom;
  //  th = ty + rSource.GetHeight();

  mCurrentState.mpTexture->ImageToTextureCoord(tx,ty);
  mCurrentState.mpTexture->ImageToTextureCoord(tw,th);

  mCurrentState.mFillColor.Apply();
  glBegin(GL_QUADS);

  glTexCoord2f(tx,ty); 
  glVertex2f(x0, y0);
  //  glVertex2i(ToNearest(rDest.mLeft),ToNearest(rDest.mTop));

  glTexCoord2f(tw,ty); 
  glVertex2f(x1, y1);
  //  glVertex2i(ToNearest(rDest.mRight),ToNearest(rDest.mTop));

  glTexCoord2f(tw,th); 
  glVertex2f(x2, y2);
  //  glVertex2i(ToNearest(rDest.mRight),ToNearest(rDest.mBottom));

  glTexCoord2f(tx,th); 
  glVertex2f(x3, y3);
  //  glVertex2i(ToNearest(rDest.mLeft),ToNearest(rDest.mBottom));

  glEnd();
}

#define NUI_COMPLEX_SHAPE_THRESHOLD 6

void nuiGLDrawContext::DrawCachedShape(std::list<nuiShape::CacheElement*>* pElements, nuiShapeMode Mode)
{
  bool blend = mCurrentState.mBlending;
  uint func1,func2;
  bool tex2D;
  func1 = mCurrentState.mBlendSourceFactor;
  func2 = mCurrentState.mBlendDestinationFactor;
  tex2D = mCurrentState.mTexture2D;

  nuiColor c;

  if (Mode == eStrokeShape)
  {
    if (mCurrentState.mAntialiasing && mPermitAntialising)
    {
      EnableTexture2D(true);
      EnableBlending(true);
      EnableAlphaTest(true);
      glBindTexture(GL_TEXTURE_2D, glAA_texture[0]);
      SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glAlphaFunc(GL_GREATER, 1.0f/255.0f);

      // texture init

      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glScalef(0.25/phf, 0.25/phf, 1.0);              // glAArg requires rectangle coordinate setup
      glMatrixMode(GL_MODELVIEW);

      glAALineWidth(mCurrentState.mLineWidth);

      std::list<nuiShape::CacheElement*>::iterator it;
      std::list<nuiShape::CacheElement*>::iterator end = pElements->end();

      for (it = pElements->begin(); it != end; ++it)
      {
        nuiShape::CacheElement* pElement = *it;
        uint i = 0;

        switch (pElement->mOperation)
        {
        case GL_POINTS:
        case GL_LINES:
        case GL_LINE_LOOP:
        case GL_LINE_STRIP:
          c = mCurrentState.mStrokeColor;
          break;

        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
        case GL_QUADS:
        case GL_QUAD_STRIP:
        case GL_POLYGON:
          c = mCurrentState.mFillColor;
          break;
        }

        /*
        if (pElement->mCount > 16)
        glAADisable(GLAA_VERTEX_ARRAY); // we want VAR acceleration and we will handle flushing
        else
        glAAEnable(GLAA_VERTEX_ARRAY); // we want VAR acceleration and we will handle flushing
        */

        glAAColor4f(
          c.Red(),
          c.Green(),
          c.Blue(),
          c.Alpha()
          );
        glAABegin(pElement->mOperation);

        for (i = 0; i<pElement->mCount; i++)
        {
          nuiShape::CacheElement* pElement = *it;
          /*float* c = pElement->mpVertices[i].mColor; (unused) */
          float* coords = pElement->mpVertices[i].mCoord;
          glAAVertex2f(coords[0], coords[1]);
        }
        glAAEnd();
        glAAFlush();
      }

      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);

      SetTexture(NULL);
      EnableTexture2D(false);
      EnableBlending(false);
      EnableAlphaTest(false);
      return;
    }

    glLineWidth(mCurrentState.mLineWidth);
  }

  std::list<nuiShape::CacheElement*>::iterator it;
  std::list<nuiShape::CacheElement*>::iterator end = pElements->end();

  for (it = pElements->begin(); it != end; ++it)
  {
    nuiShape::CacheElement* pElement = *it;
    switch (pElement->mOperation)
    {
    case GL_POINTS:
    case GL_LINES:
    case GL_LINE_LOOP:
    case GL_LINE_STRIP:
      c = mCurrentState.mStrokeColor;
      break;

    case GL_TRIANGLES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_QUADS:
    case GL_QUAD_STRIP:
    case GL_POLYGON:
      c = mCurrentState.mFillColor;
      break;
    }
    glColor4f(
      c.Red(),
      c.Green(),
      c.Blue(),
      c.Alpha()
      );
    if (pElement->mCount <= NUI_COMPLEX_SHAPE_THRESHOLD)
    {
      uint i = 0;
      glBegin(pElement->mOperation);

      for (i = 0; i<pElement->mCount; i++)
      {
        nuiShape::CacheElement* pElement = *it;
        glVertex3fv(pElement->mpVertices[i].mCoord);
      }
      glEnd();
    }
    else
    {
      glVertexPointer(3,GL_FLOAT,sizeof(nuiShape::CacheElement::Vertex),pElement->mpVertices->mCoord);
      glEnableClientState(GL_VERTEX_ARRAY);
      glDrawArrays(pElement->mOperation, 0, pElement->mCount);
    }
  }

  //EnableLineSmooth(false);
  glDisable(GL_LINE_SMOOTH);
  SetBlendFunc(func1, func2);
  EnableTexture2D(tex2D);
  EnableBlending(blend);
}

void nuiGLDrawContext::DrawRect(const nuiRect& Rect, nuiShapeMode Mode)
{
  if (Mode == eStrokeAndFillShape || Mode == eFillShape)
  {
    mCurrentState.mFillColor.Apply();
    glBegin(GL_QUADS);
    glVertex2f(Rect.mLeft,Rect.mTop);
    glVertex2f(Rect.mRight,Rect.mTop);
    glVertex2f(Rect.mRight,Rect.mBottom);
    glVertex2f(Rect.mLeft,Rect.mBottom);
    glEnd();
  }

  if (Mode == eStrokeAndFillShape || Mode == eStrokeShape) // draw border???
  {
    mCurrentState.mStrokeColor.Apply();
    glBegin(GL_LINE_LOOP);
    glVertex2f(Rect.mLeft,Rect.mTop+1);
    glVertex2f(Rect.mRight-1,Rect.mTop+1);
    glVertex2f(Rect.mRight-1,Rect.mBottom);
    glVertex2f(Rect.mLeft,Rect.mBottom);
    glEnd();
  }
}

void nuiGLDrawContext::DrawLine(const nglVector2f& rOrigin, const nglVector2f& rDestination)
{
  DrawLine(rOrigin[0],rOrigin[1],rDestination[0],rDestination[1]);
}

void nuiGLDrawContext::DrawLine(float x1, float y1, float x2, float y2)
{
  if (mCurrentState.mAntialiasing && mPermitAntialising)
  {
    EnableTexture2D(true);
    EnableBlending(true);
    EnableAlphaTest(true);
    glBindTexture(GL_TEXTURE_2D, glAA_texture[0]);
    SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 1.0f/255.0f);

    // texture init

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(0.25/phf, 0.25/phf, 1.0);              // glAArg requires rectangle coordinate setup
    glMatrixMode(GL_MODELVIEW);

    glAALineWidth(mCurrentState.mLineWidth);

    glAABegin(GL_LINES);
    glAAColor4f(mCurrentState.mStrokeColor.Red(), mCurrentState.mStrokeColor.Green(), mCurrentState.mStrokeColor.Blue(), mCurrentState.mStrokeColor.Alpha());
    glAAVertex2f(x1, y1);
    glAAVertex2f(x2, y2);
    glAAEnd();

    glAAFlush();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    SetTexture(NULL);
    EnableTexture2D(false);
    EnableBlending(false);
    EnableAlphaTest(false);
  }
  else
  {
    mCurrentState.mStrokeColor.Apply();
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
  }
}

void nuiGLDrawContext::Clear()
{
  glClearColor(mCurrentState.mClearColor.Red(),mCurrentState.mClearColor.Green(),mCurrentState.mClearColor.Blue(),mCurrentState.mClearColor.Alpha());
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


#define SHADE_ALPHA 0.2f

void nuiGLDrawContext::DrawShade (const nuiRect& rSourceRect, const nuiRect& rShadeRect)
{
  //  return;

  bool blending = mCurrentState.mBlending;
  GLenum blendfunc1,blendfunc2;
  blendfunc1 = mCurrentState.mBlendSourceFactor;
  blendfunc2 = mCurrentState.mBlendDestinationFactor;

  EnableBlending(true);
  SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  nuiSize ShadeSize = rSourceRect.mLeft - rShadeRect.mLeft;

  // Left shadow
  uint8 pLUT[256*4];
  uint i;
  for (i = 0; i<256; i++)
  {
    pLUT[0+(i*4)] = 0;
    pLUT[1+(i*4)] = 0;
    pLUT[2+(i*4)] = 0;
    pLUT[3+(i*4)] = i;
  }

  EnableTexture2D(false);
  EnableTexture1D(true);

  glBindTexture(GL_TEXTURE_1D,0);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pLUT);

  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_T,GL_CLAMP);

  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glBegin(GL_QUADS);
  glColor4f(0,0,0,0);
  glTexCoord1f(0);
  glVertex2f(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop);
  glTexCoord1f(1);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mTop);
  glColor4f(0,0,0,SHADE_ALPHA);
  glTexCoord1f(1);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mTop+ShadeSize);
  glTexCoord1f(0);
  glVertex2f(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop+ShadeSize);
  glEnd();

  EnableTexture1D(false);

  glBegin(GL_QUADS);
  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop+ShadeSize);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mTop+ShadeSize);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mBottom);
  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom);
  glEnd();

  // bottom shadow
  glBegin(GL_QUADS);
  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mBottom+ShadeSize);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mBottom);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mRight,rSourceRect.mBottom);
  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mRight,rSourceRect.mBottom+ShadeSize);
  glEnd();

  // Right shadow
  EnableTexture1D(true);
  glBegin(GL_QUADS);
  glColor4f(0,0,0,0);
  glTexCoord1f(1);
  glVertex2f(rSourceRect.mRight,rSourceRect.mTop);
  glTexCoord1f(0);
  glVertex2f(rSourceRect.mRight+ShadeSize,rSourceRect.mTop);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mRight+ShadeSize,rSourceRect.mTop+ShadeSize);
  glTexCoord1f(1);
  glVertex2f(rSourceRect.mRight,rSourceRect.mTop+ShadeSize);
  glEnd();   
  EnableTexture1D(false);


  glBegin(GL_QUADS);
  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mRight+ShadeSize,rSourceRect.mTop+ShadeSize);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mRight,rSourceRect.mTop+ShadeSize);
  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mRight,rSourceRect.mBottom);
  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom);
  glEnd();


  EnableTexture1D(true);
  glBegin(GL_QUADS);

  // Right Corner
  glColor4f(0,0,0,SHADE_ALPHA);
  glTexCoord1f(1);
  glVertex2f(rSourceRect.mRight,rSourceRect.mBottom);

  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom);

  glColor4f(0,0,0,0);
  glTexCoord1f(0);
  glVertex2f(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom+ShadeSize);

  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mRight,rSourceRect.mBottom+ShadeSize);

  // Left Corner:
  glColor4f(0,0,0,0);
  glTexCoord1f(1);
  glVertex2f(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom);

  glColor4f(0,0,0,SHADE_ALPHA);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mBottom);

  glColor4f(0,0,0,SHADE_ALPHA);
  glTexCoord1f(0);
  glVertex2f(rSourceRect.mLeft,rSourceRect.mBottom+ShadeSize);

  glColor4f(0,0,0,0);
  glVertex2f(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom+ShadeSize);

  glEnd();
  EnableTexture1D(false);

  EnableBlending(blending);
  SetBlendFunc(blendfunc1, blendfunc2);
}

void nuiGLDrawContext::BlurRect(const nuiRect& rRect, uint Strength)
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
  EnableTexture2D(true);
  SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
  EnableTexture2D(false);

  glPopMatrix();
}


static void IntersectLines(float x0,float y0,float x1,float y1, 
                           float x2,float y2,float x3,float y3,
                           float& xi,float& yi)
{
  x1 += x0;
  y1 += y0;
  x3 += x2;
  y3 += y2;

  float a1,b1,c1, // constants of linear equations
    a2,b2,c2,
    det_inv,  // the inverse of the determinant of the coefficient
    m1,m2;    // the slopes of each line

  // compute slopes, note the cludge for infinity, however, this will
  // be close enough

  if ((x1-x0)!=0)
    m1 = (y1-y0)/(x1-x0);
  else
    m1 = (float)1e+10;   // close enough to infinity

  if ((x3-x2)!=0)
    m2 = (y3-y2)/(x3-x2);
  else
    m2 = (float)1e+10;   // close enough to infinity

  // compute constants
  a1 = m1;
  a2 = m2;

  b1 = -1;
  b2 = -1;

  c1 = (y0-m1*x0);
  c2 = (y2-m2*x2);

  // compute the inverse of the determinate

  if (m1 >= (float)1e+10 || m1 >= (float)1e+10)
    det_inv = 0;
  else
    det_inv = 1/(a1*b2 - a2*b1);

  // use Kramers rule to compute xi and yi

  xi=((b1*c2 - b2*c1)*det_inv);
  yi=((a2*c1 - a1*c2)*det_inv);

}


void nuiGLDrawContext::DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, nuiSize x1, nuiSize y1, nuiSize x2, nuiSize y2)
{
  nglVector2f vec(x2 - x1, y2 - y1);
  nglVector2f para(-vec[1], vec[0]);
  nglVector2f vec1(vec);
  nglVector2f para1(para);
  vec1.Normalize();
  para1.Normalize();

  // What Quadrant are we in?:
  //         |
  //     a   |   b
  //         |
  //  ----------------
  //         |
  //     c   |   d
  //         |
  float xa, xb, xc, xd;
  float ya, yb, yc, yd;
  float x, y;
  float xp, yp;
  float xx, yy;
  float xxp, yyp;

  xa = xc = rEnclosingRect.Left();
  xb = xd = rEnclosingRect.Right();
  ya = yb = rEnclosingRect.Top();
  yc = yd = rEnclosingRect.Bottom();

  if (x1 < x2)
  {
    // Go from a to d or c to b
    if (y1 == y2)
    {
      x  = xa; y  = ya;
      xp = xc; yp = yc;
      xx = xd; yy = yd;
      xxp= xb; yyp= yb;
    }
    else if (y1 < y2)
    {
      // a to d
      IntersectLines(xa,ya, para1[0], para1[1], xb, yb, vec1[0], vec1[1], x, y);
      IntersectLines(xa,ya, para1[0], para1[1], xc, yc, vec1[0], vec1[1], xp, yp);
      IntersectLines(xd,yd, para1[0], para1[1], xc, yc, vec1[0], vec1[1], xx, yy);
      IntersectLines(xd,yd, para1[0], para1[1], xb, yb, vec1[0], vec1[1], xxp, yyp);
    }
    else
    {
      // c to d
      IntersectLines(xc,yc, para1[0], para1[1], xa, ya, vec1[0], vec1[1], x, y);
      IntersectLines(xc,yc, para1[0], para1[1], xd, yd, vec1[0], vec1[1], xp, yp);
      IntersectLines(xb,yb, para1[0], para1[1], xd, yd, vec1[0], vec1[1], xx, yy);
      IntersectLines(xb,yb, para1[0], para1[1], xa, ya, vec1[0], vec1[1], xxp, yyp);
    }
  }
  else
  {
    if (y1 == y2)
    {
      x  = xd; y  = yd;
      xp = xb; yp = yb;
      xx = xa; yy = ya;
      xxp= xc; yyp= yc;
    }
    else if (y1 < y2)
    {
      // b to c
      IntersectLines(xb,yb, para1[0], para1[1], xd, yd, vec1[0], vec1[1], x, y);
      IntersectLines(xb,yb, para1[0], para1[1], xa, ya, vec1[0], vec1[1], xp, yp);
      IntersectLines(xc,yc, para1[0], para1[1], xa, ya, vec1[0], vec1[1], xx, yy);
      IntersectLines(xc,yc, para1[0], para1[1], xd, yd, vec1[0], vec1[1], xxp, yyp);
    }
    else
    {
      // d to a
      IntersectLines(xd,yd, para1[0], para1[1], xc, yc, vec1[0], vec1[1], x, y);
      IntersectLines(xd,yd, para1[0], para1[1], xb, yb, vec1[0], vec1[1], xp, yp);
      IntersectLines(xa,ya, para1[0], para1[1], xb, yb, vec1[0], vec1[1], xx, yy);
      IntersectLines(xa,ya, para1[0], para1[1], xc, yc, vec1[0], vec1[1], xxp, yyp);
    }
  }

  float startx,starty;
  float startxp,startyp;
  float stopx,stopy;
  float stopxp,stopyp;

  if (y1 != y2)
  {
    IntersectLines(x1, y1, para1[0], para1[1], x,  y,  vec1[0], vec1[1], startx,  starty);
    IntersectLines(x1, y1, para1[0], para1[1], xp, yp, vec1[0], vec1[1], startxp, startyp);
    IntersectLines(x2, y2, para1[0], para1[1], x,  y,  vec1[0], vec1[1], stopx,   stopy);
    IntersectLines(x2, y2, para1[0], para1[1], xp, yp, vec1[0], vec1[1], stopxp,  stopyp);
  }
  else
  {
    startx  = x1; starty  = y;
    startxp = x1; startyp = yp;
    stopx   = x2; stopy   = y;
    stopxp  = x2; stopyp  = yp;
  }

  nuiGradientStopList::const_iterator it = rGradient.GetStopList().begin();
  nuiGradientStopList::const_iterator end = rGradient.GetStopList().end();

  float px1, py1;
  float px2, py2;

  PushClipping();
  nuiRect r = rEnclosingRect;
  nglMatrixf m(GetMatrix());
  nglVectorf v1(r.Left(), r.Top(), 0);
  v1 = m * v1;
  nglVectorf v2 = nglVectorf(r.Right(), r.Bottom(), 0);
  v2 = m * v2;
  r.Set(v1[0], v1[1], v2[0], v2[1], false);

  Clip(r);
  SetClipping(true);

  std::vector<nuiShape::CacheElement::Vertex> vertices;

  nuiColor col = it->second;
  vertices.push_back(nuiShape::CacheElement::Vertex(x, y, col));
  vertices.push_back(nuiShape::CacheElement::Vertex(xp, yp, col));

  for ( ; it != end; ++it)
  {
    float r = it->first;
    float rm = 1.0f - r;
    px1 = startx * rm + stopx * r;
    py1 = starty * rm + stopy * r;
    px2 = startxp * rm + stopxp * r;
    py2 = startyp * rm + stopyp * r;

    col = it->second;
    vertices.push_back(nuiShape::CacheElement::Vertex(px1, py1, col));
    vertices.push_back(nuiShape::CacheElement::Vertex(px2, py2, col));
  }

  vertices.push_back(nuiShape::CacheElement::Vertex(xxp, yyp, col));
  vertices.push_back(nuiShape::CacheElement::Vertex(xx, yy, col));

  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(4,  GL_FLOAT, sizeof(nuiShape::CacheElement::Vertex), vertices[0].mColor);
  glVertexPointer(3, GL_FLOAT, sizeof(nuiShape::CacheElement::Vertex), vertices[0].mCoord);

  glDrawArrays(GL_QUAD_STRIP, 0, vertices.size());

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  PopClipping();
}

void nuiGLDrawContext::DrawArray(const nuiRenderArray& rArray)
{
  if (mCurrentState.mAntialiasing)
  {
/*
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
//    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_MULTISAMPLE_ARB);
  }

  if (rArray.IsArrayEnabled(nuiRenderArray::eVertex))
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(rArray.GetVertexElements(), GL_FLOAT, 0, &rArray.GetVertices()[0]);
  }
  else
    glDisableClientState(GL_VERTEX_ARRAY);

  if (rArray.IsArrayEnabled(nuiRenderArray::eColor))
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(rArray.GetColorElements(), GL_FLOAT, 0, &rArray.GetColors()[0]);
  }
  else
  {
    nuiColor c;
    switch (rArray.GetMode())
    {
      case GL_POINTS:
      case GL_LINES:
      case GL_LINE_LOOP:
      case GL_LINE_STRIP:
        c = mCurrentState.mStrokeColor;
        break;

      case GL_TRIANGLES:
      case GL_TRIANGLE_STRIP:
      case GL_TRIANGLE_FAN:
      case GL_QUADS:
      case GL_QUAD_STRIP:
      case GL_POLYGON:
        c = mCurrentState.mFillColor;
        break;
    }
    glColor4f(
      c.Red(),
      c.Green(),
      c.Blue(),
      c.Alpha()
      );
    glDisableClientState(GL_COLOR_ARRAY);
  }

  if (rArray.IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(rArray.GetTexCoordElements(), GL_FLOAT, 0, &rArray.GetTexCoords()[0]);
  }
  else
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  if (rArray.IsArrayEnabled(nuiRenderArray::eNormal))
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &rArray.GetNormals()[0]);
  }
  else
    glDisableClientState(GL_NORMAL_ARRAY);

  if (rArray.IsArrayEnabled(nuiRenderArray::eEdgeFlag))
  {
    glEnableClientState(GL_EDGE_FLAG_ARRAY);
    glEdgeFlagPointer(1, &rArray.GetEdgeFlags()[0]);
  }
  else
    glDisableClientState(GL_EDGE_FLAG_ARRAY);

  glDrawArrays(rArray.GetMode(), 0, rArray.GetSize());

  if (mCurrentState.mAntialiasing)
  {
    glDisable(GL_MULTISAMPLE_ARB);
  }

  glColor3f(1.0f, 1.0f, 1.0f);
}

void nuiGLDrawContext::DrawObject(const nuiRenderObject& rObject)
{
  uint32 count = rObject.GetSize();
  for (uint32 i = 0; i < count; i++)
  {
    DrawArray(*(rObject.GetArray(i)));
  }
}


void nuiGLDrawContext::SetShader(nuiShader* pShader) 
{
  if (mCurrentState.mpShader)
    mCurrentState.mpShader->UnBind();

  if (pShader)
    pShader->Bind();

  mCurrentState.mpShader = pShader; 
}

/****************************************************************************
*
* Enable/Disable RenderState properties
*
****************************************************************************/
#define __RENDERSTATE_ENABLE(group,localname,glname) \
  if (val != localname) \
{ \
  if (val) glEnable(glname); \
    else glDisable(glname); \
    localname=val; \
    group = true; \
} \
  return val; 

#define RENDERSTATE_ENABLE1(localname,glname) __RENDERSTATE_ENABLE(mCurrentState.mGroup1Changed,localname,glname)
#define RENDERSTATE_ENABLE2(localname,glname) __RENDERSTATE_ENABLE(mCurrentState.mGroup2Changed,localname,glname)
#define RENDERSTATE_ENABLE3(localname,glname) __RENDERSTATE_ENABLE(mCurrentState.mGroup3Changed,localname,glname)


#define RENDERSTATE_ENABLECLIENT(localname,glname) \
  if (val != localname) \
{ \
  if (val) glEnableClientState(glname); \
    else glDisableClientState(glname); \
    localname=val; \
    mCurrentState.mGroup2Changed = true; \
} \
  return val; 


bool nuiGLDrawContext::EnableBlending (bool val)       { RENDERSTATE_ENABLE1 ( mCurrentState.mBlending      , GL_BLEND          ); }
bool nuiGLDrawContext::EnableAlphaTest (bool val)      { RENDERSTATE_ENABLE1 ( mCurrentState.mAlphaTest     , GL_ALPHA_TEST     ); }

bool nuiGLDrawContext::EnableScissorTest (bool val)    { RENDERSTATE_ENABLE1 ( mCurrentState.mScissorTest   , GL_SCISSOR_TEST   ); }
bool nuiGLDrawContext::EnableStencilTest (bool val)    { RENDERSTATE_ENABLE2 ( mCurrentState.mStencilTest   , GL_STENCIL_TEST   ); }

bool nuiGLDrawContext::EnableTexture2D (bool val)      { RENDERSTATE_ENABLE1 ( mCurrentState.mTexture2D     , GL_TEXTURE_2D     ); }
bool nuiGLDrawContext::EnableTexture1D (bool val)      { RENDERSTATE_ENABLE3 ( mCurrentState.mTexture1D     , GL_TEXTURE_1D     ); }


#undef __RENDERSTATE_ENABLE

#undef RENDERSTATE_ENABLE1
#undef RENDERSTATE_ENABLE2
#undef RENDERSTATE_ENABLE3

#undef RENDERSTATE_ENABLECLIENT

#endif //   #ifndef __NUI_NOGL__

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiPainter.h"

///////////////////////////////////
// nuiPainter implementation:
nuiPainter::nuiPainter(const nuiRect& rRect, nglContext* pContext) 
{
  ResetStats();
  mWidth = ToNearest(rRect.GetWidth());
  mHeight = ToNearest(rRect.GetHeight());
  mMatrixStack.push(nuiMatrix());
  mDummyMode = false;
  mpSurface = NULL;

  mAngle=0;

  mEnableDrawArray = true;
}

nuiPainter::~nuiPainter() 
{
  // Empty the clip stack:
  while (!mpClippingStack.empty())
  {
    delete mpClippingStack.top();
    mpClippingStack.pop();
  }

  while (!mpSurfaceStack.empty())
  {
    PopSurface();
  }
  SetSurface(NULL);
}

void nuiPainter::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  mClipOffsetX = ClipOffsetX;
  mClipOffsetY = ClipOffsetY;
  while (!mpClippingStack.empty())
  {
    delete mpClippingStack.top();
    mpClippingStack.pop();
  }

  uint32 w=mWidth, h=mHeight;
  mClip.Set(ToNearest(mClipOffsetX), ToNearest(mClipOffsetY), w, h);
  while (!mMatrixStack.empty())
    mMatrixStack.pop();
  mMatrixStack.push(nuiMatrix());
}

void nuiPainter::PushMatrix()
{
  NGL_ASSERT(!mMatrixStack.empty());
  mMatrixStack.push(mMatrixStack.top());
}

void nuiPainter::PopMatrix()
{
  NGL_ASSERT(!mMatrixStack.empty());
  mMatrixStack.pop();
}

void nuiPainter::LoadMatrix(const nuiMatrix& Matrix)
{
  NGL_ASSERT(!mMatrixStack.empty());
  mMatrixStack.top() = Matrix;
}

void nuiPainter::MultMatrix(const nuiMatrix& Matrix)
{
  NGL_ASSERT(!mMatrixStack.empty());
  mMatrixStack.top() *= Matrix;
}

const nuiMatrix& nuiPainter::GetMatrix() const
{
  return mMatrixStack.top();
}



// Clipping using Scissor :
void nuiPainter::PushClipping()
{
  nuiClipper* pClipper = new nuiClipper(mClip);
  mpClippingStack.push(pClipper);
}

void nuiPainter::PopClipping()
{
  NGL_ASSERT(mpClippingStack.size());

  nuiClipper* pClipper = mpClippingStack.top();
  mClip = *pClipper;
  mpClippingStack.pop();
  delete pClipper;
}

void nuiPainter::Clip(const nuiRect& rRect)
{
  nuiVector p1(rRect.mLeft, rRect.mTop, 0.0f),p2(rRect.mRight, rRect.mBottom, 0.0f);
  nuiMatrix m(GetMatrix());
  p1 = m * p1;
  p2 = m * p2;
  nuiRect l(p1[0], p1[1], p2[0], p2[1], false);
  /*bool res = (unused)*/ mClip.Intersect(mClip,l);
}

void nuiPainter::ResetClipRect()
{
  uint32 w=mWidth, h=mHeight;
  mClip.Set(ToNearest(mClipOffsetX), ToNearest(mClipOffsetY), w, h);
}

void nuiPainter::EnableClipping(bool set)
{
  mClip.mEnabled = set;
}

bool nuiPainter::GetClipRect(nuiRect& rRect, bool LocalRect) const
{
  if (mClip.mEnabled)
  {
    rRect = mClip;
  }
  else
  {
    rRect.Set(0, 0, mWidth, mHeight);
  }
  
  if (LocalRect)
  {
    // Transform the rect with the inverse of the current matrix
    nglMatrixf m(GetMatrix());
    m.InvertHomogenous();
    nglVectorf v1(rRect.Left(), rRect.Top(), 0, 1);
    nglVectorf v2(rRect.Right(), rRect.Bottom(), 0, 1);
    v1 = m * v1;
    v2 = m * v2;
    rRect.Set(v1[0], v1[1], v2[0], v2[1], false);
  }
  return mClip.mEnabled;
}


void nuiPainter::ResetStats()
{
  mRenderOperations = 0;
  mVertices = 0;
  mBatches = 0;
}

uint32 nuiPainter::GetRenderOperations() const
{
  return mRenderOperations;
}

uint32 nuiPainter::GetVertices() const
{
  return mVertices;
}

uint32 nuiPainter::GetBatches() const
{
  return mBatches;
}

uint32 nuiPainter::GetClipStackSize() const
{
  return mpClippingStack.size();
}


uint32 nuiPainter::GetRectangleTextureSupport() const
{
  return 0;
}

void nuiPainter::DEBUG_EnableDrawArray(bool set) const
{
  mEnableDrawArray = set;
}

bool nuiPainter::DEBUG_GetEnableDrawArray() const
{
  return mEnableDrawArray;
}

uint32 nuiPainter::mNeedTextureBackingStore = 0;

void nuiPainter::AddNeedTextureBackingStore()
{
  mNeedTextureBackingStore++;
  if (mNeedTextureBackingStore)
  {
    nuiTexture::RetainBuffers(true);
  }
}

void nuiPainter::DelNeedTextureBackingStore()
{
  mNeedTextureBackingStore--;
  if (!mNeedTextureBackingStore)
  {
    nuiTexture::RetainBuffers(false);
  }
}

nuiSurface* nuiPainter::GetSurface() const
{
  return mpSurface;
}

void nuiPainter::SetSurface(nuiSurface* pSurface)
{
  if (pSurface == mpSurface)
    return;
  
  if (pSurface)
    pSurface->Acquire();
  if (mpSurface)
    mpSurface->Release();
  
  mpSurface = pSurface;
}

void nuiPainter::PushSurface()
{
  if (mpSurface)
    mpSurface->Acquire();
  mpSurfaceStack.push(mpSurface);
}

void nuiPainter::PopSurface()
{
  nuiSurface* pSurface = mpSurfaceStack.top();
  mpSurfaceStack.pop();
  if (pSurface)
  {
    if (mpSurface != pSurface)
      SetSurface(pSurface);
    pSurface->Release();
  }
}


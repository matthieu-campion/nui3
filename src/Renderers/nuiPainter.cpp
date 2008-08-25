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
}

nuiPainter::~nuiPainter() 
{
  // Empty the clip stack:
  while (!mpClippingStack.empty())
  {
    delete mpClippingStack.top();
    mpClippingStack.pop();
  }
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
  mClip.Set(ToNearest(mClipOffsetX), ToNearest(mClipOffsetY), mWidth, mHeight);
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
  mClip.Set(ToNearest(mClipOffsetX), ToNearest(mClipOffsetY), mWidth, mHeight);
}

void nuiPainter::EnableClipping(bool set)
{
  mClip.mEnabled = set;
}

bool nuiPainter::GetClipRect(nuiRect& rRect) const
{
  if (mClip.mEnabled)
  {
    rRect = mClip;
  }
  else
  {
    rRect.Set(0, 0, mWidth, mHeight);
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

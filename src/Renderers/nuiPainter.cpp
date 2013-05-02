/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

///////////////////////////////////
// nuiPainter implementation:
nuiPainter::nuiPainter(nglContext* pContext)
{
  ResetStats();
  mWidth = 0;
  mHeight = 0;
  mMatrixStack.push(nuiMatrix());
  mProjectionMatrixStack.push(nuiMatrix());
  mProjectionViewportStack.push(nuiRect());
  
  mDummyMode = false;
  mpSurface = NULL;

  mAngle=0;

  mEnableDrawArray = true;
}

nuiPainter::~nuiPainter() 
{
  // Empty the clip stack:
  mpClippingStack = std::stack<nuiClipper>();
}

void nuiPainter::StartRendering()
{
  mpClippingStack = std::stack<nuiClipper>();

  uint32 w = mWidth, h = mHeight;
  mClip.Set(0, 0, w, h);
  
  while (!mMatrixStack.empty())
    mMatrixStack.pop();
  mMatrixStack.push(nuiMatrix());

  while (!mProjectionMatrixStack.empty())
  {
    mProjectionMatrixStack.pop();
    mProjectionViewportStack.pop();
  }
  nuiMatrix m;
  m.Translate(-1.0f, 1.0f, 0.0f);
  m.Scale(2.0f/(float)mWidth, -2.0f/(float)mHeight, 1.0f);
  mProjectionMatrixStack.push(m);
  mProjectionViewportStack.push(nuiRect(0, 0, mWidth, mHeight));
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

///////
void nuiPainter::PushProjectionMatrix()
{
  NGL_ASSERT(!mProjectionMatrixStack.empty());
  mProjectionMatrixStack.push(mProjectionMatrixStack.top());
  mProjectionViewportStack.push(mProjectionViewportStack.top());
}

void nuiPainter::PopProjectionMatrix()
{
  NGL_ASSERT(!mProjectionMatrixStack.empty());
  mProjectionMatrixStack.pop();
  mProjectionViewportStack.pop();
}

void nuiPainter::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix)
{
  NGL_ASSERT(!mProjectionMatrixStack.empty());
  mProjectionMatrixStack.top() = rMatrix;

  nuiMatrix LocalMatrix(mMatrixStack.top());
  nuiVector vec1(rViewport.Left(), rViewport.Top(), 0.0f);
  nuiVector vec2(rViewport.Right(), rViewport.Bottom(), 0.0f);
  vec1 = LocalMatrix * vec1;
  vec2 = LocalMatrix * vec2;
  mProjectionViewportStack.top().Set(vec1[0], vec1[1], vec2[0], vec2[1], false);
}

void nuiPainter::MultProjectionMatrix(const nuiMatrix& rMatrix)
{
  NGL_ASSERT(!mProjectionMatrixStack.empty());
  mProjectionMatrixStack.top() *= rMatrix;
}

const nuiMatrix& nuiPainter::GetProjectionMatrix() const
{
  return mProjectionMatrixStack.top();
}

const nuiRect& nuiPainter::GetProjectionViewport() const
{
  return mProjectionViewportStack.top();
}


// Clipping using Scissor :
void nuiPainter::PushClipping()
{
  mpClippingStack.push(mClip);
}

void nuiPainter::PopClipping()
{
  NGL_ASSERT(mpClippingStack.size());

  mClip = mpClippingStack.top();
  mpClippingStack.pop();
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
  mClip.Set(0, 0, w, h);
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

nuiSurface* nuiPainter::GetSurface() const
{
  return mpSurface;
}


void nuiPainter::GetSize(uint32& rX, uint32& rY) const
{
  rX = mWidth;
  rY = mHeight;
}

const nuiRenderState& nuiPainter::GetState() const
{
  return mState;
}

void nuiPainter::SetDummyMode(bool set)
{
  mDummyMode = set;
}

bool nuiPainter::GetDummyMode() const
{
  return mDummyMode;
}

void nuiPainter::SetAngle(int32 Angle)
{
  mAngle = Angle % 360;
  while (mAngle < 0)
    mAngle += 360;
}

int32 nuiPainter::GetAngle() const
{
  if (mpSurface)
    return 0;
  return mAngle;
}

int32 nuiPainter::GetCurrentWidth() const
{
  if (mpSurface)
    return mpSurface->GetWidth();
  return mWidth;
}

int32 nuiPainter::GetCurrentHeight() const
{
  if (mpSurface)
    return mpSurface->GetHeight();
  return mHeight;
}


// Debug:
void nuiPainter::AddBreakPoint()
{
  // do nothing by default, this is only used to debug defered rendering (i.e. nuiMetaPainter).
}


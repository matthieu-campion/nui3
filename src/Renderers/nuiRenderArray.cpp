/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiRenderArray.h"
#include "nuiColor.h"

/// class nuiRenderArray
nuiRenderArray::nuiRenderArray(GLenum mode, bool Static)
{
  //std::vector<float> mFloats[5];
  for (uint i = 0; i < 5; i++)
    mEnabled[i] = false;
  mEnabled[eVertex] = true; // Enable Vertices by default
  mStatic = Static;
  mMode = mode;

  mVertexElements = 2;
  mColorElements = 4;
  mTexCoordElements = 2;

  mpCacheHandle = NULL;
  mpCacheManager = NULL;

  mUseGLAATexture = false;
  
  mCurrentVertex.mX = 0.0f;
  mCurrentVertex.mY = 0.0f;
  mCurrentVertex.mZ = 0.0f;
  mCurrentVertex.mTX = 0.0f;
  mCurrentVertex.mTY = 0.0f;
  mCurrentVertex.mR = 0;
  mCurrentVertex.mG = 0;
  mCurrentVertex.mB = 0;
  mCurrentVertex.mA = 255;
  mCurrentVertex.mEdgeFlag = true;
  
}

nuiRenderArray::nuiRenderArray(const nuiRenderArray& rArray)
: mVertices(rArray.mVertices)
{
  for (uint i = 0; i < 5; i++)
    mEnabled[i] = rArray.mEnabled[i];
  mStatic = rArray.mStatic;
  mMode = rArray.mMode;

  mVertexElements = rArray.mVertexElements;
  mColorElements = rArray.mColorElements;
  mTexCoordElements = rArray.mTexCoordElements;

  mpCacheHandle = NULL;
  mpCacheManager = NULL;

  mUseGLAATexture = rArray.mUseGLAATexture;

  mCurrentVertex = rArray.mCurrentVertex;
}

nuiRenderArray::~nuiRenderArray()
{
  mpCacheHandle = NULL;
  if (mpCacheManager)
    mpCacheManager->ReleaseCacheObject(mpCacheHandle);
}

void nuiRenderArray::SetMode(GLenum mode)
{
  mMode = mode;
}

GLenum nuiRenderArray::GetMode() const
{
  return mMode;
}

void nuiRenderArray::EnableArray(DataType tpe, bool Set)
{
  mEnabled[tpe] = Set;
}

bool nuiRenderArray::IsArrayEnabled(DataType tpe) const
{
  return mEnabled[tpe];
}

void nuiRenderArray::PushVertex()
{
  NGL_ASSERT(mCurrentVertex.mX != std::numeric_limits<float>::infinity());
  NGL_ASSERT(mCurrentVertex.mX != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(mCurrentVertex.mX != std::numeric_limits<float>::quiet_NaN());
  NGL_ASSERT(mCurrentVertex.mY != std::numeric_limits<float>::infinity());
  NGL_ASSERT(mCurrentVertex.mY != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(mCurrentVertex.mY != std::numeric_limits<float>::quiet_NaN());
  NGL_ASSERT(mCurrentVertex.mZ != std::numeric_limits<float>::infinity());
  NGL_ASSERT(mCurrentVertex.mZ != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(mCurrentVertex.mZ != std::numeric_limits<float>::quiet_NaN());

  NGL_ASSERT(mCurrentVertex.mTX != std::numeric_limits<float>::infinity());
  NGL_ASSERT(mCurrentVertex.mTX != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(mCurrentVertex.mTX != std::numeric_limits<float>::quiet_NaN());
  NGL_ASSERT(mCurrentVertex.mTY != std::numeric_limits<float>::infinity());
  NGL_ASSERT(mCurrentVertex.mTY != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(mCurrentVertex.mTY != std::numeric_limits<float>::quiet_NaN());

  mVertices.push_back(mCurrentVertex);
}

void nuiRenderArray::SetEdgeFlag( bool set )
{
  mCurrentVertex.mEdgeFlag = set;
}

uint32 nuiRenderArray::GetSize() const
{
  return (uint32)mVertices.size();
}

void nuiRenderArray::Reserve(uint Count)
{
  mVertices.reserve(Count);
}

void nuiRenderArray::Reset()
{
  mVertices.clear();
}

void* nuiRenderArray::GetCacheHandle(nuiCacheManager* pManager) const
{
  return mpCacheHandle;
}

void nuiRenderArray::SetCacheHandle(nuiCacheManager* pManager, void* pHandle) const
{
  mpCacheManager = pManager;
  mpCacheHandle = pHandle;
}

uint32 nuiRenderArray::GetTotalSize() const
{
  uint32 size = GetSize();
  uint32 vertexsize = sizeof(Vertex);
  return vertexsize * size;
}

void nuiRenderArray::FillBuffer(GLubyte* pBuffer) const
{
  uint32 size = GetSize();
  uint32 bytes = size * sizeof(Vertex);
  memcpy(pBuffer, &mVertices[0], bytes);
}

bool nuiRenderArray::UseGLAATexture() const
{
  return mUseGLAATexture;
}

void nuiRenderArray::UseGLAATexture(bool set)
{
  mUseGLAATexture = set;
}

void nuiRenderArray::SetVertex(float x, float y, float z)
{
  mCurrentVertex.mX = x;
  mCurrentVertex.mY = y;
  mCurrentVertex.mZ = z;
}

void nuiRenderArray::SetVertex(const nuiVector& rVf)
{
  mCurrentVertex.mX = rVf[0];
  mCurrentVertex.mY = rVf[1];
  mCurrentVertex.mZ = rVf[2];
}

void nuiRenderArray::SetVertex(const nuiVector3& rV3f)
{
  mCurrentVertex.mX = rV3f[0];
  mCurrentVertex.mY = rV3f[1];
  mCurrentVertex.mZ = rV3f[2];
}

void nuiRenderArray::SetVertex(const nuiVector2& rV2f)
{
  mCurrentVertex.mX = rV2f[0];
  mCurrentVertex.mY = rV2f[1];
  mCurrentVertex.mZ = 0;
}

void nuiRenderArray::SetColor(float r, float g, float b, float a)
{
  NGL_ASSERT(r <= 1.0);
  NGL_ASSERT(r >= 0.0);
  NGL_ASSERT(g <= 1.0);
  NGL_ASSERT(g >= 0.0);
  NGL_ASSERT(b <= 1.0);
  NGL_ASSERT(b >= 0.0);
  NGL_ASSERT(a <= 1.0);
  NGL_ASSERT(a >= 0.0);
  NGL_ASSERT(r != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(r != std::numeric_limits<float>::quiet_NaN());
  NGL_ASSERT(g != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(g != std::numeric_limits<float>::quiet_NaN());
  NGL_ASSERT(b != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(b != std::numeric_limits<float>::quiet_NaN());
  NGL_ASSERT(a != std::numeric_limits<float>::signaling_NaN());
  NGL_ASSERT(a != std::numeric_limits<float>::quiet_NaN());

  mCurrentVertex.mR = (uint8)ToBelow(r * 255.0f);
  mCurrentVertex.mG = (uint8)ToBelow(g * 255.0f);
  mCurrentVertex.mB = (uint8)ToBelow(b * 255.0f);
  mCurrentVertex.mA = (uint8)ToBelow(a * 255.0f);
}

void nuiRenderArray::SetColor(uint8 r, uint8 g, uint8 b, uint8 a)
{
  mCurrentVertex.mR = r;
  mCurrentVertex.mG = g;
  mCurrentVertex.mB = b;
  mCurrentVertex.mA = a;
}

void nuiRenderArray::SetColor(const nuiColor& rColor)
{
  SetColor(rColor.Red(), rColor.Green(), rColor.Blue(), rColor.Alpha());
}

void nuiRenderArray::SetTexCoords(float tx, float ty)
{
  mCurrentVertex.mTX = tx;
  mCurrentVertex.mTY = ty;
}

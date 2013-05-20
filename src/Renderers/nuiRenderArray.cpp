/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"



/// class nuiRenderArray
nuiRenderArray::nuiRenderArray(uint32 mode, bool Static, bool _3dmesh, bool _shape)
{
  Acquire();
  
  for (uint i = 0; i < 4; i++)
    mEnabled[i] = false;
  mEnabled[eVertex] = true; // Enable Vertices by default
  mStatic = Static;
  mMode = mode;
#ifdef GL_RECT
  NGL_ASSERT(mode != GL_RECT); // GL_RECT Can
#endif
  m3DMesh = _3dmesh;
  mShape = _shape;

  mpCacheHandle = NULL;
  mpCacheManager = NULL;

  mCurrentVertex.mX = 0.0f;
  mCurrentVertex.mY = 0.0f;
  mCurrentVertex.mZ = 0.0f;
  mCurrentVertex.mW = 1.0f;
  mCurrentVertex.mTX = 0.0f;
  mCurrentVertex.mTY = 0.0f;
  mCurrentVertex.mR = 0;
  mCurrentVertex.mG = 0;
  mCurrentVertex.mB = 0;
  mCurrentVertex.mA = 255;
  mCurrentVertex.mNX = 0;
  mCurrentVertex.mNY = 0;
  mCurrentVertex.mNZ = 1;
  mCurrentVertex.mNW = 0;

}

nuiRenderArray::nuiRenderArray(const nuiRenderArray& rArray)
: mVertices(rArray.mVertices)
{
  Acquire();
  
  for (uint i = 0; i < 5; i++)
    mEnabled[i] = rArray.mEnabled[i];
  mStatic = rArray.mStatic;
  mMode = rArray.mMode;
  m3DMesh = rArray.m3DMesh;
  mShape = rArray.mShape;

  mpCacheHandle = NULL;
  mpCacheManager = NULL;

  mCurrentVertex = rArray.mCurrentVertex;
}

nuiRenderArray::~nuiRenderArray()
{
  if (mpCacheManager)
    mpCacheManager->ReleaseCacheObject(mpCacheHandle);
  mpCacheHandle = NULL;
  
  for (uint32 i = 0; i < mIndexedArrays.size(); i++)
    delete mIndexedArrays[i];
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
  NGL_ASSERT(!std::isnan(mCurrentVertex.mX));
  NGL_ASSERT(mCurrentVertex.mY != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mY));
  NGL_ASSERT(mCurrentVertex.mZ != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mZ));

  NGL_ASSERT(mCurrentVertex.mTX != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mTX));
  NGL_ASSERT(mCurrentVertex.mTY != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mTY));

  NGL_ASSERT(mCurrentVertex.mNX != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mNX));
  NGL_ASSERT(mCurrentVertex.mNY != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mNY));
  NGL_ASSERT(mCurrentVertex.mNZ != std::numeric_limits<float>::infinity());
  NGL_ASSERT(!std::isnan(mCurrentVertex.mNZ));

  // Grow the bounding rect:
  UpdateBounds(mCurrentVertex.mX, mCurrentVertex.mY, mCurrentVertex.mZ);

  mVertices.push_back(mCurrentVertex);
}

uint32 nuiRenderArray::GetSize() const
{
  return (uint32)mVertices.size();
}

void nuiRenderArray::Reserve(uint Count)
{
  mVertices.reserve(Count);
}

void nuiRenderArray::Resize(uint Count)
{
  mVertices.resize(Count);
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
  NGL_ASSERT(!std::isnan(r));
  NGL_ASSERT(!std::isnan(g));
  NGL_ASSERT(!std::isnan(b));
  NGL_ASSERT(!std::isnan(a));

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

void nuiRenderArray::SetColor(uint32 Color)
{
  uint8* pCol = (uint8*)&Color;
  SetColor(pCol[0], pCol[1], pCol[2], pCol[3]);
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

void nuiRenderArray::SetTexCoords(const nglVector2f& rV)
{
  SetTexCoords(rV[0], rV[1]);
}

void nuiRenderArray::SetNormal(float x, float y, float z)
{
  mCurrentVertex.mNX = x;
  mCurrentVertex.mNY = y;
  mCurrentVertex.mNZ = z;
}

void nuiRenderArray::SetNormal(const nuiVector& rVf)
{
  mCurrentVertex.mNX = rVf[0];
  mCurrentVertex.mNY = rVf[1];
  mCurrentVertex.mNZ = rVf[2];
}

void nuiRenderArray::SetNormal(const nuiVector3& rV3f)
{
  mCurrentVertex.mNX = rV3f[0];
  mCurrentVertex.mNY = rV3f[1];
  mCurrentVertex.mNZ = rV3f[2];
}

bool nuiRenderArray::Is3DMesh() const
{
  return m3DMesh;
}

void nuiRenderArray::Set3DMesh(bool set)
{
  m3DMesh = set;
}

bool nuiRenderArray::IsShape() const
{
  return mShape;
}

void nuiRenderArray::SetShape(bool set)
{
  mShape = set;
}

bool nuiRenderArray::IsStatic() const
{
  return mStatic;
}

void nuiRenderArray::SetStatic(bool set)
{
  mStatic = set;
}


//////////////
// Indexed accessors:
void nuiRenderArray::SetVertex(uint32 index, float x, float y, float z)
{
  // Grow the bounding rect:
  UpdateBounds(x, y, z);
  mVertices[index].mX = x;
  mVertices[index].mY = y;
  mVertices[index].mZ = z;
  mVertices[index].mW = 1;
}

void nuiRenderArray::SetVertex(uint32 index, const nuiVector& rVf)
{
  UpdateBounds(rVf[0], rVf[1], rVf[2]);
  mVertices[index].mX = rVf[0];
  mVertices[index].mY = rVf[1];
  mVertices[index].mZ = rVf[2];
  mVertices[index].mW = rVf[3];
}

void nuiRenderArray::SetVertex(uint32 index, const nuiVector3& rV3f)
{
  UpdateBounds(rV3f[0], rV3f[1], rV3f[2]);
  mVertices[index].mX = rV3f[0];
  mVertices[index].mY = rV3f[1];
  mVertices[index].mZ = rV3f[2];
  mVertices[index].mW = 1;
}

void nuiRenderArray::SetVertex(uint32 index, const nuiVector2& rV2f)
{
  UpdateBounds(rV2f[0], rV2f[1], 0);
  mVertices[index].mX = rV2f[0];
  mVertices[index].mY = rV2f[1];
  mVertices[index].mZ = 0;
  mVertices[index].mW = 1;
}

void nuiRenderArray::SetColor(uint32 index, float r, float g, float b, float a)
{
  NGL_ASSERT(r <= 1.0);
  NGL_ASSERT(r >= 0.0);
  NGL_ASSERT(g <= 1.0);
  NGL_ASSERT(g >= 0.0);
  NGL_ASSERT(b <= 1.0);
  NGL_ASSERT(b >= 0.0);
  NGL_ASSERT(a <= 1.0);
  NGL_ASSERT(a >= 0.0);
  NGL_ASSERT(!std::isnan(r));
  NGL_ASSERT(!std::isnan(g));
  NGL_ASSERT(!std::isnan(b));
  NGL_ASSERT(!std::isnan(a));
  
  mVertices[index].mR = (uint8)ToBelow(r * 255.0f);
  mVertices[index].mG = (uint8)ToBelow(g * 255.0f);
  mVertices[index].mB = (uint8)ToBelow(b * 255.0f);
  mVertices[index].mA = (uint8)ToBelow(a * 255.0f);
}

void nuiRenderArray::SetColor(uint32 index, uint8 r, uint8 g, uint8 b, uint8 a)
{
  mVertices[index].mR = r;
  mVertices[index].mG = g;
  mVertices[index].mB = b;
  mVertices[index].mA = a;
}

void nuiRenderArray::SetColor(uint32 index, uint32 Color)
{
  uint8* pCol = (uint8*)&Color;
  SetColor(index, pCol[0], pCol[1], pCol[2], pCol[3]);
}


void nuiRenderArray::SetColor(uint32 index, const nuiColor& rColor)
{
  SetColor(index, rColor.Red(), rColor.Green(), rColor.Blue(), rColor.Alpha());
}

void nuiRenderArray::SetTexCoords(uint32 index, float tx, float ty)
{
  mVertices[index].mTX = tx;
  mVertices[index].mTY = ty;
}

void nuiRenderArray::SetNormal(uint32 index, float x, float y, float z)
{
  mVertices[index].mNX = x;
  mVertices[index].mNY = y;
  mVertices[index].mNZ = z;
  mVertices[index].mNW = 0;
}

void nuiRenderArray::SetNormal(uint32 index, const nuiVector& rVf)
{
  mVertices[index].mNX = rVf[0];
  mVertices[index].mNY = rVf[1];
  mVertices[index].mNZ = rVf[2];
  mVertices[index].mNW = 0;
}

void nuiRenderArray::SetNormal(uint32 index, const nuiVector3& rV3f)
{
  mVertices[index].mNX = rV3f[0];
  mVertices[index].mNY = rV3f[1];
  mVertices[index].mNZ = rV3f[2];
  mVertices[index].mNW = 0;
}


//////////////////////////
// Indexed rendering
nuiRenderArray::IndexArray::IndexArray(GLenum mode, uint32 reserve_count, bool resize_reserve)
{
  mMode = mode;
  if (reserve_count)
  {
    if (resize_reserve)
      mIndices.resize(reserve_count);
    else
      mIndices.reserve(reserve_count);
  }
}

void nuiRenderArray::AddIndicesArray(uint32 mode, uint32 reserve_count, bool resize_reserve)
{
  mIndexedArrays.push_back(new IndexArray(mode, reserve_count, resize_reserve));
}

nuiRenderArray::IndexArray& nuiRenderArray::GetIndexArray(uint32 ArrayIndex)
{
  return *mIndexedArrays[ArrayIndex];
}

uint32 nuiRenderArray::GetIndexArrayCount() const
{
  return mIndexedArrays.size();
}

void nuiRenderArray::PushIndex(uint32 VertexIndex)
{
  uint32 size = mIndexedArrays.size();
  mIndexedArrays[size - 1]->mIndices.push_back(VertexIndex);
}

void nuiRenderArray::PushIndex(uint32 ArrayIndex, uint32 VertexIndex)
{
  mIndexedArrays[ArrayIndex]->mIndices.push_back(VertexIndex);
}

void nuiRenderArray::SetIndex(uint32 IndexInArray, uint32 VertexIndex)
{
  uint32 size = mIndexedArrays.size();
  mIndexedArrays[size - 1]->mIndices[IndexInArray] = VertexIndex;
}

void nuiRenderArray::SetIndex(uint32 ArrayIndex, uint32 IndexInArray, uint32 VertexIndex)
{
  mIndexedArrays[ArrayIndex]->mIndices[IndexInArray] = VertexIndex;
}

void nuiRenderArray::GetBounds(float *pBounds) const
{
  pBounds[0] = mMinX;
  pBounds[1] = mMinY;
  pBounds[2] = mMinZ;
  pBounds[3] = mMaxX;
  pBounds[4] = mMaxY;
  pBounds[5] = mMaxZ;
}

void nuiRenderArray::UpdateBounds(float x, float y, float z)
{
  if (mVertices.empty())
  {
    mMinX = x;
    mMinY = y;
    mMinZ = z;
    mMaxX = x;
    mMaxY = y;
    mMaxZ = z;
  }
  else
  {
    mMinX = MIN(mMinX, x);
    mMinY = MIN(mMinY, y);;
    mMinZ = MIN(mMinY, z);;
    mMaxX = MAX(mMaxX, x);
    mMaxY = MAX(mMaxY, y);
    mMaxZ = MAX(mMaxZ, z);
  }
}

nglString nuiRenderArray::Dump() const
{
  nglString str;
  for (int i = 0; i < mVertices.size(); i++)
  {
    nglString f;
    f.CFormat(_T("%d:  %3f %3f - %3f %3f (%3f %3f)"), i, mVertices[i].mX, mVertices[i].mY, mVertices[i].mTX, mVertices[i].mTY, mVertices[i].mTX * 2, mVertices[i].mTY * 2);
    NGL_OUT(_T("%s\n"), f.GetChars());
  }
  
  NGL_OUT(_T("\n"));
  return str;
}

////////////
//class StreamDesc
nuiRenderArray::StreamDesc::StreamDesc(int32 StreamID, int32 count_per_vertex, int32 vertex_count, const float* pData, bool CopyData, bool Normalize)
{
  mStreamID = StreamID;
  mType = eFloat;
  mCount = count_per_vertex;
  mOwnData = CopyData;
  mNormalize = Normalize;

  if (CopyData)
  {
    int32 s = vertex_count * count_per_vertex;
    mData.mpFloats = new float[s];
    memcpy(const_cast<float*>(mData.mpFloats), pData, sizeof(float) * s);
  }
  else
  {
    mData.mpFloats = pData;
  }
}

nuiRenderArray::StreamDesc::StreamDesc(int32 StreamID, int32 count_per_vertex, int32 vertex_count, const int32* pData, bool CopyData, bool Normalize)
{
  mStreamID = StreamID;
  mType = eInt;
  mCount = count_per_vertex;
  mOwnData = CopyData;
  mNormalize = Normalize;

  if (CopyData)
  {
    int32 s = vertex_count * count_per_vertex;
    mData.mpInts = new int32[s];
    memcpy(const_cast<int32*>(mData.mpInts), pData, sizeof(int32) * s);
  }
  else
  {
    mData.mpInts = pData;
  }
}

nuiRenderArray::StreamDesc::StreamDesc(int32 StreamID, int32 count_per_vertex, int32 vertex_count, const uint8* pData, bool CopyData, bool Normalize)
{
  mStreamID = StreamID;
  mType = eByte;
  mCount = count_per_vertex;
  mOwnData = CopyData;
  mNormalize = Normalize;

  if (CopyData)
  {
    int32 s = vertex_count * count_per_vertex;
    mData.mpBytes = new uint8[s];
    memcpy(const_cast<uint8*>(mData.mpBytes), pData, sizeof(int32) * s);
  }
  else
  {
    mData.mpBytes = pData;
  }
}

nuiRenderArray::StreamDesc::~StreamDesc()
{
  if (mOwnData)
  {
    switch (mType)
    {
      case eFloat:
        delete[] mData.mpFloats;
        break;
      case eInt:
        delete[] mData.mpInts;
        break;
      case eByte:
        delete[] mData.mpBytes;
        break;
    }
  }
  mData.mpBytes = NULL;
}


int32 nuiRenderArray::AddStream(int32 StreamID, int32 count_per_vertex, const float* pData, bool CopyData, bool Normalize)
{
  mStreams.push_back(new StreamDesc(StreamID, count_per_vertex, GetSize(), pData, CopyData, Normalize));
}

int32 nuiRenderArray::AddStream(int32 StreamID, int32 count_per_vertex, const int32* pData, bool CopyData, bool Normalize)
{
  mStreams.push_back(new StreamDesc(StreamID, count_per_vertex, GetSize(), pData, CopyData, Normalize));
}

int32 nuiRenderArray::AddStream(int32 StreamID, int32 count_per_vertex, const uint8* pData, bool CopyData, bool Normalize)
{
  mStreams.push_back(new StreamDesc(StreamID, count_per_vertex, GetSize(), pData, CopyData, Normalize));
}

const nuiRenderArray::StreamDesc& nuiRenderArray::GetStream(int32 index) const
{
  NGL_ASSERT(mStreams.size() > index);
  return *mStreams[index];
}

int32 nuiRenderArray::GetStreamCount() const
{
  return mStreams.size();
}



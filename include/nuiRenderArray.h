/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRenderArray_h__
#define __nuiRenderArray_h__

//#include "nui.h"
#include "nuiColor.h"
#include "nglVector.h"

class NUI_API nuiCacheManager
{
public:
  nuiCacheManager() {}
  virtual ~nuiCacheManager() {}
  virtual void ReleaseCacheObject(void* pHandle) = 0;
};

class NUI_API nuiRenderArray : public nuiRefCount
{
public:
  nuiRenderArray(uint32 mode, bool Static = false, bool _3dmesh = false, bool _shape = false);
  nuiRenderArray(const nuiRenderArray& rArray);
  virtual ~nuiRenderArray();

  struct Vertex
  {
    GLfloat mX;
    GLfloat mY;
    GLfloat mZ;
    GLfloat mW;

    GLfloat mTX;
    GLfloat mTY;

    GLfloat mNX;
    GLfloat mNY;
    GLfloat mNZ;
    GLfloat mNW;

    GLubyte mR;
    GLubyte mG;
    GLubyte mB;
    GLubyte mA;
  };

  enum StreamType
  {
    eFloat = GL_FLOAT,
    eInt = GL_INT,
    eByte = GL_UNSIGNED_BYTE,
  };

  class StreamDesc
  {
  public:
    StreamDesc(int32 StreamID, int32 count_per_vertex, int32 vertex_count, const float* pData, bool CopyData, bool Normalize);
    StreamDesc(int32 StreamID, int32 count_per_vertex, int32 vertex_count, const int32* pData, bool CopyData, bool Normalize);
    StreamDesc(int32 StreamID, int32 count_per_vertex, int32 vertex_count, const uint8* pData, bool CopyData, bool Normalize);
    ~StreamDesc();

    int32 mStreamID;
    StreamType mType;
    int32 mCount;
    bool mOwnData;
    bool mNormalize;

    union
    {
      const float* mpFloats;
      const int32* mpInts;
      const uint8* mpBytes;
    } mData;

  };

  enum DataType
  {
    eVertex = 0,
    eColor,
    eTexCoord,
    eNormal
  };

  class IndexArray
  {
  public:
    IndexArray(GLenum mode, uint32 reserve_count, bool resize_reserve);
    GLenum mMode;
#ifdef _UIKIT_
    std::vector<GLushort> mIndices;
#else
    std::vector<GLuint> mIndices;
#endif
  };
  
  std::vector<Vertex>& GetVertices()
  { 
    return mVertices; 
  }
  
  const std::vector<Vertex>& GetVertices() const
  { 
    return mVertices; 
  }
  
  Vertex& GetVertex(int32 index)
  { 
    return mVertices[index];
  }
  
  const Vertex& GetVertex(int32 index) const
  { 
    return mVertices[index];
  }
  
  Vertex& operator[](int32 index)
  { 
    return mVertices[index];
  }
  
  const Vertex& operator[](int32 index) const
  { 
    return mVertices[index];
  }

  int32 AddStream(const StreamDesc& rDesc);
    
  void SetMode(GLenum mode); ///< GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES
  GLenum GetMode() const; ///< GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES

  void EnableArray(DataType tpe, bool Set = true);
  bool IsArrayEnabled(DataType tpe) const;
  
  bool Is3DMesh() const;
  void Set3DMesh(bool set);
    bool IsShape() const;
  void SetShape(bool set);
  bool IsStatic() const;
  void SetStatic(bool set);


  uint32 GetSize() const;
  void Reserve(uint Count);
  void Resize(uint Count);
  void Reset();
  void* GetCacheHandle(nuiCacheManager* pManager) const;
  void SetCacheHandle(nuiCacheManager* pManager, void* pHandle) const;
  uint32 GetTotalSize() const;
  void FillBuffer(GLubyte* pBuffer) const;

  void SetVertex(float x, float y, float z = 0.0f);
  void SetVertex(const nuiVector& rVf);
  void SetVertex(const nuiVector3& rV3f);
  void SetVertex(const nuiVector2& rV2f);
  void SetColor(float r, float g, float b, float a);
  void SetColor(uint8 r, uint8 g, uint8 b, uint8 a);
  void SetColor(const nuiColor& rColor);
  void SetColor(uint32 Color);
  void SetTexCoords(float tx, float ty);
  void SetTexCoords(const nglVector2f& rV);
  void SetNormal(float x, float y, float z);
  void SetNormal(const nuiVector& rVf);
  void SetNormal(const nuiVector3& rV3f);

  void SetVertex(uint32 index, float x, float y, float z = 0.0f);
  void SetVertex(uint32 index, const nuiVector& rVf);
  void SetVertex(uint32 index, const nuiVector3& rV3f);
  void SetVertex(uint32 index, const nuiVector2& rV2f);
  void SetColor(uint32 index, float r, float g, float b, float a);
  void SetColor(uint32 index, uint8 r, uint8 g, uint8 b, uint8 a);
  void SetColor(uint32 index, const nuiColor& rColor);
  void SetColor(uint32 index, uint32 Color);
  void SetTexCoords(uint32 index, float tx, float ty);
  void SetNormal(uint32 index, float x, float y, float z);
  void SetNormal(uint32 index, const nuiVector& rVf);
  void SetNormal(uint32 index, const nuiVector3& rV3f);

  void PushVertex();
  
  IndexArray& GetIndexArray(uint32 ArrayIndex);
  void AddIndicesArray(uint32 mode, uint32 reserve_count = 0, bool resize_reserve = false);
  uint32 GetIndexArrayCount() const;
  void PushIndex(uint32 VertexIndex);
  void PushIndex(uint32 ArrayIndex, uint32 VertexIndex);
  void SetIndex(uint32 IndexInArray, uint32 VertexIndex);
  void SetIndex(uint32 ArrayIndex, uint32 IndexInArray, uint32 VertexIndex);

  void GetBounds(float* bounds) const; ///< bounds must contain at least 6 floats to store the minums and maximums coordinates of this array

  int32 AddStream(int32 StreamID, int32 count_per_vertex, const float* pData, bool CopyData, bool Normalize = false);
  int32 AddStream(int32 StreamID, int32 count_per_vertex, const int32* pData, bool CopyData, bool Normalize = false);
  int32 AddStream(int32 StreamID, int32 count_per_vertex, const uint8* pData, bool CopyData, bool Normalize = false);

  const StreamDesc& GetStream(int32 index) const;
  int32 GetStreamCount() const;

  nglString Dump() const;
private:
  GLenum mMode;
  bool mEnabled[4];
  bool mStatic : 1;
  bool m3DMesh : 1;
  bool mShape : 1;
  mutable void* mpCacheHandle;
  mutable nuiCacheManager* mpCacheManager;

  Vertex mCurrentVertex;
  std::vector<Vertex> mVertices;
  std::vector<StreamDesc*> mStreams;
  
  nuiRect mBounds;

  std::vector<IndexArray*> mIndexedArrays;

  void UpdateBounds(float x, float y, float z);
  float mMinX;
  float mMinY;
  float mMinZ;
  float mMaxX;
  float mMaxY;
  float mMaxZ;
};

class NUI_API nuiRenderObject
{
public:
  nuiRenderObject()
  {
  }

  nuiRenderObject(const nuiRenderObject& rObject)
  {
    size_t size = rObject.GetSize();
    mpArrays.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
      AddArray(rObject.GetArray((uint)i));
    }
  }

  ~nuiRenderObject()
  {
    for (uint i = 0; i < mpArrays.size(); i++)
      mpArrays[i]->Release();
  }

  void AddArray(nuiRenderArray* pArray)
  {
    pArray->Acquire();
    mpArrays.push_back(pArray);
  }

  nuiRenderArray* GetArray(uint i) const
  {
    NGL_ASSERT(i < mpArrays.size());
    return mpArrays[i];
  }

  nuiRenderArray* GetLastArray() const
  {
    NGL_ASSERT(!mpArrays.empty());
    return mpArrays.back();
  }

  uint32 GetSize() const
  {
    return (uint32)mpArrays.size();
  }

private:
  std::vector<nuiRenderArray*> mpArrays;
};

#endif //__nuiRenderArray_h__


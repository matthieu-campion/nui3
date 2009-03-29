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
  nuiRenderArray(uint32 mode, bool Static = false, bool _3dmesh = false);
  nuiRenderArray(const nuiRenderArray& rArray);
  virtual ~nuiRenderArray();

  struct Vertex
  {
    GLfloat mX;
    GLfloat mY;
    GLfloat mZ;
    GLubyte mR;
    GLubyte mG;
    GLubyte mB;
    GLubyte mA;
    GLfloat mTX;
    GLfloat mTY;
  };

  enum DataType
  {
    eVertex = 0,
    eColor,
    eTexCoord,
    eNormal
  };

  inline std::vector<Vertex>& GetVertices()
  { 
    return mVertices; 
  }
  
  inline const std::vector<Vertex>& GetVertices() const
  { 
    return mVertices; 
  }
  
  void SetMode(GLenum mode); ///< GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON
  GLenum GetMode() const; ///< GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON

  void EnableArray(DataType tpe, bool Set = true);
  bool IsArrayEnabled(DataType tpe) const;
  
  bool Is3DMesh() const;
  void Set3DMesh(bool set);

  uint32 GetSize() const;
  void Reserve(uint Count);
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
  
  void PushVertex();
private:
  uint mVertexElements;
  uint mColorElements;
  uint mTexCoordElements;

  GLenum mMode;
  bool mEnabled[4];
  bool mStatic;
  bool m3DMesh;
  mutable void* mpCacheHandle;
  mutable nuiCacheManager* mpCacheManager;

  Vertex mCurrentVertex;
  std::vector<Vertex> mVertices;
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


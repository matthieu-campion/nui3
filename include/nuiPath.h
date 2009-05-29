/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPath_h__
#define __nuiPath_h__

//#include "nui.h"
#include "nuiPoint.h"

class NUI_API nuiPath
{
public:
  nuiPath();
  nuiPath(const nuiPath& rPath);

  ~nuiPath();

  void StopPath();
  int AddVertex(const nuiPoint& rPoint);
  int AddVertexOptim(const nuiPoint& rPoint); // Only add the vertex if it is not already the last vertex in this path
  void AddPath(const nuiPath& rPath);
  void AddPathOptim(const nuiPath& rPath);
  void Reverse();
  void InsertVertex(int32 index, const nuiPoint& rPoint);
  void SetVertex(int32 index, const nuiPoint& rPoint);
  void DelVertex(int32 index);
  const nuiPoint& GetVertex(int32 index) const;
  nuiPoint& GetVertex(int32 index);
  uint32 GetCount() const;
  bool IsEmpty() const;
  void Reserve(uint32 count);

  const nuiPoint& operator[] (int32 index) const;
  nuiPoint& operator[] (int32 index);

  const nuiPoint& Back() const;
  nuiPoint& Back();
  const nuiPoint& Front() const;
  nuiPoint& Front();

  bool IsClosed() const;

  void Clear();

  float GetDistanceFromPoint(float X, float Y) const;
  
private:
  std::vector<nuiPoint> mVertices;
};

#endif // #ifndef __nuiPath_h__

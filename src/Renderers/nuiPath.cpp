/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiPath::nuiPath()
{
}

nuiPath::nuiPath(const nuiPath& rPath)
{
  AddPath(rPath);
}

nuiPath::~nuiPath()
{
}

void nuiPath::StopPath()
{
  AddVertex(nuiPoint(nuiPointTypeStop));
}

int nuiPath::AddVertex(const nuiPoint& rPoint)
{
  mVertices.push_back(rPoint);
  return mVertices.size()-1;
}

int nuiPath::AddVertexOptim(const nuiPoint& rPoint)
{
  if (!rPoint.IsValid())
  {
    NGL_OUT(_T("Invalid path point: {%f, %f, %f, %f}\n"), rPoint[0], rPoint[1], rPoint[2], rPoint[3]);
    NGL_ASSERT(false);
  }
  if (mVertices.empty() || !(rPoint == mVertices.back()))
    mVertices.push_back(rPoint);
  return mVertices.size()-1;
}

void nuiPath::AddPath(const nuiPath& rPath)
{
  mVertices.insert(mVertices.end(), rPath.mVertices.begin(), rPath.mVertices.end());
}

void nuiPath::AddPathOptim(const nuiPath& rPath)
{
  uint toadd = rPath.GetCount();
  for (uint i = 0; i < toadd; i++)
    AddVertexOptim(rPath[i]);
}

void nuiPath::Reverse()
{
  reverse(mVertices.begin(), mVertices.end());
}

void nuiPath::InsertVertex(int32 index, const nuiPoint& rPoint)
{
  index %= mVertices.size();
  mVertices.insert(mVertices.begin()+index, rPoint);
}

void nuiPath::SetVertex(int32 index, const nuiPoint& rPoint)
{
  index %= mVertices.size();
  mVertices[index] = rPoint;
}

void nuiPath::DelVertex(int32 index)
{
  index %= mVertices.size();
  mVertices.erase(mVertices.begin() + index);
}

const nuiPoint& nuiPath::GetVertex(int32 index) const
{
  index %= mVertices.size();
  return mVertices[index];
}

nuiPoint& nuiPath::GetVertex(int32 index)
{
  index %= mVertices.size();
  return mVertices[index];
}

uint32 nuiPath::GetCount() const
{
  return mVertices.size();
}

void nuiPath::Reserve(uint32 count)
{
  if (count >= mVertices.size())
    return;

  mVertices.reserve(count);
}


const nuiPoint& nuiPath::operator[] (int32 index) const
{
  return GetVertex(index);
}

nuiPoint& nuiPath::operator[] (int32 index)
{
  return GetVertex(index);
}

void nuiPath::Clear()
{
  mVertices.clear();
}

const nuiPoint& nuiPath::Back() const
{
  NGL_ASSERT(!mVertices.empty());
  return mVertices[mVertices.size()-1];
}

nuiPoint& nuiPath::Back()
{
  NGL_ASSERT(!mVertices.empty());
  return mVertices[mVertices.size()-1];
}

const nuiPoint& nuiPath::Front() const
{
  NGL_ASSERT(!mVertices.empty());
  return mVertices[0];
}

nuiPoint& nuiPath::Front()
{
  NGL_ASSERT(!mVertices.empty());
  return mVertices[0];
}

bool nuiPath::IsClosed() const
{
  if (GetCount() < 3)
    return false;

  return Back() == Front();
}

bool nuiPath::IsEmpty() const
{
  return mVertices.empty();
}

#undef max

float nuiPath::GetDistanceFromPoint(float X, float Y) const
{
  if (mVertices.empty())
    return std::numeric_limits<float>::infinity();
  
  float dist = std::numeric_limits<float>::max();
  int32 size = mVertices.size();
  int32 total = size + (IsClosed() ? 0 : -1);
  for (int32 i = 0; i < total; i++)
  {
    int32 index1 = i % size;
    int32 index2 = (i + 1) % size;
    
    const nuiPoint& rP1(mVertices[index1]);
    const nuiPoint& rP2(mVertices[index2]);
    
    if (rP2.GetType() == nuiPointTypeStop)
      break;
    
    dist = MIN(dist, nuiDistanceFromSegment(X, Y, rP1[0], rP1[1] , rP2[0], rP2[1]));
    
    if (dist == 0.0f)
      return dist;
  }
  
  return dist;
}


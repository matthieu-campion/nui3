/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiPath.h"

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
  AddVertex(nuiPointTypeStop);
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
    NGL_OUT(_T("Invalid point: {%f, %f, %f, %f}\n"), rPoint[0], rPoint[1], rPoint[2], rPoint[3]);
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

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


// nuiRectPath:
nuiRectPath::nuiRectPath(const nuiRect& rRect, bool CCW)
{
  mRect = rRect;
  mCCW = CCW;
}

nuiRectPath::~nuiRectPath()
{
}

bool nuiRectPath::Tessellate(nuiPath& rVertices, float Quality) const
{
  if (mCCW)
  {
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
  }
  else
  {
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
  }
  return true;
}

nuiPoint nuiRectPath::GetStartPoint() const
{
  return nuiPoint(mRect.mLeft, mRect.Top());
}

nuiPoint nuiRectPath::GetEndPoint() const
{
  return nuiPoint(mRect.mLeft, mRect.Top());
}


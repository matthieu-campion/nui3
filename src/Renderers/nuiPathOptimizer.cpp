/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


// nuiPathOptimizer.cpp

#include "nui.h"

nuiPathOptimizer::nuiPathOptimizer(nuiPathGenerator* pPath, float AngleThreshold, float DistThreshold)
{
  mpPath = pPath;
  mAngleThreshold = AngleThreshold;
  mDistThreshold = DistThreshold;

}

nuiPathOptimizer::~nuiPathOptimizer()
{
  delete mpPath;
}

bool nuiPathOptimizer::Tessellate(nuiPath& rVertices, float Quality) const
{
  nuiPath Points;

  mpPath->Tessellate(Points, Quality);

  // Remove unneeded points:
  nuiPoint p1,p2 = Points.Front(),p3;
  rVertices.AddVertex(p2);
  bool first = true;
  bool added = false;
  for (uint32 i = 0; i < Points.GetCount(); i++)
  {
    p3= Points[i];
    if (!first)
    {
      nuiVector v1 = p2;
      v1 -= p1;
      nuiVector v2 = p3;
      v2 -= p2;

      float prodscal = v1 * v2;
      float angle = (float)acos(prodscal / (v1.Length() * v2.Length()));
      if ( angle > mAngleThreshold && v2.Length() >= mDistThreshold)
      {
        rVertices.AddVertex(p3);
        p1 = p2;
        p2 = p3;
        added = true;
      }
      else
      {
        added = false;
      }
    }
    else
    {
      first = false;
      rVertices.AddVertex(p3);
      p1 = p2;
      p2 = p3;
    }
  }
  if (!added)
    rVertices.AddVertex(Points.Back());

  return true;
}



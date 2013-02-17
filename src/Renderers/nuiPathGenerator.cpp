/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

// nuiPathGenerator
nuiPathGenerator::nuiPathGenerator()
{
}

nuiPathGenerator::~nuiPathGenerator()
{
}

nuiPoint nuiPathGenerator::GetStartPoint() const
{
  return nuiPoint::Zero;
}

nuiPoint nuiPathGenerator::GetEndPoint() const
{
  return nuiPoint::Zero;
}


nuiRect nuiPathGenerator::GetRect()
{
  float minx, maxx;
  float miny, maxy;
  bool first = true;

  nuiPath points;
  Tessellate(points, 0.01f);

  for (uint32 i = 0; i < points.GetCount(); i++)
  {
    const nuiPoint& rP = points[i];
    float x = rP.Elt[0];
    float y = rP.Elt[1];
    if (!first)
    {
      minx = MIN(minx,x);
      maxx = MAX(maxx,x);
      miny = MIN(miny,y);
      maxy = MAX(maxy,y);
    }
    else
    {
      minx = maxx = x;
      miny = maxy = y;
      first = false;
    }
  }

  return nuiRect(minx, miny, maxx, maxy, false);
}

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPolyLine_h__
#define __nuiPolyLine_h__

//#include "nui.h"
#include "nuiPoint.h"
#include "nuiPathGenerator.h"

// nuiPolyLines:
class NUI_API nuiPolyLine : public nuiPathGenerator
{
public:
  nuiPolyLine(const nuiPath& rVertices);
  nuiPolyLine(const nuiPoint& rVertex);
  virtual ~nuiPolyLine();

  virtual bool Tessellate(nuiPath& rVertices, float Quality = 0.5f) const;

  void AddLines(const nuiPath& rVertices);
  void LineTo(const nuiPoint& rVertex);

  uint GetPointsCount() const;
  virtual nuiPoint GetStartPoint() const;
  virtual nuiPoint GetEndPoint() const;
private:
  nuiPath mVertices;
};

#endif //__nuiPolyLine_h__

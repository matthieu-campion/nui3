/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiOutliner_h__
#define __nuiOutliner_h__

#include "nuiPathGenerator.h"
#include "nuiFlags.h"

class nuiRenderObject;

class NUI_API nuiOutliner : public nuiPathGenerator
{
public:
  nuiOutliner(nuiPathGenerator* pPath, float linewidth = 1.0f);
  virtual ~nuiOutliner();

  virtual bool Tessellate(nuiPath& rPoints, float Quality);
  virtual bool TessellateObj(nuiRenderObject& rObject, float Quality);

  void SetLineWidth(float width);
  float GetLineWidth() const;
  void SetLineJoin(nuiLineJoin join);
  nuiLineJoin GetLineJoin()const;
  void SetLineCap(nuiLineCap cap);
  nuiLineCap GetLineCap() const;
  void SetMiterLimit(float limit);
  float GetMiterLimit() const;

  void SetPath(nuiPathGenerator* pPath);
private:
  void Tessellate(nuiPath& rPoints, const nuiPath& rVertices, uint offset, int count, float Quality);
  void AddJoin(const nuiPoint& Point0, const nuiPoint& Point1, const nuiPoint& Point2, const nuiPoint& Point3, nuiPath& rPoints);
  void AddCap(const nuiPoint& rFirstPoint, const nuiPoint& rLastPoint, nuiPath& rPoints);

  void TessellateObj(nuiRenderObject& rObject, const nuiPath& rVertices, uint offset, int count, float Quality);

  float mLineWidth;
  nuiLineJoin mLineJoin;
  nuiLineCap mLineCap;
  nuiSize mMiterLimit;

  nuiPathGenerator* mpPath;

  float mRealLineWidth;
};

#endif // __nuiOutliner_h__


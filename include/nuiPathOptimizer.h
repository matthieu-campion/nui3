/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

// nuiPathOptimizer.h

#ifndef __nuiPathOptimizer_h__
#define __nuiPathOptimizer_h__

#include "nuiPathGenerator.h"

class NUI_API nuiPathOptimizer : public nuiPathGenerator
{
public:
  nuiPathOptimizer(nuiPathGenerator* pPath, float AngleThreshold = .007f, float DistThreshold = 4.0f);
  virtual ~nuiPathOptimizer();

  bool Tessellate(nuiPath& rVertices, float Quality = 0.5f) const;


private:
  nuiPathGenerator* mpPath;
  float mAngleThreshold;
  float mDistThreshold;
};

#endif // __nuiPathOptimizer_h__


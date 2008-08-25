/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

// nuiGradient.h

#ifndef __nuiGradient_h__
#define __nuiGradient_h__

#include "nuiRect.h"
#include <list>
#include <utility>

typedef std::pair<nuiSize, nuiColor> nuiGradientPair;
typedef std::list<nuiGradientPair> nuiGradientStopList;

class nuiDrawContext;
class nuiShape;


class NUI_API nuiGradient
{
public:
  nuiGradient();
  nuiGradient(const nuiGradient& rGradient);
  virtual ~nuiGradient();

  void AddStop(const nuiColor& rColor, nuiSize position); ///< add a stop element to the gradient at the position (position E [0,1]).
  const nuiGradientStopList& GetStopList() const;
  const nuiColor GetColorAt(nuiSize position) const;
protected:
  nuiGradientStopList mStops;

};

class NUI_API nuiReflection
{
public:
  nuiReflection(float Intensity, float Center, float Curve = 0, bool RoundLeft = false, bool RoundRight = false);
  virtual ~nuiReflection();

  void Draw(nuiDrawContext* pContext, const nuiRect& rRect, nuiShape* pShp = NULL);

private:
  nuiRect mRect;
  nuiColor mColor;
  float mCenter;
  float mCurve;
  bool mRoundLeft;
  bool mRoundRight;
  nuiShape* mpSkyShape;
  nuiShape* mpGroundShape;
  nuiGradient* mpSkyGradient;
  nuiGradient* mpGroundGradient;
  bool mRecalc;
  
  void Recalc();
};

#endif //#ifndef __nuiGradient_h__

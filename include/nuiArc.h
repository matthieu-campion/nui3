/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiArc_h__
#define __nuiArc_h__

//#include "nui.h"
#include "nuiPoint.h"
#include "nuiPathGenerator.h"

// nuiArc:
class NUI_API nuiArc : public nuiPathGenerator
{
public:
  nuiArc(const nuiPoint& rStartVertex, const nuiPoint& rStopVertex, float XRadius, float YRadius, float Angle, bool LargeArc, bool Sweep); ///< Create an arc from a start point/end point description. (See SVG documentations).
  nuiArc(float cX, float cY, float rX, float rY, float Theta1, float Theta2, float Phi); ///< Create an Arc from the center of an ellipse, the radii, a start and stop angles an a rotation angle.
  nuiArc(nuiXMLNode* pNode);
  virtual ~nuiArc();

  /** @name Serialization */
  //@{
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode) const; ///< This method saves the state in the given XML Node.
  //@}

  virtual bool Tessellate(nuiPath& rVertices, float Quality = 0.5f);
  virtual nuiPoint GetStartPoint() const;
  virtual nuiPoint GetEndPoint() const;

private:
  nuiPath mVertices;
  nuiPoint mStartVertex;
  nuiPoint mStopVertex;
  float mXRadius;
  float mYRadius;
  float mAngle;
  bool mLargeArc;
  bool mSweep;
};

#endif // nuiArc

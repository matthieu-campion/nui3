/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRectPath_h__
#define __nuiRectPath_h__

#include "nuiPathGenerator.h"
#include "nuiRect.h"

// nuiRectPath:
class NUI_API nuiRectPath : public nuiPathGenerator
{
public:
  nuiRectPath(const nuiRect& rRect, bool CCW = false);
  nuiRectPath(nuiXMLNode* pNode);
  virtual ~nuiRectPath();

  /** @name Serialization */
  //@{
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode) const; ///< This method saves the state in the given XML Node.
  //@}

  virtual bool Tessellate(nuiPath& rVertices, float Quality = 0.5f) const;
  virtual nuiPoint GetStartPoint() const;
  virtual nuiPoint GetEndPoint() const;
private:
  nuiRect mRect;
  bool mCCW;
};

#endif // __nuiRectPath_h__

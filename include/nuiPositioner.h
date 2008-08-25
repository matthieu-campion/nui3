/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPositioner_h__
#define __nuiPositioner_h__

//#include "nui.h"
#include "nuiContainer.h"

/// nuiPositioner is a widget container that keep all its children at their original position and size.
class NUI_API nuiPositioner : public nuiSimpleContainer
{
public:
  nuiPositioner (nuiPosition Position = nuiCenter, bool expandWidth=false, bool expandHeight=false);
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiPositioner();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool Recursive) const;

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);

  virtual void Invalidate();

  void SetLimitBounds(bool limitbounds);
  bool GetLimitBounds() const;

protected:
  nuiPosition mPPosition;
  bool mLimitBounds;
  bool mExpandWidth;
  bool mExpandHeight;
};

#endif // __nuiPositioner_h__


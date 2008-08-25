/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSVGShape_h__
#define __nuiSVGShape_h__

//#include "nui.h"

class nuiXML;
class nuiDrawContext;

class nuiSVGElement;

class NUI_API nuiSVGShape
{
public:
  nuiSVGShape();
  virtual ~nuiSVGShape();

  bool Load(nuiXMLNode* pNode); /// Load an nuiSVGShape from an XML description

  void Draw(nuiDrawContext* pContext, bool Antialias);

  nuiRect GetBoundingRect();

  const nglString& GetTitle();
private:

  bool InterpretStyle(const nglString& rStyleDesc);

  nuiShapeMode mTmpMode;

  nglString mTitle;

  std::list<nuiSVGElement*> mpChildren;

  GLuint mList; /// Try to optimise for OpenGL lists.
  bool mValidBRect;
  nuiRect mBRect;
};

#endif // __nuiSVGShape_h__

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"

class nuiMetaDecoration : public nuiDecoration
{
public:
  nuiMetaDecoration(const nglString& rName);
  virtual ~nuiMetaDecoration();
  
  void InitAttributes();
  
  virtual void DrawBack(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  virtual void DrawFront(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  
  void AddDecoration(const nglString& rDecoName);
  const nglString& GetDecoration();
  void AddDecoration(nuiDecoration* pDeco);
  void Clear();
  
protected:
  std::vector<nuiDecoration*> mDecorations;
};


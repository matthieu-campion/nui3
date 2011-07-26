/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"



class nuiTreeHandleDecoration : public nuiDecoration
{
public:
  nuiTreeHandleDecoration(const nglString& rName, nuiColor handleColor = nuiColor(80,80,80), uint32 handleSize = 8, const nuiBorder& rHandleBorder = nuiBorder(0,0,0,0));

  virtual ~nuiTreeHandleDecoration();
  
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);  
  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;

  void SetHandleBorder(const nuiBorder& rHandleBorder);
  const nuiBorder& GetHandleBorder() const;
  
private:
  
  // attributes
  void InitAttributes();
  uint32 GetHandleSize();
  void SetHandleSize(uint32 size);
  const nuiColor& GetHandleColor();
  void SetHandleColor(const nuiColor& handleColor);

  nuiBorder mBorder;

  nuiColor mHandleColor;
  nuiSize mHandleSize;
};

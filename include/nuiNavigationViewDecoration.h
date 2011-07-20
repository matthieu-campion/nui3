/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"

#define DEVICESTYLE_DEFAULT _T("Default")
#define DEVICESTYLE_IPHONE _T("iPhone")
#define DEVICESTYLE_IPAD _T("iPad")

class nuiNavigationViewDecoration : public nuiDecoration
{
public:

  nuiNavigationViewDecoration(const nglString& rName);
  virtual ~nuiNavigationViewDecoration();
  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;

  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;

//  const nuiColor& GetBackgroundColor() const;
//  void SetBackgroundColor(const nuiColor& color);
//  
//  const nuiColor& GetTileColor() const;
//  void SetTileColor(const nuiColor& color);
//  
//  void SetNbTiles(uint32 nb);
//  uint32 GetNbTiles() const;
//  
//  void SetTileWidth(float size);
//  float GetTileWidth() const;

  // "SystemDefault", "iPhone", "iPad"
  void SetDeviceStyle(const nglString& style);
  const nglString& GetDeviceStyle() const;

private:

  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);

  void InitAttributes();

  float mEvenWidth;
  float mOddWidth;
  nuiColor mColumnColor;
  nglString mDeviceStyle;
  nuiColor mBackgroundColor;
  nuiRect mClientRect;

};


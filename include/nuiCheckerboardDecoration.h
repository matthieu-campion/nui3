/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"


class nuiCheckerboardDecoration : public nuiDecoration
{
public:

  nuiCheckerboardDecoration(const nglString& rName);
  virtual ~nuiCheckerboardDecoration();
  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;

  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;

  const nuiColor& GetBackgroundColor() const;
  void SetBackgroundColor(const nuiColor& color);
  
  const nuiColor& GetTileColor() const;
  void SetTileColor(const nuiColor& color);
  
  void SetTilesPerLine(uint32 nb);
  uint32 GetTilesPerLine() const;

  const nuiColor& GetStrokeColor() const;
	void SetStrokeColor(const nuiColor& color);

  void SetStrokeSize(uint32 size);
  uint32 GetStrokeSize() const;

private:

  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);

  void InitAttributes();

  uint32 mStrokeSize;
  uint32 mTilesPerLine;
  nuiColor mStrokeColor;
  nuiColor mTileColor;
  nuiColor mBackgroundColor;
  nuiRect mClientRect;

};


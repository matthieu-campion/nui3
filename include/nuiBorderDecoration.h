/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"


enum nuiBorderType
{
  eBorderAll=0,
  eBorderLeft,
  eBorderRight,
  eBorderTop,
  eBorderBottom,
  eBorderHorizontal,
  eBorderVertical,
  eBorderNone
};


enum nuiBorderMode
{
  eBorderNormal=0,
  eBorder3DIn,
  eBorder3DOut
};

class nuiBorderDecoration : public nuiDecoration
{
public:

  nuiBorderDecoration(const nglString& rName);
  //#FIXME, create some constructors
  virtual ~nuiBorderDecoration();
  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;

  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;


  const nuiColor& GetStrokeColor() const;
	void SetStrokeColor(const nuiColor& color);

  const nuiColor& GetStrokeLeftColor() const;
	void SetStrokeLeftColor(const nuiColor& color);

  const nuiColor& GetStrokeRightColor() const;
	void SetStrokeRightColor(const nuiColor& color);

  const nuiColor& GetStrokeTopColor() const;
	void SetStrokeTopColor(const nuiColor& color);

  const nuiColor& GetStrokeBottomColor() const;
	void SetStrokeBottomColor(const nuiColor& color);
  
  
  void SetStrokeSize(uint32 size);
  uint32 GetStrokeSize() const;
  void SetBorderMode(nglString mode);
  nglString GetBorderMode() const;
  void SetBorderType(nglString type);
  nglString GetBorderType();

  	
private:

  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);


  void InitAttributes();

  uint32 mStrokeSize;
  bool mUseStrokeGlobalColor;
  nuiColor mStrokeColor;
  bool mUseStrokeLeftColor;
  nuiColor mStrokeLeftColor;
  bool mUseStrokeRightColor;
  nuiColor mStrokeRightColor;
  bool mUseStrokeTopColor;
  nuiColor mStrokeTopColor;
  bool mUseStrokeBottomColor;
  nuiColor mStrokeBottomColor;
  nuiBorderMode mBorderMode;
  nuiBorderType mBorderType;
  nuiBlendFunc mBlendFunc;
  nuiRect mClientRect;

};


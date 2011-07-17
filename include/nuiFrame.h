/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"



class nuiFrame : public nuiDecoration
{
public:
  nuiFrame(const nglString& rName);
  nuiFrame(const nglString& rName, nuiTexture* pTexture, const nuiRect& rClientRect, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  nuiFrame(const nglString& rName, const nglPath& rTexturePath, const nuiRect& rClientRect, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
	void InitAttributes();
  virtual ~nuiFrame();
  
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  

  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;

  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;
  
  nuiTexture* GetTexture() const;
  
  bool IsInterpolated();
  void SetInterpolated(bool set);

  const nglPath& GetTexturePath() const;
  void SetTexturePath(const nglPath& rPath);

	const nuiColor& GetColor() const;
	void SetColor(const nuiColor& color);

  void UseWidgetFrameColor(bool);
  bool IsWidgetFrameColorUsed() const;

  void SetDebug(bool set);

  
	
private:
  void UpdateTextureCoordinates();
  nuiTexture* mpTexture;
  nglPath mTexturePath;
	// attributes ***********************************
  nuiColor mColor;
  bool mUseWidgetFrameColor;
  nuiRect mClientRect;
  bool mDebug;
  bool mInterpolated;
  float mTx[24];
  float mTy[24];
  bool mNeedUpdate;
};


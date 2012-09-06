/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiDecoration.h"



class nuiImageDecoration : public nuiDecoration
{
public:
  nuiImageDecoration(const nglString& rName);
  nuiImageDecoration(const nglString& rName, nuiTexture* pTexture, const nuiRect& rClientRect = nuiRect(0,0,0,0), nuiPosition position = nuiCenter, const nuiColor& rColor = nuiColor(255,255,255,255));
  nuiImageDecoration(const nglString& rName, const nglPath& rTexturePath, const nuiRect& rClientRect = nuiRect(0,0,0,0), nuiPosition position = nuiCenter, const nuiColor& rColor = nuiColor(255,255,255,255));
	void InitAttributes();
  virtual ~nuiImageDecoration();
  
  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;
  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;
  
  nuiTexture* GetTexture() const;
  
  nglPath GetTexturePath() const;
  void SetTexturePath(nglPath path);
  
  nuiPosition GetPosition();
  void SetPosition(nuiPosition pos);
  
  const nuiColor& GetColor() const;
  void SetColor(const nuiColor& rColor);
  
  bool GetRepeatX() const;
  void SetRepeatX(bool set);
  bool GetRepeatY() const;
  void SetRepeatY(bool set);
  
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect);
	
private:
  nuiTexture* mpTexture;
  bool mRepeatX;
  bool mRepeatY;
  
	// attributes ***********************************
  nuiRect mClientRect;
  nuiPosition mPosition;
  nuiColor mColor;
};


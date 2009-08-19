/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiAttributeAnimation;

class nuiHugeImage : public nuiWidget
{
public:
  nuiHugeImage(const nglPath& rImagePath);
  virtual ~nuiHugeImage();
  
  virtual nuiRect CalcIdealSize();
  virtual bool Draw(nuiDrawContext* pContext);
  
  virtual bool MouseClicked(const nglMouseInfo& rInfo);
  virtual bool MouseUnclicked(const nglMouseInfo& rInfo);
  virtual bool MouseMoved(const nglMouseInfo& rInfo);

  bool Load(const nglPath& rImagePath);
  
  void ZoomTo(float zoom);
  void SetZoom(float zoom);
  float GetZoom() const;
private:
  void InitAttributes();
  std::vector<std::vector<nuiTexture*> > mTextures;
  void InitImage();
  void ClearImage();
  nuiRect mImageSize;
  float mZoom;
  float mMinZoom;
  float mMaxZoom;

  float mX;
  float mY;
  bool mClicked;
  float mLastX;
  float mLastY;

  void SetCenterX(float set);
  float GetCenterX() const;
  void SetCenterY(float set);
  float GetCenterY() const;
  
  nuiAttributeAnimation* mpZoom;
  nuiAttributeAnimation* mpPanX;
  nuiAttributeAnimation* mpPanY;
};
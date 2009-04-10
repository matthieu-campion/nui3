/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"


class nuiImageSequence : public nuiWidget
{
public:
  nuiImageSequence();
  nuiImageSequence(uint32 nbFrames, nglImage* pImage, nuiOrientation orientation, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  nuiImageSequence(uint32 nbFrames, const nglPath& rTexturePath, nuiOrientation orientation, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  void InitAttributes();
  virtual ~nuiImageSequence();
  
  bool Load(const nuiXMLNode* pNode);
  nuiXMLNode* Serialize(nuiXMLNode* pNode);
  
  virtual nuiRect CalcIdealSize();  
  virtual bool Draw(nuiDrawContext* pContext, nuiWidget* pWidget = NULL);
  
  uint32 GetFrameIndex(nuiWidget* pWidget) const;
  void SetFrameIndex(uint32 index);
  
  uint32 GetNbFrames() const;
  void SetNbFrames(uint32 nbFrames);
  
  void SetOrientation(nglString orientation);
  nglString GetOrientation();
  
  bool IsInterpolated();
  void SetInterpolated(bool set);
  
  const nglPath& GetTexturePath() const;
  void SetTexturePath(const nglPath& rPath);
  
  const nuiColor& GetColor() const;
  void SetColor(const nuiColor& color);
  
  
private:

  bool CreateTextures(); // return true if the textures have been properly created

  nglPath mTexturePath;
  std::vector<nuiTexture*> mTextures;
  
  // attributes ***********************************
  nuiColor mColor;
  nuiRect mTexRect;
  bool mInterpolated;
  uint32 mNbFrames;
  uint32 mFrameIndex;
  nuiOrientation mOrientation;
  bool mUseAlpha;
  
  nglImage* mpTempImage;
  bool mRefreshTextures;
};


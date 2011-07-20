/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"


class nuiImageSequence : public nuiObject
{
public:
  nuiImageSequence();
  nuiImageSequence(uint32 nbFrames, nglImage* pImage, nuiOrientation orientation = nuiVertical);
  nuiImageSequence(uint32 nbFrames, const nglPath& rTexturePath, bool framesInSingleFile = true, nuiOrientation orientation = nuiVertical); // all frames in a single file, or stored in separated png files
  void InitAttributes();
  virtual ~nuiImageSequence();
  
  virtual nuiRect CalcIdealSize();  
  virtual bool Draw(nuiDrawContext* pContext);
  virtual bool Draw(nuiDrawContext* pContext, nuiWidget* pWidget);
  
  uint32 GetFrameIndex() const;
  void SetFrameIndex(uint32 index);
  
  uint32 GetNbFrames() const;
  void SetNbFrames(uint32 nbFrames);
  
  void SetOrientation(nuiOrientation orientation);
  nuiOrientation GetOrientation() const;
  
  bool IsInterpolated() const;
  void SetInterpolated(bool set);
  
  const nglPath& GetTexturePath() const;
  void SetTexturePath(const nglPath& rPath);
  
  const nuiColor& GetColor() const;
  void SetColor(const nuiColor& color);
  
  void SetAlpha(float alpha);
  float GetAlpha() const;
  
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
  
  bool mFramesInSingleFile;
  nglImage* mpTempImage;
  bool mRefreshTextures;
  float mAlpha;
  nuiRect mRect;
};


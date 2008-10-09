/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiDecoration.h"



class nuiFrameSequence : public nuiDecoration
{
public:
  nuiFrameSequence(const nglString& rName);
  nuiFrameSequence(const nglString& rName, uint32 nbFrames, nglImage* pImage, nuiOrientation orientation, const nuiRect& rClientRect, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  nuiFrameSequence(const nglString& rName, uint32 nbFrames, const nglPath& rTexturePath, nuiOrientation orientation, const nuiRect& rClientRect, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  void InitAttributes();
  virtual ~nuiFrameSequence();
  
  bool Load(const nuiXMLNode* pNode);
  nuiXMLNode* Serialize(nuiXMLNode* pNode);
  
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  
  uint32 GetFrameIndex(nuiWidget* pWidget) const;
  void SetFrameIndex(nuiWidget* pWidget, uint32 index);
  
  uint32 GetNbFrames() const;
  void SetNbFrames(uint32 nbFrames);
  
  void SetOrientation(nglString orientation);
  nglString GetOrientation();
  
  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;
  
  void EnableBorder(bool set);
  bool IsBorderEnabled() const;
  
  virtual nuiSize GetBorder(nuiPosition position) const;
  virtual nuiRect GetIdealClientRect() const;
  
  bool IsInterpolated();
  void SetInterpolated(bool set);
  
  const nglPath& GetTexturePath() const;
  void SetTexturePath(const nglPath& rPath);
  
  const nuiColor& GetColor() const;
  void SetColor(const nuiColor& color);
  
  
private:

  bool CreateTextures(); // return true if the textures have been properly created

  nglString mGlobalTexturePath;
  std::vector<nuiTexture*> mTextures;
  
  // attributes ***********************************
  nuiColor mColor;
  nuiRect mClientRect;
  nuiRect mTexRect;
  bool mBorderEnabled;
  bool mInterpolated;
  std::map<nuiWidget*, uint32> mIndex;
  uint32 mNbFrames;
  nuiOrientation mOrientation;
  
  nglImage* mpTempImage;
  bool mRefreshTextures;
};


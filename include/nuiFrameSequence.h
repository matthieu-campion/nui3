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
  nuiFrameSequence(const nglString& rName, uint32 nbFrames, uint32 firstFrameIndex, nuiTexture* pTexture, nuiOrientation orientation, const nuiRect& rClientRect, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  nuiFrameSequence(const nglString& rName, uint32 nbFrames, uint32 firstFrameIndex, const nglPath& rTexturePath, nuiOrientation orientation, const nuiRect& rClientRect, const nuiColor& rColor = nuiColor(255, 255, 255, 255));
  void InitAttributes();
  virtual ~nuiFrameSequence();
  
  bool Load(const nuiXMLNode* pNode);
  nuiXMLNode* Serialize(nuiXMLNode* pNode);
  
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  
  uint32 GetFrameIndex() const;
  void SetFrameIndex(uint32 index);
  
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
  
  nuiTexture* GetTexture() const;
  
  bool IsInterpolated();
  void SetInterpolated(bool set);
  
  const nglPath& GetTexturePath() const;
  void SetTexturePath(const nglPath& rPath);
  
  const nuiColor& GetColor() const;
  void SetColor(const nuiColor& color);
  
  void SetDebug(bool set);
  
  
  
private:

  void UpdateTexSize();

  nuiTexture* mpTexture;
  
  // attributes ***********************************
  nuiColor mColor;
  nuiRect mClientRect;
  nuiRect mTexRect;
  bool mBorderEnabled;
  bool mInterpolated;
  uint32 mIndex;
  uint32 mNbFrames;
  nuiOrientation mOrientation;
};


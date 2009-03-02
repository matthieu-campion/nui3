/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiWidget.h"
#include "nuiTexture.h"

class nuiCoverFlow : public nuiWidget
{
public:
  nuiCoverFlow();
  virtual ~nuiCoverFlow();
  
  bool Draw(nuiDrawContext* pContext);
  bool SetRect(const nuiRect& rRect);
  
  uint32 AddImage(nuiTexture* pTexture, nuiTexture* pBefore = NULL);
  uint32 AddImage(nuiTexture* pTexture, uint32 index);
  void DelImage(nuiTexture* pTexture);
  void DelImage(uint32 index);

  void SelectImage(nuiTexture* pTexture);
  void SelectImage(int32 index);
  
  const std::vector<nuiTexture*>& GetImages() const;
  
  void SetBackground(const nuiColor& rColor);
  const nuiColor& GetBackground() const;
  
  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
protected:
  void DrawCard(nuiDrawContext* pContext, uint32 index, float start, float end);
  bool OnUpdateTime(const nuiEvent& rEvent);
  
  std::vector<nuiTexture*> mImages;
  int32 mSelectedImage;
  nuiColor mBackground;
  float mReflectionStart;
  float mReflectionEnd;
  bool mDrawBackground;
  float mYOffset;
  float mPos;
  nuiTimer mTimer;
  double mLastTime;
  nuiEventSink<nuiCoverFlow> mFlowSink;
};
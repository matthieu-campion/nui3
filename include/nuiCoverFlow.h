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
  nuiRect CalcIdealSize();
  
  int32 AddImage(nuiTexture* pTexture, nuiTexture* pBefore = NULL);
  int32 AddImage(nuiTexture* pTexture, int32 index);
  void DelImage(nuiTexture* pTexture);
  void DelImage(int32 index);

  void SelectImage(nuiTexture* pTexture);
  void SelectImage(int32 index);

  void SelectImageNow(nuiTexture* pTexture);
  void SelectImageNow(int32 index);

  int32 GetSelectedIndex() const;
  nuiTexture* GetSelectedTexture() const;
  
  void SetReflectionStart(float set);
  float GetReflectionStart() const;
  
  void SetReflectionEnd(float set);
  float GetReflectionEnd() const;
  
  void SetDrawBackground(bool set);
  bool GetDrawBackground() const;

  void SetYOffset(float set);
  float GetYOffset() const;
  
  void SetSelectionYOffset(float set);
  float GetSelectionYOffset() const;
  
  void SetAngle(float set);
  float GetAngle() const;
  
  void SetSideShift(float set);
  float GetSideShift() const;

  void SetSideGap(float set);
  float GetSideGap() const;

  void SetSideDepth(float set);
  float GetSideDepth() const;
  
  const std::vector<nuiTexture*>& GetImages() const;
  
  void SetBackgroundColor(const nuiColor& rColor);
  const nuiColor& GetBackgroundColor() const;
  
  bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
  
  nuiSimpleEventSource<0> SelectionChanged;
  
protected:
  void DrawCard(nuiDrawContext* pContext, int32 index, float start, float end);
  void OnUpdateTime(const nuiEvent& rEvent);
  
  std::vector<nuiTexture*> mImages;
  int32 mSelectedImage;
  nuiColor mBackground;
  float mReflectionStart;
  float mReflectionEnd;
  bool mDrawBackground;
  float mYOffset;
  float mSelectionYOffset;
  float mAngle;
  float mSideShift;
  float mSideGap;
  float mSideDepth;
  
  float mPos;
  nuiTimer mTimer;
  double mLastTime;
  nuiEventSink<nuiCoverFlow> mFlowSink;
  void SelectImageIndex(int32 index);
  void SelectImageIndexNow(int32 index);
};
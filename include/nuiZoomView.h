/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiZoomView_h__
#define __nuiZoomView_h__

//#include "nui.h"
#include "nuiWidget.h"
#include "nuiSimpleContainer.h"
#include "nuiScrollBar.h"
#include "nuiSlider.h"
#include "nuiRange.h"
#include "nuiContainer.h"

class NUI_API nuiZoomView : public nuiSimpleContainer
{
public:
  nuiZoomView(nuiScrollBar *pHorizontalScrollbar = NULL, nuiScrollBar *pVerticalScrollbar = NULL, nuiSlider * pHorizontalSlider = NULL, nuiSlider * pVerticalSlider = NULL, bool calcWidthFromIdealSize = true, bool calcHeightFromIdealSize = true);
  virtual ~nuiZoomView();

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  void Scrolled(const nuiEvent& rEvent);
  void Zoomed(const nuiEvent& rEvent);

  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);

  virtual nuiRange* GetScrollbarRange(nuiOrientation Orientation);
  
  void SetZoom(float hZoom, float vZoom) { mHorizontalZoomLevel = hZoom; mVerticalZoomLevel = vZoom; InvalidateLayout(); }
  void GetZoom(float& rHZoom, float& rVZoom) { rHZoom = mHorizontalZoomLevel; rVZoom = mVerticalZoomLevel; }

  void SetAlwaysVisibleScrollbars(bool h=false, bool v=false);

private:

  void OnChildAdded(const nuiEvent& rEvent);
  void OnChildRemoved(const nuiEvent& rEvent);
  void OnHotRectChanged(const nuiEvent& rEvent);

  nuiScrollBar* mpHorizontalScrollbar;
  nuiScrollBar* mpVerticalScrollbar;
  
  nuiSlider* mpHorizontalSlider;
  nuiSlider* mpVerticalSlider;

  float mHorizontalZoomLevel;
  float mVerticalZoomLevel;

  bool mCalcWidthFromIdealSize;
  bool mCalcHeightFromIdealSize;

  bool mAlwaysDisplayVScrollbar;
  bool mAlwaysDisplayHScrollbar;

  nuiEventSink<nuiZoomView> mSVSink;
};

#endif // __nuiZoomView_h__



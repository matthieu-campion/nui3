/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiScrollView_h__
#define __nuiScrollView_h__

//#include "nui.h"
#include "nuiWidget.h"
#include "nuiComposite.h"
#include "nuiScrollBar.h"
#include "nuiContainer.h"

class NUI_API nuiScrollView : public nuiComposite
{
public:
  nuiScrollView(bool Horizontal = true, bool Vertical = true);
  nuiScrollView(nuiScrollBar* pHorizontalScrollbar, nuiScrollBar* pVerticalScrollbar );
  virtual bool Load(const nuiXMLNode* pNode);
  virtual ~nuiScrollView();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);

  bool Scrolled(const nuiEvent& rEvent);

  void SetFillChildren(bool Set); ///< If filling is enabled the scroll view will try to move its child in order to honor the available space
  bool GetFillChildren();

  virtual bool Clear(bool Delete);

  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);

  const nuiRect& GetChildrenUnionRect() { return mChildrenUnionRect; }

  virtual nuiRange* GetRange(nuiOrientation Orientation);
  nuiScrollBar* GetScrollBar(nuiOrientation Orientation);

  void SetHIncrement(nuiSize hIncrement);
  nuiSize GetHIncrement() const;
  void SetVIncrement(nuiSize vIncrement);
  nuiSize GetVIncrement() const;

  void SetBarSize(float Set)    { mBarSize = Set; InvalidateLayout(); }

  void EnableSmoothScrolling(bool set);
  bool IsSmoothScrollingEnabled() const;

  void EnableMinimalResize(bool set);
  bool IsMinimalResizeEnabled() const;

  nuiSimpleEventSource<0> OffsetsChanged;
  nuiSize GetXOffset() const;
  nuiSize GetYOffset() const;

  void SetForceNoSmartScroll(bool set);
  bool GetForceNoSmartScroll(bool set) const;

  bool GetEnableHorizontalScroll() const;
  void SetEnableHorizontalScroll(bool set);
  bool GetEnableVerticalScroll() const;
  void SetEnableVerticalScroll(bool set);
  
private:
  void InitAttributes();
  void Init(nuiScrollBar* pHorizontalScrollBar, nuiScrollBar* pVerticalScrollBar, bool Horizontal, bool Vertical);

  bool OnChildAdded(const nuiEvent& rEvent);
  bool OnChildRemoved(const nuiEvent& rEvent);
  bool OnHotRectChanged(const nuiEvent& rEvent);

  nuiRect mChildrenUnionRect;
  bool SetChildrenRect(nuiSize x, nuiSize y, nuiSize xx, nuiSize yy, nuiSize scrollv, nuiSize scrollh);

  nuiScrollBar* mpHorizontal;
  nuiScrollBar* mpVertical;

  bool mForceNoHorizontal;
  bool mForceNoVertical;
  bool mForceNoSmartScroll;

  bool mHorizontalIsExternal;
  bool mVerticalIsExternal;

  float mBarSize;

  bool mFillChildren;
  nuiSize mVIncrement, mHIncrement;

  bool OnSmoothScrolling(const nuiEvent& rEvent);
  
  bool OnHThumbPressed(const nuiEvent& rEvent);
  bool OnHThumbDepressed(const nuiEvent& rEvent);
  bool OnVThumbPressed(const nuiEvent& rEvent);
  bool OnVThumbDepressed(const nuiEvent& rEvent);
  
  bool mHThumbPressed;
  bool mVThumbPressed;
  bool mSmoothScrolling;
  nuiSize mXOffset; // Used for Smooth Scrolling
  nuiSize mYOffset;
  nuiTimer* mpSmoothTimer;

  bool mMinimalResize;
  nuiRect mOldIdealRect;

  nuiEventSink<nuiScrollView> mSVSink;
};

#endif // __nuiScrollView_h__


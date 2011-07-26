/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiScrollBar_h__
#define __nuiScrollBar_h__

#include "nuiContainer.h"
#include "nuiEvent.h"
#include "nuiTimer.h"
#include "nuiRange.h"

#define SBSIZE 12 // Basic standard size... Should go in the theme!
#define SBMINTHUMB 20.f // Min Thumb size

/// Basic scroll bar with range support. Used in too many compound widgets to name :).
class NUI_API nuiScrollBar : public nuiSimpleContainer
{
public:
  nuiScrollBar(nuiOrientation orientation = nuiHorizontal, const nuiRange& rRange = nuiRange(), nuiWidgetPtr pThumb = NULL);
  virtual ~nuiScrollBar();

  virtual bool SetRect(const nuiRect& rRect);
  
  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();

  virtual void DisableDrawBackground(bool disabled = true); ///< Disable the scrollbar background drawing

  void SetRange(nuiRange* pRange); ///< Sets the current range used
  virtual nuiRange& GetRange(); ///< Return the Range used by this scroll bar. 
  virtual void SetOrientation(nuiOrientation Orientation); ///< Change the orientation of the scroll bar
  virtual nuiOrientation GetOrientation(); ///< Return the orientation of the scroll bar

  virtual void SetThumbWidget(nuiWidget* pThumb);
  const nuiRect& GetThumbRect() { return mThumbRect; } ///< Return the Thumb rect defined in SetRect
  const nuiRect& GetRangeRect() { return mRangeRect; }

  //Received Events:
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);


  // Sent events:
  nuiSimpleEventSource<nuiValueChanged> ValueChanged; ///< This event is sent whenever the scrollbar's thumb position is changed (by the user or by the program).
  void HandlePageDown(const nuiEvent& rEvent);
  void HandlePageUp(const nuiEvent& rEvent);

  void Increment(const nuiEvent& rEvent);
  void Decrement(const nuiEvent& rEvent);

  void SetIdealWidth(nuiSize width);
  nuiSize GetIdealWidth() const;
  
  void SetThumbSideSize(float SideSize);
  float GetThumbSideSize();
  void SetThumbMiddleSize(float MiddleSize);
  float GetThumbMiddleSize();
  nuiSize GetThumbMinSize() const;
  void SetThumbMinSize(nuiSize MinSize);
  
  void SetForegroundDeco(nuiDecoration* pDeco);
  void SetBackgroundDeco(nuiDecoration* pDeco);
  nuiDecoration* GetForegroundDeco();
  nuiDecoration* GetBackgroundDeco();
  
  void SetForegroundDecoName(const nglString& rName);
  void SetBackgroundDecoName(const nglString& rName);
  const nglString& GetForegroundDecoName();
  const nglString& GetBackgroundDecoName();
  
  
  nuiSimpleEventSource<0> ThumbPressed;
  nuiSimpleEventSource<0> ThumbDepressed;
  
protected:
  void DoInvalidate(const nuiEvent& rEvent);
  void DoInvalidateLayout(const nuiEvent& rEvent);

  void SetThumbRect(const nuiRect& rRect);

  nuiOrientation mOrientation;

  nuiSize mClickValue;

  bool mClicked;
  bool mThumbClicked;
  bool mPageUpClicked;
  bool mPageDownClicked;

  bool mDrawBackground;

  nuiSize mClickX;
  nuiSize mClickY;

  float mThumbSideSize;
  float mThumbMiddleSize;
  nuiSize mThumbMinSize;
  
  bool mRightSideClicked;
  bool mLeftSideClicked;
  
  nuiSize mIdealWidth;
  
  nuiRange mRange;
  nuiRange* mpRange;

  nuiWidget*  mpThumb;
  nuiDecoration* mpForegroundDeco;
  nuiDecoration* mpBackgroundDeco;
  
  nuiSize mThumbLength;
  nuiSize mThumbPosition;
  nuiRect mThumbRect;
  nuiRect mRangeRect;
  nuiTimer mTimer;
  nuiEventSink<nuiScrollBar> mScrollBarSink;
};

/// Custom scroll bar with basic layout options
class NUI_API nuiCustomScrollBar : public nuiScrollBar
{
public:
  nuiCustomScrollBar(nuiOrientation orientation, const nuiRange& rRange = nuiRange(), nuiWidgetPtr pThumb = NULL);
  virtual ~nuiCustomScrollBar();

  void AddWidget(nuiWidget* pWidget, nuiPosition position); ///< add child with given nuiPosition position
   
  virtual bool SetRect(const nuiRect& rRect);
  virtual nuiRect CalcIdealSize();
private:
  typedef std::list< std::pair<nuiWidget*, nuiPosition> > PositionedWidgetList;
  PositionedWidgetList mScrollBarPositionedChildren;
  nuiWidgetList mScrollBarCenteredChildren;
};

#endif // __nuiScrollBar_h__

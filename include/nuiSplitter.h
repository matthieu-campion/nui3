/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSplitter_h__
#define __nuiSplitter_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiEvent.h"

/// defines the way the splitter bar position is computed
enum nuiSplitterMode
{
	eModePixel, 
	eModePercentage 
};


class nuiSplitterHandle : public nuiSimpleContainer
{
public:
  nuiSplitterHandle(nuiSplitter* pParent);
  virtual ~nuiSplitterHandle();
  
  nuiSplitter* GetSplitter();
  
  bool mClicked;
  
private:
  
  // Received Mouse events:
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);
  
  virtual nuiWidgetPtr DispatchMouseMove(const nglMouseInfo& rInfo);

  
  nuiSplitter* mpParent;
  double mClickPos;
};


/// This is a split view widget. It divides its client area in two views (horizontaly or verticaly) which acts as two subcontainers. The user can change the proportional size of the area, but it is also possible to lock the split to prevent modification.
class NUI_API nuiSplitter : public nuiSimpleContainer
{
  friend class nuiSplitterHandle;

public:
  nuiSplitter(nuiOrientation orientation = nuiHorizontal, nuiSplitterMode mode=eModePercentage);

  virtual ~nuiSplitter();
  
  virtual void SetDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode);
 

  void InitProperties();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  void SetOrientation(nuiOrientation orientation);
  nuiOrientation GetOrientation();
  
  void SetMode (nuiSplitterMode mode);
  nuiSplitterMode GetMode ();

  nuiSplitterHandle* GetHandle() const;

  nuiSize GetHandlePos(); ///< Retrieve the Position of the handle separating the 2 widgets (in % if mode == eModePercentage).
  void SetHandlePos(nuiSize HandlePos); ///< Set the the position of the handle separation in between the 2 widgets (in % if mode == eModePercentage).

  nuiSize GetHandlePosMin(); ///< Retrieve the minimum Position of the handle separating the 2 widgets (in % if mode == eModePercentage).
  void SetHandlePosMin(nuiSize HandlePos); ///< Set the the minimum position of the handle separation the 2 widgets (in % if mode == eModePercentage).
  nuiSize GetHandlePosMax(); ///< Retrieve the maximum Position of the handle separating the 2 widgets (in % if mode == eModePercentage).
  void SetHandlePosMax(nuiSize HandlePos); ///< Set the the maximum position of the handle separation the 2 widgets (in % if mode == eModePercentage).
  
  nuiSize GetFirstWidgetMinPixels(); ///< Retrieve the minimum Size of the First Widget (in pixels).
  void SetFirstWidgetMinPixels(nuiSize MinPixels); ///< Set the minimum Size of the First Widget (in pixels).
  nuiSize GetSeconWidgetMinPixels(); ///< Retrieve the minimum Size of the Second Widget (in pixels).
  void SetSeconWidgetMinPixels(nuiSize MinPixels); ///< Set the minimum Size of the Second Widget (in pixels).

  bool GetHandleHover() const;
  void GotoHandlePos(nuiSize HandlePos, nuiSize handleStep = 1, double timerStep = 0.02); ///< go to handle pos

  nuiSize GetHandleSize(); ///< Retrieve the size of the handle separation the 2 widgets.

  void SetFixed(bool flag); ///< Enable or disable mouse modifications of the splitter widget.
  bool GetFixed(); ///< Returns true if mouse modifications of the splitter widget is enabled.
  void SetMasterChild(bool MasterChild); ///< We will try to display the first widget completely if true.
  bool GetMasterChild(); ///< Return true if MasterChild is enabled.


  nuiSimpleEventSource<0> HandleMoved;
  nuiSimpleEventSource<0> GoToHandlePosAnimDone;

protected:
//  void StepHandlePos(const nuiEvent& rEvent);  
  nuiOrientation mOrientation; /// mHorizontal is true if the containers' layout if horizontal, false if vertical.
  nuiSplitterMode mMode;
  std::list<nuiRect> mPositions;
  bool mFixed;
  bool mHandleHover;

  float mHandlePos;
  float mHandlePosMin;
  float mHandlePosMax;
  nuiSize mFirstWidgetMinPixels;
  nuiSize mSeconWidgetMinPixels;

  float mStartHandlePos; ///< original position
  float mEndHandlePos; ///< end position
  float mGotoStep; ///< step
//  nuiEventSink<nuiSplitter> mSplitterSink; //sink for timer

  bool mMasterChild;

  bool SetRectHorizontal(const nuiRect& rRect);
  bool SetRectVertical(const nuiRect& rRect);

private:
  
  void InitAttributes();


  nuiSplitterHandle* mpHandle;

};

#endif // __nuiSplitter_h__

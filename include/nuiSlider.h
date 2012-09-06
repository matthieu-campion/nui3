/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/




#ifndef __nuiSlider_h__
#define __nuiSlider_h__

#include "nuiWidget.h"
#include "nuiEvent.h"
#include "nuiTimer.h"
#include "nuiRange.h"
#include "nuiContainer.h"

/// Basic slider with range support.
class NUI_API nuiSlider : public nuiSimpleContainer
{
public:
  nuiSlider(nuiOrientation orientation = nuiHorizontal, const nuiRange& rRange = nuiRange());
  virtual ~nuiSlider();

  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  virtual nuiRange& GetRange(); ///< Return the Range used by this scroll bar. 
  virtual void SetOrientation(nuiOrientation Orientation); ///< Change the orientation of the scroll bar
  virtual nuiOrientation GetOrientation(); ///< Return the orientation of the scroll bar

  /// Handle Visual Offsets:
  virtual void SetHandleOffset(nuiSize Offset); ///< Set Offset in the handle widget for the hotspot (in pixels).
  virtual void SetHandlePosMin(nuiSize Min); ///< Set the minimum position of the handle in pixels.
  virtual void SetHandlePosMax(nuiSize Max); ///< Set the maximum position of the handle in pixels.

  virtual nuiSize GetHandleOffset();
  virtual nuiSize GetHandlePosMin();
  virtual nuiSize GetHandlePosMax();

  virtual bool SetHandle(nuiWidget* pWidget); ///< Replace the default handle drawing routine by the given widget which becomes a child of the nuiSlider.
  virtual bool SetBackground(nuiWidget* pWidget); ///< Replace the default background drawing routine by the given widget which becomes a child of the nuiSlider.
  virtual nuiWidget* GetHandle(); ///< Get the widget used to draw the handle. If this method returns NULL the default handle is used.
  virtual nuiWidget* GetBackground(); ///< Get the widget used to draw the background. If this method returns NULL the default background is used.
  virtual void SetAutoAdjustHandle(bool Adjust); ///< when enabled, the handle offset, min and max position are automatically adjusted when an operation on them is made. The default is true.
  bool GetAutoAdjustHandle();

  virtual void DrawBackground(bool DrawBackground);
  
  void HookMouse();
  

  // Keyboard events:
  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
  
  // Mouse Events:
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);

  // Sent events:
  nuiSimpleEventSource<nuiValueChanged> InteractiveValueChanged; ///< This event is sent whenever the Slider's thumb position is changed (by the user or by the program).
  nuiSimpleEventSource<nuiValueChanged> ValueChanged; ///< This event is sent whenever the Slider's thumb position is changed and the mouse is unclicked.
  void HandlePageDown(const nuiEvent& rEvent);
  void HandlePageUp(const nuiEvent& rEvent);

  // Settings
  // the bigger the ratio is, the slower the slider will move.
  static void SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio);
  void SetFineSensitivityRatio(nuiSize FineSensitivityRatio);

  static void SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey);
  void SetFineSensitivityKey(nglKeyCode FineSensitivityKey);


protected:
  void InitAttributes();
  void DoInvalidate(const nuiEvent& rEvent);
  void HandleClicked(const nuiEvent& rEvent);
  void HandleUnclicked(const nuiEvent& rEvent);

  nuiOrientation mOrientation;

  nuiSize mClickValue;

  bool mClicked;
  bool mThumbClicked;

  nuiSize mClickX;
  nuiSize mClickY;
  
  nuiSize mIdealWidth;
  nuiSize mHandleIdealWidth;
  
  bool mInteractiveValueChanged;

  nuiRange mRange;

  static nuiSize mDefaultFineSensitivityRatio;
  nuiSize mFineSensitivityRatio;

  static nglKeyCode mDefaultFineSensitivityKey;
  nglKeyCode mFineSensitivityKey;

  nuiEventSink<nuiSlider> mSliderSink;

  nuiWidget* mpBackground;
  nuiWidget* mpHandle;

  nuiSize mHandleOffset;
  nuiSize mHandlePosMax;
  nuiSize mHandlePosMin;

  bool mDrawBackground;

  void AdjustHandle();
  bool mAutoAdjustHandle;
};

#endif // __nuiSlider_h__

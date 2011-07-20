/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiButton_h__
#define __nuiButton_h__

//#include "nui.h"
#include "nuiSimpleContainer.h"
#include "nuiEvent.h"
#include "nuiImage.h"
#include "nuiDecoration.h"

class nuiButton;


/// This class implements a simple push button that acts as a container for a label, an image, or any other inactive widget element.
class NUI_API nuiButton : public nuiSimpleContainer
{ 
public:
  nuiButton(); ///< Create a simple button (empty).
  nuiButton(const nglString& rText); ///< Create a simple button with a nuiLabel as a child filling the client area.
  nuiButton(nuiDecoration* pDeco, bool AlreadyAcquired=false); ///< Create a simple button with a Decoration.
  nuiButton(const nglImage& rImage); ///< Create a simple button with a nuiImage as a child filling the client area.
  virtual ~nuiButton();

  void InitAttributes();

  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  // Rendering:
  virtual bool Draw(nuiDrawContext* pContext);

  // Public events:
  nuiSimpleEventSource<nuiButtonPressed> ButtonPressed;
  nuiSimpleEventSource<nuiButtonDePressedInactive> ButtonDePressedInactive;
  nuiSimpleEventSource<nuiButtonDePressed> ButtonDePressed;
  nuiSimpleEventSource<nuiWidgetActivated> Activated;

  // Received Mouse events:
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);
  virtual bool MouseUngrabbed();

  void SetDefaultBorders(float pixels);

  bool IsPressed() const;
  virtual void SetPressed(bool Pressed = true);
  void Activate(); ///< Simulate the action of the user pressing the button (useful for hotkey graphical feedback).


  void EnableAutoRepeat(bool set);
  bool GetAutoRepeat() const;
  void SetAutoRepeatDelay(float Delay);
  float GetAutoRepeatDelay() const;
  void SetAutoRepeatMinDelay(float Delay);
  float GetAutoRepeatMinDelay() const;
  
  void SetActivationOffset(nuiSize Offset);
  nuiSize GetActivationOffset() const;

  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
protected:
  bool mClicked;
  bool mPressed;
  bool mAutoRepeat;
  nglTime mLastTime;
  float mRepeatDelay;
  float mRepeatMinDelay;
  float mCurrentRepeatDelay;
  float mUntilRepeat;
  nuiTimer* mpAutoRepeatTimer;
  nuiSize mActivationOffset;
  nuiEventSink<nuiButton> mEventSink;
  void OnAutoRepeat(const nuiEvent& rEvent);
  nuiTask* mpTask;
  
  static float mDefaultBorders;
};




#endif // __nuiButton_h__

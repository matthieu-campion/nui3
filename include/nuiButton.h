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
  virtual bool Load(const nuiXMLNode* pNode); ///< Create a button from an XML description.
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


  bool IsPressed() const;
  virtual void SetPressed(bool Pressed = true);

  void EnableAutoRepeat(bool set);
  bool GetAutoRepeat() const;
protected:
  bool mClicked;
  bool mPressed;
  bool mAutoRepeat;
  nuiTimer* mpAutoRepeatTimer;
  nuiEventSink<nuiButton> mEventSink;
  bool OnAutoRepeat(const nuiEvent& rEvent);
};




#endif // __nuiButton_h__

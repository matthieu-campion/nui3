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
  nuiButton(nuiDecoration* pDeco); ///< Create a simple button with a nuiLabel as a child filling the client area.
  nuiButton(const nglImage& rImage); ///< Create a simple button with a nuiImage as a child filling the client area.
  virtual bool Load(const nuiXMLNode* pNode); ///< Create a button from an XML description.
  virtual ~nuiButton();

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

  // Other attributes:
  nuiSize GetBorderSize() const; ///< Retrieve the current border around the area of the enclosed widget.
  void SetBorderSize(nuiSize BorderSize); ///< Set the current border around the area of the enclosed widget.
  void SetDrawSelf(bool val); ///< Permits to enable and disable the drawing of the button background and frame.
  bool GetDrawSelf() const; ///< See SetDrawSelf
  nuiSize GetShadeSize() const; ///< Retrieve the current size of the shading
  void SetShadeSize(nuiSize ShadeSize); /// Set the current shading size below the button

  // Class attributes:
  static nuiSize GetDefaultBorderSize(); ///< Retrieve the default border around the area of the enclosed widget of every button that will be created.
  static void SetDefaultBorderSize(nuiSize BorderSize); ///< Set the default border around the area of the enclosed widget of every button that will be created.

  bool IsPressed() const;
  virtual void SetPressed(bool Pressed = true);

  
protected:
  bool mClicked;
  nuiSize mBorderSize;
  nuiSize mShadeSize;
  bool mDrawSelf;
  bool mPressed;

  static nuiSize mDefaultBorderSize;
};




#endif // __nuiButton_h__

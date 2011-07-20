/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWindow_h__
#define __nuiWindow_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiImage.h"
#include "nuiEvent.h"
#include "nuiMouseEvent.h"

class nuiButton;
class nuiMainWindow;
class nuiWindowManager;

/// A nui window is an overlapped popup window. It can only be owned by a MainWindow.
class NUI_API nuiWindow : public nuiSimpleContainer
{
public:
  // nuiWindow flags:
  static const nuiWindowFlags NoFlag;
  static const nuiWindowFlags StaticChildren; ///< The window's children are static and do not occupy all the window space but the size and position they are given at creation.
  static const nuiWindowFlags NoMove;         ///< The window cannot be moved by the user, even if it have a caption bar.
  static const nuiWindowFlags NoResize;       ///< The window can't be resized by the user.
  static const nuiWindowFlags Raw;            ///< The window is raw and will not draw it self. Its children have to do this job. This flag implies NoCaption.
  static const nuiWindowFlags NoCaption;      ///< The window have no caption bar. It implies also the NoClose flag.
  static const nuiWindowFlags NoClose;        ///< The window have no to close button.
  static const nuiWindowFlags Modal;          ///< The window if modal: no other window created before it can receive any mouse of keyboard event until it is destroyed.
  static const nuiWindowFlags DecoratedBackground; ///< Apply a default decoration to the client part of the window. By default the window has a white client rect
  virtual nuiRect GetRectFromFlags(const nuiRect& Rect, nuiWindowFlags flags, bool RectIsClient);

  // Construction/Destruction
  nuiWindow();
  nuiWindow(const nuiRect& rRect, nuiWindowFlags Flags=NoFlag, nglString Title=nglString(_T("nuiWindow")));
  virtual ~nuiWindow();

  // Parenting:
  virtual bool IsParentActive() const; ///< Return true if this object is the active object.
  virtual bool SetParent(nuiContainerPtr pParent);

  // Rendering & theme ops:
  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
//  virtual bool IsInside(nuiSize X, nuiSize Y) const;
  virtual void Invalidate();

  // Events callbacks:
  virtual void OnActivation();    ///< Called when the window gains the keyboard focus.
  virtual void OnDesactivation(); ///< Called when the window looses the keyboard focus.

  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved  (nuiSize X, nuiSize Y);

  // Flags:
  virtual void SetFlags(nuiWindowFlags Flags);
  virtual nuiWindowFlags GetFlags();
  virtual bool IsModal(); ///< Returns true if the window is modal. Every window is modeless by default.
  virtual bool SetModal(bool modal = false); ///< Changes the behaviour of the window (modal or modeless). Returns true if the modal flag change succeded. Every window is modeless by default.
  virtual void SetTitle(const nglString& rTitle); ///< Change the title of the window.
  virtual nglString GetTitle(); ///< Get the title of the window.

  // nui Events:
  nuiSimpleEventSource<nuiButtonPressed> CloseButtonPressed;
  nuiSimpleEventSource<nuiValueChanged> FlagsChanged;
  
protected:
  friend class nuiWindowManager;
  friend class nuiWindowDecoration;

  void InitProperties();

  bool mModal; ///< True if the window is modal. Every window is modeless by default.
  bool mStaticChildren; ///< This flag is true if the children keep their original size and positions.
  bool mNoResize;
  bool mNoBorder;
  bool mNoCaption;
  bool mNoClose;
  bool mNoMove;
  bool mRawWindow;
  bool mDecoratedBackground;
  nglString mTitle;

  nuiSize mMinimumHeight;
  nuiSize mMinimumWidth;

  nuiWindowManager* mpManager;
  
  void HandleCloseButtonPressed(const nuiEvent& rEvent);

  nuiEventSink<nuiWindow> mNuiWindowSink;

  nuiWidget* mpCloseButton;

  nuiSize mClickX, mClickY; ///< Mouse coordinates captured from last click.
  nuiRect mClickRect;   ///< Window rect captured from last click.
  enum 
  {
    eNoMove,
    ePreMove,
    eMove
  } mMoving;
  nuiPosition mClickPos; ///< Indicate where the last click was (Center is inside the client area, Fill is the caption bar, the other values are the border if the window is resizable).
};

//typedef nuiSmartPtr<nuiWindow> nuiWindowPtr;
typedef nuiWindow* nuiWindowPtr;
typedef std::list<nuiWindowPtr> nuiWindowList;

#endif // __nuiWindow_h__

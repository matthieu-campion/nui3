/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWindowManager_h__
#define __nuiWindowManager_h__

#include "nuiContainer.h"

class nuiWindow;
typedef nuiWindow* nuiWindowPtr;
class nuiWindow;

class NUI_API nuiWindowManager : public nuiSimpleContainer
{
public:
  nuiWindowManager();
  virtual ~nuiWindowManager();

  /* @name Popup windows management */
  //@{
  // The Active window receives all the keyboard events and dipatches them to its widgets. It also is the top level window.
  nuiWindow* GetActiveWindow() const; ///< Return the current active popup window.
  bool ActivateWindow(nuiWindow* pWindow); ///< Change the current active popup window.
  bool IsParentActive() const; ///< Allways true for the mainwindow!
  uint GetWindowCount() const; ///< Return the number of popup windows contained in the main window.
  nuiWindow* GetWindow(uint index); ///< Return the window object with the given index rank in the list.
  nuiWindow* GetWindow(nuiSize X, nuiSize Y, bool ClientAreaOnly = false); ///< Return the window object directly under the (X,Y) pixel. If \param ClientAreaOnly is true the window decorations are not taken into account when searching for the window.
  bool ValidateWindowRect(nuiRect& rRect) const; ///< Keep the given rect inside the window manager. Returns false if the given rect was invalid and had to be moved.
  //@}

  virtual nuiWidgetPtr GetChild(nuiSize X, nuiSize Y); ///< Reimplemented from nuiContainer::GetChild
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  /* @name Events management */
  //@{
  virtual bool KeyDown     (const nglKeyEvent& rEvent);
  virtual bool KeyUp       (const nglKeyEvent& rEvent);
  virtual bool DispatchMouseClick  (const nglMouseInfo& rInfo);
  virtual bool DispatchMouseUnclick(const nglMouseInfo& rInfo);
  //@}

  /* @name Background methods*/
  //@{
  void SetBackgroundColor(const nuiColor& rColor); ///< Set unique bg color.
  //@}

  /* @name Inherited from nuiWidget */
  //@{
  bool Draw(class nuiDrawContext *);
  bool IsInsideFromSelf(nuiSize X, nuiSize Y);
  bool AddChild(nuiWidgetPtr pChild);
  bool DelChild(nuiWidgetPtr pChild);
  //@}

private:
  /** This class contains informations about each window contained in the window manager */
  void WindowFlagChanged(const nuiEvent& rEvent);
  std::list<nuiWindow*> mWindows;

  nuiEventSink<nuiWindowManager> mWMSink;
};

/*
class NUI_API nuiWindow : public nuiSimpleContainer
{
public:
nuiWindow(nuiWindowManager* pParent, nuiWindow* pWindow);
virtual ~nuiWindow();

nuiWindow* GetWindow();

virtual bool Draw(class nuiDrawContext * pContext);
virtual nuiRect CalcIdealSize();
virtual void ApplyWindowFlags(const nuiEvent& rEvent);
virtual void TargetRectChanged(const nuiEvent& rEvent);
virtual void HandleCloseButtonPressed(const nuiEvent& rEvent);
};
*/


#endif // __nuiWindowManager_h__


/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiMainWindow_h__
#define __nuiMainWindow_h__

//#include "nui.h"
#include "nuiTopLevel.h"

class nuiLabel;
class nuiMainWindow;
class nuiTimer;
class nuiMainMenu;

class NUI_API nuiContextInfo : public nglContextInfo
{
public:
  enum Type
  {
    StandardContext2D,
    StandardContext3D,
    OffscreenContext2D,
    OffscreenContext3D
  };

  nuiContextInfo(Type type = StandardContext2D);
};

/// This class implements the root nui object: the main window of any application.
class NUI_API nuiMainWindow :  public nuiTopLevel
{
public:
  nuiMainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglContext* pShared = NULL, const nglPath& mResPath = nglPath(ePathCurrent));
  nuiMainWindow(uint Width = 320, uint Height = 240, bool Fullscreen=false, const nglPath& mResPath = nglPath(ePathCurrent));
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual ~nuiMainWindow();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;


  bool IsKeyDown (nglKeyCode Key) const;
//  void GetMouseInfo(nglMouseInfo& rMouseInfo);

  /* @name Inherited from nuiWidget */
  //@{
  virtual void InvalidateLayout();
  virtual void BroadcastInvalidate(nuiWidgetPtr pSender);
  virtual void BroadcastInvalidateLayout(nuiWidgetPtr pSender, bool BroadCastOnly);
  virtual void BroadcastInvalidateRect(nuiWidgetPtr pSender, const nuiRect& rRect);
  bool SetMouseCursor(nuiMouseCursor Cursor);
  //@}

  /** @name Debugging helper functions */
  //@{
  void DBG_DisplayMouseOverInfo(); ///< Displays a tool tip near the mouse with informations on the object currently pointed to by the mouse.
  void DBG_SetMouseOverInfo(bool set);
  bool DBG_GetMouseOverInfo();
  void DBG_DisplayMouseOverObject(); ///< Displays a frame over the object currently pointed to by the mouse.
  void DBG_SetMouseOverObject(bool set);
  bool DBG_GetMouseOverObject();
  //@}

  void SetQuitOnClose(bool Set);
  bool GetQuitOnClose() const;
  bool Trash();

  void SetDebugMode(bool Set);

  /** @name Redirected to nglWindow via nuiMainWindow::NGLWindow */
  //@{
  void BeginSession();
  nglContext* GetNGLContext() const;
  nglWindow* GetNGLWindow() const;
  void SetState (nglWindow::StateChange State);
  uint32 GetWidth() const;
  uint32 GetHeight() const;
  int32 GetPosX() const;
  int32 GetPosY() const;
  bool SetSize(uint Width, uint Height);
  bool SetPos(int x, int y);
  void SetRotation(uint Angle);
  uint GetRotation() const;
  void EnableAutoRotation(bool set); ///< Change the rotation and size of the screen to follow the device's screen orientation (this is the default behaviour).
  bool GetAutoRotation() const; ///< Change the rotation and size of the screen to follow the device's screen orientation (this is the default behaviour).
  uint GetError() const;               ///< Retrieve the current error code
  const nglChar* GetErrorStr() const;            ///< Retrieve the current error message
  const nglChar* GetErrorStr(uint Error) const;  ///< Retrieve error message by code

  virtual void OnDragEnter();
  virtual void OnDragLeave();
  virtual nglDropEffect OnCanDrop (nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button);

  virtual void OnDropped (nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button);
  
  class NUI_API WidgetCanDrop : public TestWidgetFunctor
  {
  public:
    WidgetCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y) 
    { mpDragObject = pDragObject; mX = X; mY = Y; };
    ~WidgetCanDrop() {};
    
    bool operator()(nuiWidgetPtr pWidget)
    {
      nuiSize x = mX, y = mY;
      pWidget->GlobalToLocal(x, y);
      return (pWidget->OnCanDrop(mpDragObject, x, y) != eDropEffectNone);
    }

  protected:
    nglDragAndDrop* mpDragObject;
    nuiSize mX, mY;
  };
  virtual bool Drag(nuiWidget* pDragSource, nglDragAndDrop* pDragObject);
  virtual void OnDragRequestData (nglDragAndDrop* pDragObject, const nglString& rMimeType);
  virtual void OnDragStop(bool canceled);
  
  //@}

  virtual void EnterModalState();
  virtual void ExitModalState();
  virtual void ForceRepaint(); ///< Process events that have already arrived on this window's event queue (but don't process events that are not for THIS particular window).
  
  virtual void StartTextInput(int32 X, int32 Y, int32 W, int32 H);
  virtual void EndTextInput();
  virtual bool IsEnteringText() const;
  
  void SetPaintEnabled(bool set);
  bool IsPaintEnabled() const;
  
  void SetMainMenu(nuiMainMenu* pMainMenu);
  nuiMainMenu* GetMainMenu();
  
  // attributes
  nuiRect GetWindowRect() const;
  void SetWindowRect(nuiRect rect);

  double GetLastInteractiveEventTime() const;
  double GetLastEventTime() const;
protected:
  /* @name Redirected from nglWindow via nuiMainWindow::NGLWindow */
  //@{
  virtual void OnPaint();
  virtual void Paint();
  virtual void LazyPaint();
  virtual void OnResize (uint Width, uint Height);
  virtual void OnCreation();
  virtual void OnDestruction();
  virtual void OnActivation();
  virtual void OnDesactivation();
  virtual void OnClose();
  virtual void OnState(nglWindow::StateInfo State);
  virtual void OnTextCompositionStarted();
  virtual void OnTextCompositionConfirmed();
  virtual void OnTextCompositionCanceled();
  virtual void OnTextCompositionUpdated(const nglString& rString, int32 CursorPosition);
  virtual nglString OnGetTextComposition() const;
  virtual void OnTextCompositionIndexToPoint(int32 CursorPosition, float& x, float& y) const;
  virtual bool OnTextInput(const nglString& rUnicodeText);
  virtual void OnTextInputCancelled();
  virtual bool OnKeyDown     (const nglKeyEvent& rEvent);
  virtual bool OnKeyUp       (const nglKeyEvent& rEvent);
  virtual bool OnMouseClick  (nglMouseInfo& rInfo);
  virtual bool OnMouseUnclick(nglMouseInfo& rInfo);
  virtual bool OnMouseMove   (nglMouseInfo& rInfo);
  virtual bool OnRotation    (uint Angle);
  //@}

  nuiEventSink<nuiMainWindow> mMainWinSink;

  bool mQuitOnClose;
  float mMaxFPS;
  nglTime mLastRendering;
  void InvalidateTimer(const nuiEvent& rEvent);
  bool mPaintEnabled;
  bool mDebugSlowRedraw;

  nuiWidget*      mpDragSource; ///< widget that has initialized a drag operation
  nuiWidget*      mpWidgetCanDrop; ///< if not NULL, this is the last widget that return true to an OnCanDrop call (used for DragLeave)
  
  bool ShowWidgetInspector();
private:
  
  // attributes
  void InitAttributes();
  
  /* @name Debugging */
  //@{
  bool mDisplayMouseOverInfo;
  bool mDisplayMouseOverObject;
  //@}

  bool mDebugMode;
//  bool ShowWidgetInspector();
  nuiMainWindow* mpInspectorWindow;

  bool mInvalidatePosted;
  
  friend class NGLWindow;

  class NUI_API NGLWindow : public nglWindow
  {
  public:
    NGLWindow(nuiMainWindow* pMainWindow, uint Width, uint Height, bool FullScreen);
    NGLWindow(nuiMainWindow* pMainWindow, const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglContext* pShared);
    virtual ~NGLWindow();

    /* @name Inheritance from nglWindow */
    //@{
    virtual void OnPaint();
    virtual void OnResize (uint Width, uint Height);
    virtual void OnCreation();
    virtual void OnDestruction();
    virtual void OnActivation();
    virtual void OnDesactivation();
    virtual void OnClose();
    virtual void OnState(StateInfo State);
    virtual bool OnTextInput(const nglString& rUnicodeText);
    virtual void OnTextInputCancelled();
    virtual bool OnKeyDown(const nglKeyEvent& rEvent);
    virtual bool OnKeyUp(const nglKeyEvent& rEvent);
    virtual bool OnMouseClick(nglMouseInfo& rInfo);
    virtual bool OnMouseUnclick(nglMouseInfo& rInfo);
    virtual bool OnMouseMove(nglMouseInfo& rInfo);
    virtual bool OnRotation(uint Angle);

    virtual void OnTextCompositionStarted(); ///< Tells the widget that a complex text input session is starting (mostly used to enter diacritics with dead keys and complex scripts like east asian glyphs)
    virtual void OnTextCompositionConfirmed(); ///< Tells the widget to confirm the composed text as final and end the composition session start with TextCompositionStart.
    virtual void OnTextCompositionCanceled(); ///< Tells the widget to cancel the composed text input and end the composition session started with TextCompositionStart.
    virtual void OnTextCompositionUpdated(const nglString& rString, int32 CursorPosition);
    virtual nglString OnGetTextComposition() const;
    virtual void OnTextCompositionIndexToPoint(int32 CursorPosition, float& x, float& y) const;
    
    
    // Dnd receive
    virtual void OnDragEnter();
    virtual void OnDragLeave();
    virtual nglDropEffect OnCanDrop(nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button);
    virtual void OnDropped(nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button);

    // Dnd send
    virtual bool Drag(nglDragAndDrop* pDragObject);
    virtual void OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType);
    virtual void OnDragStop(bool canceled);
    //@}

  private:
    nuiMainWindow* mpMainWindow;
  };

  NGLWindow* mpNGLWindow;
  uint mFullFrameRedraw;
  void OnInspectorDeath(const nuiEvent& rEvent);
  double mLastInteractiveEventTime;
  double mLastEventTime;
  
  double mLastPaint;
  double mFPSDelay;
  uint32 mFPSCount;
  float mFPS;
};

#endif // __nuiMainWindow_h__

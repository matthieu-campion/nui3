/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglCarbonDragAndDrop.h
\brief Carbon specific Handlers and Procedures for Drag and drop operations
*/

#ifndef __nglDragCarbonDragAndDrop_h__
#define __nglDragCarbonDragAndDrop_h__

#include "nui.h"
#include "nglKernel.h"
#include "nglWindow.h"
#include "nglDragAndDropObjects.h"
#include "nglDataObjects.h"

OSErr nglDragTrackingHandler (DragTrackingMessage message, WindowRef theWindow, void * handlerRefCon, DragRef theDrag);
OSErr nglDragReceiveHandler(WindowRef theWindow, void * handlerRefCon, DragRef theDrag);
OSErr nglDragSendData(FlavorType theType, void * dragSendRefCon, DragItemRef theItemRef, DragRef theDrag);

void PrintErr(OSErr err);
DragActions GetDragActions(nglDropEffect effect);
void SetAllowedDropEffects(nglDragAndDrop* pObject, DragActions actions);

class nglCarbonDragAndDrop
{
public:
  nglCarbonDragAndDrop(nglWindow* pWin, WindowRef winRef);
  ~nglCarbonDragAndDrop();

  bool Drag(nglDragAndDrop* pDragObject);

  void SetDropObject(nglDragAndDrop* pDrop) { mpDropObject = pDrop; }
  void SetDragObject(nglDragAndDrop* pDrag) { mpDragObject = pDrag; }
  nglDragAndDrop* GetDropObject()           { return mpDropObject; }
  nglDragAndDrop* GetDragObject()           { return mpDragObject; }
  
  bool HasDropObject()                      { return mpDropObject ? true : false; }
  bool HasDragObject()                      { return mpDragObject ? true : false; }

  void SetCanDrop(bool can)                 { mCanDrop = can; }
  bool CanDrop()                            { return mCanDrop; }

protected:
  nglWindow*  mpWin;
  WindowRef   mWinRef;

  bool        mCanDrop;
  
  nglDragAndDrop* mpDragObject;
  nglDragAndDrop* mpDropObject;
  
  
  EventRecord mEventRecord; // stupid but needed...
  DragRef     mDragRef;
  DragTrackingHandlerUPP  mDragTrackingHandler;
  DragReceiveHandlerUPP   mDragReceiveHandler;
  DragSendDataUPP         mSendProc;

  friend OSErr nglDragSendData(FlavorType theType, void * dragSendRefCon, DragItemRef theItemRef, DragRef theDrag);
  friend OSErr nglDragReceiveHandler(WindowRef theWindow, void * handlerRefCon, DragRef theDrag);
  friend OSErr nglDragTrackingHandler (DragTrackingMessage message, WindowRef theWindow, void * handlerRefCon, DragRef theDrag);
  friend class nglWindow;
};

#endif//__nglDragCarbonDragAndDrop_h__

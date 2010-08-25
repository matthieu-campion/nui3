/*
 NGL - C++ cross-platform framework for OpenGL based applications
 Copyright (C) 2000-2003 NGL Team
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "nui.h"
#include "nglApplication.h"
#include "nglVideoMode.h"
#include "nglContext.h"
#include "nglWindow.h"
#include "nglKeyboard.h"
#include "ngl_carbon.h"

#include "nglImage.h"
#include "nuiMouseCursor.h"

#include "nuiMainMenu.h"

#define NGL_WINDOW_EBASE      (NGL_CONTEXT_ELAST+1)

/*
 #define NGL_WINDOW_ENONE      0 (in nglWindow.h)
 */
#define NGL_WINDOW_ESYSWIN    (NGL_WINDOW_EBASE+1)
#define NGL_WINDOW_EVIDMODE   (NGL_WINDOW_EBASE+2)
#define NGL_WINDOW_ERANGE     (NGL_WINDOW_EBASE+3)

const nglChar* gpWindowErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("System window creation failed"),
/*  2 */ _T("Full screen video mode unavailable"),
/*  3 */ _T("Properties out of range"),
NULL
};

/*
 #define EVENTCLASS 'pute'
 #define EVENTTYPE 'PUte'
 */

/*
 #define EVENTCLASS kEventClassWindow
 #define EVENTTYPE kEventWindowDrawContent
 */

TSMDocumentID nuiTSMDocument;

/*
 * OS specific info
 */

nglWindow::OSInfo::OSInfo()
{
  WindowHandle = NULL;
  Parent = NULL;
}

pascal void nglWindowTimerAction(EventLoopTimerRef theTimer, void* userData)
{
  nglWindow* pWindow = (nglWindow*)userData;
  if (pWindow)
    pWindow->TimerAction();
}

void nglWindow::TimerAction()
{
  //  if (mInvalidatePosted)// && !mRedrawing)
  {
    OSStatus err;
    EventRef outEvent;
    err = CreateEvent( NULL, kEventClassWindow, kEventWindowDrawContent , GetCurrentEventTime(), kEventAttributeNone /*kEventAttributeUserEvent*/, &outEvent);
    EventTargetRef Target = GetWindowEventTarget(mWindow);
    err = SetEventParameter(outEvent, kEventParamPostTarget, typeEventTargetRef, sizeof(void*), &Target);
    err = PostEventToQueue(GetMainEventQueue(), outEvent, kEventPriorityStandard);
    //    ReleaseEvent(outEvent);
    //    mRedrawing = true;
  }
}

/*
 * Drag And Drop support
 */

#include "nglCarbonDragAndDrop.h"

void PrintErr(OSErr err)
{
  switch (err)
  {
    case procNotFound:
      NGL_OUT(_T("Proc Not Found Error\n")); 
      break;    
    case badDragRefErr:
      NGL_OUT(_T("Unknown drag reference\n"));
      break;
    case badDragItemErr:
      NGL_OUT(_T("Unknown drag item reference\n"));
      break;
    case badDragFlavorErr:
      NGL_OUT(_T("Unknown flavor type\n"));
      break;
    case duplicateFlavorErr:
      NGL_OUT(_T("Flavor type already exists\n"));
      break;
    case cantGetFlavorErr:
      NGL_OUT(_T("Error while trying to get flavor data\n"));
      break;
    case duplicateHandlerErr:
      NGL_OUT(_T("Handler already exists\n"));
      break;
    case handlerNotFoundErr:
      NGL_OUT(_T("Handler not found\n"));
      break;
    case dragNotAcceptedErr:
      NGL_OUT(_T("Drag was not accepted by receiver\n"));
      break;
    case unsupportedForPlatformErr:
      NGL_OUT(_T("Call is for PowerPC only\n"));
      break;
    case noSuitableDisplaysErr:
      NGL_OUT(_T("No displays support translucency\n"));
      break;
    case badImageRgnErr:
      NGL_OUT(_T("Bad translucent image region\n"));
      break;
    case badImageErr:
      NGL_OUT(_T("Bad translucent image PixMap\n"));
      break;
    case nonDragOriginatorErr:
      NGL_OUT(_T("Illegal attempt to access originator only data\n"));
      break;
    default:
      if (!err)
        NGL_OUT(_T("No error returned\n"));
      else
        NGL_OUT(_T("Unknown error returned\n"));
      break;
  }
}


DragActions GetDragActions(nglDropEffect effect)
{
  switch (effect)
  {
    case eDropEffectCopy:
      return kDragActionCopy;
      break;
    case eDropEffectMove:
      return kDragActionMove;
      break;
    case eDropEffectLink:
      return kDragActionAlias;
      break;
    default:
      return kDragActionNothing;
      break;
  }
}

void SetAllowedDropEffects(nglDragAndDrop* pObject, DragActions actions)
{
  if (actions & kDragActionNothing)
  {
    pObject->AddSupportedDropEffect(eDropEffectNone);
    return;
  }
  else if (actions & kDragActionAll)
  {
    pObject->AddSupportedDropEffect(eDropEffectCopy);
    pObject->AddSupportedDropEffect(eDropEffectMove);
    pObject->AddSupportedDropEffect(eDropEffectLink);
    return;
  }
  
  if (actions & kDragActionCopy)
    pObject->AddSupportedDropEffect(eDropEffectCopy);
  if (actions & kDragActionMove)
    pObject->AddSupportedDropEffect(eDropEffectMove);
  if (actions & kDragActionAlias)
    pObject->AddSupportedDropEffect(eDropEffectLink);
}


nglCarbonDragAndDrop::nglCarbonDragAndDrop(nglWindow* pWin, WindowRef winRef)
: mpWin(pWin), mWinRef(winRef)
{
  mDragTrackingHandler = NewDragTrackingHandlerUPP((DragTrackingHandlerUPP)(nglDragTrackingHandler));
  mDragReceiveHandler = NewDragReceiveHandlerUPP((DragReceiveHandlerUPP)(nglDragReceiveHandler));
  
  mSendProc = NewDragSendDataUPP((DragSendDataProcPtr)(nglDragSendData));
  
  OSStatus osErr;
  osErr = InstallTrackingHandler(mDragTrackingHandler, mWinRef, this);
  osErr = InstallReceiveHandler(mDragReceiveHandler, mWinRef, this);
  
  mpDropObject = NULL;
  mpDragObject = NULL;
}

nglCarbonDragAndDrop::~nglCarbonDragAndDrop()
{
  RemoveTrackingHandler(mDragTrackingHandler, mWinRef);    
  RemoveReceiveHandler(mDragReceiveHandler, mWinRef);
  
  DisposeDragTrackingHandlerUPP(mDragTrackingHandler);
  DisposeDragReceiveHandlerUPP(mDragReceiveHandler);
  DisposeDragSendDataUPP(mSendProc);
}

bool nglCarbonDragAndDrop::Drag(nglDragAndDrop* pDragObject)
{
  //NGL_OUT(_T("\n\nnglCarbonDragAndDrop::Drag\n\n"));
  OSErr err = noErr;
  mpDragObject = pDragObject; 
  
  err = NewDrag(&mDragRef);
  NGL_ASSERT(!err);
  
  DragItemRef item = 1;
  
  const std::map<nglString, nglDataObject*>& typesMap( pDragObject->GetSupportedTypesMap() );
  for ( std::map<nglString, nglDataObject*>::const_iterator it = typesMap.begin(); it != typesMap.end(); it++ )
  {
    nglDataObject* pObj = it->second;
    if (pObj)
    {
      nglNativeObjectType type = pObj->GetNativeType();
      pObj->AddDragItemFlavor(mDragRef, item, type);
    }
  }
  
  err = SetDragSendProc(mDragRef, mSendProc, this);
  NGL_ASSERT(!err);
  
  mDragRgn = NewRgn();
  Point offsetPt;
  SetPt(&offsetPt, 0, 0);
  LocalToGlobal(&offsetPt);
  OffsetRgn(mDragRgn, offsetPt.h, offsetPt.v);
	
	const nglImage *pImage = pDragObject->GetFeedbackImage();
	
	if (pImage)
	{
		size_t imgSize = pImage->GetWidth() * pImage->GetHeight() * pImage->GetPixelSize();
		CGDataProviderRef dataProviderRef = CGDataProviderCreateWithData(NULL, pImage->GetBuffer(), imgSize, NULL);
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		
		CGBitmapInfo bitmapInfo;
		switch (pImage->GetPixelFormat())
		{
			case eImagePixelRGBA:
				bitmapInfo = kCGImageAlphaLast;
				break;
			case eImagePixelRGB:
				bitmapInfo = kCGImageAlphaNone;
				break;
			case eImagePixelAlpha:
				bitmapInfo = kCGImageAlphaOnly;
				break;
		}
		
		CGImageRef imgRef = CGImageCreate (pImage->GetWidth(), pImage->GetHeight(), 8, pImage->GetPixelSize() * 8,
                                       pImage->GetBytesPerLine(),
                                       colorSpace,
                                       bitmapInfo,
                                       dataProviderRef,
                                       NULL,
                                       FALSE,
                                       kCGRenderingIntentDefault);
		CGColorSpaceRelease(colorSpace);
		CGDataProviderRelease(dataProviderRef);
		
		size_t width = CGImageGetWidth(imgRef);
		size_t height = CGImageGetHeight(imgRef);
		HIPoint imgOffset;
		imgOffset.x = -(float)pDragObject->GetFeedbackImageOffsetX();
		imgOffset.y = -(float)pDragObject->GetFeedbackImageOffsetY();
		
		err = SetDragImageWithCGImage (mDragRef, imgRef, &imgOffset, kDragStandardTranslucency);
		CGImageRelease(imgRef);
	}
  
  
  err = TrackDrag(mDragRef, &mEventRecord, mDragRgn);


  bool res = !err ? true : false;
  
  DisposeRgn (mDragRgn);
  
  err = DisposeDrag(mDragRef);
  
  NGL_ASSERT(!err);
  delete mpDragObject;
  mpDragObject = NULL;

  //NGL_OUT(_T("\n\nnglCarbonDragAndDrop::Drag DONE\n\n"));
  return res;
  
}

OSErr nglDragSendData(FlavorType theType, void * dragSendRefCon, DragItemRef theItemRef, DragRef theDrag)
{
  nglCarbonDragAndDrop* pDnd = (nglCarbonDragAndDrop*)dragSendRefCon;
  
  NGL_ASSERT(pDnd);
  NGL_ASSERT(pDnd->HasDragObject());
  nglDragAndDrop* pDrag = pDnd->GetDragObject();
  
  nglString mime;
  if (App->GetDataTypesRegistry().GetRegisteredMimeType(theType, mime))
  {
    if (pDrag->IsTypeSupported(mime))
    {
      pDnd->mpWin->OnDragRequestData(pDrag, mime);
      nglDataObject* pObj = (nglDataObject*)pDrag->GetType(mime);
      NGL_ASSERT(pObj);
      
      pObj->SetDragItemFlavorData(theDrag, theItemRef, theType);    
    }
  }
  
  return noErr;
}


OSErr nglDragReceiveHandler(WindowRef theWindow, void * handlerRefCon, DragRef theDrag)
{
  OSErr err = noErr;
  nglCarbonDragAndDrop* pDnd = (nglCarbonDragAndDrop*)handlerRefCon;
  NGL_ASSERT(pDnd);
  
  if (!pDnd->CanDrop())
    return dragNotAcceptedErr;
  
  nglDragAndDrop* pDrag = pDnd->GetDropObject();
  
  UInt16 numItems;
  err = CountDragItems (theDrag, &numItems);
  NGL_ASSERT(!err);
  
  /*for (UInt16 i = 1; i <= numItems; i++)
  {
    DragItemRef theItemRef;
    err = GetDragItemReferenceNumber (theDrag, i, &theItemRef);
    NGL_ASSERT(!err);
    UInt16 numFlavors;
    err = CountDragItemFlavors (theDrag, theItemRef, &numFlavors);
    NGL_ASSERT(!err);
    
    // test types
    
    for (UInt16 f = 1; f <= numFlavors; f++)
    {
      FlavorType theType;
      err = GetFlavorType (theDrag, theItemRef, f, &theType);
      NGL_ASSERT(!err);
      
      nglString mime;
      if (App->GetDataTypesRegistry().GetRegisteredMimeType(theType, mime))
      {
        nglDataObject* pObj = pDrag->GetType(mime);
        if (!pObj)
        {
          pObj = App->GetDataTypesRegistry().Create(mime);
          pDrag->AddType(pObj);
        }
        pObj->GetFlavorData(theDrag, theItemRef, theType);
      }
    }
  }*/
  
  
  Point mouse;
  err = GetDragMouse(theDrag, &mouse, NULL);
  NGL_ASSERT(!err);
	SetPort(GetWindowPort(pDnd->mpWin->mWindow));
  GlobalToLocal(&mouse);
  nglMouseInfo::Flags Button;
	
	DragAttributes flags;
	err = GetDragAttributes(theDrag, &flags);
	int XOffset = pDnd->mpWin->mXOffset;
	int YOffset = pDnd->mpWin->mYOffset;
	if (flags & kDragInsideSenderWindow)
	{
		XOffset = 0;
		YOffset = 0;
	}
	
  
  pDnd->mpWin->OnDropped(pDnd->GetDropObject(), mouse.h - XOffset, mouse.v - YOffset, Button);
  /* Clean up Done in nglDragTrackingHandler when leaving Window, which also occurs after a drop */

  return noErr;
}

OSErr nglDragTrackingHandler (DragTrackingMessage message, WindowRef theWindow, void * handlerRefCon, DragRef theDrag)
{
  OSErr err = noErr;
  nglCarbonDragAndDrop* pDnd = (nglCarbonDragAndDrop*)handlerRefCon;
  NGL_ASSERT(pDnd);
  
   
  switch (message)
  {
    case kDragTrackingEnterHandler:
      //NGL_OUT(_T("nglDragTrackingHandler(kDragTrackingEnterHandler, 0x%x, 0x%x, 0x%x)\n"), theWindow, handlerRefCon, theDrag);
      //Dont really care ..
      break;
      
    case kDragTrackingEnterWindow:
      {
        //NGL_OUT(_T("nglDragTrackingHandler(kDragTrackingEnterWindow, 0x%x, 0x%x, 0x%x)\n"), theWindow, handlerRefCon, theDrag);
        NGL_ASSERT(!pDnd->HasDropObject());
        
        nglDragAndDrop* pDrag = new nglDragAndDrop(eDropEffectCopy);
        pDnd->SetDropObject(pDrag);
        
        DragActions actions;
        err = GetDragAllowableActions(theDrag, &actions);
        NGL_ASSERT(!err);
        SetAllowedDropEffects(pDrag, actions);
        
        UInt16 numItems;
        err = CountDragItems (theDrag, &numItems);
        NGL_ASSERT(!err);
        
        for (UInt16 i = 1; i <= numItems; i++)
        {
          DragItemRef theItemRef;
          err = GetDragItemReferenceNumber (theDrag, i, &theItemRef);
          NGL_ASSERT(!err);
          UInt16 numFlavors;
          err = CountDragItemFlavors (theDrag, theItemRef, &numFlavors);
          NGL_ASSERT(!err);
          
          for (UInt16 f = 1; f <= numFlavors; f++)
          {
            FlavorType theType;
            err = GetFlavorType (theDrag, theItemRef, f, &theType);
            NGL_ASSERT(!err);
            
            nglString mime;
            if (App->GetDataTypesRegistry().GetRegisteredMimeType(theType, mime))
            {
              nglDataObject* pObj = pDrag->GetType(mime);
              if (!pObj)
              {
                pObj = App->GetDataTypesRegistry().Create(mime);
                pDrag->AddType(pObj);
              }
              pObj->GetFlavorData(theDrag, theItemRef, theType);
            }
          }
        }
        pDnd->mpWin->OnDragEnter();
      }
      break;
      
      
    case kDragTrackingInWindow:
      { 
        //NGL_OUT(_T("nglDragTrackingHandler(kDragTrackingInWindow, 0x%x, 0x%x, 0x%x)\n"), theWindow, handlerRefCon, theDrag);
        Point mouse;
        err = GetDragMouse (theDrag, &mouse, NULL);
        SetPort(GetWindowPort(pDnd->mpWin->mWindow));
        GlobalToLocal(&mouse);
        nglMouseInfo::Flags Button;
        
        NGL_ASSERT(pDnd->GetDropObject());
        DragAttributes flags;
        err = GetDragAttributes(theDrag, &flags);
        int XOffset = pDnd->mpWin->mXOffset;
        int YOffset = pDnd->mpWin->mYOffset;
        if (flags & kDragInsideSenderWindow)
        {
          XOffset = 0;
          YOffset = 0;
        }
        
        bool can = pDnd->mpWin->OnCanDrop(pDnd->GetDropObject(), mouse.h - XOffset, mouse.v - YOffset, Button);
        pDnd->SetCanDrop(can);
        
        if (can)
        {
          nglDropEffect effect = pDnd->GetDropObject()->GetDesiredDropEffect();
          DragActions actions = GetDragActions(effect);
          SetDragDropAction(theDrag, actions);
        }
        else
          SetDragDropAction (theDrag, kDragActionNothing);


      }
      break;
      
      
      
    case kDragTrackingLeaveWindow:
      {
        //NGL_OUT(_T("nglDragTrackingHandler(kDragTrackingLeaveWindow, 0x%x, 0x%x, 0x%x)\n"), theWindow, handlerRefCon, theDrag);
        pDnd->mpWin->OnDragLeave();
        NGL_ASSERT(pDnd->HasDropObject());
        delete pDnd->GetDropObject();
        pDnd->SetDropObject(NULL);
      }
      break;
      
      
    case kDragTrackingLeaveHandler:
      {
        //Dont really care ..
        //NGL_OUT(_T("nglDragTrackingHandler(kDragTrackingLeaveHandler, 0x%x, 0x%x, 0x%x)\n"), theWindow, handlerRefCon, theDrag);
      }
      break;
      
    default:
      break;
  }

  // We try to keep events flowing but only in order to keep the window updated, so we watch for Invalidate events:
//  EventRef event;
//  EventTargetRef eventDispatcher = GetEventDispatcherTarget();
//  
//  while ( pDnd->mpWin->mInvalidatePosted && ReceiveNextEvent( 0, NULL, 0.0, TRUE, &event ) == noErr )
//  {
//    SendEventToEventTarget (event, eventDispatcher);
//    ReleaseEvent(event);
//  }
  if (pDnd->mpWin->mInvalidatePosted)
  {
    pDnd->mpWin->mInvalidatePosted = false;
    //BeginUpdate(mWindow);                /* this sets up the visRgn */
    pDnd->mpWin->CallOnPaint();
    //EndUpdate(mWindow);
    
    pDnd->mpWin->mRedrawing = false;
  }
  
  
	return err;
}

/*
 * Constructors
 */

nglWindow::nglWindow (uint Width, uint Height, bool IsFullScreen)
{
  mInited = false;
  mTitle = "";
  
  nglContextInfo context; // Get default context
  nglWindowInfo info(Width, Height, IsFullScreen);
  
  mEventHandlerRef = NULL;
  mKeyboardEventHandlerRef = NULL;
  mpCarbonDragAndDrop = NULL;
  mLastButtonDown = nglMouseInfo::ButtonNone;
  
  mpMainMenu = NULL;
	
  
  InternalInit (context, info, NULL);
}

nglWindow::nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  mInited = false;
  mTitle = "";
	
	mEventHandlerRef = NULL;
  mKeyboardEventHandlerRef = NULL;
	mpCarbonDragAndDrop = NULL;
  
  mpMainMenu = NULL;
  
  InternalInit (rContext, rInfo, pShared);
}

/* Current status
 RelativeMouse : to tune (grab is akward)
 FullScreen : todo
 */

int ngl_scode_table[0x80] = 
{
/*  0 */ NK_A, NK_S, NK_D, NK_F, NK_H, NK_G, NK_Z, NK_X, NK_C, NK_V,
/* 10 */ NK_GRAVE, NK_B, NK_Q, NK_W, NK_E, NK_R, NK_Y, NK_T, NK_1, NK_2,
/* 20 */ NK_3, NK_4, NK_6, NK_5, NK_EQUAL, NK_9, NK_7, NK_MINUS, NK_8, NK_0,
/* 30 */ NK_RBRACKET, NK_O, NK_U, NK_LBRACKET, NK_I, NK_P, NK_ENTER, NK_L, NK_J, NK_APOSTROPHE,
/* 40 */ NK_K, NK_SEMICOLON, NK_NUMBERSIGN, NK_COMMA, NK_SLASH, NK_N, NK_M, NK_PERIOD, NK_TAB, NK_SPACE,
/* 50 */ NK_BACKSLASH, NK_BACKSPACE, 0, NK_ESC, 0, 0, 0, 0, 0, 0,
/* 60 */ 0, 0, 0, 0, 0, NK_PAD_PERIOD, 0, NK_PAD_ASTERISK, 0, NK_PAD_PLUS,
/* 70 */ 0, NK_PAD_LOCK, 0, 0, 0, NK_PAD_SLASH, NK_PAD_ENTER, 0, NK_PAD_MINUS, 0,
/* 80 */ 0, 0, NK_PAD_0, NK_PAD_1, NK_PAD_2, NK_PAD_3, NK_PAD_4, NK_PAD_5, NK_PAD_6, NK_PAD_7,
/* 90 */ 0, NK_PAD_8, NK_PAD_9, 0, 0, 0, NK_F5, NK_F6, NK_F7, NK_F3,
/* 100 */ NK_F8, NK_F9, 0, NK_F11, NK_SYSREQ, 0, NK_SCRLOCK, 0, 0, NK_F10,
/* 110 */ NK_MENU, NK_F12, 0, NK_PAUSE, 0, NK_HOME, NK_PAGEUP, NK_DELETE, NK_F4, NK_END,
/* 120 */ NK_F2, NK_PAGEDOWN, NK_F1, NK_LEFT, NK_RIGHT, NK_DOWN, NK_UP 
};

pascal OSStatus nglWindowEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData)
{
  nglWindow* pWindow = (nglWindow*) userData;
  
  if (!pWindow)
  {
    printf("Invalid window in nglWindowEventHandler!\n");
    return noErr;
  }
  return pWindow->WindowEventHandler (eventHandlerCallRef, eventRef, userData);
}

pascal OSStatus nglWindowKeyboardEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData)
{
  nglWindow* pWindow = (nglWindow*) userData;
  
  if (!pWindow)
  {
    printf("Invalid window in nglWindowKeyboardEventHandler!\n");
    return noErr;
  }
  return pWindow->WindowKeyboardEventHandler (eventHandlerCallRef, eventRef, userData);
}

bool IsThisKeyDown(const short theKey)
{
  union
  {
    KeyMap asMap;
    Byte asBytes[16];
  }u;
  
  GetKeys(u.asMap);
  return u.asBytes[theKey >> 3] & (1 << (theKey & 0x07)) ? true : false;
}


bool IsCommandKeyDown()
{
  const short kCommandKey = 55;
  return IsThisKeyDown(kCommandKey);
}

bool IsControlKeyDown()
{
  const short kCtlKey = 0x3B;
  return IsThisKeyDown(kCtlKey);
}

bool IsOptionKeyDown()
{
  const short kOptionKey = 58;
  return IsThisKeyDown(kOptionKey);
}

bool IsShiftKeyDown()
{
  const short kShiftKey = 56;
  return IsThisKeyDown(kShiftKey);
}

bool nglWindow::IsKeyDown (nglKeyCode Key) const
{
  if (Key == NK_LMETA || Key == NK_RMETA || Key == NK_META)
    return IsCommandKeyDown();
  else if (Key == NK_LCTRL || Key == NK_RCTRL || Key == NK_CTRL)
    return IsControlKeyDown();
  else if (Key == NK_LSHIFT || Key == NK_RSHIFT || Key == NK_SHIFT)
    return IsShiftKeyDown();
  else if (Key == NK_LALT || Key == NK_RALT || Key == NK_ALT)
    return IsOptionKeyDown();
  else if ((Key > 0) && (Key <= NGL_KEY_MAX))
    return mpKeyState[Key-1];
  else
    return false;
}


OSStatus nglWindow::WindowKeyboardEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData)
{
  InitDragAndDrop();
  
  OSStatus result = eventNotHandledErr;
  UInt32 eventKind;
  eventKind = GetEventKind(eventRef);
  UInt32 eventClass = GetEventClass (eventRef);
  
  ConvertEventRefToEventRecord(eventRef, &(mpCarbonDragAndDrop->mEventRecord)); // just in case this event initiates a drag
  
  switch (eventClass)
  {
    case kEventClassKeyboard:
    {
      uint16 unicodetext;
      uint16 rawunicodetext = 0;
      uint32 keycode;
      if (eventKind != kEventRawKeyModifiersChanged)
      {
        OSStatus err = GetEventParameter (eventRef, kEventParamKeyCode, typeUInt32, NULL, sizeof(keycode), NULL, &keycode);
        if (err != noErr)
          return eventNotHandledErr;
        err = GetEventParameter (eventRef, kEventParamKeyUnicodes, typeUnicodeText, NULL, sizeof(unicodetext), NULL, &unicodetext);
        rawunicodetext = unicodetext;
#if 1
        // Try to get the Raw Key Code (that is the unicode string for the pressed key without dead keys and modifiers:
        KeyboardLayoutRef layoutRef;
        UCKeyboardLayout* uchrKeyLayout;
        UInt32 deadKeyState;
        const int kMaxUnicodeInputStringLength = 32;
        UniChar unicodeInputString[kMaxUnicodeInputStringLength];
        OSStatus status; 
        
        // initialization
        KLGetCurrentKeyboardLayout(&layoutRef);
        status = KLGetKeyboardLayoutProperty(layoutRef, kKLuchrData, (const void **)(&uchrKeyLayout));
        
        
        // if there is a 'uchr' for the current keyboard layout, 
        // use it
        if (uchrKeyLayout != NULL)
        {
          UInt32 keyboardType;
          UInt32 modifierKeyState;
          UInt16 virtualKeyCode;
          UInt16 keyAction;
          UniCharCount actualStringLength;
          
          virtualKeyCode = keycode;
          keyAction = keyDown;
          modifierKeyState = 0;
          keyboardType = LMGetKbdType();
          status = UCKeyTranslate(uchrKeyLayout, 
                                  virtualKeyCode, keyAction,
                                  modifierKeyState, keyboardType, 0,
                                  &deadKeyState,
                                  kMaxUnicodeInputStringLength,
                                  &actualStringLength, unicodeInputString);
          // now do something with status and unicodeInputString
          if (status == noErr)
          {
            rawunicodetext = unicodeInputString[0];
            //printf("Raw unicode key : 0x%x (%c)\n", rawunicodetext, (char)rawunicodetext);
          }
        }
        else
        {
          // no 'uchr' resource, do something with 'KCHR'?
          // add your code here
          Ptr kchrKeyLayout;
          status = KLGetKeyboardLayoutProperty(layoutRef, kKLKCHRData, (const void **)(&kchrKeyLayout));
          if (kchrKeyLayout)
          {
            UInt32 state = 0;
            rawunicodetext = KeyTranslate(kchrKeyLayout, keycode, &state);
          }
        }
#endif        
        
        if (err != noErr)
          return eventNotHandledErr;
      }
      
      switch (eventKind)
      {
        case kEventRawKeyModifiersChanged:
        {
          UInt32 modKeys;
          UInt32 upKeys;
          UInt32 dnKeys;
          GetEventParameter(eventRef, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(typeUInt32), NULL, &modKeys);
          upKeys = ~modKeys & mModifiers;
          dnKeys = modKeys & ~mModifiers;
          mModifiers = modKeys;
          
          bool res = false;
          if (upKeys & shiftKey)
            res |= CallOnKeyUp(nglKeyEvent(NK_LSHIFT, 0, 0));
          if (upKeys & controlKey)
            res |= CallOnKeyUp(nglKeyEvent(NK_LCTRL, 0, 0));
          if (upKeys & optionKey)
            res |= CallOnKeyUp(nglKeyEvent(NK_LALT, 0, 0));
          
          if (dnKeys & shiftKey)
            res |= CallOnKeyDown(nglKeyEvent(NK_LSHIFT, 0, 0));
          if (dnKeys & controlKey)
            result |= CallOnKeyDown(nglKeyEvent(NK_LCTRL, 0, 0));
          if (dnKeys & optionKey)
            res |= CallOnKeyDown(nglKeyEvent(NK_LALT, 0, 0));
          
          //NGL_OUT("ModifiersChanged %ls\n", YESNO(res));
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;          
        }
          break;
        case kEventRawKeyDown:
        {      
          //NGL_OUT("%2.2d\"%c\"\n", keycode, (char)unicodetext, unicodetext);
          bool res = CallOnKeyDown(nglKeyEvent(ngl_scode_table[keycode],unicodetext,rawunicodetext));
          //NGL_OUT("KeyDown %ls\n", YESNO(res));
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
        }
          break;
          
        case kEventRawKeyUp:
        {
          //NGL_OUT("KeyUp 0x%x ['%c' (0x%x)]\n", keycode, (char)unicodetext, unicodetext);
          bool res = CallOnKeyUp(nglKeyEvent(ngl_scode_table[keycode],unicodetext, rawunicodetext));
          //NGL_OUT("KeyUp %ls\n", YESNO(res));
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
        }
          break;
        case kEventRawKeyRepeat:
        {
          //NGL_OUT("KeyRepeat\n");
          bool res = CallOnKeyDown(nglKeyEvent(ngl_scode_table[keycode],unicodetext, rawunicodetext));
          //NGL_OUT("KeyRepeat %ls\n", YESNO(res));
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
        }
          break;
      }
    }
      break;
    case kEventClassTextInput:
    {
      switch (eventKind)
      {
        case kEventTextInputUnicodeText:
          {
            printf("kEventTextInputUnicodeText\n");
            UInt32 size = 0;
            OSStatus err = GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, NULL, &size, NULL);
            //printf("GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, NULL, &size, NULL) = %d [size = %d]\n", err, size);
            
            uint16 unicodetext[size + 1];
            memset(unicodetext, 0, sizeof(uint16) * (size + 1));
            err = GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, size, NULL, unicodetext);
            //printf("GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, size, NULL, unicodetext) = %d\n", err);
            //for (uint i = 0; i < size; i++)
            //{
            //  printf("%d: '%c' {%d}\n", i, unicodetext[i], unicodetext[i]);
            //}
            if (err != noErr)
              return eventNotHandledErr;
            
            nglChar ucs4[size+1];
            memset(ucs4, 0, sizeof(nglChar) * (size + 1));
            for (uint i = 0; i < size; i++)
              ucs4[i] = unicodetext[i];
            nglString str(ucs4);
            //char* pStr = str.Export(eUTF8);
            //printf("Unicode text entered: '%s' [%d] {%x}\n", pStr, str.GetLength(), str[0]);
            //delete[] pStr;
            if (CallOnTextInput(str))
              result = noErr;
          }
          break;
        case kEventTextInputUnicodeForKeyEvent:
          {
            printf("kEventTextInputUnicodeForKeyEvent\n");
            UInt32 size = 0;
            OSStatus err = GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, NULL, &size, NULL);
            //printf("GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, NULL, &size, NULL) = %d [size = %d]\n", err, size);
            
            uint16 unicodetext[size + 1];
            memset(unicodetext, 0, sizeof(uint16) * (size + 1));
            err = GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, size, NULL, unicodetext);
            //printf("GetEventParameter(eventRef, kEventParamTextInputSendText, typeUnicodeText, NULL, size, NULL, unicodetext) = %d\n", err);
            //for (uint i = 0; i < size; i++)
            //{
            //  printf("%d: '%c' {%d}\n", i, unicodetext[i], unicodetext[i]);
            //}
            if (err != noErr)
              return eventNotHandledErr;
            
            nglChar ucs4[size+1];
            memset(ucs4, 0, sizeof(nglChar) * (size + 1));
            for (uint i = 0; i < size; i++)
              ucs4[i] = unicodetext[i];
            nglString str(ucs4);
            //char* pStr = str.Export(eUTF8);
            //printf("Unicode text entered: '%s' [%d] {%x}\n", pStr, str.GetLength(), str[0]);
            //delete[] pStr;
            if (CallOnTextInput(str))
              result = noErr;
          }
          break;
          
        case kEventTextInputShowHideBottomWindow:
          {
            Boolean r = false;
            OSErr err = GetEventParameter(eventRef, kEventParamTextInputSendShowHide, typeBoolean, NULL, sizeof(r), NULL, &r);
            printf("kEventTextInputShowHideBottomWindow\n");
            if (r)
              printf("kEventTextInputShowHideBottomWindow Activated Input window\n");
            else
              printf("kEventTextInputShowHideBottomWindow Disabled Input window\n");
            result = eventNotHandledErr;  
          }
          break;
          
        case kEventTextInputFilterText:
          {
            printf("kEventTextInputFilterText\n");
          }
          break;
          
          
        case kEventTextInputUpdateActiveInputArea:
          {
            printf("kEventTextInputUpdateActiveInputArea\n");
          }
          break;
          
        case kEventTextInputPosToOffset:
          {
            printf("kEventTextInputPosToOffset\n");
          }
          break;
          
        case kEventTextInputGetSelectedText:
          {
            printf("kEventTextInputGetSelectedText\n");
          }
          break;
          
        case kEventTextInputOffsetToPos:
          {
            printf("kEventTextInputOffsetToPos\n");
            long byte_offset;
            Point p;
            
            OSErr err = GetEventParameter(eventRef, kEventParamTextInputSendTextOffset, typeLongInteger, NULL, sizeof (long), NULL, &byte_offset);
            if (err != noErr)
              break;
            
            p.h = 50;
            p.v = 100;
            
            //              SetEventParameter (event, kEventParamTextInputReplyPointSize, typeFixed, sizeof (Fixed), &point_size);
            //              SetEventParameter (event, kEventParamTextInputReplyLineHeight, typeShortInteger, sizeof (short), &height);
            //              SetEventParameter (event, kEventParamTextInputReplyLineAscent, typeShortInteger, sizeof (short), &ascent);
            err = SetEventParameter (eventRef, kEventParamTextInputReplyPoint, typeQDPoint, sizeof (Point), &p);
            if (err == noErr)
              result = noErr;
          }
          break;
      }
    }
      break;
  }
  
  return result;
}


void nglWindow::InitDragAndDrop()
{
  if (mInited == false)
  {
    
    /*
     * Drag and Drop stuff
     */
    App->GetDataTypesRegistry().RegisterDataType(_T("ngl/Text"), 'TEXT', nglDataTextObject::Create);
    App->GetDataTypesRegistry().RegisterDataType(_T("ngl/Files"), kDragFlavorTypeHFS, nglDataFilesObject::Create);
		App->GetDataTypesRegistry().RegisterDataType(_T("ngl/PromiseFiles"), kDragFlavorTypePromiseHFS, nglDataFilesObject::Create);
    mpCarbonDragAndDrop = new nglCarbonDragAndDrop(this, mWindow);
    
    CallOnCreation();
    mInited = true;
    
		Rect Bounds;
		MakeCurrent();
		InvalWindowRect(mWindow, GetWindowPortBounds(mWindow, &Bounds));
		
		if (!mIsFakeChildWindow)
		{
			CallOnResize(Bounds.right, Bounds.bottom);
		}
		else
		{
			CallOnResize(mWindowWidth, mWindowHeight);
		}
  }
}

OSStatus nglWindow::WindowEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData)
{
  InitDragAndDrop();

  OSStatus result = eventNotHandledErr;
  UInt32 eventKind;
  eventKind = GetEventKind(eventRef);
  UInt32 eventClass = GetEventClass (eventRef);
  
  ConvertEventRefToEventRecord(eventRef, &(mpCarbonDragAndDrop->mEventRecord)); // just in case this event initiates a drag
  
  switch (eventClass)
  {
    case kEventClassMouse:
    {
      Point mouseLocation;
      GetEventParameter (eventRef, kEventParamMouseLocation, typeQDPoint, 
                         NULL, sizeof(mouseLocation), NULL, &mouseLocation);
      
			Rect winBounds;
			GetWindowBounds(mWindow, kWindowContentRgn, &winBounds);
			mouseLocation.v -= winBounds.top;
			mouseLocation.h -= winBounds.left;
			
      switch (eventKind)
      {
        case kEventMouseDragged:
        case kEventMouseMoved:
        {
          nglMouseInfo minfo;
          minfo.X = mouseLocation.h - mXOffset;
          minfo.Y = mouseLocation.v - mYOffset;
          minfo.Buttons = 0; // FIXME
          minfo.TouchId = 0;
          bool res = CallOnMouseMove(minfo);
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
        }
          break;
        case kEventMouseUp:
        {
          nglMouseInfo minfo;
          UInt16 button;
          GetEventParameter (eventRef, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
          minfo.Buttons = nglMouseInfo::ButtonLeft;
          minfo.TouchId = 0;
          switch (button)
          {
            case kEventMouseButtonPrimary: 
              minfo.Buttons = IsControlKeyDown()? nglMouseInfo::ButtonRight : nglMouseInfo::ButtonLeft;
              break;
            case kEventMouseButtonSecondary: 
              minfo.Buttons = nglMouseInfo::ButtonRight;
              break;
            case kEventMouseButtonTertiary: 
              minfo.Buttons = nglMouseInfo::ButtonMiddle;
              break;
          }
          minfo.X = mouseLocation.h - mXOffset;
          minfo.Y = mouseLocation.v - mYOffset;
          bool res = CallOnMouseUnclick(minfo);
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
        }
          break;
        case kEventMouseDown:
        {
          SetUserFocusWindow(mWindow);
          nglMouseInfo minfo;
          UInt16 button;
          GetEventParameter (eventRef, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
          minfo.Buttons = nglMouseInfo::ButtonLeft;
          minfo.TouchId = 0;
          switch (button)
          {
            case kEventMouseButtonPrimary: 
              minfo.Buttons = IsControlKeyDown()? nglMouseInfo::ButtonRight : nglMouseInfo::ButtonLeft;
              break;
            case kEventMouseButtonSecondary: 
              minfo.Buttons = nglMouseInfo::ButtonRight;
              break;
            case kEventMouseButtonTertiary: 
              minfo.Buttons = nglMouseInfo::ButtonMiddle;
              break;
          }
          
          UInt32 count;
          GetEventParameter (eventRef, kEventParamClickCount, typeUInt32, NULL, sizeof(count), NULL, &count);
          if (count>1)
          {
            if (minfo.Buttons & mLastButtonDown)
            {
              minfo.Buttons |= nglMouseInfo::ButtonDoubleClick;
            }
          }
			    mLastButtonDown = minfo.Buttons;
          
          minfo.X = mouseLocation.h - mXOffset;
          minfo.Y = mouseLocation.v - mYOffset;
          bool res = CallOnMouseClick(minfo);
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
          //Debugger();
          if (!IsWindowActive(mWindow))
          {
            SelectWindow(mWindow);
            ActivateWindow(mWindow, true);
            
            // returns as we didn t handle the event so the window get activated
            // a bit hacky but didn t found a way to do otherwise
            result = eventNotHandledErr;
          }
        }
          break;
        case kEventMouseWheelMoved:
        {
          long delta;
          GetEventParameter (eventRef, kEventParamMouseWheelDelta, typeLongInteger, NULL, sizeof(delta), NULL, &delta);
          EventMouseWheelAxis wheelAxis;
          GetEventParameter (eventRef, kEventParamMouseWheelAxis, typeMouseWheelAxis, NULL, sizeof(wheelAxis), NULL, &wheelAxis);
          nglMouseInfo minfo;
          
          if (wheelAxis == kEventMouseWheelAxisY)
          {
            if (delta < 0) 
              minfo.Buttons = nglMouseInfo::ButtonWheelDown;
            else
              minfo.Buttons = nglMouseInfo::ButtonWheelUp;
          }
          else
          {
            if (delta > 0) 
              minfo.Buttons = nglMouseInfo::ButtonWheelLeft;
            else
              minfo.Buttons = nglMouseInfo::ButtonWheelRight;
          }
          
          minfo.X = mouseLocation.h - mXOffset;
          minfo.Y = mouseLocation.v - mYOffset;
          
          bool res = CallOnMouseClick(minfo);
          res |= CallOnMouseUnclick(minfo);
          result = res ? (OSStatus)noErr : (OSStatus)eventNotHandledErr;
        }
          break;
        default:
          NGL_DEBUG( NGL_LOG(_T("window"), NGL_LOG_INFO, _T("WindowEventHandler: unhandled event %d\n"), eventKind); )
          break;
      }
    }
      break;
    case kEventClassWindow:
      result = noErr;
      switch (eventKind)
      {
        case kEventWindowDrawContent:
        case kEventWindowUpdate:
          //          printf("Event: Paint\n");
          mInvalidatePosted = false;
          
          //BeginUpdate(mWindow);                /* this sets up the visRgn */
          CallOnPaint();
          //EndUpdate(mWindow);
          
          mRedrawing = false;
          result = eventNotHandledErr;
          
          break;
        case kEventWindowInit:
          CallOnCreation();
          break;
        case kEventWindowActivated:
          ActivateTSMDocument(nuiTSMDocument);
          UseInputWindow(nuiTSMDocument, TRUE);
          RgnHandle rgn = NewRgn();
          SetRectRgn(rgn, 20, 20, 100, 30);
          TSMSetInlineInputRegion(nuiTSMDocument, mWindow, rgn);
          DisposeRgn(rgn);
          SetUserFocusWindow(mWindow);
          CallOnActivation();
          if (mIsFakeChildWindow)
            result = eventNotHandledErr;
          break;
        case kEventWindowDeactivated:
          DeactivateTSMDocument(nuiTSMDocument);
          CallOnDesactivation();
          if (mIsFakeChildWindow)
            result = eventNotHandledErr;
          break;
        case kEventWindowClose:
          //          printf("Event: Close\n");
          if (mIsFakeChildWindow)
            result = eventNotHandledErr;
          CallOnClose();
          break;
        case kEventWindowClosed:
          //          printf("Event: Closed\n");
          if (mIsFakeChildWindow)
            result = eventNotHandledErr;
          CallOnDestruction();
          break;
        case kEventWindowBoundsChanged:
        {
          // Window is being resized
          Rect bounds;
          
          if (mIsFakeChildWindow)
          {
            GLint bufferRect[4];
            Rect portBounds;
            GetWindowPortBounds(mWindow, &bounds);
            
            int iPortHeight = bounds.bottom - bounds.top;
            int iPortWidth = bounds.right - bounds.left;
            
            int iHeight = iPortHeight;
            int iY = iHeight - mYOffset;
            
            bufferRect[0] = mXOffset; // 0 = left edge
            bufferRect[1] = iY - mWindowHeight; // 0 = bottom edge
            bufferRect[2] = mWindowWidth; // width of buffer rect
            bufferRect[3] = mWindowHeight; // height of buffer rect
            aglSetInteger (mCtx, AGL_BUFFER_RECT, bufferRect);
            aglEnable (mCtx, AGL_BUFFER_RECT);
            
            MakeCurrent();
            InvalWindowRect(mWindow, &bounds);
            
            CallOnResize(mWindowWidth, mWindowHeight);
            
            result = eventNotHandledErr;
          }
          else
          {
            GetWindowPortBounds(mWindow, &bounds);
            MakeCurrent();
            InvalWindowRect(mWindow, &bounds);
            
            CallOnResize(bounds.right, bounds.bottom);
          }
          
          Invalidate();
        }
      }
      break;
  }
  
  return result;
}

void nglWindow::InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{  
  mModifiers = 0;
  mInvalidatePosted = false;
  mRedrawing = false;
  mInModalState = 0;
  mAngle = 0;
  EventTypeSpec WindowEvents[]=
  {
    // Mouse Class:
    { kEventClassMouse, kEventMouseMoved },
    { kEventClassMouse, kEventMouseDragged },                              
    { kEventClassMouse, kEventMouseDown },                              
    { kEventClassMouse, kEventMouseUp },
    { kEventClassMouse, kEventMouseWheelMoved },
    // Window class:
    //                                { kEventClassWindow, kEventWindowDrawContent},
    { kEventClassWindow, kEventWindowUpdate},
    { kEventClassWindow, kEventWindowInit},
    { kEventClassWindow, kEventWindowBoundsChanged},
    { kEventClassWindow, kEventWindowActivated},
    { kEventClassWindow, kEventWindowClose},
    { kEventClassWindow, kEventWindowClosed},
    { kEventClassWindow, kEventWindowDeactivated}
  };
  
  EventTypeSpec KeyboardEvents[]=
  {
    // Key Input class:
    { kEventClassKeyboard, kEventRawKeyDown},
    { kEventClassKeyboard, kEventRawKeyUp},
    { kEventClassKeyboard, kEventRawKeyRepeat},
    { kEventClassKeyboard, kEventRawKeyModifiersChanged},
    { kEventClassTextInput, kEventTextInputUnicodeForKeyEvent },
    { kEventClassTextInput, kEventTextInputUnicodeText },
    
    { kEventClassTextInput, kEventTextInputUpdateActiveInputArea },
    { kEventClassTextInput, kEventTextInputUnicodeForKeyEvent },
    { kEventClassTextInput, kEventTextInputOffsetToPos },
    { kEventClassTextInput, kEventTextInputPosToOffset },
    //kEventTSMDocumentAccessGetFirstRectForRange
    { kEventClassTextInput, kEventTextInputShowHideBottomWindow },
    { kEventClassTextInput, kEventTextInputGetSelectedText },
    { kEventClassTextInput, kEventTextInputUnicodeText },
    { kEventClassTextInput, kEventTextInputFilterText }
  };
  
  mpContext = NULL;
  mpVMode = NULL;
  mpLastVMode = NULL;
  SetError (NGL_WINDOW_ENONE);
  
	mIsFakeChildWindow = false;
	
	// we init those to zero so there is no offset in mouse handling by default
	mXOffset = 0;
	mYOffset = 0;
	
  Rect wRect;
  
  mOSInfo = rInfo.OSInfo;
  mTitle = rInfo.Title;  
  
  // set bounding rectangle
  if (rInfo.Pos == nglWindowInfo::ePosUser)
  {
    SetRect(&wRect,rInfo.XPos,rInfo.YPos,rInfo.XPos+rInfo.Width,rInfo.YPos+rInfo.Height); /* left, top, right, bottom */
  }
  else if (rInfo.Pos == nglWindowInfo::ePosCenter)
  {
    nglVideoMode videoMode;
    int w = videoMode.GetWidth();
    int h = videoMode.GetHeight();
    
    int x = (w - rInfo.Width)/2;
    int y = (h - rInfo.Height)/2;
    
    SetRect(&wRect,x,y,x+rInfo.Width,y+rInfo.Height); /* left, top, right, bottom */
  }
  else if (rInfo.Pos == nglWindowInfo::ePosMouse)
  {
  }
  else if (rInfo.Pos == nglWindowInfo::ePosAuto)
  {
    // TODO: implement correct behaviour
    SetRect(&wRect,50,50,50+rInfo.Width,50+rInfo.Height); /* left, top, right, bottom */
  }
  
  // set title
  char buffer[257];
  std::string str(rInfo.Title.GetStdString());
  snprintf(buffer+1,255,"%s",(const char*)str.c_str());
  buffer[0]=MIN(255,rInfo.Title.GetLength());
  
  if (!(rInfo.Flags & FullScreen))
  {    
    WindowClass windowClass = kDocumentWindowClass;
    WindowAttributes attributes = kWindowStandardHandlerAttribute | kWindowLiveResizeAttribute | kWindowCollapseBoxAttribute | 
      kWindowAsyncDragAttribute;
    
    //if (rInfo.Flags & FullScreen)
    //    {
    //      attributes |= kWindowNoConstrainAttribute;
    //      
    //      int x=0;
    //      int y=0;
    //      
    //      if (rInfo.Pos == nglWindowInfo::ePosUser)
    //      {
    //        x = rInfo.XPos;
    //        y = rInfo.YPos;
    //      }
    //      else if (rInfo.Pos == nglWindowInfo::ePosCenter)
    //      {
    //        nglVideoMode videoMode;
    //        int w = videoMode.GetWidth();
    //        int h = videoMode.GetHeight();
    //        
    //        x = (w - rInfo.Width)/2;
    //        y = (h - rInfo.Height)/2;      
    //      }
    //      
    //      SetRect(&wRect,x,y,x+rInfo.Width,y+rInfo.Height); /* left, top, right, bottom */
    //    }
    if (rInfo.Flags & NoBorder)
    {
      attributes |= kWindowNoTitleBarAttribute;
      
      int x=0;
      int y=0;
      
      if (rInfo.Pos == nglWindowInfo::ePosUser)
      {
        x = rInfo.XPos;
        y = rInfo.YPos;
      }
      else if (rInfo.Pos == nglWindowInfo::ePosCenter)
      {
        nglVideoMode videoMode;
        int w = videoMode.GetWidth();
        int h = videoMode.GetHeight();
        
        x = (w - rInfo.Width)/2;
        y = (h - rInfo.Height)/2;      
      }
      
      SetRect(&wRect,x,y,x+rInfo.Width,y+rInfo.Height); /* left, top, right, bottom */
    }
    else
    {
      if (!(rInfo.Flags & NoResize))
        attributes |= kWindowResizableAttribute | kWindowFullZoomAttribute;
      attributes |= kWindowCloseBoxAttribute; 
    }
    
		if (rInfo.OSInfo.Parent == 0)
		{
			mOSInfo.WindowHandle = NULL;
			OSStatus err = CreateNewWindow (windowClass, attributes, &wRect, &(mOSInfo.WindowHandle));
			SetWTitle(mOSInfo.WindowHandle, (const unsigned char*)buffer);
			//RepositionWindow(mOSInfo.WindowHandle, NULL, kWindowCascadeOnMainScreen);
			//TransitionWindow(mOSInfo.WindowHandle, kWindowZoomTransitionEffect, kWindowShowTransitionAction, NULL);
      HideWindow(mOSInfo.WindowHandle);
		}
		else
		{
			mOSInfo.WindowHandle = rInfo.OSInfo.Parent;
			if (mOSInfo.WindowHandle)
			{
				mIsFakeChildWindow = true;
				
				mXOffset = rInfo.XPos;
				mYOffset = rInfo.YPos;
				mWindowWidth = rInfo.Width;
				mWindowHeight = rInfo.Height;				
			}
		}
    mWindow = mOSInfo.WindowHandle;
    
    if (!nuiTSMDocument)
    {
      InterfaceTypeList supportedServices =
      {
        kUnicodeDocumentInterfaceType
      };
      NewTSMDocument(1, supportedServices, &nuiTSMDocument, 0);
      // We don't support inline input yet, use input window by default
      UseInputWindow(nuiTSMDocument, FALSE);
    }
    
    if (!mWindow)
    {
      return;
    }
    
    if (!mIsFakeChildWindow)
		{
      ChangeWindowAttributes(mWindow, kWindowStandardHandlerAttribute, 0);
    }
    InstallWindowEventHandler(mWindow,
                              NewEventHandlerUPP( ::nglWindowEventHandler), 
                              GetEventTypeCount(WindowEvents),
                              WindowEvents, 
                              (void*) this, 
                              &mEventHandlerRef);
    
    InstallWindowEventHandler(mWindow,
                              NewEventHandlerUPP( ::nglWindowKeyboardEventHandler), 
                              GetEventTypeCount(KeyboardEvents),
                              KeyboardEvents, 
                              (void*) this, 
                              &mKeyboardEventHandlerRef);
    
    SetPortWindowPort(mWindow);  /* set port to new window */
    
    if (rContext.TargetAPI == eTargetAPI_OpenGL)
    {
      if (!Build(mWindow, rContext, pShared, rInfo.Flags & FullScreen))
        return; // An error is already raised by nglContext's code
      
#ifndef __NOGLCONTEXT__
      if (mIsFakeChildWindow)
      {
        GLint bufferRect[4];
        Rect portBounds;
        GetWindowPortBounds(mWindow, &portBounds);
        
        int iPortHeight = portBounds.bottom - portBounds.top;
        int iPortWidth = portBounds.right - portBounds.left;
        
        int iHeight = iPortHeight;
        int iY = iHeight - rInfo.YPos;
        
        
        bufferRect[0] = rInfo.XPos; // 0 = left edge
        bufferRect[1] = iY - rInfo.Height; // 0 = bottom edge
        bufferRect[2] = rInfo.Width; // width of buffer rect
        bufferRect[3] = rInfo.Height; // height of buffer rect
        aglSetInteger (mCtx, AGL_BUFFER_RECT, bufferRect);
        aglEnable (mCtx, AGL_BUFFER_RECT);  
      }
#endif
    }
  }
  else // FULLSCREEN
  {
    SetSystemUIMode(kUIModeAllHidden, kUIOptionAutoShowMenuBar);
    
    WindowClass windowClass = kSimpleWindowClass; //kOverlayWindowClass;
    WindowAttributes attributes = kWindowNoAttributes;//kWindowStandardHandlerAttribute;
    //attributes |= kWindowNoConstrainAttribute;
    
    SetRect(&wRect,0,0,rInfo.Width,rInfo.Height); /* left, top, right, bottom */
    
    mOSInfo.WindowHandle = NULL;
    OSStatus err = CreateNewWindow (windowClass, attributes, &wRect, &(mOSInfo.WindowHandle));
    SetWTitle(mOSInfo.WindowHandle, (const unsigned char*)buffer);
    //RepositionWindow(mOSInfo.WindowHandle, NULL, kWindowCenterOnMainScreen);
    //TransitionWindow(mOSInfo.WindowHandle, kWindowZoomTransitionEffect, kWindowShowTransitionAction, NULL);
    mWindow = mOSInfo.WindowHandle;
    HideWindow(mWindow);
    if (!mWindow)
    {
      return;
    }
    
    
    //ChangeWindowAttributes(mWindow,kWindowStandardHandlerAttribute, 0);
    InstallWindowEventHandler(mWindow,
                              NewEventHandlerUPP( ::nglWindowEventHandler), 
                              GetEventTypeCount(WindowEvents),
                              WindowEvents, 
                              (void*) this, 
                              &mEventHandlerRef);
    
    InstallWindowEventHandler(mWindow,
                              NewEventHandlerUPP( ::nglWindowKeyboardEventHandler), 
                              GetEventTypeCount(KeyboardEvents),
                              KeyboardEvents, 
                              (void*) this, 
                              &mKeyboardEventHandlerRef);
    
    SetPortWindowPort(mWindow);  /* set port to new window */
    
    if (rContext.TargetAPI == eTargetAPI_OpenGL)
    {
      if (!Build(mWindow, rContext, pShared, rInfo.Flags & FullScreen))
        return; // An error is already raised by nglContext's code
#ifndef __NOGLCONTEXT__
      GLint swap = 0;
      aglEnable(mCtx, AGL_SWAP_INTERVAL);
      aglSetInteger(mCtx, AGL_SWAP_INTERVAL, &swap);
      //  swap = 1;
      //  aglEnable(mCtx, AGL_SWAP_LIMIT);
      //  aglSetInteger(mCtx, AGL_SWAP_LIMIT, &swap);
      
      glClearColor(0,0,0,0);
      glClear(GL_COLOR_BUFFER_BIT);
#endif
    }
  }
  
  Invalidate();
  mInited = false;
}


nglWindow::~nglWindow()
{
  //  App->DelWindow (this);
  //  App->DelEvent (this);
  
  // automatic deleting of the main menu
  if (mpMainMenu)
  {
    mpMainMenu->UnregisterFromWindow(this);
    delete mpMainMenu;
  }
  
  if (mpCarbonDragAndDrop)
    delete mpCarbonDragAndDrop;
  
  if (mpLastVMode)
  {
    nglVideoMode::SetMode (mpLastVMode, false);
    delete mpLastVMode;
    delete mpVMode;
  }
  
	if (mEventHandlerRef)
		RemoveEventHandler(mEventHandlerRef);
	
	if (mKeyboardEventHandlerRef)
		RemoveEventHandler(mKeyboardEventHandlerRef);
	
  if (mTimer)
    RemoveEventLoopTimer(mTimer);
  
	if (!mIsFakeChildWindow)
	{
		if (mWindow != nil)
			DisposeWindow (mWindow);
#ifdef __NGL_MACHO__
    //CGReleaseAllDisplays (); // is this needed ?
#endif
	}
  else
  {
    //NGL_OUT("nglWindow: Destroying the gl context!\n");
    Destroy();
  }
}


/*
 * All services
 */

void nglWindow::SetState (StateChange State)
{
  //  static bool was_mapped = false;
  //  static int last_x, last_y;
  switch (State)
  {
    case eHide:
      HideWindow(mWindow);
      break;
    case eShow:
      CallOnPaint();
      ShowWindow(mWindow);
      CollapseWindow(mWindow, false);
      break;
    case eMinimize:
      CollapseWindow(mWindow, true);
			break;
		case eMaximize:
			ShowWindow(mWindow);
      CollapseWindow(mWindow, false);
			if (!IsWindowInStandardState(mWindow, NULL, NULL))
			{
				Rect rect;
				GetWindowBounds(mWindow, kWindowGlobalPortRgn, &rect);
				SetWindowIdealUserState(mWindow, &rect);
				SetWindowUserState(mWindow, &rect);
			}
			ZoomWindow(mWindow, inZoomOut, false);
      break;
  };
}

nglWindow::StateInfo nglWindow::GetState() const
{
  return eVisible;
}

void nglWindow::GetSize (uint& rWidth, uint& rHeight) const
{
	if (!mIsFakeChildWindow)
	{
		Rect Bounds;
		GetWindowBounds (mWindow, kWindowContentRgn, &Bounds); 
		rWidth = Bounds.right - Bounds.left;
		rHeight = Bounds.bottom - Bounds.top;
	}
	else
	{
		rWidth = mWindowWidth;
		rHeight = mWindowHeight;
	}
}

uint nglWindow::GetWidth () const
{
	if (!mIsFakeChildWindow)
	{
		Rect Bounds;
		GetWindowBounds (mWindow, kWindowContentRgn, &Bounds); 
		return Bounds.right - Bounds.left;
	}
	else
	{
		return mWindowWidth;
	}
}

uint nglWindow::GetHeight () const
{
	if (!mIsFakeChildWindow)
	{
		Rect Bounds;
		GetWindowBounds (mWindow, kWindowContentRgn, &Bounds); 
		return Bounds.bottom - Bounds.top;
	}
	else
	{
		return mWindowHeight;
	}
}

bool nglWindow::SetSize (uint Width, uint Height)
{
  Rect Bounds;
  GetWindowBounds (mWindow, kWindowContentRgn, &Bounds); 
  if (Width < 1) Width = 1;
  if (Height < 1) Height = 1;
  Bounds.right = Bounds.left + Width;
  Bounds.bottom = Bounds.top + Height;
  SetWindowBounds(mWindow, kWindowContentRgn, &Bounds);
  return true;
}

void nglWindow::GetPosition (int& rXPos, int& rYPos) const
{
  Rect Bounds;
  GetWindowBounds (mWindow, kWindowContentRgn, &Bounds); 
  rXPos = Bounds.left;
  rYPos = Bounds.top;
}

bool nglWindow::SetPosition (int XPos, int YPos)
{
  Rect Bounds;
  GetWindowBounds (mWindow, kWindowContentRgn, &Bounds); 
  if (XPos < 0) XPos = 0;
  if (YPos < 0) YPos = 0;
  int Width = Bounds.right - Bounds.left;
  int Height = Bounds.bottom - Bounds.top;
  Bounds.left = XPos;
  Bounds.top = YPos;
  Bounds.right = XPos + Width;
  Bounds.bottom = YPos + Height;
  SetWindowBounds(mWindow, kWindowContentRgn, &Bounds);
}

nglString nglWindow::GetTitle() const
{
  return mTitle;
}

void nglWindow::SetTitle (const nglString& rTitle)
{
  mTitle = rTitle;
  std::string str(rTitle.GetStdString());
  CFStringRef windowTitle = CFStringCreateWithCString(kCFAllocatorDefault, str.c_str(), kCFStringEncodingMacRoman);
  SetWindowTitleWithCFString(mWindow, windowTitle);
  CFRelease(windowTitle);
}

void nglWindow::GetMouse (nglMouseInfo&, bool Local) const
{
  
}

const nglWindow::OSInfo* nglWindow::GetOSInfo() const
{
  return &mOSInfo;
}

void nglWindow::BeginSession()
{
  MakeCurrent();
}

void nglWindow::EndSession()
{
#ifndef __NOGLCONTEXT__
  //glFinish();
  aglSwapBuffers(mCtx);
  if (mValidBackBufferRequestedNotGranted)
  {
    glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_BACK);
    glCopyPixels(0,0, GetWidth(), GetHeight(), GL_COLOR);
    glReadBuffer(GL_BACK);
  }
#endif
}

void nglWindow::Invalidate()
{
  if (!mInvalidatePosted)
  {
    OSStatus err;
    EventRef outEvent;
    //    err = CreateEvent( NULL, kEventClassWindow, kEventWindowDrawContent , GetCurrentEventTime(), kEventAttributeNone /*kEventAttributeUserEvent*/, &outEvent);
    err = CreateEvent( NULL, kEventClassWindow, kEventWindowUpdate ,GetCurrentEventTime(), kEventAttributeNone /*kEventAttributeUserEvent*/, &outEvent);
    EventTargetRef Target = GetWindowEventTarget(mWindow);
    err = SetEventParameter(outEvent, kEventParamPostTarget, typeEventTargetRef, sizeof(void*), &Target);
    if (outEvent)
		{
			//err = PostEventToQueue(GetMainEventQueue(), outEvent, kEventPriorityLow); /// Normal window update events are low priority.
			err = PostEventToQueue(GetMainEventQueue(), outEvent, kEventPriorityLow);
			ReleaseEvent(outEvent);
		}
    
    mInvalidatePosted = true;
  }
}

bool nglWindow::SetCursor(nuiMouseCursor Cursor)
{
  switch (Cursor)
  {
      /*
       kThemeArrowCursor             = 0,
       kThemeCopyArrowCursor         = 1,
       kThemeAliasArrowCursor        = 2,
       kThemeContextualMenuArrowCursor = 3,
       kThemeIBeamCursor             = 4,
       kThemeCrossCursor             = 5,
       kThemePlusCursor              = 6,
       kThemeWatchCursor             = 7,    * Can Animate *
       kThemeClosedHandCursor        = 8,
       kThemeOpenHandCursor          = 9,
       kThemePointingHandCursor      = 10,
       kThemeCountingUpHandCursor    = 11,   * Can Animate *
       kThemeCountingDownHandCursor  = 12,   * Can Animate *
       kThemeCountingUpAndDownHandCursor = 13, * Can Animate *
       kThemeSpinningCursor          = 14,   * Can Animate *
       kThemeResizeLeftCursor        = 15,
       kThemeResizeRightCursor       = 16,
       kThemeResizeLeftRightCursor   = 17,
       kThemeNotAllowedCursor        = 18,   * available on Mac OS X 10.2 and later *
       
       * Available in Mac OS X 10.3 or later.
       kThemeResizeUpCursor          = 19,
       
       * Available in Mac OS X 10.3 or later.
       kThemeResizeDownCursor        = 20,
       
       * Available in Mac OS X 10.3 or later.
       kThemeResizeUpDownCursor      = 21,
       
       * A special cursor to indicate that letting up the mouse will cause
       * a dragged item to go away. When the item goes away, a poof cloud
       * animation should occur. This cursor should be updated dynamically
       * dependeding on whether the mouse up action will remove the item.
       * Available in Mac OS X 10.3 or later.
       kThemePoofCursor              = 22
       */
      
    case eCursorHelp:
      break;
    case eCursorDoNotSet:
      break;
    case eCursorNone:
      break;
    case eCursorArrow:
      ::SetThemeCursor(kThemeArrowCursor);
      break;
    case eCursorCross:
      ::SetThemeCursor(kThemeCrossCursor);
      break;
    case eCursorIBeam:
      ::SetThemeCursor(kThemeIBeamCursor);
      break;
    case eCursorHand:
      ::SetThemeCursor(kThemeOpenHandCursor);
      break;
    case eCursorClosedHand:
      ::SetThemeCursor(kThemeClosedHandCursor);
      break;
    case eCursorPointingHand:
      ::SetThemeCursor(kThemePointingHandCursor);
      break;
    case eCursorWait:
      ::SetThemeCursor(kThemeWatchCursor);
      break;
    case eCursorDnD:
      ::SetThemeCursor(kThemeCopyArrowCursor);
      break;
    case eCursorCaret:
      ::SetThemeCursor(kThemeIBeamCursor);
      break;
    case eCursorForbid:
      ::SetThemeCursor(kThemeNotAllowedCursor);
      break;
    case eCursorMove:
      ::SetThemeCursor(kThemeOpenHandCursor);
      break;
    case eCursorResize:
      ::SetThemeCursor(kThemeCrossCursor);
      break;
    case eCursorResizeNS:
      ::SetThemeCursor(kThemeResizeUpDownCursor);
      break;
    case eCursorResizeWE:
      ::SetThemeCursor(kThemeResizeLeftRightCursor);
      break;
    case eCursorResizeN:
      ::SetThemeCursor(kThemeResizeUpCursor);
      break;
    case eCursorResizeS:
      ::SetThemeCursor(kThemeResizeDownCursor);
      break;
    case eCursorResizeW:
      ::SetThemeCursor(kThemeResizeLeftCursor);
      break;
    case eCursorResizeE:
      ::SetThemeCursor(kThemeResizeRightCursor);
      break;
    case eCursorResizeNW:
      ::SetThemeCursor(kThemePointingHandCursor);
      break;
    case eCursorResizeNE:
      ::SetThemeCursor(kThemePointingHandCursor);
      break;
    case eCursorResizeSW:
      ::SetThemeCursor(kThemePointingHandCursor);
      break;
    case eCursorResizeSE:
      ::SetThemeCursor(kThemePointingHandCursor);
      break;
    default:
      break;
  }
  return true;
}

nuiMouseCursor nglWindow::GetCursor() const
{
  return eCursorNone;
}

nglWindow::EventMask nglWindow::GetEventMask() const
{
  return AllEvents;
}

void nglWindow::SetEventMask(EventMask Events)
{
  
}

bool nglWindow::GetResolution(float& rHorizontal, float& rVertical) const
{
  rHorizontal = rVertical = 72.f;
  return false;
}

// Drag and drop:
void nglWindow::OnDragEnter()
{

}

void nglWindow::OnDragLeave()
{
}

nglDropEffect nglWindow::OnCanDrop (nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button)
{
  return eDropEffectNone;
}

void nglWindow::OnDropped (nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button)
{
}

bool nglWindow::Drag(nglDragAndDrop* pDragObject)
{
  bool res = mpCarbonDragAndDrop->Drag(pDragObject);
  
  // advise drag source about result
  OnDragStop(!res /* canceled or not*/); 
  
  return res;
}


void nglWindow::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
}

void nglWindow::OnDragStop(bool canceled)
{
}

bool nglWindow::MakeCurrent() const
{
  if (mCtx)
  {
    return nglContext::MakeCurrent(mWindow);
  }
  else
  {
    return true;
  }
}

void nglWindow::EnterModalState()
{
  mInModalState++;
  uint32 state = mInModalState;
  
  SetState(eShow);
  
  ProcessSerialNumber CurrentProcess;
  ProcessSerialNumber FrontProcess;
  GetCurrentProcess(&CurrentProcess);
  GetFrontProcess(&FrontProcess);
  
  if (!IsWindowActive(mWindow) || CurrentProcess.highLongOfPSN != FrontProcess.highLongOfPSN || CurrentProcess.lowLongOfPSN != FrontProcess.lowLongOfPSN)
  {
    static NMRec nmr;
    bzero(&nmr, sizeof(nmr));
    nmr.nmMark = 1;
    nmr.qType = nmType;
    int32 iErr = NMInstall(&nmr);
  }
  
  
  while (state <= mInModalState && !((nglApplication*)App)->IsQuitRequested())
  {
    OSStatus err = RunAppModalLoopForWindow(mWindow);
    NGL_ASSERT(err == 0);
  }
  
}

void nglWindow::ExitModalState()
{
  mInModalState--;
  
  OSStatus err = QuitAppModalLoopForWindow(mWindow);
}

void nglWindow::StartTextInput(int32 X, int32 Y, int32 W, int32 H)
{
  //#FIXME
}

void nglWindow::EndTextInput()
{
  //#FIXME
}

bool nglWindow::IsEnteringText() const
{
  //#FIXME
  return false;
}



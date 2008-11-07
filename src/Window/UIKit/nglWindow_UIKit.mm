#include "nui.h"

#include "nglApplication.h"
#include "nglVideoMode.h"
#include "nglContext.h"
#include "nglWindow.h"
#include "nglKeyboard.h"
#include "nuiMouseCursor.h"

#include <QuartzCore/QuartzCore.h>


#include <string.h>

#include "nglWindow_UIKit.h"


//#include "nglImage.h"

#define NGL_WINDOW_EBASE      (NGL_CONTEXT_ELAST+1)
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

#ifndef _DEBUG_
#undef NGL_OUT
#define NGL_OUT {}
#endif

#if defined(_MULTI_TOUCHES_) && 0//defined(_DEBUG_)
# define NGL_TOUCHES_OUT NGL_OUT
#else//!_MULTI_TOUCHES_
# define NGL_TOUCHES_OUT {}
#endif//!_MULTI_TOUCHES_

//#define _DEBUG_WINDOW_

static std::list<nglTouchId> gAvailableTouches;
static nglTouchId gPressedTouches[_NUI_MAX_TOUCHES_];

void AdjustFromAngle(uint Angle, const nuiRect& rRect, nglMouseInfo& rInfo)
{
  switch (Angle)
  {
    case 90:
      {
        int tmpX= rInfo.X;
        rInfo.X = ((int)rRect.GetWidth()) - rInfo.Y;
        rInfo.Y = tmpX;
      }
      break;
    case 270:
      {
        int tmpY = rInfo.Y;
        rInfo.Y = ((int)rRect.GetHeight()) - rInfo.X;
        rInfo.X = tmpY;
      }
      break;
    case 180:
      {
        rInfo.X = ((int)rRect.GetWidth()) - rInfo.X;
        rInfo.Y = ((int)rRect.GetHeight()) - rInfo.Y;
      }
      break;
    case 0:
      {
      }
      break;
    default:
      {
        NGL_ASSERT(!"Unsupported rendering angle");
      }
  }
  NGL_TOUCHES_OUT(_T("AdjustFromAngle %d, X:%d Y:%d\n"), Angle, rInfo.X, rInfo.Y);
}

@implementation nglUIWindow

- (nglWindow *) getNGLWindow
{
	return self->mpNGLWindow;
}

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) rect andNGLWindow: (nglWindow*) pNGLWindow
{
//NGL_OUT(_T("[nglUIWindow initWithFrame]\n"));
  if ( (self = [super initWithFrame: rect]) )
  {
    mpNGLWindow = pNGLWindow;
  }
  else
  {
    NGL_ASSERT(!"initWithFrame: Could not initialize UIWindow");
  }
  mInited = false;
  mInvalidated = true;
  mInvalidationTimer = [NSTimer scheduledTimerWithTimeInterval:(1.0f/60.0f) target:self selector:@selector(Paint) userInfo:nil repeats:YES];

  return self;
}

- (void) dealloc
{
//NGL_OUT(_T("[nglUIWindow dealloc]\n"));
  mpNGLWindow->CallOnDestruction();
  [mInvalidationTimer release];
  [super dealloc];
}

- (void) InitNGLWindow
{
  if (!mInited)
  {
    mInited = true;
    mpNGLWindow->CallOnCreation();
    mpNGLWindow->Invalidate();
    mpNGLWindow->OnPaint();
  }
}

- (void) sendEvent: (UIEvent*) pEvent
{
  [self InitNGLWindow];
  
//  NGL_OUT(_T("[nglUIWindow sendEvent]\n"));
  nglTime t0;
//  [self dumpTouches: pEvent];
  nglWindow::EventMask mask = mpNGLWindow->GetEventMask();
  if (mask & nglWindow::MouseEvents)
  {
    [self handleEvent: pEvent];
  }
  nglTime t1;
  //NGL_OUT(_T("sendEvent: elapsed time: %f sec, handleEvent time: %f\n"),
  //        (double)(t1 - mLastEventTime), (double)(t1 - t0));

  mLastEventTime=t1;
  [super sendEvent: pEvent];
}
/*
- (void) touchesChangedWithEvent: (UIEvent*) pEvent
{
//NGL_OUT(_T("[nglUIWindow touchesChangedWithEvent]\n"));
  [super touchesChangedWithEvent: pEvent];
}
*/
- (void) dumpTouch: (UITouch*) pTouch
{
  UITouchPhase touchPhase = [pTouch phase];
  uint touchTapCount = [pTouch tapCount];

  CGPoint newp = [pTouch locationInView: (UIView*)self];
  CGPoint oldp = [pTouch previousLocationInView: (UIView*)self];    

  NGL_OUT(_T("Touch Info [%p] [@%d]: [%s], [%d], [x:%f y:%f]->[x:%f y:%f]\n"),
            pTouch, [pTouch timestamp],
            touchPhase == UITouchPhaseBegan ?       "Clicked"   :
            touchPhase == UITouchPhaseMoved ?       "Moved"     :
            touchPhase == UITouchPhaseStationary ?  "Static"    :
            touchPhase == UITouchPhaseEnded ?       "Unclicked" :
            touchPhase == UITouchPhaseCancelled ?   "Canceled"  : "Unknown",
            touchTapCount,
            oldp.x, oldp.y,
            newp.x, newp.y
          );
}
- (void) dumpTouches: (UIEvent*) pEvent
{
  NSSet* pSet = [pEvent allTouches];
  NGL_ASSERT(pSet);
  NSArray* pArray = [pSet allObjects];
  NGL_ASSERT(pArray);
  NSUInteger count = [pArray count];
  NGL_OUT(_T("Dump touches info: %d\n"), count);

  std::vector<UITouch*> touches;
  touches.resize((uint)count);
  [pArray getObjects: &touches[0]];
  
  std::vector<UITouch*>::const_iterator end = touches.end();
  for (std::vector<UITouch*>::const_iterator it = touches.begin(); it != end; ++it)
  {
    UITouch* pTouch = *it;
    [self dumpTouch: pTouch];
  }
}

- (void)Paint
{
  [self InitNGLWindow];
  if (mInvalidated)
  {
    mInvalidated = false;
    mpNGLWindow->CallOnPaint();
  }
}


- (void) handleEvent: (UIEvent*) pEvent
{
	static double sOldTimestamp = 0.0;
  NSSet* pSet = [pEvent allTouches];
  NGL_ASSERT(pSet);
  NSArray* pArray = [pSet allObjects];
  NGL_ASSERT(pArray);
  NSUInteger count = [pArray count];

  std::vector<UITouch*> touches;
  touches.resize((uint)count);
  [pArray getObjects: &touches[0]];

  const nuiRect rect(0,0, mpNGLWindow->GetWidth(), mpNGLWindow->GetHeight());

  for (uint n = 0; n < count; n++)
  {
    UITouch* pTouch = touches[n];

//    UITouchInfo touchInfo = [pTouch info];
    UITouchPhase touchPhase = [pTouch phase];
    uint touchTapCount = [pTouch tapCount];

    CGPoint newp = [pTouch locationInView: (UIView*)self];
    CGPoint oldp = [pTouch previousLocationInView: (UIView*)self];

    int x = (int)newp.x;
    int y = (int)newp.y;
    
    TouchesInfo::iterator it = mTouches.find(pTouch);

    if (it != mTouches.end())
    {
///< this touch exists
      nglTouchInfo& rTouch(it->second);
      NGL_ASSERT(touchPhase != UITouchPhaseBegan);
      if (touchPhase == UITouchPhaseEnded || touchPhase == UITouchPhaseCancelled)
      {
///< this touch has been released
        NGL_TOUCHES_OUT(_T("[%p][%d] [available: %d] Release X:%d Y:%d, phase: %s\n"), pTouch, rTouch.mTouchId, gAvailableTouches.size(), x, y, touchPhase == UITouchPhaseEnded ? "UITouchPhaseEnded" : touchPhase == UITouchPhaseCancelled ? "UITouchPhaseEnded" : "SHALL NOT");
        
        nglMouseInfo info;
        info.Buttons = nglMouseInfo::ButtonLeft;
        info.X = x;
        info.Y = y;

        AdjustFromAngle(mpNGLWindow->GetRotation(), rect, info);

        info.SwipeInfo = nglMouseInfo::eNoSwipe;
        info.mTouchId = rTouch.mTouchId;
        mpNGLWindow->CallOnMouseUnclick(info);

        gAvailableTouches.push_back(rTouch.mTouchId);
        gPressedTouches[rTouch.mTouchId] = false;
        
        mTouches.erase(it);
        
      }
      else if (rTouch.X != x || rTouch.Y != y)
      {
///< this touch has moved
        NGL_TOUCHES_OUT(_T("[%p][%d] Move X:%d Y:%d\n"), pTouch, rTouch.mTouchId, x, y);
        NGL_ASSERT(touchPhase == UITouchPhaseMoved);
        rTouch.X = x;
        rTouch.Y = y;

        nglMouseInfo info;
        info.Buttons = nglMouseInfo::ButtonLeft;
        info.X = x;
        info.Y = y;

        AdjustFromAngle(mpNGLWindow->GetRotation(), rect, info);

        info.SwipeInfo = nglMouseInfo::eNoSwipe;
        info.mTouchId = rTouch.mTouchId;

        mpNGLWindow->CallOnMouseMove(info);
      }
      else {
        NGL_ASSERT(touchPhase == UITouchPhaseStationary);
      }
    }
    else if (touchPhase == UITouchPhaseBegan && (gAvailableTouches.size() > 0))
    {
///< this is a new touch
      NGL_TOUCHES_OUT(_T("[%p][available: %d] UITouchPhaseBegan X:%d Y:%d\n"), pTouch, gAvailableTouches.size(), x, y);

      NGL_ASSERT(touchPhase == UITouchPhaseBegan);

      nglTouchInfo newTouch;
      newTouch.X = x;
      newTouch.Y = y;
      NGL_ASSERT(gAvailableTouches.size() > 0);
      newTouch.mTouchId = gAvailableTouches.front();

      gAvailableTouches.pop_front();
      gPressedTouches[newTouch.mTouchId] = true;

      NGL_TOUCHES_OUT(_T("[%p][%d] New X:%d Y:%d\n"), pTouch, newTouch.mTouchId, x, y);
      mTouches[pTouch] = newTouch;

      nglMouseInfo info;
      info.Buttons = nglMouseInfo::ButtonLeft;

			// Double tapping events are way too slow on the iPhone
			// compare timestamps to see if we should declare the event a double click
#define DOUBLE_TAP_DELAY	0.25

///< if tapcount > 1, it is a double click
      if (touchTapCount > 1)// && ([pTouch timestamp] - sOldTimestamp < DOUBLE_TAP_DELAY))
        info.Buttons |= nglMouseInfo::ButtonDoubleClick;

			sOldTimestamp = [pTouch timestamp];

      info.X = x;
      info.Y = y;

      AdjustFromAngle(mpNGLWindow->GetRotation(), rect, info);

      info.SwipeInfo = nglMouseInfo::eNoSwipe;
      info.mTouchId = newTouch.mTouchId;

      mpNGLWindow->CallOnMouseClick(info);
    }
    else {
      NGL_TOUCHES_OUT(_T("[%p][available: %d] Discarding event: [UITouchPhaseBegan X:%d Y:%d]\n"), pTouch, gAvailableTouches.size(), x, y);
    }
  }

/*
** Debug prints:
*/
/*
  NGL_OUT(_T("Available Touches count: %d\nIds: "), gAvailableTouches.size());
  for (std::list<nglTouchId>::const_iterator it = gAvailableTouches.begin(); it != gAvailableTouches.end(); ++it) {
    NGL_OUT(_T("[%d]"), *it);
  }
  NGL_OUT(_T("\n"));
*/
}

- (void) invalidate
{
  mInvalidated = true;
}

@end///< nglUIWindow


/*
 * OS specific info
 */

nglWindow::OSInfo::OSInfo()
{
  mpUIWindow = NULL;
}

/*
 * Constructors
 */

nglWindow::nglWindow (uint Width, uint Height, bool IsFullScreen)
{
  mpUIWindow=NULL;
  nglContextInfo context; // Get default context
  nglWindowInfo info(Width, Height, IsFullScreen);
  InternalInit (context, info, NULL); 
}

nglWindow::nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  mpUIWindow=NULL;
  InternalInit (rContext, rInfo, pShared);
}

void nglWindow::InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  SetError (NGL_WINDOW_ENONE);
  SetEventMask(nglWindow::AllEvents);

  for (nglTouchId t = 0; t < _NUI_MAX_TOUCHES_; t++)
  {
    gPressedTouches[t] = false;
    gAvailableTouches.push_back(t);
  }

  mInSession  = 0;
  mWidth      = rInfo.Width;
  mHeight     = rInfo.Height;
  mAngle      = rInfo.Rotate;

  CGRect rect = [[UIScreen mainScreen] bounds];

  if (mWidth == 0 || mHeight == 0)
  {
    if (mAngle == 90 || mAngle == 270) { ///< invert screen sizes
      mWidth = (uint)rect.size.height;
      mHeight = (uint)rect.size.width;
    }
    else
    {
      mWidth = (uint)rect.size.width;
      mHeight = (uint)rect.size.height;
    }
  }

///< Create the actual window
  nglUIWindow* pUIWindow = [[nglUIWindow alloc] initWithFrame: rect andNGLWindow: this];

  mOSInfo.mpUIWindow = pUIWindow;
  mpUIWindow = pUIWindow;

// Enable multitouch
  [pUIWindow setMultipleTouchEnabled: YES];
  
  [pUIWindow makeKeyAndVisible];
  
  NGL_LOG(_T("window"), NGL_LOG_INFO, _T("trying to create GLES context"));
  rContext.Dump(NGL_LOG_INFO);
  
  if (rContext.TargetAPI != eTargetAPI_OpenGLES)
  {
///< UIKit Implementation only supports OpenGLES renderer so far
    NGL_LOG(_T("window"), NGL_LOG_INFO, _T("bad renderer"));
    NGL_ASSERT(0);
    return;
  }
  if (!Build(rContext, pShared, rInfo.Flags & nglWindow::FullScreen))
  {
///< An error has been raised by nglContext's code
    NGL_LOG(_T("window"), NGL_LOG_INFO, _T("could not create its context"));
    NGL_ASSERT(0);
    return;
  }


///< Rendering takes place in a Core Animation Layer
  CAEAGLLayer* pLayer = (CAEAGLLayer*)[pUIWindow layer];
  NGL_ASSERT(pLayer);  
	//pLayer.opaque = YES;
  BOOL retainBacking = rContext.CopyOnSwap ? YES : NO;
	[pLayer setDrawableProperties:
   [NSDictionary dictionaryWithObjectsAndKeys:
    [NSNumber numberWithBool:retainBacking],
    kEAGLDrawablePropertyRetainedBacking,
    (NSString*)mpEAGLPixelFormat,
    kEAGLDrawablePropertyColorFormat,
    nil
   ]
  ];
//mpEAGLLayer = pLayer;

  rect = [(nglUIWindow*)mpUIWindow frame];
  CGSize newSize;
	newSize = [pLayer bounds].size;
	newSize.width = roundf(newSize.width);
	newSize.height = roundf(newSize.height);
  
//	mWidth = newSize.width;
//  mHeight = newSize.height;


///< This layer is then used as the color attachement for a framebuffer based rendering
	GLuint oldRenderbuffer;
	GLuint oldFramebuffer;
	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	
	glGenRenderbuffersOES(1, &mRenderBuffer);
	glGenFramebuffersOES(1, &mFrameBuffer);
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderBuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFrameBuffer);

	[(EAGLContext*)mpContext renderbufferStorage: GL_RENDERBUFFER_OES
                                  fromDrawable: pLayer];

	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                               GL_COLOR_ATTACHMENT0_OES,
                               GL_RENDERBUFFER_OES,
                               mRenderBuffer);
  
	if (mDepthFormat)
  {
		glGenRenderbuffersOES(1, &mDepthBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, mDepthBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES,
                             mDepthFormat,
                             mWidth,
                             mHeight);

		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                 GL_DEPTH_ATTACHMENT_OES,
                                 GL_RENDERBUFFER_OES,
                                 mDepthBuffer);
	}
	
	NGL_ASSERT (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) == GL_FRAMEBUFFER_COMPLETE_OES);
  //  glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
  //	glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldRenderbuffer);

  
}

nglWindow::~nglWindow()
{
  //  App->DelWindow (this);
  //  App->DelEvent (this);
  
//  if (mTimer)
//    RemoveEventLoopTimer(mTimer);
  
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
      break;
    case eShow:
      break;
    case eMinimize:
      break;
		case eMaximize:
      break;
  };
}

nglWindow::StateInfo nglWindow::GetState() const
{
  return eVisible;
}

void nglWindow::GetSize (uint& rWidth, uint& rHeight) const
{
  rWidth  = mWidth;
  rHeight = mHeight;
}

uint nglWindow::GetWidth () const
{
	return mWidth;
}

uint nglWindow::GetHeight () const
{
  return mHeight;
}

bool nglWindow::SetSize (uint Width, uint Height)
{
  return false;
}

void nglWindow::GetPosition (int& rXPos, int& rYPos) const
{
  // FIXME
}

bool nglWindow::SetPosition (int XPos, int YPos)
{
  // FIXME
  return false;
}

nglString nglWindow::GetTitle() const
{
  return _T("UIKit Window\n");
}

void nglWindow::SetTitle (const nglString& rTitle)
{
}

bool nglWindow::IsKeyDown (nglKeyCode Key) const
{
  return false;
}

void nglWindow::GetMouse (nglMouseInfo& rMouseInfo, bool Local) const
{
}

const nglWindow::OSInfo* nglWindow::GetOSInfo() const
{
  return &mOSInfo;
}

void nglWindow::BeginSession()
{
#ifdef _DEBUG_WINDOW_
  NGL_LOG(_T("window"), NGL_LOG_INFO, _T("BeginSession\n"));
#endif
  MakeCurrent();
  mInSession++;
}

void nglWindow::EndSession()
{
#ifndef __NOGLCONTEXT__

#ifdef _DEBUG_WINDOW_
  NGL_LOG(_T("window"), NGL_LOG_INFO, _T("EndSession\n"));
#endif

  if (MakeCurrent())
  {
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderBuffer);	

    if ( ![(EAGLContext*)mpContext presentRenderbuffer: GL_RENDERBUFFER_OES] )
    {
      printf("Failed to swap renderbuffer in %s\n", __FUNCTION__);
    }
    mInSession=0;
  }

#endif
}

bool nglWindow::MakeCurrent() const
{
  EAGLContext* pContext = [EAGLContext currentContext];	
	if (pContext != mpContext)
    return InternalMakeCurrent(mpContext);
  return true;
}

void nglWindow::Invalidate()
{
///< FIXME? Direct Drawing Invalidation directly Calls OnPaint for now ...
//  CallOnPaint();
  [(nglUIWindow*)mpUIWindow invalidate];
}

bool nglWindow::SetCursor(nuiMouseCursor Cursor)
{
///< No Cursor relevance here
  return true;
}

nuiMouseCursor nglWindow::GetCursor() const
{
///< No Cursor relevance here
  return eCursorNone;
}

nglWindow::EventMask nglWindow::GetEventMask() const
{
  return mEventMask;
}

void nglWindow::SetEventMask(EventMask Events)
{
  mEventMask = Events;
}

bool nglWindow::GetResolution(float& rHorizontal, float& rVertical)
{
  rHorizontal = rVertical = 72.f;
  return false;
}


void nglWindow::EnterModalState()
{
  SetState(eShow);
  NGL_ASSERT(!"Not Implemented");
}

void nglWindow::ExitModalState()
{
  NGL_ASSERT(!"Not Implemented");  
}

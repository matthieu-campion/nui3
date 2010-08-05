#include "nui.h"

#include "nglApplication.h"
#include "nglVideoMode.h"
#include "nglContext.h"
#include "nglWindow.h"
#include "nglKeyboard.h"
#include "nuiMouseCursor.h"
#include "nuiStopWatch.h"

#include <QuartzCore/QuartzCore.h>


#include <string.h>

#include "nglWindow_Cocoa.h"

#include <Cocoa/Cocoa.h>
#include <OpenGL/OpenGL.h>
#include <AppKit/AppKit.h>

#define NGL_WINDOW_FPS 60.0f

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

@interface customGLView : NSView
{
	NSOpenGLContext *oglContext;
}

- (NSOpenGLContext*) getContext;

@end
// ---------


@implementation customGLView

- (NSOpenGLContext*) getContext
{
  return oglContext;
}

- (NSSize) windowWillResize: (NSWindow*) win toSize: (NSSize) size
{
  printf("windowWillResize %f x %f\n", size.width, size.height);
  // inform the context that the view has been resized
  NSRect rect = {0};
  rect.size = size;
  [win resize: [win contentRectForFrameRect: rect].size];
  return size;
}

-(void)windowWillClose:(NSNotification *)note
{
  [[NSApplication sharedApplication] terminate:self];
}

/* default initializer for descendents of NSView */
- (id)initWithFrame:(NSRect)frame {
  
  self = [super initWithFrame:frame];
  if(self == nil)
    return nil;

  // create and activate the context object which maintains the OpenGL state
  NSOpenGLPixelFormatAttribute attribs[] =
  {
    NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)24,
    NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFABackingStore,
    //NSOpenGLPFAWindow,
    (NSOpenGLPixelFormatAttribute)0
  };
  NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
  
  oglContext = [[NSOpenGLContext alloc] initWithFormat: format shareContext: nil];
  GLint v = 1;
  [oglContext setValues:&v forParameter:NSOpenGLCPSwapInterval];
  [oglContext setView:self];
  [oglContext makeCurrentContext];
  return self;
}

- (void)lockFocus
{
  NSOpenGLContext* context = oglContext;
  
  // make sure we are ready to draw
  [super lockFocus];
  
  // when we are about to draw, make sure we are linked to the view
  // It is not possible to call setView: earlier (will yield 'invalid drawable')
  if ([context view] != self)
  {
    [context setView:self];
  }
  
  // make us the current OpenGL context
  [context makeCurrentContext];
}

// this is called whenever the view changes (is unhidden or resized)
- (void)drawRect:(NSRect)frameRect
{
  [oglContext update];
  [[self window] doPaint];
}

// this tells the window manager that nothing behind our view is visible
-(BOOL) isOpaque
{
  return YES;
}

-(void) dealloc
{
  [super dealloc];
}

@end


@implementation nglNSWindow

- (nglWindow *) getNGLWindow
{
	return self->mpNGLWindow;
}

//+ (Class) layerClass
//{
//	return [CANSGLLayer class];
//}


- (id) initWithFrame: (NSRect) rect andNGLWindow: (nglWindow*) pNGLWindow
{
	mInited = false;
	mInvalidated = true;
	
	mInvalidationTimer = nil;
	mDisplayLink = nil;
	
  BOOL deffering = NO;
  uint32 styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
  NSBackingStoreType buffering = NSBackingStoreBuffered;
    
  if ( (self = [self initWithContentRect:rect styleMask:styleMask backing:buffering defer:deffering]) )
  {
    mpNGLWindow = pNGLWindow;
  }
  else
  {
    NGL_ASSERT(!"initWithFrame: Could not initialize NSWindow");
  }

  [self setTitle:@"Testing Simple Cocoa Application"];
  
  NSRect glrect = {0};
  glrect.size.width = 320;
  glrect.size.height = 240;
  
  customGLView* pView = [[[customGLView alloc] initWithFrame: glrect] autorelease];
  [self setContentView: pView];
  [self setDelegate: pView];
  
  
  //[self makeKeyAndOrderFront:nil];
  
//NGL_OUT(_T("[nglNSWindow initWithFrame]\n"));


  int frameInterval = 1;
//  NSString* sysVersion = [[UIDevice currentDevice] systemVersion];
//  if ([sysVersion compare:@"3.1" options:NSNumericSearch] != NSOrderedAscending) ///< CADisplayLink requires version 3.1 or greater
//  {
//    mDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(Paint)];
//    [mDisplayLink setFrameInterval:frameInterval];
//    [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
//  }
//  else ///< NSTimer is used as fallback
  {
    mInvalidationTimer = [NSTimer scheduledTimerWithTimeInterval:(1.0f / NGL_WINDOW_FPS) target:self selector:@selector(Paint) userInfo:nil repeats:YES];
  }
  
  mpTimer = nuiAnimation::AcquireTimer();
  mpTimer->Stop();

  return self;
}

- (void) dealloc
{
  //NGL_OUT(_T("[nglNSWindow dealloc]\n"));
  mpNGLWindow->CallOnDestruction();

  nuiAnimation::ReleaseTimer();
  if (mInvalidationTimer != nil)
  {
    [mInvalidationTimer invalidate];
    //    [mInvalidationTimer release]; ///< should be auto released
    mInvalidationTimer = nil;
  }
  if (mDisplayLink != nil)
  {
    [mDisplayLink invalidate];
    //    [mDisplayLink release]; ///< should be auto released
    mDisplayLink = nil;
  }
  
  [super dealloc];
}


- (void) InitNGLWindow
{  
  if (!mInited)
  {
    mInited = true;
    mpNGLWindow->CallOnCreation();
//    mpNGLWindow->Invalidate();
//    mpNGLWindow->CallOnPaint();
  }

  static int32 counter = 0;
  if (counter)
  {
    counter--;
    return;
  }
  
  counter = 60;
}

- (void)mouseDown:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = 1 << [theEvent buttonNumber];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseClick(info);
}

- (void)mouseUp:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = 1 << [theEvent buttonNumber];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseUnclick(info);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = (uint64)[NSEvent pressedMouseButtons];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseMove(info);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = (uint64)[NSEvent pressedMouseButtons];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseMove(info);
}

////
- (void)rightMouseDown:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = 1 << [theEvent buttonNumber];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseClick(info);
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = 1 << [theEvent buttonNumber];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseUnclick(info);
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = (uint64)[NSEvent pressedMouseButtons];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseMove(info);
}

- (void)scrollWheel:(NSEvent *)theEvent
{
  float x = [theEvent deltaX];
  float y = [theEvent deltaY];
  
  nglMouseInfo info;
  info.Buttons = 0;
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;

  if (x > 0)
  {
    info.Buttons = nglMouseInfo::ButtonWheelRight;
    mpNGLWindow->CallOnMouseClick(info);
    mpNGLWindow->CallOnMouseUnclick(info);
  }
  else if (x < 0)
  {
    info.Buttons = nglMouseInfo::ButtonWheelLeft;
    mpNGLWindow->CallOnMouseClick(info);
    mpNGLWindow->CallOnMouseUnclick(info);
  }

  if (y > 0)
  {
    info.Buttons = nglMouseInfo::ButtonWheelUp;
    mpNGLWindow->CallOnMouseClick(info);
    mpNGLWindow->CallOnMouseUnclick(info);
  }
  else if (y < 0)
  {
    info.Buttons = nglMouseInfo::ButtonWheelDown;
    mpNGLWindow->CallOnMouseClick(info);
    mpNGLWindow->CallOnMouseUnclick(info);
  }
}
//////

- (void)otherMouseDown:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = 1 << [theEvent buttonNumber];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseClick(info);
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = 1 << [theEvent buttonNumber];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseUnclick(info);
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
  nglMouseInfo info;
  info.Buttons = (uint64)[NSEvent pressedMouseButtons];
  info.TouchId = 0;
  NSPoint p = [self mouseLocationOutsideOfEventStream];
  info.X = p.x;
  info.Y = [self frame].size.height - p.y;
  mpNGLWindow->CallOnMouseMove(info);
}

//////////
- (void)resize: (NSSize) size
{
  //printf("resize %f x %f\n", size.width, size.height);
  mpNGLWindow->CallOnResize(size.width, size.height);
  mInvalidated = true;
}

- (BOOL)acceptsMouseMovedEvents
{
  return YES;
}

- (void)Paint
{
  [self InitNGLWindow];

  mpTimer->Tick();
  if (mInvalidated)
  {
    mInvalidated = false;
    [self display];
    //mpNGLWindow->CallOnPaint();
  }
}

- (void)doPaint
{
  printf("doPaint 0x%x\n", mpNGLWindow);
  
  mpNGLWindow->CallOnPaint();
}

- (void) invalidate
{
  printf("invalidate\n");
  mInvalidated = true;
}


- (void) MakeCurrent
{
  //printf("MakeCurrent\n");
  [[[self contentView] getContext] makeCurrentContext];
}

- (void) BeginSession
{
  //printf("BeginSession\n");
  [[[self contentView] getContext] makeCurrentContext];
}

- (void) EndSession
{
  //printf("EndSession\n");
  glFlush();
  //[[[self contentView] getContext] flushBuffer];
}


@end///< nglNSWindow




/////////////////////////////////////////////////////////////////////////////////////////////////// NGL WINDOW:



/*
 * OS specific info
 */

nglWindow::OSInfo::OSInfo()
{
  mpNSWindow = NULL;
}

/*
 * Constructors
 */

nglWindow::nglWindow (uint Width, uint Height, bool IsFullScreen)
{
  mpNSWindow = NULL;
  nglContextInfo context; // Get default context
  nglWindowInfo info(Width, Height, IsFullScreen);
  InternalInit (context, info, NULL); 
}

nglWindow::nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  mpNSWindow = NULL;
  InternalInit (rContext, rInfo, pShared);
}

void nglWindow::InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  mState = eHide;
  mAngle = 0;

  SetError (NGL_WINDOW_ENONE);
  SetEventMask(nglWindow::AllEvents);

  mAutoRotate = true;
  
  mAngle = rInfo.Rotate;
  NSRect rect = [[NSScreen mainScreen] visibleFrame];
  mWidth = 0;
  mHeight = 0;
  rect.origin.x = rInfo.XPos;
  rect.origin.y = rect.size.height - rInfo.YPos;
  rect.size.width = rInfo.Width;
  rect.size.height = rInfo.Height;

  switch (rInfo.Pos)
  {
    case nglWindowInfo::ePosUser:
      break;
    case nglWindowInfo::ePosCenter:
      rect.origin.x = (rect.origin.x - rect.size.width) / 2;
      rect.origin.y = (rect.origin.y - rect.size.height) / 2;
      break;
    case nglWindowInfo::ePosMouse:
      break;
    case nglWindowInfo::ePosAuto:
      break;
  }
  

  // Create the actual window
  nglNSWindow* pNSWindow = [[nglNSWindow alloc] initWithFrame:rect andNGLWindow: this];

  mOSInfo.mpNSWindow = pNSWindow;
  mpNSWindow = pNSWindow;


  //[pNSWindow makeKeyAndVisible];
  
  NGL_LOG(_T("window"), NGL_LOG_INFO, _T("trying to create GLES context"));
  rContext.Dump(NGL_LOG_INFO);
  
  if (rContext.TargetAPI != eTargetAPI_OpenGL)
  {
    // UIKit Implementation only supports OpenGLES renderer so far
    NGL_LOG(_T("window"), NGL_LOG_INFO, _T("bad renderer"));
    NGL_ASSERT(0);
    return;
  }
	
	NSRect r = [(nglNSWindow*)mpNSWindow frame];
	printf("currentFrame: %f, %f - %f, %f\n", r.origin.x, r.origin.y, r.size.width, r.size.height);
	r = [NSScreen mainScreen].visibleFrame;
	printf("applicationFrame: %f, %f - %f, %f\n", r.origin.x, r.origin.y, r.size.width, r.size.height);
	
	SetSize(rect.size.width, rect.size.height);
	
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  
//  [pNSWindow UpdateOrientation];

/* Ultra basic UIKit view integration on top of nuiWidgets
  UIWebView* pWebView = [[UIWebView alloc] initWithFrame: CGRectMake(50, 50, 200, 200)];
	[pNSWindow addSubview: pWebView];
	pWebView.hidden = NO;
  NSURL* pURL = [[NSURL alloc] initWithString: @"http://libnui.net"];
  NSURLRequest* pReq = [[NSURLRequest alloc] initWithURL: pURL];
  [pWebView loadRequest: pReq];
 */
}

nglWindow::~nglWindow()
{
}


/*
 * All services
 */

void nglWindow::SetState (StateChange State)
{
  if (mState == State)
    return;

	switch (State)
	{
		case eHide:
      [mpNSWindow hide:nil];
			break;
		case eShow:
      [mpNSWindow makeKeyAndOrderFront:nil];
			break;
		case eMinimize:
      [mpNSWindow makeKeyAndOrderFront:nil];
			break;
		case eMaximize:
      [mpNSWindow makeKeyAndOrderFront:nil];
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
  NGL_OUT(_T("nglWindow::SetSize(%d, %d)\n"), Width, Height);
  mWidth  = Width;
  mHeight = Height;
  CallOnResize(Width, Height);
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
  NSString* pR = [mpNSWindow title];
  nglString r((CFStringRef)pR);
  [pR release];
  return r;
}

void nglWindow::SetTitle (const nglString& rTitle)
{
  [mpNSWindow setTitle: (NSString*)rTitle.ToCFString()];
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
  //  MakeCurrent();
  NGL_ASSERT(mpNSWindow);
  [mpNSWindow BeginSession];
}

void nglWindow::EndSession()
{
#ifndef __NOGLCONTEXT__

#ifdef _DEBUG_WINDOW_
  NGL_LOG(_T("window"), NGL_LOG_INFO, _T("EndSession\n"));
#endif
	
  NGL_ASSERT(mpNSWindow);
  [mpNSWindow EndSession];
#endif
}

bool nglWindow::MakeCurrent() const
{
  NGL_ASSERT(mpNSWindow);
  [mpNSWindow MakeCurrent];

  return true;
}

void nglWindow::Invalidate()
{
  printf("nglWindow::Invalidate()\n");
  [(nglNSWindow*)mpNSWindow invalidate];
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

bool nglWindow::GetResolution(float& rHorizontal, float& rVertical) const
{
  rHorizontal = rVertical = 72.f;
  return false;
}


void nglWindow::EnterModalState()
{
  SetState(eShow);
  NGL_OUT(_T("nglWindow::EnterModalState NOT IMPLENTED YET!"));
//  NGL_ASSERT(!"Not Implemented");
}

void nglWindow::ExitModalState()
{
  NGL_OUT(_T("nglWindow::ExitModalState NOT IMPLENTED YET!"));
//  NGL_ASSERT(!"Not Implemented");  
}

void nglWindow::StartTextInput(int32 X, int32 Y, int32 W, int32 H)
{
  [(nglNSWindow*)mpNSWindow showKeyboard];
}

void nglWindow::EndTextInput()
{
  [(nglNSWindow*)mpNSWindow hideKeyboard];
}

bool nglWindow::IsEnteringText() const
{
  //[(nglNSWindow*)mpNSWindow frame]
  return false;
}

/// Drag and Drop:
bool nglWindow::Drag(nglDragAndDrop* pDragObject)
{
  return false;
}

nglDropEffect nglWindow::OnCanDrop(nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button)
{
  return eDropEffectNone;
}

void nglWindow::OnDragEnter()
{
}

void nglWindow::OnDragLeave()
{
}

void nglWindow::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
}

void nglWindow::OnDragStop(bool canceled)
{
}

void nglWindow::OnDropped(nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button)
{
}


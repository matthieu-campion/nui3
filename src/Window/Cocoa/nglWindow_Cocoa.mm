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

#include <OpenGL/OpenGL.h>

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
    NSOpenGLPFAColorSize, 24,
    NSOpenGLPFADepthSize, 16,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFABackingStore,
    //NSOpenGLPFAWindow,
    0
  };
  NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
  
  oglContext = [[NSOpenGLContext alloc] initWithFormat: format shareContext: nil];
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
  
  float aspectRatio;
  
  // inform the context that the view has been resized
  [oglContext update];
  
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  
//  // adjust the viewport and frustum transforms
//  // (gluLookAt() doesn't work properly)
//  glViewport(0, 0, frameRect.size.width, frameRect.size.height);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  aspectRatio = (float) frameRect.size.width / (float) frameRect.size.height;
//  glFrustum(-aspectRatio, aspectRatio, -1.0, 1.0, 2.0, 100.0);
//  
//  // prepare for primitive drawing
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  glColor3f( 1.0, 0.5, 1.0 );
//  glEnableClientState( GL_VERTEX_ARRAY );
//  
//  // submit the vertex information for drawing
////  glVertexPointer( 3, GL_FLOAT, 0, verts );
////  
////  glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, tris );
//  glDisableClientState( GL_VERTEX_ARRAY );
//  
//  glFlush();
  
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
  NSUInteger styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
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


- (void)Paint
{
  [self InitNGLWindow];

  //if (mInvalidated)
  {
    mInvalidated = false;
    mpTimer->Tick();
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
  mInvalidated = true;
}


- (void) MakeCurrent
{
  printf("MakeCurrent\n");
  //[[[self contents] getContext] makeCurrent];
}

- (void) BeginSession
{
  printf("BeginSession\n");
  //[[[self contents] getContext] makeCurrent];
}

- (void) EndSession
{
  printf("EndSession\n");
  glFlush();
  //[[[self contents] getContext] flushBuffer];
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
  float w, h;
  mWidth = 0;
  mHeight = 0;
  if (mAngle == 270 || mAngle == 90)
  {
    w = rect.size.height;
    h = rect.size.width;
  }
  else
  {
    w = rect.size.width;
    h = rect.size.height;
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
	
	SetSize(w, h);
	
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


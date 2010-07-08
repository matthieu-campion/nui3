//
//  Application.m
//  RecreatingNSApplication
//
//  Created by Matt Gallagher on 17/01/09.
//  Copyright 2009 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

#import "Application.h"
#import "ApplicationDelegate.h"
#import <Cocoa/Cocoa.h>


#import <AppKit/AppKit.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
@interface NSApplication(NSAppleMenu)
- (void)setAppleMenu:(NSMenu *)menu;
@end
#endif

@interface customGLView : NSView
{
	NSOpenGLContext *oglContext;
	GLfloat *verts;
	GLubyte *tris;
}

@end
// ---------


@implementation customGLView

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
  
  // set the background colour
  glClearColor (0.0, 0.0, 1.0, 1.0);
  glClearDepth(1.0);
  
  // define a polygon primitive. first allocate some space for the data
  verts = calloc( 3*3, sizeof(GLfloat) );
  tris = calloc( 3, sizeof(GLubyte) );
  
  // initialize the values for the triangle's vertices and indices
  // this triangle is perpendicular to the default view vector
  if( verts != NULL && tris != NULL ) {
    verts[0] = 1.0f;
    verts[1] = -1.0f;
    verts[2] = -5.0f;
    verts[3] = -1.0f;
    verts[4] = -1.0f;
    verts[5] = -5.0f;
    verts[6] = 0.0f;
    verts[7] = 0.5f;
    verts[8] = -5.0f;
    
    tris[0] = 0;
    tris[1] = 2;
    tris[2] = 1;
  }
  else {
    // if we couldn't get the memory
    [self release];
    self = nil;
  }
  
  return self;
}

- (void)lockFocus
{
  NSOpenGLContext* context = oglContext;
  
  // make sure we are ready to draw
  [super lockFocus];
  
  // when we are about to draw, make sure we are linked to the view
  // It is not possible to call setView: earlier (will yield 'invalid drawable')
  if ([context view] != self) {
    [context setView:self];
  }
  
  // make us the current OpenGL context
  [context makeCurrentContext];
}

// this is called whenever the view changes (is unhidden or resized)
- (void)drawRect:(NSRect)frameRect {

  float aspectRatio;
  
  // inform the context that the view has been resized
  [oglContext update];
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // adjust the viewport and frustum transforms
  // (gluLookAt() doesn't work properly)
  glViewport(0, 0, frameRect.size.width, frameRect.size.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspectRatio = (float) frameRect.size.width / (float) frameRect.size.height;
  glFrustum(-aspectRatio, aspectRatio, -1.0, 1.0, 2.0, 100.0);
  
  // prepare for primitive drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f( 1.0, 0.5, 1.0 );
  glEnableClientState( GL_VERTEX_ARRAY );
  
  // submit the vertex information for drawing
  glVertexPointer( 3, GL_FLOAT, 0, verts );
  
  glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, tris );
  glDisableClientState( GL_VERTEX_ARRAY );
  
  glFlush();
}

// this tells the window manager that nothing behind our view is visible
-(BOOL) isOpaque {
  
  return YES;
}

-(void) dealloc {
  
  // clean ourselves up nicely by freeing our dynamic memory allocations
  if( verts )
    free( verts );
  if( tris )
    free( tris );
  
  [super dealloc];
}

@end

int ApplicationMain(int argc, const char **argv)
{
  NSAutoreleasePool *pool = [NSAutoreleasePool new];
  NSApplication *applicationObject = [Application sharedApplication];
    

  ApplicationDelegate* appDelegate = [[ApplicationDelegate alloc] init];
  [applicationObject setDelegate:appDelegate];
  
  NSRect rect = {0};
  rect.origin.x = 100;
  rect.origin.y = 600;
  rect.size.width = 320;
  rect.size.height = 240;
  
  BOOL deffering = NO;
  NSUInteger styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
  NSBackingStoreType buffering = NSBackingStoreBuffered;
  NSWindow* pWin = [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing:buffering defer:deffering];

  [pWin setTitle:@"Testing Simple Cocoa Application"];
  
  NSRect glrect = {0};
  glrect.size.width = 320;
  glrect.size.height = 240;

  customGLView* pView = [[[customGLView alloc] initWithFrame: glrect] autorelease];
  [pWin setContentView: pView];
  [pWin setDelegate: pView];


  [pWin makeKeyAndOrderFront:nil];
  
  [applicationObject run];
	
	[pool release];
	
	return 0;
}

@implementation Application


@end


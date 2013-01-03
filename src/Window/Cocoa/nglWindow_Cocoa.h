#pragma once

#include <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGL/OpenGL.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
** nglNSWindow
*/
@interface nglNSWindow : NSWindow
{
  nglWindow*    mpNGLWindow;
  nglTime       mLastEventTime;
  bool          mInited;
  bool          mInvalidated;
  id            mDisplayLink;
  NSTimer*      mInvalidationTimer;

  nuiTimer*     mpTimer;
  uint32        mModifiers;
  NSEvent*      mpLastMouseEvent;
}

+ (Class) layerClass;
- (id) initWithFrame: (NSRect) rect andNGLWindow: (nglWindow*) pNGLWindow;
- (id) initWithWindow: (nglNSWindow*)pNSWindow;
- (void) setContext: (void*) pContext renderBuffer: (GLint) buffer;
- (void) dealloc;
- (void) invalidate;
- (nglWindow *) getNGLWindow;

- (BOOL)keyDown:(NSEvent *)theEvent;
- (BOOL)keyUp:(NSEvent *)theEvent;
- (BOOL)flagsChanged:(NSEvent *)theEvent;

- (BOOL) startDragging: (nglDragAndDrop*) pDragged;
- (NSDragOperation) draggingEntered: (id <NSDraggingInfo>)sender;
- (NSDragOperation) draggingUpdated: (id <NSDraggingInfo>)sender;
- (void) draggingExited: (id <NSDraggingInfo>)sender;
//- (BOOL) prepareForDragOperation: (id <NSDraggingInfo>)sender;
- (BOOL) performDragOperation: (id <NSDraggingInfo>)sender;
//- (void) concludeDragOperation: (id <NSDraggingInfo>)sender;
- (void) draggingEnded: (id <NSDraggingInfo>)sender;
//- (BOOL) wantsPeriodicDraggingUpdates;


@end///< nglNSWindow


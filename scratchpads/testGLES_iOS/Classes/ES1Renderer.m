//
//  ES1Renderer.m
//  testGL
//
//  Created by Sébastien Métrot on 6/24/10.
//  Copyright MXP4 2010. All rights reserved.
//

#import "ES1Renderer.h"

@implementation ES1Renderer

static GLint texture = 0;

// Create an OpenGL ES 1.1 context
- (id)initWithShareGroup: (EAGLSharegroup*) sharegroup
{
  if ((self = [super init]))
  {
    if (sharegroup)
      context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1 sharegroup: sharegroup];
    else
      context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!context || ![EAGLContext setCurrentContext:context])
    {
      [self release];
      return nil;
    }
    
    // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
    glGenFramebuffersOES(1, &defaultFramebuffer);
    glGenRenderbuffersOES(1, &colorRenderbuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
  }
  
  return self;
}

- (EAGLContext*)context
{
  return context;
}

- (void)render
{
  // Replace the implementation of this method to do your own custom drawing
  
#define SIZE 100
  static const GLfloat squareVertices[] = {
    0,  0,
    SIZE,  0,
    0,   SIZE,
    SIZE,   SIZE,
  };
  
  static const GLubyte squareColors[] = {
    255, 255,   0, 255,
    0,   255, 255, 255,
    0,     0,   0,   0,
    255,   0, 255, 255,
  };
  
  static const GLfloat squareTex[] = {
    0, 0,
    1, 0,
    0, 1,
    1, 1
  };
  
  static float transY = 0.0f;
  
  // This application only creates a single context which is already set current at this point.
  // This call is redundant, but needed if dealing with multiple contexts.
  [EAGLContext setCurrentContext:context];
  
  // This application only creates a single default framebuffer which is already bound at this point.
  // This call is redundant, but needed if dealing with multiple framebuffers.
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
  glViewport(0, 0, backingWidth, backingHeight);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float w = backingWidth;
  float h = backingHeight;
  if (angle == 270 || angle == 90)
  {
    w = backingHeight;
    h = backingWidth;
  }

  glRotatef(angle, 0.f,0.f,1.f);
  glTranslatef(-1.0f, 1.0f, 0.0f);
  glScalef(2.0f/w, -2.0f/h, 1.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glVertexPointer(2, GL_FLOAT, 0, squareVertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
  glEnableClientState(GL_COLOR_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, squareTex);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_SRC_COLOR);
  glDisable(GL_TEXTURE_2D);
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glLoadIdentity();
  glTranslatef(w - SIZE, 0, 0.0f);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
  
  glLoadIdentity();
  glTranslatef(w - SIZE, h - SIZE, 0.0f);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
  
  glLoadIdentity();
  glTranslatef(0, h - SIZE, 0.0f);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

  int err = glGetError();
  if (!texture)
  {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    const unsigned char data[] =
    {
      0,0,0,255,          255,255,255,255,
      255,255,255,255,    0,0,0,255
    };
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    err = glGetError();

    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    err = glGetError();
    
    
    // the texture wraps over at the edges (repeat)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    err = glGetError();
  }
  else
  {
    glBindTexture(GL_TEXTURE_2D, texture);
    err = glGetError();
  }
  err = glGetError();

  glEnable(GL_TEXTURE_2D);
  //glDisableClientState(GL_COLOR_ARRAY);
  
  glLoadIdentity();
  glTranslatef((w - SIZE) / 2, (h - SIZE) / 2, 0.0f);
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  // This application only creates a single color renderbuffer which is already bound at this point.
  // This call is redundant, but needed if dealing with multiple renderbuffers.
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
  [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{	
  // Allocate color buffer backing based on the current layer size
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
  [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
  
  if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
  {
    NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    return NO;
  }
  
  printf("Resize frame buffer: %d x %d\n", backingWidth, backingHeight);
  
  // Angle:
  UIDevice* pUIDev = [UIDevice currentDevice];
  unsigned int orientation = pUIDev.orientation;
  
  switch (orientation)
  {
    case UIDeviceOrientationUnknown:
    case UIDeviceOrientationFaceUp:
    case UIDeviceOrientationFaceDown:
      angle = 0;
      break;
    case UIDeviceOrientationPortrait:
      angle = 0;
      break;
    case UIDeviceOrientationPortraitUpsideDown:
      angle = 180;
      break;
    case UIDeviceOrientationLandscapeLeft:
      angle = 270;
      break;
    case UIDeviceOrientationLandscapeRight:
      angle = 90;
      break;
    default:
      break;
  }
  
  return YES;
}

- (void)dealloc
{
  // Tear down GL
  if (defaultFramebuffer)
  {
    glDeleteFramebuffersOES(1, &defaultFramebuffer);
    defaultFramebuffer = 0;
  }
  
  if (colorRenderbuffer)
  {
    glDeleteRenderbuffersOES(1, &colorRenderbuffer);
    colorRenderbuffer = 0;
  }
  
  // Tear down context
  if ([EAGLContext currentContext] == context)
    [EAGLContext setCurrentContext:nil];
  
  [context release];
  context = nil;
  
  [super dealloc];
}

@end

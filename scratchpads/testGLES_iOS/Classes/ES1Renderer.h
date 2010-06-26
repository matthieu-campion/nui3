//
//  ES1Renderer.h
//  testGL
//
//  Created by Sébastien Métrot on 6/24/10.
//  Copyright MXP4 2010. All rights reserved.
//

#import "ESRenderer.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface ES1Renderer : NSObject <ESRenderer>
{
@private
  EAGLContext *context;
  
  // The pixel dimensions of the CAEAGLLayer
  GLint backingWidth;
  GLint backingHeight;
  
  int angle;
  
  // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
  GLuint defaultFramebuffer, colorRenderbuffer;
}

- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;

@end

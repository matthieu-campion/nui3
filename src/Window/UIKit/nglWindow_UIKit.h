#ifndef __nglWindow_UIKit_h__
#define __nglWindow_UIKit_h__

#include <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

/*
** Touches tracking ..
*/

class nglTouchInfo ///< Touch info
{
public:
  UITouch* mpUITouch;
  nglTouchId mTouchId;
  int X, Y;
};

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{    
@private
  
  EAGLContext *context;
  nglContextInfo* mpContextInfo;

  // The pixel dimensions of the CAEAGLLayer
  GLint backingWidth;
  GLint backingHeight;
  
  int angle;
  
  // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
  GLuint defaultFrameBuffer, colorRenderBuffer, depthRenderBuffer;
  
}
@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::map<UITouch*,nglTouchInfo> TouchesInfo;

/*
** nglUIWindow
*/
@interface nglUIWindow : UIWindow<UITextFieldDelegate>
{
  nglWindow*    mpNGLWindow;
  nglContextInfo* mpContextInfo;
  TouchesInfo   mTouches;
  TouchesInfo   mpTouches[_NUI_MAX_TOUCHES_];
  uint8         mTouchCount;
  nglTime       mLastEventTime;
  bool          mInited;
  bool          mInvalidated;
  id            mDisplayLink;
  NSTimer*      mInvalidationTimer;
	UITextField*  mpTextField;
	BOOL          mKeyboardVisible;

  nuiTimer*     mpTimer;

  EAGLView* glView;
  EAGLView* glViewOld;
  NSTimer* OrientationTimer;
  UIDeviceOrientation oldorientation;
  int mAngle;
}

+ (Class) layerClass;
- (id) initWithFrame: (CGRect) rect andNGLWindow: (nglWindow*) pNGLWindow;
- (id) initWithWindow: (nglUIWindow*)pUIWindow;
- (void) setContext: (void*) pContext renderBuffer: (GLint) buffer;
- (void) dealloc;
- (void) invalidate;
- (void) sendEvent: (UIEvent*) pEvent;
//- (void) touchesChangedWithEvent: (UIEvent*) pEvent;
- (void) dumpTouch: (UITouch*) pTouch;
- (void) dumpTouches: (UIEvent*) pEvent;
- (void) handleEvent: (UIEvent*) pEvent;
- (nglWindow *) getNGLWindow;
- (void) UpdateOrientation;
- (void) recreateWindow;
- (void) disconnect;

- (void)showKeyboard;
- (void)hideKeyboard;
- (void)initializeKeyboard;
@end///< nglUIWindow

#endif//__nglWindow_UIKit_h__

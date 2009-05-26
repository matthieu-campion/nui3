#ifndef __nglWindow_UIKit_h__
#define __nglWindow_UIKit_h__

#include <UIKit/UIKit.h>

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

typedef std::map<UITouch*,nglTouchInfo> TouchesInfo;

/*
** nglUIWindow
*/
@interface nglUIWindow : UIWindow<UITextFieldDelegate>
{
  nglWindow*    mpNGLWindow;
  TouchesInfo   mTouches;
  TouchesInfo   mpTouches[_NUI_MAX_TOUCHES_];
  uint8         mTouchCount;
  nglTime       mLastEventTime;
  bool          mInited;
  bool          mInvalidated;
  NSTimer*      mInvalidationTimer;
	UITextField*  mpTextField;
	BOOL          mKeyboardVisible;
}

+ (Class) layerClass;
- (id) initWithFrame: (CGRect) rect andNGLWindow: (nglWindow*) pNGLWindow;
- (void) dealloc;
- (void) invalidate;
- (void) sendEvent: (UIEvent*) pEvent;
//- (void) touchesChangedWithEvent: (UIEvent*) pEvent;
- (void) dumpTouch: (UITouch*) pTouch;
- (void) dumpTouches: (UIEvent*) pEvent;
- (void) handleEvent: (UIEvent*) pEvent;
- (nglWindow *) getNGLWindow;

- (void)showKeyboard;
- (void)hideKeyboard;
- (void)initializeKeyboard;
@property (readonly) BOOL mKeyboardVisible;
@end///< nglUIWindow

#endif//__nglWindow_UIKit_h__

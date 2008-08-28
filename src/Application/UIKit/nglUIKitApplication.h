#ifndef __nglUIKitApplication_h__
#define __nglUIKitApplication_h__

#include "ngl.h"
#import <UIKit/UIKit.h>

@class nglUIWindow;

@interface nglUIApplication : UIApplication
{
}
- (void) dealloc;
- (BOOL) openURL: (NSURL*) pUrl;
- (void) sendEvent: (UIEvent*) pEvent;

@end//nglUIApplication

@interface nglUIApplicationDelegate : NSObject
{
}
- (void) dealloc;
- (void) applicationDidFinishLaunching:       (UIApplication*) pUIApp;
- (void) applicationDidReceiveMemoryWarning:  (UIApplication*) pUIApp;
- (void) applicationSignificantTimeChange:    (UIApplication*) pUIApp;
- (void) applicationWillTerminate:            (UIApplication*) pUIApp;

@end//nglUIApplicationDelegate


#endif//__nglUIKitApplication_h__

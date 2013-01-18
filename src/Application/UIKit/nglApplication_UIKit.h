#ifndef __nglUIApplication_h__
#define __nglUIApplication_h__

#include "ngl.h"
#import <UIKit/UIKit.h>

@class nglUIWindow;

@interface nglUIApplication : UIApplication
{
}
- (void) dealloc;
- (BOOL) openURL: (NSURL*) pUrl;
- (void) sendEvent: (UIEvent*) pEvent;
<<<<<<< HEAD
=======
- (void) didReceiveMemoryWarning;

>>>>>>> 65d97d72a1c22c34b561b003ed5f25458b4bed43
@end//nglUIApplication

@interface nglUIApplicationDelegate : NSObject
{
}
- (void) dealloc;
- (void) applicationDidFinishLaunching:       (UIApplication*) pUIApp;
- (BOOL) application:(UIApplication *)pUIApplication didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (void) applicationDidBecomeActive:          (UIApplication*) pUIApp;
- (void) applicationDidEnterBackground:         (UIApplication*) pUIApp;
- (void) applicationDidReceiveMemoryWarning:  (UIApplication*) pUIApp;
- (void) applicationSignificantTimeChange:    (UIApplication*) pUIApp;
- (void) applicationWillTerminate:            (UIApplication*) pUIApp;


@end//nglUIApplicationDelegate


#endif//__nglUIApplication_h__

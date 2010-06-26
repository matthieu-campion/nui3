//
//  testGLAppDelegate.h
//  testGL
//
//  Created by Sébastien Métrot on 6/24/10.
//  Copyright MXP4 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface MainWindow : UIWindow
{
  EAGLView* glView;
  NSTimer* OrientationTimer;
  int oldorientation;
}

@end




@interface testGLAppDelegate : NSObject <UIApplicationDelegate>
{
  MainWindow *window;
}
@end




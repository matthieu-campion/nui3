//
//  testGLAppDelegate.m
//  testGL
//
//  Created by Sébastien Métrot on 6/24/10.
//  Copyright MXP4 2010. All rights reserved.
//

#import "testGLAppDelegate.h"
#import "EAGLView.h"

@implementation MainWindow

- (id) initWithFrame: (CGRect)frame
{
  UIDevice* pUIDev = [UIDevice currentDevice];
  oldorientation = pUIDev.orientation;

  self = [super initWithFrame:frame];
  glView = [[EAGLView alloc] initWithFrame:frame replacing: nil];
  [self addSubview:glView];
  
  [glView startAnimation];
  
  OrientationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 10.0)) target:self selector:@selector(UpdateOrientation) userInfo:nil repeats:TRUE];
  [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

  return self;
}


- (void)layoutSubviews
{
  CGRect rect = self.frame;
  rect.origin.x = 0;
  rect.origin.y = 0;
  glView.frame = rect;
  //[self drawView:nil];
}

- (void)dealloc
{
  [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
  [OrientationTimer dealloc];
  [super dealloc];
}


- (void) UpdateOrientation
{
  int angle = 0;
  CGRect rect = [[UIScreen mainScreen] applicationFrame];
  UIApplication* pApp = [UIApplication sharedApplication];
  UIDevice* pUIDev = [UIDevice currentDevice];
  unsigned int orientation = pUIDev.orientation;
  unsigned int apporientation = pApp.statusBarOrientation;
  
  if (oldorientation == orientation)
    return;
  
  int w = rect.size.width;
  int h = rect.size.height;
  switch (orientation)
  {
    case UIDeviceOrientationUnknown:
    case UIDeviceOrientationFaceUp:
    case UIDeviceOrientationFaceDown:
      return;
      break;
    case UIDeviceOrientationPortrait:
      angle = 0;
      pApp.statusBarOrientation = UIInterfaceOrientationPortrait;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
      //[self UpdateKeyboard];
      w = [UIScreen mainScreen].applicationFrame.size.width;
      h = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    case UIDeviceOrientationPortraitUpsideDown:
      angle = 180;
      pApp.statusBarOrientation = UIInterfaceOrientationPortraitUpsideDown;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortraitUpsideDown];
      //[self UpdateKeyboard];
      w = [UIScreen mainScreen].applicationFrame.size.width;
      h = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    case UIDeviceOrientationLandscapeLeft:
      angle = 270;
      pApp.statusBarOrientation = UIInterfaceOrientationLandscapeRight;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
      //[self UpdateKeyboard];
      h = [UIScreen mainScreen].applicationFrame.size.width;
      w = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    case UIDeviceOrientationLandscapeRight:
      angle = 90;
      pApp.statusBarOrientation = UIInterfaceOrientationLandscapeLeft;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft];
      //[self UpdateKeyboard];
      h = [UIScreen mainScreen].applicationFrame.size.width;
      w = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    default:
      break;
  }

  
  rect = [[UIScreen mainScreen] applicationFrame];
  if (angle == 270 || angle == 90)
  {
    rect.size.width = h;
    rect.size.height = w;
  }
  else
  {
    rect.size.width = w;
    rect.size.height = h;
  }
  
  if (oldorientation != orientation)
  {
    EAGLView* old = glView;
    self.frame = rect;
    
    glView = [[EAGLView alloc] initWithFrame:rect replacing: old];
    [self addSubview:glView];
    [glView startAnimation];

    [old removeFromSuperview];
    [old dealloc];
  }
  oldorientation = orientation;
}


@end


@implementation testGLAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions   
{
  CGRect rect = [[UIScreen mainScreen] applicationFrame];

  int angle;
  UIApplication* pApp = [UIApplication sharedApplication];
  UIDevice* pUIDev = [UIDevice currentDevice];
  unsigned int orientation = pUIDev.orientation;
  unsigned int apporientation = pApp.statusBarOrientation;
  int w = rect.size.width;
  int h = rect.size.height;
  switch (orientation)
  {
    case UIDeviceOrientationUnknown:
    case UIDeviceOrientationFaceUp:
    case UIDeviceOrientationFaceDown:
      break;
    case UIDeviceOrientationPortrait:
      angle = 0;
      pApp.statusBarOrientation = UIInterfaceOrientationPortrait;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
      //[self UpdateKeyboard];
      w = [UIScreen mainScreen].applicationFrame.size.width;
      h = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    case UIDeviceOrientationPortraitUpsideDown:
      angle = 180;
      pApp.statusBarOrientation = UIInterfaceOrientationPortraitUpsideDown;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortraitUpsideDown];
      //[self UpdateKeyboard];
      w = [UIScreen mainScreen].applicationFrame.size.width;
      h = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    case UIDeviceOrientationLandscapeLeft:
      angle = 270;
      pApp.statusBarOrientation = UIInterfaceOrientationLandscapeRight;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
      //[self UpdateKeyboard];
      h = [UIScreen mainScreen].applicationFrame.size.width;
      w = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    case UIDeviceOrientationLandscapeRight:
      angle = 90;
      pApp.statusBarOrientation = UIInterfaceOrientationLandscapeLeft;
      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft];
      //[self UpdateKeyboard];
      h = [UIScreen mainScreen].applicationFrame.size.width;
      w = [UIScreen mainScreen].applicationFrame.size.height;
      break;
    default:
      break;
  }

  rect.size.width = w;
  rect.size.height = h;
  window = [[MainWindow alloc] initWithFrame:rect];
  //window = [[UIWindow alloc] initWithFrame:rect];
  //[window setMultipleTouchEnabled: YES];
  //EAGLView* glView = [[EAGLView alloc] initWithFrame:rect];
  //[window addSubview:glView];
  [window makeKeyAndVisible];
  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
//    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
//    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
//    [glView stopAnimation];
}

- (void)dealloc
{
    [window release];
 //   [glView release];

    [super dealloc];
}

@end

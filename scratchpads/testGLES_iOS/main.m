//
//  main.m
//  testGL
//
//  Created by Sébastien Métrot on 6/24/10.
//  Copyright MXP4 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[])
{
    
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

  int retVal = UIApplicationMain(argc, argv, nil, @"testGLAppDelegate");

  //int retVal = UIApplicationMain(argc, argv, nil, nil);
  [pool release];
  return retVal;
}

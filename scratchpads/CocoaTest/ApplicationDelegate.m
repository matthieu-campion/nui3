//
//  ApplicationDelegate.m
//  RecreatingNSApplication
//
//  Created by Sébastien Métrot on 7/6/10.
//  Copyright 2010 MXP4. All rights reserved.
//

#import "ApplicationDelegate.h"


@implementation ApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
  printf("ApplicationDelegate applicationDidFinishLaunching\n");
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
  printf("ApplicationDelegate applicationShouldTerminate\n");
  return NSTerminateNow;
}

- (void)applicationWillHide:(NSNotification *)notification
{
  printf("applicationWillHide\n");
}

- (void)applicationDidHide:(NSNotification *)notification
{
  printf("applicationDidHide\n");
}

- (void)applicationWillUnhide:(NSNotification *)notification
{
  printf("applicationWillUnhide\n");
}

- (void)applicationDidUnhide:(NSNotification *)notification
{
  printf("applicationDidUnhide\n");
}

- (void)applicationWillBecomeActive:(NSNotification *)notification
{
  printf("applicationWillBecomeActive\n");
}

- (void)applicationDidBecomeActive:(NSNotification *)notification
{
  printf("applicationDidBecomeActive\n");
}

- (void)applicationWillResignActive:(NSNotification *)notification
{
  printf("applicationWillResignActive\n");
}

- (void)applicationDidResignActive:(NSNotification *)notification
{
  printf("applicationDidResignActive\n");
}

- (void)applicationWillUpdate:(NSNotification *)notification
{
  printf("applicationWillUpdate\n");
}

- (void)applicationDidUpdate:(NSNotification *)notification
{
  printf("applicationDidUpdate\n");
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
  printf("applicationWillTerminate\n");
}

- (void)applicationDidChangeScreenParameters:(NSNotification *)notification
{
  printf("applicationDidChangeScreenParameters\n");
}

@end

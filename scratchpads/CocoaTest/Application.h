//
//  MyApplication.h
//  RecreatingNSApplication
//
//  Created by Matt Gallagher on 17/01/09.
//  Copyright 2009 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <AppKit/NSApplication.h>

int ApplicationMain(int argc, const char **argv);

@interface Application : NSApplication
{
	bool shouldKeepRunning;
}

- (void)run;
- (void)terminate:(id)sender;

@end


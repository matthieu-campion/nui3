//
//  ApplicationDelegate.h
//  RecreatingNSApplication
//
//  Created by Sébastien Métrot on 7/6/10.
//  Copyright 2010 MXP4. All rights reserved.
//

#import <Cocoa/Cocoa.h>


#if (MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5)
@interface ApplicationDelegate : NSObject
#else
@interface ApplicationDelegate : NSObject <NSApplicationDelegate>
#endif
{
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;

@end


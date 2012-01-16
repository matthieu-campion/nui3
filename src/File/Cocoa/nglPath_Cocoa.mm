/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nglPath_Cocoa.h"

#ifdef _UIKIT_
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

nglPath nuiCocoaGetPath_UserDocuments()
{
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString* docs = [paths objectAtIndex: 0];
  nglString str((CFStringRef)docs);
  return nglPath(str);  
}


nglPath nuiCocoaGetPath_Temp()
{
  NSString* docs = NSTemporaryDirectory();
  nglString str((CFStringRef)docs);
  return nglPath(str);  
}

nglPath nuiCocoaGetPath_App()
{
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString* docs = [paths objectAtIndex: 0];
  nglString str((CFStringRef)docs);
  return nglPath(str);  
}

nglPath nuiCocoaGetPath_AppSettings()
{
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString* docs = [paths objectAtIndex: 0];
  nglString str((CFStringRef)docs);
  return nglPath(str);  
}

nglPath nuiCocoaGetPath_UserPreferences()
{
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString* docs = [paths objectAtIndex: 0];
  nglString str((CFStringRef)docs);
  return nglPath(str);  
}

nglPath nuiCocoaGetPath_UserDesktop()
{
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, YES);
  NSString* docs = [paths objectAtIndex: 0];
  nglString str((CFStringRef)docs);
  return nglPath(str);  
}


nglPath nuiCocoaGetPath_LibraryCache()
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* docs = [paths objectAtIndex: 0];
    nglString str((CFStringRef)docs);
    return nglPath(str);  
}

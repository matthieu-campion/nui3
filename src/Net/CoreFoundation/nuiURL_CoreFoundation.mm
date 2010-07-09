/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiURL.h"

#ifndef _CARBON_
#ifdef _UIKIT_
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif
#endif

nuiURL::nuiURL(const nglString& rUrl)
{
  mUrl = rUrl;
}

nuiURL::~nuiURL()
{
  
}

bool nuiURL::IsNull()
{
  return (mUrl == nglString::Null);
}

bool nuiURL::OpenBrowser()
{

  if (mUrl == nglString::Null)
    return false;
  
  // Create a CFString and then a CFUrl
  std::string s = mUrl.GetStdString();
  CFStringRef str = CFStringCreateWithCString(NULL, s.c_str(), kCFStringEncodingASCII);
  if(!str)
  {
    NGL_ASSERT(0); 
    return false;
  }
  CFURLRef url = CFURLCreateWithString(NULL, str, NULL);
  NGL_ASSERT(url);
  
  // Launch the URL
#ifdef _UIKIT_
  //[NSWorkSpace openURL: url];
  [[UIApplication sharedApplication] openURL: [NSURL URLWithString: [NSString stringWithCString: s.c_str()]]];
#elif defined _CARBON_
  OSStatus err = LSOpenCFURLRef(url, NULL);  
  NGL_ASSERT(err == noErr);
#else
  [[NSApplication sharedApplication] openURL: [NSURL URLWithString: [NSString stringWithCString: s.c_str()]]];
#endif
  
  // Release the URL and string
  CFRelease(url);
  CFRelease(str);
  
  return true;
}

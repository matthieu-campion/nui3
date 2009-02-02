/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiURL.h"

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

#ifndef _UIKIT_
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
  OSStatus err = LSOpenCFURLRef(url, NULL);
  NGL_ASSERT(err == noErr);
  
  // Release the URL and string
  CFRelease(url);
  CFRelease(str);
  
  return true;
}
#endif

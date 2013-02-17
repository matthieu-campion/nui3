/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include <UIKit/UIKit.h>

bool nuiURL::OpenBrowser()
{
  NSString* str = [[NSString alloc] initWithCString:mUrl.GetStdString().c_str()];
  NSURL *url = [[NSURL alloc] initWithString:str];
  UIApplication* pApp = [UIApplication sharedApplication];
  [pApp openURL:url];
  [str release];
  return true;
}


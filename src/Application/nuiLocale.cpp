/*
 *  nuiLocale.cpp
 *  nui3
 *
 *  Created by Sebastien Metrot on 11/7/08.
 *  Copyright 2008 MXP4. All rights reserved.
 *
 */

#include "nui.h"
#include "nuiLocale.h"

nglString nuiLocale::GetCurrentCountry()
{
#ifdef __APPLE__
  CFLocaleRef loc = CFLocaleCopyCurrent();
  CFStringRef str = (CFStringRef)CFLocaleGetValue(loc, kCFLocaleCountryCode);
  
  nglString s(str);
  
  CFRelease(str);
  CFRelease(loc);
  
  return s;
#else
  nglString str(setlocale(LC_ALL, NULL));
  int32 i = str.Find(_T('_'));
  if (i < 0)
    return nglString::Null;
  
  uint32 j = str.Find(_T('.'));
  if (j < 0)
    j = str.GetLength();
  
  return str.Extract(i, j - i);
#endif
}


nglString nuiLocale::GetCurrentLanguage()
{
#ifdef __APPLE__
  CFLocaleRef loc = CFLocaleCopyCurrent();
  CFStringRef str = (CFStringRef)CFLocaleGetValue(loc, kCFLocaleLanguageCode);
  
  nglString s(str);
  
  CFRelease(str);
  CFRelease(loc);
  
  return s;
#else
  nglString str(setlocale(LC_ALL, NULL));
  int32 i = str.Find(_T('_'));
  if (i < 0)
    return str;
    
  return str.Extract(0, i);
#endif
}

nglString nuiLocale::Get()
{
#ifdef __APPLE__
  CFLocaleRef loc = CFLocaleCopyCurrent();
  CFStringRef str = (CFStringRef)CFLocaleGetValue(loc, kCFLocaleIdentifier);
  
  nglString s(str);
  
  CFRelease(str);
  CFRelease(loc);
  
  return s;
#else
  nglString str(setlocale(LC_ALL, NULL));
  return str;
#endif
}

void nuiLocale::Set(const nglString& rLocale)
{
  
}

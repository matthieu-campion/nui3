/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiLocale
{
public:
  static nglString GetCurrentCountry();
  static nglString GetCurrentLanguage();
  static nglString Get();
  static void Set(const nglString& rLocale);
};
/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"

#define DECO_TOOLBAR _T("DECO_TOOLBAR")

#define DECO_PANE_TITLE _T("DECO_PANE_TITLE")
#define DECO_PANE_CONTENTS _T("DECO_PANE_CONTENTS")


class DecorationManager
{
public:
  DecorationManager();
  ~DecorationManager();
  
  static nuiDecoration* Get(const nglString& rName);
  
private:

  void Init();
  static std::map<nglString, nuiDecoration*> mDecorations;
};

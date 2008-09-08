/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nui.h"


class nuiWidget;

typedef nuiFastDelegate::FastDelegate1<nuiWidget*> nuiDecorationDelegate;


class nuiDefaultDecoration
{
public:
  
  nuiDefaultDecoration();
  ~nuiDefaultDecoration();
  
  static void Init();

  static void nuiScrollView(nuiWidget* pWidget);

};
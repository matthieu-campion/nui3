/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"


class nuiFontInspector : public nuiSimpleContainer
{
public:
  nuiFontInspector();
  virtual ~nuiFontInspector();

protected:
  nuiEventSink<nuiFontInspector> mSink;
  
  bool OnFontsChanged(const nuiEvent& rEvent);
  void UpdateFonts();
  
};

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


  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();

protected:
  nuiEventSink<nuiFontInspector> mSink;
  
  void OnFontsChanged(const nuiEvent& rEvent);
  void UpdateFonts();
  
  static int32 UpdatingFonts;
};

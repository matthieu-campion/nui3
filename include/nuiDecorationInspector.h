/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"

class nuiGrid;

class nuiDecorationInspector : public nuiSimpleContainer
{
public:
  nuiDecorationInspector();
  virtual ~nuiDecorationInspector();
  
protected:
  nuiEventSink<nuiDecorationInspector> mSink;
  
  bool OnDecosChanged(const nuiEvent& rEvent);
  bool OnDecoSelection(const nuiEvent& rEvent);
  void UpdateDecos();
  void DumpDecl();
  
  nuiLabel* mpLabel;
  nuiLabel* mpInfoLabel;
  nuiGrid* mpAttributeGrid;
  nuiSlotsSink mSlot;
};

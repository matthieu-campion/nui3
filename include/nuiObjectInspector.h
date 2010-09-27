/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"

class nuiGrid;
class nuiText;

class nuiObjectInspector : public nuiSimpleContainer
{
public:
  nuiObjectInspector();
  virtual ~nuiObjectInspector();
  
protected:
  nuiEventSink<nuiObjectInspector> mSink;
  
  void OnObjectsChanged(const nuiEvent& rEvent);
  void OnObjectSelection(const nuiEvent& rEvent);
  void UpdateObjects();
  
  nuiImage* mpImage;
  nuiGrid* mpAttributeGrid;
  nuiSlotsSink mSlot;
};

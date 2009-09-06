/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiAttributeEditor.h"

class nuiEditLine;

class nuiPopupValueAttributeEditor : public nuiAttributeEditor
{
public:
  
  nuiPopupValueAttributeEditor(const nuiAttrib<uint32>& rAttribute, const nuiRange& rRange);
  virtual ~nuiPopupValueAttributeEditor();
  
protected:
  
  bool OnAttributeChanged(const nuiEvent& rEvent);
  bool OnValidated(const nuiEvent& rEvent);
  bool OnCanceled(const nuiEvent& rEvent);
  
private:
  
  nuiEditLine* mpEditLine;
  nuiAttrib<uint32> mAttribute;
  nuiRange mRange;
  
  nuiEventSink<nuiPopupValueAttributeEditor> mEventSink;
};
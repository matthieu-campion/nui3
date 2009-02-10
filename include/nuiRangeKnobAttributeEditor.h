/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class nuiKnob;

class NUI_API nuiRangeKnobAttributeEditor : public nuiAttributeEditor
{
public:

	nuiRangeKnobAttributeEditor(const nuiAttrib<const nuiRange&>& rAttribute);
	virtual ~nuiRangeKnobAttributeEditor();
	
private : 

  bool OnChanged(const nuiEvent& rEvent);

	void OnAttributeChanged(const nuiRange& rRange);
  void Update();

	nuiSlotsSink mSink;
	nuiAttrib<const nuiRange&> mAttribute;
  
  nuiRange mRange;
  nuiKnob* mpKnob;
  
  nuiEventSink<nuiRangeKnobAttributeEditor> mEventSink;
};

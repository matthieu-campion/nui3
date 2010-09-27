/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "nuiSlider.h"
#include "nuiFormatedLabel.h"

class NUI_API nuiRangeAttributeEditor : public nuiAttributeEditor
{
public:

	nuiRangeAttributeEditor(const nuiAttrib<const nuiRange&>& rAttribute);
	virtual ~nuiRangeAttributeEditor();
	
private : 

  void OnChanged(const nuiEvent& rEvent);

//	void OnAttributeChanged(const nuiRange& rRange);
  void Update();

	nuiSlotsSink mSink;
	nuiAttrib<const nuiRange&> mAttribute;
  
  nuiRange mRange;
  nuiHBox* mpMainBox;
  nuiSlider* mpSlider;
  nuiFormatedLabel* mpValue;
  
  nuiEventSink<nuiRangeAttributeEditor> mEventSink;
};

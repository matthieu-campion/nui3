/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"
#include "nuiToggleButton.h"


class NUI_API nuiBooleanAttributeEditor : public nuiAttributeEditor
{
public:

	nuiBooleanAttributeEditor(const nuiAttrib<bool>& rAttribute);
	virtual ~nuiBooleanAttributeEditor();
	
	

protected : 


	void OnAttributeChanged(bool value);
	void OnButtonPressed(const nuiEvent& rEvent);
	void OnButtonDePressed(const nuiEvent& rEvent);

	
private : 

	nuiSlotsSink mSink;
	nuiEventSink<nuiBooleanAttributeEditor> mEventSink;
	nuiAttrib<bool> mAttribute;
	nuiToggleButton* mpBtn;

};

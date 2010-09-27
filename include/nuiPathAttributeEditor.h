/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiPathAttributeEditor : public nuiAttributeEditor
{
public:

	nuiPathAttributeEditor(const nuiAttrib<nglPath>& pAttribute);
	virtual ~nuiPathAttributeEditor();
	
	

protected : 

//	void OnAttributeChanged(float value);
//	void OnChanged(const nuiEvent& rEvent);

	
private : 

	nuiSlotsSink mSink;
	nuiAttrib<nglPath> mAttribute;

};

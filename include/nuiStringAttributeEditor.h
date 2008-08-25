/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiStringAttributeEditor : public nuiAttributeEditor
{
public:

	nuiStringAttributeEditor(const nuiAttrib<nglString>& rAttribute);
	nuiStringAttributeEditor(const nuiAttrib<const nglString&>& rAttribute);
	virtual ~nuiStringAttributeEditor();


protected : 

	bool OnActivated(const nuiEvent& rEvent);
	bool OnRenamed(const nuiEvent& rEvent);
	void OnAttributeChanged(nglString string);
	void OnConstAttributeChanged(const nglString& string);

private : 

	nuiEventSink<nuiStringAttributeEditor> mEventSink;
	nuiSlotsSink mSink;
	nuiLabel* mpLabel;
	nuiAttrib<nglString> mAttribute;
	nuiAttrib<const nglString&> mConstAttribute;

};

/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiGenericAttributeEditor : public nuiAttributeEditor
{
public:
  
	nuiGenericAttributeEditor(const nuiAttribBase& rAttribute);
	virtual ~nuiGenericAttributeEditor();

  void SetAutoPollForChanges(bool set);

protected:
  
	void OnActivated(const nuiEvent& rEvent);
	void OnRenamed(const nuiEvent& rEvent);
	void OnAttributeChanged(const nuiEvent& rEvent);
  
private:
  
	nuiEventSink<nuiGenericAttributeEditor> mEventSink;
	nuiLabel* mpLabel;
	nuiAttribBase mAttribute;  
  bool mAutoPollForChanges;
};

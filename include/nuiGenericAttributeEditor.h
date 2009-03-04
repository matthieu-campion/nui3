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
  
  
  protected : 
  
	bool OnActivated(const nuiEvent& rEvent);
	bool OnRenamed(const nuiEvent& rEvent);
	bool OnAttributeChanged(const nuiEvent& rEvent);
  
  private : 
  
	nuiEventSink<nuiGenericAttributeEditor> mEventSink;
	nuiLabel* mpLabel;
	nuiAttribBase mAttribute;  
};

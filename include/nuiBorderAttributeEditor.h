/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiBorderAttributeEditor : public nuiAttributeEditor
{
public:

	nuiBorderAttributeEditor(const nuiAttrib<nuiBorder>& rAttribute);
	nuiBorderAttributeEditor(const nuiAttrib<const nuiBorder&>& rAttribute);
	virtual ~nuiBorderAttributeEditor();
	
	

protected : 

	void OnAttributeChanged(nuiBorder border);
	void OnConstAttributeChanged(const nuiBorder& border);

	
private : 

	nuiSlotsSink mSink;
	nuiAttrib<nuiBorder> mAttribute;
	nuiAttrib<const nuiBorder&> mConstAttribute;
	nuiLabel* mpLabel;

};

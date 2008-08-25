/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiSizeAttributeEditor : public nuiAttributeEditor
{
public:

	nuiSizeAttributeEditor(const nuiAttrib<double>& rAttribute);
	virtual ~nuiSizeAttributeEditor();
	
	

protected : 

	void OnAttributeChanged(double size);

	
private : 

	nuiSlotsSink mSink;
	nuiAttrib<double> mAttribute;
	nuiLabel* mpLabel;

};

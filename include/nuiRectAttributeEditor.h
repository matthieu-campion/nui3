/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiRectAttributeEditor : public nuiAttributeEditor
{
public:

	nuiRectAttributeEditor(const nuiAttrib<nuiRect>& rAttribute);
	nuiRectAttributeEditor(const nuiAttrib<const nuiRect&>& rAttribute);
	virtual ~nuiRectAttributeEditor();
	
	

protected : 

	void OnAttributeChanged(nuiRect rect);
	void OnConstAttributeChanged(const nuiRect& rect);

	
private : 

	nuiSlotsSink mSink;
	nuiAttrib<nuiRect> mAttribute;
	nuiAttrib<const nuiRect&> mConstAttribute;
	nuiLabel* mpLabel;

};

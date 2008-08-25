/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"

class NUI_API nuiPointAttributeEditor : public nuiAttributeEditor
{
public:

	nuiPointAttributeEditor(const nuiAttrib<nuiPoint>& rAttribute);
	virtual ~nuiPointAttributeEditor();


protected : 

	void OnAttributeChanged(nuiPoint point);

private : 

	nuiSlotsSink mSink;
	nuiLabel* mpLabel;
	nuiAttrib<nuiPoint> mAttribute;

};

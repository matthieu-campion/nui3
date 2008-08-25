/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiAttribute.h"
#include "nuiAttributeEditor.h"
#include "nuiPane.h"
#include "nuiButton.h"
#include "nuiColorSelector.h"
#include "nuiHBox.h"

class NUI_API nuiColorAttributeEditor : public nuiAttributeEditor
{
public:

	nuiColorAttributeEditor(const nuiAttrib<nuiColor>& rAttribute);
	nuiColorAttributeEditor(const nuiAttrib<const nuiColor&>& rAttribute);
  nuiHBox* Init();
	virtual ~nuiColorAttributeEditor();
	
protected : 

	void OnAttributeChanged(nuiColor color);
	void OnConstAttributeChanged(const nuiColor& color);
	bool OnChanged(const nuiEvent& rEvent);

	bool OnClicked(const nuiEvent& rEvent);
	
	std::vector<nuiColor> mColors;

private : 

	nuiEventSink<nuiColorAttributeEditor> mEventSink;
	nuiSlotsSink mSink;
	nuiAttrib<nuiColor> mAttribute;
	nuiAttrib<const nuiColor&> mConstAttribute;
	nuiPane* mpColorPane;
	nuiButton* mpButton;
	nuiColorSelector* mpSelector;

};

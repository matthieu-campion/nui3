/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

 
nuiPointAttributeEditor::nuiPointAttributeEditor(const nuiAttrib<nuiPoint>& rAttribute)
	: nuiAttributeEditor(),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiPointAttributeEditor"));
	
	nglString contents;
	mAttribute.Format(contents);
	
	mpLabel = new nuiLabel(contents);
	AddChild(mpLabel);

	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiPointAttributeEditor::OnAttributeChanged));
}


nuiPointAttributeEditor::~nuiPointAttributeEditor()
{

}


void nuiPointAttributeEditor::OnAttributeChanged(nuiPoint point)
{
	nglString contents;
	mAttribute.Format(contents);
	mpLabel->SetText(contents);
}

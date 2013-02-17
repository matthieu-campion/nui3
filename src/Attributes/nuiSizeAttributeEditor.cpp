/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

 
nuiSizeAttributeEditor::nuiSizeAttributeEditor(const nuiAttrib<double>& rAttribute)
	: nuiAttributeEditor(),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiSizeAttributeEditor"));
	
	nglString contents;
	mAttribute.Format(contents);
	
	mpLabel = new nuiLabel(contents);
	AddChild(mpLabel);

	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiSizeAttributeEditor::OnAttributeChanged));
}




nuiSizeAttributeEditor::~nuiSizeAttributeEditor()
{

}


void nuiSizeAttributeEditor::OnAttributeChanged(double size)
{
	nglString contents;
	mAttribute.Format(contents);
	mpLabel->SetText(contents);
}


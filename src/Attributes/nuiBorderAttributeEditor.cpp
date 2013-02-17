/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

 
nuiBorderAttributeEditor::nuiBorderAttributeEditor(const nuiAttrib<nuiBorder>& rAttribute)
	: nuiAttributeEditor(),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiBorderAttributeEditor"));
	
	nglString contents;
	mAttribute.Format(contents);
	
	mpLabel = new nuiLabel(contents);
	AddChild(mpLabel);

	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiBorderAttributeEditor::OnAttributeChanged));
}



nuiBorderAttributeEditor::nuiBorderAttributeEditor(const nuiAttrib<const nuiBorder&>& rAttribute)
	: nuiAttributeEditor(),
    mConstAttribute(rAttribute)
{
	SetObjectClass(_T("nuiBorderAttributeEditor"));
	
	nglString contents;
	mConstAttribute.Format(contents);
	
	mpLabel = new nuiLabel(contents);
	AddChild(mpLabel);

	mSink.Connect(mConstAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiBorderAttributeEditor::OnConstAttributeChanged));
}




nuiBorderAttributeEditor::~nuiBorderAttributeEditor()
{

}


void nuiBorderAttributeEditor::OnAttributeChanged(nuiBorder border)
{
	nglString contents;
	mAttribute.Format(contents);
	mpLabel->SetText(contents);
}


void nuiBorderAttributeEditor::OnConstAttributeChanged(const nuiBorder& border)
{
	nglString contents;
	mConstAttribute.Format(contents);
	mpLabel->SetText(contents);
}

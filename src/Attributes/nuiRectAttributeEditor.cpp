/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

 
nuiRectAttributeEditor::nuiRectAttributeEditor(const nuiAttrib<nuiRect>& rAttribute)
	: nuiAttributeEditor(),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiRectAttributeEditor"));
	
	nglString contents;
	mAttribute.Format(contents);
	
	mpLabel = new nuiLabel(contents);
	AddChild(mpLabel);

	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiRectAttributeEditor::OnAttributeChanged));
}



nuiRectAttributeEditor::nuiRectAttributeEditor(const nuiAttrib<const nuiRect&>& rAttribute)
	: nuiAttributeEditor(),
    mConstAttribute(rAttribute)
{
	SetObjectClass(_T("nuiRectAttributeEditor"));
	
	nglString contents;
	mConstAttribute.Format(contents);
	
	mpLabel = new nuiLabel(contents);
	AddChild(mpLabel);

	mSink.Connect(mConstAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiRectAttributeEditor::OnConstAttributeChanged));
}




nuiRectAttributeEditor::~nuiRectAttributeEditor()
{

}


void nuiRectAttributeEditor::OnAttributeChanged(nuiRect rect)
{
	nglString contents;
	mAttribute.Format(contents);
	mpLabel->SetText(contents);
}


void nuiRectAttributeEditor::OnConstAttributeChanged(const nuiRect& rect)
{
	nglString contents;
	mConstAttribute.Format(contents);
	mpLabel->SetText(contents);
}

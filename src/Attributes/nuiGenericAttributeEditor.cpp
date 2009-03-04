/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiLabelRenamer.h"
#include "nuiGenericAttributeEditor.h"
#include "nuiMouseEvent.h"
#include "nuiPane.h"
#include "nuiHBox.h"


nuiGenericAttributeEditor::nuiGenericAttributeEditor(const nuiAttribBase& rAttribute)
: nuiAttributeEditor(),
  mEventSink(this),
  mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiGenericAttributeEditor"));
	
	nglString contents;
	mAttribute.ToString(contents);
	//NGL_OUT(_T("Generic Attrib editor '%ls': '%ls'\n"), rAttribute.GetName().GetChars(), contents.GetChars());
	if (!mAttribute.IsReadOnly())
	{
		nuiPane* pPane = new nuiPane();
		mpLabel = new nuiLabel(contents);
		pPane->AddChild(mpLabel);
    AddChild(pPane);
		mEventSink.Connect(mpLabel->Clicked, &nuiGenericAttributeEditor::OnActivated);
	}
	else
	{
		mpLabel = new nuiLabel(contents);
    AddChild(mpLabel);
	}
	
	mEventSink.Connect(mAttribute.GetChangedEvent(), &nuiGenericAttributeEditor::OnAttributeChanged);
}

nuiGenericAttributeEditor::~nuiGenericAttributeEditor()
{
  
}


bool nuiGenericAttributeEditor::OnAttributeChanged(const nuiEvent& rEvent)
{
	nglString contents;
	mAttribute.ToString(contents);
	mpLabel->SetText(contents);
  
  return false;
}

bool nuiGenericAttributeEditor::OnActivated(const nuiEvent& rEvent)
{
	nuiLabelRenamer* pRenamer = new nuiLabelRenamer(mpLabel);
	mEventSink.Connect(pRenamer->Renamed, &nuiGenericAttributeEditor::OnRenamed, pRenamer);
	
	return true;
}


bool nuiGenericAttributeEditor::OnRenamed(const nuiEvent& rEvent)
{
	nuiLabelRenamer* pRenamer = (nuiLabelRenamer*)rEvent.mpUser;
  mAttribute.FromString(pRenamer->GetText());
	return true;
}


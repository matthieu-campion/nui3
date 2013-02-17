/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

 
nuiStringAttributeEditor::nuiStringAttributeEditor(const nuiAttrib<nglString>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiStringAttributeEditor"));
	
	nglString contents;
	mAttribute.Format(contents);
	
	if (!mAttribute.IsReadOnly())
	{
		nuiPane* pPane = new nuiPane();
		mpLabel = new nuiLabel(contents);
		pPane->AddChild(mpLabel);
    AddChild(pPane);
		mEventSink.Connect(mpLabel->Clicked, &nuiStringAttributeEditor::OnActivated);
	}
	else
	{
		mpLabel = new nuiLabel(contents);
    AddChild(mpLabel);
	}
	
	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiStringAttributeEditor::OnAttributeChanged));
}



nuiStringAttributeEditor::nuiStringAttributeEditor(const nuiAttrib<const nglString&>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mConstAttribute(rAttribute)
{
	SetObjectClass(_T("nuiStringAttributeEditor"));
	
	nglString contents;
	mConstAttribute.Format(contents);

	if (!mConstAttribute.IsReadOnly())
	{
		nuiPane* pPane = new nuiPane();
		mpLabel = new nuiLabel(contents);
		pPane->AddChild(mpLabel);
    AddChild(pPane);
		mEventSink.Connect(mpLabel->Clicked, &nuiStringAttributeEditor::OnActivated);
	}
	else
	{
		mpLabel = new nuiLabel(contents);
    AddChild(mpLabel);
	}
	
	mSink.Connect(mConstAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiStringAttributeEditor::OnConstAttributeChanged));
}

nuiStringAttributeEditor::~nuiStringAttributeEditor()
{

}


void nuiStringAttributeEditor::OnAttributeChanged(nglString string)
{
	nglString contents;
	mAttribute.Format(contents);
	mpLabel->SetText(contents);
}

void nuiStringAttributeEditor::OnConstAttributeChanged(const nglString& string)
{
	nglString contents;
	mConstAttribute.Format(contents);
	mpLabel->SetText(contents);
}


void nuiStringAttributeEditor::OnActivated(const nuiEvent& rEvent)
{
	nuiLabelRenamer* pRenamer = new nuiLabelRenamer(mpLabel);
	mEventSink.Connect(pRenamer->Renamed, &nuiStringAttributeEditor::OnRenamed, pRenamer);
  rEvent.Cancel();
}


void nuiStringAttributeEditor::OnRenamed(const nuiEvent& rEvent)
{
	nuiLabelRenamer* pRenamer = (nuiLabelRenamer*)rEvent.mpUser;
	if (mAttribute.IsValid())
	{
		mAttribute.Set(pRenamer->GetText());
	}
	else if (mConstAttribute.IsValid())
	{
		mConstAttribute.Set(pRenamer->GetText());
	}
  rEvent.Cancel();
}


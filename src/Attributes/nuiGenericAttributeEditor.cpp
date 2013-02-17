/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


nuiGenericAttributeEditor::nuiGenericAttributeEditor(const nuiAttribBase& rAttribute)
: nuiAttributeEditor(),
  mEventSink(this),
  mAttribute(rAttribute),
  mAutoPollForChanges(false)
{
	SetObjectClass(_T("nuiGenericAttributeEditor"));
	
	//NGL_OUT(_T("Generic Attrib editor '%s': '%s'\n"), rAttribute.GetName().GetChars(), contents.GetChars());
  mpLabel = new nuiLabel();
  AddChild(mpLabel);
	if (!mAttribute.IsReadOnly())
    mEventSink.Connect(mpLabel->Clicked, &nuiGenericAttributeEditor::OnActivated);
  else
    mpLabel->SetEnabled(false);
	
	mEventSink.Connect(mAttribute.GetChangedEvent(), &nuiGenericAttributeEditor::OnAttributeChanged);
  OnAttributeChanged(nuiEvent());
}

nuiGenericAttributeEditor::~nuiGenericAttributeEditor()
{
  
}


void nuiGenericAttributeEditor::OnAttributeChanged(const nuiEvent& rEvent)
{
	nglString contents;
  
  if (!mAttribute.CanGet())
  {
    mpLabel->SetText(_T("Write only attribute"));
    return;
  }
  
  switch (mAttribute.GetDimension())
  {
    case 0:
      {
        mAttribute.ToString(contents);
        mpLabel->SetText(contents);
      }
      break;
    case 1:
      {
//        for (uint32 i = 0; i < mAttribute.GetDimensionRange(0); i++)
//        {
//          mAttribute.FromString(i, pRenamer->GetText());
//        }
        mpLabel->SetText(_T("MultiDimension attribute not supported"));

      }
      break;
    case 2:
    {
//      for (uint32 i = 0; i < mAttribute.GetDimensionRange(0); i++)
//      {
//        for (uint32 j = 0; j < mAttribute.GetDimensionRange(1); i++)
//        {
//          mAttribute.FromString(i, j, pRenamer->GetText());
//          
//        }
//      }
      mpLabel->SetText(_T("MultiDimension attribute not supported"));
    }
      break;
  }
}

void nuiGenericAttributeEditor::OnActivated(const nuiEvent& rEvent)
{
	nuiLabelRenamer* pRenamer = new nuiLabelRenamer(mpLabel);
	mEventSink.Connect(pRenamer->Renamed, &nuiGenericAttributeEditor::OnRenamed, pRenamer);
	
  rEvent.Cancel();
}


void nuiGenericAttributeEditor::OnRenamed(const nuiEvent& rEvent)
{
	nuiLabelRenamer* pRenamer = (nuiLabelRenamer*)rEvent.mpUser;
  uint32 dim = mAttribute.GetDimension();
  switch (dim)
  {
    case 0:
      {
        mAttribute.FromString(pRenamer->GetText());
      }
      break;
    case 1:
      {
//        for (uint32 i = 0; i < mAttribute.GetDimensionRange(0); i++)
//        {
//          mAttribute.FromString(i, pRenamer->GetText());
//        }
      }
      break;
    case 2:
      {
//        for (uint32 i = 0; i < mAttribute.GetDimensionRange(0); i++)
//        {
//          for (uint32 j = 0; j < mAttribute.GetDimensionRange(1); i++)
//          {
//            mAttribute.FromString(i, j, pRenamer->GetText());
//            
//          }
//        }
      }
      break;
  }
  rEvent.Cancel();
}

void nuiGenericAttributeEditor::SetAutoPollForChanges(bool set)
{
  if (mAutoPollForChanges == set)
    return;

  if (set)
    mEventSink.Connect(nuiAnimation::GetTimer()->Tick, &nuiGenericAttributeEditor::OnAttributeChanged);
  else
    mEventSink.Disconnect(nuiAnimation::GetTimer()->Tick, &nuiGenericAttributeEditor::OnAttributeChanged);

  mAutoPollForChanges = set;
}


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
  mAttribute(rAttribute),
  mAutoPollForChanges(false)
{
	SetObjectClass(_T("nuiGenericAttributeEditor"));
	
	//NGL_OUT(_T("Generic Attrib editor '%ls': '%ls'\n"), rAttribute.GetName().GetChars(), contents.GetChars());
	if (!mAttribute.IsReadOnly())
	{
		nuiPane* pPane = new nuiPane();
		mpLabel = new nuiLabel();
		pPane->AddChild(mpLabel);
    AddChild(pPane);
		mEventSink.Connect(mpLabel->Clicked, &nuiGenericAttributeEditor::OnActivated);
	}
	else
	{
		mpLabel = new nuiLabel();
    AddChild(mpLabel);
	}
	
	mEventSink.Connect(mAttribute.GetChangedEvent(), &nuiGenericAttributeEditor::OnAttributeChanged);
  OnAttributeChanged(nuiEvent());
}

nuiGenericAttributeEditor::~nuiGenericAttributeEditor()
{
  
}


bool nuiGenericAttributeEditor::OnAttributeChanged(const nuiEvent& rEvent)
{
	nglString contents;
  
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
  switch (mAttribute.GetDimension())
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
	return true;
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


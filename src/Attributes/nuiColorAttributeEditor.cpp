/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

 
nuiColorAttributeEditor::nuiColorAttributeEditor(const nuiAttrib<nuiColor>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mAttribute(rAttribute)
{
  SetObjectClass(_T("nuiColorAttributeEditor"));
  Init(mAttribute.Get());
	// the visual object of the property need to be warned when the property value has been set without a user control
  if (mAttribute.GetDimension() == 0)
    mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiColorAttributeEditor::OnAttributeChanged));
}



nuiColorAttributeEditor::nuiColorAttributeEditor(const nuiAttrib<const nuiColor&>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mConstAttribute(rAttribute)
{
  SetObjectClass(_T("nuiColorAttributeEditor"));
  Init(mConstAttribute.Get());
	// the visual object of the property need to be warned when the property value has been set without a user control
  if (mConstAttribute.GetDimension() == 0)
    mSink.Connect(mConstAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiColorAttributeEditor::OnConstAttributeChanged));
}

void nuiColorAttributeEditor::Init(const nuiColor& rColor)
{
  //Init static color list
  mColors.push_back(nuiColor(255, 172, 42, 255));
  mColors.push_back(nuiColor(5, 45, 104, 255));
  mColors.push_back(nuiColor(139, 28, 36, 255));
  mColors.push_back(nuiColor(184, 0, 0, 255));
  mColors.push_back(nuiColor(0, 114, 54, 255));
  mColors.push_back(nuiColor(237, 10, 104, 255));
  mColors.push_back(nuiColor(241, 58, 36, 255));
  mColors.push_back(nuiColor(77, 184, 73, 255));
  mColors.push_back(nuiColor(234, 27, 60, 255));
  mColors.push_back(nuiColor(40, 78, 182, 255));
  mColors.push_back(nuiColor(86, 42, 143, 255));
  mColors.push_back(nuiColor(241, 114, 33, 255));
  mColors.push_back(nuiColor(51, 51, 51, 255));
  mColors.push_back(nuiColor(230, 230, 230, 255));

	nuiHBox* hbox = new nuiHBox(2);
	AddChild(hbox);
	
	hbox->SetCell(0, new nuiLabel(_T("color")));
	hbox->SetCellExpand(0, nuiExpandShrinkAndGrow);
	
  mpButton = new nuiButton();

  mpColorPane = new nuiPane(rColor, nuiColor(0.f, 0.f, 0.f, 1.f));
  mpColorPane->SetUserSize(10.f, 10.f);
  mpButton->AddChild(mpColorPane);
  
	mpButton->SetPosition(nuiRight);
  hbox->SetCell(1, mpButton);
	hbox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  
  mEventSink.Connect(mpButton->Activated, &nuiColorAttributeEditor::OnClicked);
}


nuiColorAttributeEditor::~nuiColorAttributeEditor()
{

}


void nuiColorAttributeEditor::OnClicked(const nuiEvent& rEvent)
{
  if (mAttribute.IsValid())
    mpSelector = new nuiColorSelector(mAttribute.Get(), mColors);
  else
    mpSelector = new nuiColorSelector(mConstAttribute.Get(), mColors);
    
  nuiPopupView* pPopup = new nuiPopupView(mpColorPane, mpColorPane->GetRect().Left(), mpColorPane->GetRect().Top());
  pPopup->AddChild(mpSelector);
  
  mEventSink.Connect(mpSelector->ColorChanged, &nuiColorAttributeEditor::OnChanged);
  rEvent.Cancel();
}


void nuiColorAttributeEditor::OnChanged(const nuiEvent& rEvent)
{
  if (mAttribute.IsValid())
    mAttribute.Set(mpSelector->GetCurrentColor());
	else 
    mConstAttribute.Set(mpSelector->GetCurrentColor());
}


void nuiColorAttributeEditor::OnAttributeChanged(nuiColor value)
{
  mpColorPane->SetFillColor(value);
  mpColorPane->Invalidate();
}

void nuiColorAttributeEditor::OnConstAttributeChanged(const nuiColor& value)
{
  mpColorPane->SetFillColor(value);
  mpColorPane->Invalidate();
}

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiRangeAttributeEditor::nuiRangeAttributeEditor(const nuiAttrib<const nuiRange&>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiRangeAttributeEditor"));
  
  mRange = mAttribute.Get();
	
  mpMainBox = NULL;
  Update();
//	mSink.Connect(mpAttribute->AttributeChanged, nuiMakeDelegate(this, &nuiRangeAttributeEditor::OnAttributeChanged));
}




nuiRangeAttributeEditor::~nuiRangeAttributeEditor()
{

}

void nuiRangeAttributeEditor::OnChanged(const nuiEvent& rEvent)
{
  mRange.SetValue(mpSlider->GetRange().GetValue());
  mpValue->SetValue(mRange.GetValue());
  mAttribute.Set(mRange);
  rEvent.Cancel();
}


void nuiRangeAttributeEditor::Update()
{
  if (mpMainBox)
    mpMainBox->Trash();
    
  mpMainBox = new nuiHBox(2);
  AddChild(mpMainBox);
  
  // font
  nuiFontRequest rFontRequest;
  rFontRequest.SetGenericName(_T("sans-serif"), 1.0f);
  rFontRequest.SetBold(false, 1.0); 
  rFontRequest.MustHaveSize(10, 1.0);
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(rFontRequest);

  // display the attribute's value
  mpValue = new nuiFormatedLabel(nuiFormatedLabel::DefaultFormater, mRange.GetValue(), pFont);
  mpMainBox->SetCell(0, mpValue);
  mpMainBox->SetCellMinPixels(0, 25);
  mpMainBox->SetCellMaxPixels(0, 25);
  
  // display the slider with a rule
  nuiVBox* pBox = new nuiVBox(2);
  mpMainBox->SetCell(1, pBox);
  
  
  // rules
  nuiHBox* pRule = new nuiHBox(0);
  pBox->SetCell(0, pRule);
  pBox->SetCellMinPixels(0, 10);
  pBox->SetCellMaxPixels(0, 10);
  nglString tmp;
  tmp.Format(_T("%.0fs"), mRange.GetMinimum());
  pRule->AddCell(new nuiLabel(tmp, pFont), nuiBottomLeft);
  pRule->SetCellExpand(0, nuiExpandShrinkAndGrow);
  tmp.Format(_T("%.0fs"), mRange.GetMaximum());
  pRule->AddCell(new nuiLabel(tmp, pFont), nuiBottomRight);
  pRule->SetCellExpand(0, nuiExpandShrinkAndGrow);
  
  
  // slider
  mpSlider = new nuiSlider(nuiVertical, mRange);
  mEventSink.Connect(mpSlider->InteractiveValueChanged, &nuiRangeAttributeEditor::OnChanged);
  pBox->SetCell(1, mpSlider);
}

//void nuiRangeAttributeEditor::OnAttributeChanged(const nuiRange& rRange)
//{
//  mRange = rRange;
//  Update();
//}


/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"



nuiPercentAttributeEditor::nuiPercentAttributeEditor(const nuiAttrib<float>& rAttribute)
: nuiAttributeEditor(),
  mEventSink(this),
  mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiPercentAttributeEditor"));
    
	nuiVBox* vbox = new nuiVBox(0);
	vbox->SetExpand(nuiExpandShrinkAndGrow);
	AddChild(vbox);
	
	
	vbox->AddCell(new nuiLabel(mAttribute.GetName()));
	
	nuiRange range = mAttribute.GetRange();
	uint nbincrem = (range.GetMaximum() - range.GetMinimum()) / mAttribute.GetRange().GetIncrement();
	
	
	nuiHBox* signs = new nuiHBox(nbincrem +1);
	vbox->AddCell(signs);
	
	uint j;
	double i;
	for (i=range.GetMinimum(), j=0; i <= range.GetMaximum(); i+=range.GetIncrement(), j++)
	{
		nglString label;
		if ((i==range.GetMinimum()) || (i==range.GetMaximum()))
			label.CFormat(_T("%.0f%%"), i*100.0f);
		else
      label = _T("|");
		
		signs->SetCell(j, new nuiLabel(label, nuiFont::GetFont(8)));
		signs->SetCellExpand(j, nuiExpandShrinkAndGrow);
	} 
	
	mpSlider = new nuiSlider(nuiHorizontal, mAttribute.GetRange());
	vbox->AddCell(mpSlider);
	
	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiPercentAttributeEditor::OnAttributeChanged));	
	
	mEventSink.Connect(mpSlider->InteractiveValueChanged, &nuiPercentAttributeEditor::OnChanged);
}

nuiPercentAttributeEditor::~nuiPercentAttributeEditor()
{
  
}



void nuiPercentAttributeEditor::OnChanged(const nuiEvent& rEvent)
{
	float value = (float)mpSlider->GetRange().GetValue();
	mAttribute.Set(value);
  rEvent.Cancel();
}


void nuiPercentAttributeEditor::OnAttributeChanged(float value)
{
	mpSlider->GetRange().SetValue(value);
}

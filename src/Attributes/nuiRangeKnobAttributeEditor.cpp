/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiRangeKnobAttributeEditor::nuiRangeKnobAttributeEditor(const nuiAttrib<const nuiRange&>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiRangeKnobAttributeEditor"));
  
  mpKnob = NULL;
  mRange = mAttribute.Get();
	
  Update();
	mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiRangeKnobAttributeEditor::OnAttributeChanged));
}




nuiRangeKnobAttributeEditor::~nuiRangeKnobAttributeEditor()
{

}

void nuiRangeKnobAttributeEditor::OnChanged(const nuiEvent& rEvent)
{
  mRange.SetValue(mpKnob->GetRange().GetValue());
  mAttribute.Set(mRange);
  rEvent.Cancel();
}


void nuiRangeKnobAttributeEditor::Update()
{
  if (mpKnob)
    mpKnob->Trash();
    
  mpKnob = new nuiKnob(mRange);
  AddChild(mpKnob);
  
  mEventSink.Connect(mpKnob->InteractiveValueChanged, &nuiRangeKnobAttributeEditor::OnChanged);
}



void nuiRangeKnobAttributeEditor::OnAttributeChanged(const nuiRange& rRange)
{
  mRange = rRange;
  Invalidate();
}


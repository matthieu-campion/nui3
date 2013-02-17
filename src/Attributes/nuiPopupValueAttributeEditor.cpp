/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"



nuiPopupValueAttributeEditor::nuiPopupValueAttributeEditor(const nuiAttrib<uint32>& rAttribute, const nuiRange& rRange)
: nuiAttributeEditor(), mAttribute(rAttribute), mRange(rRange), mEventSink(this)
{
  SetObjectClass(_T("nuiPopupValueAttributeEditor"));
  
  mpEditLine = new nuiEditLine();
  mpEditLine->SetObjectName(_T("NameAttributeEditor::EditLine"));
  
  AddChild(mpEditLine);
  
  nglString str;
  mAttribute.ToString(str);
  mpEditLine->SetText(str);
  
  mpEditLine->Focus();
  mpEditLine->Do(nuiEditLine::eSelectAll,NULL);
  
  mEventSink.Connect(mpEditLine->Activated, &nuiPopupValueAttributeEditor::OnValidated);
  mEventSink.Connect(mpEditLine->Aborted, &nuiPopupValueAttributeEditor::OnCanceled);
  mEventSink.Connect(mAttribute.GetChangedEvent(), &nuiPopupValueAttributeEditor::OnAttributeChanged);
}




nuiPopupValueAttributeEditor::~nuiPopupValueAttributeEditor()
{
  
}

void nuiPopupValueAttributeEditor::OnValidated(const nuiEvent& rEvent)
{
  nglString userStrValue = mpEditLine->GetText();
  double value = userStrValue.GetCDouble();
  
  nglString finalStrValue = userStrValue;
  double min = mRange.GetMinimum();
  double max = mRange.GetMaximum();
  if (value < min)
  {
    finalStrValue.SetCDouble(min);
  }
  else if (value > max)
  {
    finalStrValue.SetCDouble(max);
  }

  mAttribute.FromString(finalStrValue);
  mpEditLine->SetText(finalStrValue);
  
  
  // auto trash
  Trash();
  
  rEvent.Cancel();
}

void nuiPopupValueAttributeEditor::OnCanceled(const nuiEvent& rEvent)
{
  // auto trash
  Trash();
  rEvent.Cancel();
}


void nuiPopupValueAttributeEditor::OnAttributeChanged(const nuiEvent& rEvent)
{
  nglString str;
  mAttribute.ToString(str);
  mpEditLine->SetText(str);
}

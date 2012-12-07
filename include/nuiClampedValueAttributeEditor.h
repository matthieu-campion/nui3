/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once


#include "nui.h"
#include "nuiVBox.h"
#include "nuiAttributeEditor.h"
#include "nuiKnob.h"
#include "nuiPopupValueAttributeEditor.h"

template <class T>
class nuiClampedValueAttributeEditor : public nuiAttributeEditor
{
public:
  
  nuiClampedValueAttributeEditor(const nuiAttrib<T>& rAttribute, const nuiRange& rRange, const nglString& rName = nglString::Null)
	: nuiAttributeEditor(),
  mAttribute(rAttribute),
  mEventSink(this),
  mRange(rRange),
  mIndexed(false),
  mIndex(0)
  {
    if (!rName.IsNull())
      mName = rName;
    else
      mName = mAttribute.GetName();
    
    SetObjectClass(_T("nuiClampedValueAttributeEditor"));
    
    mpKnob = NULL;
    mpLabel = NULL;
    mInteractiveChange = false;
    
    mpBox = new nuiVBox(3);
    mpBox->SetExpand(nuiExpandShrinkAndGrow);
    AddChild(mpBox);
    
    Init();
    mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiClampedValueAttributeEditor::OnAttributeChanged));
  }
  
  
  nuiClampedValueAttributeEditor(const nglString& rName, const nuiAttrib<T>& rAttribute, uint32 index, const nuiRange& rRange)
	: nuiAttributeEditor(),
  mAttribute(rAttribute),
  mName(rName),
  mEventSink(this),
  mRange(rRange),
  mIndexed(true),
  mIndex(index)
  {
    SetObjectClass(_T("nuiClampedValueAttributeEditor"));
    
    mpKnob = NULL;
    mpLabel = NULL;
    mInteractiveChange = false;
    
    mpBox = new nuiVBox(3);
    mpBox->SetExpand(nuiExpandShrinkAndGrow);
    AddChild(mpBox);
    
    Init();
    mSink.Connect(mAttribute.GetChangedSignal0(), nuiMakeDelegate(this, &nuiClampedValueAttributeEditor::OnIndexedAttributeChanged));
  }
  
  
  
  
  virtual ~nuiClampedValueAttributeEditor()
  {
    
  }
  
protected:
  
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
  {
    if ((Button & nglMouseInfo::ButtonLeft) && (Button & nglMouseInfo::ButtonDoubleClick))
    {
      nuiPopupValueAttributeEditor* pEditor = new nuiPopupValueAttributeEditor(mAttribute, mRange);
      pEditor->SetPosition(nuiCenter);
      pEditor->SetUserSize(40,40);
      AddChild(pEditor);
      return true;
    }
    
    return false;
  }
  
  
  private : 
  
  
  
  void Init()
  {
    T v = 0;
    if (mIndexed)
      v = mAttribute.Get(mIndex);
    else
      v = mAttribute.Get();
    
    mRange.SetValue(v);
    nuiLabel* pLabel = new nuiLabel(mName);
    pLabel->SetObjectName("nuiClampedValueAttributeEditor::Name");
    pLabel->SetPosition(nuiCenter);
    pLabel->SetBorder(0,0,0,4);
    mpBox->SetCell(2, pLabel);
    
    mpKnob = new nuiKnob(mRange);
    mpKnob->SetObjectName("nuiClampedValueAttributeEditor::Knob");
    nuiImageSequence* pSequence = NULL;
    
    mpKnob->SetImageSequence(pSequence);
    
    mpKnob->SetPosition(nuiCenter);
    mpBox->SetCell(0, mpKnob);
    
    nglString str;
    if (mIndexed)
      mAttribute.Format(mIndex, str);
    else
      mAttribute.Format(str);
    
    mpLabel = new nuiLabel(str);
    mpLabel->SetObjectName("nuiClampedValueAttributeEditor::Value");
    mpLabel->SetPosition(nuiCenter);
    mpLabel->SetBorder(0,0,2,0);
    mpBox->SetCell(1, mpLabel);
    
    mValueBackup = mpKnob->GetRange().GetValue();
    
    mEventSink.Connect(mpKnob->InteractiveValueChanged, &nuiClampedValueAttributeEditor::OnInteractiveChanged);
    mEventSink.Connect(mpKnob->ValueChanged, &nuiClampedValueAttributeEditor::OnChanged);
  }
  
  
  void OnInteractiveChanged(const nuiEvent& rEvent)
  {
    if (!mInteractiveChange)
    {
      mInteractiveChange = true;
    }
    
    nglString str;
    T setValue;
    
    if (mIndexed)
    {
      mAttribute.Set(mIndex, (T)mpKnob->GetRange().GetValue());
      mAttribute.Format(mIndex, str);
      setValue = mAttribute.Get(mIndex);
    }
    else
    {
      mAttribute.Set((T)mpKnob->GetRange().GetValue());
      mAttribute.Format(str);
      setValue = mAttribute.Get();
    }
    
    {
      // update the knob with the value really set
      mpKnob->GetRange().SetValue(setValue);
      
      mpLabel->SetText(str);
    }
    rEvent.Cancel();
  }  
  
  
  
  void OnChanged(const nuiEvent& rEvent)
  {
    
    nglString value;
    nglString valueBackup;
    
    if (mIndexed)
      mAttribute.ToString(mIndex, value);
    else
      mAttribute.ToString(value);
    
    mAttribute.FormatValue(mValueBackup, valueBackup);
    
    nglString str;
    if (mIndexed)
    {
      mAttribute.Format(mIndex, str);
      mpLabel->SetText(str);
      mValueBackup = mAttribute.Get(mIndex);    
    }
    else
    {
      mAttribute.Format(str);
      mpLabel->SetText(str);
      mValueBackup = mAttribute.Get();    
    }
    
    mInteractiveChange = false;
    T setValue;
    
    if (mIndexed)
    {
      mAttribute.Format(mIndex, str);
      setValue = mAttribute.Get(mIndex);
    }
    else
    {
      mAttribute.Format(str);
      setValue = mAttribute.Get();
    }
    
    // update the knob with the value really set
    {
      mpKnob->GetRange().SetValue(setValue);
      mpLabel->SetText(str);
    }
    rEvent.Cancel();
  }  
  
  
  void OnAttributeChanged(T val)
  {
    if (mInteractiveChange)
      return;
    
    nglString str;
    T setValue;
    
    if (mIndexed)
    {
      mAttribute.Format(mIndex, str);
      setValue = mAttribute.Get(mIndex);
    }
    else
    {
      mAttribute.Format(str);
      setValue = mAttribute.Get();
    }
    
    mpKnob->GetRange().SetValue(setValue);
    
    mpLabel->SetText(str);
    
    Invalidate();
  }
  
	
  void OnIndexedAttributeChanged(uint32 index, T val)
  {
    if (mInteractiveChange)
      return;
    
    nglString str;
    if (mIndexed)
      mAttribute.Format(mIndex, str);
    mpKnob->GetRange().SetValue(mAttribute.Get(mIndex));
    
    mpLabel->SetText(str);
    
    Invalidate();
  }
  
  
	nuiSlotsSink mSink;
	nuiAttrib<T> mAttribute;
  
  nglString mName;
  
  nuiVBox* mpBox;
  nuiRange mRange;
  nuiKnob* mpKnob;
  nuiLabel* mpLabel;
  bool mInteractiveChange;
  T mValueBackup;
  
  bool mIndexed;
  uint32 mIndex;
  
  nuiEventSink<nuiClampedValueAttributeEditor<T> > mEventSink;
};

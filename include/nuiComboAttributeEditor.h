/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once


#include "nui.h"
#include "nuiAttributeEditor.h"
#include "nuiComboBox.h"
#include "nuiPopupValueAttributeEditor.h"

template <class T>
class nuiComboAttributeEditor : public nuiAttributeEditor
{
public:
  
  nuiComboAttributeEditor(const nuiAttrib<T>& rAttribute, const std::vector<std::pair<nglString, T> >& rValues, const nglString& rName = nglString::Null, uint32 flag = 1)
	: nuiAttributeEditor(),
  mAttribute(rAttribute),
  mEventSink(this),
  mValues(rValues),
  mIndexed(false),
  mIndex(0)
  {
    if (rName != nglString::Null)
      mName = rName;
    else
      mName = mAttribute.GetName();
    
    SetObjectClass(_T("nuiComboAttributeEditor"));
    
    mpCombo = NULL;
    
    Init(flag);
    mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiComboAttributeEditor::OnAttributeChanged));
  }
  
  
  nuiComboAttributeEditor(const nglString& rName, const nuiAttrib<T>& rAttribute, uint32 index, std::vector<std::pair<nglString, T> >& rValues, uint32 flag = 1)
	: nuiAttributeEditor(),
  mAttribute(rAttribute),
  mName(rName),
  mEventSink(this),
  mValues(rValues),
  mIndexed(true),
  mIndex(index)
  {
    SetObjectClass(_T("nuiComboAttributeEditor"));
    
    mpCombo = NULL;
    
    Init(flag);
    mSink.Connect(mAttribute.GetChangedSignal0(), nuiMakeDelegate(this, &nuiComboAttributeEditor::OnIndexedAttributeChanged));
  }
  
  
  
  
  virtual ~nuiComboAttributeEditor()
  {
    
  }
  

private:
  
  void Init(uint32 flag)
  {
    // Create choice tree:
    nuiTreeNode* pTree = new nuiTreeNode(nglString::Empty);    
    for (uint32 i = 0; i < mValues.size(); i++)
    {
      nuiTreeNode* pNode = new nuiTreeNode(mValues[i].first);
      nglString val;
      mAttribute.FormatValue(mValues[i].second, val);
      pNode->SetProperty(_T("Value"), val);
      pTree->AddChild(pNode);
    }
    
    mpCombo = new nuiComboBox(pTree);
    AddChild(mpCombo);
    
    mEventSink.Connect(mpCombo->SelectionChanged, &nuiComboAttributeEditor::OnChanged);
  }
  
  
  bool OnChanged(const nuiEvent& rEvent)
  {
    mpCombo->SelectionChanged.Disable();
    if (mIndexed)
    {
      mAttribute.FromString(mIndex, mpCombo->GetSelected()->GetProperty(_T("Value")));
    }
    else
    {
      mAttribute.FromString(mpCombo->GetSelected()->GetProperty(_T("Value")));
    }
    mpCombo->SelectionChanged.Enable();
    return true;
  }  
  
  
  
  void OnAttributeChanged(T val)
  {
    nglString str;
    mAttribute.FormatValue(val, str);

    const nuiTreeNode* pTree = mpCombo->GetChoices();
    for (uint32 i = 0; i < pTree->GetChildrenCount(); i++)
    {
      const nuiTreeNode* pNode = (const nuiTreeNode*)pTree->GetChild(i);
      if (pNode->GetProperty(_T("Value")) == str)
      {
        mpCombo->SetSelected(pNode);
        break;
      }
    }
  }
  
	
  void OnIndexedAttributeChanged(uint32 index, T val)
  {
    OnAttributeChanged(val);
  }
  
  
	nuiSlotsSink mSink;
	nuiAttrib<T> mAttribute;
  
  nglString mName;
  std::vector<std::pair<nglString, T> > mValues;
  nuiComboBox* mpCombo;
  
  bool mIndexed;
  uint32 mIndex;
  
  nuiEventSink<nuiComboAttributeEditor<T> > mEventSink;
};

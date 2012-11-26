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
    if (!rName.IsNull())
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
      nuiToken<T>* pToken = new nuiToken<T>(mValues[i].second);
      pNode->SetToken(pToken);
      pTree->AddChild(pNode);
    }
    
    mpCombo = new nuiComboBox(pTree);
    AddChild(mpCombo);
    
    mEventSink.Connect(mpCombo->SelectionChanged, &nuiComboAttributeEditor::OnChanged);
    
    if (mIndexed)
      OnAttributeChanged(mAttribute.Get(mIndex));
    else
      OnAttributeChanged(mAttribute.Get());
  }
  
  
  void OnChanged(const nuiEvent& rEvent)
  {
    const nuiTreeNode* pNode = mpCombo->GetSelected();
    nglString str;
    if (pNode)
    {
      nuiToken<T>* pToken = (nuiToken<T>*)pNode->GetToken();
      T val = pToken->Token;

      mpCombo->SelectionChanged.Disable();
      if (mIndexed)
      {
        mAttribute.Set(mIndex, val);
      }
      else
      {
        mAttribute.Set(val);
      }
      mpCombo->SelectionChanged.Enable();
    }
    
    rEvent.Cancel();
  }  
  
  
  
  void OnAttributeChanged(T val)
  {
    const nuiTreeNode* pTree = mpCombo->GetChoices();
    for (uint32 i = 0; i < pTree->GetChildrenCount(); i++)
    {
      const nuiTreeNode* pNode = (const nuiTreeNode*)pTree->GetChild(i);
      nuiToken<T>* pToken = (nuiToken<T>*)pNode->GetToken();
      T tval = pToken->Token;
      if (val == tval)
      {
        mpCombo->SelectionChanged.Disable();
        mpCombo->SetSelected(pNode);
        mpCombo->SelectionChanged.Enable();
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


template <typename T>
class nuiComboAttributeEditor<const T&> : public nuiAttributeEditor
{
public:
  
  nuiComboAttributeEditor(const nuiAttrib<const T&>& rAttribute, const std::vector<std::pair<nglString, T> >& rValues, const nglString& rName = nglString::Null, uint32 flag = 1)
	: nuiAttributeEditor(),
  mAttribute(rAttribute),
  mEventSink(this),
  mValues(rValues),
  mIndexed(false),
  mIndex(0)
  {
    if (!rName.IsNull())
      mName = rName;
    else
      mName = mAttribute.GetName();
    
    SetObjectClass(_T("nuiComboAttributeEditor(const ref)"));
    
    mpCombo = NULL;
    
    Init(flag);
    mSink.Connect(mAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiComboAttributeEditor<const T&>::OnAttributeChanged));
  }
  
  
  nuiComboAttributeEditor(const nglString& rName, const nuiAttrib<const T&>& rAttribute, uint32 index, std::vector<std::pair<nglString, T> >& rValues, uint32 flag = 1)
	: nuiAttributeEditor(),
  mAttribute(rAttribute),
  mName(rName),
  mEventSink(this),
  mValues(rValues),
  mIndexed(true),
  mIndex(index)
  {
    SetObjectClass(_T("nuiComboAttributeEditor(const ref)"));
    
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
      nuiToken<T>* pToken = new nuiToken<T>(mValues[i].second);
      pNode->SetToken(pToken);
      pTree->AddChild(pNode);
    }
    
    mpCombo = new nuiComboBox(pTree);
    AddChild(mpCombo);
    
    mEventSink.Connect(mpCombo->SelectionChanged, &nuiComboAttributeEditor::OnChanged);
    
    if (mIndexed)
      OnAttributeChanged(mAttribute.Get(mIndex));
    else
      OnAttributeChanged(mAttribute.Get());
  }
  
  
  void OnChanged(const nuiEvent& rEvent)
  {
    const nuiTreeNode* pNode = mpCombo->GetSelected();
    nglString str;
    if (pNode)
    {
      nuiToken<T>* pToken = (nuiToken<T>*)pNode->GetToken();
      T val = pToken->Token;
      
      mpCombo->SelectionChanged.Disable();
      if (mIndexed)
      {
        mAttribute.Set(mIndex, val);
      }
      else
      {
        mAttribute.Set(val);
      }
      mpCombo->SelectionChanged.Enable();
    }
    
    rEvent.Cancel();
  }  
  
  
  
  void OnAttributeChanged(const T& val)
  {
    const nuiTreeNode* pTree = mpCombo->GetChoices();
    for (uint32 i = 0; i < pTree->GetChildrenCount(); i++)
    {
      const nuiTreeNode* pNode = (const nuiTreeNode*)pTree->GetChild(i);
      nuiToken<T>* pToken = (nuiToken<T>*)pNode->GetToken();
      T tval = pToken->Token;
      if (val == tval)
      {
        mpCombo->SelectionChanged.Disable();
        mpCombo->SetSelected(pNode);
        mpCombo->SelectionChanged.Enable();
        break;
      }
    }
  }
  
	
  void OnIndexedAttributeChanged(uint32 index, const T& val)
  {
    OnAttributeChanged(val);
  }
  
  
	nuiSlotsSink mSink;
	nuiAttrib<const T&> mAttribute;
  
  nglString mName;
  std::vector<std::pair<nglString, T> > mValues;
  nuiComboBox* mpCombo;
  
  bool mIndexed;
  uint32 mIndex;
  
  nuiEventSink<nuiComboAttributeEditor<const T&> > mEventSink;
};



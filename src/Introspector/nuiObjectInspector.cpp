/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

nuiObjectInspector::nuiObjectInspector()
: mSink(this)
{
  SetObjectClass(_T("nuiObjectInspector"));
  
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  mpImage = NULL;
  mpAttributeGrid = NULL;
  
  if (pDeco)
  {
    SetDecoration(pDeco, eDecorationBorder);
  }
  
  UpdateObjects();
  //mSink.Connect(nuiObject::ObjectsChanged, &nuiObjectInspector::OnObjectsChanged);
}

nuiObjectInspector::~nuiObjectInspector()
{  
  
}

void nuiObjectInspector::OnObjectsChanged(const nuiEvent& rEvent)
{
  UpdateObjects();
}

void nuiObjectInspector::UpdateObjects()
{
  Clear();
  
  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  AddChild(pSplitter);
  pSplitter->SetMasterChild(true);
  
  nuiScrollView* pScrollView1 = new nuiScrollView(false, true);
  nuiScrollView* pScrollView2 = new nuiScrollView(false, true);
  pSplitter->AddChild(pScrollView1);
  pSplitter->AddChild(pScrollView2);
  
  nuiList* pList = new nuiList();
  pScrollView1->AddChild(pList);
  
  for (uint32 i = 0; i < nuiObject::GetClassCount(); i++)
  {
    nglString name(GetClassNameFromIndex(i));
    nglString index;
    index.SetCInt(i);
    nuiLabel* pLabel = new nuiLabel(name);
    pLabel->SetProperty(_T("Object"), name);
    pLabel->SetProperty(_T("Index"), index);
    pList->AddChild(pLabel);
  }
  
  /// Attribute list
  mpAttributeGrid = new nuiGrid(2, 0);
  mpAttributeGrid->DisplayGridBorder(true, 1.0f);
  mpAttributeGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);
  
  pScrollView2->AddChild(mpAttributeGrid);
  
  mSink.Connect(pList->SelectionChanged, &nuiObjectInspector::OnObjectSelection, (void*)pList);
}

void nuiObjectInspector::OnObjectSelection(const nuiEvent& rEvent)
{
  nuiList* pList = (nuiList*)rEvent.mpUser;
  nuiWidget* pW = pList->GetSelected();
  nglString info(_T("no info"));
  nglString name;
  uint32 index = -1;
  if (pW)
  {
    name = pW->GetProperty(_T("Object"));
    index = pW->GetProperty(_T("Index")).GetCUInt();
  }
  
  mSlot.DisconnectAll();
  //  mSlot.Connect(pObject->Changed, nuiMakeDelegate(mpLabel, &nuiWidget::Invalidate));
  //  mSlot.Connect(pObject->Changed, nuiMakeDelegate(mpLabel->GetParent(), &nuiWidget::Invalidate));

  if (index < 0)
    return;
  
  // build attributes list
  std::map<nglString, nuiAttributeBase*> attributes;
  nuiObject::GetAttributesOfClass(index, attributes);
  uint32 i = 0;
  std::map<nglString, nuiAttributeBase*>::const_iterator it_a = attributes.begin();
  std::map<nglString, nuiAttributeBase*>::const_iterator end_a = attributes.end();
  
  uint32 rows = mpAttributeGrid->GetNbRows();
  if (rows)
    mpAttributeGrid->RemoveRows(0, rows);
  
  rows = attributes.size();
  if (rows)
  {
    mpAttributeGrid->AddRows(0, rows);
  }
  
  while (it_a != end_a)
  {
    nglString pname(it_a->first);
    //printf("\tattr: %s\n", pname.GetChars());
    //nuiAttribBase Base = it_a->second;
    //nuiAttributeEditor* pEditor = Base.GetEditor();
    mpAttributeGrid->SetCell(0, i, new nuiLabel(pname + nglString(":")));
        
    ++it_a;
    i++;
  }
}


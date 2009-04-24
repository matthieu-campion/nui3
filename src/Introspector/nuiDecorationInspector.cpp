/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nuiDecorationInspector.h"
#include "nuiGrid.h"
#include "nuiToggleButton.h"
#include "nuiLabel.h"
#include "nuiFont.h"
#include "nuiIntrospector.h"
#include "nuiGrid.h"

nuiDecorationInspector::nuiDecorationInspector()
: mSink(this)
{
  SetObjectClass(_T("nuiDecorationInspector"));
  
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  mpLabel = NULL;
  mpInfoLabel = NULL;
  mpAttributeGrid = NULL;
  
  if (pDeco)
  {
    SetDecoration(pDeco, eDecorationBorder);
  }
  
  UpdateDecos();
  //mSink.Connect(nuiDecoration::DecorationsChanged, &nuiDecorationInspector::OnDecosChanged);
}

nuiDecorationInspector::~nuiDecorationInspector()
{  
  
}

bool nuiDecorationInspector::OnDecosChanged(const nuiEvent& rEvent)
{
  UpdateDecos();
  return false;
}

void nuiDecorationInspector::UpdateDecos()
{
  Clear();

  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  AddChild(pSplitter);
  nuiSplitter* pSplit = new nuiSplitter(nuiHorizontal);
  pSplitter->AddChild(pSplit);
  
  nuiScrollView* pScrollView1 = new nuiScrollView(false, true);
  nuiScrollView* pScrollView2 = new nuiScrollView(false, true);
  pSplit->AddChild(pScrollView1);
  pSplit->AddChild(pScrollView2);
  
  nuiList* pList = new nuiList();
  pScrollView1->AddChild(pList);
  
  const nuiDecoration::DecorationMap& rMap(nuiDecoration::Enum());
  nuiDecoration::DecorationMap::const_iterator it = rMap.begin();
  nuiDecoration::DecorationMap::const_iterator end = rMap.end();
  while (it != end)
  {
    nglString name(it->first);
    nuiLabel* pLabel = new nuiLabel(name);
    pLabel->SetProperty(_T("Deco"), name);
    pList->AddChild(pLabel);
    ++it;
  }
  
  /// Attribute list
  mpAttributeGrid = new nuiGrid(2, 0);
  mpAttributeGrid->DisplayGridBorder(true, 1.0f);
  mpAttributeGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);

  pScrollView2->AddChild(mpAttributeGrid);
  
  
  nuiSimpleContainer* pCont = new nuiSimpleContainer();
  pSplitter->AddChild(pCont);
  mpLabel = new nuiLabel(_T("Test Widget.\nApply decorations\nto me, from the list\non the left"), nuiFont::GetFont(16));
  mpLabel->SetPosition(nuiCenter);
  pCont->AddChild(mpLabel);

  mpInfoLabel = new nuiLabel(_T("no info"), nuiFont::GetFont(8));
  mpInfoLabel->SetPosition(nuiTopLeft);
  mpInfoLabel->SetBorder(10, 10);
  pCont->AddChild(mpInfoLabel);
  
  mSink.Connect(pList->SelectionChanged, &nuiDecorationInspector::OnDecoSelection, (void*)pList);
}

bool nuiDecorationInspector::OnDecoSelection(const nuiEvent& rEvent)
{
  nuiList* pList = (nuiList*)rEvent.mpUser;
  nuiWidget* pW = pList->GetSelected();
  nuiDecoration* pDeco = NULL;
  nglString info(_T("no info"));
  if (pW)
  {
    nglString name = pW->GetProperty(_T("Deco"));
    pDeco = nuiDecoration::Get(name);
    info = nglString::Empty;
    info.Add(name).AddNewLine().Add(pDeco->GetObjectClass());
  }
  mpLabel->SetDecoration(pDeco);
  mpInfoLabel->SetText(info);

  // build attributes list
  std::map<nglString, nuiAttribBase> attributes;
  pDeco->GetAttributes(attributes);
  uint32 i = 0;
  std::map<nglString, nuiAttribBase>::const_iterator it_a = attributes.begin();
  std::map<nglString, nuiAttribBase>::const_iterator end_a = attributes.end();
  
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
    //printf("\tattr: %ls\n", pname.GetChars());
    nuiAttribBase Base = it_a->second;
    nuiAttributeEditor* pEditor = Base.GetEditor();
    mpAttributeGrid->SetCell(0, i, new nuiLabel(pname + nglString(":")));
    
    if (!pEditor)
    {
      nglString value;
      switch (Base.GetDimension())
      {
        case 0:
          Base.ToString(value);
          break;
        case 1:
        {
          nglString str;
          uint32 count = Base.GetIndexRange(0);
          for (uint32 i = 0; i < count; i++)
          {
            Base.ToString(i, str);
            value.Add(i).Add(_T(":")).Add(str).Add("\t");
          }
          value.Trim(_T('\t'));
        }
          break;
        case 2:
        {
          nglString str;
          uint32 counti = Base.GetIndexRange(0);
          uint32 countj = Base.GetIndexRange(1);
          for (uint32 i = 0; i < MIN(10, counti); i++)
          {
            for (uint32 j = 0; j < MIN(10, countj); j++)
            {
              Base.ToString(i, j, str);
              value.Add(i).Add(",").Add(j).Add(_T(":")).Add(str).Add("\t");
            }
          }
          value.Trim(_T('\t'));
        }
          break;
      }
      nuiLabel* pLabel = new nuiLabel(value);
      mpAttributeGrid->SetCell(1, i, pLabel, nuiLeft);
    }
    else
    {
      mpAttributeGrid->SetCell(1, i, pEditor, nuiFill);
    }
    
    ++it_a;
    i++;
  }
  
  
  return false;
}

/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

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
}

nuiDecorationInspector::~nuiDecorationInspector()
{
  mSlot.DisconnectAll();
}

void nuiDecorationInspector::OnDecosChanged(const nuiEvent& rEvent)
{
  UpdateDecos();
}

void nuiDecorationInspector::UpdateDecos()
{
  Clear();

  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  AddChild(pSplitter);
  nuiSplitter* pSplit = new nuiSplitter(nuiHorizontal);
  pSplitter->AddChild(pSplit);
  pSplit->SetMasterChild(false);
  
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
  
  nuiSplitter* pSplit2 = new nuiSplitter(nuiHorizontal);
  pSplit2->SetMasterChild(false);
  pSplitter->AddChild(pSplit2);
  nuiSimpleContainer* pCont = new nuiSimpleContainer();
  pSplit2->AddChild(pCont);
  mpLabel = new nuiLabel(_T("Test Widget.\nApply decorations\nto me, from the list\non the left"), nuiFont::GetFont(16));
  mpLabel->SetPosition(nuiCenter);
  pCont->AddChild(mpLabel);

  mpInfoLabel = new nuiLabel(_T("no info"), nuiFont::GetFont(8));
  mpInfoLabel->SetPosition(nuiTopLeft);
  mpInfoLabel->SetBorder(10, 10);
  pCont->AddChild(mpInfoLabel);
  
  nuiScrollView* pScrollCSS = new nuiScrollView();
  mpCSSLabel = new nuiText();
  pScrollCSS->AddChild(mpCSSLabel);
  pSplit2->AddChild(pScrollCSS);
  
  mSink.Connect(pList->SelectionChanged, &nuiDecorationInspector::OnDecoSelection, (void*)pList);
}

void nuiDecorationInspector::OnDecoSelection(const nuiEvent& rEvent)
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

  mSlot.DisconnectAll();
  mSlot.Connect(pDeco->Changed, nuiMakeDelegate(mpLabel, &nuiWidget::Invalidate));
  mSlot.Connect(pDeco->Changed, nuiMakeDelegate(mpLabel->GetParent(), &nuiWidget::Invalidate));
  mSlot.Connect(pDeco->Changed, nuiMakeDelegate(this, &nuiDecorationInspector::DumpDecl));
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
    //printf("\tattr: %s\n", pname.GetChars());
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
  
  DumpDecl();
}

void nuiDecorationInspector::DumpDecl()
{
  nuiDecoration* pDeco = mpLabel->GetDecoration();
  if (pDeco)
  {
    nglString decl = pDeco->GetCSSDeclaration();
    mpCSSLabel->SetText(decl);
    NGL_OUT(_T("Decoration Declaration:\n%s\n"), decl.GetChars());
  }
}

void nuiDecorationInspector::OnTrash()
{
  mSlot.DisconnectAll();
}

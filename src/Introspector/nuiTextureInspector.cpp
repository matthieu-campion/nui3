/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

nuiTextureInspector::nuiTextureInspector()
: mSink(this)
{
  SetObjectClass(_T("nuiTextureInspector"));
  
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  mpImage = NULL;
  mpAttributeGrid = NULL;
  
  if (pDeco)
  {
    SetDecoration(pDeco, eDecorationBorder);
  }
  
  UpdateTextures();
  //mSink.Connect(nuiTexture::TexturesChanged, &nuiTextureInspector::OnTexturesChanged);
}

nuiTextureInspector::~nuiTextureInspector()
{  
  
}

void nuiTextureInspector::OnTexturesChanged(const nuiEvent& rEvent)
{
  UpdateTextures();
}

void nuiTextureInspector::UpdateTextures()
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
  
  const nuiTextureMap& rMap(nuiTexture::Enum());
  nuiTextureMap::const_iterator it = rMap.begin();
  nuiTextureMap::const_iterator end = rMap.end();
  while (it != end)
  {
    nglString name(it->first);
    nuiLabel* pLabel = new nuiLabel(name);
    pLabel->SetProperty(_T("Texture"), name);
    pList->AddChild(pLabel);
    ++it;
  }
  
  /// Attribute list
  mpAttributeGrid = new nuiGrid(2, 0);
  mpAttributeGrid->DisplayGridBorder(true, 1.0f);
  mpAttributeGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);
  
  pScrollView2->AddChild(mpAttributeGrid);

  nuiSimpleContainer* pCont = new nuiSimpleContainer();
  nuiColorDecoration* pColDec = new nuiColorDecoration(_T("TextureInspectorBg"), nuiColor(128, 128, 128), 0, nuiColor(0, 0, 0), eFillShape, nuiBlendTransp, nuiRect(0,0,0,0));
  pCont->SetDecoration(pColDec);
  pSplitter->AddChild(pCont);
  mpImage = new nuiImage();
  mpImage->SetPosition(nuiCenter);
  pCont->AddChild(mpImage);
  
  mSink.Connect(pList->SelectionChanged, &nuiTextureInspector::OnTextureSelection, (void*)pList);
}

void nuiTextureInspector::OnTextureSelection(const nuiEvent& rEvent)
{
  nuiList* pList = (nuiList*)rEvent.mpUser;
  nuiWidget* pW = pList->GetSelected();
  nuiTexture* pTexture = NULL;
  nglString info(_T("no info"));
  if (pW)
  {
    nglString name = pW->GetProperty(_T("Texture"));
    pTexture = nuiTexture::GetTexture(name);
    if (pTexture)
      mpImage->SetTexture(pTexture);
  }
  
  mSlot.DisconnectAll();
//  mSlot.Connect(pTexture->Changed, nuiMakeDelegate(mpLabel, &nuiWidget::Invalidate));
//  mSlot.Connect(pTexture->Changed, nuiMakeDelegate(mpLabel->GetParent(), &nuiWidget::Invalidate));
  
  // build attributes list
  std::map<nglString, nuiAttribBase> attributes;
  if (pTexture)
    pTexture->GetAttributes(attributes);
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
}


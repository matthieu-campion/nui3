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


nuiDecorationInspector::nuiDecorationInspector()
: mSink(this)
{
  SetObjectClass(_T("nuiDecorationInspector"));
  
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  mpLabel = NULL;
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

  nuiScrollView* pScrollView = new nuiScrollView();
  pSplitter->AddChild(pScrollView);
  
  nuiList* pList = new nuiList();
  pScrollView->AddChild(pList);
  
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
  
  nuiSimpleContainer* pCont = new nuiSimpleContainer();
  pSplitter->AddChild(pCont);
  mpLabel = new nuiLabel(_T("Test Widget.\nApply decorations\nto me, from the list\non the left"), nuiFont::GetFont(16));
  mpLabel->SetPosition(nuiCenter);
  pCont->AddChild(mpLabel);

  mSink.Connect(pList->SelectionChanged, &nuiDecorationInspector::OnDecoSelection, (void*)pList);
}

bool nuiDecorationInspector::OnDecoSelection(const nuiEvent& rEvent)
{
  nuiList* pList = (nuiList*)rEvent.mpUser;
  nuiWidget* pW = pList->GetSelected();
  nuiDecoration* pDeco = NULL;
  if (pW)
  {
    nglString name = pW->GetProperty(_T("Deco"));
    pDeco = nuiDecoration::Get(name);
  }
  mpLabel->SetDecoration(pDeco);
  
  return false;
}

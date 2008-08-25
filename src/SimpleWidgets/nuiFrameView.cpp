/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiFrameView.h"


////////////////// nuiFrameView
//class nuiFrameView : public nuiSimpleContainer
nuiFrameView::nuiFrameView(nuiFrame* pFrame)
: nuiSimpleContainer()
{
  mpFrame = pFrame;
}

nuiFrameView::~nuiFrameView()
{
  delete mpFrame;
}

bool nuiFrameView::Load(const nuiXMLNode* pNode)
{
  mpFrame = new nuiFrame(pNode->GetAttribute(_T("Name")));
  
  uint count = pNode->GetChildrenCount();
  
  for (uint i = 0; i < count; i++)
  {
    nuiXMLNode* pChildNode = pNode->GetChild(i);
    
    if (pChildNode->GetName() == _T("nuiFrame"))
    {
      nuiFrame* pFrame = new nuiFrame(pNode->GetAttribute(_T("Name")));
      pFrame->Load(pChildNode);
    }
    else
      nuiBuilder::Get().CreateWidget(pChildNode->GetName());
  }
  
  return true;
}

nuiRect nuiFrameView::CalcIdealSize()
{
  if (mpFrame)
  {
    nuiRect ideal = nuiSimpleContainer::CalcIdealSize();
    ideal.Grow(mpFrame->GetBorder(nuiFillHorizontal) * 0.5, mpFrame->GetBorder(nuiFillVertical) * 0.5);
    ideal.MoveTo(0, 0);
    ideal.RoundToNearest();
    return ideal;
  }

  return nuiSimpleContainer::CalcIdealSize();
}

bool nuiFrameView::SetRect(const nuiRect& rRect)
{
  nuiRect r(rRect.Size());
  if (mpFrame)
    mpFrame->GlobalToClientRect(r);
  nuiWidget::SetRect(rRect);
  
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (mCanRespectConstraint)
      pItem->SetLayoutConstraint(mConstraint);
    pItem->GetIdealRect();
    r.RoundToSmallest();
    pItem->SetLayout(r);
  }
  delete pIt;
  
  DebugRefreshInfo();  
  return true;
}

bool nuiFrameView::Draw(nuiDrawContext* pContext)
{
  if (mpFrame)
    mpFrame->DrawBack(pContext, this, GetRect().Size());
  DrawChildren(pContext);
  if (mpFrame)
    mpFrame->DrawFront(pContext, this, GetRect().Size());
  return true;
}


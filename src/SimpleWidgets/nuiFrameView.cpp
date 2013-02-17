/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


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

nuiRect nuiFrameView::CalcIdealSize()
{
  if (mpFrame)
  {
    nuiRect ideal = nuiSimpleContainer::CalcIdealSize();
    ideal.Grow(mpFrame->GetBorder(nuiFillHorizontal, this) * 0.5, mpFrame->GetBorder(nuiFillVertical, this) * 0.5);
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
    mpFrame->GlobalToClientRect(r, this);
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


/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiFixed::nuiFixed()
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiFixed"));
}

nuiFixed::~nuiFixed()
{
}

nuiRect nuiFixed::CalcIdealSize()
{
  mIdealRect = nuiRect();
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    mIdealRect.Union(mIdealRect,pItem->GetIdealRect());
  }
  delete pIt;

  mIdealRect.mLeft = 0;
  mIdealRect.mTop = 0;
  return mIdealRect;
}

bool nuiFixed::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect rect = pItem->GetIdealRect();
    pItem->SetLayout(rect);
  }
  delete pIt;

  return true;

}

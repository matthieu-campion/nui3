/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiAnimView::nuiAnimView()
: nuiSimpleContainer()
{
  nuiSimpleContainer::SetObjectClass(_T("nuiAnimView"));
  SetDuration(5);
}

nuiAnimView::~nuiAnimView()
{
}

bool nuiAnimView::Draw(nuiDrawContext* pContext)
{
  nuiWidget* pItem = NULL;

  uint32 count = (int32)mpChildren.size();
  if (!count)
    return true;

  if (count)
  {
    count--;
    UpdateTime();
  }
  double value = nuiAnimation::GetPosition() * GetChildrenCount();
  count = ToBelow(value);

  nuiWidgetList::iterator it = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  for ( ; count && it != end; count--)
    ++it;

  if (it == end)
    return true;

  pItem = *it;

  if (pItem)
    DrawChild(pContext, pItem);

  return true;
}

void nuiAnimView::OnFrame()
{
  Invalidate();
}

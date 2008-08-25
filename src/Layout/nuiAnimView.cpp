/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAnimView.h"
#include "nuiXML.h"
#include "nglMath.h"

nuiAnimView::nuiAnimView()
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiAnimView"));
}

bool nuiAnimView::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  return true;
}

nuiAnimView::~nuiAnimView()
{
}

nuiXMLNode* nuiAnimView::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode);
  nuiAnimation::Serialize(pNode, false); // Add the animation parameters...

  return pNode;
}


bool nuiAnimView::Draw(nuiDrawContext* pContext)
{
  nuiWidget* pItem = NULL;

  long count = mpChildren.size();
  if (!count)
    return true;

  if (mCount)
  {
    count--;
    UpdateTime();
  }
  double value = TimeToFrame(GetTime());
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

double nuiAnimView::GetFrameCount()
{
  return (double)GetChildrenCount();
}


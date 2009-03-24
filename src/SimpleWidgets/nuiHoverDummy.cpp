/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiHoverDummy.h"
#include "nuiXML.h"

nuiHoverDummy::nuiHoverDummy(nuiWidgetPtr pHoverOn, nuiWidgetPtr pHoverOff)
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiHoverDummy"));
  mpHover[1] = pHoverOn;
  mpHover[0] = pHoverOff;
  int i;
  for (i=0; i < 2; i++)
    if (mpHover[i])
      AddChild(mpHover[i]);
}

bool nuiHoverDummy::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiHoverDummy"));

  int i;
  for (i=0; i < 2; i++)
    mpHover[i] = NULL;

  mpHover[0] = GetChild(pNode->GetAttribute(_T("HoverOffChild")),false);
  mpHover[1] = GetChild(pNode->GetAttribute(_T("HoverOnChild")),false);
  
  return true;
}

nuiXMLNode* nuiHoverDummy::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode, Recursive);

  if (mpHover[0])
    pNode->SetAttribute(_T("HoverOffChild"), mpHover[0]->GetObjectName());
  if (mpHover[1])
    pNode->SetAttribute(_T("HoverOnChild"),  mpHover[1]->GetObjectName());

  return pNode;
}

nuiHoverDummy::~nuiHoverDummy()
{
}

void nuiHoverDummy::SetDummy(bool Hover, nuiWidgetPtr pWidget)
{
  nuiWidgetPtr pOld = mpHover[Hover?1:0];
  mpHover[Hover?1:0] = pWidget;
  if (pWidget)
    AddChild(pWidget);
  if (pOld)
  {
    // Destroy the widget if it is not used anymore here:
    int i;
    for (i = 0; i < 2 && mpHover[i] != pOld; i++) ;
    if (i == 2)
      pOld->Trash();
  }
  if (Hover == GetHover())
    InvalidateLayout();
}

nuiWidgetPtr nuiHoverDummy::GetDummy(bool Hover)
{
  return mpHover[GetHover()?1:0];
}

bool nuiHoverDummy::Draw(nuiDrawContext* pContext)
{
  nuiWidgetPtr pWidget = mpHover[GetHover()?1:0];
//  NGL_OUT(_T("Draw %d %p\n"), GetHover(), pWidget);
  if (pWidget)
    DrawChild(pContext, pWidget);
  return true;
}

nuiRect nuiHoverDummy::CalcIdealSize()
{
  nuiWidgetPtr pWidget = mpHover[GetHover()?1:0];
  if (pWidget)
    mIdealRect = pWidget->GetIdealRect();
  else
    mIdealRect = GetRect();
  return mIdealRect;
}

bool nuiHoverDummy::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  nuiWidgetPtr pWidget = mpHover[GetHover()?1:0];
  if (pWidget)
    pWidget->SetLayout(rRect.Size());
  return true;
}

void nuiHoverDummy::SetHover(bool hover)
{
  if (hover == GetHover())
    return;
  nuiSimpleContainer::SetHover(hover);
  InvalidateLayout();
}

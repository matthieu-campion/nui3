/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiFixed.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"

nuiFixed::nuiFixed()
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiFixed"));
}

bool nuiFixed::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiFixed"));
  // Nothing to do for fixed widget...
  return true;
}

nuiXMLNode* nuiFixed::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;
  return pNode;
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

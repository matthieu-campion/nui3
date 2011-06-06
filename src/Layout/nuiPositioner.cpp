/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiPositioner.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"

nuiPositioner::nuiPositioner(nuiPosition Position, bool expandWidth, bool expandHeight)
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiPositioner"));
  mPPosition = Position;
  mLimitBounds = true;
  mCanRespectConstraint = true;
  mExpandWidth = expandWidth;
  mExpandHeight = expandHeight;
}

bool nuiPositioner::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiPositioner"));
  // Nothing to do for fixed widget...
  mPPosition = nuiGetPosition(pNode, _T("Position"), nuiCenter);
  mLimitBounds = nuiGetBool(pNode, _T("LimitBounds"), true);
  mCanRespectConstraint = true;
  
  return true;
}

nuiXMLNode* nuiPositioner::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode,true);
  if (!pNode)
    return NULL;
  return pNode;
}

nuiPositioner::~nuiPositioner()
{
}

nuiRect nuiPositioner::CalcIdealSize()
{
  nuiRect rect;
  IteratorPtr pIt;
  //NGL_OUT(_T("nuiPositioner CalcIdealSize\n"));
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect wrect(pItem->GetIdealRect());
    //NGL_OUT(_T("  WRect %s\n"), wrect.GetValue().GetChars());
    rect.Union(rect,wrect.Size());
  }
  delete pIt;

  //NGL_OUT(_T("nuiPositioner Ideal Size: %d x %d\n"),(int)rect.GetWidth(),(int)rect.GetHeight());
  return rect.Size();
}

bool nuiPositioner::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  //NGL_OUT(_T("nuiPositioner::SetRect: %d x %d\n"),(int)rRect.GetWidth(),(int)rRect.GetHeight());

  nuiWidget::LayoutConstraint contraint;
  contraint.mMaxWidth = rRect.GetWidth();
  contraint.mMaxHeight = rRect.GetHeight();

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->SetLayoutConstraint(contraint);
  }
  delete pIt;

  mIdealRect = CalcIdealSize();

  nuiRect mainrect(rRect.Size());

  pIt = NULL;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect rect = pItem->GetIdealRect().Size();
    if (mExpandWidth)
      rect.SetSize(mainrect.GetWidth(), rect.GetHeight());
    if (mExpandHeight)
      rect.SetSize(rect.GetWidth(), mainrect.GetHeight());

    rect.SetPosition(mPPosition, mainrect);
    if (mLimitBounds)
      rect.Intersect(rect, mainrect);
    rect.RoundToBiggest();
    pItem->SetLayout(rect);
  }
  delete pIt;

  return true;
}


bool nuiPositioner::Draw(nuiDrawContext* pContext)
{
  return DrawChildren(pContext);
}

void nuiPositioner::SetLimitBounds(bool limitbounds)
{
  mLimitBounds = limitbounds;
  InvalidateLayout();
}

bool nuiPositioner::GetLimitBounds() const
{
  return mLimitBounds;
}


void nuiPositioner::Invalidate()
{
  // Get the rect of all the children:
  nuiRect rect;
  IteratorPtr pIt;
  //NGL_OUT(_T("nuiPositioner CalcIdealSize\n"));
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect wrect(pItem->GetIdealRect());
    //NGL_OUT(_T("  WRect %s\n"), wrect.GetValue().GetChars());
    rect.Union(rect,wrect.Size());
  }
  delete pIt;

  rect.SetPosition(mPPosition, GetRect().Size());

  nuiWidget::InvalidateRect(rect);
  SilentInvalidate();

  if (mpParent)
    ((nuiPositioner*)mpParent)->BroadcastInvalidate(this); //#HACK!
  DebugRefreshInfo();
}

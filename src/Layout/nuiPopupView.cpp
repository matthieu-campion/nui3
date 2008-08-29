/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiPopupView.h"

nuiPopupView::nuiPopupView(nuiWidget* pWidget, nuiSize X, nuiSize Y)
{
  SetObjectClass(_T("nuiPopupView"));
  mX = X;
  mY = Y;
  pWidget->LocalToGlobal(mX, mY);

  nuiTopLevel* pTop = NULL;
  if (pWidget)
    pTop = pWidget->GetTopLevel();
  if (pTop)
    pTop->AddChild(this);

  SetWantKeyboardFocus(true);
  
  GetTopLevel()->CancelGrab();
}

nuiPopupView::~nuiPopupView()
{
  
}

bool nuiPopupView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  
  nuiRect Rect(GetIdealRect());
  nuiRect ParentRect(mpParent->GetRect().Size());
  
  nuiSize x, y;
  x = mX;
  y = mY;
  
  if (x + Rect.GetWidth() > ParentRect.GetWidth())
    x = ParentRect.GetWidth() - Rect.GetWidth();
  
  if (y + Rect.GetHeight() > ParentRect.GetHeight())
    y = ParentRect.GetHeight() - Rect.GetHeight();
  
  mChildrenRect = Rect;
  mChildrenRect.MoveTo(x, y);
  
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->SetRect(mChildrenRect);
  }
  delete pIt;
  
  return true;
}

bool nuiPopupView::Draw(nuiDrawContext* pContext)
{
  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);
  pTheme->DrawWindowShade(pContext, mChildrenRect, true);
  pTheme->Release();
  
  SetFillColor(pContext, eActiveWindowBg);
  SetStrokeColor(pContext, eActiveWindowFg);
  
  pContext->DrawRect(mChildrenRect, eStrokeAndFillShape);
  
  return DrawChildren(pContext);
}

bool nuiPopupView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mChildrenRect.IsInside(X, Y) && !IsTrashed())
  {
    Trash();
  }
  return true;
}

bool nuiPopupView::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mChildrenRect.IsInside(X, Y) && !IsTrashed())
  {
    Trash();
  }
  return true;
}

bool nuiPopupView::KeyUp(const nglKeyEvent& rEvent)
{
  if (!IsTrashed())
  {
    Trash();
  }
  return true;
}

bool nuiPopupView::KeyDown(const nglKeyEvent& rEvent)
{
  if (!IsTrashed())
  {
    Trash();
  }
  return true;
}

void nuiPopupView::SetPos(nuiSize X, nuiSize Y)
{
  mX = X;
  mY = Y;
  UpdateLayout();
}

void nuiPopupView::ConnectTopLevel()
{
  Grab();
  Focus();
  nuiSimpleContainer::ConnectTopLevel();
}


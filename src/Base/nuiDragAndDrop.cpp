/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#if 0 // Currently disabled!

#include "nuiDragAndDrop.h"
#include "nuiMainWindow.h"

using namespace std;

nuiDragAndDrop::nuiDragAndDrop(nuiWidgetPtr pSource, nuiSize X, nuiSize Y)
: nuiSimpleContainer(pSource ? pSource->GetTopLevel() : NULL)
{
  SetProperty(_T("Class"),_T("nuiDragAndDrop"));
  UnsetUserSize();
  mpSource = pSource;
  SetAlpha(.5f);
  Grab();
  mPosX = X;
  mPosY = Y;
}

nuiDragAndDrop::~nuiDragAndDrop()
{
  nuiDragObjectMap::iterator it;
  nuiDragObjectMap::iterator end = mpElements.end();;
  for (it = mpElements.begin(); it != end; ++it)
  {
    if ((*it).second)
      delete (*it).second;
  }
}

nuiRect nuiDragAndDrop::CalcIdealSize()
{
  nuiRect rect;
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    rect.Union(rect,pItem->GetIdealRect());
  }
  delete pIt;

  mIdealRect.Set(0.0f, 0.0f, rect.GetWidth(), rect.GetHeight());
  return mIdealRect;
}

bool nuiDragAndDrop::SetRect(const nuiRect& rRect)
{
  nuiSimpleContainer::SetRect(rRect);
  nuiRect rect;
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    rect = pItem->GetIdealRect();
    rect.MoveTo(mPosX+10, mPosY + 10);
    pItem->SetLayout(rect);
  }
  delete pIt;

  return true;
}

void nuiDragAndDrop::SetSource(nuiWidgetPtr pSource)
{
  mpSource = pSource;
}

nuiWidgetPtr nuiDragAndDrop::GetSource()
{
  return mpSource;
}

void nuiDragAndDrop::SetDestination(nuiWidgetPtr pDest)
{
  mpDestination = pDest;
}

nuiWidgetPtr nuiDragAndDrop::GetDestination()
{
  return mpDestination;
}

void nuiDragAndDrop::AddType(const nglString& rType)
{
  mpElements[rType] = NULL;
}

bool nuiDragAndDrop::HasType(const nglString& rType)
{
  return (mpElements.find(rType) != mpElements.end());
}


void nuiDragAndDrop::SetData(const nglString& rType, nuiObjectPtr pData)
{
  mpElements[rType] = pData;
}

nuiObjectPtr nuiDragAndDrop::GetData(const nglString& rType)
{
  if (!HasType(rType))
    return NULL;
  return mpElements[rType];
}

bool nuiDragAndDrop::MouseClicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return true;
}

bool nuiDragAndDrop::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  Ungrab();
  nuiSize x,y;

  x = X;
  y = Y;
  LocalToGlobal(x,y);
  SetUserPos(x+10,y+10);
  nuiWidgetPtr pWidget = GetRoot()->GetChild(x,y);
  if (pWidget)
  {
    pWidget->GlobalToLocal(x,y);
    if (pWidget->OnCanDrop(this,x,y))
    {
      SetDestination(pWidget);
      mpSource->OnDragged(this);
      mpDestination->OnDropped(this,x,y,Button);
    }
    else
    {
      SetDestination(NULL);
      mpSource->OnCancelDrag(this);
    }

  }

  SetMouseCursor(nglWindow::eCursorArrow);
  Trash();
  return true;
}

bool nuiDragAndDrop::MouseMoved(nuiSize X, nuiSize Y)
{
  nuiSize x,y;
  x = X;
  y = Y;
  LocalToGlobal(x,y);
  mPosX = x;
  mPosY = y;

  nuiWidgetPtr pWidget = GetRoot()->GetChild(x,y);
  if (pWidget)
  {
    nuiSize x,y;
    x = (nuiSize)X;
    y = (nuiSize)Y;
    pWidget->GlobalToLocal(x,y);
    if (pWidget->OnCanDrop(this,x,y))
    {
      SetDestination(pWidget);
      SetMouseCursor(nglWindow::eCursorDnD);
    }
    else
    {
      SetDestination(NULL);
      SetMouseCursor(nglWindow::eCursorForbid);
    }
  }

  Invalidate();
  return true;
}


bool nuiDragAndDrop::IsInsideLocal(nuiSize X, nuiSize Y)
{
  // Let's pretend don't exist :).
  return false;
}

bool nuiDragAndDrop::IsInside(nuiSize X, nuiSize Y)
{
  // Let's pretend don't exist :).
  return false;
}

#endif // _DEBUG

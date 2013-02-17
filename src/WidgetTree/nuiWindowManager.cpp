/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiWindowManager::nuiWindowManager()
: nuiSimpleContainer(),
  mWMSink(this)
{
  SetObjectClass(_T("nuiWindowManager"));
}

nuiWindowManager::~nuiWindowManager()
{
}

bool nuiWindowManager::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_TAB)
  {
    if (IsKeyDown(NK_LCTRL) || IsKeyDown(NK_LCTRL))
    {
      if (mpChildren.front() && mpChildren.size())
      { 
        //        if (!(((nuiWindow*)mpChildren.front())->IsModal()))
        {
          nuiWidgetList::iterator it;
          nuiWidgetList::iterator end = mpChildren.begin();

          it = mpChildren.end();
          it--;
          for (; it!=end; it--)
          {
            if (*it)
            {
              if ((*it)->IsVisible())
              {
                ActivateWindow((nuiWindow*)*it);
                return true;
              }
            }
          }
          if (*it)
          {
            if ((*it)->IsVisible())
            {
              ActivateWindow((nuiWindow*)*it);
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool nuiWindowManager::KeyUp(const nglKeyEvent& rEvent)
{
  //OUT("OnKeyUp\n");
  if (rEvent.mKey == NK_TAB)
  {
    if (IsKeyDown(NK_LCTRL) || IsKeyDown(NK_LCTRL))
    {
      /*
      if (mpChildren.front() && mpChildren.size())
      if (!(((nuiWindow*)mpChildren.front())->IsModal()))
      return true; // eat this event...
      */
    }
  }

  return false;
}

bool nuiWindowManager::DispatchMouseUnclick(const nglMouseInfo& rInfo)
{
  //OUT("OnMouseUnclick\n");
  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsVisible() && pItem->IsEnabled())
    {
      if (pItem->DispatchMouseUnclick(rInfo))
        return true;
    }
  }
  delete pIt;

  return nuiSimpleContainer::DispatchMouseUnclick(rInfo);
}

bool nuiWindowManager::Draw(class nuiDrawContext *pContext)
{
  // Draw the windows:
  DrawChildren(pContext);
  return true;
}

nuiWidgetPtr nuiWindowManager::GetChild(nuiSize X, nuiSize Y)
{
  nuiWidgetPtr pChild = NULL;
  nuiWindow* pWin = GetWindow(X, Y);
  if (pWin)
    pChild = pWin->GetChild(X, Y);
  if (!pChild) 
    pChild = nuiSimpleContainer::GetChild(X, Y);
  return pChild;
}

nuiWindow* nuiWindowManager::GetActiveWindow() const
{
  if (!mWindows.empty())
  {
    return (mWindows.back());
  }
  else
  {
    return NULL;
  }
}

bool nuiWindowManager::ActivateWindow(nuiWindow* pWin)
{
  nuiTopLevel* pRoot = GetTopLevel();
  NGL_ASSERT(pRoot != NULL);

  pRoot->CancelGrab();
  pRoot->SetFocus(NULL);

  mWindows.remove(pWin);
  mWindows.push_back(pWin);

  RaiseChildToFront(pWin);
  pWin->Invalidate();
  Invalidate();
  return true;
}


nuiWindow* nuiWindowManager::GetWindow(nuiSize X, nuiSize Y, bool ClientAreaOnly)
{
  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideFromParent(X,Y))
    {
      delete pIt;
      return (nuiWindow*)pItem;
    }
  }
  delete pIt;

  return NULL;
}

uint nuiWindowManager::GetWindowCount() const
{
  return mpChildren.size();
}

nuiWindow* nuiWindowManager::GetWindow(uint index)
{
  if (mpChildren.size()<index)
    return NULL;

  nuiWidgetList::const_iterator it;
  nuiWidgetList::const_iterator end = mpChildren.end();

  for (it=mpChildren.begin(); it!=end; ++it)
  {
    if (!index)
      return (nuiWindow*)(*it);

    index--;
  }

  return NULL;
}

bool nuiWindowManager::ValidateWindowRect(nuiRect& rRect) const
{
  bool res = true;
  
  if (rRect.mTop > (mRect.GetHeight() - 10))
  {
    rRect.Move(0, mRect.GetHeight() - 10 - rRect.mTop);
    res = false;
  }
  if (rRect.mTop < 0)
  {
    rRect.Move(0, - rRect.mTop);
    res = false;
  }
  
  if (rRect.mLeft > (mRect.GetWidth() - 10))
  {
    rRect.Move(mRect.GetWidth() - 10 - rRect.mLeft, 0);
    res = false;
  }
  if (rRect.mRight < 10)
  {
    rRect.Move(10 - rRect.mRight, 0);
    res = false;
  }
  return res;
}

bool nuiWindowManager::IsParentActive() const
{
  return true;
}

nuiRect nuiWindowManager::CalcIdealSize()
{
  nuiRect IdealRect;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();

  for (it=mpChildren.begin(); it!=end; ++it)
  {
    nuiWidget* pItem = *it;
    IdealRect.Union(IdealRect, pItem->GetIdealRect());
  }

  mIdealRect = IdealRect;
  return mIdealRect;
}

bool nuiWindowManager::SetRect(const nuiRect& rRect)
{
  //NGL_OUT(_T("nuiWindowManager::SetRect\n"));
  nuiWidget::SetRect(rRect);

  {
    std::list<nuiWindow*>::iterator it;
    std::list<nuiWindow*>::iterator end = mWindows.end();
    for (it = mWindows.begin(); it!=end; ++it)
    {
      nuiWindow* win = *it;

      if (win->GetPosition() == nuiNoPosition)
      {
        nuiRect rect = win->GetIdealRect();
        win->SetLayout(rect);
      }
      else
      {
        win->SetLayout(rRect);
      }
    }
  }

  return true;
}

bool nuiWindowManager::IsInsideFromSelf(nuiSize X, nuiSize Y)
{
  return GetWindow(X,Y) != NULL;
}

bool nuiWindowManager::AddChild(nuiWidgetPtr pChild)
{
  bool res = nuiSimpleContainer::AddChild(pChild);
  if (res)
  {
    nuiWindow* pWin = (nuiWindow*)pChild;
    mWindows.push_back(pWin);
    pChild->InvalidateLayout();
    InvalidateLayout();
  }
  return res;
}

bool nuiWindowManager::DelChild(nuiWidgetPtr pChild)
{
  nuiWindowPtr pWin = dynamic_cast<nuiWindow*>(pChild);
  if (pWin)
  {
    std::list<nuiWindow*>::iterator it;
    std::list<nuiWindow*>::iterator end = mWindows.end();
    for (it = mWindows.begin(); it!=end; ++it)
    {
      nuiWindow* win = *it;
      if (win == pChild)
      {
        mWindows.erase(it);
        break;
      }
    }
  }

  return nuiSimpleContainer::DelChild(pChild);
}

bool nuiWindowManager::DispatchMouseClick(const nglMouseInfo& rInfo)
{
  if (!IsEnabled())
    return false;

  if (IsInsideFromRoot(rInfo.X, rInfo.Y))
  {
    if (!mWindows.empty())
    {
      std::list<nuiWindow*>::iterator it;
      std::list<nuiWindow*>::iterator begin = mWindows.begin();
      it = mWindows.end();
      do 
      {
        it--;
        nuiWindow* win = *it;
        if (win->IsInsideFromRoot(rInfo.X, rInfo.Y))
        {
          if (GetActiveWindow() != win)
            ActivateWindow(win);
          break;
        }
      } while (it!=begin);
    }

    IteratorPtr pIt;
    for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (pItem && pItem->IsVisible() && pItem->IsEnabled())
      {
        if (pItem->DispatchMouseClick(rInfo))
        {
          delete pIt;
          return true;
        }
      }
    }
    delete pIt;

    nglMouseInfo info(rInfo);
    GlobalToLocal(info.X, info.Y);
    bool ret = MouseClicked(info);
    ret |= Clicked(info);
    return ret;
  }
  return false;
}

/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/




#include "nui.h"



nuiTabView::nuiTabView(nuiPosition tabPosition, bool decoratedBackground)
: nuiSimpleContainer(), mTabViewEvents(this),
  mCurrentTabIndex(0), mTabPosition(tabPosition), mDecoratedBackground(decoratedBackground),
mChangeOnDrag(false)
{
  SetObjectClass(_T("nuiTabView"));
  mChildrenRectUnion = true;
  mFoldable = false;
  mFolded = false;
  mSliding = false;
  mFoldRatio = 0.0f;
}

nuiTabView::~nuiTabView()
{
}

nuiRect nuiTabView::CalcIdealSize()
{
  nuiRect IdealRect(0.f, 0.f, 0.f, 0.f);
  
  nuiSize w = 0.f, h = 0.f;
  nuiSize W = 0.f, H = 0.f;

  for (std::vector<Tab*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
  {
    W += (*it)->GetIdealRect().GetWidth();
    H += (*it)->GetIdealRect().GetHeight();
    w = MAX(w, (*it)->GetIdealRect().GetWidth());
    h = MAX(h, (*it)->GetIdealRect().GetHeight());
  }
  IdealRect.SetSize(w, h);
  mIdealIconsRect = IdealRect;
  
  w = h = 0.f;
  for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++)
  {
    w = MAX(w, (*it)->GetIdealRect().GetWidth());
    h = MAX(h, (*it)->GetIdealRect().GetHeight());
  }
  IdealRect.SetSize(w, h);
  mIdealTabsRect = IdealRect;
  if (mTabPosition == nuiTop || mTabPosition == nuiBottom)
  { 
    mIdealIconsRect.SetSize(W, mIdealIconsRect.GetHeight());
    IdealRect.SetSize(w, h + mIdealIconsRect.GetHeight());
  }
  else // nuiLeft || nuiRight
  {
    mIdealIconsRect.SetSize(mIdealIconsRect.GetWidth(), MIN(H,h));
    IdealRect.SetSize(w + mIdealIconsRect.GetWidth(), h);
  }
   
  //mIdealRect = IdealRect;
  return IdealRect;
}

bool nuiTabView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  nuiRect TabRect;

  if (mFoldable)
  {
    // Adjust XOff and YOff
    if (mSliding)
    {
      if (!mFolded)
      {
        mFoldRatio += 0.1f;
        if (mFoldRatio >= 0.0f)
        {
          mSliding = false;
          mFoldRatio = 0.0f;
          StopAutoDraw();
        }
      }
      else
      {
        mFoldRatio -= 0.1f;
        if (mFoldRatio <= -1.0f)
        {
          mSliding = false;
          mFoldRatio = -1.0f;
          StopAutoDraw();
        }
      }
    }
    else
    {
      if (mFolded)
        mFoldRatio = -1.0f;
    }
  }

  nuiSize Y = 0;
  nuiSize X = 0;
  nuiSize R = 1.f;
  nuiSize Tmp;
  uint i = 0;

  if (mTabPosition == nuiRight || mTabPosition == nuiLeft)
  {
    if (mIdealIconsRect.GetHeight() > rRect.GetHeight())
      R = rRect.GetHeight() / mIdealIconsRect.GetHeight();        
  }
  else // nuiTop || nuiBottom
  {
    if (mIdealIconsRect.GetWidth() > rRect.GetWidth())
      R = rRect.GetWidth() / mIdealIconsRect.GetWidth();
  }

  if (mChildrenRectUnion)
  {
    if (mTabPosition == nuiRight)
    {
      TabRect = nuiRect(0.f, 0.f, rRect.GetWidth() - mIdealIconsRect.GetWidth(), rRect.GetHeight());
      X = TabRect.GetWidth();
      float XOff = TabRect.GetWidth() * mFoldRatio;
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else if (mTabPosition == nuiTop)
    {
      TabRect = nuiRect(0.f, mIdealIconsRect.GetHeight(), rRect.GetWidth(), rRect.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = -TabRect.GetHeight() * mFoldRatio;
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
    else if (mTabPosition == nuiLeft)
    {
      TabRect = nuiRect(mIdealIconsRect.GetWidth(), 0.f, rRect.GetWidth() - mIdealIconsRect.GetWidth(), rRect.GetHeight());
      float XOff = -TabRect.GetWidth() * mFoldRatio;
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else// if (mTabPosition == nuiBottom)
    {
      TabRect = nuiRect(0.f, 0.f, rRect.GetWidth(), rRect.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = TabRect.GetHeight() * mFoldRatio;
      Y = TabRect.Bottom();
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
  }
  else
  {
    nuiRect r(mTabs[mCurrentTabIndex]->GetIdealRect());
    if (mTabPosition == nuiRight)
    {
      r.Right() = (MIN(rRect.GetWidth() - mIdealIconsRect.GetWidth(), r.GetWidth()));
      TabRect = nuiRect(0.f, 0.f, r.GetWidth(), rRect.GetHeight());
      float XOff = TabRect.GetWidth() * mFoldRatio;
      X = TabRect.GetWidth();
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else if (mTabPosition == nuiTop)
    {
      r.Bottom() = (MIN(rRect.GetHeight() - mIdealIconsRect.GetHeight(), r.GetHeight()));
      TabRect = nuiRect(0.f, mIdealIconsRect.GetHeight(), rRect.GetWidth(), r.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = -TabRect.GetHeight() * mFoldRatio;
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
    else if (mTabPosition == nuiLeft)
    {
      r.Right() = (MIN(rRect.GetWidth() - mIdealIconsRect.GetWidth(), r.GetWidth()));
      TabRect = nuiRect(mIdealIconsRect.GetWidth(), 0.f, r.GetWidth() - mIdealIconsRect.GetWidth(), rRect.GetHeight());
      float XOff = -TabRect.GetWidth() * mFoldRatio;
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else// if (mTabPosition == nuiBottom)
    {
      r.Bottom() = (MIN(rRect.GetHeight() - mIdealIconsRect.GetHeight(), r.GetHeight()));
      TabRect = nuiRect(0.f, 0.f, rRect.GetWidth(), r.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = TabRect.GetHeight() * mFoldRatio;
      Y = TabRect.Bottom();
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
  }

  return true;
}

bool nuiTabView::Draw(nuiDrawContext* pContext)
{
  pContext->SetStrokeColor(nuiColor(1.f,1.f,0.f));
  for (std::vector<Tab*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
  {
    DrawChild(pContext, (*it));
  }
  if (mFoldRatio > -1.0f)
    DrawChild(pContext, mTabs[mCurrentTabIndex]);
  return true;
}

bool nuiTabView::MouseClicked   (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  for (std::vector<Tab*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
    if ((*it)->GetRect().IsInside(X,Y))
      return true;
  return false;
}

bool nuiTabView::MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  for (std::vector<Tab*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
    if ((*it)->GetRect().IsInside(X,Y))
      return true;
  return false;
}
 

void nuiTabView::AddTab(const nglString& rTitle, nuiWidget* pContents)
{
  nuiLabel* pLabel = new nuiLabel(rTitle);
  pLabel->SetBorder(4,2);
  AddTab(pLabel, pContents);
}

void nuiTabView::AddTab(nuiWidget* pTitle, nuiWidget* pContents)
{
  InsertTab(pTitle, pContents, mIcons.size());
}

void nuiTabView::InsertTab(nuiWidget* pTitle, nuiWidget* pContents, uint pos)
{
  nuiSimpleContainer* pDecoContents;
  
  NGL_ASSERT(pTitle);
  NGL_ASSERT(pContents);
  
  Tab* pTitleWidget = NULL;
  nuiWidget* pContentsWidget = pContents;
  
  if (mDecoratedBackground)
  {
    pDecoContents = new nuiSimpleContainer();
    pDecoContents->SetObjectName(_T("nuiTabView::Contents"));
    pDecoContents->SetObjectClass(_T("nuiTabView::Contents"));
    pDecoContents->AddChild(pContents);
    
    pContentsWidget = pDecoContents;
  }

  pTitleWidget = new Tab(pTitle);
  
  nuiColor color;
  nuiColor::GetColor(_T("nuiDefaultClrNormalTab"), color);
  pTitleWidget->SetColor(eNormalTextFg, color);
  nuiColor::GetColor(_T("nuiDefaultClrSelectedTab"), color);
  pTitleWidget->SetColor(eSelectedTextFg, color);
  
  AddChild(pContentsWidget);
  AddChild(pTitleWidget);
  
  if (!mTabs.empty())
    pContentsWidget->SetEnabled(false);
  pTitleWidget->SetSelected(false);
  mTabViewEvents.Connect(pTitleWidget->Clicked, &nuiTabView::OnIconClicked, pTitleWidget);
  mTabViewEvents.Connect(pTitleWidget->EnterDrag, &nuiTabView::OnTabEnterDrag, pTitleWidget);
  
  if (pos >= mIcons.size())
  {
    mTabs.push_back(pContentsWidget);
    mIcons.push_back(pTitleWidget);
  }
  else
  {
    uint t = 0;
    std::vector<Tab*>::iterator i = mIcons.begin();
    for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++, t++)
    {
      if (t == pos)
      {
        mIcons.insert(i, pTitleWidget);
        mTabs.insert(it, pContentsWidget);
      }
    }
  }
  
  if (mDecoratedBackground)
  {
    nuiDefaultDecoration::TabView_Tab(this, pTitleWidget);
    nuiDefaultDecoration::TabView_Contents(this, pContentsWidget);
  }
  
  SelectTab(0);
}

void nuiTabView::RemoveTab(nuiWidget* pTab)
{
  std::vector<Tab*>::iterator i = mIcons.begin();
  for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
  {
    if (*it == pTab)
    {
      DelChild((*it));
      mTabViewEvents.DisconnectSource((*i)->Clicked);
      DelChild((*i));

      mTabs.erase(it);     
      mIcons.erase(i);
      return;
    }
  }
}

void nuiTabView::RemoveTab(const uint& tab_index)
{
  NGL_ASSERT(tab_index < mIcons.size());

  uint t = 0;
  std::vector<Tab*>::iterator i = mIcons.begin();
  for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++, t++)
  {
    if (t == tab_index)
    {
      DelChild(*it);
      mTabViewEvents.DisconnectSource((*i)->Clicked);
      DelChild(*i);

      mTabs.erase(it);     
      mIcons.erase(i);
      return;
    }
  }
}

void nuiTabView::OnIconClicked(const nuiEvent& rEvent)
{
  Tab* pIcon = (Tab*)(rEvent.mpUser);
  for (uint i = 0; i < mIcons.size(); i++)
  {
    if (mIcons[i] == pIcon)
    {
      if (mFoldable)
      {
        if (mFolded || (i == mCurrentTabIndex))
        {
          // Slide in or out
          mFolded = !mFolded;
          mSliding = true;
          SetAutoDrawAnimateLayout(true);
          StartAutoDraw();
        }
      }

      pIcon->SetSelected(true);
      SelectTab(i);
      TabSelect(TabEvent(mCurrentTabIndex));
    }
    else
      ((Tab*)(mIcons[i]))->SetSelected(false);
  }
  rEvent.Cancel();
}

void nuiTabView::OnTabEnterDrag(const nuiEvent& rEvent)
{
  if (mChangeOnDrag)
    OnIconClicked(rEvent);
}

void nuiTabView::SelectTab(const uint& rIndex)
{
  NGL_ASSERT(rIndex < mIcons.size());
//  if (mCurrentTabIndex == rIndex)
//    return;

  ((Tab*)(mIcons[mCurrentTabIndex]))->SetSelected(false);
  mTabs[mCurrentTabIndex]->SetEnabled(false);
  mCurrentTabIndex = rIndex;
  mTabs[mCurrentTabIndex]->SetEnabled(true);

  ((Tab*)(mIcons[mCurrentTabIndex]))->SetSelected(true);
  if (mChildrenRectUnion)
    Invalidate();
  else
    InvalidateLayout();
}

void nuiTabView::SetTabPosition(nuiPosition tabPosition)
{
  if (mTabPosition == tabPosition)
    return;
  mTabPosition = tabPosition;
 InvalidateLayout();
}

void nuiTabView::EnableChildrenRectUnion(bool set)
{
  if (mChildrenRectUnion != set)
  {
    mChildrenRectUnion = set;
    InvalidateLayout();
  }
}

bool nuiTabView::GetChildrenRectUnion() const
{
  return mChildrenRectUnion;
}

nuiOrientation nuiTabView::GetOrientation()
{
  return mTabPosition == nuiLeft || mTabPosition == nuiRight ? nuiVertical : nuiHorizontal;
}

nuiPosition nuiTabView::GetTabPosition()
{
  return mTabPosition;
}

uint nuiTabView::GetTabCount()
{
  return (uint)mIcons.size();
}

void nuiTabView::SetFoldable( bool Foldable )
{
  mFoldable = Foldable;
}

bool nuiTabView::GetFoldable() const
{
  return mFoldable;
}

void nuiTabView::SetFolded( bool set, bool Animate )
{
  mFolded = set;
  if (Animate)
  {
    mSliding = true;
    SetAutoDrawAnimateLayout(true);
    StartAutoDraw();
  }
  else
  {
    mSliding = false;
    StopAutoDraw();
    if (mRect.GetSurface() > 0.0f)
      UpdateLayout();
    else
      InvalidateLayout();
  }
}

bool nuiTabView::GetFolded() const
{
  return mFolded;
}

void nuiTabView::SetChangeOnDrag(bool change)
{
  mChangeOnDrag = change;
}

bool nuiTabView::GetChangeOnDrag() const
{
  return mChangeOnDrag;
}

TabEvent::TabEvent(const uint& index) : nuiEvent(), mTabIndex(index)
{

}

/////////
//class Tab : public nuiSimpleContainer
nuiTabView::Tab::Tab(nuiWidget* pWidget)
{
  SetObjectName(_T("nuiTabView::Tab"));
  SetObjectClass(_T("nuiTabView::Tab"));
  AddChild(pWidget);
}

nglDropEffect nuiTabView::Tab::OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y)
{
  if (!IsSelected())
  {
    EnterDrag();
  }
  return eDropEffectNone;
}


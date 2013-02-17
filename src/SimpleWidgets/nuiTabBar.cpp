/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

// nuiTab
nuiTab::nuiTab()
: nuiButton(), mIdentifier(-1)
{
  SetObjectClass(_T("nuiTab"));
}

nuiTab::nuiTab(const nglString & text)
: nuiButton(text), mIdentifier(-1)
{
  SetObjectClass(_T("nuiTab"));
}

nuiTab::~nuiTab()
{
}


bool nuiTab::Draw(nuiDrawContext* pContext)
{
  DrawChildren(pContext);
  return true;
}

// nuiTabBar
nuiTabBar::nuiTabBar()
: nuiSimpleContainer(),
  mTabsSink(this)
{
  SetObjectClass(_T("nuiTabBar"));
}

nuiTabBar::~nuiTabBar()
{
}

int nuiTabBar::AddTab(nuiTab * pNewTab)
{
  AddChild(pNewTab);
  mTabs.push_back(pNewTab);
  mTabsSink.Connect(pNewTab->ButtonPressed, &nuiTabBar::OnTabSelected, pNewTab);
  int index = mTabs.size() - 1;
  pNewTab->SetIdentifier(index);

  InvalidateLayout();
  return index;
}

int nuiTabBar::InsertTab(nuiTab * pNewTab, int index)
{
  if (index < 0)
  {
    mTabs.push_back(pNewTab);
    index = mTabs.size() -  1;
  }
  else
  {
    mTabs[index] = pNewTab;
  }
  pNewTab->SetIdentifier(index);
  mTabsSink.Connect(pNewTab->ButtonPressed, &nuiTabBar::OnTabSelected,pNewTab);
  return index;
}

void nuiTabBar::RemoveTab (nuiTab * pTab)
{
  std::remove(mTabs.begin(), mTabs.end(), pTab);
  mTabsSink.DisconnectSource(pTab->ButtonPressed);
}

bool nuiTabBar::Draw(nuiDrawContext* pContext)
{
  DrawChildren(pContext);
  return true;
}

nuiRect nuiTabBar::CalcIdealSize()
{
  nuiSize width = 0;
  nuiSize height = 0;
  std::vector<nuiTab*>::const_iterator end = mTabs.end();
  for (std::vector<nuiTab*>::const_iterator it = mTabs.begin(); it != end; ++it)
  {
    const nuiRect &childIdealRect = (*it)->GetIdealRect();
    height = MAX(height, childIdealRect.GetHeight());
    width += childIdealRect.GetWidth();
  }
  mIdealRect = nuiRect(0.f, 0.f, width, height); 
  return mIdealRect;
}

bool nuiTabBar::SetRect(const nuiRect& rRect)
{
  nuiSize width = 0;
  nuiWidget::SetRect(rRect);

  IteratorPtr pIt;  
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    const nuiRect  &childIdealRect = pItem->GetIdealRect();
    nuiRect rect(width, rRect.Top(), childIdealRect.GetWidth(), childIdealRect.GetHeight());
    width += childIdealRect.GetWidth();
    pItem->SetLayout(rect);
  }
  delete pIt;

  return true;
}


void nuiTabBar::SelectTab(int id)
{
  std::list<nuiTab*>::const_iterator end = mSelectedTabs.end();
  for (std::list<nuiTab*>::const_iterator it = mSelectedTabs.begin(); it != end; ++it)
  {
    (*it)->SetSelected(false);
  }
  mSelectedTabs.clear();

  mTabs[id]->SetSelected(true);

  mSelectedTabs.push_back(mTabs[id]);

  Invalidate();

  TabsSelected(mSelectedTabs);
}

void nuiTabBar::OnTabSelected(const nuiEvent& rEvent)
{
  if (!(IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL)))
  {
    std::list<nuiTab*>::const_iterator end = mSelectedTabs.end();
    for (std::list<nuiTab*>::const_iterator it = mSelectedTabs.begin(); it != end; ++it)
    {
      (*it)->SetSelected(false);
    }
    mSelectedTabs.clear();
  }

  nuiTab *tab = (nuiTab*)rEvent.mpUser;
  tab->SetSelected(true);

  mSelectedTabs.push_back(tab);

  Invalidate();

  TabsSelected(mSelectedTabs);

  rEvent.Cancel();
}


/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiFlowView::nuiFlowView(float IdealWidth, float HSpace, float VSpace)
{
  mIdealWidth = IdealWidth;
  mVSpace = VSpace;
  mHSpace = HSpace;
}

nuiFlowView::~nuiFlowView()
{
  
}

nuiRect nuiFlowView::CalcIdealSize()
{
  float IdealWidth = mIdealWidth;
  if (mRect.GetWidth() > 0)
    IdealWidth = mRect.GetWidth();
  return Layout(false, mIdealWidth);
}

void nuiFlowView::LayoutLine(nuiWidgetList& line, float& x, float &y, float& w, float& h, float& HSpace, float &VSpace, bool setLayout)
{
  // Process the line
  if (setLayout)
  {
    int32 size = line.size();
    x = 0;
    for (int32 j = 0; j < size; j++)
    {
      nuiWidgetPtr pWidget = line[j];
      nuiRect r(pWidget->GetIdealRect());
      r.SetHeight(h);
      r.MoveTo(x, y);
      pWidget->SetLayout(r);
      x += r.GetWidth();
      x += HSpace;
    }
  }
  line.clear();
  y += h +VSpace;
  h = 0;
  w = 0;
  
}

nuiRect nuiFlowView::Layout(bool setLayout, float IdealWidth)
{
  float x = 0;
  float y = 0;
  float VSpace = mVSpace;
  float HSpace = mHSpace;
  
  nuiWidgetList line;
  
  int32 count = mpChildren.size();
  float w = 0;
  float h = 0;
  for (int32 i = 0; i < count; i++)
  {
    nuiWidgetPtr pWidget = mpChildren[i];
    
    nuiRect r(pWidget->GetIdealRect());
    if (w + r.GetWidth() > IdealWidth)
    {
      LayoutLine(line, x, y, w, h, HSpace, VSpace, setLayout);
    }
    
    h = MAX(h, r.GetHeight());
    w += r.GetWidth();
    if (!line.empty())
      w += HSpace;
    line.push_back(pWidget);
  }
  if (!line.empty())
    LayoutLine(line, x, y, w, h, HSpace, VSpace, setLayout);
  
  if (y > 0)
    y -= VSpace;

  return nuiRect(mIdealWidth, y);
}

bool nuiFlowView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  Layout(true, rRect.GetWidth());
  return true;
}

bool nuiFlowView::Draw(nuiDrawContext* pContext)
{
  nuiSimpleContainer::Draw(pContext);
  return true;
}

void nuiFlowView::SetIdealWidth(float IdealWidth)
{
  mIdealWidth = IdealWidth;
  InvalidateLayout();
}

float nuiFlowView::GetIdealWidth() const
{
  return mIdealWidth;
}

float nuiFlowView::GetVSpace() const
{
  return mVSpace;
}

float nuiFlowView::GetHSpace() const
{
  return mHSpace;
}

void nuiFlowView::SetVSpace(float VSpace)
{
  mVSpace = VSpace;
  InvalidateLayout();
}

void nuiFlowView::SetHSpace(float HSpace)
{
  mHSpace = HSpace;
  InvalidateLayout();
}


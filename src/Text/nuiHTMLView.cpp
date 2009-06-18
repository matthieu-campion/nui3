/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#include "nuiHTMLView.h"

nuiHTMLView::nuiHTMLView(float IdealWidth, float HSpace, float VSpace)
{
  mIdealWidth = IdealWidth;
  mVSpace = VSpace;
  mHSpace = HSpace;
}

nuiHTMLView::~nuiHTMLView()
{
  delete mpHTML;
}

nuiRect nuiHTMLView::CalcIdealSize()
{
  float IdealWidth = mIdealWidth;
  if (mRect.GetWidth() > 0)
    IdealWidth = mRect.GetWidth();
  return Layout(mpHTML, false, mIdealWidth);
}

void nuiHTMLView::LayoutLine(nuiWidgetList& line, float& x, float &y, float& w, float& h, float& HSpace, float &VSpace, bool setLayout)
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

nuiRect nuiHTMLView::Layout(nuiHTMLNode* pNode, bool setLayout, float IdealWidth)
{
  float x = 0;
  float y = 0;
  float VSpace = mVSpace;
  float HSpace = mHSpace;
  
  nuiWidgetList line;
  
  float w = 0;
  float h = 0;

  
  while (pNode)
  {
    switch (pNode->GetTagType())
    {
    default:
      break;
    }
    
    nuiWidgetPtr pWidget = NULL; //mpChildren[i];
    
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

bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  Layout(mpHTML, true, rRect.GetWidth());
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  nuiSimpleContainer::Draw(pContext);
  return true;
}

void nuiHTMLView::SetIdealWidth(float IdealWidth)
{
  mIdealWidth = IdealWidth;
  InvalidateLayout();
}

float nuiHTMLView::GetIdealWidth() const
{
  return mIdealWidth;
}

float nuiHTMLView::GetVSpace() const
{
  return mVSpace;
}

float nuiHTMLView::GetHSpace() const
{
  return mHSpace;
}

void nuiHTMLView::SetVSpace(float VSpace)
{
  mVSpace = VSpace;
  InvalidateLayout();
}

void nuiHTMLView::SetHSpace(float HSpace)
{
  mHSpace = HSpace;
  InvalidateLayout();
}

bool nuiHTMLView::SetText(const nglString& rHTMLText)
{
  Clear();
  delete mpHTML;
  mpHTML = new nuiHTML();
  
  std::string str(rHTMLText.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = mpHTML->Load(mem);
  
  InvalidateLayout();
  return res;
}


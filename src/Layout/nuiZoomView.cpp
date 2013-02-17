/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiZoomView::nuiZoomView(nuiScrollBar *pHorizontalScrollbar, nuiScrollBar *pVerticalScrollbar, nuiSlider * pHorizontalSlider, nuiSlider * pVerticalSlider, bool calcWidthFromIdealSize, bool calcHeightFromIdealSize)
: nuiSimpleContainer(),
  mSVSink(this)
{
  SetObjectClass(_T("nuiZoomView"));

  mAlwaysDisplayVScrollbar = false;
  mAlwaysDisplayHScrollbar = false;

  mpHorizontalScrollbar = pHorizontalScrollbar;
  mpVerticalScrollbar = pVerticalScrollbar;
  mpHorizontalSlider = pHorizontalSlider;
  mpVerticalSlider = pVerticalSlider;

  mCalcWidthFromIdealSize = calcWidthFromIdealSize;
  mCalcHeightFromIdealSize = calcHeightFromIdealSize;

  mHorizontalZoomLevel = 1.f;
  mVerticalZoomLevel = 1.f;


  if (mpHorizontalScrollbar)
    mSVSink.Connect(mpHorizontalScrollbar->GetRange().ValueChanged, &nuiZoomView::Scrolled);
  if (mpVerticalScrollbar)
    mSVSink.Connect(mpVerticalScrollbar->GetRange().ValueChanged,   &nuiZoomView::Scrolled);

  if (mpHorizontalSlider)
    mSVSink.Connect(mpHorizontalSlider->GetRange().Changed, &nuiZoomView::Zoomed);
  if (mpVerticalSlider)
    mSVSink.Connect(mpVerticalSlider->GetRange().Changed,   &nuiZoomView::Zoomed);

  mSVSink.Connect(ChildAdded, &nuiZoomView::OnChildAdded);
  mSVSink.Connect(ChildDeleted, &nuiZoomView::OnChildRemoved);
}

nuiZoomView::~nuiZoomView()
{
}

nuiRect nuiZoomView::CalcIdealSize()
{
  nuiRect rect;

  nuiSize HorizontalZoomLevel = mHorizontalZoomLevel; 
  nuiSize VerticalZoomLevel = mVerticalZoomLevel;

  if (mpVerticalSlider)
    VerticalZoomLevel = mpVerticalSlider->GetRange().GetValue();
  if (mpHorizontalSlider)
    HorizontalZoomLevel = mpHorizontalSlider->GetRange().GetValue();


  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect itemRect = pItem->GetIdealRect();
    itemRect.SetSize(itemRect.GetWidth() * HorizontalZoomLevel, itemRect.GetHeight() * VerticalZoomLevel);
    rect.Union(rect, itemRect); 
  }
  delete pIt;

  rect.mLeft = 0;
  rect.mTop = 0;

  mIdealRect = rect;
  return mIdealRect;
}

bool nuiZoomView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  nuiSize XOffset = 0;
  nuiSize YOffset = 0;

  if (mpHorizontalScrollbar)
    XOffset = (nuiSize)ToNearest(mpHorizontalScrollbar->GetRange().GetValue());
  if (mpVerticalScrollbar)
    YOffset = (nuiSize)ToNearest(mpVerticalScrollbar->GetRange().GetValue());

  nuiSize HorizontalZoomLevel = mHorizontalZoomLevel; 
  nuiSize VerticalZoomLevel = mVerticalZoomLevel;

  if (mpVerticalSlider)
    VerticalZoomLevel = mpVerticalSlider->GetRange().GetValue();
  if (mpHorizontalSlider)
    HorizontalZoomLevel = mpHorizontalSlider->GetRange().GetValue();

  nuiSize x;
  nuiSize y;
  nuiSize xx = rRect.GetWidth();
  nuiSize yy = rRect.GetHeight();

  if (mCalcWidthFromIdealSize)
    x = GetIdealRect().GetWidth();
  else
    x = xx * HorizontalZoomLevel;
  if (mCalcHeightFromIdealSize)
    y = GetIdealRect().GetHeight();
  else
    y = yy * VerticalZoomLevel;

  bool needv = y > yy;
  bool needh = x > xx;
  
  if (mpHorizontalScrollbar)
  {
    nuiRange& hrange = mpHorizontalScrollbar->GetRange();
    hrange.SetRange(0,x);
    hrange.SetPageSize(MAX(0, xx));
    hrange.SetIncrement(16.0f);
    hrange.SetPageIncrement(MAX(0, xx - xx / 8.f)); // 1/8th of page skip overlaps with the next and previous
    hrange.EnableEvents(false); 
    hrange.SetValue(XOffset); 
    hrange.EnableEvents(true);
    XOffset = hrange.GetValue();
  }
  if (mpVerticalScrollbar)
  {
    nuiRange& vrange = mpVerticalScrollbar->GetRange();
    vrange.SetRange(0,y);
    vrange.SetPageSize(MAX(0, yy));
    vrange.SetIncrement(16.0f);
    vrange.SetPageIncrement(MAX(0, yy - yy / 8.f)); // 1/8th of page skip overlaps with the next and previous
    vrange.EnableEvents(false); 
    vrange.SetValue(YOffset); 
    vrange.EnableEvents(true);
    YOffset = vrange.GetValue();
  }

  if (x < xx)
    x = xx;
  if (y < yy)
    y = yy;

  IteratorPtr pIt;  
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect rect(-XOffset, -YOffset, x, y);
    pItem->SetLayout(rect);
  }
  delete pIt;

  needv = needv | mAlwaysDisplayVScrollbar;
  needh = needh | mAlwaysDisplayHScrollbar;

  if (mpVerticalScrollbar)
    mpVerticalScrollbar->SetVisible(needv);
  if (mpHorizontalScrollbar)
    mpHorizontalScrollbar->SetVisible(needh);

  return true;
}


void nuiZoomView::Scrolled(const nuiEvent& rEvent)
{
  UpdateLayout();
}

void nuiZoomView::Zoomed(const nuiEvent& rEvent)
{
  InvalidateLayout();
}

void nuiZoomView::OnChildAdded(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiWidget>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiWidget>*>(&rEvent);
  nuiWidgetPtr pNode = dynamic_cast<nuiWidgetPtr>(pTreeEvent->mpChild);

  if (pNode)
  {
    mSVSink.Connect(pNode->HotRectChanged, &nuiZoomView::OnHotRectChanged, pNode);
  }
}

void nuiZoomView::OnChildRemoved(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiWidget>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiWidget>*>(&rEvent);
  nuiWidgetPtr pNode = dynamic_cast<nuiWidgetPtr>(pTreeEvent->mpChild);

  if (pNode)
  {
    mSVSink.DisconnectSource(pNode->HotRectChanged);
  }
}

void nuiZoomView::OnHotRectChanged(const nuiEvent& rEvent)
{
  nuiWidgetPtr pChild = dynamic_cast<nuiWidgetPtr>((nuiObject*)rEvent.mpUser);

  if (!pChild)
    return;

  nuiRect rect;
  pChild->GetHotRect(rect);

  if (mpVerticalScrollbar)
  {
    mpVerticalScrollbar->GetRange().MakeInRange(rect.Top(), rect.GetHeight());
  }
  if (mpHorizontalScrollbar)
  {
    mpHorizontalScrollbar->GetRange().MakeInRange(rect.Left(), rect.GetWidth());
  }
}

bool nuiZoomView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonWheelUp)
  {
    if (mpVerticalScrollbar)
    {
      mpVerticalScrollbar->GetRange().Decrement();
      return true;
    }
    else if (mpHorizontalScrollbar)
    {
      mpHorizontalScrollbar->GetRange().Decrement();
      return true;
    }
  }
  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    if (mpVerticalScrollbar)
    {
      mpVerticalScrollbar->GetRange().Increment();
      return true;
    }
    else if (mpHorizontalScrollbar)
    {
      mpHorizontalScrollbar->GetRange().Increment();
      return true;
    }
  }

  return false;
}

nuiRange* nuiZoomView::GetScrollbarRange(nuiOrientation Orientation)
{
  if (Orientation == nuiHorizontal)
  {
    if (mpHorizontalScrollbar)
      return &mpHorizontalScrollbar->GetRange();
  }
  else
  {
    if (mpVerticalScrollbar)
      return &mpVerticalScrollbar->GetRange();
  }
  return NULL;
}

void nuiZoomView::SetAlwaysVisibleScrollbars(bool h, bool v)
{
  mAlwaysDisplayVScrollbar = v;
  mAlwaysDisplayHScrollbar = h;
  InvalidateLayout();
}

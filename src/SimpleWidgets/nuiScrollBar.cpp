/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define SCROLL_IDEAL_WIDTH 12
#define SCROLL_IDEAL_SIZE 50

nuiScrollBar::nuiScrollBar(nuiOrientation orientation, const nuiRange& rRange, nuiWidgetPtr pThumb)
  : nuiSimpleContainer(),
    mRange(rRange),
    mpRange(NULL),
    mpThumb(pThumb),
    mTimer(.2),
    mScrollBarSink(this)
{
  if (SetObjectClass(_T("nuiScrollBar")))
  {
    AddAttribute(new nuiAttribute<nuiSize>(nglString(_T("ThumbMinSize")), nuiUnitNone,
                                           nuiMakeDelegate(this, &nuiScrollBar::GetThumbMinSize),
                                           nuiMakeDelegate(this, &nuiScrollBar::SetThumbMinSize)));
    
    AddAttribute(new nuiAttribute<const nglString&>(nglString(_T("ForegroundDecoration")), nuiUnitName,
                                                    nuiMakeDelegate(this, &nuiScrollBar::GetForegroundDecoName),
                                                    nuiMakeDelegate(this, &nuiScrollBar::SetForegroundDecoName)));
    
    AddAttribute(new nuiAttribute<const nglString&>(nglString(_T("BackgroundDecoration")), nuiUnitName,
                                                    nuiMakeDelegate(this, &nuiScrollBar::GetBackgroundDecoName),
                                                    nuiMakeDelegate(this, &nuiScrollBar::SetBackgroundDecoName)));
  }

  SetRedrawOnHover(true);
  mOrientation = orientation;
  mClicked = false;
  mThumbClicked = false;
  mPageUpClicked = false;
  mPageDownClicked = false;
  mDrawBackground = true;

  mThumbSideSize = 0.0;
  mThumbMiddleSize = 0.0;
  mThumbMinSize = SBMINTHUMB;
  
  mRightSideClicked = false;
  mLeftSideClicked = false;
  
  mIdealWidth = SCROLL_IDEAL_WIDTH;
  
  mScrollBarSink.Connect(mTimer.Tick, &nuiScrollBar::HandlePageUp);
  mScrollBarSink.Connect(mTimer.Tick, &nuiScrollBar::HandlePageDown);

  SetRange(&mRange);

  if (mpThumb)
    AddChild(mpThumb);
  
  
  if (mOrientation == nuiVertical)
  {
    mpBackgroundDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarVerticalBkg"));
    mpForegroundDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarVerticalHdl"));
  }
  else
  {
    mpBackgroundDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarHorizontalBkg"));
    mpForegroundDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarHorizontalHdl"));
  }

  NUI_ADD_EVENT(ValueChanged);
}

nuiScrollBar::~nuiScrollBar()
{
}

#define SHADESIZE 4 

bool nuiScrollBar::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  mRangeRect = rRect.Size();
  SetThumbRect(mRangeRect);

  return true;
}

void nuiScrollBar::SetThumbRect(const nuiRect& rRect)
{
  nuiRange& rRange = GetRange();

  float length = rRange.GetRange();
  float thumbPosition, thumbLength;

  if (length <= 0)
  {
    thumbLength = 1;
    length = 1;
    thumbPosition  = 0;
  }
  else
  {
    
    //thumbLength = rRange.GetPageSize() / length;
    float tempLength = rRange.GetPageSize() / length;
    if (mOrientation == nuiHorizontal)
    {
      float offset = (2*mThumbSideSize + mThumbMiddleSize) / rRect.GetWidth();
      thumbLength = offset + (1 - offset) * tempLength;
      thumbPosition = (1 - offset) * ((rRange.GetValue() - rRange.GetMinimum()) / length);
    }
    if (mOrientation == nuiVertical)
    {
      float offset = (2*mThumbSideSize + mThumbMiddleSize) / rRect.GetHeight();
      thumbLength = offset + (1 - offset) * tempLength;
      thumbPosition = (1 - offset) * ((rRange.GetValue() - rRange.GetMinimum()) / length);
    }
  }
  
  mThumbRect = rRect;
  if (mOrientation == nuiHorizontal)
  {
    mThumbRect.mLeft = (nuiSize)ToNearest(rRect.Left() + (thumbPosition * rRect.GetWidth()));
    mThumbRect.mRight = (nuiSize)ToNearest(rRect.Left() + ((thumbLength+thumbPosition) * rRect.GetWidth()));

    if (mThumbRect.GetWidth() < mThumbMinSize)
      mThumbRect.SetSize(mThumbMinSize, rRect.GetHeight());
    if (mThumbRect.Right() > rRect.Right())
      mThumbRect.Move(rRect.Right() - mThumbRect.Right(), 0.f);
  }
  else
  {
    mThumbRect.mTop = (nuiSize)ToNearest(rRect.Top() + (thumbPosition* rRect.GetHeight()));
    mThumbRect.mBottom = (nuiSize)ToNearest(rRect.Top() + ((thumbLength+thumbPosition) * rRect.GetHeight()));

    if (mThumbRect.GetHeight() < mThumbMinSize)
      mThumbRect.SetSize(rRect.GetWidth(), mThumbMinSize);
    if (mThumbRect.Bottom() > rRect.Bottom())
      mThumbRect.Move(0.f, rRect.Bottom() - mThumbRect.Bottom());
    
    
  }
  
  
  if (mpThumb)
    mpThumb->SetLayout(mThumbRect);
}

bool nuiScrollBar::Draw(nuiDrawContext* pContext)
{
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  
  if (mDrawBackground && mpBackgroundDeco)
  {
    const nuiRect& rRect = GetRangeRect();
    
    nuiRect rectDest(0.0f, 0.0f, rRect.GetWidth(), rRect.GetHeight());
    mpBackgroundDeco->Draw(pContext, this, rectDest);
  }
  
  if (!mpThumb && mpForegroundDeco)
  {
    nuiRect rRect = GetThumbRect();
    rRect.Grow(-1.f, -1.f);
    
    mpForegroundDeco->Draw(pContext, this, rRect);
  }
  
  DrawChildren(pContext);
  
  return true;
}

nuiRect nuiScrollBar::CalcIdealSize()
{
  NGL_ASSERT(mpRange);
  nuiRect Rect;
  if (mOrientation == nuiHorizontal)
  {
    Rect.mRight = MAX(mpRange->GetRange(),SCROLL_IDEAL_SIZE);
    Rect.mBottom= mIdealWidth;
  }
  else
  {
    Rect.mRight = mIdealWidth;
    Rect.mBottom= MAX(mpRange->GetRange(),SCROLL_IDEAL_SIZE);
  }
  
  mIdealRect = Rect;
  return mIdealRect;
}

void nuiScrollBar::SetIdealWidth(nuiSize width)
{
  mIdealWidth = width;
  InvalidateLayout();
}

nuiSize nuiScrollBar::GetIdealWidth() const
{
  return mIdealWidth;
}

// Received Mouse events:
bool nuiScrollBar::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClickX = X;
  mClickY = Y;

  //NGL_OUT(_T("nuiScrollBar::MouseClicked\n"));
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    Grab();
    Invalidate();

    mThumbClicked = false;
    mPageUpClicked = false;
    mPageDownClicked = false;
    if (mOrientation == nuiHorizontal)
    {
      if (X < mThumbRect.Left()) // PageUp
      {
        mTimer.Start(true,true);
        mPageUpClicked = true;
      }
      else if (X > mThumbRect.Right()) // PageDown
      {
        mTimer.Start(true,true);
        mPageDownClicked = true;
      }
      else if (X > mThumbRect.Right() - mThumbSideSize)
      {
        mClickValue = mpRange->GetValue();
        mRightSideClicked = true;
      }
      else if (X < mThumbRect.Left() + mThumbSideSize)
      {
        mClickValue = mpRange->GetValue();
        mLeftSideClicked = true;
      }
      else
      {
        mClickValue = mpRange->GetValue();
        mThumbClicked = true;
      }
    }
    else
    {
      if (Y < mThumbRect.Top()) // PageUp
      {
        mTimer.Start(true,true);
        mPageUpClicked = true;
      }
      else if (Y > mThumbRect.Bottom()) // PageDown
      {
        mTimer.Start(true,true);
        mPageDownClicked = true;
      }
      else
      {
        mClickValue = mpRange->GetValue();
        mThumbClicked = true;
        ThumbPressed();
      }
    }
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelUp)
  {
    mpRange->Decrement();
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    mpRange->Increment();
    return true;
  }
  return false;
}            

bool nuiScrollBar::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = false;

    if (mPageUpClicked || mPageDownClicked)
    {
      mTimer.Stop();
    }
    else
    {
      ThumbDepressed();
    }

    mThumbClicked = false;
    mPageUpClicked = false;
    mPageDownClicked = false;
    Ungrab();
   
    Invalidate();
    return true;
  }
  return false;
}

bool nuiScrollBar::MouseMoved  (nuiSize X, nuiSize Y)
{
  //NGL_OUT(_T("nuiScrollBar::MouseMoved\n"));
  NGL_ASSERT(mpRange);

  if (mThumbClicked)
  {
    nuiSize x,y;
    x = X-mClickX;
    y = Y-mClickY;

    nuiSize length = mpRange->GetRange();
    nuiSize start=(mClickValue/length);
    
    nuiSize movement;

    if (mOrientation == nuiHorizontal)
    {
      movement = x;
      start +=movement/mRect.GetWidth();
    }
    else
    {
      movement = y;
      start +=movement/mRect.GetHeight();
    }

    mpRange->SetValue(start*length);
    return true;
  }

  return false;
}

void nuiScrollBar::Increment(const nuiEvent& rEvent)
{
  NGL_ASSERT(mpRange);
  mpRange->Increment();
}

void nuiScrollBar::Decrement(const nuiEvent& rEvent)
{
  NGL_ASSERT(mpRange);
  mpRange->Decrement();
}

// Data management:
void nuiScrollBar::HandlePageDown(const nuiEvent& rEvent)
{
  NGL_ASSERT(mpRange);
  if (mPageDownClicked)
    mpRange->PageIncrement();
}

void nuiScrollBar::HandlePageUp(const nuiEvent& rEvent)
{
  NGL_ASSERT(mpRange);
  if (mPageUpClicked)
    mpRange->PageDecrement();
}

void nuiScrollBar::SetRange(nuiRange* pRange)
{
  if (!pRange)
    pRange = &mRange;
  
  if (pRange == mpRange)
    return;
  
  if (mpRange)
  {
    mScrollBarSink.Disconnect(mpRange->Changed, &nuiScrollBar::DoInvalidateLayout);
    mScrollBarSink.Disconnect(mpRange->ValueChanged, &nuiScrollBar::DoInvalidate);
  }
  
  mpRange = pRange;
  if (mpRange)
  {
    mScrollBarSink.Connect(mpRange->Changed, &nuiScrollBar::DoInvalidateLayout);
    mScrollBarSink.Connect(mpRange->ValueChanged, &nuiScrollBar::DoInvalidate);
  }

  ValueChanged();
  InvalidateLayout();  
}

nuiRange& nuiScrollBar::GetRange()
{
  NGL_ASSERT(mpRange);
  return *mpRange;
}

void nuiScrollBar::DoInvalidate(const nuiEvent& rEvent)
{
  ValueChanged();
  if (mRect.GetWidth() > 0 && mRect.GetHeight() > 0)
    UpdateLayout();
}

void nuiScrollBar::DoInvalidateLayout(const nuiEvent& rEvent)
{
  ValueChanged();
  if (mRect.GetWidth() > 0 && mRect.GetHeight() > 0)
    UpdateLayout();
}

void nuiScrollBar::SetOrientation(nuiOrientation Orientation)
{
  if (mOrientation == Orientation)
    return;
  mOrientation = Orientation;
  InvalidateLayout();
}

nuiOrientation nuiScrollBar::GetOrientation()
{
  return mOrientation;
}

void nuiScrollBar::DisableDrawBackground(bool disabled)
{
  mDrawBackground = !disabled;
  Invalidate();
}

void nuiScrollBar::SetThumbWidget(nuiWidget* pThumb)
{
  if (mpThumb)
    mpThumb->Trash();
  
  mpThumb = pThumb;
  if (pThumb)
    AddChild(pThumb);
  InvalidateLayout();
}



void nuiScrollBar::SetThumbSideSize(float sideSize)
{
  mThumbSideSize = sideSize;
}

float nuiScrollBar::GetThumbSideSize()
{
  return mThumbSideSize;
}

void nuiScrollBar::SetThumbMiddleSize(float middleSize)
{
  mThumbMiddleSize = middleSize;
}

float nuiScrollBar::GetThumbMiddleSize()
{
  return mThumbMiddleSize;
}

nuiSize nuiScrollBar::GetThumbMinSize() const
{
  return mThumbMinSize;
}
void nuiScrollBar::SetThumbMinSize(nuiSize MinSize)
{
  mThumbMinSize = MinSize;
  InvalidateLayout();
}


void nuiScrollBar::SetForegroundDeco(nuiDecoration* pDeco)
{
  if (pDeco)
    pDeco->Acquire();
  if (mpForegroundDeco)
    mpForegroundDeco->Release();
  mpForegroundDeco = pDeco;
  Invalidate();
}

void nuiScrollBar::SetBackgroundDeco(nuiDecoration* pDeco)
{
  if (pDeco)
    pDeco->Acquire();
  if (mpBackgroundDeco)
    mpBackgroundDeco->Release();
  mpBackgroundDeco = pDeco;
  Invalidate();
}

nuiDecoration* nuiScrollBar::GetForegroundDeco()
{
  return mpForegroundDeco;
}

nuiDecoration* nuiScrollBar::GetBackgroundDeco()
{
  return mpBackgroundDeco;
}

void nuiScrollBar::SetForegroundDecoName(const nglString& rName)
{
  nuiDecoration* pDecoration = nuiDecoration::Get(rName);
  SetForegroundDeco(pDecoration);
}

void nuiScrollBar::SetBackgroundDecoName(const nglString& rName)
{
  nuiDecoration* pDecoration = nuiDecoration::Get(rName);
  SetBackgroundDeco(pDecoration);
}

const nglString& nuiScrollBar::GetForegroundDecoName()
{
  if (mpForegroundDeco)
    return mpForegroundDeco->GetObjectName();
  
  return nglString::Null;
}

const nglString& nuiScrollBar::GetBackgroundDecoName()
{
  if (mpBackgroundDeco)
    return mpBackgroundDeco->GetObjectName();
  
  return nglString::Null;
}

/// Custom ScrollBar

nuiCustomScrollBar::nuiCustomScrollBar(nuiOrientation orientation, const nuiRange& rRange, nuiWidgetPtr pThumb)
: nuiScrollBar(orientation, rRange, pThumb)
{
}

nuiCustomScrollBar::~nuiCustomScrollBar()
{
}

void nuiCustomScrollBar::AddWidget(nuiWidget* pWidget, nuiPosition position)
{
  if (pWidget)
  {
    AddChild(pWidget);
    
    if (position == nuiCenter || position == nuiFill) /// those children will use the same layout as the ScrollBar range background rect
      mScrollBarCenteredChildren.push_back(pWidget);
    else
      mScrollBarPositionedChildren.push_back(std::make_pair(pWidget, position));
    InvalidateLayout();
  }
}
  
bool nuiCustomScrollBar::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  if (mOrientation == nuiHorizontal)
  {
    nuiSize LastLeft = 0.f, LastRight = 0.f;
    PositionedWidgetList::const_iterator end = mScrollBarPositionedChildren.end();
    for (PositionedWidgetList::const_iterator it = mScrollBarPositionedChildren.begin(); it != end; ++it)
    {
      nuiWidget* pWidget = it->first;
      nuiPosition pos = it->second;
      
      const nuiRect& rIdeal = pWidget->GetIdealRect();
      
      if (pos == nuiLeft)
      {
        pWidget->SetLayout( nuiRect(LastLeft, 0.f, rIdeal.GetWidth(), MIN(rIdeal.GetHeight(), rRect.GetHeight())) );
        LastLeft += rIdeal.GetWidth();
      }
      else // considered as right
      {
        pWidget->SetLayout( nuiRect(rRect.GetWidth() - rIdeal.GetWidth() - LastRight, 0.f, rIdeal.GetWidth(), MIN(rIdeal.GetHeight(), rRect.GetHeight())) );
        LastRight += rIdeal.GetWidth();
      }
    }
    mRangeRect.Set(LastLeft, 0.f, rRect.GetWidth() - LastLeft - LastRight, rRect.GetHeight());
  }
  else// nuiVertical
  {
    nuiSize LastTop = 0.f, LastBottom = 0.f;
    PositionedWidgetList::const_iterator end = mScrollBarPositionedChildren.end();
    for (PositionedWidgetList::const_iterator it = mScrollBarPositionedChildren.begin(); it != end; ++it)
    {
      nuiWidget* pWidget = it->first;
      nuiPosition pos = it->second;

      const nuiRect& rIdeal = pWidget->GetIdealRect();

      if (pos == nuiTop)
      {
        pWidget->SetLayout( nuiRect(LastTop, 0.f, MIN(rIdeal.GetWidth(), rRect.GetWidth()), rIdeal.GetHeight()) );
        LastTop += rIdeal.GetHeight();
      }
      else // considered as Bottom
      {
        pWidget->SetLayout( nuiRect(0.f, rRect.GetHeight() - rIdeal.GetHeight() - LastBottom, MIN(rIdeal.GetWidth(), rRect.GetWidth()), rIdeal.GetHeight()) );
        LastBottom += rIdeal.GetHeight();
      }
    }
    mRangeRect.Set(0.f, LastTop, rRect.GetWidth(), rRect.GetHeight() - LastTop - LastBottom);
  }
  
  {// setting over the range rect children layout
    nuiWidgetList::const_iterator end = mScrollBarCenteredChildren.end();
    for (nuiWidgetList::const_iterator it = mScrollBarCenteredChildren.begin(); it != end; ++it)
      (*it)->SetLayout(mRangeRect);
  }

  SetThumbRect(mRangeRect);
  return true;
}

nuiRect nuiCustomScrollBar::CalcIdealSize()
{
  nuiScrollBar::CalcIdealSize();

  nuiSize ChildrenSize = 0.f;
  PositionedWidgetList::const_iterator end = mScrollBarPositionedChildren.end();
  if (mOrientation == nuiHorizontal)
    for (PositionedWidgetList::const_iterator it = mScrollBarPositionedChildren.begin(); it != end; ++it)
      ChildrenSize += it->first->GetIdealRect().GetWidth(); 
  else
    for (PositionedWidgetList::const_iterator it = mScrollBarPositionedChildren.begin(); it != end; ++it)
      ChildrenSize += it->first->GetIdealRect().GetHeight();
  
  if (mOrientation == nuiHorizontal)
    mIdealRect.SetSize(mIdealRect.GetWidth() + ChildrenSize, mIdealRect.GetHeight());
  else
    mIdealRect.SetSize(mIdealRect.GetWidth(), mIdealRect.GetHeight() + ChildrenSize);
  return mIdealRect;

}

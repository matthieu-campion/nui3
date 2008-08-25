/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiScrollBar.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiTheme.h"

#define SCROLL_IDEAL_WIDTH 12
#define SCROLL_IDEAL_SIZE 50

nuiScrollBar::nuiScrollBar(nuiOrientation orientation, const nuiRange& rRange, nuiWidgetPtr pThumb)
  : nuiSimpleContainer(),
    mRange(rRange),
    mpThumb(pThumb),
    mTimer(.2),
    mScrollBarSink(this)
{
  SetObjectClass(_T("nuiScrollBar"));
  SetRedrawOnHover(true);
  mOrientation = orientation;
  mClicked = false;
  mThumbClicked = false;
  mPageUpClicked = false;
  mPageDownClicked = false;
  mDrawBackground = true;

  mThumbSideSize = 0.0;
  mThumbMiddleSize = 0.0;
  
  mRightSideClicked = false;
  mLeftSideClicked = false;
  
  mIdealWidth = SCROLL_IDEAL_WIDTH;
  
  mRange = rRange;

  mScrollBarSink.Connect(mRange.Changed, &nuiScrollBar::DoInvalidateLayout);
  mScrollBarSink.Connect(mRange.ValueChanged, &nuiScrollBar::DoInvalidate);

  if (mpThumb)
    AddChild(mpThumb);

#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Orientation"].Bind(&mOrientation,true,false);
#endif
  NUI_ADD_EVENT(ValueChanged);
}

bool nuiScrollBar::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  mpThumb = NULL;
  mTimer.SetPeriod(.2);
      
  SetObjectClass(_T("nuiScrollBar"));
  mScrollBarSink.SetTarget(this);
  SetRedrawOnHover(true);
  mClicked = false;
  mThumbClicked = false;
  mPageUpClicked = false;
  mPageDownClicked = false;
  mDrawBackground = true;

  // FIXME: interpret other attributes...
  mOrientation = nuiGetOrientation(pNode);
  mScrollBarSink.Connect(mRange.Changed, &nuiScrollBar::DoInvalidateLayout);
  mScrollBarSink.Connect(mRange.ValueChanged, &nuiScrollBar::DoInvalidate);

#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Orientation"].Bind(&mOrientation,true,false);
#endif
  NUI_ADD_EVENT(ValueChanged);
  
  return true;
}

nuiXMLNode* nuiScrollBar::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiWidget::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;
  

  pNode->SetAttribute(_T("Orientation"),mOrientation);
  return pNode;
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

    if (mThumbRect.GetWidth() < SBMINTHUMB)
      mThumbRect.SetSize(SBMINTHUMB, rRect.GetHeight());
    if (mThumbRect.Right() > rRect.Right())
      mThumbRect.Move(rRect.Right() - mThumbRect.Right(), 0.f);
  }
  else
  {
    mThumbRect.mTop = (nuiSize)ToNearest(rRect.Top() + (thumbPosition* rRect.GetHeight()));
    mThumbRect.mBottom = (nuiSize)ToNearest(rRect.Top() + ((thumbLength+thumbPosition) * rRect.GetHeight()));

    if (mThumbRect.GetHeight() < SBMINTHUMB)
      mThumbRect.SetSize(rRect.GetWidth(), SBMINTHUMB);
    if (mThumbRect.Bottom() > rRect.Bottom())
      mThumbRect.Move(0.f, rRect.Bottom() - mThumbRect.Bottom());
  }
  
  if (mpThumb)
    mpThumb->SetLayout(mThumbRect);
}

bool nuiScrollBar::Draw(nuiDrawContext* pContext)
{
  pContext->PushState();
  pContext->ResetState();

  pContext->PushClipping();
  pContext->Clip(GetRect().Size());
  pContext->EnableClipping(true);
  
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  
  nuiTheme* pTheme = GetTheme();
  if (mDrawBackground)
    pTheme->DrawScrollBarBackground(pContext,this);
  if (!mpThumb)
    pTheme->DrawScrollBarForeground(pContext,this);
  pTheme->Release();
  
  DrawChildren(pContext);
  
  pContext->PopClipping();
  pContext->PopState();
  
  return true;
}

nuiRect nuiScrollBar::CalcIdealSize()
{
  nuiRect Rect;
  if (mOrientation == nuiHorizontal)
  {
    Rect.mRight = MAX(mRange.GetRange(),SCROLL_IDEAL_SIZE);
    Rect.mBottom= mIdealWidth;
  }
  else
  {
    Rect.mRight = mIdealWidth;
    Rect.mBottom= MAX(mRange.GetRange(),SCROLL_IDEAL_SIZE);
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
        mScrollBarSink.Connect(mTimer.Tick, &nuiScrollBar::HandlePageUp);
        mTimer.Start(true,true);
        mPageUpClicked = true;
      }
      else if (X > mThumbRect.Right()) // PageDown
      {
        mScrollBarSink.Connect(mTimer.Tick, &nuiScrollBar::HandlePageDown);
        mTimer.Start(true,true);
        mPageDownClicked = true;
      }
      else if (X > mThumbRect.Right() - mThumbSideSize)
      {
        mClickValue = mRange.GetValue();
        mRightSideClicked = true;
      }
      else if (X < mThumbRect.Left() + mThumbSideSize)
      {
        mClickValue = mRange.GetValue();
        mLeftSideClicked = true;
      }
      else
      {
        mClickValue = mRange.GetValue();
        mThumbClicked = true;
      }
    }
    else
    {
      if (Y < mThumbRect.Top()) // PageUp
      {
        mScrollBarSink.Connect(mTimer.Tick, &nuiScrollBar::HandlePageUp);
        mTimer.Start(true,true);
        mPageUpClicked = true;
      }
      else if (Y > mThumbRect.Bottom()) // PageDown
      {
        mScrollBarSink.Connect(mTimer.Tick, &nuiScrollBar::HandlePageDown);
        mTimer.Start(true,true);
        mPageDownClicked = true;
      }
      else
      {
        mClickValue = mRange.GetValue();
        mThumbClicked = true;
        ThumbPressed();
      }
    }
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelUp)
  {
    mRange.Decrement();
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    mRange.Increment();
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
      mScrollBarSink.Disconnect(&nuiScrollBar::HandlePageUp);
      mScrollBarSink.Disconnect(&nuiScrollBar::HandlePageDown);

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

  if (mThumbClicked)
  {
    nuiSize x,y;
    x = X-mClickX;
    y = Y-mClickY;

    nuiSize length = mRange.GetRange();
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

    mRange.SetValue(start*length);
    return true;
  }

  return false;
}

bool nuiScrollBar::Increment(const nuiEvent& rEvent)
{
  mRange.Increment();
  return false;
}

bool nuiScrollBar::Decrement(const nuiEvent& rEvent)
{
  mRange.Decrement();
  return false;
}

// Data management:
bool nuiScrollBar::HandlePageDown(const nuiEvent& rEvent)
{
  mRange.PageIncrement();
  return false;
}

bool nuiScrollBar::HandlePageUp(const nuiEvent& rEvent)
{
  mRange.PageDecrement();
  return false;
}

nuiRange& nuiScrollBar::GetRange()
{
  return mRange;
}

bool nuiScrollBar::DoInvalidate(const nuiEvent& rEvent)
{
  ValueChanged();
  UpdateLayout();
  return false;
}

bool nuiScrollBar::DoInvalidateLayout(const nuiEvent& rEvent)
{
  ValueChanged();
  UpdateLayout();
  return false;
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


/// Custom ScrollBar

nuiCustomScrollBar::nuiCustomScrollBar(nuiOrientation orientation, const nuiRange& rRange, nuiWidgetPtr pThumb)
: nuiScrollBar(orientation, rRange, pThumb)
{
}

bool nuiCustomScrollBar::Load(const nuiXMLNode* pNode)
{
  return nuiScrollBar::Load(pNode);
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

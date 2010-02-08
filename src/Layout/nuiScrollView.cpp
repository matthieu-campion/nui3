/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiScrollView.h"

#include "nuiGradientDecoration.h"

#define SCROLL_SIZE 12.0f
#define NUI_SMOOTH_SCROLL_RATIO (0.2f/1.f)


nuiScrollView::nuiScrollView(bool Horizontal, bool Vertical)
  : nuiComposite(),
    mSVSink(this)
{
  Init(NULL, NULL, Horizontal, Vertical);
}

nuiScrollView::nuiScrollView(nuiScrollBar* pHorizontalScrollBar, nuiScrollBar* pVerticalScrollBar )
: nuiComposite(),
  mSVSink(this)
{
  Init(pHorizontalScrollBar, pVerticalScrollBar, false, false);
}



bool nuiScrollView::Load(const nuiXMLNode* pNode)
{
  nuiComposite::Load(pNode);

  bool Horizontal = nuiGetBool(pNode, _T("Horizontal"), true);
  bool Vertical =   nuiGetBool(pNode, _T("Vertical"), true);

  Init(NULL, NULL, Horizontal, Vertical);

  mFillChildren = nuiGetBool(pNode, _T("FillChildren"), mFillChildren);
  
  return true;
}

void nuiScrollView::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>(_T("EnableHorizontalScroll"), nuiUnitYesNo, nuiMakeDelegate(this, &nuiScrollView::GetEnableHorizontalScroll), nuiMakeDelegate(this, &nuiScrollView::SetEnableHorizontalScroll)));
  AddAttribute(new nuiAttribute<bool>(_T("EnableVerticalScroll"), nuiUnitYesNo, nuiMakeDelegate(this, &nuiScrollView::GetEnableHorizontalScroll), nuiMakeDelegate(this, &nuiScrollView::SetEnableHorizontalScroll)));
}

void nuiScrollView::Init(nuiScrollBar* pHorizontalScrollBar, nuiScrollBar* pVerticalScrollBar, bool Horizontal, bool Vertical)
{
  if (SetObjectClass(_T("nuiScrollView")))
    InitAttributes();

  mFillChildren = true;
  mVIncrement = 64.f;
  mHIncrement = 64.f;
  mCanRespectConstraint = true;

  mSmoothScrolling = true;
  mXOffset = 0;
  mYOffset = 0;
  mpSmoothTimer = nuiAnimation::AcquireTimer();
  mSVSink.Connect(mpSmoothTimer->Tick, &nuiScrollView::OnSmoothScrolling);
  mTimerOn = false;
  mBarSize = SCROLL_SIZE;

  mMinimalResize = false;

  mpHorizontal = NULL;
  mpVertical   = NULL;

  if (pHorizontalScrollBar)
  {
    mHorizontalIsExternal = true;
    mForceNoHorizontal    = false;
  }
  else
  {
    mHorizontalIsExternal = false;
    mForceNoHorizontal    = !Horizontal;
  }

  if (pVerticalScrollBar)
  {
    mVerticalIsExternal = true;
    mForceNoVertical    = false;
  }
  else
  {
    mVerticalIsExternal = false;
    mForceNoVertical    = !Vertical;
  }

  if (pHorizontalScrollBar)
  {
    mpHorizontal = pHorizontalScrollBar;
  }
  else
  {
    mpHorizontal = new nuiScrollBar(nuiHorizontal);
    AddChild(mpHorizontal);
  }

  if (pVerticalScrollBar)
  {
    mpVertical = pVerticalScrollBar;
  }
  else
  {
    mpVertical = new nuiScrollBar(nuiVertical);
    AddChild(mpVertical);
  }
  
  
  mpHorizontal->SetSerializeMode(eDontSaveNode);
  mpVertical->SetSerializeMode(eDontSaveNode);

  mHThumbPressed = false;
  mVThumbPressed = false;

  
  mSVSink.Connect(mpHorizontal->GetRange().ValueChanged, &nuiScrollView::Scrolled);
  mSVSink.Connect(mpVertical->GetRange().ValueChanged,   &nuiScrollView::Scrolled);

  mSVSink.Connect(mpHorizontal->ThumbPressed, &nuiScrollView::OnHThumbPressed);
  mSVSink.Connect(mpHorizontal->ThumbDepressed, &nuiScrollView::OnHThumbDepressed);
  mSVSink.Connect(mpVertical->ThumbPressed, &nuiScrollView::OnVThumbPressed);
  mSVSink.Connect(mpVertical->ThumbDepressed, &nuiScrollView::OnVThumbDepressed);
  
  mSVSink.Connect(ChildAdded, &nuiScrollView::OnChildAdded);
  mSVSink.Connect(ChildDeleted, &nuiScrollView::OnChildRemoved);
}

bool nuiScrollView::GetEnableHorizontalScroll() const
{
  return (mpHorizontal || !mForceNoHorizontal);
}

void nuiScrollView::SetEnableHorizontalScroll(bool set)
{
  mForceNoHorizontal = !set;
  //mpVertical->SetVisible(set);
  InvalidateLayout();
}

bool nuiScrollView::GetEnableVerticalScroll() const
{
  return (mpVertical || !mForceNoVertical);
}

void nuiScrollView::SetEnableVerticalScroll(bool set)
{
  mForceNoVertical = !set;
  //mpVertical->SetVisible(set);
  InvalidateLayout();
}

nuiScrollView::~nuiScrollView()
{
    nuiAnimation::ReleaseTimer();

  if (mHorizontalIsExternal && mpHorizontal)
  {
//    mSVSink.Disconnect(mpHorizontal->GetRange().ValueChanged);
  }
  if (mVerticalIsExternal && mpVertical)
  {
//    mSVSink.Disconnect(mpVertical->GetRange().ValueChanged);
  }
}



nuiRect nuiScrollView::CalcIdealSize()
{
  nuiRect rect;

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem != mpHorizontal && pItem != mpVertical)
    {
      pItem->SetLayoutConstraint(mConstraint);
      rect.Union(rect, pItem->GetIdealRect()); 
    }
    else if (pItem)
    {
      pItem->GetIdealRect(); // Dummy get ideal rect for the scroll bars
    }
  }
  delete pIt;

  rect.mLeft = 0;
  rect.mTop = 0;

  mChildrenUnionRect = rect;
  ///< take scrollbars in IdealRect calculation if necessary
  rect.SetSize(rect.GetWidth() + (mpHorizontal && !mHorizontalIsExternal && !mForceNoHorizontal ? mBarSize : 0),
               rect.GetHeight() + (mpVertical && !mVerticalIsExternal && !mForceNoVertical ? mBarSize : 0));
  mIdealRect = rect;

  if (GetDebug())
  {
    NGL_OUT(_T("nuiScrollView::CalcIdealSize: %ls\n"), mIdealRect.GetValue().GetChars());
  }
  
  return mIdealRect;
}

bool nuiScrollView::SetRect(const nuiRect& rRect)
{
  if (GetDebug())
  {
    NGL_OUT(_T("nuiScrollView::SetRect: %ls\n"), rRect.GetValue().GetChars());
  }
      
  nuiWidget::SetRect(rRect);

  if (!mpHorizontal || !mpVertical)
    return false;

  nuiSize XOffset = mpHorizontal->GetRange().GetValue();
  nuiSize YOffset = mpVertical->GetRange().GetValue();

  if (mSmoothScrolling)
  {
    if (!mHThumbPressed)
      XOffset = mXOffset;
    if (!mVThumbPressed)
      YOffset = mYOffset;
  }

  mXOffset = XOffset;
  mYOffset = YOffset;

  XOffset = (nuiSize)ToNearest(XOffset);
  YOffset = (nuiSize)ToNearest(YOffset);

  GetIdealRect();
  nuiRect rIdealRect(mChildrenUnionRect); ///< needed because GetIdealRect may return a UserRect
  
  if (mMinimalResize)
  {
    rIdealRect.Right() = MAX(rIdealRect.Right(), mOldIdealRect.Right());
    rIdealRect.Bottom() = MAX(rIdealRect.Bottom(), mOldIdealRect.Bottom());
    mOldIdealRect = rIdealRect;
  }

  nuiSize x = rIdealRect.GetWidth();
  nuiSize y = rIdealRect.GetHeight();
  
  nuiRange& hrange = mpHorizontal->GetRange();
  nuiRange& vrange = mpVertical->GetRange();

  if (mMinimalResize)
  {
    if( (hrange.GetValue() + hrange.GetPageSize()) < hrange.GetMaximum() )
    {
      x = mChildrenUnionRect.GetWidth();
      mOldIdealRect = mChildrenUnionRect;
    }
    if( (vrange.GetValue() + vrange.GetPageSize()) < vrange.GetMaximum() )
    {
      y = mChildrenUnionRect.GetHeight();
      mOldIdealRect = mChildrenUnionRect;
    }
  }

  nuiSize xx = rRect.GetWidth();
  nuiSize yy = rRect.GetHeight();

  bool needv = y > yy;
  bool needh = x > xx;

  needh = needh && !mForceNoHorizontal;
  needv = needv && !mForceNoVertical;

  nuiSize scrollh = (needh && !mHorizontalIsExternal) ? mBarSize:0;
  nuiSize scrollv = (needv && !mVerticalIsExternal)   ? mBarSize:0;

  for (int i = 0; i<2; i++)
  {
    if (needv && !mForceNoHorizontal)
    {
      needh = x > (xx - scrollv);
      if (!mVerticalIsExternal)
        scrollv = needv?mBarSize:0;
    }
    
    if (needh && !mForceNoVertical)
    {
      needv = y > (yy - scrollh);
      if (!mHorizontalIsExternal)
        scrollh = needh?mBarSize:0;
    }
  }

  if (needh)
  {
    hrange.SetRange(0,x);
    hrange.SetPageSize(MAX(0, xx - scrollv));
    hrange.SetPageIncrement(MAX(0, xx - scrollv));
    hrange.SetIncrement(mHIncrement);
    hrange.EnableEvents(false); 
    nuiSize oldv = hrange.GetValue();
    hrange.SetValue(hrange.GetValue()); 
    if (hrange.GetValue() != oldv)
    {
      mXOffset = hrange.GetValue();  
    }
    hrange.EnableEvents(true);
  }
  else
  {
    hrange.SetRange(0,x);
    hrange.SetPageSize(MAX(0, x));
    hrange.SetPageIncrement(MAX(0, x));
    hrange.SetIncrement(mHIncrement);
    mXOffset = XOffset= 0.f;  
    hrange.EnableEvents(false); 
    hrange.SetValue(XOffset); 
    hrange.EnableEvents(true);
  }
  
  if (needv)
  {
    vrange.SetRange(0,y);
    vrange.SetPageSize(MAX(0, yy - scrollh));
    vrange.SetPageIncrement(MAX(0, yy - scrollh));
    vrange.SetIncrement(mVIncrement);
    vrange.EnableEvents(false); 
    nuiSize oldv = vrange.GetValue();
    vrange.SetValue(vrange.GetValue()); 
    if (vrange.GetValue() != oldv)
    {
      mYOffset = vrange.GetValue();  
    }
    vrange.EnableEvents(true);
  }
  else
  {
    vrange.SetRange(0,y);
    vrange.SetPageSize(MAX(0, y));
    vrange.SetPageIncrement(MAX(0, y));
    vrange.SetIncrement(mVIncrement);
    mYOffset = YOffset = 0.f;
    vrange.EnableEvents(false); 
    vrange.SetValue(YOffset); 
    vrange.EnableEvents(true);
  }
  OffsetsChanged();

  SetChildrenRect(x, y, xx, yy, scrollv, scrollh);

  if (!mHorizontalIsExternal)
  {
    mpHorizontal->SetVisible(needh);
    mpHorizontal->SetLayout(nuiRect(0.0f, yy-scrollh, xx - scrollv, scrollh));
  }
  if (!mVerticalIsExternal)
  {
    mpVertical->SetVisible(needv);
    mpVertical->SetLayout(nuiRect(xx - scrollv, 0.0f, scrollv, yy-scrollh));
  }

  return true;
}


bool nuiScrollView::SetChildrenRect(nuiSize x, nuiSize y, nuiSize xx, nuiSize yy, nuiSize scrollv, nuiSize scrollh)
{  
  if (GetDebug())
  {
    NGL_OUT(_T("\tnuiScrollView::SetChildrenRect: (%f, %f) (%f, %f) - (%f, %f)\n"), x, y, xx, yy, scrollv, scrollh);
  }
  
  nuiRect VisibleRect = GetRect().Size();
  VisibleRect.Right() -= scrollv;
  VisibleRect.Bottom() -= scrollh;
  
  nuiRange& hrange = mpHorizontal->GetRange();
  nuiRange& vrange = mpVertical->GetRange();
  nuiSize XOffset = hrange.GetValue();
  nuiSize YOffset = vrange.GetValue();

  if (mSmoothScrolling)
  {
    if (mTimerOn)
    {
      if (!mHThumbPressed)
        XOffset = mXOffset;
      if (!mVThumbPressed)
        YOffset = mYOffset;
    }
  }

  XOffset = (nuiSize)ToNearest(XOffset);
  YOffset = (nuiSize)ToNearest(YOffset);

  if (!mFillChildren)
  {
    IteratorPtr pIt;  
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (pItem != mpVertical && pItem != mpHorizontal)
      {
        nuiRect rect = pItem->GetIdealRect();
        rect.Move(-XOffset, -YOffset);

        float w = rect.GetWidth();
        float h = rect.GetHeight();
        
        if (mForceNoHorizontal)
          w = xx;
        if (mForceNoVertical)
          h = yy;
        
        rect.SetSize(w, h);

        if (GetDebug())
        {
          NGL_OUT(_T("\tnuiScrollView::SetChildrenRect: SetLayout(%ls)\n"), rect.GetValue().GetChars());
        }

        pItem->SetLayout(rect);
        rect = VisibleRect;
        LocalToLocal(pItem, rect);
        pItem->SetVisibleRect(rect);
      }
    }
    delete pIt;
  }
  else
  {
    if (x < xx - scrollv)
      x = xx - scrollv;
    if (y < yy - scrollh)
      y = yy - scrollh;

    IteratorPtr pIt;  
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (pItem != mpVertical && pItem != mpHorizontal)
      {
        float w = x, h = y;

        if (mForceNoHorizontal)
				{
          w = xx - scrollv;
				}
        if (mForceNoVertical)
				{
          h = yy - scrollh;
				}
        
        nuiRect rect(-XOffset, -YOffset, w, h);
        
        if (GetDebug())
        {
          NGL_OUT(_T("\tnuiScrollView::SetChildrenRect: SetLayout(%ls)\n"), rect.GetValue().GetChars());
        }
        pItem->SetLayout(rect);
        rect = VisibleRect;
        LocalToLocal(pItem, rect);
        pItem->SetVisibleRect(rect);
      }
    }
    delete pIt;
  }

  return true;
}


bool nuiScrollView::Draw(nuiDrawContext* pContext)
{
  nuiSize scrollv = (mpVertical->IsVisible() && !mForceNoVertical && !mVerticalIsExternal)?mBarSize:0;
  nuiSize scrollh = (mpHorizontal->IsVisible() && !mForceNoHorizontal && !mHorizontalIsExternal)?mBarSize:0;
  nuiRect rect(0.0f,0.0f,mRect.GetWidth()-scrollv, mRect.GetHeight()-scrollh);

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiRect intersect;
    if (intersect.Intersect(rect, pItem->GetOverDrawRect(true, true)))
    {
      DrawChild(pContext, pItem);
    }
  }

  delete pIt;

  if (GetDebug())
  {
    pContext->SetStrokeColor(nuiColor("red"));
    pContext->DrawRect(GetRect().Size(), eStrokeShape);
    pContext->SetStrokeColor(nuiColor("blue"));
    pContext->DrawRect(GetIdealRect().Size(), eStrokeShape);
    pContext->SetStrokeColor("yellow");
    float h = GetRect().GetHeight() - scrollh - 1;
    pContext->DrawLine(0, h, GetRect().GetWidth(), h);
  }
  return true;
}

bool nuiScrollView::Scrolled(const nuiEvent& rEvent)
{
  if (mSmoothScrolling && !mHThumbPressed && !mVThumbPressed)
    mTimerOn = true;
  else
    UpdateLayout();
  Invalidate();
  return false;
}

bool nuiScrollView::Clear(bool Delete)
{
  int childCount = GetChildrenCount();
  for (childCount--; childCount >= 0; childCount--)
  {
    nuiWidget* pWidget = GetChild(childCount);
    if (pWidget && pWidget != mpHorizontal && pWidget != mpVertical)
    {
      DelChild(pWidget, Delete);
    }
  }
  mpChildren.clear();
  if (!mVerticalIsExternal)
    mpChildren.push_back(mpVertical);
  if (!mHorizontalIsExternal)
    mpChildren.push_back(mpHorizontal);
  InvalidateLayout();
  DebugRefreshInfo();
  return true;
}

bool nuiScrollView::OnChildAdded(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiWidget>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiWidget>*>(&rEvent);
  nuiWidgetPtr pNode = dynamic_cast<nuiWidgetPtr>(pTreeEvent->mpChild);

  if (pNode)
  {
    mSVSink.Connect(pNode->HotRectChanged, &nuiScrollView::OnHotRectChanged, pNode);
  }

  // The scroll bar must be the first to get mouse events, so let's put them on top of the list:
  if (!mHorizontalIsExternal && mpHorizontal)
  {
    RaiseChildToFront(mpHorizontal);
  }
  if (!mVerticalIsExternal && mpVertical)
  {
    RaiseChildToFront(mpVertical);
  }
  return false;
}

bool nuiScrollView::OnChildRemoved(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiWidget>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiWidget>*>(&rEvent);
  nuiWidgetPtr pNode = dynamic_cast<nuiWidgetPtr>(pTreeEvent->mpChild);

  if (pNode)
  {
    mSVSink.DisconnectSource(pNode->HotRectChanged);
  }

  return false;
}

bool nuiScrollView::OnHotRectChanged(const nuiEvent& rEvent)
{
  UpdateLayout();
  nuiWidgetPtr pChild = (nuiWidgetPtr)rEvent.mpUser;

  if (!pChild)
    return false;

  nuiRect rect;
  pChild->GetHotRect(rect);

  if (!mForceNoVertical)
  {
    mpVertical->GetRange().MakeInRange(rect.Top(), rect.GetHeight());
  }

  if (!mForceNoHorizontal)
  {
    mpHorizontal->GetRange().MakeInRange(rect.Left(), rect.GetWidth());
  }
  return false;
}

void nuiScrollView::SetFillChildren(bool Set) ///< If filling is enabled the scroll view will try to move its child in order to honor the available space
{
  mFillChildren = Set;
  Invalidate();
}

bool nuiScrollView::GetFillChildren()
{
  return mFillChildren;
}

bool nuiScrollView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool v = IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT);
  if (Button & nglMouseInfo::ButtonWheelUp)
  {
    if (v)
    {
      if (mpHorizontal && !mForceNoHorizontal)
      {
        mpHorizontal->GetRange().Decrement();
        return true;
      }
      else if (mpVertical && !mForceNoVertical)
      {
        mpVertical->GetRange().Decrement();
        return true;
      }
    }
    else
    {
      if (mpVertical && !mForceNoVertical)
      {
        mpVertical->GetRange().Decrement();
        return true;
      }
      else if (mpHorizontal && !mForceNoHorizontal && !mForceNoSmartScroll)
      {
        mpHorizontal->GetRange().Decrement();
        return true;
      }
    }

  }
  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    if (v)
    {
      if (mpHorizontal && !mForceNoHorizontal)
      {
        mpHorizontal->GetRange().Increment();
        return true;
      }
      else if (mpVertical && !mForceNoVertical)
      {
        mpVertical->GetRange().Increment();
        return true;
      }

    }
    else
    {
      if (mpVertical && !mForceNoVertical)
      {
        mpVertical->GetRange().Increment();
        return true;
      }
      else if (mpHorizontal && !mForceNoHorizontal && !mForceNoSmartScroll)
      {
        mpHorizontal->GetRange().Increment();
        return true;
      }
    }
  }
  else if (Button & nglMouseInfo::ButtonWheelLeft)
  {
    if (mpHorizontal && !mForceNoHorizontal)
    {
      mpHorizontal->GetRange().Decrement();
      return true;
    }
  }
  else if (Button & nglMouseInfo::ButtonWheelRight)
  {
    if (mpHorizontal && !mForceNoHorizontal)
    {
      mpHorizontal->GetRange().Increment();
      return true;
    }
  }
  
  return false;
}

nuiScrollBar* nuiScrollView::GetScrollBar(nuiOrientation Orientation)
{
  return Orientation == nuiHorizontal ? mpHorizontal : mpVertical;
}

nuiRange* nuiScrollView::GetRange(nuiOrientation Orientation)
{
  if (Orientation == nuiHorizontal)
  {
    if (mpHorizontal)
      return &mpHorizontal->GetRange();
  }
  else
  {
    if (mpVertical)
      return &mpVertical->GetRange();
  }
  return NULL;
}

void nuiScrollView::SetHIncrement(nuiSize hIncrement)
{
  mHIncrement = hIncrement;
  mpHorizontal->GetRange().SetIncrement(mHIncrement);
}
nuiSize nuiScrollView::GetHIncrement() const
{
  return mHIncrement;
}


void nuiScrollView::SetVIncrement(nuiSize vIncrement)
{
  mVIncrement = vIncrement;
  mpVertical->GetRange().SetIncrement(mVIncrement);
}
nuiSize nuiScrollView::GetVIncrement() const
{
  return mVIncrement;
}

bool nuiScrollView::OnSmoothScrolling(const nuiEvent& rEvent)
{
  if (!mTimerOn)
    return false;

  float XOffset = mpHorizontal->GetRange().GetValue();
  float YOffset = mpVertical->GetRange().GetValue();

  float xdiff = XOffset - mXOffset;
  float ydiff = YOffset - mYOffset;

  if (xdiff > 2 || xdiff < -2)
    mXOffset += xdiff * NUI_SMOOTH_SCROLL_RATIO;
  else
    mXOffset = XOffset;

  if (ydiff > 2 || ydiff < -2)
    mYOffset += ydiff * NUI_SMOOTH_SCROLL_RATIO;
  else
    mYOffset = YOffset;

  if (mXOffset == XOffset && mYOffset == YOffset)
  {
    mTimerOn = false;
  }

  UpdateLayout();
  OffsetsChanged();
  return false;
}

void nuiScrollView::EnableSmoothScrolling(bool set)
{
  mSmoothScrolling = set;
  mXOffset = mpHorizontal->GetRange().GetValue();
  mYOffset = mpVertical->GetRange().GetValue();
  OffsetsChanged();
}

bool nuiScrollView::IsSmoothScrollingEnabled() const
{
  return mSmoothScrolling;
}

void nuiScrollView::EnableMinimalResize(bool set)
{
  mMinimalResize = set;
}

bool nuiScrollView::IsMinimalResizeEnabled() const
{
  return mMinimalResize;
}

bool nuiScrollView::OnHThumbPressed(const nuiEvent& rEvent)
{
  mHThumbPressed = true;
  return false;
}

bool nuiScrollView::OnHThumbDepressed(const nuiEvent& rEvent)
{
  mHThumbPressed = false;
  return false;
}

bool nuiScrollView::OnVThumbPressed(const nuiEvent& rEvent)
{
  mVThumbPressed = true;
  return false;
}

bool nuiScrollView::OnVThumbDepressed(const nuiEvent& rEvent)
{
  mVThumbPressed = false;
  return false;
}

nuiSize nuiScrollView::GetXOffset() const
{
  return mXOffset;
}

nuiSize nuiScrollView::GetYOffset() const
{
  return mYOffset;
}

void nuiScrollView::SetForceNoSmartScroll(bool set)
{
  mForceNoSmartScroll = set;
}

bool nuiScrollView::GetForceNoSmartScroll(bool set) const
{
  return mForceNoSmartScroll;
}


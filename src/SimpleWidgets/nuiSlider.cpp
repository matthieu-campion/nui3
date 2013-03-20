/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */




#include "nui.h"

#define SLIDER_IDEAL_WIDTH 12
#define SLIDER_IDEAL_SIZE 100
#define HANDLE_SIZE SLIDER_IDEAL_WIDTH

nuiSize nuiSlider::mDefaultFineSensitivityRatio = 10;
nglKeyCode nuiSlider::mDefaultFineSensitivityKey = NK_LSHIFT;

nuiSlider::nuiSlider(nuiOrientation orientation, const nuiRange& rRange)
: nuiSimpleContainer(),
mRange(rRange),
mSliderSink(this)
{
  if (SetObjectClass(_T("nuiSlider")))
    InitAttributes();
  
  mOrientation = orientation;
  mClicked = false;
  mThumbClicked = false;
  mDrawBackground = true;
  mAutoAdjustHandle = true;
  mRange.SetPageSize(0);
  mInteractiveValueChanged = false;
  
  mFineSensitivityRatio = mDefaultFineSensitivityRatio;
  mFineSensitivityKey = mDefaultFineSensitivityKey;
  
  mSliderSink.Connect(mRange.Changed, &nuiSlider::DoInvalidate);
  mSliderSink.Connect(mRange.ValueChanged, &nuiSlider::DoInvalidate);
  
  mpHandle = NULL;
  mpBackground = NULL;
  SetHandleOffset(0);
  SetHandlePosMin(0);
  SetHandlePosMax(100);
  
  SetWantKeyboardFocus(true);
  
  NUI_ADD_EVENT(ValueChanged);
  NUI_ADD_EVENT(InteractiveValueChanged);
}

void nuiSlider::InitAttributes()
{
  AddAttribute(new nuiAttribute<float>(nglString(_T("HandleOffset")), nuiUnitPixels,
                                       nuiMakeDelegate(this, &nuiSlider::GetHandleOffset),
                                       nuiMakeDelegate(this, &nuiSlider::SetHandleOffset)));
  
  AddAttribute(new nuiAttribute<float>(nglString(_T("HandlePosMin")), nuiUnitPixels,
                                       nuiMakeDelegate(this, &nuiSlider::GetHandlePosMin),
                                       nuiMakeDelegate(this, &nuiSlider::SetHandlePosMin)));
  
  AddAttribute(new nuiAttribute<float>(nglString(_T("HandlePosMax")), nuiUnitPixels,
                                       nuiMakeDelegate(this, &nuiSlider::GetHandlePosMax),
                                       nuiMakeDelegate(this, &nuiSlider::SetHandlePosMax)));

  AddAttribute(new nuiAttribute<nuiOrientation>(nglString(_T("Orientation")), nuiUnitOrientation,
                                       nuiMakeDelegate(this, &nuiSlider::GetOrientation),
                                       nuiMakeDelegate(this, &nuiSlider::SetOrientation)));

  AddAttribute(new nuiAttribute<bool>(_T("AutoAdjustHandle"), nuiUnitBoolean,
    nuiMakeDelegate(this, &nuiSlider::GetAutoAdjustHandle),
    nuiMakeDelegate(this, &nuiSlider::SetAutoAdjustHandle)));
}

nuiSlider::~nuiSlider()
{
}

#define SHADESIZE 4 

bool nuiSlider::Draw(nuiDrawContext* pContext)
{
  //pContext->PushState();
  pContext->ResetState();
  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);
  
  if (mDrawBackground)
  {
    if (!mpBackground)
      pTheme->DrawSliderBackground(pContext,this);
    else
      DrawChild(pContext, mpBackground);
  }
  
  if (!mpHandle)
    pTheme->DrawSliderForeground(pContext,this);
  else
    DrawChild(pContext, mpHandle);
  
  //pContext->PopState();
  pTheme->Release();
  
  return true;
}

nuiRect nuiSlider::CalcIdealSize()
{
  nuiRect Rect;
  nuiRect BgRect = mpBackground ? mpBackground->GetIdealRect() : nuiRect();
  nuiRect HnRect = mpHandle ? mpHandle->GetIdealRect() : nuiRect();
  
  
  if (mOrientation == nuiHorizontal)
  {
    Rect.mRight  = mpBackground ? BgRect.GetWidth() : MAX(mRange.GetRange(),SLIDER_IDEAL_SIZE);
    Rect.mBottom = mpHandle ? HnRect.GetHeight()    : SLIDER_IDEAL_WIDTH;
  }
  else
  {
    Rect.mRight  = mpHandle ? HnRect.GetWidth() : SLIDER_IDEAL_WIDTH;
    Rect.mBottom = mpBackground ? BgRect.GetHeight()      : MAX(mRange.GetRange(),SLIDER_IDEAL_SIZE);
  }
  
  mIdealRect = Rect;
  
  return mIdealRect;
}


void nuiSlider::HookMouse()
{
  mClicked = true;
  mThumbClicked = true;
  mInteractiveValueChanged = true;
  Grab();
  Invalidate();
  mClickValue = mRange.GetValue();
  nglMouseInfo info;
  GetTopLevel()->GetMouseInfo(info);
  mClickX = info.X;
  mClickY = info.Y;
  GlobalToLocal(mClickX, mClickY);
}


// Keyboard events:
bool nuiSlider::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_DOWN || rEvent.mKey == NK_LEFT)
  {
    mRange.Decrement();
    return true;
  }
  if (rEvent.mKey == NK_UP || rEvent.mKey == NK_RIGHT)
  {
    mRange.Increment();
    return true;
  }
  if (rEvent.mKey == NK_PAGEDOWN)
  {
    mRange.PageIncrement();
    return true;
  }
  if (rEvent.mKey == NK_PAGEDOWN)
  {
    mRange.PageDecrement();
    return true;
  }
  if (rEvent.mKey == NK_HOME)
  {
    double v = mOrientation == nuiHorizontal ? mRange.GetMinimum() : mRange.GetMaximum();
    mRange.SetValue(v);
    return true;
  }
  if (rEvent.mKey == NK_END)
  {
    double v = mOrientation == nuiHorizontal ? mRange.GetMaximum() : mRange.GetMinimum();
    mRange.SetValue(v);
    return true;
  }
  
  
  return false;
}

bool nuiSlider::KeyUp(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_DOWN || rEvent.mKey == NK_LEFT || rEvent.mKey == NK_UP || rEvent.mKey == NK_RIGHT || rEvent.mKey == NK_PAGEDOWN || rEvent.mKey == NK_PAGEDOWN)
    return true;
  
  return false;
}

// Received Mouse events:
bool nuiSlider::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClickX = X;
  mClickY = Y;
  
  if (Button == nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    mThumbClicked = true;
    mInteractiveValueChanged = true;
    Grab();
    Invalidate();
    mClickValue = mRange.GetValue();
    
    return true;
  }
  else if (Button == nglMouseInfo::ButtonWheelUp)
  {
    mInteractiveValueChanged = false;
    
    if (IsKeyDown(mFineSensitivityKey))
    {
      mRange.SetValue(mRange.GetValue() + mRange.GetIncrement() / mFineSensitivityRatio);
    }
    else
    {
      mRange.Increment();
    }
    ActivateToolTip(this, true);
    return true;
  }
  else if (Button == nglMouseInfo::ButtonWheelDown)
  {
    mInteractiveValueChanged = false;
    
    if (IsKeyDown(mFineSensitivityKey))
    {
      mRange.SetValue(mRange.GetValue() - mRange.GetIncrement() / mFineSensitivityRatio);
    }
    else
    {
      mRange.Decrement();
    }
    ActivateToolTip(this, true);
    return true;
  }
  return false;
}            

bool nuiSlider::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if ((Button == nglMouseInfo::ButtonLeft) && mClicked)
  {
    mInteractiveValueChanged = false;
    ValueChanged();
    
    mClicked = false;
    
    mThumbClicked = false;
    Ungrab();
    
    Invalidate();
    return true;
  }
  return false;
}

bool nuiSlider::MouseMoved  (nuiSize X, nuiSize Y)
{
  if (mThumbClicked)
  {
    
    
    nuiSize x,y;
    nuiSize range = (nuiSize)fabs(mHandlePosMax - mHandlePosMin);
    x = X - mClickX;
    y = mClickY - Y;
    
    //NGL_OUT(_T("MouseMoved : %.2f %.2f     %.2f %.2f\n"), X, Y,x,y);
    
    
    nuiSize start = mClickValue;
    nuiSize movement = (mOrientation == nuiHorizontal) ? x : y;
    
    if (IsKeyDown(mFineSensitivityKey))
    {
      movement /= mFineSensitivityRatio;
    }
    
    start += (mRange.GetRange() - mRange.GetPageSize()) * (movement / range);
    
    mRange.SetValue(start);
    return true;
  }
  
  return false;
}

// Data management:
void nuiSlider::HandlePageDown(const nuiEvent& rEvent)
{
  mRange.PageDecrement();
}

void nuiSlider::HandlePageUp(const nuiEvent& rEvent)
{
  mRange.PageIncrement();
}

nuiRange& nuiSlider::GetRange()
{
  return mRange;
}

void nuiSlider::DoInvalidate(const nuiEvent& rEvent)
{
  //ValueChanged();
  InteractiveValueChanged();
  if (!mInteractiveValueChanged)
    ValueChanged();
  
  // mInteractiveValueChanged = true;
  
  UpdateLayout();
}

void nuiSlider::SetOrientation(nuiOrientation Orientation)
{
  if (mOrientation == Orientation)
    return;
  mOrientation = Orientation;
  UpdateLayout();
}

nuiOrientation nuiSlider::GetOrientation()
{
  return mOrientation;
}

bool nuiSlider::SetHandle(nuiWidget* pWidget)
{
  if (mpHandle)
    mpHandle->Trash();
  AddChild(pWidget);
  mpHandle = pWidget;
  AdjustHandle();
  return true;
}

bool nuiSlider::SetBackground(nuiWidget* pWidget)
{
  if (mpBackground)
    mpBackground->Trash();
  if (pWidget)
  {
    AddChild(pWidget);
    mpBackground = pWidget;
  }
  else
  {
    mpBackground = NULL;
  }
  AdjustHandle();
  return true;
}

void nuiSlider::DrawBackground(bool DrawBackground)
{
  mDrawBackground = DrawBackground;
}

nuiWidget* nuiSlider::GetHandle()
{
  return mpHandle;
}

nuiWidget* nuiSlider::GetBackground()
{
  return mpBackground;
}

void nuiSlider::SetHandleOffset(nuiSize Offset)
{
  mHandleOffset = Offset;
  Invalidate();
}

void nuiSlider::SetHandlePosMin(nuiSize Min)
{
  mHandlePosMin = Min;
  UpdateLayout();
}

void nuiSlider::SetHandlePosMax(nuiSize Max)
{
  mHandlePosMax = Max;
  UpdateLayout();
}


nuiSize nuiSlider::GetHandleOffset()
{
  return mHandleOffset;
}

nuiSize nuiSlider::GetHandlePosMin()
{
  return mHandlePosMin;
}

nuiSize nuiSlider::GetHandlePosMax()
{
  return mHandlePosMax;
}

static const nglString tmp(_T("HandleOffset"));

void nuiSlider::AdjustHandle()
{
  if (mAutoAdjustHandle)
  {
    nuiSize offset = 0;
    nuiSize offset2 = 0;
    if (mpHandle)
    {
      nuiRect r = mpHandle->GetIdealRect();
      nuiSize s = (mOrientation == nuiHorizontal) ? r.GetWidth(): r.GetHeight();
      offset = s/2;
      offset2 = s - offset;
      SetHandleOffset(offset);
    }
    else
    {
      offset = HANDLE_SIZE/2;
      offset2 = HANDLE_SIZE/2;
    }
    
    if (mpBackground)
    {
      nuiRect r = mpBackground->GetIdealRect();
      nuiSize s;
      
      s = (mOrientation == nuiHorizontal) ? r.GetWidth() : r.GetHeight();
      
      nuiSize Min = offset;
      nuiSize Max = s - offset2;
      
      mHandlePosMin = Min;
      mHandlePosMax = Max;
    }
    else
    {
      nuiRect r = GetRect();
      nuiSize s = (mOrientation == nuiHorizontal) ? r.GetWidth() : r.GetHeight();
      nuiSize Min = offset;
      nuiSize Max = s - offset2;
      
      mHandlePosMin = Min;
      mHandlePosMax = Max;
    }
  }
}

void nuiSlider::SetAutoAdjustHandle(bool Enable)
{
  mAutoAdjustHandle = Enable;
}

bool nuiSlider::GetAutoAdjustHandle()
{
  return mAutoAdjustHandle;
}

bool nuiSlider::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  AdjustHandle();
  
  if (mpHandle)
  {
    nuiRect rect = GetRect().Size();
    nuiOrientation Orientation = GetOrientation();
    float start;
    
    start  = mRange.ConvertToUnit(mRange.GetValue());
    
    nuiRect HandleRect = mpHandle->GetIdealRect().Size();
    
    nuiSize Range = mHandlePosMax - mHandlePosMin;
    nuiSize Pos = mHandlePosMin + start * Range;
    
    if (Orientation == nuiHorizontal)
      HandleRect.MoveTo(Pos - mHandleOffset, rect.Bottom()/2 - HandleRect.Bottom()/2);
    else
      HandleRect.MoveTo(rect.Right()/2 - HandleRect.Right()/2, rect.Bottom() - Pos - mHandleOffset);
    
    HandleRect.RoundToNearest();
    
    mpHandle->SetLayout(HandleRect);
  }
  
  if (mpBackground)
  {
    mpBackground->SetLayout(GetRect().Size());
  }
  
  return true;
}

void nuiSlider::SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio)
{ 
  mDefaultFineSensitivityRatio = DefaultFineSensitivityRatio;
}

void nuiSlider::SetFineSensitivityRatio(nuiSize FineSensitivityRatio) 
{ 
  mFineSensitivityRatio = FineSensitivityRatio; 
} 

void nuiSlider::SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey)
{
  mDefaultFineSensitivityKey = DefaultFineSensitivityKey;
}

void nuiSlider::SetFineSensitivityKey(nglKeyCode FineSensitivityKey)
{
  mFineSensitivityKey = FineSensitivityKey;
}


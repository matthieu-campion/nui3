/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define INACTIVE_SHADE_SIZE 8.f

nuiButton::nuiButton()
: nuiSimpleContainer(),
  mEventSink(this)
{
  if (SetObjectClass(_T("nuiButton")))
  {
    InitAttributes();
  }
  mClicked = false;
  mPressed = false;
  SetRedrawOnHover(true);
  mAutoRepeat = false;
  mRepeatDelay = 0.5;
  mRepeatMinDelay = 0.01;
  mpAutoRepeatTimer = NULL;
  mActivationOffset = 0;
  EnableInteractiveDecoration(true);
  
  SetBorders(mDefaultBorders);
  SetWantKeyboardFocus(true);
  SetFocusVisible(true);
  mpTask = NULL;
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(const nglString& rText)
  : nuiSimpleContainer(), mEventSink(this)
{
  if (SetObjectClass(_T("nuiButton")))
  {
    InitAttributes();
  }
  mClicked = false;
  mPressed = false;
  mAutoRepeat = false;
  mRepeatDelay = 0.5;
  mRepeatMinDelay = 0.01;
  mpAutoRepeatTimer = NULL;
  mActivationOffset = 0;
  nuiLabel* pLabel = new nuiLabel(rText);
  AddChild(pLabel);
  pLabel->SetPosition(nuiCenter);
  SetRedrawOnHover(true);
  EnableInteractiveDecoration(true);
  
  SetBorders(mDefaultBorders);
  SetWantKeyboardFocus(true);
  SetFocusVisible(true);
  mpTask = NULL;
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(const nglImage& rImage)
  : nuiSimpleContainer(), mEventSink(this)
{
  if (SetObjectClass(_T("nuiButton")))
  {
    InitAttributes();
  }
  mClicked = false;
  mPressed = false;
  mAutoRepeat = false;
  mRepeatDelay = 0.5;
  mRepeatMinDelay = 0.01;
  mpAutoRepeatTimer = NULL;
  mActivationOffset = 0;
  SetRedrawOnHover(true);
  EnableInteractiveDecoration(true);

  nuiImage* pImage = new nuiImage(rImage);
  AddChild(pImage);
  pImage->SetPosition(nuiCenter);
  
  SetBorders(mDefaultBorders);
  SetWantKeyboardFocus(true);
  SetFocusVisible(true);
  mpTask = NULL;
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(nuiDecoration* pDeco, bool AlreadyAcquired)
: nuiSimpleContainer(), mEventSink(this)
{
  if (SetObjectClass(_T("nuiButton")))
  {
    InitAttributes();
  }
  mClicked = false;
  mPressed = false;
  mAutoRepeat = false;
  mRepeatDelay = 0.5;
  mRepeatMinDelay = 0.01;
  mpAutoRepeatTimer = NULL;
  mActivationOffset = 0;
  SetRedrawOnHover(true);
  EnableInteractiveDecoration(true);
  
  SetDecoration(pDeco, eDecorationOverdraw, AlreadyAcquired);
  
  SetBorders(mDefaultBorders);
  SetWantKeyboardFocus(true);
  SetFocusVisible(true);
  mpTask = NULL;
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}


nuiButton::~nuiButton()
{
  if (mpAutoRepeatTimer)
  {
    nuiAnimation::ReleaseTimer();
    mpAutoRepeatTimer=NULL;
  }
  
  if (mpTask)
  {
    mpTask->Cancel();
    mpTask->Release();
    mpTask = NULL;
  }
}

void nuiButton::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Pressed")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiButton::IsPressed),
                nuiMakeDelegate(this, &nuiButton::SetPressed)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("AutoRepeat")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiButton::GetAutoRepeat),
                nuiMakeDelegate(this, &nuiButton::EnableAutoRepeat)));

  AddAttribute(new nuiAttribute<float>
               (nglString(_T("AutoRepeatDelay")), nuiUnitSeconds,
                nuiMakeDelegate(this, &nuiButton::GetAutoRepeatDelay),
                nuiMakeDelegate(this, &nuiButton::SetAutoRepeatDelay)));
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("AutoRepeatMinDelay")), nuiUnitSeconds,
                nuiMakeDelegate(this, &nuiButton::GetAutoRepeatMinDelay),
                nuiMakeDelegate(this, &nuiButton::SetAutoRepeatMinDelay)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("ActivationOffset")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiButton::GetActivationOffset),
                nuiMakeDelegate(this, &nuiButton::SetActivationOffset)));
}

bool nuiButton::Draw(nuiDrawContext* pContext)
{
  DrawChildren(pContext);    
  return true;
}



nuiRect nuiButton::CalcIdealSize()
{
  nuiDecoration* pDeco = GetDecoration();
    
  if (pDeco)
  {
    mIdealRect = pDeco->GetIdealClientRect(this);
  }
  
  nuiRect Rect;
  if (mpChildren.size())
  {
    mIdealRect = Rect = mpChildren.front()->GetIdealRect();

    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (pItem)
      {
        nuiRect tmp = Rect;
        Rect.Union(tmp,pItem->GetIdealRect().Size());
      }
    }
    delete pIt;
    mIdealRect = Rect;
  }
  else
    if (!pDeco)
      mIdealRect = mRect;

  if (pDeco)
    mIdealRect.Union(mIdealRect, pDeco->GetIdealClientRect(this));

  return mIdealRect;
}

bool nuiButton::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  nuiRect Rect = rRect.Size();

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      pItem->SetLayout(Rect);
  }
  delete pIt;
  return true;
}


// Activate:
void nuiButton::Activate()
{
  SetPressed(true);
  Activated();
  if (mpTask)
  {
    mpTask->Cancel();
    mpTask->Release();
    mpTask = NULL;
  }
  mpTask = nuiMakeTask(this, &nuiButton::SetPressed, false);
  mpTask->Acquire();
  nuiAnimation::RunOnAnimationTick(mpTask, 4);
}

// Keyboard events:
bool nuiButton::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
  {
    Activate();
    return true;
  }

  return false;
}

bool nuiButton::KeyUp(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
  {
    SetPressed(false);
    if (mpTask)
    {
      mpTask->Cancel();
      mpTask->Release();
      mpTask = NULL;
    }
    return true;
  }
  
  return false;
}

// Received Mouse events:
bool nuiButton::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (IsDisabled())
    return false;
  if (Button & nglMouseInfo::ButtonLeft)
  {
//    printf("clicked\n");
    mClicked = true;
    SetPressed(true);
    Grab();
    Invalidate();
    
    if (mAutoRepeat)
    {
      Activated();

      mLastTime = nglTime();
      mCurrentRepeatDelay = mRepeatDelay;
      mUntilRepeat = mCurrentRepeatDelay;

      if (!mpAutoRepeatTimer)
      {
        mpAutoRepeatTimer = nuiAnimation::AcquireTimer();
        mEventSink.Connect(mpAutoRepeatTimer->Tick, &nuiButton::OnAutoRepeat);
      }
    }
    return true;
  }
  return false;
}            

bool nuiButton::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
//  printf("unclicked\n");
  if ( (Button & nglMouseInfo::ButtonLeft) && mClicked)
  {
//    printf("was clicked\n");
    mClicked = false;
    Ungrab();
    SetPressed(false);
    Invalidate();
    if (!mAutoRepeat)
    {
      if (IsInsideFromSelf(X,Y, GetActivationOffset()))
      {
        //      printf("activated\n");
        Activated();
      }
      else
      {
        //      printf("unclicked inactive\n");
        ButtonDePressedInactive();
      }
      
    }
    return true;
  }
  return false;
}

bool nuiButton::MouseUngrabbed()
{
  nuiWidget::MouseUngrabbed();
  if (mClicked)
  {
    mClicked = false;
    ButtonDePressedInactive();
    SetPressed(false);
  }
  if (mpAutoRepeatTimer)
  {
    mEventSink.Disconnect(mpAutoRepeatTimer->Tick, &nuiButton::OnAutoRepeat);
    mpAutoRepeatTimer = NULL;
    nuiAnimation::ReleaseTimer();
  }

  return false;
}

bool nuiButton::MouseMoved(nuiSize X, nuiSize Y)
{
  if (mClicked)
  {
    if (IsInsideFromSelf(X,Y, GetActivationOffset()))
    {
      SetPressed(true);
    }
    else
    {
      SetPressed(false);
    }
    return true;
  }
  else
  {
    if (IsDisabled())
      return false;
  }
  return false;
}


bool nuiButton::IsPressed() const
{
  return mPressed;
}

void nuiButton::SetPressed(bool Pressed)
{
  if (mPressed != Pressed)
  {
    mPressed = Pressed;
    if (Pressed)
    {
      SetSelected(true);
      ButtonPressed();
    }
    else
    {    
      SetSelected(false);
      ButtonDePressed();
      if (mpTask)
      {
        mpTask->Release();
        mpTask->Cancel();
        mpTask = NULL;
      }
    }
    Invalidate();
  }
}

void nuiButton::EnableAutoRepeat(bool set)
{
  mAutoRepeat = set;
}
bool nuiButton::GetAutoRepeat() const
{
  return mAutoRepeat;
}
void nuiButton::SetAutoRepeatDelay(float Delay)
{
  mRepeatDelay = Delay;
}
float nuiButton::GetAutoRepeatDelay() const
{
  return mRepeatDelay;
}
void nuiButton::SetAutoRepeatMinDelay(float Delay)
{
  mRepeatMinDelay = Delay;
}
float nuiButton::GetAutoRepeatMinDelay() const
{
  return mRepeatMinDelay;
}

void nuiButton::OnAutoRepeat(const nuiEvent& rEvent)
{
  if (!mAutoRepeat)
    return;

  if (IsPressed())
  {
    nglTime now;
    mUntilRepeat -= (now.GetValue() - mLastTime.GetValue());
    mLastTime = now;
    if (mUntilRepeat <= 0)
    {
      mCurrentRepeatDelay = MAX(mRepeatMinDelay, mCurrentRepeatDelay-mCurrentRepeatDelay/4.0);
      mUntilRepeat = mCurrentRepeatDelay;
      Activated();
    }
  }
}

void nuiButton::SetActivationOffset(nuiSize Offset)
{
  mActivationOffset = Offset;
}

nuiSize nuiButton::GetActivationOffset() const
{
  return mActivationOffset;
}

void nuiButton::SetDefaultBorders(float pixels)
{
  mDefaultBorders = pixels;
}

float nuiButton::mDefaultBorders = 0;

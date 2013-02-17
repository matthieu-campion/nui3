/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiSize nuiToggleButton::mDefaultCheckSize = 14;

nuiToggleButton::nuiToggleButton()
: nuiButton()
{
  SetObjectClass(_T("nuiToggleButton"));
  mDisplayAsCheckBox = false;
  mDisplayAsFrameBox = false;
  mCheckSize = mDefaultCheckSize;
  mHasContents = false;
}

nuiToggleButton::nuiToggleButton(const nglString& rText)
: nuiButton(rText)
{
  SetObjectClass(_T("nuiToggleButton"));
  mDisplayAsCheckBox = false;
  mDisplayAsFrameBox = false;
  mCheckSize = mDefaultCheckSize;
  mHasContents = true;
}

nuiToggleButton::nuiToggleButton(nuiStateDecoration* pDecoration)
: nuiButton(pDecoration)
{
  SetObjectClass(_T("nuiToggleButton"));
  mDisplayAsCheckBox = false;
  mDisplayAsFrameBox = false;
  mCheckSize = mDefaultCheckSize;
  mHasContents = false;
}

nuiToggleButton::nuiToggleButton(const nglImage& rImage)
: nuiButton(rImage)
{
  SetObjectClass(_T("nuiToggleButton"));
  mDisplayAsCheckBox = false;
  mDisplayAsFrameBox = false;
  mCheckSize = mDefaultCheckSize;
  mHasContents = true;
}

nuiToggleButton::~nuiToggleButton()
{
}

void nuiToggleButton::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("CheckBox")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiToggleButton::GetDisplayAsCheckBox),
                nuiMakeDelegate(this, &nuiToggleButton::SetDisplayAsCheckBox)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("FrameBox")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiToggleButton::GetDisplayAsFrameBox),
                nuiMakeDelegate(this, &nuiToggleButton::SetDisplayAsFrameBox)));
}


nuiSize nuiToggleButton::GetDefaultCheckSize()
{
  return mDefaultCheckSize;
}

void nuiToggleButton::SetDefaultCheckSize(nuiSize CheckSize)
{
  mDefaultCheckSize = CheckSize;
}




// Rendering:
bool nuiToggleButton::Draw(nuiDrawContext* pContext)
{
  //NGL_OUT(_T("0x%x nuitoggleButton::Draw(%d)\n"), this, GetState(false));

  nuiDecoration* pDeco = GetDecoration();

//  if (mDisplayAsCheckBox)
//  {
//    pContext->ResetState();
//    nuiTheme* pTheme = GetTheme();
//    NGL_ASSERT(pTheme);
//    pTheme->DrawCheckBox(pContext, this);
//    pTheme->Release();
//
//    DrawChildren(pContext);
//  }
//  else 
    if (mDisplayAsFrameBox)
  {
    pContext->ResetState();

    if (!mPressed)
      SetStrokeColor(pContext, eNormalButtonBg);
    else
      SetStrokeColor(pContext, eSelectedButtonBg);
    
    nuiRect r(mRect.Size());
    pContext->DrawRect(r, eStrokeShape);
        
    DrawChildren(pContext);
  }
  else
  {
    nuiButton::Draw(pContext);
  }
  return true;
}


// Keyboard events:
bool nuiToggleButton::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
  {
    mWasPressed = IsPressed();
    SetPressed(!mWasPressed);
    if (IsPressed())
      Activated();
    else
      Deactivated();
    return true;
  }
  
  return false;
}

bool nuiToggleButton::KeyUp(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
    return true;
  
  return false;
}


// Received Mouse events:
bool nuiToggleButton::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (IsDisabled())
    return false;

  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    mWasPressed = IsPressed();
    SetPressed(!mWasPressed);
    Grab();
    Invalidate();
    return true;
  }
  return false;
}            

bool nuiToggleButton::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if ( (Button & nglMouseInfo::ButtonLeft) && mClicked)
  {
    mClicked = false;
    Ungrab();

    if (IsInsideFromSelf(X,Y, GetActivationOffset()))
    {
      SetPressed(!mWasPressed);
      if (IsPressed())
				Activated();
			else
				Deactivated();
    }
    else
    {
      SetPressed(mWasPressed);
    }

    Invalidate();
    return true;
  }
  return false;
}

bool nuiToggleButton::MouseMoved(nuiSize X, nuiSize Y)
{
  if (IsDisabled())
    return false;
  
  if (mClicked)
  {
    if (IsInsideFromSelf(X,Y, GetActivationOffset()))
    {
      SetPressed(!mWasPressed);
    }
    else
    {
      SetPressed(mWasPressed);
    }
    return true;
  }
  return false;
}

bool nuiToggleButton::MouseUngrabbed()
{
  nuiWidget::MouseUngrabbed();
  if (mClicked)
  {
    mClicked = false;
    SetPressed(mWasPressed);
  }
  return false;
}

void nuiToggleButton::SetDisplayAsCheckBox(bool set) 
{ 
  if (set)
  {
    mDisplayAsFrameBox = false;
  }
  
  mDisplayAsCheckBox = set;
  IteratorPtr pIt = GetFirstChild();
  nuiLabel* pItem = dynamic_cast<nuiLabel*>(pIt->GetWidget());
  
  if (pItem)
  {
    if (set)
    {
      pItem->SetPosition(nuiLeft);
    }
    else
    {
      pItem->SetPosition(nuiCenter);
    }
  }
  
  delete pIt;

  InvalidateLayout(); 
}

void nuiToggleButton::SetDisplayAsFrameBox(bool set) 
{ 
  if (set)
  {
    mDisplayAsCheckBox = false;
  }

  mDisplayAsFrameBox = set;
  
  IteratorPtr pIt = GetFirstChild();
  nuiLabel* pItem = dynamic_cast<nuiLabel*>(pIt->GetWidget());
  
  if (pItem)
  {
    if (mDisplayAsCheckBox)
    {
      pItem->SetPosition(nuiLeft);
    }
    else
    {
      pItem->SetPosition(nuiCenter);
    }
  }
  
  delete pIt;
  
  InvalidateLayout(); 
}

nuiRect nuiToggleButton::CalcIdealSize()
{
  if (mDisplayAsFrameBox)
  {
    nuiRect r(nuiSimpleContainer::CalcIdealSize());
    //r.Grow(2,2);
    return r.Size();
  }
  return nuiButton::CalcIdealSize();
}

bool nuiToggleButton::SetRect(const nuiRect& rRect)
{
  if (mDisplayAsFrameBox)
  {
    nuiWidget::SetRect(rRect);
    nuiRect rect(rRect.Size());
    //rect.Grow(-2,-2);

    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (mCanRespectConstraint)
        pItem->SetLayoutConstraint(mConstraint);
      pItem->GetIdealRect();
      pItem->SetLayout(rect);
    }
    delete pIt;
    return true;
  }
  return nuiButton::SetRect(rRect);
}


bool nuiToggleButton::HasContents() const
{
  return mHasContents;
}

bool nuiToggleButton::GetDisplayAsCheckBox() const
{
  return mDisplayAsCheckBox;
}

bool nuiToggleButton::GetDisplayAsFrameBox() const
{
  return mDisplayAsFrameBox;
}


/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiToggleButton.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"

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

bool nuiToggleButton::Load(const nuiXMLNode* pNode)
{
  nuiButton::Load(pNode);
  SetObjectClass(_T("nuiToggleButton"));
  mDisplayAsCheckBox = false;
  mDisplayAsFrameBox = false;
  mCheckSize = mDefaultCheckSize;
  mHasContents = true;
  
  return true;
}

nuiToggleButton::~nuiToggleButton()
{
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

  if (pDeco)
  {
    DrawChildren(pContext);
    return true;
  }
  
  if (mDisplayAsCheckBox)
  {
    pContext->ResetState();
    nuiTheme* pTheme = GetTheme();
    NGL_ASSERT(pTheme);
    pTheme->DrawCheckBox(pContext, this);
    pTheme->Release();

    DrawChildren(pContext);
  }
  else if (mDisplayAsFrameBox)
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

// Received Mouse events:
bool nuiToggleButton::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (IsDisabled())
    return false;

  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    SetSelected(true);
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

    SetPressed(!IsPressed());
    if (mRect.Size().IsInside(X,Y))
    {
      if (IsPressed())
				Activated();
			else
				Deactivated();
    }
    else
    {
      SetSelected(IsPressed());
    }

    Invalidate();
    return true;
  }
  return false;
}

bool nuiToggleButton::MouseUngrabbed()
{
  nuiWidget::MouseUngrabbed();
  return false;
}

void nuiToggleButton::SetDisplayAsCheckBox(bool set) 
{ 
  if (set)
  {
    mDisplayAsFrameBox = false;
  }
  
  mDisplayAsCheckBox = set;
  mDrawSelf = !set;
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
  
  mDrawSelf = !set;
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
  if (!mDisplayAsCheckBox)
  {
    if (mDisplayAsFrameBox)
    {
      nuiRect r(nuiSimpleContainer::CalcIdealSize());
      //r.Grow(2,2);
      return r.Size();
    }
    return nuiButton::CalcIdealSize();
  }
  else
  {
    nuiRect rect = nuiButton::CalcIdealSize();
    //#FIXME : check that. removed extra size from the checkbox
    // rect.SetSize(rect.Size().GetWidth() + GetCheckSize() + 1.f, MAX(rect.Size().GetHeight(),GetCheckSize()));
    rect.SetSize(GetCheckSize(), GetCheckSize());
    return rect;
  }
}

bool nuiToggleButton::SetRect(const nuiRect& rRect)
{
  if (mDisplayAsCheckBox)
  {
    nuiWidget::SetRect(rRect);
    nuiRect Rect = rRect.Size();

    Rect.Left() += mCheckSize;

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
  else
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
      
    }
    return nuiButton::SetRect(rRect);
  }
}


bool nuiToggleButton::HasContents() const
{
  return mHasContents;
}


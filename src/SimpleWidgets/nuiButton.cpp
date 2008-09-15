/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiButton.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiDecoration.h"
#include "nuiFrame.h"

#define INACTIVE_SHADE_SIZE 0.f

nuiButton::nuiButton()
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiButton"));
  mClicked = false;
  mPressed = false;
  mDrawSelf = true;
  mBorderSize = mDefaultBorderSize;
  mShadeSize = INACTIVE_SHADE_SIZE;
  SetRedrawOnHover(true);
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(const nglString& rText)
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiButton"));
  mClicked = false;
  mDrawSelf = true;
  mPressed = false;
  mBorderSize = mDefaultBorderSize;
  mShadeSize = INACTIVE_SHADE_SIZE;
  nuiLabel* pLabel = new nuiLabel(rText);
  AddChild(pLabel);
  pLabel->SetPosition(nuiCenter);
  SetRedrawOnHover(true);

  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(const nglImage& rImage)
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiButton"));
  mClicked = false;
  mDrawSelf = true;
  mPressed = false;
  mBorderSize = mDefaultBorderSize;
  mShadeSize = INACTIVE_SHADE_SIZE;
  SetRedrawOnHover(true);

  nuiImage* pImage = new nuiImage(rImage);
  AddChild(pImage);
  pImage->SetPosition(nuiCenter);

#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Clicked"].Bind(&mClicked,false,true);
  mProperties["DrawSelf"].Bind(&mDrawSelf,true,false);
  mProperties["BorderSize"].Bind(&mBorderSize,true, false);
#endif
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(nuiDecoration* pDeco)
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiButton"));
  mClicked = false;
  mDrawSelf = true;
  mPressed = false;
  mBorderSize = mDefaultBorderSize;
  mShadeSize = INACTIVE_SHADE_SIZE;
  SetRedrawOnHover(true);
  
  SetDecoration(pDeco);
  
#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Clicked"].Bind(&mClicked,false,true);
  mProperties["DrawSelf"].Bind(&mDrawSelf,true,false);
  mProperties["BorderSize"].Bind(&mBorderSize,true, false);
#endif
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}


bool nuiButton::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiButton"));
  mDrawSelf = true;
  mClicked = false;
  mPressed = false;
  mBorderSize = nuiGetVal ( pNode, _T("BorderSize"), mDefaultBorderSize);
  mShadeSize = nuiGetVal ( pNode, _T("ShadeSize"), INACTIVE_SHADE_SIZE);
  mDrawSelf = nuiGetBool ( pNode, _T("DrawSelf"), true);

#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Clicked"].Bind(&mClicked,false,true);
  mProperties["DrawSelf"].Bind(&mDrawSelf,true,false);
  mProperties["BorderSize"].Bind(&mBorderSize,true, false);
#endif
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
  
  return true;
}

nuiButton::~nuiButton()
{
}

bool nuiButton::Draw(nuiDrawContext* pContext)
{
  nuiDecoration* pDeco = GetDecoration();


  if (pDeco)
  {
    DrawChildren(pContext);    
  }
  else 
  {
    if (mDrawSelf)
    {
      pContext->ResetState();
      nuiTheme* pTheme = GetTheme();
      NGL_ASSERT(pTheme);
      pTheme->DrawButton(pContext,this);
      pTheme->Release();
    }
    
    DrawChildren(pContext);
  }
  return true;
}



nuiRect nuiButton::CalcIdealSize()
{
  nuiDecoration* pDeco = GetDecoration();
    
  if (pDeco)
  {
    mIdealRect = pDeco->GetIdealClientRect();
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
  {
  
    mIdealRect.Union(mIdealRect, pDeco->GetIdealClientRect());
  }
  else if (mDrawSelf)
  {
    mIdealRect.SetSize(mIdealRect.GetWidth()+ 2*(mBorderSize + mShadeSize),
    mIdealRect.GetHeight()+ 2*mBorderSize + mShadeSize);
  }


  return mIdealRect;
}

bool nuiButton::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  nuiRect Rect = rRect.Size();

  if (mDrawSelf && !GetDecoration())
  {
    if ( Rect.GetWidth() >= (int)( 2 * mBorderSize ) )
    {
      Rect.mLeft += (int)mBorderSize + mShadeSize;
      Rect.mRight -= (int)mBorderSize + mShadeSize;
    }

    if (Rect.GetHeight()>= (int)( 2 * mBorderSize ) )
    {
      Rect.mTop += (int)mBorderSize;
      Rect.mBottom -= (int)mBorderSize + mShadeSize;
    }
  }

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


// Received Mouse events:
bool nuiButton::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (IsDisabled())
    return false;
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    SetPressed(true);
    Grab();
    Invalidate();
    return true;
  }
  return false;
}            

bool nuiButton::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if ( (Button & nglMouseInfo::ButtonLeft) && mClicked)
  {
    mClicked = false;
    Ungrab();
    SetPressed(false);
    if (mRect.Size().IsInside(X,Y))
    {
      Activated();
    }
    else
    {
      ButtonDePressedInactive();
    }

    Invalidate();
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

  return false;
}

bool nuiButton::MouseMoved(nuiSize X, nuiSize Y)
{
  if (mClicked)
  {
    if (mRect.Size().IsInside(X,Y))
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

nuiSize nuiButton::GetBorderSize() const
{
  return mBorderSize;
}

void nuiButton::SetBorderSize(nuiSize BorderSize)
{
  if (mBorderSize == BorderSize)
    return;
  mBorderSize = BorderSize;
  Invalidate();
}

nuiSize nuiButton::mDefaultBorderSize = 1;

nuiSize nuiButton::GetDefaultBorderSize()
{
  return mDefaultBorderSize;
}

void nuiButton::SetDefaultBorderSize(nuiSize BorderSize)
{
  mDefaultBorderSize = BorderSize;
}

void nuiButton::SetDrawSelf(bool val)
{
  mDrawSelf = val;
  InvalidateLayout();
}

bool nuiButton::GetDrawSelf() const
{
  return mDrawSelf;
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
    }
    Invalidate();
  }
}

nuiSize nuiButton::GetShadeSize() const
{
  return mShadeSize;
}

void nuiButton::SetShadeSize(nuiSize ShadeSize)
{
  mShadeSize = ShadeSize;
}



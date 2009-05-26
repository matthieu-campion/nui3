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

#define INACTIVE_SHADE_SIZE 8.f

nuiButton::nuiButton()
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiButton"));
  mClicked = false;
  mPressed = false;
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
  mPressed = false;
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
  mPressed = false;
  SetRedrawOnHover(true);

  nuiImage* pImage = new nuiImage(rImage);
  AddChild(pImage);
  pImage->SetPosition(nuiCenter);

#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Clicked"].Bind(&mClicked,false,true);
#endif
  
  NUI_ADD_EVENT(ButtonPressed);
  NUI_ADD_EVENT(ButtonDePressed);
  NUI_ADD_EVENT(ButtonDePressedInactive);
  NUI_ADD_EVENT(Activated);
}

nuiButton::nuiButton(nuiDecoration* pDeco, bool AlreadyAcquired)
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiButton"));
  mClicked = false;
  mPressed = false;
  SetRedrawOnHover(true);
  
  SetDecoration(pDeco, eDecorationOverdraw, AlreadyAcquired);
  
#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Clicked"].Bind(&mClicked,false,true);
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
  mClicked = false;
  mPressed = false;

#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties["Clicked"].Bind(&mClicked,false,true);
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
  DrawChildren(pContext);    
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
    mIdealRect.Union(mIdealRect, pDeco->GetIdealClientRect());

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
    if (mRect.Size().IsInside(X,Y))
    {
//      printf("activated\n");
      Activated();
    }
    else
    {
//      printf("unclicked inactive\n");
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


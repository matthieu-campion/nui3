/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiNavigationBar::nuiNavigationBar()
: nuiHBox(3), mEventSink(this)
{
  SetObjectClass(_T("nuiNavigationBar"));
  
  nuiHBox::SetCellExpand(0, nuiExpandFixed);
  nuiHBox::SetCellExpand(1, nuiExpandShrinkAndGrow);
  nuiHBox::SetCellExpand(2, nuiExpandFixed);
  nuiHBox::SetExpand(nuiExpandShrinkAndGrow);
 
  SetPosition(nuiFillTop);
  
  mBarStyle = eBarStyleDefault;
  //mTintColor = nuiColor(0,0,0);
  mTranslucent = false;
}

// virtual 
nuiNavigationBar::~nuiNavigationBar()
{

}

bool nuiNavigationBar::IsVisible()
{
  return (mBarStyle != eBarStyleNone);
}

nuiRect nuiNavigationBar::CalcIdealSize()
{
  return nuiRect(320, 44);
}

void nuiNavigationBar::SetBarStyle(nuiNavigationBarStyle style)
{
  mBarStyle = style;
  nuiDefaultDecoration::NavigationBar(this);
}


//void nuiNavigationBar::SetTintColor(const nuiColor& rColor)
//{
//  mTintColor = rColor;
//}

void nuiNavigationBar::SetTranslucent(bool set)
{
  mTranslucent = set;
}

nuiNavigationBarStyle nuiNavigationBar::GetBarStyle() const
{
  return mBarStyle;
}


//const nuiColor& nuiNavigationBar::GetTintColor() const
//{
//  return mTintColor;
//}

bool nuiNavigationBar::GetTranslucent()
{
  return mTranslucent;
}



void nuiNavigationBar::SetTitle(const nglString& rTitle)
{
  nuiLabel* pLabel = new nuiLabel(rTitle);
  
  nuiFontRequest Request;
//  Request.SetName(_T("Helvetica"), 1.f);
  Request.SetGenericName(_T("sans-serif"), 1.f);
  Request.MustHaveSize(14, 1.f);
  Request.SetBold(true, 1.f);
  pLabel->SetFont(nuiFontManager::GetManager().GetFont(Request));
  pLabel->SetPosition(nuiCenter);
  pLabel->SetTextColor(nuiColor(255,255,255));
  SetTopNavigationItem(pLabel);
}

void nuiNavigationBar::SetBackNavigationItem(bool set)
{
  nuiNavigationButton* pButton = new nuiNavigationButton(_T("Back"));
  nuiDefaultDecoration::NavigationButton(pButton, mBarStyle, true /*leftyButton*/);  
  nuiHBox::SetCell(0, pButton);
}


void nuiNavigationBar::SetTopNavigationItem(nuiWidget* pWidget)
{
  //nuiDefaultDecoration::NavigationButton(pButton, mBarStyle);  
  nuiHBox::SetCell(1, pWidget);
}

void nuiNavigationBar::SetLeftNavigationItem(nuiNavigationButton* pButton)
{
  nuiDefaultDecoration::NavigationButton(pButton, mBarStyle);  
  nuiHBox::SetCell(0, pButton);
}

void nuiNavigationBar::SetRightNavigationItem(nuiNavigationButton* pButton)
{
  nuiDefaultDecoration::NavigationButton(pButton, mBarStyle);  
  nuiHBox::SetCell(2, pButton);
}


 


// virtual 
//LBDEBUG
//nuiRect nuiNavigationBar::CalcIdealSize()
//{
//  // plateform? orientation? 
//  return nuiRect(0, 0, 320, 44);
//}
//

// virtual 
bool nuiNavigationBar::Draw(nuiDrawContext* pContext)
{
  if (mBarStyle == eBarStyleNone)
    return true;
  return nuiHBox::Draw(pContext);
}
//  
//  pContext->EnableAntialiasing(false);
//  pContext->EnableBlending(true);
//  pContext->SetBlendFunc(nuiBlendTransp);
//  pContext->EnableTexturing(false);
//  
//  nuiGradient gradient;
//  nuiRect rect = GetRect();
//  rect.Set(rect.Left(), rect.Top(), rect.GetWidth(), rect.GetHeight() / 2.f);
//  gradient.AddStop(nuiColor(255,255,255,72), 0.f);
//  gradient.AddStop(nuiColor(255,255,255,16), 1.f);
//  pContext->DrawGradient(gradient, rect, 0, rect.Top(), 0, rect.Bottom());
//  
//  bool res = nuiSimpleContainer::Draw(pContext);
//  
//  return res;
//}
//

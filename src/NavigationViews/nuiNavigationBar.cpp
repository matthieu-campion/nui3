/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nuiNavigationBar.h"


nuiNavigationBar::nuiNavigationBar()
: nuiSimpleContainer(), mEventSink(this)
{
  SetObjectClass(_T("nuiNavigationBar"));
  
  mBarStyle = eBarStyleDefault;
  mTintColor = nuiColor(0,0,0);
  mTranslucent = false;
}

// virtual 
nuiNavigationBar::~nuiNavigationBar()
{

}



void nuiNavigationBar::SetBarStyle(nuiNavigationBarStyle style)
{
  mBarStyle = style;
}


void nuiNavigationBar::SetTintColor(const nuiColor& rColor)
{
  mTintColor = rColor;
}

void nuiNavigationBar::SetTranslucent(bool set)
{
  mTranslucent = set;
}
  
void nuiNavigationBar::SetBackNavigationItem(nuiNavigationButton* pButton)
{

}


void nuiNavigationBar::SetTopNavigationItem(nuiNavigationButton* pButton)
{

}


void nuiNavigationBar::SetLeftNavigationItem(nuiNavigationButton* pButton)
{

}

void nuiNavigationBar::SetRightNavigationItem(nuiNavigationButton* pButton)
{

}
  


// virtual 
//nuiRect nuiNavigationBar::CalcIdealSize()
//{
//
//}


// virtual 
bool nuiNavigationBar::Draw(nuiDrawContext* pContext)
{

}


/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nuiNavigationBar.h"


nuiNavigationBar::nuiNavigationBar()
{

}

// virtual 
nuiNavigationBar::~nuiNavigationBar()
{

}


void nuiNavigationBar::SetBarStyle(nuiNavigationBarStyle style)
{

}


void nuiNavigationBar::SetTintColor(const nuiColor& rColor);
void nuiNavigationBar::SetTranslucent(bool set);
  
void nuiNavigationBar::SetBackNavigationItem(nuiNavigationButton* pButton = NULL);
void nuiNavigationBar::SetTopNavigationItem(nuiNavigationButton* pButton);
void nuiNavigationBar::SetLeftNavigationItem(nuiNavigationButton* pButton);
  void SetRightNavigationItem(nuiNavigationButton* pButton);
  
protected:
  
  
private:
  
  nuiNavigationBarStyle mBarStyle;
  nuiColor mTintColor;
  bool mTranslucent;
  
  nuiEventSink<nuiNavigationBar> mEventSink;

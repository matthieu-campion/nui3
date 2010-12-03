/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#pragma once
#include "nui.h"
#include "nuiHBox.h"



enum nuiNavigationBarStyle
{
  eBarStyleDefault = 0,
  eBarStyleBlack   = 1
};


class nuiNavigationBar : public nuiSimpleContainer
{
public:
  nuiNavigationBar();
  virtual ~nuiNavigationBar();

  void SetBarStyle(nuiNavigationBarStyle style);
  void SetTintColor(const nuiColor& rColor);
  void SetTranslucent(bool set);

  void SetBackNavigationItem(nuiNavigationButton* pButton);
  void SetTopNavigationItem(nuiNavigationButton* pButton);
  void SetLeftNavigationItem(nuiNavigationButton* pButton);
  void SetRightNavigationItem(nuiNavigationButton* pButton);
    
protected:
  
  
private:

  nuiNavigationBarStyle mBarStyle;
  nuiColor mTintColor;
  bool mTranslucent;
  
  nuiEventSink<nuiNavigationBar> mEventSink;
};
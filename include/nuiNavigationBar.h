/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#pragma once
#include "nui.h"
#include "nuiHBox.h"
#include "nuiNavigationBarStyle.h"


class nuiNavigationButton;


class nuiNavigationBar : public nuiHBox
{
public:
  nuiNavigationBar();
  virtual ~nuiNavigationBar();

  void SetBarStyle(nuiNavigationBarStyle style);
  //void SetTintColor(const nuiColor& rColor);
  void SetTranslucent(bool set);
  
  bool IsVisible();

  nuiNavigationBarStyle GetBarStyle() const;
  //const nuiColor& GetTintColor() const;
  bool GetTranslucent();

  void SetTitle(const nglString& rTitle);
  void SetBackNavigationItem(bool set);
  void SetTopNavigationItem(nuiWidget* pWidget);
  void SetLeftNavigationItem(nuiNavigationButton* pButton);
  void SetRightNavigationItem(nuiNavigationButton* pButton);
    
protected:
  
  virtual nuiRect CalcIdealSize();
  virtual bool Draw(nuiDrawContext* pContext);
  
private:
  

  nuiNavigationBarStyle mBarStyle;
  //nuiColor mTintColor;
  bool mTranslucent;
  
  nuiEventSink<nuiNavigationBar> mEventSink;
};
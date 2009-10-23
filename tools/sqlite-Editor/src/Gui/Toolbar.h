/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHBox.h"

class nuiButton;

#define BUTTON_OPEN _T("open")
#define BUTTON_REFRESH _T("refresh")

/// implements a simple box for buttons
class Toolbar : public nuiHBox
{
public:
  
  Toolbar();
  virtual ~Toolbar();
  
  /// add buttons
  void AddButton(const nglString& rObjectClass);   
  void AddToggleButton(const nglString& rObjectClass, bool initialPressed = false);   
  
  void Clear();  
  
  nuiSimpleEventSource<0> Activated;  
  const nglString& GetActivatedButton() const; 
  
private:
  
  bool OnActivated(const nuiEvent& rEvent);
  
  nglString mActivatedButton;
  std::vector<std::pair<nglString, nuiButton*> > mButtons;
  
  nuiEventSink<Toolbar> mEventSink;
};

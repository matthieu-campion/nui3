/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiVBox.h"
#include "nuiHBox.h"

#include "guiTheme.h"

class Gui : public nuiVBox
{
public:
  Gui();
  virtual ~Gui();

protected:
  
  nuiWidget* BuildControls();
  
  void ShowControls(bool show);
  void ShowText(bool show);
  
  // gui events receivers
  void OnStartButtonPressed(const nuiEvent& rEvent);
  void OnStartButtonDePressed(const nuiEvent& rEvent);

  void OnSwitchChanged(const nuiEvent& rEvent);
  void OnVolumeSliderChanged(const nuiEvent& rEvent);
  void OnFreqKnobChanged(const nuiEvent& rEvent);
  void OnQKnobChanged(const nuiEvent& rEvent);
  
  void OnShowText(const nuiEvent& rEvent);
  void OnShowControls(const nuiEvent& rEvent);
  
  void OnTitleButtonActivated(const nuiEvent& rEvent);
  
private:

  nuiSimpleContainer* mpPaneBkg;
  nuiVBox* mpTextPanel;
  nuiWidget* mpControls;
  float mAlphaControls;
  float mAlphaText;
  bool mShownControls;
  bool mShownText;

  nuiTimer mTimer;
//  uint32 mTextPaneWidth;
//  uint32 mControlsPaneWidth;
  
  
  // a sink to connect the event sources to the local event receivers.
  nuiEventSink<Gui> mEventSink;
  // the nuiEventSink HAS TO be connected to the class instance (here, Gui instance) in order to run properly.
  //
  // Forgetting to set ups that connection happened all the time and each time, it was a pain in the ass to understand the bug.
  // => now, the nuiEventSink is templated: it makes the sink connection be mandatory in the class constructor, 
  // otherwise the compiler complains and you can understand right away what it complains for.

  // another sink, for timer animation (could use mEventSink instead, it's just for coding convenience)
  nuiEventSink<Gui> mTimerSink;
  
  guiTheme mTheme; // for overloading some of the widgets drawing (sliders in our case)
};


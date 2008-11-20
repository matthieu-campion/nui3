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
  
  // gui events receivers
  bool OnStartButtonPressed(const nuiEvent& rEvent);
  bool OnStartButtonDePressed(const nuiEvent& rEvent);

  bool OnSwitchChanged(const nuiEvent& rEvent);
  bool OnQSliderChanged(const nuiEvent& rEvent);
  bool OnFreqKnobChanged(const nuiEvent& rEvent);
  bool OnQKnobChanged(const nuiEvent& rEvent);
  
  bool OnShowText(const nuiEvent& rEvent);
  bool OnShowControls(const nuiEvent& rEvent);
  
private:

  nuiLabel* mpText;
  nuiWidget* mpControls;
  nuiTimer mTimer;
  uint32 mTextPaneWidth;
  uint32 mControlsPaneWidth;
  
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


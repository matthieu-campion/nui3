/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"

class Generator : public nuiSimpleContainer
{
public:
  
  Generator();
  virtual ~Generator();
  
private:
  bool OnBrowseSourceRequest(const nuiEvent& rEvent);
  bool OnBrowseToolRequest(const nuiEvent& rEvent);

  bool OnBrowseSource(const nuiEvent& rEvent);
  bool OnBrowseTool(const nuiEvent& rEvent);

  bool OnSourceSelected(const nuiEvent& rEvent);
  bool OnToolSelected(const nuiEvent& rEvent);

  bool OnStart(const nuiEvent& rEvent);
  

  nuiLabel* mpToolLabel;
  nuiLabel* mpSourceLabel;
  
  nuiTimer mTimer;
  nuiEventSink<Generator> mEventSink;
  nuiEventSink<Generator> mTimerSink;
};
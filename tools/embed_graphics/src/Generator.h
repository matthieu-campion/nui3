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
  void OnBrowseSourceRequest(const nuiEvent& rEvent);
  void OnBrowseToolRequest(const nuiEvent& rEvent);

  void OnBrowseSource(const nuiEvent& rEvent);
  void OnBrowseTool(const nuiEvent& rEvent);

  void OnSourceSelected(const nuiEvent& rEvent);
  void OnToolSelected(const nuiEvent& rEvent);

  void OnStart(const nuiEvent& rEvent);
  void ParsePngFiles(const nglPath& rootSource, const nglPath& pngSource, const nglPath& codeSource);
  void DumpIncluder(const nglPath& rootSource, const nglPath& pngSource,const nglPath& codeSource, const nglPath& HincluderPath, const nglPath& CPPincluderPath, nglString& HincluderStr, nglString& CPPincluderStr);
  

  nuiLabel* mpToolLabel;
  nuiLabel* mpSourceLabel;
  
  nglPath mTool;
  
  nuiTimer mTimer;
  nuiEventSink<Generator> mEventSink;
  nuiEventSink<Generator> mTimerSink;
};
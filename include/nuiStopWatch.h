/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiStopWatch
{
public:
  nuiStopWatch(const nglString& LogTitle);
  virtual ~nuiStopWatch();
  
  void AddIntermediate(const nglString& title);
  
  double GetElapsedTime() const;
  
private:
  nglTime mCreationTime;
  nglString mLogTitle;
  
  std::list<std::pair<nglString, nglTime> > mIntermediatePoints;
};
/*
 *  nuiStopWatch.h
 *  nui3
 *
 *  Created by mat on 9/8/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
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
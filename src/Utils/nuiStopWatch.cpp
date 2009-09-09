/*
 *  nuiStopWatch.cpp
 *  nui3
 *
 *  Created by mat on 9/8/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nuiStopWatch.h"

nuiStopWatch::nuiStopWatch(const nglString& LogTitle)
: mLogTitle(LogTitle)
{
  
}

nuiStopWatch::~nuiStopWatch()
{
  {
    nglTime DestructionTime;
    nglTime Diff = DestructionTime - mCreationTime;
    double seconds = Diff.GetValue();
    
    nglString TimeStr;
    TimeStr.Format(" %lf sec\n", seconds);
    
    nglString log = mLogTitle + TimeStr;
    NGL_OUT(log);
  }
  
  nglTime ref = mCreationTime;
  
  while (mIntermediatePoints.size())
  {
    std::pair<nglString, nglTime> point = mIntermediatePoints.front();
    nglString str = point.first;
    nglTime time = point.second;
    nglTime diff = time - ref;
    double sec = diff.GetValue();
    
    nglString log(_T("\t"));
    log.Add(str).Add(_T(" ")).Add(sec).Add(_T("\n"));
    NGL_OUT(log);
    
    ref = time;
    mIntermediatePoints.pop_front();
  }
}

void nuiStopWatch::AddIntermediate(const nglString& title)
{
  nglTime now;
  std::pair<nglString, nglTime> point = std::make_pair(title, now);
  mIntermediatePoints.push_back(point);
}

double nuiStopWatch::GetElapsedTime() const
{
  nglTime now;
  nglTime diff = now - mCreationTime;
  double elapsed = diff.GetValue();
  return elapsed;
}
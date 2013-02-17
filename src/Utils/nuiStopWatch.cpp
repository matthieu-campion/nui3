/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiStopWatch::nuiStopWatch(const nglString& LogTitle)
: mLogTitle(LogTitle)
{
  
}

nuiStopWatch::~nuiStopWatch()
{
  double seconds = 0.0f;
  {
    nglTime DestructionTime;
    nglTime Diff = DestructionTime - mCreationTime;
    seconds = Diff.GetValue();
    
    NGL_LOG(_T("StopWatch"), NGL_LOG_DEBUG, _T("%s\t\t%lf sec (%f Hz)\n"), mLogTitle.GetChars(), seconds, 1.0f / seconds);
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
    log.Add(str).Add(_T("\t")).Add(sec).Add(_T(" s\t(")).Add(100.0 * sec / seconds).Add(_T("%%)"));
    NGL_LOG(_T("StopWatch"), NGL_LOG_DEBUG, log.GetChars());
    
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
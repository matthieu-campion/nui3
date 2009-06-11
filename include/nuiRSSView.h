/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHyperLink.h"
#include "nuiSimpleContainer.h"
#include "nuiRSS.h"

class nuiVBox;

class nuiRSSView : public nuiSimpleContainer
{
public:
  nuiRSSView(const nglString& rURL, int32 SecondsBetweenUpdates = (30*60), nglIStream* pOriginalStream = NULL);
  virtual ~nuiRSSView();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  
  void ForceUpdate();
protected:
  bool Update(const nuiEvent& rEvent);
  nuiEventSink<nuiRSSView> mViewSink;
  nuiRSS* mpRSS;
  nuiVBox* mpBox;
};
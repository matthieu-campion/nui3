/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"

class nuiGrid;
class nuiTreeView;
class nuiHTML;
class nuiScrollView;
class nuiSplitter;

class HTMLTree : public nuiSimpleContainer
{
public:
  HTMLTree();
  ~HTMLTree();
  
  bool Parse(const nglString& rString);
  bool ParseURL(const nglString& rString);
  
protected:
  nuiEventSink<HTMLTree> mSink;
  nuiHTML* mpHTML;
  nglString mHTML;

  nuiGrid* mpAttributes;
  nuiTreeView* mpTreeView;
  nuiSplitter* mpSplitter;
  nuiScrollView* mpScroll1;
  nuiScrollView* mpScroll2;
  
  bool OnSelectionChanged(const nuiEvent& rEvent);
};


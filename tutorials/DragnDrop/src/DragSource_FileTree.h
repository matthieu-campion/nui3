/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
 

#include "nui.h"
#include "nuiVBox.h"
#include "nuiScrollView.h"
#include "nuiFileTree.h"

class DragFileTree : public nuiSimpleContainer
{
public:
  
  DragFileTree();
  virtual ~DragFileTree();
  
private:
  
  void OnFileActivated(const nuiEvent& rEvent);
  
  nuiScrollView* mpView;
  nuiEventSink<DragFileTree> mEventSink;
  
};
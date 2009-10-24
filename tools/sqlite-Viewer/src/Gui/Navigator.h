/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiVBox.h"

/// implements a list to display the tables of the current database
class Navigator: public nuiSimpleContainer
{
public:
  Navigator();
  virtual ~Navigator();
  
  /// refresh the list of tables
  void Update();
  
protected:
  
  /// when a table item has been selected
  bool OnItemSelected(const nuiEvent& rEvent);
  
private:
  
  bool SelectItem(nuiButton* pButtonItem);
  
  nuiVBox* mpBox;
  nglString mSelectedItem;
  nuiEventSink<Navigator> mEventSink;

};

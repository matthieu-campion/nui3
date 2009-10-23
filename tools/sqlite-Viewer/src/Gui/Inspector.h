/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiVBox.h"


/// allows to display the results of sql requests
class Inspector: public nuiSimpleContainer
{
public:
  Inspector();
  virtual ~Inspector();

  /// display all results from the given sql table (<=> Select *)
  void Update(const nglString& rTable);
  
};

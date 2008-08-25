/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiFolderPane.h"
#include "nuiVBox.h"

class ToolPane : public nuiFolderPane
{
public:
	ToolPane(const nglString& rTitle, nuiWidget* pWidget=NULL);
	ToolPane(nuiLabelAttribute* rLabel, nuiWidget* pWidget=NULL);
	virtual ~ToolPane();

  virtual void SetContents(nuiWidget* pWidget, nuiPosition position=nuiFill);
  
};



/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#pragma once

#include "nui.h"
#include "ToolPane.h"
#include "nuiVBox.h"

class ToolPaneAttributes : public ToolPane
{
public:

	ToolPaneAttributes(const nglString& rTitle, const std::list<nuiAttribBase>& rAttributes, nuiButton* pBtn=NULL);
	ToolPaneAttributes(nuiLabelAttribute* rLabel, const std::list<nuiAttribBase>& rAttributes, nuiButton* pBtn=NULL);
	virtual ~ToolPaneAttributes();
  


private:

  void Init(const std::list<nuiAttribBase>& rAttributes, nuiButton* pBtn=NULL);
	void AddAttributes(const std::list<nuiAttribBase>& rAttributes);

	nuiVBox* mpVBox;

};


